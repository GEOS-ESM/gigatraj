#!/bin/sh

# Runs parcels forwarda and backwards for five days,

#METSRC="MERRA"
#METSRC="MERRA2"
#METSRC="GEOSfp"
METSRC="GridSBRot"

GVERT="Theta"

GCACHE=" /misc/lrl02/lait/metcache/"

case "${METSRC}" in
"MERRA")
    BEGDATE="2014-03-20T06:30:00"
    ENDDATE="2014-03-25T06:30:00"
    ;;
"MERRA2")    
    BEGDATE="2018-03-20T06:30:00"
    ENDDATE="2018-03-25T06:30:00"
    ;;
"GEOSfp")    
    BEGDATE="2018-03-20T06:30:00"
    ENDDATE="2018-03-25T06:30:00"
    ;;
"GridSBRot")    
    BEGDATE="2018-03-20T06:30:00"
    ENDDATE="2018-03-25T06:30:00"
    ;;
esac

IFILE="gtmodel_s01_input.txt"

FMT="%t, %i, %14.9o, %14.9a, %v"

OFILE="gtmodel_s01_output.txt"

CFILE="gtmodel_s01_original.txt"

BDAY=`echo "${BEGDATE}" | cut -c1-10`T00:00
EDAY=`echo "${ENDDATE}" | cut -c1-10`T21:00


   ../src/gtmodel_s01 \
      --begdate ${BEGDATE} \
      --enddate ${ENDDATE} \
      --zerodate "${BEGDATE}" \
      --source Met${METSRC} \
      --iso \
      --metspacinghr 6 \
      --format "${FMT}" \
      --verbose \
      -f 0.24 \
      --parcels ${IFILE} \
      > ${OFILE}
    OK=$?

if [ ${OK} != 0 ] ; then
   echo "gtmodel_s01 failed: $OK" >&2
   exit 1
fi

if cmp ${CFILE} ${OFILE} ; then
   echo "Files match" >&2
   /bin/rm -f "${OFILE}"
   exit 0
else
   echo "Files do NOT match" >&2
   exit 1
fi
