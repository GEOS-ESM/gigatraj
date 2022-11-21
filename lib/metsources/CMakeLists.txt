# Note that directory and library name do not correspond!
esma_set_this()

set (srcs
BalanceThetaDot1OTF.cc    LogLinearVinterp.cc   MetMERRA.cc
BilinearHinterp.cc        MERRA2_Directory.cc   MetOnTheFly.cc
DensOTF.cc                MERRA_Directory.cc    MetSBRot.cc
GEOSfpAssim_Directory.cc  MetData.cc            MetSelector.cc
GEOSfpFcast_Directory.cc  MetGEOSfpAssim.cc     MPVOTF.cc
GridField3D.cc            MetGEOSfp.cc          PAltDotOTF.cc
GridField.cc              MetGEOSfpFcast.cc     PAltOTF.cc
GridFieldSfc.cc           MetGEOSPortal.cc      PressOTF.cc
GridLatLonField3D.cc      MetGridData.cc        ThetaDotOTF.cc
GridLatLonFieldSfc.cc     MetGridLatLonData.cc  ThetaOTF.cc
Hinterp.cc                MetGridSBRot.cc       TropOTF.cc
LinearVinterp.cc          MetMERRA2.cc          Vinterp.cc
GEOS_Giga.cc)

set (BASEDIR $ENV{BASEDIR})
esma_add_library (${this}
    SRCS  ${srcs}
    )

include_directories(${BASEDIR}/Linux/include/netcdf ${GIGATRAJ_SOURCE_DIR}/include)

#target_include_directories (${this} PUBLIC
#  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
#  $<BUILD_INTERFACE:${GIGATRAJ_SOURCE_DIR}/include>
#  $<BUILD_INTERFACE:${BASEDIR}/Linux/include/netcdf>)
#target_link_libraries(${this} gigatraj)
