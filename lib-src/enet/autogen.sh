#! /bin/bash
set -o errexit -o nounset -o pipefail
IFS=$'\n'

aclocal && automake --add-missing --foreign --copy && autoconf
