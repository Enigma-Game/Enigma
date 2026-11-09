# CMake Support in Enigma

Enigma supports building with CMake (minimum version 3.16).

## Dependencies

Enigma uses SDL3 and official SDL3 support libraries:
- SDL3 (`find_package(SDL3 CONFIG REQUIRED)`)
- SDL3_image (`find_package(SDL3_image CONFIG REQUIRED)`)
- SDL3_mixer (`find_package(SDL3_mixer CONFIG REQUIRED)`)
- SDL3_ttf (`find_package(SDL3_ttf CONFIG REQUIRED)`)

Also required:
- XercesC (`find_package(XercesC REQUIRED)`)
- zlib (`find_package(ZLIB REQUIRED)`)
- curl (`find_package(CURL REQUIRED)`)

See `doc/REQUIREMENTS` for more information.

## Building

On Windows, install or extract the official development packages (e.g. `SDL3-devel-*-VC.zip`) and add them to `CMAKE_PREFIX_PATH`.
Alternatively, use modern IDEs such as e.g CLion to download the reqruired packages after import using vcpkg.
Then start the building process.

On Linux or macOS with package managers (e.g., Homebrew, or distro packages etc.), in the project root folder:

```sh
cmake -B build
cmake --build build
```
The build is then started.
