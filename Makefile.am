#
# Dependencies:
#   tools -> lib-src/lua
#   src   -> tools, lib-src
#
SUBDIRS = m4 lib-src tools intl src data doc po etc
EXTRA_DIST = enigma CHANGES ACKNOWLEDGEMENTS

docdir = @datadir@/doc/enigma
doc_DATA = README CHANGES COPYING ACKNOWLEDGEMENTS

ACLOCAL_AMFLAGS = -I m4

.PHONY: tolua
tolua:
	cd src && $(MAKE) $(AM_MAKEFLAGS) tolua

.PHONY: pot
pot:
	cd po && $(MAKE) $(AM_MAKEFLAGS) enigma.pot-update

.PHONY: pos
pos:
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-po

.PHONY: gmo
gmo:
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo

.PHONY: macapp
macapp:
	cd src && $(MAKE) $(AM_MAKEFLAGS) bundle && $(MAKE) $(AM_MAKEFLAGS) bundle-fw && $(MAKE) $(AM_MAKEFLAGS) bundle-doc

.PHONY: macpreview
macpreview:
	cd src && $(MAKE) $(AM_MAKEFLAGS) bundle-preview

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

