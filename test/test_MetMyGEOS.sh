#!/bin/sh

# set defaults
VALGRIND=""

# get command-line options
while getopts v: ARG
do
    case "${ARG}" in
    v) VALGRIND="valgrind ${OPTARG}";;
    \?) "Bad Option ${ARG}" >&2
        exit 1;;
    esac
done
shift `expr ${OPTIND} - 1`


# how was this script invoked?
THIS="$0"
# extact the script's location
HERE=`dirname "${THIS}"`
# extract the name of what we are testing
TEST=`basename "${THIS}" | sed -e 's/\.sh$//' -e 's/^test_//'`

# tell the test program where it can find its catalog file
export GIGATRAJ_CATALOGS="${HERE}"

# run the test program
${VALGRIND} ./test_${TEST}
if [ $? != 0 ] ; then
   echo "${TEST} test failed" >&2
   exit 1
fi
