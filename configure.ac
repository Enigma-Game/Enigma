# Process this file with autoconf to produce a configure script.
AC_INIT([enigma],[1.30],[enigma-devel@nongnu.org])
AC_PREREQ([2.72])
AC_CANONICAL_BUILD
AC_CANONICAL_TARGET
AC_CONFIG_SRCDIR([src/enigma.cc])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([src/config.h])
AC_USE_SYSTEM_EXTENSIONS

ALL_LINGUAS="be bs cs da de el es fi fr gd hr hu it ja nl no pl pt ru sk sl sv uk zh"
AC_SUBST([ALL_LINGUAS])

case $host_os in
  *mingw32* ) MINGW32=yes;;
          * ) MINGW32=no;;
esac

case $host_os in
  *cygwin* ) CYGWIN=yes;;
         * ) CYGWIN=no;;
esac

case $host_os in
  *linux* ) LINUX=yes;;
        * ) LINUX=no;;
esac

case $host_os in
 *darwin* ) MACOSX=yes;;
        * ) MACOSX=no;;
esac

AM_CONDITIONAL([MINGW32], [test x$MINGW32 = xyes])
AM_CONDITIONAL([LINUX], [test x$LINUX = xyes])
AM_CONDITIONAL([MACOSX], [test x$MACOSX = xyes])

dnl ======================================================================
dnl Check for programs
dnl ======================================================================

dnl Require C++14 support
AX_CXX_COMPILE_STDCXX_14(noext, mandatory)

CXXTMPFLAGS="$CXXFLAGS"
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_RANLIB
PKG_PROG_PKG_CONFIG

dnl do not add the AC_PROG_CXX default CXXFLAGS
CXXFLAGS="$CXXTMPFLAGS"

dnl Add -mno-cygwin to CXXFLAGS and CFLAGS if working under cygwin
if test "x$CYGWIN" = xyes; then
   CXXFLAGS="$CXXFLAGS -mno-cygwin"
   CFLAGS="$CFLAGS -mno-cygwin"
   CPPFLAGS="$CXXFLAGS -mno-cygwin"
   WINDRES="${WINDRES-windres}"
fi
if test "x$WINDRES" = x; then
   WINDRES="windres"
fi
AC_SUBST([WINDRES])

dnl Add -DMACOSX to CXXFLAGS and CFLAGS if working under darwin
if test "x$MACOSX" = xyes; then
   CPPFLAGS="$CPPFLAGS -DMACOSX"
fi

dnl ---------- Texi2html ----------
AC_PATH_PROG([TEXI2HTML], [texi2html], [""])

dnl ---------- PdfLatex ----------
AC_PATH_PROG([PDFLATEX], [pdflatex], [""])

dnl ---------- ImageMagick ----------
AC_PATH_PROG([CONVERT], [convert], [""])

dnl ---------- Use the included tolua ----------
AC_SUBST([TOLUA])
TOLUA=["\$(top_builddir)/tools/tolua"]
dnl AC_PATH_PROG(TOLUA, tolua, "", ["\$(top_builddir)/tools/tolua"])

dnl ======================================================================
dnl Check for system headers
dnl ======================================================================
AC_CHECK_HEADERS([dirent.h])

dnl ======================================================================
dnl Check for libraries
dnl ======================================================================
AC_SEARCH_LIBS([dlopen], [dl])

dnl This is to check for winmm which must be included to satisfy enet
dnl when compiling under mingw32
if test "$MINGW32" = yes; then
   AC_CHECK_LIB([winmm], [main])
fi

dnl Save a _NOSDL state set of flags for compilation of tools
CFLAGS_NOSDL="$CFLAGS"
CXXFLAGS_NOSDL="$CXXFLAGS"
LIBS_NOSDL="$LIBS"

AM_PATH_SDL2(2.0.0)
AC_SUBST([SDL_LIBS])
AC_SUBST([SDL_CFLAGS])
dnl The flags are now substituted inside the Makefile, but not used for the
dnl rest of the script. So we manually put them in. This is necessary if
dnl SDL has not been installed in the canonical locations.

CFLAGS="$CFLAGS $SDL_CFLAGS"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

dnl ----------------------------------------
dnl Check for SDL support libraries
dnl ----------------------------------------
AC_CHECK_LIB([SDL2_image], [main],,
        [AC_MSG_ERROR([SDL2_image is required to compile Enigma])])
SDL_LIBS="$SDL_LIBS -lSDL2_image"
AC_CHECK_LIB([SDL2_mixer], [main],,
        [AC_MSG_ERROR([SDL2_mixer is required to compile Enigma])])
SDL_LIBS="$SDL_LIBS -lSDL2_mixer"
AC_CHECK_LIB([SDL2_ttf], [main],,
        [AC_MSG_ERROR([SDL2_ttf is required to compile Enigma])])
SDL_LIBS="$SDL_LIBS -lSDL2_ttf"

if test "x$MACOSX" = xyes; then
   SDL2_PREFIX=`$SDL2_CONFIG $sdl2_config_args --variable=libdir`
   SDL_LIBS="-Wl,-rpath,@loader_path/../lib -framework AudioToolbox -framework AudioUnit -framework Cocoa -framework CoreAudio -framework IOKit -framework CoreFoundation -framework Carbon -framework CoreServices -framework ApplicationServices -framework Foundation -framework AppKit -framework OpenGL -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf $SDL2_PREFIX/libSDL2main.a"
   AC_SUBST([SDL2_PREFIX])
fi

dnl ---------------------------------------
dnl Restoring variables to _NOSDL State
dnl ---------------------------------------

CFLAGS="$CFLAGS_NOSDL"
CXXFLAGS="$CXXFLAGS_NOSDL"
LIBS="$LIBS_NOSDL"

dnl ----------------------------------------
dnl Check for libpng
dnl ---------------------------------------
if test "$MINGW32" = no; then
   AC_CHECK_LIB([png], [png_create_write_struct],,
        [AC_MSG_ERROR([libpng is required to compile Enigma])])
fi

dnl ----------------------------------------
dnl Check for xerces
dnl ----------------------------------------
if test "$MINGW32" = yes; then
   AC_MSG_CHECKING([for Xerces release 3.0])
   AC_EGREP_HEADER([gXercesMajVersion = 3], [xercesc/util/XercesVersion.hpp],
      [AC_MSG_RESULT([found])],
      [AC_MSG_ERROR([not found])])
   AC_CHECK_LIB([xerces-c], [main],,
      [AC_MSG_ERROR([xerces is required to compile Enigma])])
else
   AC_MSG_CHECKING([for current Xerces release 3.0])
   AC_EGREP_HEADER([gXercesMajVersion = 3], [xercesc/util/XercesVersion.hpp],
      [xerces3exp="yes"],
      [xerces3exp="no"])
   if test "x$xerces3exp" = xyes; then
      AC_MSG_RESULT([found])
      AC_CHECK_LIB([xerces-c], [main],,
         [AC_MSG_ERROR([xerces is required to compile Enigma])])
   else
      AC_MSG_RESULT([not found])
      AC_MSG_CHECKING([for old Xerces release >=2.4])
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <xercesc/util/XercesVersion.hpp>]],
         [[#if XERCES_VERSION_MAJOR < 2
           #error too old
           #else
           #if XERCES_VERSION_MAJOR == 2 && XERCES_VERSION_MINOR < 4
           #error too old
           #endif
           #endif
           ]])],
         [AC_MSG_RESULT([found])],
         [AC_MSG_ERROR([Xerces >= 2.4 not found.])])
    AC_CHECK_LIB([xerces-c], [main],,
       [AC_MSG_ERROR([xerces is required to compile Enigma])])
  fi
fi

dnl ----------------------------------------
dnl On Mac SDL flags are set up differently. This is a bit of a kludge for it
dnl ---------------------------------------
if test "x$MACOSX" = xyes; then
   CFLAGS="$CFLAGS_NOSDL"
   CXXFLAGS="$CXXFLAGS_NOSDL"
fi

dnl ----------------------------------------
dnl Check for libcurl
dnl ---------------------------------------
if test "$MINGW32" = yes; then
   AC_CHECK_LIB([curl], [main],,
      [AC_MSG_ERROR([libcurl is required to compile Enigma])])
else
   AC_CHECK_LIB([curl], [curl_global_init],,
      [AC_MSG_ERROR([libcurl is required to compile Enigma])])
fi

dnl ----------------------------------------
dnl Activate optimizations when profiling, to get rid of inlineable functions.
dnl ----------------------------------------
AC_MSG_CHECKING([whether to include profiling information])
AC_ARG_ENABLE([profile],
   [AS_HELP_STRING([--enable-profile], [Compile with profiling information])],,
   [enable_profile=no])
if test "x$enable_profile" = xyes; then
   CXXFLAGS="$CXXFLAGS -pg -O2"
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

dnl ----------------------------------------
dnl Build the developer tools?
dnl ----------------------------------------
AC_ARG_ENABLE([tools],
   [AS_HELP_STRING([--enable-tools], [Build developer tools [default=no]])],,
   [enable_tools=no])

AM_CONDITIONAL([BUILDTOOLS], [test "$enable_tools" = yes])
AM_CONDITIONAL([BUILDTTF2BMF], [test "$have_sdl2ttf" = yes -a "$enable_tools" = yes])

dnl ----------------------------------------
dnl Include experimental features?
dnl ----------------------------------------
AC_ARG_ENABLE([experimental],
   [AS_HELP_STRING([--enable-experimental], [Include experimental features [default=no]])],,
   [enable_experimental=no])
if test "x$enable_experimental" = xyes; then
   AC_DEFINE([ENABLE_EXPERIMENTAL],, [Include experimental features])
fi

dnl ----------------------------------------
dnl Include asserts?
dnl ----------------------------------------
AC_MSG_CHECKING([whether to include assert statements])
AC_ARG_ENABLE([assert],
   [AS_HELP_STRING([--enable-assert], [Include assert statements [default=yes]])],,
   [enable_assert=yes])
if test "x$enable_assert" = xyes; then
   CXXFLAGS="$CXXFLAGS -DENABLE_ASSERT"
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

dnl ----------------------------------------
dnl Optimize?
dnl ----------------------------------------
AC_MSG_CHECKING([whether to optimize])
AC_ARG_ENABLE([optimize],
   [AS_HELP_STRING([--enable-optimize], [Compile with optimizations [default=no]])],,
   [enable_optimize=no])
if test "x$enable_optimize" = xyes; then
   CXXFLAGS="$CXXFLAGS -O2 -ffast-math -fomit-frame-pointer"
   AC_MSG_RESULT([yes])
else
   CXXFLAGS="$CXXFLAGS -g"
   AC_MSG_RESULT([no])
fi

dnl ----------------------------------------
dnl Check for gdb debugging
dnl ----------------------------------------
AC_MSG_CHECKING([whether to debug the game with gdb])
AC_ARG_ENABLE([debug-gdb],
   [AS_HELP_STRING([--enable-debug-gdb], [Compile with special debugging options for gdb])],,
   [enable_debug_gdb=no])
if test "x$enable_debug_gdb" = xyes; then
   CXXFLAGS="$CXXFLAGS -ggdb3 -fno-inline -fno-default-inline -fno-omit-frame-pointer -fno-optimize-sibling-calls"
   AC_MSG_RESULT([yes])
else
   CXXFLAGS="$CXXFLAGS"
   AC_MSG_RESULT([no])
fi

dnl ----------------------------------------
dnl Check for profiling
dnl ----------------------------------------
AC_MSG_CHECKING([whether to profile the game])
AC_ARG_ENABLE([profile],
   [AS_HELP_STRING([--enable-profile], [Compile with profiling option for gdb])],,
   [enable_profile=no])
if test "x$enable_profile" = xyes; then
   CXXFLAGS="$CXXFLAGS -pg"
   AC_MSG_RESULT([yes])
else
   CXXFLAGS="$CXXFLAGS"
   AC_MSG_RESULT([no])
fi

dnl ----------------------------------------
dnl Check for C++ Lua
dnl ----------------------------------------
AC_MSG_CHECKING([whether to build lua using c++])
AC_ARG_ENABLE([cxxlua],
   [AS_HELP_STRING([--enable-cxxlua], [Build Lua with C++ [default=yes]])],,
   [enable_cxxlua=yes])
if test "x$enable_cxxlua" = xyes; then
   CXXFLAGS="$CXXFLAGS -DCXXLUA"
   AC_MSG_RESULT([yes])
else
   AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([CXXLUA], [test x$enable_cxxlua = xyes])

dnl ----------------------------------------------------------
dnl Check whether compiler warnings should be emitted
dnl ----------------------------------------------------------
AC_MSG_CHECKING([whether to enable warnings])
AC_ARG_ENABLE([warnings],
   [AS_HELP_STRING([--enable-warnings], [Enable additional compiler warnings])],,
   [enable_warnings=no])
if test "x$enable_warnings" = xyes; then
   CXXFLAGS="$CXXFLAGS -O2 -Wall -W"
   CXXFLAGS="$CXXFLAGS -Wundef -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align"
   CXXFLAGS="$CXXFLAGS -Wwrite-strings -Wconversion -Wsign-compare -Wformat"
   CXXFLAGS="$CXXFLAGS -Wdisabled-optimization -Wno-system-headers"
   AC_MSG_RESULT([yes])
else
   CXXFLAGS="$CXXFLAGS"
   AC_MSG_RESULT([no])
fi

dnl ----------------------------------------------------------
dnl Decide whether to use system enet
dnl ----------------------------------------------------------
AC_MSG_CHECKING([whether to use system enet])
AC_ARG_WITH([system-enet],
   [AS_HELP_STRING([--with-system-enet], [Use system enet])],
   [system_enet=$withval],
   [system_enet=no])

AM_CONDITIONAL([SYSTEM_ENET], [test "x$system_enet" = xyes])
AM_COND_IF([SYSTEM_ENET], [PKG_CHECK_MODULES([LIBENET],[libenet],,
   AC_MSG_ERROR([libenet not found!]))])

dnl Even if using system enet, still need to config the local version for running make dist
AC_CONFIG_SUBDIRS([lib-src/enet])

dnl ----------------------------------------------------------
dnl Configure Makefiles
dnl ----------------------------------------------------------

AC_CONFIG_FILES([Makefile
           m4/Makefile
           data/Makefile
           data/gfx/Makefile
           data/gfx16/Makefile
           data/gfx32/Makefile
           data/gfx40/Makefile
           data/gfx48/Makefile
           data/gfx64/Makefile
           data/gfx32-050/Makefile
           data/gfx32-retro/Makefile
           data/gfx32-retro2/Makefile
           data/levels/Makefile
           data/levels/tutorial_advanced/Makefile
           data/levels/tutorial_basic/Makefile
           data/levels/tutorial_training/Makefile
           data/levels/tutorial_old/Makefile
           data/levels/enigma_i/Makefile
           data/levels/enigma_ii/Makefile
           data/levels/enigma_iii/Makefile
           data/levels/enigma_iv/Makefile
           data/levels/enigma_v/Makefile
           data/levels/enigma_vi/Makefile
           data/levels/enigma_vii/Makefile
           data/levels/enigma_viii/Makefile
           data/levels/enigma_ix/Makefile
           data/levels/enigma_x/Makefile
           data/levels/enigma_xi/Makefile
           data/levels/enigma_xii/Makefile
           data/levels/enigma_cross/Makefile
           data/levels/enigma_esprit/Makefile
           data/levels/enigma_oxyd/Makefile
           data/levels/enigma_peroxyd/Makefile
           data/levels/enigma_oxydmagnum/Makefile
           data/levels/enigma_oxydextra/Makefile
           data/levels/enigma_experimental/Makefile
           data/levels/enigma_demolevels/Makefile
           data/levels/enigma_advent_10/Makefile
           data/levels/pentomino_i/Makefile
           data/levels/soko/Makefile
           data/levels/soko/skinner_microban01/Makefile
           data/levels/soko/skinner_microban02/Makefile
           data/levels/soko/skinner_microban03/Makefile
           data/levels/soko/skinner_microban04/Makefile
           data/levels/soko/skinner_microban05/Makefile
           data/levels/soko/skinner_sasquatch01/Makefile
           data/levels/soko/skinner_sasquatch02/Makefile
           data/levels/soko/skinner_sasquatch03/Makefile
           data/levels/soko/skinner_sasquatch04/Makefile
           data/levels/soko/skinner_sasquatch05/Makefile
           data/levels/soko/skinner_sasquatch06/Makefile
           data/levels/soko/skinner_sasquatch07/Makefile
           data/levels/soko/skinner_sasquatch08/Makefile
           data/levels/soko/skinner_sasquatch09/Makefile
           data/levels/soko/skinner_sasquatch10/Makefile
           data/levels/soko/skinner_sasquatch11/Makefile
           data/levels/soko/skinner_sasquatch12/Makefile
           data/levels/lib/Makefile
           data/levels/patches/Makefile
           data/fonts/Makefile
           data/fonts/MicroHei/Makefile
           data/music/Makefile
           data/music/game/Makefile
           data/music/menu/Makefile
           data/soundsets/Makefile
           data/soundsets/enigma/Makefile
           data/schemas/Makefile
           data/locale/Makefile
           doc/Makefile
           doc/images/Makefile
           doc/images/flags25x15/Makefile
           doc/manual/Makefile
           doc/manual/images/Makefile
           doc/reference/Makefile
           doc/reference/images/Makefile
           etc/Makefile
           lib-src/Makefile
           lib-src/oxydlib/Makefile
           lib-src/tinygettext/Makefile
           lib-src/lua/Makefile
           lib-src/enigma-core/Makefile
           src/Makefile
           tools/Makefile
           etc/enigma.spec
           etc/enigma.nsi
           etc/Info.plist
])

AC_CONFIG_FILES([etc/mingw32-dist.sh], [chmod +x etc/mingw32-dist.sh])
AC_CONFIG_FILES([etc/mxe-dist.sh], [chmod +x etc/mxe-dist.sh])

AC_OUTPUT

AC_MSG_RESULT([

Enigma is now configured

   Source directory:    $srcdir
   Installation prefix: $prefix
   C++ compiler:        $CXX $CXXFLAGS
   Libraries:           $LIBS
   Linker options:      $LDFLAGS
   Languages:           $ALL_LINGUAS
   System enet:         $system_enet

If these values seem to make sense, you can now run make.
])
