#ifndef GIGATRAJ_MetGEOSDistributedData_H
#define GIGATRAJ_MetGEOSDistributedData_H

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/HLatLonInterp.hh"
#include "gigatraj/Vinterp.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "mpi.h"

namespace gigatraj {

class MetGEOSDistributedData {

   public:

      MetGEOSDistributedData();
      MetGEOSDistributedData(
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

      ~MetGEOSDistributedData();

      MetGEOSDistributedData( const MetGEOSDistributedData& src );

      MetGEOSDistributedData& operator=(const MetGEOSDistributedData& src);

      void get_uvw( double time, real lon, real lat, real z, real *u, real *v, real *w);
      void get_uvw( double time, int n, real* lons, real* lats, real* zs, real *u, real *v, real *w);
      void getData( string quantity, double time, int n, real* lons, real* lats, real* zs, real* values, int flags=0 );
      void getData( string quantity, double time, int n, real* lons, real* lats, real* values, int flags=0 );

      std::string time2Cal( const double time, int format=-999 );
      int debug = 100;
   //protected:

      int comm;
      int my_rank;
      int npes;
      int nlons_global;
      int nlats_global;
      int nlevs_global;
      int nlons_local;
      int nlats_local;

      std::vector<std::vector<int>> CellToRank;
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
      static const gigatraj::CalGregorian cal;

      Vinterp *vin;
      HLatLonInterp *hin;
      float wfctr = 0.01; //  omega get scaled from Pa/s to hPa/s
      std::vector<float> npz;
      std::vector<float> xlats;
      std::vector<float> xlons;
      std::vector<float> xlevs;
      
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
