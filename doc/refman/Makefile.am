# info_TEXINFOS = enigma-ref.texi

HTML = enigma-ref.html

EXTRA_DIST = enigma-ref.texi texi2html.init $(HTML)

htmldir=@datadir@/doc/enigma/reference
html_DATA = $(HTML)

enigma-ref.html: $(srcdir)/enigma-ref.texi $(srcdir)/texi2html.init
	@if [ "${TEXI2HTML}" ]; then \
	  echo "Creating reference manual"; \
	  echo "... texi2html ..."; \
	  ${TEXI2HTML} -Verbose -init_file $(srcdir)/texi2html.init $(srcdir)/enigma-ref.texi; \
	else \
	  echo "Warning: texi2html not found"; \
	fi
