#
# Based in part on:
#   Makefile for PO directory in any package using GNU gettext.
#   Copyright (C) 1995-1997, 2000-2004 by Ulrich Drepper <drepper@gnu.ai.mit.edu>
#   Origin: gettext-0.14
#
# This file can be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU General Public
# License but which still want to provide support for the GNU gettext
# functionality.
# Please note that the actual code of GNU gettext is covered by the GNU
# General Public License and is *not* in the public domain.
#
pkgdatadir = $(datadir)/@PACKAGE@/locale
pkgdata_DATA = $(wildcard $(srcdir)/*.po)
EXTRA_DIST = $(pkgdata_DATA)

PACKAGE = @PACKAGE@
VERSION = @VERSION@

@SET_MAKE@

XGETTEXT = @XGETTEXT@
MSGMERGE = msgmerge
MSGMERGE_UPDATE = @MSGMERGE@ --update
MSGINIT = msginit

POFILES = @POFILES@
UPDATEPOFILES = @UPDATEPOFILES@
DISTFILES = remove-potcdate.sin POTFILES.in $(PACKAGE).pot stamp-po $(POFILES)

CATALOGS = @CATALOGS@

# These two variables depend on the location of this directory.
subdir = data/locale
top_builddir = ../..

# These options get passed to xgettext.
XGETTEXT_OPTIONS = --keyword=_ --keyword=N_

# This is the copyright holder that gets inserted into the header of the
# $(PACKAGE).pot file.  Set this to the copyright holder of the surrounding
# package.  (Note that the msgstr strings, extracted from the package's
# sources, belong to the copyright holder of the package.)  Translators are
# expected to transfer the copyright for their translations to this person
# or entity, or to disclaim their copyright.  The empty string stands for
# the public domain; in this case the translators are expected to disclaim
# their copyright.
COPYRIGHT_HOLDER = Free Software Foundation, Inc.

# This is the email address or URL to which the translators shall report
# bugs in the untranslated strings:
# - Strings which are not entire sentences, see the maintainer guidelines
#   in the GNU gettext documentation, section 'Preparing Strings'.
# - Strings which use unclear terms or require additional context to be
#   understood.
# - Strings which make invalid assumptions about notation of date, time or
#   money.
# - Pluralisation problems.
# - Incorrect English spelling.
# - Incorrect formatting.
# It can be your email address, or a mailing list address where translators
# can write to without being subscribed, or the URL of a web page through
# which the translators can contact you.
MSGID_BUGS_ADDRESS = enigma-devel@nongnu.org

.PHONY: $(PACKAGE).pot-update

.PHONY: update-po

SUFFIXES = .po .sed .sin .nop .po-create .po-update

.sin.sed:
	sed -e '/^#/d' $< > t-$@
	mv t-$@ $@

all: stamp-po

# stamp-po is a timestamp denoting the last time at which the CATALOGS have
# been loosely updated. Its purpose is that when a developer or translator
# checks out the package via CVS, and the $(PACKAGE).pot file is not in CVS,
# "make" will update the $(PACKAGE).pot and the $(CATALOGS), but subsequent
# invocations of "make" will do nothing. This timestamp would not be necessary
# if updating the $(CATALOGS) would always touch them; however, the rule for
# $(POFILES) has been designed to not touch files that don't need to be
# changed.
stamp-po: $(srcdir)/$(PACKAGE).pot
	@echo "touch stamp-po"
	@echo timestamp > stamp-poT
	@mv stamp-poT stamp-po

# Note: Target 'all' must not depend on target '$(PACKAGE).pot-update',
# otherwise packages like GCC can not be built if only parts of the source
# have been downloaded.

# This target rebuilds $(PACKAGE).pot; it is an expensive operation.
# Note that $(PACKAGE).pot is not touched if it doesn't need to be changed.
$(PACKAGE).pot-update: $(srcdir)/POTFILES.in remove-potcdate.sed
	rm -f $(builddir)/level_i18n.cc
	find $(top_srcdir)/data/levels -name '.svn' -prune -o -name 'enigma_cross' -prune -o -name '*.xml' -type f ! -name 'index.xml' -exec xsltproc --path $(top_srcdir)/data/schemas --encoding UTF-8 $(top_srcdir)/tools/i18n.xsl '{}' \; >> $(builddir)/level_i18n.tmp
	lua $(top_srcdir)/tools/sub_quote_gettext.lua $(builddir)/level_i18n.tmp > $(builddir)/level_i18n.cc
	rm -f $(builddir)/level_i18n.tmp
	$(XGETTEXT) --default-domain=$(PACKAGE) --directory=$(top_srcdir) --directory=$(builddir) \
	  --add-comments=TRANSLATORS: $(XGETTEXT_OPTIONS) \
	  `cat $(srcdir)/POTFILES.in` \
	  level_i18n.cc \
	  --copyright-holder='$(COPYRIGHT_HOLDER)' \
	  --from-code='utf-8' \
	  --msgid-bugs-address='$(MSGID_BUGS_ADDRESS)'
	test ! -f $(PACKAGE).po || { \
	  if test -f $(srcdir)/$(PACKAGE).pot; then \
	    sed -f remove-potcdate.sed < $(srcdir)/$(PACKAGE).pot > $(PACKAGE).1po && \
	    sed -f remove-potcdate.sed < $(PACKAGE).po > $(PACKAGE).2po && \
	    if cmp $(PACKAGE).1po $(PACKAGE).2po >/dev/null 2>&1; then \
	      rm -f $(PACKAGE).1po $(PACKAGE).2po $(PACKAGE).po; \
	    else \
	      rm -f $(PACKAGE).1po $(PACKAGE).2po $(srcdir)/$(PACKAGE).pot && \
	      mv $(PACKAGE).po $(srcdir)/$(PACKAGE).pot; \
	    fi; \
	  else \
	    mv $(PACKAGE).po $(srcdir)/$(PACKAGE).pot; \
	  fi; \
	}

# This rule has no dependencies: we don't need to update $(PACKAGE).pot at
# every "make" invocation, only create it when it is missing.
# Only "make $(PACKAGE).pot-update" or "make dist" will force an update.
$(srcdir)/$(PACKAGE).pot:
	$(MAKE) $(PACKAGE).pot-update

# This target rebuilds a PO file if $(PACKAGE).pot has changed.
# Note that a PO file is not touched if it doesn't need to be changed.
$(POFILES): $(srcdir)/$(PACKAGE).pot
	@lang=`echo $@ | sed -e 's,.*/,,' -e 's/\.po$$//'`;\
	if test -f "$(srcdir)/$${lang}.po"; then \
	  test "$(srcdir)" = . && cdcmd="" || cdcmd="cd $(srcdir) && "; \
	  echo "$${cdcmd}$(MSGMERGE_UPDATE) $${lang}.po $(PACKAGE).pot"; \
	  cd $(srcdir) && $(MSGMERGE_UPDATE) $${lang}.po $(PACKAGE).pot; \
	  if test -f "$(builddir)/level_i18n.cc"; then \
	    (cd $(builddir); \
	     lua $(top_srcdir)/tools/merge_level_i18n.lua $${lang}); \
	  fi \
	else \
	  $(MAKE) $${lang}.po-create; \
	  if test -f "$(builddir)/level_i18n.cc"; then \
	    (cd $(builddir); \
             lua $(top_srcdir)/tools/merge_level_i18n.lua $${lang}); \
	  fi \
	fi

mostlyclean:
	rm -f remove-potcdate.sed
	rm -f stamp-poT
	rm -f core core.* $(PACKAGE).po $(PACKAGE).1po $(PACKAGE).2po *.new.po
	rm -fr *.o

clean: mostlyclean

distclean: clean
	rm -f Makefile Makefile.in POTFILES level_i18n.cc

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)
dist:
	$(MAKE) update-po
	@$(MAKE) dist2
# This is a separate target because 'update-po' must be executed before.
dist2: $(DISTFILES)
	dists="$(DISTFILES)"; \
	if test -f $(srcdir)/LINGUAS; then dists="$$dists LINGUAS"; fi; \
	for file in $$dists; do \
	  if test -f $$file; then \
	    cp -p $$file $(distdir); \
	  else \
	    cp -p $(srcdir)/$$file $(distdir); \
	  fi; \
	done

update-po: Makefile
	$(MAKE) $(PACKAGE).pot-update
	test -z "$(UPDATEPOFILES)" || $(MAKE) $(UPDATEPOFILES)

# General rule for creating PO files.

.nop.po-create:
	@lang=`echo $@ | sed -e 's/\.po-create$$//'`; \
	echo "File $$lang.po does not exist. If you are a translator, you can create it through 'msginit'." 1>&2; \
	exit 1

# General rule for updating PO files.

.nop.po-update:
	@lang=`echo $@ | sed -e 's/\.po-update$$//'`; \
	tmpdir=`pwd`; \
	echo "$$lang:"; \
	test "$(srcdir)" = . && cdcmd="" || cdcmd="cd $(srcdir) && "; \
	echo "$${cdcmd}$(MSGMERGE) $$lang.po $(PACKAGE).pot -o $$lang.new.po"; \
	cd $(srcdir); \
	if $(MSGMERGE) $$lang.po $(PACKAGE).pot -o $$tmpdir/$$lang.new.po; then \
	  if cmp $$lang.po $$tmpdir/$$lang.new.po >/dev/null 2>&1; then \
	    rm -f $$tmpdir/$$lang.new.po; \
	  else \
	    if mv -f $$tmpdir/$$lang.new.po $$tmpdir/$$lang.po; then \
	      :; \
	      ( cd $(builddir) \
		lua $(top_srcdir)/tools/merge_level_i18n.lua $${lang} $(top_srcdir) ); \
	    else \
	      echo "msgmerge for $$lang.po failed: cannot move $$tmpdir/$$lang.new.po to $$lang.po" 1>&2; \
	      exit 1; \
	    fi; \
	  fi; \
	else \
	  echo "msgmerge for $$lang.po failed!" 1>&2; \
	  rm -f $$tmpdir/$$lang.new.po; \
	fi

