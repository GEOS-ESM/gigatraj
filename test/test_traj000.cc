/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


// Tests the basic advance() method of an individual Parcel

#include <iostream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/MetSBRot.hh"
#include "gigatraj/Earth.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;

int main() 
{

    Parcel p;
    Earth e;
    MetSBRot metsrc;
    real lon;
    real lat;
    real z;
    real time;
    real dt;
    real flon;
    real flat;
    real olon;
    real olat;
    real duration;
    double tt;
    int status;
    
    dt = 0.01 ; // 15-minute time step
    duration = 12.0; // run for 12 days of model time

    // set the meteorological data source
    // (setting this for one parcel sets it for all parcels)
    p.setMet( metsrc );
    

    ////////////  first trial: plain untilted solid-body rotation
    
    // set starting position of sample parcel
    olon = 0.0;
    olat = 0.0;
    p.setPos( olon, olat );
    
    // We start with a simple solid-body rotation, withOUT any tilted axis
    metsrc.set( 40.0, 0.0 );
    // run the parcel forward
    for ( time=0.0; time<=(duration+dt/2.0) ; time+=dt )
    {
       p.advance( dt );
       lon = p.getLon();
       lat = p.getLat();
       tt = p.getTime();
       //std::cout << time << "+dt ->" << tt << ", " << lon << ", " << lat << std::endl;
    }
    lon = p.getLon();
    lat = p.getLat();
    tt = p.getTime();
    // compute the final longitude
    //flon = olon;
    //flat = olat;
    //e.deltaxy( &flon, &flat, 40.0, 0.0, duration/1000.0, -1 );
    flon = 40.0 * ( time*86400.0) /1000.0 /(2.0*PI*e.radius())*360.0;
    while ( flon > 180.0 ) 
    {
       flon -= 360.0;
    }   
    if ( mismatch( lon, flon, 0.01 ) || mismatch(lat, flat, 0.01 ) ) 
    {
       std::cout << "Bad ending lon,lat: ( " << flon << ", " << 0.0 << " )" <<  
           " ) --> ( " << lon << ", " << lat << " )" <<endl;
       exit(1);
    }  
    
    // now go backwards
    for ( ; time>(0.0+dt/2.0) ; time-=dt )
    {
       p.advance( -dt );
       lon = p.getLon();
       lat = p.getLat();
       tt = p.getTime();
       //std::cout << time << "-dt ->" << tt << ", " << lon << ", " << lat << std::endl;
    }
    lon = p.getLon();
    lat = p.getLat();
    tt = p.getTime();
    if ( mismatch( lon, 0.0, 0.01 ) || mismatch(lat, 0.0, 0.01 ) ) 
    {
       cerr << "Bad restarting lon,lat: ( " << 0.01 << ", " << 0.01 << " )" <<  
           " ) --> ( " << lon << ", " << lat << " )" <<endl;
       exit(1);
    }  
    
    
    ////////////  second trial: tilted solid-body rotation
    
    // set starting position of sample parcel
    p.setPos( 0.0, 0.0 );
    
    // Tilt the axis this time, by 30 degrees
    metsrc.set( 40.0, 30.0 );
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
