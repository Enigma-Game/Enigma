cmake_minimum_required(VERSION 3.14)

# Automatically locate vcpkg toolchain if not explicitly provided
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if (EXISTS "$ENV{USERPROFILE}/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{USERPROFILE}/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain")
    elseif (DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain")
    elseif (EXISTS "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "vcpkg toolchain")
    endif()
endif()
if (DEFINED CMAKE_TOOLCHAIN_FILE AND NOT _VCPKG_TOOLCHAIN)
    include("${CMAKE_TOOLCHAIN_FILE}")
endif()

project(Enigma VERSION 1.3)

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options(/permissive-)
    add_link_options(/MAP)
endif()

# Search for additional cmake modules in cmake/
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(XercesC REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)

Include(FindIntl)

add_compile_definitions(ENABLE_ASSERT="")
add_compile_definitions(SYSTEM_DATA_DIR="${CMAKE_SOURCE_DIR}/data")
add_compile_definitions(DOCDIR="${CMAKE_SOURCE_DIR}/doc")
add_compile_definitions(LOCALEDIR="${CMAKE_SOURCE_DIR}/data/locale")
add_compile_definitions(ENABLE_NLS="")
add_compile_definitions(NOMINMAX)
add_compile_definitions(PACKAGE_NAME="Enigma")
#add_compile_definitions(CXXLUA=1)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_compile_definitions(MACOSX=1)
endif()

set(HAVE_SDL ON CACHE BOOL "Use SDL_iconv instead of system iconv" FORCE)

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
