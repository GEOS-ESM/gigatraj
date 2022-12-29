#!/bin/sh

# name of this script
ME="$0"


# number of processors
NPROCS=5

# log file
LOG=""

USAGE="test_MPI.sh [-n number] [-l logfile]"

while getopts n:l: ARG
do
    case "${ARG}" in
    n) NPROCS="${OPTARG}";;
    l) LOG="${OPTARG}";;
    \?) echo "Usage: $USAGE" >&2
        exit 1;;
    esac
done
shift `expr ${OPTIND} - 1`

LOGARG="";
if [ "X${LOG}" != "X" ] ; then
   LOGARG=" --output-filename ${LOG} "
fi

# We strip the ".sh" off of the name we were called as,
# and what is left is the name of the compiled executable
# for this test. Note that the shell script wrapper is
# in ${top_srcdir}/test/ and the compiled executable is in
# ${top_builddir}/test/, so we use basename to strip off
# the shell script directory before removing the ".sh".
PROG=`basename "${ME}" | sed -e 's/\.sh$//'`

mpirun -np ${NPROCS} --tag-output ${LOGARG} "${PROG}"

