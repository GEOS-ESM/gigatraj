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
#include "gigatraj/StreamDump.hh"
#include "gigatraj/StreamLoad.hh"

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
    ifstream *input;
    ofstream *output;
    istringstream sss;
    StreamDump *sd;
    StreamLoad *si;
    StreamLoad *ss;
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
    
    output = new ofstream("parcels_out.dat");
    sd = new StreamDump(*output);
    *(output) << "This is a dump of parcel states" << endl;
    try {
       sd->apply(p);
    } catch (...) {
       cerr << "Stream Dump failed " << endl;
       exit(1);  
    }  
    delete sd; 
    output->close();
    delete output;
    
    input = new ifstream("parcels_test00.dat");
    si = new StreamLoad(*input);
    input->getline(junk, SZ);
    try {
       si->apply(q);
    } catch (...) {
       cerr << "Stream Load failed " << endl;
       exit(1);  
    }   
    z = q.getZ();
    q.getPos( &lon, &lat );
    time = q.getTime();
    if ( mismatch( lat, 43.4568709) ) {
       cerr << "Bad restore from parcel file: lat " << lat << endl;
       exit(1);
    }
    if ( mismatch( lon, 30.2345768 ) ) {
       cerr << "Bad restore from parcel file: lon " << lon << endl;
       exit(1);
    }
    if ( mismatch(z,12.3457869 ) ) {
       cerr << "Bad restore from parcel file: z " << z << endl;
       exit(1);
    }
    if ( mismatch(time,234550.0) ) {
       cerr << "Bad restore from parcel file: time " << time << endl;
       exit(1);
    }
    delete si;
    input->close();
    delete input;
    
    
    // check binary 
    
    p.setPos( 120.3456, -20.34563 );
    p.setTime(4502340.0);
    p.setZ(7.4565435);
    
    output = new ofstream("parcels_out2.dat");
    sd = new StreamDump(*output, 1);
    try {
       //sd->apply(p);
       *(sd) << p;
    } catch (...) {
       cerr << "Binary Stream Dump failed " << endl;
       exit(1);  
    }  
    delete sd; 
    output->close();
    delete output;
    
    input = new ifstream("parcels_out2.dat");
    si = new StreamLoad( *input, 1);
    try {
       //si->apply(q);
       *(si) >> q;
    } catch (...) {
       cerr << "Binary Stream Load failed " << endl;
       exit(1);  
    }   
    z = q.getZ();
    q.getPos( &lon, &lat );
    time = q.getTime();
    if ( mismatch( lat, -20.34563) || mismatch( lon,120.3456  ) 
       || mismatch(z, 7.4565435) || mismatch(time,4502340.0) ) {
       cerr << "Bad restore from binary parcel dump" << endl;
       exit(1);
    }
    delete si;

}
