#ifndef GIGATRAJ_MetGEOSDistributedLatLonData_H
#define GIGATRAJ_MetGEOSDistributedLatLonData_H

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/HLatLonInterp.hh"
#include "gigatraj/Vinterp.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/MetGEOSDistributedData.hh"
#include "mpi.h"

namespace gigatraj {

class MetGEOSDistributedLatLonData : public MetGEOSDistributedData  {

   public:

      MetGEOSDistributedLatLonData();
      MetGEOSDistributedLatLonData(
        int comm
      , int* ItoRank
      , int Iglobal
      , int Jglobal
      , int lev
      , int nlon
      , int nlat
      , int nzs
      , float* lons
      , float* lats
      , float* eta
      , char* ctime 
      );  

      ~MetGEOSDistributedLatLonData();

      MetGEOSDistributedLatLonData( const MetGEOSDistributedLatLonData& src );

      MetGEOSDistributedLatLonData& operator=(const MetGEOSDistributedLatLonData& src);

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

      GridLatLonField3D *u0;
      GridLatLonField3D *v0;
      GridLatLonField3D *w0;
      GridLatLonField3D *u1;
      GridLatLonField3D *v1;
      GridLatLonField3D *w1;
      GridLatLonField3D *vertical;

      GridLatLonField3D  * other;
      GridLatLonFieldSfc * gridSfc;

       /// Object for dealing with Dates
      
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
