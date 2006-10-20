# info_TEXINFOS = enigma.texi

SUBDIRS = images

HTML =  $(wildcard enigma_*.html) \
	enigma.html \
	index.html

htmldir=@datadir@/doc/enigma
html_DATA = $(HTML)

EXTRA_DIST = enigma.texi enigma_de.texi enigma_fr.texi $(html_DATA)

$(HTML) : enigma.texi
	@if [ "${TEXI2HTML}" ]; then \
	  echo "Running texi2html"; \
	  ${TEXI2HTML} -toc_file=index.html -split=chapter -output=. -nosec_nav $(srcdir)/enigma.texi; \
	else \
	  echo "Warning: texi2html not found"; \
	fi

pdf: enigma.pdf enigma_de.pdf enigma_fr.pdf

%.pdf: %.texi
	texi2pdf $<