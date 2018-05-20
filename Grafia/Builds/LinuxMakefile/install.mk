#this makefile installs the software on the local machine
prefix = /usr

.PHONY: install
install: all
	cp -rf build/Grafia $(prefix)/bin/
	cp -rf Grafia_512logo.png $(prefix)/share/icons/hicolor/512x512/apps/
	cp -rf Grafia_512logo2.png $(prefix)/share/icons/hicolor/512x512/mimetypes/application-x-Grafia.png
.PHONY: uninstall

