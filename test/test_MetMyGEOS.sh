#!/bin/sh

HERE=`/bin/pwd`

export GIGATRAJ_CATALOGS="${HERE}"

./test_MetMyGEOS
if [ $? != 0 ] ; then
   echo "MetMyGEOS test failed" >&2
   exit 1
fi
