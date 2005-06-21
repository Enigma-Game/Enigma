#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: 04crushprevievs.sh files.."
else
    echo 'Crushing previews..'
    pngcrush -brute -e .png.new $* | grep -i '^   .*png'

    for PNG in *.png */*.png; do
        NEW=$PNG.new
        if [ -s $NEW ]; then
            OLDSIZE=`stat -c '%s' $PNG`
            NEWSIZE=`stat -c '%s' $NEW`
            PERCENT=$(($NEWSIZE*100/$OLDSIZE))
            if [ $PERCENT -lt 100 ]; then
                ls -al $PNG
                mv $NEW $PNG
                ls -al $PNG
             else
                 rm $NEW
             fi
        fi
    done
fi

