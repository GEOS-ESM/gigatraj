#!/bin/sh

VALGRIND=""
while getopts v: ARG
do
    case "${ARG}" in
    v) VALGRIND="valgrind ${OPTARG}";;
    \?) "Bad Option ${ARG}" >&2
        exit 1;;
    esac
done
shift `expr ${OPTIND} - 1`



THIS="$0"
HERE=`dirname "${THIS}"`

export GIGATRAJ_CATALOGS="${HERE}"

${VALGRIND} ./test_MetMyGEOS
if [ $? != 0 ] ; then
   echo "MetMyGEOS test failed" >&2
   exit 1
fi
