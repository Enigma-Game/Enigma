# info_TEXINFOS = enigma.texi

SUBDIRS = images

TEXI = $(wildcard $(srcdir)/enigma*.texi)
HTML = $(addprefix $(if $(builddir), $(builddir)/,), $(addsuffix .html, $(basename $(notdir $(TEXI)))))


htmldir=@datadir@/doc/enigma/manual
html_DATA = $(HTML)

EXTRA_DIST = $(TEXI) $(html_DATA)

MOSTLYCLEANFILES = enigma{,_de,_fr}.{at,ats,aux,cp,cps,fn,fns,ky,log,ms,mss,ob,obs,pg,toc,tp,vr,vrs}
CLEANFILES = enigma{,_de,_fr}.{pdf,html}


%.html : %.texi
	@if [ "${TEXI2HTML}" ]; then \
	  if test "$<" = "enigma_ru.texi"; then \
            echo "skip ru manual"; \
          else \
            echo "Running texi2html"; \
	    ${TEXI2HTML} -nosec_nav $<; \
          fi \
	else \
	  echo "Warning: texi2html not found"; \
	fi

pdf: enigma.pdf enigma_de.pdf enigma_fr.pdf

%.pdf: %.texi
	texi2dvi --pdf $<