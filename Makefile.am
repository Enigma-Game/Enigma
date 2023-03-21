#
# Dependencies:
#   tools -> lib-src/lua
#   src   -> tools, lib-src
#
SUBDIRS = m4 lib-src tools src data doc etc
EXTRA_DIST = enigma CHANGES ACKNOWLEDGEMENTS

docdir = @datadir@/doc/enigma
doc_DATA = README CHANGES COPYING ACKNOWLEDGEMENTS

ACLOCAL_AMFLAGS = -I m4

.PHONY: tolua
tolua:
	cd src && $(MAKE) $(AM_MAKEFLAGS) tolua

.PHONY: pot
pot:
	cd data/locale && $(MAKE) $(AM_MAKEFLAGS) enigma.pot-update

.PHONY: pos
pos:
	cd data/locale && $(MAKE) $(AM_MAKEFLAGS) update-po

.PHONY: gmo
gmo:
	echo "make gmo is obsolete, ignored."

.PHONY: pdf
pdf:
	cd doc/manual/images && $(MAKE) pdf
	cd doc/manual && $(MAKE) pdf
	cd doc/reference/images && $(MAKE) pdf
	cd doc/reference && $(MAKE) pdf

.PHONY: splitman
splitman:
	cd doc/reference && $(MAKE) splitman

.PHONY: doxygen
doxygen:
	cd doc && $(MAKE) doxygen

