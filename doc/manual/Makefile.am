# info_TEXINFOS = enigma.texi

SUBDIRS = images

TEXI = $(wildcard $(srcdir)/enigma*.texi)
HTML = $(addprefix $(if $(builddir),$(builddir)/,), $(addsuffix .html, $(basename $(notdir $(TEXI)))))


htmldir=@datadir@/doc/enigma/manual
html_DATA = $(HTML)

EXTRA_DIST = $(TEXI) $(html_DATA)

%.html : %.texi
	@if [ "${TEXI2HTML}" ]; then \
	  echo "Running texi2html"; \
	  ${TEXI2HTML} -nosec_nav $<; \
	else \
	  echo "Warning: texi2html not found"; \
	fi

pdf: enigma.pdf enigma_de.pdf enigma_fr.pdf

%.pdf: %.texi
	texi2dvi --pdf $<