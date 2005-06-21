# info_TEXINFOS = enigma.texi

SUBDIRS = images

HTML =  $(wildcard enigma_*.html) \
	enigma.html \
	index.html

htmldir=@datadir@/doc/enigma
html_DATA = $(HTML)

EXTRA_DIST = enigma.texi $(html_DATA)

$(HTML) : enigma.texi
	@if [ "${TEXI2HTML}" ]; then \
	  echo "Running texi2html"; \
	  ${TEXI2HTML} -toc_file=index.html -split=chapter -nosec_nav $(srcdir)/enigma.texi; \
	else \
	  echo "Warning: texi2html not found"; \
	fi

pdf: enigma.pdf

enigma.pdf: enigma.texi
	texi2pdf $<