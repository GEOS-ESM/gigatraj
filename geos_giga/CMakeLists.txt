esma_set_this()

set (srcs
GEOS_Giga_InterOp.cc
)

esma_add_library (${this}
   SRCS ${srcs}
   )
target_link_libraries (${this} PUBLIC NetCDF::NetCDF_C)
target_include_directories (${this} PRIVATE ${GIGATRAJ_SOURCE_DIR}/include)
target_link_libraries(${this} PUBLIC gigatraj metsources)
