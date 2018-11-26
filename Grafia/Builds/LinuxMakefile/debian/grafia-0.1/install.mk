#this makefile installs the software on the local machine
prefix = /usr

.PHONY: install unsinstall
install: all
	install build/Grafia $(prefix)/bin/
	install Grafia_512logo.png $(prefix)/share/icons/hicolor/512x512/apps/
	install Grafia_512logo2.png $(prefix)/share/icons/hicolor/512x512/mimetypes/application-x-Grafia.png

uninstall: all
	rm $(prefix)/bin/
	rm $(prefix)/share/icons/hicolor/512x512/apps/
	rm $(prefix)/share/icons/hicolor/512x512/mimetypes/application-x-Grafia.png

