pkgdatadir = $(datadir)/@PACKAGE@/thumbs
pkgdata_DATA = README

install-data-local:
	$(INSTALL) -d -m 0777 $(pkgdatadir)

uninstall-local:
	rm -f $(pkgdatadir)/*.png $(pkgdatadir)/README

