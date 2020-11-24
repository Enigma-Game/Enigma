cmake_minimum_required(VERSION 3.14)

project(Enigma VERSION 1.3)

set(CMAKE_CXX_STANDARD 14)

# Search for additional cmake modules in cmake/
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(XercesC REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)

Include(FindIntl)

add_compile_definitions(SYSTEM_DATA_DIR="")
add_compile_definitions(DOCDIR="")
add_compile_definitions(LOCALEDIR="")
#add_compile_definitions(CXXLUA=1)

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
    set(PACKAGE_VERSION "${Enigma_VERSION}")
    configure_file(etc/Info.plist.in Info.plist)
    set(CPACK_GENERATOR Bundle ZIP)
    set(CPACK_BUNDLE_NAME "Enigma")
    set(CPACK_BUNDLE_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist")
    set(CPACK_BUNDLE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/etc/enigma.icns")
endif()

# Configure source packages
set(CPACK_SOURCE_GENERATOR TGZ ZIP)
set(CPACK_SOURCE_IGNORE_FILES
    CMakeLists.txt.user
    .git
    .DS_Store
    )

include(CPack)
