
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

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
      , GridLatLonField3D *ufield0_
      , GridLatLonField3D *vfield0_
      , GridLatLonField3D *wfield0_
      , GridLatLonField3D *ufield1_
      , GridLatLonField3D *vfield1_
      , GridLatLonField3D *wfield1_
      )  
{

  comm = mpicomm;
  MPI_Comm_size(comm, &npes);
  MPI_Comm_rank(comm, &my_rank);
  nlons_global = Iglobal;
  nlats_global = Jglobal;

  CellToRank.resize(Jglobal,std::vector<int>(Iglobal));
  int k = 0;
  for (int j=0; j<Jglobal; j++){
    for (int i=0; i<Iglobal; i++){
      CellToRank[j][i] = IJtoRank[k];
      k++;
    }
  }

 ufield0 = ufield0_;
 vfield0 = vfield0_;
 wfield0 = wfield0_;
 ufield1 = ufield1_;
 vfield1 = vfield1_;
 wfield1 = wfield1_;

 vin = new LinearVinterp();
 hin = new BilinearHinterp();

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

  // At this point, the particals are distributed 
  double t1 = ufield0->time();
  double t2 = ufield1->time();
  
  real lonvals [new_num] = {0.0};
  real latvals [new_num] = {0.0};
  real  wvals  [new_num] = {0.0};
  real lonvals1[new_num] = {0.0};
  real latvals1[new_num] = {0.0};
  real wvals1  [new_num] = {0.0};
  real lonvals2[new_num] = {0.0};
  real latvals2[new_num] = {0.0};
  real wvals2  [new_num] = {0.0};

  hin->vinterpVector( new_num, new_lons, new_lats, new_levs, lonvals1, latvals1, *ufield0, *vfield0, *vin );
  hin->vinterp      ( new_num, new_lons, new_lats, new_levs, wvals1, *wfield0, *vin );
  if (t1 < time) {
     hin->vinterpVector( new_num, new_lons, new_lats, new_levs, lonvals2, latvals2, *ufield1, *vfield1, *vin );
     hin->vinterp      ( new_num, new_lons, new_lats, new_levs, wvals2, *wfield1, *vin );
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
