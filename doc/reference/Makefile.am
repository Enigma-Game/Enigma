# info_TEXINFOS = enigma-ref.texi

SUBDIRS = images

HTML = enigma-ref.html

refdir=@docdir@/reference
ref_DATA = $(HTML) ant_lua.txt sounds.txt soundset.lua lua2xml xml2lua

EXTRA_DIST = enigma-ref.texi enigma-t2h.init level_api_syntax.tex level_api_constants.tex level_api_syntax_examples.tex level_api_tds.tex enigma-ref_ru.html $(ref_DATA)

MOSTLYCLEANFILES = enigma-ref.{at,ats,aux,cp,cps,fn,fns,ky,log,ms,mss,ob,obs,pg,toc,tp,vr,vrs} level_api_syntax.{aux,log} level_api_constants.{aux,log} level_api_syntax_examples.{aux,log} level_api_tds.{aux,log}
CLEANFILES = enigma-ref.{pdf,html} level_api_syntax.pdf level_api_constants.pdf level_api_syntax_examples.pdf level_api_tds.pdf

clean-local:
	-rm -rf enigma-ref

enigma-ref.html: $(srcdir)/enigma-ref.texi $(srcdir)/enigma-t2h.init
	@if [ "${TEXI2HTML}" ]; then \
	  echo "Creating reference manual"; \
	  echo "... texi2html ..."; \
	  ${TEXI2HTML} -I . -Verbose -init_file $(srcdir)/enigma-t2h.init $(srcdir)/enigma-ref.texi; \
	else \
	  echo "Warning: texi2html not found"; \
	fi

.PHONY: splitman
splitman:
	${TEXI2HTML} -split chapter -I . -Verbose -init_file $(srcdir)/enigma-t2h.init $(srcdir)/enigma-ref.texi

.PHONY: pdf-local
pdf-local:
	$(MAKE) -C images
	$(MAKE) pdf-files

pdf-files: enigma-ref.pdf level_api_syntax.pdf level_api_constants.pdf level_api_syntax_examples.pdf level_api_tds.pdf

%.pdf: %.texi
	$(TEXI2DVI) --pdf $<

%.pdf: %.tex
	$(PDFLATEX) $<
