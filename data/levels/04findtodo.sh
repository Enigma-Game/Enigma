echo "*** GetNamedObject"
grep -n -e 'GetNamedObject' *.lua

echo "*** SendMessage and GetNamedObject"
grep -n -e 'SendMessage.*GetNamedObject' *.lua

echo "*** levels that set holes attribute explicitly"
grep -n -e 'st-shogun.*holes' *.lua


echo "*** SendMessage with redundant nil argument"
grep -n -e 'SendMessage.*nil' *.lua

echo "*** levels that set orientation attribute explicitly"
grep -n -e 'st-oneway.*orientation' *.lua

echo "*** bolder stones with direction attribute"
grep -n -e 'st-bolder.*direction' *.lua
