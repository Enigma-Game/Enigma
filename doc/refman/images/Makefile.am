PNG = $(wildcard $(srcdir)/*.png)
PDF = $(addprefix $(builddir), $(addsuffix .pdf, $(basename $(notdir $(PNG)))))

pngdir=@datadir@/doc/refman/images
png_DATA=$(PNG)

SUFFIXES = .png .pdf

pdf: $(PDF)

.png.pdf:
	convert -density 112x112 $< $@

EXTRA_DIST = $(PNG)
