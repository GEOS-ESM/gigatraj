#ifndef GIGATRAJ_MetGEOSDistributedData_H
#define GIGATRAJ_MetGEOSDistributedData_H

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/HLatLonInterp.hh"
#include "gigatraj/Vinterp.hh"
#include "gigatraj/CalGregorian.hh"
#include "mpi.h"

namespace gigatraj {

class MetGEOSDistributedData {

   public:

      MetGEOSDistributedData();
      ~MetGEOSDistributedData();

      MetGEOSDistributedData( const MetGEOSDistributedData& src );

      MetGEOSDistributedData& operator=(const MetGEOSDistributedData& src);

      virtual void get_uvw( double time, real lon, real lat, real z, real *u, real *v, real *w) =0;
      virtual void get_uvw( double time, int n, real* lons, real* lats, real* zs, real *u, real *v, real *w) =0;
      virtual void getData( string quantity, double time, int n, real* lons, real* lats, real* zs, real* values, int flags=0 ) =0;
      virtual void getData( string quantity, double time, int n, real* lons, real* lats, real* values, int flags=0 ) =0;
      virtual void updateField( char* ctime, float* u, float* v, float* w, float* p) =0;
      std::string time2Cal( const double time, int format=-999 );
      void defineCal(std::string caldate, double time);
      double cal2Time(std::string caldate);
      virtual void setData(char* ctime, char* quantity, float* values) =0;
      virtual void setSfcData(char* ctime, char* quantity, float* values)=0;

      int debug = 100;
   //protected:
      double basetime;
      MPI_Comm comm;
      int my_rank;
      int npes;

      std::vector<std::vector<int>> CellToRank;
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
