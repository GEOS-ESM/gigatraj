
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"
#include <iomanip>
#include "math.h"
#include "mpi.h"
#include "gigatraj/BilinearHinterp.hh"
#include "gigatraj/LinearVinterp.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/MetGEOSDistributedData.hh"

using namespace gigatraj;

const gigatraj::CalGregorian MetGEOSDistributedData::cal;

// default constructor
MetGEOSDistributedData::MetGEOSDistributedData() 
{

}

MetGEOSDistributedData::MetGEOSDistributedData(
        int mpicomm
      , int* IJtoRank
      , int Iglobal
      , int Jglobal
      , int lv
      , int nlon
      , int nlat
      , int nzs
      , float* lons
      , float* lats
      , float* eta
      , char* ctime
      )  
{

  comm = mpicomm;
  MPI_Comm_size(comm, &npes);
  MPI_Comm_rank(comm, &my_rank);
  nlons_global = Iglobal;
  nlats_global = Jglobal;
  nlevs_global = lv;

  for (int i=0; i<lv; i++) {
     xlevs.push_back(float(i));
  }

  CellToRank.resize(Jglobal,std::vector<int>(Iglobal));
  int k = 0;
  for (int j=0; j<Jglobal; j++){
    for (int i=0; i<Iglobal; i++){
      CellToRank[j][i] = IJtoRank[k];
      k++;
    }
  }

  vin = new LinearVinterp();
  hin = new BilinearHinterp();

  nlons_local = nlon;
  nlats_local = nlat;
 
  std::vector<real>  xdata;

  double time = cal.day1900(ctime);

  
  u0= new GridLatLonField3D();
  v0= new GridLatLonField3D();
  w0= new GridLatLonField3D();
  u1= new GridLatLonField3D();
  v1= new GridLatLonField3D();
  w1= new GridLatLonField3D();
  vertical = new GridLatLonField3D();
  other    = new GridLatLonField3D();

  u0->set_quantity("U");
  u0->set_units("m/s");
  u0->set_vertical("air_pressure");
  u0->set_time(time,ctime);
  u1->set_quantity("U");
  u1->set_units("m/s");
  u1->set_vertical("air_pressure");
  u1->set_time(time,ctime);
  v0->set_quantity("V");
  v0->set_units("m/s");
  v0->set_vertical("air_pressure");
  v0->set_time(time,ctime);
  v1->set_quantity("V");
  v1->set_units("m/s");
  v1->set_vertical("air_pressure");
  v1->set_time(time,ctime);
  w0->set_quantity("OMEGA");
  w0->set_units("hPa/s");
  w0->set_vertical("air_pressure");
  w0->set_time(time,ctime);
  w1->set_quantity("OMEGA");
  w1->set_units("hPa/s");
  w1->set_vertical("air_pressure");
  w1->set_time(time,ctime);

  for (int i=0; i<nlons_local; i++) {
     xlons.push_back(*(lons+i));
  }
  for (int i=0; i<nlats_local; i++) {
     xlats.push_back(*(lats+i));
  }
  for (int i=0; i<nzs; i++) {
     npz.push_back(*(eta+i));
  }
  for (int i=0; i<nlons_local*nlats_local*nzs; i++) {
     xdata.push_back(0.0);
  }

   u0->load(xlons, xlats, npz, xdata);
   u1->load(xlons, xlats, npz, xdata);
   v0->load(xlons, xlats, npz, xdata);
   v1->load(xlons, xlats, npz, xdata);
   w0->load(xlons, xlats, npz, xdata);
   w1->load(xlons, xlats, npz, xdata);
}

// destructor
MetGEOSDistributedData::~MetGEOSDistributedData() 
{
   //delete x3D;
   //delete xSfc;
}

// copy constructor
MetGEOSDistributedData::MetGEOSDistributedData( const MetGEOSDistributedData&  src) 
{

}    


MetGEOSDistributedData & MetGEOSDistributedData::operator=(const MetGEOSDistributedData& src)
{
}

std::string MetGEOSDistributedData::time2Cal( double time, int format )
{
    return cal.date1900(time, format );
}

void MetGEOSDistributedData::get_uvw( double time, real lon, real lat, real z, real *u, real *v, real *w){
}

void MetGEOSDistributedData::get_uvw( double time, int n, float* lons, float* lats, float* zs, float *u, float *v, float *w){
  //double pi = 3.1415926535;
  float dlon = 360.0 /nlons_global;
  float dlat = 180.0 /nlats_global;
  int II[n]{};

  for (int i=0; i<n; i++){
    II[i] = floor(lons[i]/dlon);
  }  
  int JJ[n]{};
  for (int i=0; i<n; i++){
    JJ[i] = floor((lats[i]+90.0)/dlat);
  }  

  int Ranks[n] {};
  for (int i=0; i<n; i++){
    Ranks[i] = CellToRank[JJ[i]][II[i]];
  }

  int counts_send[npes] = {0};
  int counts_recv[npes] = {0};

  for (int i=0; i<n; i++){
    counts_send[Ranks[i]]++;
  }

  MPI_Alltoall(counts_send, 1, MPI_INT, counts_recv, 1, MPI_INT, comm);

  int new_num = 0;
  for (int rank =0; rank<npes; rank++){
    new_num+=counts_recv[rank];
  }

  int disp_send[npes] = {0};
  int disp_recv[npes] = {0};

  for (int rank = 1; rank<npes; rank++){
    disp_send[rank] = disp_send[rank-1]+ counts_send[rank-1];
    disp_recv[rank] = disp_recv[rank-1]+ counts_recv[rank-1]; 
  }

  float lons_send[n] = {0.0};
  float lats_send[n] = {0.0};
  float levs_send[n] = {0.0};

  int tmp_position[npes];
  for (int i =0; i<npes; i++){
     tmp_position[i]= disp_send[i];
  }

  int pos[n] {};
  int rank;
  for (int i =0; i<n;i++){
    rank = Ranks[i];
    pos[i] = tmp_position[rank];
    lons_send[pos[i]] = lons[i];
    lats_send[pos[i]] = lats[i];
    levs_send[pos[i]] = zs[i];
    tmp_position[rank]++;
  }

  float new_lons[new_num];
  float new_lats[new_num];
  float new_levs[new_num];

  MPI_Alltoallv(lons_send, counts_send, disp_send, MPI_FLOAT, new_lons, counts_recv, disp_recv, MPI_FLOAT, comm);
  MPI_Alltoallv(lats_send, counts_send, disp_send, MPI_FLOAT, new_lats, counts_recv, disp_recv, MPI_FLOAT, comm);
  MPI_Alltoallv(levs_send, counts_send, disp_send, MPI_FLOAT, new_levs, counts_recv, disp_recv, MPI_FLOAT, comm);

  // At this point, the particles are distributed 
  double t1 = u0->time();
  double t2 = u1->time();

  real lonvals [new_num] = {0.0};
  real latvals [new_num] = {0.0};
  real  wvals  [new_num] = {0.0};
  real lonvals1[new_num] = {0.0};
  real latvals1[new_num] = {0.0};
  real wvals1  [new_num] = {0.0};
  real lonvals2[new_num] = {0.0};
  real latvals2[new_num] = {0.0};
  real wvals2  [new_num] = {0.0};

  hin->vinterpVector( new_num, new_lons, new_lats, new_levs, lonvals1, latvals1, *u0, *v0, *vin );
  hin->vinterp      ( new_num, new_lons, new_lats, new_levs, wvals1, *w0, *vin );
  if (t1 < time) {
     hin->vinterpVector( new_num, new_lons, new_lats, new_levs, lonvals2, latvals2, *u1, *v1, *vin );
     hin->vinterp      ( new_num, new_lons, new_lats, new_levs, wvals2, *w1, *vin );
  } 
       
  for (int i = 0; i< new_num; i++){
     lonvals[i] = lonvals1[i]*(t2-time)/(t2-t1) + lonvals2[i]*(time-t1)/(t2-t1);
     latvals[i] = latvals1[i]*(t2-time)/(t2-t1) + latvals2[i]*(time-t1)/(t2-t1);
     wvals[i]   = wvals1[i]*(t2-time)/(t2-t1)   + wvals2[i]*(time-t1)/(t2-t1);
  }

 real U_recv[n];
 real V_recv[n];
 real W_recv[n];

 MPI_Alltoallv(lonvals, counts_recv, disp_recv, MPI_FLOAT, U_recv, counts_send, disp_send, MPI_FLOAT, comm);
 MPI_Alltoallv(latvals, counts_recv, disp_recv, MPI_FLOAT, V_recv, counts_send, disp_send, MPI_FLOAT, comm);
 MPI_Alltoallv(wvals,   counts_recv, disp_recv, MPI_FLOAT, W_recv, counts_send, disp_send, MPI_FLOAT, comm);

 // reorder
 for (int i = 0; i<n; i++){
    u[i] = U_recv[pos[i]];
    v[i] = V_recv[pos[i]];
    w[i] = W_recv[pos[i]];
 }

}

void MetGEOSDistributedData::getData( string quantity, double time, int n, real* lons, real* lats, real* zs, real* values, int flags ){
  //double pi = 3.1415926535;

  if( other->quantity() != quantity) {
   //more info here
  }
  if ( abs(other->time() - time) >=10.e-9){
   //more info here
  }

  float dlon = 360.0 /nlons_global;
  float dlat = 180.0 /nlats_global;
  int II[n]{};

  for (int i=0; i<n; i++){
    II[i] = floor(lons[i]/dlon);
  }  
  int JJ[n]{};
  for (int i=0; i<n; i++){
    JJ[i] = floor((lats[i]+90.0)/dlat);
  }  

  int Ranks[n] {};
  for (int i=0; i<n; i++){
    Ranks[i] = CellToRank[JJ[i]][II[i]];
  }

  int counts_send[npes] = {0};
  int counts_recv[npes] = {0};

  for (int i=0; i<n; i++){
    counts_send[Ranks[i]]++;
  }

  MPI_Alltoall(counts_send, 1, MPI_INT, counts_recv, 1, MPI_INT, comm);

  int new_num = 0;
  for (int rank =0; rank<npes; rank++){
    new_num+=counts_recv[rank];
  }

  int disp_send[npes] = {0};
  int disp_recv[npes] = {0};

  for (int rank = 1; rank<npes; rank++){
    disp_send[rank] = disp_send[rank-1]+ counts_send[rank-1];
    disp_recv[rank] = disp_recv[rank-1]+ counts_recv[rank-1]; 
  }

  float lons_send[n] = {0.0};
  float lats_send[n] = {0.0};
  float levs_send[n] = {0.0};

  int tmp_position[npes];
  for (int i =0; i<npes; i++){
     tmp_position[i]= disp_send[i];
  }

  int pos[n] {};
  int rank;
  for (int i =0; i<n;i++){
    rank = Ranks[i];
    pos[i] = tmp_position[rank];
    lons_send[pos[i]] = lons[i];
    lats_send[pos[i]] = lats[i];
    levs_send[pos[i]] = zs[i];
    tmp_position[rank]++;
  }

  float new_lons[new_num];
  float new_lats[new_num];
  float new_levs[new_num];

  MPI_Alltoallv(lons_send, counts_send, disp_send, MPI_FLOAT, new_lons, counts_recv, disp_recv, MPI_FLOAT, comm);
  MPI_Alltoallv(lats_send, counts_send, disp_send, MPI_FLOAT, new_lats, counts_recv, disp_recv, MPI_FLOAT, comm);
  MPI_Alltoallv(levs_send, counts_send, disp_send, MPI_FLOAT, new_levs, counts_recv, disp_recv, MPI_FLOAT, comm);

  // At this point, the particles are distributed 

  
  real  wvals  [new_num] = {0.0};

  hin->vinterp ( new_num, new_lons, new_lats, new_levs, wvals, *other, *vin );
       
  real W_recv[n];

  MPI_Alltoallv(wvals,   counts_recv, disp_recv, MPI_FLOAT, W_recv, counts_send, disp_send, MPI_FLOAT, comm);

 // reorder
 for (int i = 0; i<n; i++){
    values[i] = W_recv[pos[i]];
  }
}
