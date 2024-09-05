
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (NASA Ames Research Center, Code SG) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
*** 
***  Copyright (c) 2023 United States Government as represented by the Administrator of the National Aeronautics and Space Administration.  All Rights Reserved. 
*** 
*** Disclaimer:
*** No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS." 
*** Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS AGREEMENT. 
***  (Please see the NOSA_19110.pdf file for more information.) 
*** 
********************************************************************************/

#include "config.h"

#include "gigatraj/IntegRK4.hh"

using namespace gigatraj;

const std::string IntegRK4::id = "IntegRK4";

const real gigatraj::IntegRK4::NEARPOLE = 85.0;

IntegRK4 :: IntegRK4()
{
    // the default is to apply no conformal adjustment
    confml = 0;
}

void IntegRK4 :: go( real &lon, real &lat, real &z, double &t, MetData *metsrc, PlanetNav *nav, double dt0 )
{
    int flags;

    
    flags = 0;
    
    go( 1, &lon, &lat, &z, &flags, t, metsrc, nav, dt0 );
    

}

void IntegRK4 :: go( int n, real *lons, real *lats, real *zs, int *flags, double &t, MetData *metsrc, PlanetNav *nav, double dt0 )
{
    real lon, lat, z;
    // coefficients for the Runga-Kutte integration
    real k1u,k1v,k1w, k2u,k2v,k2w, k3u,k3v,k3w, k4u,k4v,k4w;
    // changes in longitude, latitude, and vertical cooridates, respectively
    real dx,dy,dz;
    // intermediate longitude and latitude variables
    real tmplon,tmplat,tmpz;
    // sine and cosine of the longitude, for tilting the winds near the poles 
    real cdlon,sdlon;
    // adjusted/"rotate" wind components
    real up,vp;
    // delta time in seconds
    double dt;
    // time at the end of a (possibly partial) step
    double xt;
    // delta-lons and delta-lats, delta-zs
    real dlon1, dlon2, dlon3, dlon4;
    real dlat1, dlat2, dlat3, dlat4;
    real dz1, dz2, dz3, dz4;
    std::string dyt;
    real r;
    real mg1, mg2;
    int debug;
    int i;
    int ii;
    int flag;
    const char *nanstr = "";
    int nuse;
    real thslon, thslat, nxtlon, nxtlat;
    int apprx;
    
    apprx = -1; // exact
    //apprx = 0; // approx
    //apprx = 1; // crude
    if ( apprx == 1 ) {
       nav->conformal(0);
    }
    
    debug = metsrc->dbug;
    //debug = 101;

    // get the planetary radius
    r = 6371.0;
    
    // convert days to seconds
    dt = dt0 * 86400.0;
    
    int*  const iused = new int[n];

    real* const plons = new real[n];
    real* const plats = new real[n];
    real* const pzs   = new real[n];

    real* const tmplons = new real[n];
    real* const tmplats = new real[n];
    real* const tmpzs = new real[n];
    
    real* const xhold = new real[n];
    real* const yhold = new real[n];
    real* const zhold = new real[n];

    real* const kus = new real[n];
    real* const kvs = new real[n];
    real* const kws = new real[n];

    // mark which parcels we are going to trace
    nuse = 0;
    ii = -1;
    for ( i=0; i<n; i++ ) {
        if ( flags[i] == 0 ) {

           nuse++;
           ii++;

           iused[ii] = i;

           plons[ii] = lons[i];
           plats[ii] = lats[i];
           pzs[ii]   = zs[i];
        }    
    }
    
    ////////// Stage 1: Use the t0 winds at the starting points to advance 1/2 time step
    
    // get the winds at the starting locations at the starting time for use in Stage 1
    metsrc->get_uvw( t, nuse, plons, plats, pzs, kus, kvs, kws);
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(t, 3);
       std::cerr << "     IntegRK4-vector @ (" << t << "/" << dyt << ", " << plons[0] << ", " << plats[0] 
       << ", " << pzs[0] << ", " << t << "): u1=" << kus[0] << ", v1=" << kvs[0] << std::endl;
    }
    for ( i=0; i<nuse; i++ ) {
        // init the positions 
        tmplons[i] = plons[i];
        tmplats[i] = plats[i];
        tmpzs[i]   = pzs[i];
        // store the winds
        xhold[i] = kus[i]/6.0;
        yhold[i] = kvs[i]/6.0;
        zhold[i] = kws[i]/6.0;
    }
    // move the parts forward 1/2 time step (and include the 1/1000 factor to convert m to km)  
    nav->deltaxy( nuse, tmplons, tmplats, kus, kvs, dt/2.0/1000.0, apprx );
    // move forward vertically as well
    for ( i=0; i<nuse; i++ ) {
        if ( FINITE(kws[i]) ) {
           dz1 = dt*kws[i]/1000.0;
           tmpzs[i] =  pzs[i] + dz1/2.0;
        } else {
           // bad data at this point, so no advancement
           dz1   = RNAN(nanstr);
           // and the parcel stays where it is
           tmpzs[i] = pzs[i];
        }
        
    }

    //// Stage 2: Use the (t0+dt/2) winds at the Stage-1 points to advance 1/2 time step
    
    // nget the winds at the first-stage intermediate points (tmplons,tmplats) 
    xt = t + dt0/2.0;
    metsrc->get_uvw( xt, nuse, tmplons, tmplats, tmpzs, kus, kvs, kws );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(xt, 3);
       std::cerr << "     IntegRK4-vector @ (" << xt << "/" << dyt << ", "  << tmplons[0] << ", " << tmplats[0] 
                 << ", " << tmpzs[0] << ", " << xt << "): u2=" << kus[0] << ", v2=" << kvs[0] << std::endl;
    }
    // conformally adjust these winds
    nav->vRelocate( nuse, tmplons, tmplats, plons, plats, kus, kvs, apprx );
    if ( debug >= 100 ) {
       std::cerr << "     IntegRK4-vector * (" << xt << "/" << dyt << ", "  << plons[0] << ", " << plats[0] 
                 << ", " << pzs[0] << ", " << xt << "): u2=" << kus[0] << ", v2=" << kvs[0] << std::endl;
    }

    ////////// initialize our stage-2 intermediate points to the starting points
    for ( i=0; i<nuse; i++ ) {
        tmplons[i] = plons[i];
        tmplats[i] = plats[i];
        tmpzs[i]   = pzs[i];
        // add to the winds
        xhold[i] = xhold[i] + kus[i]*2.0/6.0;
        yhold[i] = yhold[i] + kvs[i]*2.0/6.0;
        zhold[i] = zhold[i] + kws[i]*2.0/6.0;
    }
    // move the parts forward 1/2 time step  
    nav->deltaxy( nuse, tmplons, tmplats, kus, kvs, dt/2.0/1000.0, apprx );
    // move forward vertically as well
    for ( i=0; i<nuse; i++ ) {
        if ( FINITE(kws[i]) && FINITE(tmpzs[i]) ) {
           dz2 = dt*kws[i]/1000.0;
           tmpzs[i] =  pzs[i] + dz2/2.0;
        } else {
           // bad data at this point, so no advancement
           dz2   = RNAN(nanstr);
           // and the parcel stays where it is
           tmpzs[i] = pzs[i];
        }
        
    }

    ////// Stage 3:

    xt = t + dt0/2.0;
    metsrc->get_uvw( xt, nuse, tmplons, tmplats, tmpzs, kus, kvs, kws );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal( xt, 3);
       std::cerr << "     IntegRK4-vector @ (" << xt << "/" << dyt << ", "  << tmplons[0] << ", " << tmplats[0] 
                 << ", " << tmpzs[0] << ", " << xt << "): u3=" << kus[0] << ", v3=" << kvs[0] << std::endl;
    }
    // conformally adjust these winds
    nav->vRelocate( nuse, tmplons, tmplats, plons, plats, kus, kvs, apprx );
    if ( debug >= 100 ) {
       std::cerr << "     IntegRK4-vector * (" << xt << "/" << dyt << ", "  << plons[0] << ", " << plats[0] 
                 << ", " << pzs[0] << ", " << xt << "): u3=" << kus[0] << ", v3=" << kvs[0] << std::endl;
    }
    

    // initialize our stage-3 intermediate points to the starting points
    for ( i=0; i<nuse; i++ ) {
        tmplons[i] = plons[i];
        tmplats[i] = plats[i];
        tmpzs[i]   = pzs[i];
        // add to the winds
        xhold[i] = xhold[i] + kus[i]*2.0/6.0;
        yhold[i] = yhold[i] + kvs[i]*2.0/6.0;
        zhold[i] = zhold[i] + kws[i]*2.0/6.0;
    }
    // move the parts forward 1/2 time step  
    nav->deltaxy( nuse, tmplons, tmplats, kus, kvs, dt/2.0/1000.0, apprx );
    // move forward vertically as well
    for ( i=0; i<nuse; i++ ) {
        if ( FINITE(kws[i]) && FINITE(tmpzs[i]) ) {
           dz3 = dt*kws[i]/1000.0;
           tmpzs[i] =  pzs[i] + dz3/2.0;
        } else {
           // bad data at this point, so no advancement
           dz3   = RNAN(nanstr);
           // and the parcel stays where it is
           tmpzs[i] = pzs[i];
        }
        
    }

    // now get the winds at the fourth-stage intermediate points (tmplons,tmplats)
    xt = t + dt0;
    metsrc->get_uvw( xt , nuse, tmplons, tmplats, tmpzs, kus, kvs, kws );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal( xt , 3);
       std::cerr << "     IntegRK4-vector @ (" << xt << "/" << dyt << ", "  << tmplons[0] << ", " << tmplats[0] 
                 << ", " << tmpzs[0] << ", " << xt << "): u4=" << kus[0] << ", v4=" << kvs[0] << std::endl;
    }
    // conformally adjust these winds
    nav->vRelocate( nuse, tmplons, tmplats, plons, plats, kus, kvs, apprx );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal( xt , 3);
       std::cerr << "     IntegRK4-vector * (" << xt << "/" << dyt << ", "  << plons[0] << ", " << plats[0] 
                 << ", " << pzs[0] << ", " << xt << "): u4=" << kus[0] << ", v4=" << kvs[0] << std::endl;
    }


    for ( i=0; i<nuse; i++ ) {
        // and re-init the starting positions
        tmplons[i] = plons[i];
        tmplats[i] = plats[i];
        tmpzs[i]   = pzs[i];
        // add to the winds
        xhold[i] = xhold[i] + kus[i]/6.0;
        yhold[i] = yhold[i] + kvs[i]/6.0;
        zhold[i] = zhold[i] + kws[i]/6.0;
    }
    xt = t + dt0;
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal( xt , 3);
       std::cerr << "     IntegRK4-vector end (" << xt << "/" << dyt << ", "  << plons[0] << ", " << plats[0] 
                 << ", " << pzs[0] << ", " << xt << "): uu=" << xhold[0] << ", vv=" << yhold[0] << std::endl;
    }
    // move the parts forward 1 time step  
    nav->deltaxy( nuse, tmplons, tmplats, xhold, yhold, dt/1000.0, apprx );
    // move forward vertically as well
    for ( i=0; i<nuse; i++ ) {
        // advance in the vertical
        if ( FINITE(zhold[i]) && FINITE(tmpzs[i]) ) {
           dz1 = dt*zhold[i]/1000.0;
           tmpzs[i] =  pzs[i] + dz1;
        } else {
           // bad data at this point, so no advancement
           dz1   = RNAN(nanstr);
           // and the parcel stays where it is
           tmpzs[i] = pzs[i];
        }
    
        // and store the results
        ii = iused[i];        
        if ( FINITE(tmplons[i]) && FINITE(tmplats[i]) && FINITE(tmpzs[i]) ) {
           lons[ii] = tmplons[i];
           lats[ii] = tmplats[i];
           zs[ii]   = tmpzs[i];
        } else {
           // disable tracing this parcel 
           flags[ii] = 1;
        }   
    }
    if ( debug >= 100 ) {
       std::cerr << "     IntegRK4-vector result (" << xt << "/" << dyt << ", "  << tmplons[0] << ", " << tmplats[0] 
                 << ", " << tmpzs[0] << ", " << xt << ")" << std::endl;
    }
    
    delete []zhold;
    delete []yhold;
    delete []xhold;
    delete []tmpzs;
    delete []tmplats;
    delete []tmplons;
    delete []kws;
    delete []kvs;
    delete []kus;
    delete []pzs;
    delete []plats;
    delete []plons;
    delete []iused;


    // advance the time
    t += dt0;
    //std::cerr << "IntegRK4 END: time=" << t << ", dt0=" << dt0 << std::endl;


}
