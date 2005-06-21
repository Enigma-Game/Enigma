#!/bin/bash
#
# This script changes the transparent color of all files in the following list.
# The Gimp can't do this and having to specify colorkeys and similar attributes 
# whenever you load an image is tedious.  Storing this information in the image
# itself solves a lot of problems...
#
# btw, this doesn't affect alpha layered images, so these can be safely included 
#

if [ -z "$1" ]; then
	echo 'Usage: 00process.sh [ all | filename(s) ]'
	echo 'Convert all files | single specified file'
	exit 1
fi

if [ "$1" = "all" ]; then
	transparent="st*.png it*.png ring*.png sh*.png invisible.png"
	transparent="$transparent inv-black*.png ac-*.png expl.png oxydb*.png"
else
	transparent="$*"
fi

for f in $transparent; do
    echo "converting $f"
    mogrify -transparent "#ff00ff" $f
done
