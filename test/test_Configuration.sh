#!/bin/sh

function cleanup {

   /bin/rm -f "${TMPFILE}" "${CFG1}" "${CFG2}" "${CFG3}" "${CFG1}.X" "${CFG2}.X" "${CFG3}.X"

}

TMPFILE="./_test_Configuration_$$"

CFG1="test_config_01.cfg"
CFG2="test_config_02.cfg"
CFG3="test_config_03.cfg"


cleanup

cat - > "${CFG1}.X" << EOF1
AAA=Y
CCC=3
EOF1

cat - > "${CFG2}.X" << EOF2
CCC=5
DDD=12.3
EOF2

cat - > "${CFG3}.X" << EOF2
CCC=7
DDD=-45.67
EEE=something
EOF2


# all defaults
./test_Configuration > "${TMPFILE}"
source "${TMPFILE}"

if [ "x${AAA}" != "xN" -o "x${BBB}" != "xN" -o "x${CCC}" != "x99" -o "x${DDD}" != "x99.99" -o "x${EEE}" != "xgamma" ] ; then
   echo "----------- all-defaults test failed" >&2
   echo "  AAA = ${AAA}" >&2
   echo "  BBB = ${BBB}" >&2
   echo "  CCC = ${CCC}" >&2
   echo "  DDD = ${DDD}" >&2
   echo "  EEE = ${EEE}" >&2
   cleanup
   exit 1
fi


# first config
mv "${CFG1}.X" "${CFG1}"
./test_Configuration > "${TMPFILE}"
source "${TMPFILE}"

if [ "x${AAA}" != "xY" -o "x${BBB}" != "xN" -o "x${CCC}" != "x3" -o "x${DDD}" != "x99.99" -o "x${EEE}" != "xgamma" ] ; then
   echo "----------- first-config test failed" >&2
   echo "  AAA = ${AAA}" >&2
   echo "  BBB = ${BBB}" >&2
   echo "  CCC = ${CCC}" >&2
   echo "  DDD = ${DDD}" >&2
   echo "  EEE = ${EEE}" >&2
   cleanup
   exit 1
fi


mv "${CFG2}.X" "${CFG2}"
mv "${CFG3}.X" "${CFG3}"

# first, second config
./test_Configuration > "${TMPFILE}"
source "${TMPFILE}"

if [ "x${AAA}" != "xY" -o "x${BBB}" != "xN" -o "x${CCC}" != "x5" -o "x${DDD}" != "x12.3" -o "x${EEE}" != "xgamma" ] ; then
   echo "----------- first-second config test failed" >&2
   echo "  AAA = ${AAA}" >&2
   echo "  BBB = ${BBB}" >&2
   echo "  CCC = ${CCC}" >&2
   echo "  DDD = ${DDD}" >&2
   echo "  EEE = ${EEE}" >&2
   cleanup
   exit 1
fi

# now override w/ short command-line options
./test_Configuration -b -e whee > "${TMPFILE}"
source "${TMPFILE}"
if [ "x${AAA}" != "xY" -o "x${BBB}" != "xY" -o "x${CCC}" != "x5" -o "x${DDD}" != "x12.3" -o "x${EEE}" != "xwhee" ] ; then
   echo "----------- short cmdline config test failed" >&2
   echo "  AAA = ${AAA}" >&2
   echo "  BBB = ${BBB}" >&2
   echo "  CCC = ${CCC}" >&2
   echo "  DDD = ${DDD}" >&2
   echo "  EEE = ${EEE}" >&2
   cleanup
   exit 1
fi

# now override w/ long command-line options
./test_Configuration --BBB --EEE who > "${TMPFILE}"
source "${TMPFILE}"
if [ "x${AAA}" != "xY" -o "x${BBB}" != "xY" -o "x${CCC}" != "x5" -o "x${DDD}" != "x12.3" -o "x${EEE}" != "xwho" ] ; then
   echo "----------- long cmdline config test failed" >&2
   echo "  AAA = ${AAA}" >&2
   echo "  BBB = ${BBB}" >&2
   echo "  CCC = ${CCC}" >&2
   echo "  DDD = ${DDD}" >&2
   echo "  EEE = ${EEE}" >&2
   cleanup
   exit 1
fi

# now override w/ mixed short and long command-line options
./test_Configuration -b --CCC 88 -d -32.0 --EEE who > "${TMPFILE}"
source "${TMPFILE}"
if [ "x${AAA}" != "xY" -o "x${BBB}" != "xY" -o "x${CCC}" != "x88" -o "x${DDD}" != "x-32.0" -o "x${EEE}" != "xwho" ] ; then
   echo "----------- mixed cmdline config test failed" >&2
   echo "  AAA = ${AAA}" >&2
   echo "  BBB = ${BBB}" >&2
   echo "  CCC = ${CCC}" >&2
   echo "  DDD = ${DDD}" >&2
   echo "  EEE = ${EEE}" >&2
   cleanup
   exit 1
fi

# now override the first config file, which will itself be overridden by the second
./test_Configuration --conf1 ${CFG3} > "${TMPFILE}"
source "${TMPFILE}"
if [ "x${AAA}" != "xN" -o "x${BBB}" != "xN" -o "x${CCC}" != "x5" -o "x${DDD}" != "x12.3" -o "x${EEE}" != "xsomething" ] ; then
   echo "----------- mixed cmdline config test failed" >&2
   echo "  AAA = ${AAA}" >&2
   echo "  BBB = ${BBB}" >&2
   echo "  CCC = ${CCC}" >&2
   echo "  DDD = ${DDD}" >&2
   echo "  EEE = ${EEE}" >&2
   cleanup
   exit 1
fi

# now override the second config file
./test_Configuration --conf2 ${CFG3} > "${TMPFILE}"
source "${TMPFILE}"
if [ "x${AAA}" != "xY" -o "x${BBB}" != "xN" -o "x${CCC}" != "x7" -o "x${DDD}" != "x-45.67" -o "x${EEE}" != "xsomething" ] ; then
   echo "----------- mixed cmdline config test failed" >&2
   echo "  AAA = ${AAA}" >&2
   echo "  BBB = ${BBB}" >&2
   echo "  CCC = ${CCC}" >&2
   echo "  DDD = ${DDD}" >&2
   echo "  EEE = ${EEE}" >&2
   cleanup
   exit 1
fi



cleanup


