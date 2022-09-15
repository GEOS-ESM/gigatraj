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
#include <fstream>
#include <sstream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/StreamRead.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;

int main() 
{

    Parcel p, q;
    real lon;
    real lat;
    real z;
    real time;
    double tag;
    int status;
    istringstream sss;
    StreamRead *ss;
    static const int SZ = 500;
    char junk[SZ];
    double dx;
    std::string dexpr;
    int spos;
    std::string fmt;
    
    
    // set a horizontal position
    p.setPos( 30.2345768, 43.4568709 );
    p.setTime(234550.0);
    p.setZ(12.3457869);
    
     ss = new StreamRead(sss);

     fmt = "hello %v";
     ss->format(fmt + "\n should be ignored");
     sss.clear();
     sss.seekg(0);
     sss.str("hello 12.5678");
     ss->apply(p);
     z = p.getZ();
     if ( mismatch( z, 12.5678) ) {
        cerr << "Bad custom fmt #1: z=" << z << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #1 = <<" << dexpr << ">>" << endl;
     }
     
     fmt = "foop %i %t %o %a %v %g %{T}m";
     ss->format(fmt);
     sss.clear();
     sss.seekg(0);
     sss.str("foop 478 10.66 45.67 -67.3 12.5678 230.67 100.0\n");
     ss->apply(p);
     z = p.getZ();
     tag = p.tag();
     time = p.getTime();
     p.getPos( &lon, &lat );
     if ( mismatch( time, 10.66) || mismatch( z, 12.5678) || mismatch( lon, 45.67) 
        || mismatch( lat, -67.3) || mismatch( tag, 230.67) ) {
        cerr << "Bad custom fmt #2: time,lon,lat,z,tag=" << time << ", " << lon
        << ", " << lat << ", " << z << ", " << tag << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #2 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     fmt = "foop %3i %5.2t %5.2o %5.1a %7.4v %6.2g %5.1{T}m";
     ss->format(fmt);
     sss.clear();
     sss.seekg(0);
     sss.str("foop 478 10.66 45.67 -67.3 12.5678 230.67 100.0\n");
     ss->apply(p);
     z = p.getZ();
     tag = p.tag();
     time = p.getTime();
     p.getPos( &lon, &lat );
     if ( mismatch( time, 10.66) || mismatch( z, 12.5678) || mismatch( lon, 45.67) 
        || mismatch( lat, -67.3) || mismatch( tag, 230.67) ) {
        cerr << "Bad custom fmt #3: time,lon,lat,z,tag=" << time << ", " << lon
        << ", " << lat << ", " << z << ", " << tag << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #3 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     fmt = "foop %3i%6t%6o%6a%8v%7g%6{A}m";
     ss->format(fmt);
     sss.clear();
     sss.seekg(0);
     sss.str("foop 478 10.66 45.67 -67.3 12.5678 230.67 100.0\n");
     ss->apply(p);
     z = p.getZ();
     tag = p.tag();
     time = p.getTime();
     p.getPos( &lon, &lat );
     if ( mismatch( time, 10.66) || mismatch( z, 12.5678) || mismatch( lon, 45.67) 
        || mismatch( lat, -67.3) || mismatch( tag, 230.67) ) {
        cerr << "Bad custom fmt #4: time,lon,lat,z,tag=" << time << ", " << lon
        << ", " << lat << ", " << z << ", " << tag << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #4 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     fmt = "%T";
     ss->format(fmt);
     sss.clear();
     sss.seekg(0);
     sss.str("12.24");
     ss->apply(p);
     time = p.getTime();
     if ( mismatch( time, 12.24 ) ) {
        cerr << "Bad custom fmt #5: time=" << time << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #5 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     fmt = "%2T";
     ss->format(fmt);
     sss.clear();
     sss.seekg(0);
     p.setTime(12.24);
     sss.str("03");
     ss->apply(p);
     time = p.getTime();
     if ( mismatch( time, 3.24 ) ) {
        cerr << "Bad custom fmt #6: time=" << time << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #6 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     fmt = "%3.1T";
     ss->format(fmt);
     sss.clear();
     sss.seekg(0);
     p.setTime(12.24);
     sss.str("3.5");
     ss->apply(p);
     time = p.getTime();
     if ( mismatch( time, 13.54 ) ) {
        cerr << "Bad custom fmt #7: time=" << time << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #7 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     fmt = "%-3.2T";
     ss->format(fmt);
     sss.clear();
     sss.seekg(0);
     p.setTime(12.24);
     sss.str("3.5");
     ss->apply(p);
     time = p.getTime();
     if ( mismatch( time, 3.524 ) ) {
        cerr << "Bad custom fmt #7.1: time=" << time << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #7.1 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     // test whitespace in fixed-width input

     fmt = "xx%8oxxyy%8ayy";
     ss->format(fmt);

     sss.clear();
     sss.seekg(0);
     sss.str("xx   -87.3xxyy   13.27yy");
     ss->apply(p);
     lon = p.getLon();
     lat = p.getLat();
     if ( mismatch( lon, -87.3) || mismatch( lat, 13.27) ) {
        cerr << "Bad custom fmt #8: lon=" <<  lon << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #8 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     sss.clear();
     sss.seekg(0);
     sss.str("xx-87.3   xxyy13.27   yy");
     ss->apply(p);
     lon = p.getLon();
     lat = p.getLat();
     if ( mismatch( lon, -87.3) || mismatch( lat, 13.27) ) {
        cerr << "Bad custom fmt #8a: lon=" <<  lon << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #8a = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     sss.clear();
     sss.seekg(0);
     sss.str("xx  -87.3 xxyy 13.27  yy");
     ss->apply(p);
     lon = p.getLon();
     lat = p.getLat();
     if ( mismatch( lon, -87.3) || mismatch( lat, 13.27) ) {
        cerr << "Bad custom fmt #8b: lon=" <<  lon << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #8b = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     // now try it with an alignment specifier (which should be ignored)
     fmt = "xx%-8oxxyy%-8ayy";
     ss->format(fmt);
     sss.clear();
     sss.seekg(0);
     sss.str("xx  -87.3 xxyy 13.27  yy");
     ss->apply(p);
     lon = p.getLon();
     lat = p.getLat();
     if ( mismatch( lon, -87.3) || mismatch( lat, 13.27) ) {
        cerr << "Bad custom fmt #9: lon=" <<  lon << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #9 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 

     fmt = "%c%8o%x%5a%c";
     ss->format(fmt);

     sss.clear();
     sss.seekg(0);
     sss.str("xx   -87.3xxyy   13.27yy");
     ss->apply(p);
     lon = p.getLon();
     lat = p.getLat();
     if ( mismatch( lon, -87.3) || mismatch( lat, 13.27) ) {
        cerr << "Bad custom fmt #10: lon=" <<  lon << endl;
        exit(1);
     } 
     dexpr = ss->format();
     if ( fmt != dexpr ) {
        cerr << "format #10 = <<" << dexpr << ">>" << endl;
        exit(1);
     } 
     

    delete ss;

}
