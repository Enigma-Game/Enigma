#
# Dependencies:
#   tools -> lib-src/lua
#   src   -> tools, lib-src
#
SUBDIRS = m4 lib-src tools intl src data doc po etc
EXTRA_DIST = enigma CHANGES ChangeLog.tla

docdir = @datadir@/doc/enigma
doc_DATA = README NEWS CHANGES COPYING

ACLOCAL_AMFLAGS = -I m4

tolua:
	cd src && $(MAKE) $(AM_MAKEFLAGS) tolua

pot:
	cd po && $(MAKE) $(AM_MAKEFLAGS) enigma.pot-update

pos:
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-po

macapp:
	cd src && $(MAKE) $(AM_MAKEFLAGS) bundle && $(MAKE) $(AM_MAKEFLAGS) bundle-fw && $(MAKE) $(AM_MAKEFLAGS) bundle-dmg

pdf:
	cd doc/manual/images && $(MAKE) pdf
	cd doc/manual && $(MAKE) pdf
	cd doc/refman/images && $(MAKE) pdf
	cd doc/refman && $(MAKE) pdf

