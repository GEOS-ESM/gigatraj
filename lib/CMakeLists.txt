esma_set_this(OVERRIDE gigatraj)

set (BASEDIR $ENV{BASEDIR})

set (srcs
CalGregorian.cc   Integrator.cc       PGenFile.cc     PlanetSphereNav.cc
Configuration.cc  IntegRK4a.cc        PGenGrid.cc     ProcessGrp.cc
Earth.cc          MPIGrp.cc           PGenRep.cc      RandomSrc.cc
FileLock.cc       Parcel.cc           PGenRnd.cc      SerialGrp.cc
FilePath.cc       ParcelGenerator.cc  PGenRndDisc.cc  Swarm.cc
Flock.cc          PGenDisc.cc         PlanetNav.cc    trace.cc
)
esma_add_subdirectories(filters metsources)

esma_add_library (${this} 
   SRCS ${srcs}
   )
include_directories (${this} ${GIGATRAJ_SOURCE_DIR}/include ${BASEDIR}/Linux/include/netcdf)
#install (TARGETS ${this} EXPORT ${this}-targets
#         ARCHIVE DESTINATION lib
#         LIBRARY DESTINATION lib)
#INSTALL(EXPORT ${this}-targets DESTINATION lib)
