set (this gigatraj)

set (srcs
CalGregorian.cc   Integrator.cc       PGenFile.cc     PlanetSphereNav.cc
Configuration.cc  IntegRK4a.cc        PGenGrid.cc     ProcessGrp.cc
Earth.cc          MPIGrp.cc           PGenRep.cc      RandomSrc.cc
FileLock.cc       Parcel.cc           PGenRnd.cc      SerialGrp.cc
FilePath.cc       ParcelGenerator.cc  PGenRndDisc.cc  Swarm.cc
Flock.cc          PGenDisc.cc         PlanetNav.cc    trace.cc)

add_subdirectory (filters)
add_subdirectory (metsources)

add_library (${this} ${srcs})
target_link_libraries(${this} filters metsources)
include_directories (${this} ${CMAKE_SOURCE_DIR}/include ${BASEDIR}/Linux/include/netcdf)
