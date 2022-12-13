#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cstdio>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonFieldSfc.hh"
#include "gigatraj/MetGEOSDistributedData.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/Earth.hh"
#include "gigatraj/IntegRK4a_GEOS.hh"

using namespace gigatraj;

static Earth e;
const gigatraj::CalGregorian calendar;
gigatraj::IntegRK4a_GEOS rk4a;

// Functions to be call by GEOS Fortran
extern "C" {
   void* initGigaGridLatLonField3D(int, int, int, float *, float *, float*, char*, char *, char*);

   void* initGigaGridDistributedData(int, int*, int, int, GridLatLonField3D*,  GridLatLonField3D*,  GridLatLonField3D*, GridLatLonField3D*,  GridLatLonField3D*,  GridLatLonField3D*  );

   void updateFields(char*, float*, float*, float*, GridLatLonField3D*, GridLatLonField3D *,  GridLatLonField3D*, 
                             GridLatLonField3D*, GridLatLonField3D *,  GridLatLonField3D*);
                             
   void rk4a_advance( MetGEOSDistributedData *metsrc, char* time, double dt, int n, float* lons, float* lats, float* levs);
   void test_Field3D(GridLatLonField3D*);
   void test_metData(MetGEOSDistributedData *, double, int, float*, float*, float*, float*, float*, float*);

}


void* initGigaGridLatLonField3D(int nlons, int nlats, int nzs, float * lons, float* lats, float* levels, char* name, char* units, char* ctime) {

   std::vector<real>  xlons;
   std::vector<real>  xlats;
   std::vector<real>  xpres;
   std::vector<real>  xlevs;
   std::vector<real>  xdata;

   GridLatLonField3D *s = new GridLatLonField3D();

   s->set_quantity(name);
   s->set_units(units);
   s->set_vertical("air pressure");
   double time = calendar.day1900(ctime);
   s->set_time(time,ctime);
   std::cout<<"day1900 time" << time;

   std::cout << calendar.date1900(time,3) << std::endl;

   for (int i=0; i<nlons; i++) {
     xlons.push_back(*(lons+i));
   }
   for (int i=0; i<nlats; i++) {
     xlats.push_back(*(lats+i));
   }
   for (int i=0; i<nzs; i++) {
     xlevs.push_back(*(levels+i));
   }
   for (int i=0; i<nlons*nlats*nzs; i++) {
     xdata.push_back(0.0);
   }
   
   s->load(xlons, xlats, xlevs, xdata);

   return (void*) s;

}

void* initGigaGridDistributedData(int comm, int* IJ_rank, int Ig, int Jg,   
                                        GridLatLonField3D* u0,  GridLatLonField3D* v0,  GridLatLonField3D* w0,
                                        GridLatLonField3D* u1,  GridLatLonField3D* v1,  GridLatLonField3D* w1){

   MetGEOSDistributedData * s = new MetGEOSDistributedData(comm, IJ_rank, Ig, Jg, u0, v0, w0, u1, v1, w1);
   return ( void* ) s;

}

void updateFields( char* ctime,float* u, float* v, float* w, GridLatLonField3D* u0, GridLatLonField3D * v0,  GridLatLonField3D* w0,  
                   GridLatLonField3D* u1, GridLatLonField3D * v1,  GridLatLonField3D* w1 ){


  *u0 = *u1;
  *v0 = *v1;
  *w0 = *w1;

  std::vector<real> udata, vdata,wdata;
  int Nsize = u0->dataSize();
  for (int i=0; i<Nsize; i++){
      udata.push_back(*(u+i));
      vdata.push_back(*(v+i));
      wdata.push_back(*(w+i));
   }

   u1->load(udata);
   v1->load(vdata);
   w1->load(wdata);

   double time = calendar.day1900(ctime);

   u1->set_time(time,ctime);
   v1->set_time(time,ctime);
   w1->set_time(time,ctime);

}

void test_Field3D( GridLatLonField3D * field3d) {

    std::cout << field3d->quantity() << std::endl;
    std::cout << field3d->units()    << std::endl;
    std::cout << field3d->vertical() << std::endl;
    std::cout << field3d->time()     << std::endl;
}

void test_metData(MetGEOSDistributedData * metsrc, double time, int n, float* lons, float* lats, float* levs, float* u, float* v, float* w) {

  metsrc->get_uvw(time, n, lons, lats, levs, u, v, w);

}

void rk4a_advance( MetGEOSDistributedData *metsrc, char* ctime, double dt, int n, float* lons, float* lats, float* levs){

  int flags[n]={0};
  double time = calendar.day1900(ctime);
  rk4a.go(n, lons, lats, levs, flags, time, metsrc, &e, dt); 

}
