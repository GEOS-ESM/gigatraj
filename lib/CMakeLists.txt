esma_set_this(OVERRIDE gigatraj)

#add_definitions (-DWRAP_0) # defined in gigatraj.hh

set (srcs
CalGregorian.cc   Integrator.cc       PGenFile.cc     PlanetSphereNav.cc
Configuration.cc  IntegRK4a.cc        PGenGrid.cc     ProcessGrp.cc
Earth.cc          MPIGrp.cc           PGenRep.cc      RandomSrc.cc
FileLock.cc       Parcel.cc           PGenRnd.cc      SerialGrp.cc
FilePath.cc       ParcelGenerator.cc  PGenRndDisc.cc  Swarm.cc
Flock.cc          PGenDisc.cc         PlanetNav.cc    trace.cc
IntegRK4_GEOS.cc
)
esma_add_subdirectories(filters metsources)

esma_add_library (${this}
   SRCS ${srcs}
   )

target_link_libraries(${this} PUBLIC NetCDF::NetCDF_C)
target_include_directories (${this} PRIVATE ${GIGATRAJ_SOURCE_DIR}/include)
