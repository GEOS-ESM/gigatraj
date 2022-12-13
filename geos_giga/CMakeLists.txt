#esma_set_this(OVERRIDE geos_giga)
esma_set_this()

set (BASEDIR $ENV{BASEDIR})

set (srcs
GEOS_Giga_InterOp.cc
)

esma_add_library (${this} 
   SRCS ${srcs}
   )
include_directories (${this} ${GIGATRAJ_SOURCE_DIR}/include ${BASEDIR}/Linux/include/netcdf)
target_link_libraries(${this}  gigatraj metsources)

#install (TARGETS ${this} EXPORT ${this}-targets
#         ARCHIVE DESTINATION lib
#         LIBRARY DESTINATION lib)
#INSTALL(EXPORT ${this}-targets DESTINATION lib)
