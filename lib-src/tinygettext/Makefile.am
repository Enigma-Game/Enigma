EXTRA_DIST = README.md README.enigma LICENSE.md tinygettext_Export.h tinygettext.pc.in
AUTOMAKE_OPTIONS = subdir-objects

libtinygettext_a_CXXFLAGS = @CXXFLAGS@ -fpermissive -I$(top_srcdir)/lib-src/tinygettext/include

noinst_LIBRARIES = libtinygettext.a

libtinygettext_a_SOURCES = \
	src/dictionary.cpp				\
	include/tinygettext/dictionary.hpp		\
	src/dictionary_manager.cpp			\
	include/tinygettext/dictionary_manager.hpp	\
	include/tinygettext/dirent.h			\
	include/tinygettext/file_system.hpp		\
	src/iconv.cpp					\
	include/tinygettext/iconv.hpp			\
	src/language.cpp				\
	include/tinygettext/language.hpp		\
	src/log.cpp					\
	include/tinygettext/log.hpp			\
	include/tinygettext/log_stream.hpp		\
	src/plural_forms.cpp				\
	include/tinygettext/plural_forms.hpp		\
	src/po_parser.cpp				\
	include/tinygettext/po_parser.hpp		\
	src/tinygettext.cpp				\
	include/tinygettext/tinygettext.hpp		\
	src/unix_file_system.cpp			\
	include/tinygettext/unix_file_system.hpp
