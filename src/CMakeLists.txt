
ecbuild_add_executable (
   TARGET gtmodel_s01.x
   SOURCES gtmodel_s01.cc
   LIBS gigatraj filters metsources)
target_link_libraries(gtmodel_s01.x PRIVATE NetCDF::NetCDF_C)
target_include_directories (gtmodel_s01.x PRIVATE ${GIGATRAJ_SOURCE_DIR}/include)

ecbuild_add_executable (
   TARGET gtmodel_s02.x
   SOURCES gtmodel_s02.cc
   LIBS gigatraj filters metsources)
target_link_libraries(gtmodel_s02.x PRIVATE NetCDF::NetCDF_C)
target_include_directories (gtmodel_s02.x PRIVATE ${GIGATRAJ_SOURCE_DIR}/include)
