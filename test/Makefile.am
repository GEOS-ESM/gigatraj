## Process this file with automake to produce Makefile.in


## Everything in the test suite needs the gigatraj library
LDADD = -lgigatraj
AM_LDFLAGS = -L ../lib/

GIGATRAJ_INCLUDE = -I$(top_srcdir)/include/

## Everything needs the gigatraj library includes
AM_CPPFLAGS  = $(GIGATRAJ_INCLUDE)


##### Very Basic stuff

TESTS = test_RandomSrc test_Configuration.sh \
        test_FilePath test_FileLock \
        test_CalGregorian \
        test_Earth
check_PROGRAMS = test_RandomSrc test_Configuration \
        test_FilePath test_FileLock \
        test_CalGregorian \
        test_Earth 


##### Process Group stuff

TESTS += test_SerialGrp 
check_PROGRAMS +=  test_SerialGrp

TESTS += test_FileLock_Serial
check_PROGRAMS +=  test_FileLock_Serial

if MPI
   TESTS += test_MPIGrp.sh  test_FileLock_MPI.sh
   check_PROGRAMS += test_MPIGrp test_FileLock_MPI
   test_FileLock_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
endif   


##### Meteorological data stuff

TESTS += test_GridLatLonFieldSfc test_GridLatLonField3D \
         test_GridLatLonFieldSfc_serial  test_GridLatLonField3D_serial
check_PROGRAMS += test_GridLatLonField3D test_GridLatLonFieldSfc  \
         test_GridLatLonFieldSfc_serial test_GridLatLonField3D_serial
if MPI
   TESTS += test_GridLatLonFieldSfc_MPI.sh  
   check_PROGRAMS += test_GridLatLonFieldSfc_MPI
   test_GridLatLonFieldSfc_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI

   TESTS += test_GridLatLonField3D_MPI.sh
   check_PROGRAMS += test_GridLatLonField3D_MPI
   test_GridLatLonField3D_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
endif  

TESTS          += test_LinearVinterp test_LogLinearVinterp test_BilinearHinterp
check_PROGRAMS += test_LinearVinterp test_LogLinearVinterp test_BilinearHinterp
TESTS += test_BilinearHinterp_serial
check_PROGRAMS += test_BilinearHinterp_serial
if MPI
   TESTS += test_BilinearHinterp_MPI.sh
   check_PROGRAMS += test_BilinearHinterp_MPI
   test_BilinearHinterp_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
endif  

TESTS += test_ThetaOTF test_TropOTF test_PressOTF test_PAltOTF test_PAltDotOTF test_DensOTF test_ThetaDotOTF test_BalanceThetaDot1OTF
check_PROGRAMS += test_ThetaOTF test_TropOTF test_PressOTF test_PAltOTF test_PAltDotOTF test_DensOTF test_ThetaDotOTF test_BalanceThetaDot1OTF



TESTS += test_MetSBRot test_MetSBRot_serial
check_PROGRAMS += test_MetSBRot test_MetSBRot_serial
if MPI
   TESTS += test_MetSBRot_MPI.sh
   check_PROGRAMS += test_MetSBRot_MPI
   test_MetSBRot_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
endif 
TESTS += test_MetGridSBRot test_MetGridSBRot_serial
check_PROGRAMS += test_MetGridSBRot test_MetGridSBRot_serial
if MPI
   TESTS += test_MetGridSBRot_MPI.sh
   check_PROGRAMS += test_MetGridSBRot_MPI
   test_MetGridSBRot_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
endif 

if MERRA
   TESTS += test_MetMERRA
   check_PROGRAMS += test_MetMERRA
   test_MetMERRA_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
endif
if MERRA2
   TESTS += test_MetMERRA2
   check_PROGRAMS += test_MetMERRA2
   test_MetMERRA2_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
endif
if GEOSFP
   TESTS += test_MetGEOSfpFcast test_MetGEOSfpAssim test_MetGEOSfp
   check_PROGRAMS += test_MetGEOSfpFcast test_MetGEOSfpAssim test_MetGEOSfp
   test_MetGEOSfpFcast_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
   test_MetGEOSfpAssim_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
   test_MetGEOSfp_MPI_CPPFLAGS = $(AM_CPPFLAGS) -DUSING_MPI
endif
TESTS += test_MetSelector
check_PROGRAMS += test_MetSelector

##### Parcel stuff

TESTS += test_Parcel \
        test_StreamDump_Load \
        test_StreamRead \
        test_StreamPrint \
        test_FlockSerial \
        test_SwarmSerial \
        test_ChangeVertical
check_PROGRAMS += test_Parcel \
        test_StreamDump_Load \
        test_StreamRead \
        test_StreamPrint \
        test_FlockSerial \
        test_SwarmSerial \
        test_ChangeVertical

if MPI
   TESTS += test_FlockMPI.sh 
   check_PROGRAMS += test_FlockMPI  
   TESTS += test_SwarmMPI.sh 
   check_PROGRAMS += test_SwarmMPI  
   TESTS += test_StreamPrintMPI.sh
   check_PROGRAMS += test_StreamPrintMPI 
   TESTS += test_StreamReadMPI.sh
   check_PROGRAMS += test_StreamReadMPI 
endif   

TESTS += test_PGenRep test_PGenGrid test_PGenRnd \
         test_PGenRndDisc test_PGenDisc test_PGenFile \
         test_traj000 test_traj001 test_traj010
check_PROGRAMS += test_PGenRep test_PGenGrid test_PGenRnd \
        test_PGenRndDisc test_PGenDisc test_PGenFile \
        test_traj000 test_traj001 test_traj010

TESTS += test_gtmodel_s01.sh

###########  Sources

test_RandomSrc_SOURCES = test_RandomSrc.cc test_utils.cc
test_RandomSrc_DEPENDENCIES = ../lib/libgigatraj.a

test_Configuration_SOURCES = test_Configuration.cc test_utils.cc
test_Configuration_DEPENDENCIES = ../lib/libgigatraj.a

test_FilePath_SOURCES = test_FilePath.cc test_utils.cc
test_FilePath_DEPENDENCIES = ../lib/libgigatraj.a

test_FileLock_SOURCES = test_FileLock.cc test_utils.cc
test_FileLock_DEPENDENCIES = ../lib/libgigatraj.a

test_FileLock_Serial_SOURCES = test_FileLock_PGrp.cc test_utils.cc
test_FileLock_Serial_DEPENDENCIES = ../lib/libgigatraj.a

test_FileLock_MPI_SOURCES = test_FileLock_PGrp.cc test_utils.cc
test_FileLock_MPI_DEPENDENCIES = ../lib/libgigatraj.a

test_CalGregorian_SOURCES = test_CalGregorian.cc test_utils.cc
test_CalGregorian_DEPENDENCIES = ../lib/libgigatraj.a

test_Earth_SOURCES = test_Earth.cc test_utils.cc
test_Earth_DEPENDENCIES = ../lib/libgigatraj.a

test_SerialGrp_SOURCES = test_SerialGrp.cc test_utils.cc
test_SerialGrp_DEPENDENCIES = ../lib/libgigatraj.a

test_MPIGrp_SOURCES = test_MPIGrp.cc test_utils.cc
test_MPIGrp_DEPENDENCIES = ../lib/libgigatraj.a

test_GridLatLonFieldSfc_SOURCES = test_GridLatLonFieldSfc.cc test_utils.cc
test_GridLatLonFieldSfc_DEPENDENCIES = ../lib/libgigatraj.a

test_GridLatLonField3D_SOURCES = test_GridLatLonField3D.cc test_utils.cc
test_GridLatLonField3D_DEPENDENCIES = ../lib/libgigatraj.a

test_GridLatLonFieldSfc_serial_SOURCES = test_GridLatLonFieldSfc_PGrp.cc test_utils.cc
test_GridLatLonFieldSfc_serial_DEPENDENCIES = ../lib/libgigatraj.a

test_GridLatLonFieldSfc_MPI_SOURCES = test_GridLatLonFieldSfc_PGrp.cc test_utils.cc
test_GridLatLonFieldSfc_MPI_DEPENDENCIES = ../lib/libgigatraj.a

test_GridLatLonField3D_serial_SOURCES = test_GridLatLonField3D_PGrp.cc test_utils.cc
test_GridLatLonField3D_serial_DEPENDENCIES = ../lib/libgigatraj.a

test_GridLatLonField3D_MPI_SOURCES = test_GridLatLonField3D_PGrp.cc test_utils.cc
test_GridLatLonField3D_MPI_DEPENDENCIES = ../lib/libgigatraj.a

test_LinearVinterp_SOURCES = test_LinearVinterp.cc test_utils.cc
test_LinearVinterp_DEPENDENCIES = ../lib/libgigatraj.a

test_LogLinearVinterp_SOURCES = test_LogLinearVinterp.cc test_utils.cc
test_LogLinearVinterp_DEPENDENCIES = ../lib/libgigatraj.a

test_BilinearHinterp_SOURCES = test_BilinearHinterp.cc test_utils.cc
test_BilinearHinterp_DEPENDENCIES = ../lib/libgigatraj.a

test_BilinearHinterp_serial_SOURCES = test_BilinearHinterp_PGrp.cc test_utils.cc
test_BilinearHinterp_serial_DEPENDENCIES = ../lib/libgigatraj.a

test_BilinearHinterp_MPI_SOURCES = test_BilinearHinterp_PGrp.cc test_utils.cc
test_BilinearHinterp_MPI_DEPENDENCIES = ../lib/libgigatraj.a

test_ThetaOTF_SOURCES = test_ThetaOTF.cc test_utils.cc
test_ThetaOTF_DEPENDENCIES = ../lib/libgigatraj.a

test_TropOTF_SOURCES = test_TropOTF.cc test_utils.cc
test_TropOTF_DEPENDENCIES = ../lib/libgigatraj.a

test_PressOTF_SOURCES = test_PressOTF.cc test_utils.cc
test_PressOTF_DEPENDENCIES = ../lib/libgigatraj.a

test_PAltOTF_SOURCES = test_PAltOTF.cc test_utils.cc
test_PAltOTF_DEPENDENCIES = ../lib/libgigatraj.a

test_PAltDotOTF_SOURCES = test_PAltDotOTF.cc test_utils.cc
test_PAltDotOTF_DEPENDENCIES = ../lib/libgigatraj.a

test_DensOTF_SOURCES = test_DensOTF.cc test_utils.cc
test_DensOTF_DEPENDENCIES = ../lib/libgigatraj.a

test_ThetaDotOTF_SOURCES = test_ThetaDotOTF.cc test_utils.cc
test_ThetaDotOTF_DEPENDENCIES = ../lib/libgigatraj.a

test_BalanceThetaDot1OTF_SOURCES = test_BalanceThetaDot1OTF.cc test_utils.cc 
test_BalanceThetaDot1OTF_DEPENDENCIES = ../lib/libgigatraj.a

test_MetSBRot_SOURCES = test_MetSBRot.cc test_utils.cc
test_MetSBRot_DEPENDENCIES = ../lib/libgigatraj.a

test_MetSBRot_serial_SOURCES = test_MetSBRot_PGrp.cc test_utils.cc
test_MetSBRot_serial_DEPENDENCIES = ../lib/libgigatraj.a

test_MetSBRot_MPI_SOURCES = test_MetSBRot_PGrp.cc test_utils.cc
test_MetSBRot_MPI_DEPENDENCIES = ../lib/libgigatraj.a

test_MetGridSBRot_SOURCES = test_MetGridSBRot.cc test_utils.cc
test_MetGridSBRot_DEPENDENCIES = ../lib/libgigatraj.a

test_MetGridSBRot_serial_SOURCES = test_MetGridSBRot_PGrp.cc test_utils.cc
test_MetGridSBRot_serial_DEPENDENCIES = ../lib/libgigatraj.a

test_MetGridSBRot_MPI_SOURCES = test_MetGridSBRot_PGrp.cc test_utils.cc
test_MetGridSBRot_MPI_DEPENDENCIES = ../lib/libgigatraj.a

test_MetMERRA_SOURCES = test_MetMERRA.cc test_utils.cc
test_MetMERRA_DEPENDENCIES = ../lib/libgigatraj.a

test_MetMERRA2_SOURCES = test_MetMERRA2.cc test_utils.cc
test_MetMERRA2_DEPENDENCIES = ../lib/libgigatraj.a

test_MetGEOSfpFcast_SOURCES = test_MetGEOSfpFcast.cc test_utils.cc
test_MetGEOSfpFcast_DEPENDENCIES = ../lib/libgigatraj.a

test_MetGEOSfpAssim_SOURCES = test_MetGEOSfpAssim.cc test_utils.cc
test_MetGEOSfpAssim_DEPENDENCIES = ../lib/libgigatraj.a

test_MetGEOSfp_SOURCES = test_MetGEOSfp.cc test_utils.cc
test_MetGEOSfp_DEPENDENCIES = ../lib/libgigatraj.a

test_MetSelector_SOURCES = test_MetSelector.cc test_utils.cc
test_MetSelector_DEPENDENCIES = ../lib/libgigatraj.a

test_Parcel_SOURCES = test_Parcel.cc test_utils.cc
test_Parcel_DEPENDENCIES = ../lib/libgigatraj.a

test_ChangeVertical_SOURCES = test_ChangeVertical.cc test_utils.cc
test_ChangeVertical_DEPENDENCIES = ../lib/libgigatraj.a

test_StreamDump_Load_SOURCES = test_StreamDump_Load.cc test_utils.cc
test_StreamDump_Load_DEPENDENCIES = ../lib/libgigatraj.a

test_StreamRead_SOURCES = test_StreamRead.cc test_utils.cc
test_StreamRead_DEPENDENCIES = ../lib/libgigatraj.a

test_StreamPrint_SOURCES = test_StreamPrint.cc test_utils.cc
test_StreamPrint_DEPENDENCIES = ../lib/libgigatraj.a

test_FlockSerial_SOURCES = test_FlockSerial.cc test_utils.cc
test_FlockSerial_DEPENDENCIES = ../lib/libgigatraj.a

test_SwarmSerial_SOURCES = test_SwarmSerial.cc test_utils.cc
test_SwarmSerial_DEPENDENCIES = ../lib/libgigatraj.a

test_FlockMPI_SOURCES = test_FlockMPI.cc test_utils.cc
test_FlockMPI_DEPENDENCIES = ../lib/libgigatraj.a

test_SwarmMPI_SOURCES = test_SwarmMPI.cc test_utils.cc
test_SwarmMPI_DEPENDENCIES = ../lib/libgigatraj.a

test_StreamPrintMPI_SOURCES = test_StreamPrintMPI.cc test_utils.cc
test_StreamPrintMPI_DEPENDENCIES = ../lib/libgigatraj.a

test_StreamReadMPI_SOURCES = test_StreamReadMPI.cc test_utils.cc
test_StreamReadMPI_DEPENDENCIES = ../lib/libgigatraj.a

test_PGenRep_SOURCES = test_PGenRep.cc test_utils.cc
test_PGenRep_DEPENDENCIES = ../lib/libgigatraj.a

test_PGenGrid_SOURCES = test_PGenGrid.cc test_utils.cc
test_PGenGrid_DEPENDENCIES = ../lib/libgigatraj.a

test_PGenRnd_SOURCES = test_PGenRnd.cc test_utils.cc
test_PGenRnd_DEPENDENCIES = ../lib/libgigatraj.a

test_PGenRndDisc_SOURCES = test_PGenRndDisc.cc test_utils.cc
test_PGenRndDisc_DEPENDENCIES = ../lib/libgigatraj.a

test_PGenDisc_SOURCES = test_PGenDisc.cc test_utils.cc
test_PGenDisc_DEPENDENCIES = ../lib/libgigatraj.a

test_PGenFile_SOURCES = test_PGenFile.cc test_utils.cc
test_PGenFile_DEPENDENCIES = ../lib/libgigatraj.a

test_traj000_SOURCES = test_traj000.cc test_utils.cc
test_traj000_DEPENDENCIES = ../lib/libgigatraj.a

test_traj001_SOURCES = test_traj001.cc test_utils.cc
test_traj001_DEPENDENCIES = ../lib/libgigatraj.a

test_traj010_SOURCES = test_traj010.cc test_utils.cc
test_traj010_DEPENDENCIES = ../lib/libgigatraj.a


