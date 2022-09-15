/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


// Tests the basic advance() method of an individual Parcel,
// using the gridded tilted-solid-body-rotation met source

#include <iostream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/MetGridSBRot.hh"
#include "gigatraj/Earth.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;

int main() 
{

    Parcel p;
    Earth e;
    MetGridSBRot *metsrc;
    real lon;
    real lat;
    real z;
    real time;
    real dt;
    real flon;
    real duration;
    double tt;
    int status;
    
    dt = 0.01 ; // 15-minute time step
    duration = 12.0; // run for 12 days of model time

    // 1x1 grid, max wind = 40 m/s, axis tilted 30 degrees
    metsrc = new MetGridSBRot( 1.0, 1.0, 40.0, 30.0 );

    // set the meteorological data source
    // (setting this for one parcel sets it for all parcels)
    p.setMet( *metsrc );

    
    ////////////  second trial: tilted solid-body rotation
    
    // set starting position of sample parcel
    p.setPos( 0.0, 0.0 );
    
    // run the parcel forward
    for ( time=0.0; time<=(duration+dt/2.0) ; time+=dt )
    {
        p.advance( dt );
       lon = p.getLon();
       lat = p.getLat();
       tt = p.getTime();
       //cout << time << ", " << lon << ", " << lat << endl;
       //std::cout << time << "+dt ->" << tt << ", " << lon << ", " << lat << std::endl;
    }
    lon = p.getLon();
    lat = p.getLat();
    //if ( mismatch( lon, 11.2758, 0.01 ) || mismatch(lat, -6.44135, 0.01 ) ) 
    if ( mismatch( lon, 11.5486, 0.01 ) || mismatch(lat, -6.59377, 0.01 ) ) 
    {
       cerr << "Bad tilted ending lon,lat: ( " << 0.0 << ", " << 0.0 << " )" <<  
           " ) --> ( " << lon << ", " << lat << " )" <<endl;
       exit(1);
    }  
    // now go backwards
    //std::cout << "starting backwards" << std::endl;
    for ( ; time>(0.0+dt/2.0) ; time-=dt )
    {
       p.advance( -dt );
       lon = p.getLon();
       lat = p.getLat();
       tt = p.getTime();
       //cout << time << ", " << lon << ", " << lat << endl;
       //std::cout << time << "-dt ->" << tt << ", " << lon << ", " << lat << std::endl;
    }
    lon = p.getLon();
    lat = p.getLat();
    if ( mismatch( lon, 0.0, 0.01 ) || mismatch(lat, 0.0, 0.01 ) ) 
    {
       cerr << "Bad tilted restarting lon,lat: ( " << 0.0 << ", " << 0.0 << " )" <<  
           " ) --> ( " << lon << ", " << lat << " )" <<endl;
       exit(1);
    }  

    exit(0);
}
