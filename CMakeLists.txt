cmake_minimum_required(VERSION 3.14)

project(Enigma VERSION 1.3)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(XercesC REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)

Include(FindIntl)

include_directories(${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${XercesC_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS})

add_compile_definitions(SYSTEM_DATA_DIR="")
add_compile_definitions(DOCDIR="")
add_compile_definitions(LOCALEDIR="")
#add_compile_definitions(CXXLUA=1)

include_directories(lib-src/enet/include)
#include_directories(lib-src/enet/include/enet)
include_directories(lib-src/enigma-core)
include_directories(lib-src/lua)
include_directories(lib-src)
include_directories(lib-src/zipios++/src)
include_directories(lib-src/zipios++/)
include_directories(src)
include_directories(src/gui)
include_directories(src/lev)

add_subdirectory(lib-src)
add_subdirectory(data)
add_subdirectory(src)
add_subdirectory(tools)


include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${Enigma_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Enigma_VERSION_MINOR}")

# Configure macOS packaging
if (APPLE)
    set(CPACK_GENERATOR Bundle ZIP)
    set(CPACK_BUNDLE_NAME "Enigma")
    set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/etc/Info.plist")
    set(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/etc/enigma.icns")
endif()

# Configure source packages
set(CPACK_SOURCE_GENERATOR TGZ ZIP)
set(CPACK_SOURCE_IGNORE_FILES "CmakeLists.txt.user;")

include(CPack)
