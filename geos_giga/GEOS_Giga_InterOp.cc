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

   void* initGigaGridDistributedData(int comm, int* ij, int ig, int jg,int lv, int il, int jl, int nzs, float* lons, float* lats, float* zs, char* ctime);

   void updateFields(MetGEOSDistributedData * metSrc, char* ctime, float* u, float* v, float* w, float* p);
   void setData     (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, float* raw_data);
   void getData     (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, int n, float* lons, float* lats, float* zs, float* values);
   void getData2d     (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, int n, float* lons, float* lats, float* values);
                             
   void rk4a_advance( MetGEOSDistributedData *metsrc, char* ctime, double dt, int n, float* lons, float* lats, float* levs);
   void test_Field3D(GridLatLonField3D*);
   void test_metData(MetGEOSDistributedData *, double, int, float*, float*, float*, float*, float*, float*);

}


void* initGigaGridDistributedData(int comm, int* IJ_rank, int Ig, int Jg, int lv, int nlon_local, int nlat_local, int nzs,
                                  float * lons, float* lats, float* eta, char* ctime){ 

   MetGEOSDistributedData *metSrc = new MetGEOSDistributedData(comm, IJ_rank, Ig, Jg, lv, nlon_local, nlat_local, nzs, lons, lats, eta, ctime);

   return ( void* ) metSrc;

}

void updateFields(MetGEOSDistributedData * metSrc, char* ctime, float* u, float* v, float* w, float* p){

  for (auto &p : {metSrc->u0, metSrc->v0, metSrc->w0}) { delete p; }

  metSrc->u0 = metSrc->u1;
  metSrc->v0 = metSrc->v1;
  metSrc->w0 = metSrc->w1;

  std::vector<real> udata, vdata,wdata, pdata;
  int Nsize = metSrc->nlats_local*metSrc->nlons_local*metSrc->nlevs_global;
  for (int i=0; i<Nsize; i++){
      udata.push_back(*(u+i));
      vdata.push_back(*(v+i));
      wdata.push_back(*(w+i)*metSrc->wfctr);
      pdata.push_back(*(p+i)*metSrc->wfctr);
   }

   GridLatLonField3D* raw_u = new GridLatLonField3D();
   GridLatLonField3D* raw_v = new GridLatLonField3D();
   GridLatLonField3D* raw_w = new GridLatLonField3D();
   GridLatLonField3D* raw_p = new GridLatLonField3D();
   double time = metSrc->cal2Time(ctime);

   raw_u->set_quantity("U");
   raw_u->set_units("m/s");
   raw_u->set_vertical("air_pressure");
   raw_u->set_time(time,ctime);
   raw_u->load(metSrc->xlons, metSrc->xlats, metSrc->xlevs,udata);

   raw_v->set_quantity("V");
   raw_v->set_units("m/s");
   raw_v->set_vertical("air_pressure");
   raw_v->set_time(time,ctime);
   raw_v->load(metSrc->xlons, metSrc->xlats, metSrc->xlevs,vdata);

   raw_w->set_quantity("OMEGA");
   raw_w->set_units("hPa/s");
   raw_w->set_vertical("air_pressure");
   raw_w->set_time(time,ctime);
   raw_w->load(metSrc->xlons, metSrc->xlats, metSrc->xlevs, wdata);

   raw_p->set_quantity("air_pressure");
   raw_p->set_units("hPa");
   raw_p->set_vertical("air_pressure");
   raw_p->set_time(time,ctime);
   raw_p->load(metSrc->xlons, metSrc->xlats, metSrc->xlevs, pdata);

   GridLatLonField3D* vertical = dynamic_cast<GridLatLonField3D*>( metSrc->vin->invert(metSrc->npz, *raw_p));
   metSrc->u1 = dynamic_cast<GridLatLonField3D*> (metSrc->vin->reProfile(*raw_u, *vertical));
   metSrc->v1 = dynamic_cast<GridLatLonField3D*> (metSrc->vin->reProfile(*raw_v, *vertical));
   metSrc->w1 = dynamic_cast<GridLatLonField3D*> (metSrc->vin->reProfile(*raw_w, *vertical));
   
   for (auto &p : {raw_u, raw_v, raw_w, raw_p, metSrc->vertical}) { delete p; }
   metSrc->vertical = vertical;
}


void setData(MetGEOSDistributedData * metSrc, char* ctime, char* quantity, float* values){

   double time = metSrc->cal2Time(ctime);
   std::string dim="_2D";
   std::string qt = std::string(quantity);
   int nlevs = 1;
   if (qt.find(dim) == std::string::npos){
     nlevs = metSrc->nlevs_global;
   }
   int Nsize = metSrc->nlats_local*metSrc->nlons_local*nlevs;
   std::vector<real> data;
   for (int i=0; i<Nsize; i++){
     data.push_back(*(values+i));
   }
    
   if (qt.find(dim) == std::string::npos){
     GridLatLonField3D *raw_field = new GridLatLonField3D();
     raw_field->set_quantity(quantity);
     raw_field->set_vertical("air_pressure"); // not correct, not important for now
     raw_field->set_units("K");               // not coorect, not important for now
     raw_field->set_time(time,ctime);

     raw_field->load(metSrc->xlons, metSrc->xlats, metSrc->xlevs, data);
     delete(metSrc->other);
     metSrc->other =  dynamic_cast<GridLatLonField3D*> (metSrc->vin->reProfile(*raw_field, *(metSrc->vertical)));
     delete(raw_field);
   }else {
     metSrc->gridSfc->set_quantity(quantity);
     metSrc->gridSfc->set_units("K");               // not coorect, not important for now
     metSrc->gridSfc->set_time(time,ctime);
     metSrc->gridSfc->load(data);
   }
}

void setSfcData(MetGEOSDistributedData * metSrc, char* ctime, char* quantity, float* values){

   metSrc->gridSfc->set_quantity(quantity);
   double time = metSrc->cal2Time(ctime);
   metSrc->gridSfc->set_time(time,ctime);
   std::vector<real> data;
   int Nsize = metSrc->nlats_local*metSrc->nlons_local;
   for (int i=0; i<Nsize; i++){
      data.push_back(*(values+i));
    }

   metSrc->gridSfc->load(data);
}


void test_metData(MetGEOSDistributedData * metsrc, double time, int n, float* lons, float* lats, float* levs, float* u, float* v, float* w) {

  metsrc->get_uvw(time, n, lons, lats, levs, u, v, w);
}


void test_Field3D( GridLatLonField3D * field3d) {

    std::cout << field3d->quantity() << std::endl;
    std::cout << field3d->units()    << std::endl;
    std::cout << field3d->vertical() << std::endl;
    std::cout << field3d->time()     << std::endl;
}

void rk4a_advance( MetGEOSDistributedData *metSrc, char* ctime, double dt, int n, float* lons, float* lats, float* levs){

  int flags[n]={0};
  double time = metSrc->cal2Time(ctime);
  rk4a.go(n, lons, lats, levs, flags, time, metSrc, &e, dt); 

}

void getData (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, int n, float* lons, float* lats, float* zs, float* values){

  double time = metSrc->cal2Time(ctime);
  metSrc->getData( std::string(quantity), time, n, lons, lats, zs, values);

}

void getData2d (MetGEOSDistributedData * metSrc, char* ctime, char* quantity, int n, float* lons, float* lats, float* values){

  double time = metSrc->cal2Time(ctime);
  metSrc->getData( std::string(quantity), time, n, lons, lats, values);

}


