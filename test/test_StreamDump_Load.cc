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
    
    output = new ofstream( "parcels_out.dat");
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
    
    input = new ifstream(datadir("srcdir") + "parcels_test00.dat");
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
    
    input = new ifstream(datadir("srcdir") + "parcels_out2.dat");
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
    delete input;

}
