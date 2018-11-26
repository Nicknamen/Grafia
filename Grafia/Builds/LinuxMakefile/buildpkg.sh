checkinstall \
	--requires="libgraphicsmagick-q16-3,libgraphicsmagick++-q16-12,libstdc++6" \
	--pkgsource="https://github.com/Nicknamen/Grafia" \
	--pkglicense="GPL3" \
	--pkgversion="0.1" \
	--pkgname="grafia" \
	--maintainer="nico@cavalleri.net" \
	--provides="Grafia" \
	--pkgarch="$(dpkg --print-architecture)" \
	--install=no \
