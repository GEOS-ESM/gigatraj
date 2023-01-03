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

   void* initGigaGridDistributedData(int comm, int* ij, int ig, int jg,int lv, int il, int jl, int nzs, float* lons, float* lats, float* zs, char* ctime);

   void updateFields(MetGEOSDistributedData * s, char* ctime, float* u, float* v, float* w, float* p);
                             
   void rk4a_advance( MetGEOSDistributedData *metsrc, char* ctime, double dt, int n, float* lons, float* lats, float* levs);
   void test_Field3D(GridLatLonField3D*);
   void test_metData(MetGEOSDistributedData *, double, int, float*, float*, float*, float*, float*, float*);

}


void* initGigaGridLatLonField3D(int nlons, int nlats, int nzs, float * lons, float* lats, float* levels, char* name, char* units, char* ctime) {

   std::vector<real>  xlons;
   std::vector<real>  xlats;
   std::vector<real>  xlevs;
   std::vector<real>  xdata;

   GridLatLonField3D *s = new GridLatLonField3D();

   s->set_quantity(name);
   s->set_units(units);
   s->set_vertical("air_pressure");
   double time = calendar.day1900(ctime);
   s->set_time(time,ctime);

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

void* initGigaGridDistributedData(int comm, int* IJ_rank, int Ig, int Jg, int lv, int nlon_local, int nlat_local, int nzs,
                                  float * lons, float* lats, float* eta, char* ctime){ 

   MetGEOSDistributedData * s = new MetGEOSDistributedData(comm, IJ_rank, Ig, Jg, lv, nlon_local, nlat_local, nzs, lons, lats, eta, ctime);

   return ( void* ) s;

}

void updateFields(MetGEOSDistributedData * s, char* ctime, float* u, float* v, float* w, float* p){

  for (auto &p : {s->u0, s->v0, s->w0}) { delete p; }

  s->u0 = s->u1;
  s->v0 = s->v1;
  s->w0 = s->w1;

  std::vector<real> udata, vdata,wdata, pdata;
  int Nsize = s->nlats_local*s->nlons_local*s->nlevs_global;
  for (int i=0; i<Nsize; i++){
      udata.push_back(*(u+i));
      vdata.push_back(*(v+i));
      wdata.push_back(*(w+i)*s->wfctr);
      pdata.push_back(*(p+i)*s->wfctr);
   }

   GridLatLonField3D* raw_u = new GridLatLonField3D();
   GridLatLonField3D* raw_v = new GridLatLonField3D();
   GridLatLonField3D* raw_w = new GridLatLonField3D();
   GridLatLonField3D* raw_p = new GridLatLonField3D();
   double time = calendar.day1900(ctime);

   raw_u->set_quantity("U");
   raw_u->set_units("m/s");
   raw_u->set_vertical("air_pressure");
   raw_u->set_time(time,ctime);
   raw_u->load(s->xlons, s->xlats, s->xlevs,udata);

   raw_v->set_quantity("V");
   raw_v->set_units("m/s");
   raw_v->set_vertical("air_pressure");
   raw_v->set_time(time,ctime);
   raw_v->load(s->xlons, s->xlats, s->xlevs,vdata);

   raw_w->set_quantity("OMEGA");
   raw_w->set_units("hPa/s");
   raw_w->set_vertical("air_pressure");
   raw_w->set_time(time,ctime);
   raw_w->load(s->xlons, s->xlats, s->xlevs, wdata);

   raw_p->set_quantity("air_pressure");
   raw_p->set_units("hPa");
   raw_p->set_vertical("air_pressure");
   raw_p->set_time(time,ctime);
   raw_p->load(s->xlons, s->xlats, s->xlevs, pdata);

   GridLatLonField3D* vertical = dynamic_cast<GridLatLonField3D*>( s->vin->invert(s->npz, *raw_p));

   GridLatLonField3D* new_u = dynamic_cast<GridLatLonField3D*> (s->vin->reProfile(*raw_u, *vertical));
   GridLatLonField3D* new_v = dynamic_cast<GridLatLonField3D*> (s->vin->reProfile(*raw_v, *vertical));
   GridLatLonField3D* new_w = dynamic_cast<GridLatLonField3D*> (s->vin->reProfile(*raw_w, *vertical));

   s->u1 = new_u;
   s->v1 = new_v;
   s->w1 = new_w;
   for (auto &p : {raw_u, raw_v, raw_w, raw_p, vertical}) { delete p; }
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
