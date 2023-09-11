/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include <stdlib.h>

#include "math.h"
#include "mpi.h"

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/MPIGrp.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/Swarm.hh"
#include "gigatraj/NetcdfOut.hh"
#include "gigatraj/NetcdfIn.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::cout;
using std::endl;


int main(int argc, char* argv[]) 
{

    Parcel p, q;
    real lon;
    real lat;
    real lat0;
    real lon0;
    real z;
    real z0;
    real baseZ;
    double time;
    double time0;
    double basetime;
    int status;
    NetcdfOut *out;
    static const int SZ = 500;
    char junk[SZ];
    std::string sss;
    std::string ccc;
    char cc;
    int i;
    int k;
    int np;
    Parcel *pa;
    std::vector<Parcel> pv;
    std::vector<Parcel> ps;
    Parcel *pp;
    std::string outfile;
    NetcdfIn *in;
    int ip, it;
    int ms,ms0;
    Flock* pf;
    Flock *flk, *flk2;
    Flock::iterator iter;
    Swarm* pw;
    Swarm *swm, *swm2;
    Swarm::iterator siter;
    MPIGrp *pgrp;
    string cmd;
    int dr;
    std::string fmt;
    double tagval;
    double dNaN;
    double tag0;
    int n;
    int my_id;
    int nprocs;
    
    // create a process group (MPI, of course)
    pgrp = new MPIGrp(argc, argv);
    
    my_id = pgrp->id();
    nprocs = pgrp->size();

    np = 104;
    
    pgrp->sync();

    outfile = "test_netcdfout_data_01.nc4";
    
    // create a Flock of n parcels
    flk = new Flock( p, pgrp, np, 0);

    // set up the output
    out = new NetcdfOut();
    out->filename( outfile );
    sss = "test traj file";
    out->contents( sss ) ;
    sss = "someone@somewhere";
    out->contact( sss ) ;
    ms0 = 20;
    out->maxSequence( ms0 );
    
    out->init( &p, flk->size() );

    // start writing
    out->open();
    basetime = 17.34;
    baseZ = 12.3457869;
    for ( it=0; it < 7; it++ ) {
        time0 = it*0.15;
        for ( iter = flk->begin(); iter != flk->end(); iter++ ) {
            ip = iter.index();
            if ( ip >= 0 ) {
               lat0 = 45.0 + (ip - np/2)*0.5 + time0/50.0;
               lon0 = COS( lat0/180*PI );
               z0 = baseZ + time0/100.0;
            
               iter->setTime( time0 );
               iter->setPos( lon0, lat0 );
               iter->setZ(z0);
               
               if ( it >= 3 && ip == 5 ) {
                  iter->setNoTrace();
               }
            
            }
        }
        out->apply( *flk );    
    }            
    out->close();
    pgrp->sync();

    // try reading it again
    in = new NetcdfIn();
    in->open( outfile );
    ps.clear();
    for ( int i=0; i < np; i++ ) {
        ps.push_back(p);
    }
    it = 0;
    time0 = it*0.15;
    in->apply(ps);
    in->close();
    for ( ip=0; ip<np; ip++ ) {
        if ( ip >= 0 ) {
           lat0 = 45.0 + (ip - np/2)*0.5 + time0/50.0;
           lon0 = COS( lat0/180*PI );
           z0 = baseZ + time0/100.0;
    
           ps[ip].getPos( &lon, &lat );
           z = ps[ip].getZ();
           time = ps[ip].getTime();
    
           if ( mismatch(lon0, lon) || mismatch(lat0,lat) 
             || mismatch( z0, z) || mismatch( time0, time) ) {
              cerr << "NetcdfOut failed to set Flock parcel[" << ip << ", " << it << "] position "
              << " lon " << lon << " instead of " << lon0 << ", "
              << ", lat " << lat << " instead of " << lat0 
              << ", z " << z << " instead of " << z0 
              << ", time " << time << " instead of " << time0
              << endl; 
              exit(1);              
    
           }
        }
    }
    in->at_end( true );
    in->open( outfile );
    ps.clear();
    for ( int i=0; i < np; i++ ) {
        ps.push_back(p);
    }
    it = 6;
    time0 = it*0.15;
    in->apply(ps);
    in->close();
    for ( ip=0; ip<np; ip++ ) {
        if ( ip >= 0 ) {
           lat0 = 45.0 + (ip - np/2)*0.5 + time0/50.0;
           lon0 = COS( lat0/180*PI );
           z0 = baseZ + time0/100.0;
    
           ps[ip].getPos( &lon, &lat );
           z = ps[ip].getZ();
           time = ps[ip].getTime();
    
           if ( ip != 5 ) {
              if ( mismatch(lon0, lon) || mismatch(lat0,lat) 
                || mismatch( z0, z) || mismatch( time0, time) ) {
                 cerr << "NetcdfOut failed to set Flock parcel[" << ip << ", " << it << "] position "
                 << " lon " << lon << " instead of " << lon0 << ", "
                 << ", lat " << lat << " instead of " << lat0 
                 << ", z " << z << " instead of " << z0 
                 << ", time " << time << " instead of " << time0
                 << endl; 
                 exit(1);              
    
              }
           } else {
              if ( ! ps[ip].queryNoTrace() ) {
                 cerr << "Trace set for untraced parcel " << ip << " at time " << time0 << endl;
                 exit(1);
              }   
           }
        }
    }

    delete in;
    delete flk;


    // create a Swarm of n parcels
    swm = new Swarm( p, pgrp, np, 0);

    // set up the output
    out = new NetcdfOut();
    out->filename( outfile );
    sss = "test traj file";
    out->contents( sss ) ;
    sss = "someone@somewhere";
    out->contact( sss ) ;
    ms0 = 20;
    out->maxSequence( ms0 );
    
    out->init( &p, swm->size() );

    // start writing
    out->open();
    basetime = 17.34;
    baseZ = 12.3457869;
    for ( it=0; it < 7; it++ ) {
        time0 = it*0.15;
        for ( ip=0; ip<np; ip++ ) {
            lat0 = 45.0 + (ip - np/2)*0.5 + time0/50.0;
            lon0 = COS( lat0/180*PI );
            z0 = baseZ + time0/100.0;
            p.setTime( time0 );
            p.setPos( lon0, lat0 );
            p.setZ(z0);
            
            swm->set( ip, p);
        }
        out->apply( *swm );    
    }            
    out->close();
    pgrp->sync();

    // try reading it again
    in = new NetcdfIn();
    in->open( outfile );
    ps.clear();
    for ( int i=0; i < np; i++ ) {
        ps.push_back(p);
    }
    in->apply(ps);
    in->close();
    delete in;
    it = 0;
    for ( int i=0; i<np; i++ ) {
        time0 = it*0.15;
        lat0 = 45.0 + (i - np/2)*0.5 + time0/50.0;
        lon0 = COS( lat0/180*PI );
        z0 = baseZ + time0/100.0;
        
        ps[i].getPos( &lon, &lat );
        z = ps[i].getZ();
        time = ps[i].getTime();
        
        if ( mismatch(lon0, lon) || mismatch(lat0,lat) 
          || mismatch( z0, z) || mismatch( time0, time) ) {
           cerr << "NetcdfOut failed to set Swarm parcel[" << i << "] position "
           << " lon " << lon << " instead of " << lon0 << ", "
           << ", lat " << lat << " instead of " << lat0 
           << ", z " << z << " instead of " << z0 
           << ", time " << time << " instead of " << time0
           << endl; 
           exit(1);              
        
        }
    }
    delete swm;

    /* Shut down MPI */
    pgrp->shutdown();

    delete out;

    // delete the netcdf file
    cmd = "/bin/rm -f " + outfile;
    (void) system(cmd.c_str());

    exit(0);
}
