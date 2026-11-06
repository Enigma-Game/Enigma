Enigma now comes with preliminary support for the CMake build system.
Official releases will continue to rely on autotools, but CMake support is
available for development and should make it easier to compile Enigma on
platforms other than Linux. It also makes it easier to open Enigma in IDEs
such as QtCreator, CLion, or Visual Studio.

The following external libraries are required to build Enigma with CMake:
    - SDL3
    - SDL3_image
    - SDL3_ttf
    - SDL3_mixer
    - XercesC
    - ZLIB
    - CURL

To run the compiled executable from your IDE, you have to tell Enigma where to
find its data and localization files. To do this, use the "--data" and "--l10n"
command line options when running the program. For example, in CLion, you have
to add the following program arguments to the "Run/Debug" configuration:

    --data $ProjectFileDir$/data --l10n $ProjectFileDir$/data/locale

Other IDEs require similar setups.