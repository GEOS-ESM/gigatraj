#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cstdio>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetGEOSDistributedData.hh"
#include "gigatraj/MetGEOSDistributedLatLonData.hh"
#include "gigatraj/MetGEOSDistributedCubedData.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/Earth.hh"
#include "gigatraj/IntegRK4_GEOS.hh"

using namespace gigatraj;

static Earth e;
const gigatraj::CalGregorian calendar;
gigatraj::IntegRK4_GEOS RK4;

// Functions to be call by GEOS Fortran
extern "C" {

   void* initGigaGridDistributedLatLonData(int comm, int* ij, int ig, int jg,int lv, int il, int jl, int nzs, float* lons, float* lats, float* zs, char* ctime);
   void* initGigaGridDistributedCubedData(int comm, int* ij, int ig, int lv, int i1, int i2, int j1, int j2, int nzs, float* lons, float* lats, float* zs, char* ctime);

   void updateFields(MetGEOSDistributedData * metSrc, char* ctime, float* u, float* v, float* w, float* p);
   void setData     (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, float* raw_data);
   void getData     (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, int n, float* lons, float* lats, float* zs, float* values);
   void getData2d     (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, int n, float* lons, float* lats, float* values);
                             
   void RK4_advance( MetGEOSDistributedData *metsrc, char* ctime, double dt, int n, float* lons, float* lats, float* levs);
//   void test_Field3D(GridLatLonField3D*);
   void test_metData(MetGEOSDistributedData *, double, int, float*, float*, float*, float*, float*, float*);

}


void* initGigaGridDistributedLatLonData(int comm, int* IJ_rank, int Ig, int Jg, int lv, int nlon_local, int nlat_local, int nzs,
                                  float * lons, float* lats, float* eta, char* ctime){ 

   MetGEOSDistributedLatLonData *metSrc = new MetGEOSDistributedLatLonData(comm, IJ_rank, Ig, Jg, lv, nlon_local, nlat_local, nzs, lons, lats, eta, ctime);

   return ( void* ) metSrc;

}

void* initGigaGridDistributedCubedData(int comm, int* IJ_rank, int Ig, int lv, int i1, int i2, int j1, int j2, int nzs,
                                  float * lons, float* lats, float* eta, char* ctime){ 
   // convert to zero index 
   int i01 = i1 - 1;
   int i02 = i2 - 1;
   int j01 = j1 - 1;
   int j02 = j2 - 1;
   int nf  = j01/Ig + 1;  // nface start from 1
   // re adjust j
   j01 = j01 - Ig*(nf-1);
   j02 = j02 - Ig*(nf-1);

   MetGEOSDistributedCubedData *metSrc = new MetGEOSDistributedCubedData(comm, IJ_rank, Ig, lv, i01, i02, j01, j02,nzs, nf, lons, lats, eta, ctime);

   return ( void* ) metSrc;

}

void updateFields(MetGEOSDistributedData * metSrc, char* ctime, float* u, float* v, float* w, float* p){

  metSrc->updateField(ctime, u, v, w, p);

}


void setData(MetGEOSDistributedData * metSrc, char* ctime, char* quantity, float* values){

   metSrc->setData(ctime, quantity, values);
}

void setSfcData(MetGEOSDistributedData * metSrc, char* ctime, char* quantity, float* values){

   metSrc->setSfcData(ctime, quantity, values);
/*
   metSrc->gridSfc->set_quantity(quantity);
   double time = metSrc->cal2Time(ctime);
   metSrc->gridSfc->set_time(time,ctime);
   std::vector<real> data;
   int Nsize = metSrc->nlats_local*metSrc->nlons_local;
   for (int i=0; i<Nsize; i++){
      data.push_back(*(values+i));
    }

   metSrc->gridSfc->load(data);
*/
}


void test_metData(MetGEOSDistributedData * metsrc, double time, int n, float* lons, float* lats, float* levs, float* u, float* v, float* w) {

  metsrc->get_uvw(time, n, lons, lats, levs, u, v, w);
}

/*
void test_Field3D( GridLatLonField3D * field3d) {

    std::cout << field3d->quantity() << std::endl;
    std::cout << field3d->units()    << std::endl;
    std::cout << field3d->vertical() << std::endl;
    std::cout << field3d->time()     << std::endl;
}
*/
void RK4_advance( MetGEOSDistributedData *metSrc, char* ctime, double dt, int n, float* lons, float* lats, float* levs){

  int flags[n]={0};
  double time = metSrc->cal2Time(ctime);
  RK4.go(n, lons, lats, levs, flags, time, metSrc, &e, dt); 

}

void getData (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, int n, float* lons, float* lats, float* zs, float* values){

  double time = metSrc->cal2Time(ctime);
  metSrc->getData( std::string(quantity), time, n, lons, lats, zs, values);

}

void getData2d (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, int n, float* lons, float* lats, float* values){

  double time = metSrc->cal2Time(ctime);
  metSrc->getData( std::string(quantity), time, n, lons, lats, values);

}


