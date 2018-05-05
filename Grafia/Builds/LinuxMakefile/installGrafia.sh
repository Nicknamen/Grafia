#!/bin/bash

## Installer for Grafia

read -p "Install Grafia? [Y/n]: " inst
if [ $inst = y ] || [ $inst = Y ]
then
	cp -rf ./build/Grafia /usr/bin/
	cp -rf ./Grafia.desktop /usr/share/applications/
	cp -rf ./../../logo/Grafia_512logo.png /usr/share/icons/hicolor/512x512/apps/
fi
