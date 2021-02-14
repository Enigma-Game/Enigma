# tinygettext - A gettext replacement that works directly on .po files
# Copyright (C) 2006 Christoph Sommer <christoph.sommer@2006.expires.deltadevelopment.de>
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgement in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

#
# INSTRUCTIONS:
# -------------
#
# Create a directory build/ and change to it. Run
#
#   cmake ..
#
# This creates a set of Makefiles to build the project. Run
#
#   make
#

cmake_policy(SET CMP0005 NEW)

## Project name to use as command prefix

project(tinygettext)
set(VERSION "0.1")

### CMake configuration

cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
SET(CMAKE_MACOSX_RPATH ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${tinygettext_SOURCE_DIR} ${tinygettext_SOURCE_DIR}/CMake)

INCLUDE(CompilerFlags)

# move some config clutter to the advanced section
mark_as_advanced(
	CMAKE_BACKWARDS_COMPATIBILITY
	CMAKE_BUILD_TYPE
	CMAKE_INSTALL_PREFIX
	EXECUTABLE_OUTPUT_PATH
	CMAKE_OSX_ARCHITECTURES
	CMAKE_OSX_SYSROOT
)

## Reveal library type choice to users
option(BUILD_SHARED_LIBS "Produce dynamic library instead of static archive" OFF)

# TinyGetText library compilation

## build list of source files

file(GLOB TINYGETTEXT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB TINYGETTEXT_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/tinygettext/*.hpp)
list(SORT TINYGETTEXT_SOURCES)

## define a target for building the library

add_library(tinygettext ${TINYGETTEXT_SOURCES})

## Fix Windows exports

INCLUDE (GenerateExportHeader)
GENERATE_EXPORT_HEADER( tinygettext
             BASE_NAME tinygettext
             EXPORT_MACRO_NAME TINYGETTEXT_API
             EXPORT_FILE_NAME tinygettext_Export.h
             STATIC_DEFINE tinygettext_BUILT_AS_STATIC
)

## Add tinygettext dir to search path
target_include_directories(tinygettext PUBLIC include/ ${CMAKE_CURRENT_BINARY_DIR})


## SDL/iconv usage
option(HAVE_SDL "Use SDL_iconv instead of system iconv" FALSE)
if(HAVE_SDL)
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS HAVE_SDL)
target_include_directories(tinygettext SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(tinygettext PUBLIC ${SDL2_LIBRARIES})

else(HAVE_SDL)

## Add iconv to include directories
find_package(ICONV REQUIRED)
## Check iconv_const
include(CheckCXXSourceCompiles)
set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${ICONV_INCLUDE_DIR})
check_cxx_source_compiles(
	"
	#include <iconv.h>
	// this declaration will fail when there already exists a non const char** version which returns size_t
	double iconv(iconv_t cd,  char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);
	int main() { return 0; }
	"
	HAVE_ICONV_CONST
)

# TODO: better way of config (until then...)
CHECK_CXX_FLAG(fpermissive)

if(HAVE_ICONV_CONST)
  add_definitions(-DHAVE_ICONV_CONST)
else(HAVE_ICONV_CONST)
  remove_definitions(-DHAVE_ICONV_CONST)
endif(HAVE_ICONV_CONST)

target_include_directories(tinygettext PUBLIC ${ICONV_INCLUDE_DIR})
target_link_libraries(tinygettext PUBLIC ${ICONV_LIBRARY})

endif(HAVE_SDL)

## Debug options

option(WERROR "Stops on first compiler warning in debug mode" OFF)
option(WARNINGS "Enable long list of warnings for compiler to check" ON)

CHECK_CXX_FLAG(std=c++0x)
CHECK_CXX_FLAG(O3)
IF(NOT MSVC)
    #MSVC is excessivly verbose when Wall is activated
    CHECK_CXX_FLAG(Wall)
ENDIF()
CHECK_CXX_FLAG(Wextra)
CHECK_CXX_FLAG(Weffc++)
CHECK_CXX_FLAG(pedantic)

if(WARNINGS)
  CHECK_CXX_FLAG(Wabi)
  CHECK_CXX_FLAG(Wctor-dtor-privacy)
  CHECK_CXX_FLAG(Wold-style-cast)
  CHECK_CXX_FLAG(Woverloaded-virtual)
  CHECK_CXX_FLAG(Wsign-promo)
  CHECK_CXX_FLAG(Wswitch-enum)
  CHECK_CXX_FLAG(Wcast-align)
  CHECK_CXX_FLAG(Wcast-qual)
  CHECK_CXX_FLAG(Wdisabled-optimization)
  CHECK_CXX_FLAG(Wfloat-equal)
  CHECK_CXX_FLAG(Wformat=2)
  CHECK_CXX_FLAG(Winit-self)
  CHECK_CXX_FLAG(Winvalid-pch)
  CHECK_CXX_FLAG(Wmissing-format-attribute)
  CHECK_CXX_FLAG(Wmissing-include-dirs)
  CHECK_CXX_FLAG(Wmissing-noreturn)
  CHECK_CXX_FLAG(Wpacked)
  CHECK_CXX_FLAG(Wredundant-decl s)
  CHECK_CXX_FLAG(Wshadow)
  CHECK_CXX_FLAG(Wsign-conversion)
  CHECK_CXX_FLAG(Wstack-protector)
  CHECK_CXX_FLAG(Wstrict-overflow=5)
  CHECK_CXX_FLAG(Wswitch-default)
  CHECK_CXX_FLAG(Wswitch-enum)
  CHECK_CXX_FLAG(Wundef)
  CHECK_CXX_FLAG(Wconversion)
  # Still left:
  # -Wpadded      (DictionaryManager has a bool that sticks out)

  CHECK_CXX_FLAG(Wstrict-null-sentinel)
  CHECK_CXX_FLAG(Wlogical-op)
  CHECK_CXX_FLAG(Wunsafe-loop-optimizations)
endif(WARNINGS)

if(WERROR)
  CHECK_CXX_FLAG(Werror)
endif(WERROR)

## Extra definitions

add_definitions(-DVERSION=${VERSION})

## Generate test executables in the right place

set(EXECUTABLE_OUTPUT_PATH ${tinygettext_BINARY_DIR}/test)

## Build tinygettext tests

IF(NOT HAVE_SDL) 
  foreach(TEST tinygettext_test po_parser_test)
    ## Add target for tinygettext test
    add_executable(${TEST} test/${TEST}.cpp)
    ## Link with tinygettext library
    target_link_libraries(${TEST} tinygettext)
  endforeach(TEST)
ENDIF()

## Install tinygettext

# use standardized variable name
set(LIB_SUBDIR "lib${LIB_SUFFIX}"
	CACHE STRING "Subdirectory of prefix into which libraries are installed (e.g., lib32, lib64)")

## prepare tinygettext.pc
configure_file(tinygettext.pc.in tinygettext.pc @ONLY)

IF(TINYGETTEXT_IS_SUBPROJECT)
	IF(BUILD_SHARED_LIBS)
		install(TARGETS tinygettext
			RUNTIME DESTINATION bin
			LIBRARY DESTINATION ${LIB_SUBDIR})
	ENDIF()
ELSE()
	install(TARGETS tinygettext
		RUNTIME DESTINATION bin
		ARCHIVE DESTINATION ${LIB_SUBDIR}
		LIBRARY DESTINATION ${LIB_SUBDIR})
	install(FILES ${TINYGETTEXT_HEADERS} ${tinygettext_BINARY_DIR}/tinygettext_Export.h
		DESTINATION include/tinygettext)
	install(FILES ${tinygettext_BINARY_DIR}/tinygettext.pc
		DESTINATION ${LIB_SUBDIR}/pkgconfig)
ENDIF()

# EOF #
