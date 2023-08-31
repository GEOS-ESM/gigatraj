#ifndef GIGATRAJ_MetGEOSDistributedCubedData_H
#define GIGATRAJ_MetGEOSDistributedCubedData_H

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/HLatLonInterp.hh"
#include "gigatraj/Vinterp.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/GridCubedSphereField3D.hh"
#include "gigatraj/MetGEOSDistributedData.hh"
#include "mpi.h"
// W.Ji note, this class should be combine with MeyGEOSDistributedData
namespace gigatraj {

class MetGEOSDistributedCubedData : public MetGEOSDistributedData {

   public:

      MetGEOSDistributedCubedData();
      MetGEOSDistributedCubedData(
        int comm
      , int* ItoRank
      , int Iglobal
      , int lev
      , int i1
      , int i2
      , int j1
      , int j2
      , int nzs
      , int nf
      , float* lons
      , float* lats
      , float* eta
      , char* ctime 
      );  

      ~MetGEOSDistributedCubedData();

      MetGEOSDistributedCubedData( const MetGEOSDistributedCubedData& src );

      MetGEOSDistributedCubedData& operator=(const MetGEOSDistributedCubedData& src);

      void get_uvw( double time, real lon, real lat, real z, real *u, real *v, real *w);
      void get_uvw( double time, int n, real* lons, real* lats, real* zs, real *u, real *v, real *w);
      void getData( string quantity, double time, int n, real* lons, real* lats, real* zs, real* values, int flags=0 );
      void getData( string quantity, double time, int n, real* lons, real* lats, real* values, int flags=0 );
      void updateField( char* ctime, float* u, float* v, float* w, float* p);
      void setData(char* ctime, char* quantity, float* values);
      void setSfcData(char* ctime, char* quantity, float* values);

      int nlons_global;
      int nlats_global;
      int nlevs_global;
      int nlons_local;
      int nlats_local;
      int nthFace;
      int ithStart;
      int jthStart;

      GridCubedSphereField3D *u0;
      GridCubedSphereField3D *v0;
      GridCubedSphereField3D *w0;
      GridCubedSphereField3D *u1;
      GridCubedSphereField3D *v1;
      GridCubedSphereField3D *w1;
      GridCubedSphereField3D *vertical;

      GridCubedSphereField3D  * other;
      GridCubedSphereFieldSfc * gridSfc;

   private:

};
}

#endif



/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/
