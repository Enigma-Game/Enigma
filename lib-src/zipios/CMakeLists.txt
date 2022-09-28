# Zipios -- a small C++ library that provides easy access to .zip files.
# Copyright (C) 2000-2007  Thomas Sondergaard
# Copyright (c) 2015-2022  Made to Order Software Corp.  All Rights Reserved
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

cmake_minimum_required(VERSION 3.10.2)

project(zipios_project)

option(RUN_TESTS "Enable CTest support and turn on the 'test' make target." OFF)
if(${RUN_TESTS})
    enable_testing()
endif()

# See `dev/version`
set( ZIPIOS_VERSION_MAJOR 2 )
set( ZIPIOS_VERSION_MINOR 3 )
set( ZIPIOS_VERSION_PATCH 2 )
set( ZIPIOS_VERSION_BUILD 0 )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS")
    add_definitions(-DZIPIOS_WINDOWS)
else()
    if(BORLAND)
        message(FATAL_ERROR "Borland compiler not supported!")
    endif()

    # SUNOS is not set by cmake
    string(REGEX MATCH "SunOS" SUNOS ${CMAKE_SYSTEM_NAME})

    set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS} -fwrapv -fPIC"                                 )
    set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} -Werror -Wall -Wextra -Wunused-parameter")
    set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} -O3"                                   )
    #
    if(CYGWIN)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    endif()
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fdiagnostics-show-option -Werror -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Winit-self -Wlogical-op -Wmissing-include-dirs -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=4 -Wundef -Wno-unused -Wunused-variable -Wno-variadic-macros -Wno-parentheses -Wno-unknown-pragmas -Wwrite-strings -Wswitch -Wunused-parameter -Wfloat-equal -Wold-style-cast -Wnoexcept")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

option(BUILD_SHARED_LIBS "Build the ${PROJECT_NAME} libraries shared." ON)

if(BUILD_SHARED_LIBS)
    set(ZIPIOS_LIBRARY_TYPE SHARED)
else(BUILD_SHARED_LIBS)
    set(ZIPIOS_LIBRARY_TYPE STATIC)
endif(BUILD_SHARED_LIBS)

option(BUILD_DOCUMENTATION "Build the ${PROJECT_NAME} documentation." ON)

# To generate coverage, use -D<project name>_COVERAGE=ON
#                       and -DCMAKE_BUILD_TYPE=Debug
option( ${PROJECT_NAME}_COVERAGE "Turn on coverage for ${PROJECT_NAME}." OFF )

if( ${${PROJECT_NAME}_COVERAGE} )
    if( MSVC )
        message( FATAL_ERROR "Coverage is not available on this platform (yet)." )
    endif()

    message("*** COVERAGE TURNED ON ***")
    find_program( COV gcov )
    if( ${COV} STREQUAL "COV-NOTFOUND" )
        message( FATAL_ERROR "Coverage requested, but gcov not installed!" )
    endif()
    if( NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug" )
        message( FATAL_ERROR "Coverage requested, but Debug is not turned on! (i.e. -DCMAKE_BUILD_TYPE=Debug)" )
    endif()
    #
    set( COV_C_FLAGS             "-fprofile-arcs -ftest-coverage" )
    set( COV_CXX_FLAGS           "-fprofile-arcs -ftest-coverage" )
    set( COV_SHARED_LINKER_FLAGS "-fprofile-arcs -ftest-coverage" )
    set( COV_EXE_LINKER_FLAGS    "-fprofile-arcs -ftest-coverage" )
    #
    set( CMAKE_C_FLAGS             "${CMAKE_C_FLAGS} ${COV_C_FLAGS}"                         )
    set( CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} ${COV_CXX_FLAGS}"                     )
    set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${COV_SHARED_LINKER_FLAGS}" )
    set( CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${COV_EXE_LINKER_FLAGS}"       )
endif()

#
# Default install locations, override cache variables to change.
#
include(GNUInstallDirs)
set(BIN_INSTALL_DIR             ${CMAKE_INSTALL_BINDIR}             CACHE PATH "Location to install binaries relative to the install prefix."   )
set(INCLUDE_INSTALL_DIR         ${CMAKE_INSTALL_INCLUDEDIR}         CACHE PATH "Location to install headers relative to the install prefix."    )
set(LIB_INSTALL_DIR             ${CMAKE_INSTALL_LIBDIR}             CACHE PATH "Location to install libraries relative to the install prefix."  )
set(DATA_INSTALL_DIR            ${CMAKE_INSTALL_DATADIR}            CACHE PATH "Location to install data files relative to the install prefix." )
#set(CMAKE_MODULES_INSTALL_DIR   ${CMAKE_INSTALL_CMAKEMODULESDIR}    CACHE PATH "Location to install data files relative to the install prefix." )


find_package( ZLIB REQUIRED )

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/zipios/zipios-config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/zipios/zipios-config.hpp )

# Generate the RPM package specification file
set(PACKAGE "libzipios")
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/contrib/zipios.spec.in ${CMAKE_CURRENT_BINARY_DIR}/contrib/zipios.spec )
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/contrib/zipios.metainfo.xml.in ${CMAKE_CURRENT_BINARY_DIR}/contrib/zipios.metainfo.xml )

include_directories( ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

add_subdirectory( cmake )
add_subdirectory( src   )
add_subdirectory( tools )
add_subdirectory( tests )
add_subdirectory( doc   )

install(
    DIRECTORY zipios
    DESTINATION ${INCLUDE_INSTALL_DIR}
    PATTERN "*.in" EXCLUDE
)

install(
    FILES ${CMAKE_BINARY_DIR}/zipios/zipios-config.hpp
    DESTINATION ${INCLUDE_INSTALL_DIR}/zipios
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/contrib/zipios.metainfo.xml
        DESTINATION ${DATA_INSTALL_DIR}/metainfo
    )
endif()

# todo: how do we determine the correct destination?
#       (i.e. 3.x is now out and 2.8 will probably fail for those users)
#install( DIRECTORY cmake/
#    DESTINATION ${CMAKE_MODULES_INSTALL_DIR}
#)

add_custom_target(zipios_code_analysis
    # Make sure we have an output folder
    COMMAND mkdir -p ${PROJECT_BINARY_DIR}/analysis

    # Count the number of TODO, XXX, TBD, FIXME, and \todo
    COMMAND echo "TODO -- output ${PROJECT_BINARY_DIR}/analysis/todo.txt"
    COMMAND sh dev/todo.sh "${PROJECT_BINARY_DIR}/analysis"

    # Search for files with "invalid" (unwanted really) spaces
    COMMAND echo "Spaces -- output ${PROJECT_BINARY_DIR}/analysis/spaces.txt"
    COMMAND sh dev/spaces.sh "${PROJECT_BINARY_DIR}/analysis"

    # Boost inspect tool that reports various problems in the source
    # Note: I use `... || true` because inspect attempts an SVN check which
    #       obviously is going to fail here
    COMMAND echo "inspect -- output ${PROJECT_BINARY_DIR}/analysis/inspect.html"
    COMMAND inspect -tab -crlf -end -path_name -ascii -minmax -assert_macro -deprecated_macro -unnamed -copyright >"${PROJECT_BINARY_DIR}/analysis/inspect.html" || true

    # All of these are expected to work on source code so make sure we are
    # in the source code top directory
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

##
## To pack the source
##
set(CPACK_PACKAGE_NAME "zipios")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Zipios is a small C++ library for reading and writing zip files.")
set(CPACK_PACKAGE_VENDOR "Made to Order Software Corporation")
set(CPACK_PACKAGE_CONTACT "alexis@m2osw.com")
set(CPACK_RESOURCE_FILE_LICENSE "${zipios_project_SOURCE_DIR}/COPYING")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES "/CVS/;/work-files/;/.git/;.swp$;.*~;cscope.*;/tmp/;BUILD;Build")
set(CPACK_PACKAGE_VERSION "${ZIPIOS_VERSION_MAJOR}.${ZIPIOS_VERSION_MINOR}.${ZIPIOS_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${ZIPIOS_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ZIPIOS_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${ZIPIOS_VERSION_PATCH}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "zipios-${ZIPIOS_VERSION_MAJOR}.${ZIPIOS_VERSION_MINOR}.${ZIPIOS_VERSION_PATCH}")
include(CPack)

# Local Variables:
# indent-tabs-mode: nil
# tab-width: 4
# End:

# vim: ts=4 sw=4 et nocindent
