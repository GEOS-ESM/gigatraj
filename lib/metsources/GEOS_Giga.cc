#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cstdio>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonFieldSfc.hh"

using namespace gigatraj;


typedef void * OpaqueObject;

// Function prototypes
extern "C" {
   OpaqueObject initGigaGridField3d(int, int, int, void *, void *, void*);
//   void ExerciseObject1(OpaqueObject, int);
//   void ExerciseObject2(OpaqueObject);
//   void DeleteObject(OpaqueObject);
}

OpaqueObject initGigaGridField3d(int nlons, int nlats, int nzs, void * lons_, void* lats_, void* levels_) {

   float* lons   = (float*) lons_;
   float* lats   = (float*) lats_;
   float* levels = (float*) levels_;

   std::vector<real>  xlons;
   std::vector<real>  xlats;
   std::vector<real>  xpres;
   std::vector<real>  xlevs;
   std::vector<real>  xdata;

   GridLatLonField3D *s = new GridLatLonField3D();

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
     xdata.push_back(i*1.0);
   }

   s->load(xlons, xlats, xlevs, xdata);

   return (OpaqueObject) s;

}
