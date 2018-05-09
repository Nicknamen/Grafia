#!/bin/bash

## Installer for Grafia

read -p "Uninstall Grafia? [Y/n]: " inst
if [ $inst = y ] || [ $inst = Y ]
then
	rm /usr/bin/Grafia
        rm /usr/share/icons/hicolor/512x512/apps/Grafia_512logo.png
        rm /usr/share/icons/hicolor/512x512/mimetypes/application-x-Grafia.png
	rm /usr/share/applications/Grafia.desktop
	rm /usr/share/mime/packages/Grafia.xml
	update-mime-database /usr/share/mime
	echo "Uninstalled"
fi
