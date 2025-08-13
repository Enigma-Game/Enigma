#!/bin/sh

#make distclean
export CC="i686-w64-mingw32.static-gcc"
export CXX="i686-w64-mingw32.static-g++"
export CXXFLAGS="-g0 -DCURL_STATICLIB"
export SDL_CFLAGS="-D_REENTRANT -I/opt/mxe/usr/i686-w64-mingw32.static/include/SDL2"
export RANLIB=i686-w64-mingw32.static-ranlib
export AR=i686-w64-mingw32.static-ar
export WINDRES=i686-w64-mingw32.static-windres
export STRIP=i686-w64-mingw32.static-strip
export PKG_CONFIG=i686-w64-mingw32.static-pkg-config
cd ..
export PATH=/opt/mxe/usr/bin:$PATH

./configure \
	--host=i686-w64-mingw32.static \
	--target=i686-w64-mingw32.static \
	--enable-optimize \
	AR=i686-w64-mingw32.static-ar \
	$*

#	--build=i686-pc-linux \

