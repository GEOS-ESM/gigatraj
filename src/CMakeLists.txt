
set (BASEDIR $ENV{BASEDIR})

ecbuild_add_executable (
   TARGET gtmodel_s01.x 
   SOURCES gtmodel_s01.cc
   LIBS gigatraj filters metsources)

ecbuild_add_executable (
   TARGET gtmodel_s02.x 
   SOURCES gtmodel_s02.cc
   LIBS gigatraj filters metsources)

include_directories (${GIGATRAJ_SOURCE_DIR}/include ${BASEDIR}/Linux/include/netcdf)
