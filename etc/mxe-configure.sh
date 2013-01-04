#!/bin/sh

export CC="i686-pc-mingw32-gcc"
export CXX="i686-pc-mingw32-g++"
export CXXFLAGS="-g0"
export RANLIB=i686-pc-mingw32-ranlib
export AR=i686-pc-mingw32-ar
export WINDRES=i686-pc-mingw32-windres
export STRIP=i686-pc-mingw32-strip
cd ..
make distclean
PATH=/usr/i686-pc-mingw32/bin:$PATH \
	./configure \
	--host=i686-pc-mingw32 \
	--target=i686-pc-mingw32 \
	--build=i686-pc-linux \
	--enable-optimize \
	AR=i686-pc-mingw32-ar \
	$*

