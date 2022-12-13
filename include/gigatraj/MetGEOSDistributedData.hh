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
      , GridLatLonField3D *ufield0
      , GridLatLonField3D *vfield0
      , GridLatLonField3D *wfield0
      , GridLatLonField3D *ufield1
      , GridLatLonField3D *vfield1
      , GridLatLonField3D *wfield1
      );  

      ~MetGEOSDistributedData();

      MetGEOSDistributedData( const MetGEOSDistributedData& src );

      MetGEOSDistributedData& operator=(const MetGEOSDistributedData& src);

      void get_uvw( double time, real lon, real lat, real z, real *u, real *v, real *w);
      void get_uvw( double time, int n, real* lons, real* lats, real* zs, real *u, real *v, real *w);

      std::string time2Cal( const double time, int format=-999 );
      int debug = 100;
   protected:

      int comm;
      int my_rank;
      int npes;
      int nlons_global;
      int nlats_global;
      std::vector<std::vector<int>> CellToRank;
      GridLatLonField3D *ufield0;
      GridLatLonField3D *vfield0;
      GridLatLonField3D *wfield0;
      GridLatLonField3D *ufield1;
      GridLatLonField3D *vfield1;
      GridLatLonField3D *wfield1;
       /// Object for dealing with Dates
      static const gigatraj::CalGregorian cal;

      Vinterp *vin;
      HLatLonInterp *hin;

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
