# info_TEXINFOS = enigma-ref.texi

HTML = enigma-ref.html

EXTRA_DIST = enigma-ref.texi enigma-t2h.init $(HTML) demo_i18n.xml\
	demo_simple.xml lua2xml xml2lua

htmldir=@datadir@/doc/enigma/reference
html_DATA = $(HTML)

enigma-ref.html: $(srcdir)/enigma-ref.texi $(srcdir)/enigma-t2h.init
	@if [ "${TEXI2HTML}" ]; then \
	  echo "Creating reference manual"; \
	  echo "... texi2html ..."; \
	  ${TEXI2HTML} -Verbose -init_file $(srcdir)/enigma-t2h.init $(srcdir)/enigma-ref.texi; \
	else \
	  echo "Warning: texi2html not found"; \
	fi
