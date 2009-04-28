# info_TEXINFOS = enigma-ref.texi

SUBDIRS = images

HTML = enigma-ref.html

refdir=@datadir@/doc/enigma/reference
ref_DATA = $(HTML) ant_lua.txt sounds.txt soundset.lua lua2xml xml2lua

EXTRA_DIST = enigma-ref.texi enigma-t2h.init $(ref_DATA)


enigma-ref.html: $(srcdir)/enigma-ref.texi $(srcdir)/enigma-t2h.init
	@if [ "${TEXI2HTML}" ]; then \
	  echo "Creating reference manual"; \
	  echo "... texi2html ..."; \
	  ${TEXI2HTML} -Verbose -init_file $(srcdir)/enigma-t2h.init $(srcdir)/enigma-ref.texi; \
	else \
	  echo "Warning: texi2html not found"; \
	fi

pdf:
	$(MAKE) -C images
	$(MAKE) pdf_local

pdf_local: enigma-ref.pdf

%.pdf: %.texi
	$(TEXI2DVI) --pdf $<
# Better use $(TEXI2PDF). May be missing on elder systems.

clean:
	$(RM) enigma-ref.pdf
	$(RM) enigma-ref.{at,ats,aux,cp,cps,fn,fns,ky,log,ms,mss,ob,obs,pg,toc,tp,vr,vrs}
