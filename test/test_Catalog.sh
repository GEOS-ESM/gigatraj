#!/bin/sh

THIS="$0"
HERE=`dirname "${THIS}"`

export GIGATRAJ_CATALOGS="${HERE}"

./test_Catalog
if [ $? != 0 ] ; then
   echo "Catalog test failed" >&2
   exit 1
fi
