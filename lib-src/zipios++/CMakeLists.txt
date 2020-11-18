configure_file(cmake-config.h.in zipios++/zipios-config.h)

add_library(zipios++
    src/backbuffer.h
    src/basicentry.cpp
    src/collcoll.cpp
    src/deflateoutputstreambuf.cpp
    src/dircoll.cpp
    src/directory.cpp
    src/directory.h
    src/fcoll.cpp
    src/fcollexceptions.cpp
    src/fileentry.cpp
    src/filepath.cpp
    src/filterinputstreambuf.cpp
    src/filteroutputstreambuf.cpp
    src/inflateinputstreambuf.cpp
    src/outputstringstream.h
    src/zipfile.cpp
    src/ziphead.cpp
    src/zipheadio.cpp
    src/zipinputstream.cpp
    src/zipinputstreambuf.cpp
    src/zipios_common.h
    src/zipoutputstream.cpp
    src/zipoutputstreambuf.cpp
    zipios++/basicentry.h
    zipios++/collcoll.h
    zipios++/deflateoutputstreambuf.h
    zipios++/dircoll.h
    zipios++/fcoll.h
    zipios++/fcollexceptions.h
    zipios++/fileentry.h
    zipios++/filepath.h
    zipios++/filterinputstreambuf.h
    zipios++/filteroutputstreambuf.h
    zipios++/inflateinputstreambuf.h
    zipios++/meta-iostreams.h
    zipios++/simplesmartptr.h
    zipios++/virtualseeker.h
    zipios++/zipfile.h
    zipios++/ziphead.h
    zipios++/zipheadio.h
    zipios++/zipinputstream.h
    zipios++/zipinputstreambuf.h
    zipios++/zipios_defs.h
    zipios++/zipoutputstream.h
    zipios++/zipoutputstreambuf.h
    )

# Make sure that zipios-config.h is visible in all projects that link to zipios++
target_include_directories(zipios++ PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
