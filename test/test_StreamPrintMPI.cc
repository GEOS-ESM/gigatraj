/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


// Unfinished

#include <iostream>
#include <sstream>
#include <math.h>

#include "mpi.h"

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/MPIGrp.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/StreamPrint.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::istringstream;
using std::operator<<;
using std::operator>>;

int main(int argc, char* argv[]) 
{

    Parcel p;
    Parcel *pc;
    real lon;
    real lat;
    real z;
    real xlon, xlat, xz;
    int status;
    Flock *flk, *flk2;
    Flock::iterator iter;
    Swarm *swm, *swm2;
    Swarm::iterator siter;
    MPIGrp *pgrp;
    int k;
    int n;
    int my_id;
    int nprocs;
    StreamPrint *sp;
    ostringstream ooo;
    std::string sss;
    
    n = 104;
    
    // create a process group (MPI, of course)
    pgrp = new MPIGrp(argc, argv);
    
    my_id = pgrp->id();
    nprocs = pgrp->size();

    // create a Flock of n parcels
    flk = new Flock( p, pgrp, n, 0);
    pgrp->sync();

    // set the height and position of each parcel, 
    // using the set() method as is appropriate for multi-processing
    for ( k=0; k<n; k++ ) {
        
        if ( my_id == 0 ) {
           z = 300.0+k;
           lon = k*10.0;
           lat = 80.0 - k*1.0;
        } else {
           z = -999.0;
           lon = 0.0;
           lat = 0.0;
        }   
        p.setZ(z);
        p.setPos(lon,lat);

        flk->set(k, p);
        
    }
    pgrp->sync();


    // test parallelized printing,

    sp = new StreamPrint(ooo);

    try {
       sp->apply(*flk);
    } catch (...) {
       cerr << "Stream Print failed oj Flock" << endl;
       exit(1);  
    }

    sss = ooo.str();
    //cerr << "sss=<<" << sss << ">>" << endl;
    
    if ( flk->is_root() ) {
       if ( sss == "" ) {
          cerr << "Root output stream is null." << endl;
          exit(1);  
       }
    } else {
       if ( sss != "" ) {
          cerr << "Non-Root output stream is non-null." << endl;
          cerr << sss << endl;
          exit(1);  
       }
    
    }
    
    pgrp->sync();


    delete flk;


    // create a Swarm of n parcels
    swm = new Swarm( p, pgrp, n, 0);
    pgrp->sync();

    // set the height and position of each parcel, 
    // using the set() method as is appropriate for multi-processing
    for ( k=0; k<n; k++ ) {
        
        if ( my_id == 0 ) {
           z = 300.0+k;
           lon = k*10.0;
           lat = 80.0 - k*1.0;
        } else {
           z = -999.0;
           lon = 0.0;
           lat = 0.0;
        }   
        p.setZ(z);
        p.setPos(lon,lat);

        swm->set(k, p);
        
    }
    pgrp->sync();


    // test parallelized printing,

    sp = new StreamPrint(ooo);

    try {
       sp->apply(*swm);
    } catch (...) {
       cerr << "Stream Print failed oj Swarm" << endl;
       exit(1);  
    }

    sss = ooo.str();
    //cerr << "sss=<<" << sss << ">>" << endl;
    
    if ( swm->is_root() ) {
       if ( sss == "" ) {
          cerr << "Root output stream is null." << endl;
          exit(1);  
       }
    } else {
       if ( sss != "" ) {
          cerr << "Non-Root output stream is non-null." << endl;
          cerr << sss << endl;
          exit(1);  
       }
    
    }
    
    pgrp->sync();


    delete swm;

    /* Shut down MPI */
    //MPI_Finalize();
    pgrp->shutdown();

    // if we got this far, everything is OK
    return 0;

}
    
