#! /bin/bash
set -o errexit -o nounset -o pipefail
IFS=$'\n'

autoheader && aclocal && automake --add-missing --foreign --copy && autoconf
