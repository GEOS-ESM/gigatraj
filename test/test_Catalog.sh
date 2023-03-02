#!/bin/sh

HERE=`/bin/pwd`

export GIGATRAJ_CATALOGS="${HERE}"

./test_Catalog
if [ $? != 0 ] ; then
   echo "Catalog test files" >&2
   exit 1
fi
