#!/bin/sh
# PALETTE=cabana-64-8x.png
# PALETTE=resurrect-64-8x.png
# PALETTE=aap-64-8x.png
# PALETTE=journey-8x.png
# PALETTE=famicube-8x.png
PALETTE=sweet-canyon-extended-64-8x.png
# PALETTE=enigma-64-1x.png
# PALETTE=enigma-64.ppm
# PALETTE=enigma-128.ppm

OUTDIR="${PALETTE%.*}"
mkdir -p $OUTDIR
for f in ../../data/gfx32/*.png; do
  convert $f -alpha on -write MPR:orig -dither None -remap $PALETTE  MPR:orig -compose copyalpha -composite -define png:color-type=6 $OUTDIR/$(basename $f)
done

