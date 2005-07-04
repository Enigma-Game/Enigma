#! /bin/sh

bstrap ()
{
    echo -n "Bootstrapping $1    "
    if (cd $1 && sh autogen.sh); then
        echo "OK"
    else
        echo "ERROR"
        exit 1
    fi
}

echo ""
bstrap "./lib-src/zipios++"
bstrap "./lib-src/enet"

echo -n "Bootstrapping ./    "
if autoheader && aclocal -I m4 && automake --add-missing --foreign --copy && autoconf; then
    echo "OK"
else
    echo "ERROR"
    exit 1
fi
echo "You can now run configure and make."
