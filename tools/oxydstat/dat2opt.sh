#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: dat2opt datafilebasename"
    exit 1
fi

case "$1" in
    "oxyd1ibm") echo '1' ;;
    "oxydex")   echo 'e' ;;
    "oxydmibm") echo 'm' ;;
    "oxydmgg")  echo 'g' ;;
    "peroxyd")  echo 'p' ;;
    *) echo "Unknown '$1'" ;;
esac


