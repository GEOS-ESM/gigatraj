/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


/*!
     Test program for the Earth class
*/

#include <stdlib.h>     
#include <iostream>
#include <math.h>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Earth.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;

int main() 
{

    Earth e;
    real lon, lonw;
    real lat;
    real dist;
    int status;
    
    if ( e.id != string("Earth") )
    {
       cerr << "Bad ID" << endl;
       exit(1);
    }   
    
    // =========================  method wrap
    
    // simple case--should not wrap
    lonw = e.wrap(40.0);
    if ( mismatch(  lonw, 40.0)   ) 
    {
       cerr << "Bad wrap 40->" << lonw << endl;
       exit(1);
    }   
    // also should not wrap
    lonw = e.wrap(90.0);
    if ( mismatch(  lonw, 90.0)   ) 
    {
       cerr << "Bad wrap 90->" << lonw << endl;
       exit(1);
    }   
    // should wrap to 0
    lonw = e.wrap(360.0);
    if ( mismatch( lonw, 0.0) ) 
    {
       cerr << "Bad wrap 360.0 -> " << lonw << endl;
       exit(1);
    }   
#ifdef WRAP_180
    // should not wrap
    lonw = e.wrap(-40.0);
    if ( mismatch( lonw, -40.0)  ) 
    {
       cerr << "Bad wrap -40 -> " << lonw << endl;
       exit(1);
    }   
    // edge case; should not wrap
    lonw = e.wrap(-180.0);
    if ( mismatch( lonw, -180.0)  ) 
    {
       cerr << "Bad wrap -180 -> " << lonw << endl;
       exit(1);
    }   
    // should wrap
    lonw = e.wrap(270.0);
    if ( mismatch( lonw, -90.0)  ) 
    {
       cerr << "Bad wrap 270 -> " << lonw << endl;
       exit(1);
    } 
    // edge case; should wrap  
    lonw = e.wrap(180.0);
    if ( mismatch( lonw, -180.0)  ) 
    {
       cerr << "Bad wrap 180 -> " << lonw << endl;
       exit(1);
    }   
    // not-quite-edge case; should not wrap
    lonw = e.wrap(179.999);
    if ( mismatch( lonw, 179.999) ) 
    {
       cerr << "Bad wrap 179.999 -> " << lonw << endl;
       exit(1);
    }   
    
#else
    // should wrap
    lonw = e.wrap(-40.0);
    if ( mismatch( lonw, 320.0)  ) 
    {
       cerr << "Bad wrap -40 -> " << lonw << endl;
       exit(1);
    }   
    // should wrap
    lonw = e.wrap(-180.0);
    if ( mismatch( lonw, 180.0)  ) 
    {
       cerr << "Bad wrap -180 -> " << lonw << endl;
       exit(1);
    }   
    // should not wrap
    lonw = e.wrap(270.0);
    if ( mismatch( lonw, 270.0)  ) 
    {
       cerr << "Bad wrap 270 -> " << lonw << endl;
       exit(1);
    }   
    // should not wrap
    lonw = e.wrap(180.0);
    if ( mismatch( lonw, 180.0)  ) 
    {
       cerr << "Bad wrap 180 -> " << lonw << endl;
       exit(1);
    }   
    // not-quite-edge case; should not wrap
    lonw = e.wrap(359.999);
    if ( mismatch( lonw, 359.999) ) 
    {
       cerr << "Bad wrap 359.999 -> " << lonw << endl;
       exit(1);
    }   
#endif



    // ===========================  method checkpos
    // position OK
    try {
       e.checkpos( 30.0, 40.0 );
    } catch ( Earth::badlocation err) {
       cerr << "Bad location check (30.0, 40.0) " << endl;
       exit(1);  
    }   
    // edge case; position OK
    try {
       e.checkpos( 30.0, 90.0 );
    } catch ( Earth::badlocation err) {
       cerr << "Bad location check (30.0, 90.0) " << endl;
       exit(1);  
    }   
    // edge case; position OK
    try {
       e.checkpos( 30.0, -90.0 );
    } catch ( Earth::badlocation err) {
       cerr << "Bad location check (30.0, -90.0) " << endl;
       exit(1);  
    }   
    // lots of wrapping; postion OK
    try {
       e.checkpos( 800.0, -90.0 );
    } catch ( Earth::badlocation err) {
       cerr << "Bad location check (800.0, -90.0) " << endl;
       exit(1);  
    }   
    // lots of wrapping; position OK
    try {
       e.checkpos( -800.0, -90.0 );
    } catch ( Earth::badlocation err) {
       cerr << "Bad location check (-800.0, -90.0) " << endl;
       exit(1);  
    } 
    // No reason this position should be considered bad, but you never know...
    try {
       e.checkpos( 0.0, 0.0 );
    } catch ( Earth::badlocation err) {
       cerr << "Bad location check (0.0, 0.0) " << endl;
       exit(1);  
    } 


    // position validity tests using NaNs 
#ifdef USE_DOUBLE    
    lon = nan("bang");
    lat = nan("boom");
#else
    lon = nanf("bang");
    lat = nanf("boom");
#endif
    // longitude is a NaN
    status = 1;
    try {
       e.checkpos( lon, -30.0 );
    } catch ( Earth::badlocation err) {
       // we should ctach the exception, if all works well
       status = 0;
    } 
    if ( status ) {
       cerr << "Bad location check (NaN, -30.0) " << endl;
       exit(1);  
    }
    // latitude is a NaN
    status = 1;
    try {
       e.checkpos( 20.0, lat );
    } catch ( Earth::badlocation err) {
       // we should ctach the exception, if all works well
       status = 0;
    } 
    if ( status ) {
       cerr << "Bad location check (20.0, NaN) " << endl;
       exit(1);  
    }
    // both longitude and latitude are NaNs
    status = 1;
    try {
       e.checkpos( lon, lat );
    } catch ( Earth::badlocation err) {
       // we should ctach the exception, if all works well
       status = 0;
    } 
    if ( status ) {
       cerr << "Bad location check (NaN, NaN) " << endl;
       exit(1);  
    }
    // latitude is out of range
    status = 1;
    try {
       e.checkpos( 30.0, 95.0 );
    } catch ( Earth::badlocation err) {
       // we should ctach the exception, if all works well
       status = 0;
    } 
    if ( status ) {
       cerr << "Bad location check (30.0,95.0) " << endl;
       exit(1);  
    }
    // latitude is out of range
    status = 1;
    try {
       e.checkpos( 30.0, -95.0 );
    } catch ( Earth::badlocation err) {
       // we should ctach the exception, if all works well
       status = 0;
    } 
    if ( status ) {
       cerr << "Bad location check (30.0,-95.0) " << endl;
       exit(1);  
    }



    // =============================   method deltapos

    // simple case
    lon = 10.0;
    lat = 20.0;
    try {
       e.deltapos( &lon, &lat, 5.0, 10.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad increment check (10.0,20.0) -> (15,30): (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, 15.0) || mismatch(lat,30.0) ) {
       cerr << "Bad increment check (10.0,20.0) -> (15,30) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
#ifdef WRAP_180
    // increment across longitude wrapping point
    lon = 178.0;
    lat = 20.0;
    try {
       e.deltapos( &lon, &lat, 5.0, 10.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad increment check (178.0,20.0) -> (-177,20) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, -177.0) || mismatch(lat,30.0) ) {
       cerr << "Bad increment check (178.0,20.0) -> (-177,20) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
    // increment across North pole
    lon = 45.0;
    lat = 89.0;
    try {
       e.deltapos( &lon, &lat, 5.0, 3.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad increment check (45.0,89.0) -> (-130,88) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, -130.0) || mismatch(lat,88.0) ) {
       cerr << "Bad increment check (45.0,89.0) -> (-130,88) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
    // increment across South pole
    lon = 45.0;
    lat = -89.0;
    try {
       e.deltapos( &lon, &lat, 5.0, -3.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad increment check (45.0,-89.0) -> (-130,-88) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, -130.0) || mismatch(lat,-88.0) ) {
       cerr << "Bad increment check (45.0,-89.0) -> (-130,-88) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
#else
    // increment across longitude wrapping point
    lon = 358.0;
    lat = 20.0;
    try {
       e.deltapos( &lon, &lat, 5.0, 10.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad increment check (358.0,20.0) -> (3,20) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, 3.0) || mismatch(lat,30) ) {
       cerr << "Bad increment check (358.0,20.0) -> (3,20) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
    // increment across North pole
    lon = 45.0;
    lat = 89.0;
    try {
       e.deltapos( &lon, &lat, 5.0, 3.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad increment check (45.0,89.0) -> (230,88) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, 230.0) || mismatch(lat,88.0) ) {
       cerr << "Bad increment check (45.0,89.0) -> (230,88) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
    // increment across South pole
    lon = 45.0;
    lat = -89.0;
    try {
       e.deltapos( &lon, &lat, 5.0, -3.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad increment check (45.0,-89.0) -> (230,-88) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, 230.0) || mismatch(lat,-88.0) ) {
       cerr << "Bad increment check (45.0,-89.0) -> (230,-88) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
#endif


    // =============================   method deltaxy
    // simple equatorial case, along longitude
    lon = 10.0;
    lat = 0.0;
    try {
       e.deltaxy( &lon, &lat, 100.0, 0.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad xy increment check (10.0,0.0) -> (10.8993,0): (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, 10.8993) || mismatch(lat,0.0) ) {
       cerr << "Bad xy increment check (10.0,0.0) -> (10.8993,0) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
    // simple equatorial case, along latitude
    lon = 10.0;
    lat = 20.0;
    try {
       e.deltaxy( &lon, &lat, 0.0, 100.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad xy increment check (10.0,20.0) -> (10.0, 20.8993): (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, 10.0) || mismatch(lat,20.8993) ) {
       cerr << "Bad xy increment check (10.0,20.0) -> (10.0, 20.8993) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
    // simple non-equatorial case, along longitude
    lon = 40.0;
    lat = -20.0;
    try {
       e.deltaxy( &lon, &lat, 100.0, 0.0 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad xy increment check (40.0,-20.0) -> (40.9572,-20): (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, 40.9572) || mismatch(lat,-20.0) ) {
       cerr << "Bad xy increment check (40.0,-20.0) -> (40.9572,-20) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    } 
    // typical arbitrary mid-latitude case
    lon = 97.0;
    lat = -43.4;
    try {
       e.deltaxy( &lon, &lat, 56.0, -12.3 );
    } catch ( Earth::badincrement err) {
       cerr << "Bad xy increment check (97.0,-43.4) -> (97.6931,-43.5106): (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }
    if ( mismatch(lon, 97.6931) || mismatch(lat,-43.5106) ) {
       cerr << "Bad xy increment check (97.0,-43.4) -> (97.6931,-43.5106) : (" << lon << ", " << lat << ")" << endl;
       exit(1);  
    }




    // =============================   method distance

    dist = e.distance(40.0, 23.4, 57.0, 34.9);
    if ( mismatch(  dist, 2082.82 )   ) 
    {
       cerr << "Bad distance 1: 2082.82 ->" << dist << endl;
       exit(1);
    }   
    dist = e.distance( 0.0, 0.0, 180.0, 0.0);
    if ( mismatch(  dist, 20015.0 )   ) 
    {
       cerr << "Bad distance 2: 20015.0 ->" << dist << endl;
       exit(1);
    }   
    dist = e.distance( 40.0, 90.0, 40.0, -90.0);
    if ( mismatch(  dist, 20015.0 )   ) 
    {
       cerr << "Bad distance 3: 20015.0 ->" << dist << endl;
       exit(1);
    }   

    // =============================   method radius

    dist = e.radius();
    if ( mismatch(  dist, 6371.0 )   ) 
    {
       cerr << "Bad earth radius 6371.0 ->" << dist << endl;
       exit(1);
    }   


    // if we got this far, everything is OK
    exit(0);

}
    
