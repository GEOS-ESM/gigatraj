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
