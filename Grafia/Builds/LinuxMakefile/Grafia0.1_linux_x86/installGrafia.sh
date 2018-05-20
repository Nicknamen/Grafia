#!/bin/bash

## Installer for Grafia

read -p "Install Grafia? [Y/n]: " inst
if [ $inst = y ] || [ $inst = Y ]
then
	cp -rf Grafia /usr/bin/
	cp -rf Grafia_512logo.png /usr/share/icons/hicolor/512x512/apps/
    cp -rf Grafia_512logo2.png /usr/share/icons/hicolor/512x512/mimetypes/application-x-Grafia.png

    ## Create a Desktop launcher
    touch /usr/share/applications/Grafia.desktop
    echo "[Desktop Entry]
Type=Application
Version=0.1
Name=Grafia
Exec=Grafia %F
MimeType=application/x-Grafia
Icon=Grafia_512logo.png
Comment=Create mathematical symbols in LaTex
Categories=Utility;
Terminal=false" | tee /usr/share/applications/Grafia.desktop
    chmod a+x /usr/share/applications/Grafia.desktop

    ## Add the MIME type to the system, so .grproj files will automatically open with Grafia
    touch /usr/share/mime/packages/Grafia.xml
    echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
    <mime-info xmlns=\"http://www.freedesktop.org/standards/shared-mime-info\">
        <mime-type type=\"application/x-Grafia\">
            <comment>Grafia project</comment>
	    <sub-class-of type=\"application/xml\"/>
	    <icon name=\"application-x-Grafia.png\"/>
            <glob pattern=\"*.grproj\"/>
        </mime-type>
    </mime-info>" | tee /usr/share/mime/packages/Grafia.xml
    update-mime-database /usr/share/mime
    echo "application/x-Grafia=Grafia.desktop" | sudo tee --append /usr/share/applications/defaults.list
    echo "Installed"
fi
