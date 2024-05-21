/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


// Unfinished

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/MPIGrp.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/Swarm.hh"
#include "gigatraj/NetcdfIn.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::cout;
using std::endl;
using std::string;


int main(int argc, char* argv[]) 
{
    NetcdfIn input;
    string fyle;
    string vert;
    string basetime;
    double t0;
    double tt;
    size_t np;
    Parcel p;
    Parcel* ps;
    std::vector<Parcel> vps;
    Flock *fps;
    real lat,lon,z;
    int nxt;
    int i;
    std::string datdir;
    std::string fname;
    MPIGrp *pgrp;
    int my_id;
    int nprocs;
    
    
    // create a process group (MPI, of course)
    pgrp = new MPIGrp(argc, argv);
    
    my_id = pgrp->id();
    nprocs = pgrp->size();
    
    
    //input.debug( 100 );
    
    datdir = datadir("srcdir"); 
    fname = datdir + "test_NetcdfIn_data_01.nc4";
    input.filename(fname);
    
    input.open();
    
    basetime = input.cal();
    if ( basetime != "2022-07-15T00:00:00" ) {
       cerr << "the basetime is " << basetime << " instead of 2022-07-15T00:00:00" << endl;
       exit(1);          
    }

    t0 = input.time();
    if ( t0 != 0.0 ) {
       cerr << "the parcel time is " << t0 << " instead of 0" << endl;
       exit(1);          
    }
    
    np = input.n_parcels();
    if ( np != 10 ) {
       cerr << "the number of parcels is " << np << " instead of 10" << endl;
       exit(1);          
    }
    
    vert = input.vertical();
    if ( vert != "pressure" ) {
       cerr << "the vertical coordinate is " << vert << " instead of 'pressure'" << endl;
       exit(1);              
    }
    
    
    // Now try a Flock
    input.reset();
    
    fps = new Flock(p, pgrp, np, 0);
    input.apply( *fps );
    i = 1;
    p = fps->get(i);
    p.getPos( &lon, &lat );
    z = p.getZ();
    if ( mismatch( lon, 109.981 ) 
      || mismatch( lat, 4.44444 )
      || mismatch( z, 70.0 ) ) {
      
       cerr << " misread vector parcel[" << i << "]: "
       << "( " << lon << ", " << lat << ", " << z << " ) "
       << " instead of ( 109.981, 4.44444, 70 )" << endl;
    
       exit(1);
      
    }   
    delete fps;
    
    
    input.close();


    /* Shut down MPI */
    pgrp->shutdown();

    exit(0);
}
