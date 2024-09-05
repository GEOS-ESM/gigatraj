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
#include "gigatraj/StreamPrint.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::cout;
using std::endl;
using std::ofstream;
using std::ostringstream;


int main() 
{

    Parcel p, q;
    real lon;
    real lat;
    real z;
    real time;
    int status;
    ostringstream ooo;
    StreamPrint *sp;
    static const int SZ = 500;
    char junk[SZ];
    std::string sss;
    std::string ccc;
    char cc;
    int i;
    std::vector<Parcel> ps;
    Flock flk;
    Swarm swm;
    Parcel *pp;
    
    // set a horizontal position
    p.setPos( 30.2345768, -43.4568709 );
    p.setTime(17.34);
    p.setZ(12.3457869);
    
    sp = new StreamPrint(ooo);
    
    // test single parcel w/ default format
    try {
       sp->apply(p);
    } catch (...) {
       cerr << "Stream Print failed " << endl;
       exit(1);  
    }   
    sss = ooo.str();
    ccc = "17.34:   30.235, -43.457,    12.346\n";
    if ( sss != ccc ) {
       cerr << "wrongly formatted output :<<" << sss << ">>" << endl;
       exit(1);  
    
    }

    // test single parcel with specified format
    ooo.str("");
    sp->format("___%2.3T_%3i:_%12.6t_%10v_%10.4o_%8.1a___");
    try {
       sp->apply(p);
    } catch (...) {
       cerr << "Stream Print 2 failed " << endl;
       exit(1);  
    }   
    sss = ooo.str();
    ccc = "___.34_  0:_   17.340000_    12.346_   30.2346_   -43.5___";
    if ( sss != ccc ) {
       cerr << "wrongly formatted output on spec:<<" << sss << ">>" << endl;
       exit(1);  
    
    }

    // test single parcel with met field format
    ooo.str("");
    sp->format("__%v__%{alt}m__%5.2{alt}m_");
    try {
       sp->apply(p);
    } catch (...) {
       cerr << "Stream Print 3 failed " << endl;
       exit(1);  
    }   
    sss = ooo.str();
    ccc = "__   12.346__   12.346__12.35_";
    if ( sss != ccc ) {
       cerr << "wrongly formatted output on m spec:<<" << sss << ">>" << endl;
       exit(1);  
    
    }


    // test vector of parcels
    ooo.str("");

    ps.push_back( p );

    p.setPos( -23.6745674, 63.34455 );
    p.setTime(5.45);
    p.setZ(11.234434);
    ps.push_back( p );
    

    p.setPos( 215.344, 13.3990 );
    p.setTime(7.85);
    p.setZ(10.3455);
    ps.push_back( p );
    sp->format("%3i: %12.6t, %8.3a, %9.3o, %8.3v\n");
    try {
       sp->apply(ps);
    } catch (...) {
       cerr << "Stream Print failed on vector" << endl;
       exit(1);  
    }
    sss = ooo.str();
    ccc = "  0:    17.340000,  -43.457,    30.235,   12.346\n  1:     5.450000,   63.345,   -23.675,   11.234\n  2:     7.850000,   13.399,   215.344,   10.345\n";
    if ( sss != ccc ) {
       cerr << "wrongly formatted output :<<" << sss << ">>" << endl;
       exit(1);  
    
    }
    
   
    // test Flock of parcels
    ooo.str("");

    flk.set( 0, ps[0] );
    for ( i=1; i< ps.size(); i++ ) {
        flk.add( ps[i] );
    }

    try {
       sp->apply(flk);
    } catch (...) {
       cerr << "Stream Print failed oj Flock" << endl;
       exit(1);  
    }
    sss = ooo.str();
    ccc = "  0:    17.340000,  -43.457,    30.235,   12.346\n  1:     5.450000,   63.345,   -23.675,   11.234\n  2:     7.850000,   13.399,   215.344,   10.345\n";
    if ( sss != ccc ) {
       cerr << "flock wrongly formatted output :<<" << sss << ">>" << endl;
       exit(1);  
    
    }
   
    // test Swarm of parcels
    ooo.str("");

    swm.set( 0, ps[0] );
    for ( i=1; i< ps.size(); i++ ) {
        swm.add( ps[i] );
    }

    try {
       sp->apply(swm);
    } catch (...) {
       cerr << "Stream Print failed oj Swarm" << endl;
       exit(1);  
    }
    sss = ooo.str();
    ccc = "  0:    17.340000,  -43.457,    30.235,   12.346\n  1:     5.450000,   63.345,   -23.675,   11.234\n  2:     7.850000,   13.399,   215.344,   10.345\n";
    if ( sss != ccc ) {
       cerr << "swarm wrongly formatted output :<<" << sss << ">>" << endl;
       exit(1);  
    
    }
    
    delete sp;

}
