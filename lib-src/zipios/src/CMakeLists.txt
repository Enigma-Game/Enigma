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
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

project(zipios)

include_directories(${ZLIB_INCLUDE_DIR})

add_library(${PROJECT_NAME} ${ZIPIOS_LIBRARY_TYPE}
    backbuffer.cpp
    collectioncollection.cpp
    deflateoutputstreambuf.cpp
    directorycollection.cpp
    directoryentry.cpp
    dosdatetime.cpp
    filecollection.cpp
    fileentry.cpp
    filepath.cpp
    filterinputstreambuf.cpp
    filteroutputstreambuf.cpp
    gzipoutputstream.cpp
    gzipoutputstreambuf.cpp
    inflateinputstreambuf.cpp
    streamentry.cpp
    virtualseeker.cpp
    zipcentraldirectoryentry.cpp
    zipendofcentraldirectory.cpp
    zipfile.cpp
    zipinputstream.cpp
    zipinputstreambuf.cpp
    zipios_common.cpp
    ziplocalentry.cpp
    zipoutputstream.cpp
    zipoutputstreambuf.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${ZLIB_LIBRARY}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${ZIPIOS_VERSION_MAJOR}.${ZIPIOS_VERSION_MINOR}
    SOVERSION ${ZIPIOS_VERSION_MAJOR}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
)


# Local Variables:
# indent-tabs-mode: nil
# tab-width: 4
# End:

# vim: ts=4 sw=4 et
