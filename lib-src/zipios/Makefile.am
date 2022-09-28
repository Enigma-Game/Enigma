EXTRA_DIST = AUTHORS COPYING NEWS README.md
AUTOMAKE_OPTIONS = subdir-objects
CXXFLAGS = @CXXFLAGS@\
        -fpermissive \
        -I$(top_srcdir)/lib-src/zipios/zipios

noinst_LIBRARIES = libzipios.a

libzipios_a_SOURCES = \
	src/backbuffer.cpp	\
	src/backbuffer.hpp	\
	src/CMakeLists.txt	\
	src/collectioncollection.cpp	\
	src/deflateoutputstreambuf.cpp	\
	src/deflateoutputstreambuf.hpp	\
	src/directorycollection.cpp	\
	src/directoryentry.cpp	\
	src/dosdatetime.cpp	\
	src/filecollection.cpp	\
	src/fileentry.cpp	\
	src/filepath.cpp	\
	src/filterinputstreambuf.cpp	\
	src/filterinputstreambuf.hpp	\
	src/filteroutputstreambuf.cpp	\
	src/filteroutputstreambuf.hpp	\
	src/gzipoutputstreambuf.cpp	\
	src/gzipoutputstreambuf.hpp	\
	src/gzipoutputstream.cpp	\
	src/gzipoutputstream.hpp	\
	src/inflateinputstreambuf.cpp	\
	src/inflateinputstreambuf.hpp	\
	src/virtualseeker.cpp	\
	src/zipcentraldirectoryentry.cpp	\
	src/zipcentraldirectoryentry.hpp	\
	src/zipendofcentraldirectory.cpp	\
	src/zipendofcentraldirectory.hpp	\
	src/zipfile.cpp	\
	src/zipinputstreambuf.cpp	\
	src/zipinputstreambuf.hpp	\
	src/zipinputstream.cpp	\
	src/zipinputstream.hpp	\
	src/zipios_common.cpp	\
	src/zipios_common.hpp	\
	src/ziplocalentry.cpp	\
	src/ziplocalentry.hpp	\
	src/zipoutputstreambuf.cpp	\
	src/zipoutputstreambuf.hpp	\
	src/zipoutputstream.cpp	\
	src/zipoutputstream.hpp	\
	zipios/collectioncollection.hpp	\
	zipios/directorycollection.hpp	\
	zipios/directoryentry.hpp	\
	zipios/dosdatetime.hpp	\
	zipios/filecollection.hpp	\
	zipios/fileentry.hpp	\
	zipios/filepath.hpp	\
	zipios/virtualseeker.hpp	\
	zipios/zipfile.hpp	\
	zipios/zipiosexceptions.hpp

