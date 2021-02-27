#!/bin/sh

export CC="i686-w64-mingw32-gcc"
export CXX="i686-w64-mingw32-g++"
export CXXFLAGS="-g0"
export RANLIB=i686-w64-mingw32-ranlib
export AR=i686-w64-mingw32-ar
export WINDRES=i686-w64-mingw32-windres
export STRIP=i686-w64-mingw32-strip
cd ..
make distclean
cd lib-src/tinygettext
rm -Rf build
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-cross-mingw32-linux.cmake ..
cd ../../..
PATH=/usr/i686-w64-mingw32/bin:$PATH \
	./configure \
	--host=i686-w64-mingw32 \
	--target=i686-w64-mingw32 \
	--enable-optimize \
	AR=i686-w64-mingw32-ar \
	$*

#	--build=i686-pc-linux \

