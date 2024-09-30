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




#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cstdio>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/GridFieldProfile.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::istringstream;
using std::operator<<;
using std::operator>>;

int main() 
{
    GridFieldProfile grid, grid2;
    GridFieldProfile const *gridy;
    std::vector<real> ozs, xzs;
    std::vector<real> odata, xdata;
    int i, j, idx;
    int n1, n2;
    real val, val2;
    int ival, ival2;
    double tyme;
    string str;
    int ia[3], ja[3];
    real da[3];
    GridFieldProfile::iterator looper;
    GridFieldProfile::const_iterator clooper;
    std::ofstream *outfile;
    std::ifstream *infile;
    GridFieldProfile *arr;
    real area;
    
    ozs.reserve(71);
    for (i=0; i<71; i++ ) {
       ozs.push_back(i*1.0);
    }   
    odata.reserve(71);
    for (i=0; i<71; i++ ) {
       odata.push_back( i/3.0 );
    }   

  
    // =========================  method status
    // should be invalid
    if ( grid.status() == 0 ) {
       cerr << " Uninitialized GridFieldProfile object returned 0 status." << endl;
       exit(1);    
    }
    // =========================  method clear
    grid.clear(); // should have no effect
    if ( grid.status() == 0 ) {
       cerr << " Cleared GridFieldProfile object returned 0 status." << endl;
       exit(1);    
    }

    // =========================  method set_quantity
    grid.set_quantity("air temperature");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(1) GridFieldProfile object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_units
    grid.set_units("K");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(2) GridFieldProfile object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_fillval
    grid.set_fillval(-1234.0);
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(3) GridFieldProfile object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_surface
    grid.set_profile("boston");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(4) GridFieldProfile object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_time
    grid.set_time( 3.14159, "Grue 34, year of the kumquat");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(5) GridFieldProfile object returned 0 status." << endl;
       exit(1);    
    }

    // =========================  method hasdata
    if ( grid.hasdata() ) {
       cerr << " Dataless GridFieldProfile object returned true status" << endl;
       exit(1);    
    }

    // =========================  method load-1
    grid.load( ozs, odata );
    // NOW it should be valid!
    if ( grid.status() != 0 ) {
       cerr << " Fully initialized GridFieldProfile object returned non-0 status: " << grid.status() << endl;
       exit(1);    
    }
    if ( ! grid.hasdata() ) {
       cerr << " Datafull GridFieldProfile object returned false status" << endl;
       exit(1);    
    }

    // =========================  method quantity
    // now check the loaded values
    str = grid.quantity(); 
    if ( str != "air temperature" ) {
       cerr << " bad quantity <<" << str << ">>"  << endl;
       exit(1);           
    }
    // =========================  method units
    str = grid.units(); 
    if ( str != "K" ) {
       cerr << " bad units <<" << str << ">>" << endl;
       exit(1);           
    }
    // =========================  method surface
    str = grid.profile(); 
    if ( str != "boston" ) {
       cerr << " bad profile name <<" << str << ">>" << endl;
       exit(1);           
    }
    // =========================  method fillval
    val = grid.fillval(); 
    if ( mismatch( val, -1234.0 ) ) {
       cerr << " bad fill value :" << val  << endl;
       exit(1);           
    }
    // =========================  method time
    tyme = grid.time(); 
    if ( mismatch( tyme, 3.14159 ) ) {
       cerr << " bad time value :" << tyme  << endl;
       exit(1);           
    }
    // =========================  method met_time
    str = grid.met_time(); 
    if ( str != "Grue 34, year of the kumquat" ) {
       cerr << " bad calendar time <<" << str << ">>"  << endl;
       exit(1);           
    }
    // =========================  type identification
    str = grid.id(); 
    if ( str != "GridFieldProfile" ) {
       cerr << " bad gridID <<" << str << ">>"  << endl;
       exit(1);           
    }
    
    // =========================  method dataSize
    if ( grid.dataSize() != 71 ) {
       cerr << " bad dataSize : " << grid.dataSize()   << endl;
       exit(1);               
    }
    
    // ========================= method set_cacheable
    // ========================= method clear_cacheable
    // ========================= method cacheable
    grid.set_cacheable();
    if ( ! grid.cacheable() ) {
       cerr << "Grid set to cacheable but says it is not cacheable" << endl;
       exit(1);
    }
    grid.clear_cacheable();
    if ( grid.cacheable() ) {
       cerr << "Grid set to UNcacheable but says it is cacheable" << endl;
       exit(1);
    }    
    
    // =========================  method set_attribute
    grid.set_attribute("first", "1111");
    grid.set_attribute("second", "2222");
    
    // =========================  method attribute
    if ( grid.attribute("first") != "1111" ) {
       cerr << "mismatched 1st attribute: " <<  grid.attribute("first") << " vs 1111" << endl;
       exit(1);
    }    
    if ( grid.attribute("second") != "2222" ) {
       cerr << "mismatched 2nd attribute: " <<  grid.attribute("second") << " vs 2222" << endl;
       exit(1);
    }    





    
    if ( grid2.status() == 0 ) {
       cerr << " Uninitialized second GridFieldProfile object returned 0 status: " << grid2.status() << endl;
       exit(1);    
    }
    // =========================  method (assignment operator)
    grid2 = grid;
    if ( grid2.status() != 0 ) {
       cerr << " Copied GridFieldProfile object returned non-0 status: " << grid2.status() << endl;
       exit(1);    
    }
    
    // =========================  method dims
    grid2.dims( &n1 );
    if ( n1 != 71  ) {
       cerr << " Bad dimension: " << n1  <<  endl;
       exit(1);
    }

    // =========================  method levels
    xzs = grid2.levels();
    for ( i=0; i<n1; i++ ) {
       if ( mismatch( ozs[i], xzs[i] ) ) {
          cerr << " Mismatched lon " << i << ": " << ozs[i] << " vs. " << xzs[i] << endl;
          exit(1);
       }   
    }
    // =========================  method level
    for ( i=0; i<n1; i++ ) {
       val = grid2.level(i);
       if ( mismatch( ozs[i], val ) ) {
          cerr << " Mismatched Lon " << i << ": " << ozs[i] << " vs. " << val << endl;
          exit(1);
       }   
    }
        
    idx = -1;
    for ( i=0; i<n1; i++ ) {
       idx++;
       // =========================  method value
       val = grid.value(i);
       if ( mismatch(val, odata[idx] ) ) {
          cerr << " Mismatched data value (" << i << ", " << j << ") : "
              << odata[idx] << " vs. " << val << endl;
          exit(1);
       }
       // =========================  method operator()
       val = grid(i);
       if ( mismatch(val, odata[idx] ) ) {
          cerr << " Mismatched Data Value (" << i << ", " << j << ") : "
              << odata[idx] << " vs. " << val << endl;
          exit(1);
       }
    }
    
    if ( ! grid.match(grid2) ) {
       cerr << "grid and grid2 do not match" << endl;
       exit(1);
    }
       
    //==========================  method flushData
    grid.flushData();
    if ( grid.hasdata() ) {
       cerr << "grid still has data after flush" << endl;
       exit(1);
    }
    //==========================  method load-3
    grid.load( odata );
    if ( ! grid.match(grid2) || grid(3) != grid2(3) ) {
       cerr << "grid and grid2 do not match after reloading" << endl;
       exit(1);
    }

    // =========================  method &operator()
    grid(20) = -8765.0;
    val = grid.value(20);
    if ( mismatch( val, -8765.0) ) {
       cerr << " Mismatched Assigned Value (" << 20 << ", " << 10 << ") : "
           << -8765.0 << " vs. " << val << endl;
       exit(1);
    }
    
    // =========================  method compatible
    // check comparisons
    if ( ! grid.compatible( grid2 ) ) {
       cerr << " grid copy is not compatible with original! " << endl;
       exit(1);
    }
    ozs[45] = ozs[45]-0.5;
    grid.load( ozs, odata );
    if ( grid.compatible( grid2 ) ) {
       cerr << " altered grid copy(1) is compatible with original! " << endl;
       exit(1);
    }
    if ( ! grid.compatible( grid2, METCOMPAT_TIME ) ) {
       cerr << " altered grid copy(2) is (t) incompatible with original! " << endl;
       exit(1);
    }
    grid.set_time( 4.4, "Grue 67");
    if ( grid.compatible( grid2 ) ) {
       cerr << " altered grid copy(3) is compatible with original! " << endl;
       exit(1);
    }
    // reset for next test
    grid = grid2;
    grid.set_profile( "Omaha" );
    if ( grid.compatible( grid2 ) ) {
       cerr << " altered grid copy(4) is compatible with original! " << endl;
       exit(1);
    }
    if ( ! grid.compatible( grid2, METCOMPAT_HORIZ | METCOMPAT_TIME ) ) {
       cerr << " altered grid copy(4) is (h-t) incompatible with original! " << endl;
       exit(1);
    }
    // reset for next test
    grid = grid2;
    grid.set_time( 4.4, "Grue 67");
    if ( grid.compatible( grid2 ) ) {
       cerr << " altered grid copy(5) is compatible with original! " << endl;
       exit(1);
    }
    if ( ! grid.compatible( grid2, METCOMPAT_HORIZ | METCOMPAT_VERT ) ) {
       cerr << " altered grid copy(4) is (h-v) incompatible with original! " << endl;
       exit(1);
    }
    
    
    // =========================  method load-2
    // reset for next test
    grid = grid2;
    grid.loaddim(ozs);
    if ( grid.hasdata() ) {
       cerr << " Dataless loaded GridFieldProfile object returned true status " << endl;
       exit(1);    
    }
    

    // =========================  method match
    // reset for next test
    grid = grid2;
    if ( ! grid.match(grid2) ) {
       cerr << " Grid copy does not match grid " << endl;
       exit(1);    
    }
    grid.set_quantity("boo");
    grid.set_units("oob");
    if ( grid.match(grid2) ) {
       cerr << " Altered grid matches original " << endl;
       exit(1);    
    }
    
    
    // =========================  method zindex
    // test indexing functions
    val = ozs[9]+0.5;
    grid.zindex( val, &ival, &ival2 ) ;
    if ( ival != 9 || ival2 != 10) {
        cerr << " zindex " << val << " -> (9,10) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    val = ozs[70];
    grid.zindex( val, &ival, &ival2 ) ;
    if ( ival != 69 || ival2 != 70 ) {
        cerr << " zindex " << val << " -> (69,70) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    val = 0.0;
    grid.zindex( val, &ival, &ival2 ) ;
    if ( ival != 0 || ival2 != 1 ) {
        cerr << " zindex " << val << " -> (0,1) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }

   
    if ( grid.status() != 0 ) {
       cerr << "Valid GridFieldProfile object returned non-zero status." << endl;
       exit(1);    
    }
    
    
    // =========================  method gridpoints
    ia[0] = 2;
    ia[1] = 32;
    ia[2] = 7;
    grid.gridpoints( 3, ia, da);
    if ( da[0] != grid( ia[0] ) 
     ||  da[1] != grid( ia[1] ) 
     ||  da[2] != grid( ia[2] ) ) {
       cerr << " gridpoints results do not match value results" << endl;
       exit(1);    
    
    }


    // =========================  method (copy constructor)
    GridFieldProfile grid3 = grid;
    if ( grid3.status() != 0 ) {
       cerr << " copy-constructed GridFieldProfile returned non-zero status: " << grid3.status() << endl;
       exit(1);    
    }
    grid3.dims( &n1 );
    if ( n1 != 71  ) {
       cerr << " Bad dimension: " << n1 << ", " << n2 <<  endl;
       exit(1);
    }
    if ( grid3.level(4) != grid.level(4) 
     ||  grid3(7) != grid(7) ) {
       cerr << " copy-constructed grid values do not match original " << endl;
       exit(1);    
    } 


    // =========================  method <<
    // dump 
    ostringstream os;
    try {
       os << grid;
    } catch (...) {
       cerr << "GridFieldProfile output failed " << endl;
       exit(1);  
    }   

    grid.clear(); 
    if ( grid.status() == 0 ) {
       cerr << " Formerly-valid but now cleared GridFieldProfile object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method >>
    // restore 
    string pstate = os.str();
    istringstream is( pstate );
    try {
       is >> grid;
    } catch (...) {
       cerr << "GridFieldProfile input failed: " << pstate << endl;
       exit(1);  
    }
    if ( ! grid3.match(grid) ) {
       cerr << "GridFieldProfile restored grid fails to match original " << endl;
       exit(1);    
    }   
    // now try it with a file
    outfile = new std::ofstream("test_GridFieldProfile.dat", std::ios::out | std::ios::binary | std::ios::trunc );
    *outfile << grid3;
    outfile->close();
    delete outfile;
    grid.clear();
    infile = new std::ifstream("test_GridFieldProfile.dat", std::ios::in | std::ios::binary );
    *infile >> grid;
    infile->close();
    delete infile;
    if ( ! grid3.match(grid) ) {
       cerr << "GridFieldProfile restored from file grid fails to match original " << endl;
       exit(1);    
    }
    (void) remove("test_GridFieldProfile.dat");


    
    // ======================= method serialize
    // ======================= method deserialize
    //gridx = &grid;
    ostringstream os2;
    grid.serialize(os2);
    string pstate2 = os2.str();
    istringstream is2( pstate2 );
    grid3.clear();
    grid3.deserialize(is2);
    if ( grid3.quantity() != grid.quantity()
     || grid3.value(4) != grid.value(4)
     ) {
       cerr << "GridFieldProfile* restored grid fails to match original " << endl;
       exit(1);    
    }   
    
    // ========================= iterator
    // ========================= begin()
    // ========================= end()
    // ========================= assign *
    for ( i=0, looper=grid.begin(); looper != grid.end(); i++, looper++ ) {
        //cerr << i << ": " << *looper << endl;
        val = *looper;
        if ( mismatch( val, odata[i] ) ) {
          cerr << "GridFieldProfile iterator returned wrong value: " << i << ": " << val << " vs. " << odata[i] << endl;
          exit(1);
        }
        *looper = 3.14159;
        if ( mismatch( *looper, 3.14159 ) ) {
          cerr << "GridFieldProfile iterator set wrong value: " << i << ": " << val << " vs. " << 3.14159 << endl;
          exit(1);
        }
        looper.assign( 2.71828 );
        if ( mismatch( *looper, 2.71828 ) ) {
          cerr << "GridFieldProfile iterator assigned wrong value:" << i << ": " << *looper << " vs. " << 2.71828 << endl;
          exit(1);
        }
        *looper = val;
    }    
    val = 123.0;
    looper = grid.begin();
    for ( i=1; i<=13; i++ ) {
       looper++;
    }
    // ========================= iterator.indices()
    looper.indices(&i);
    if ( i != 13  ) {
       cerr << "GridFieldProfile iterator.indices failed: ( " << i << " )"<< endl;
       exit(1);
    }
    val2 = *looper;
    *looper = val;
    if ( mismatch(grid(13), val) ) {
      cerr << "GridFieldProfile assignment to iterator failed: " << val << " vs. " << grid(13) << endl;
      exit(1);
    }
    *looper = val2;
    
    // ========================= const_iterator
    // ========================= begin() const
    // ========================= end() const
    gridy = &grid;
    for ( i=0, clooper=gridy->begin(); clooper != gridy->end(); i++, clooper++ ) {
        //cerr << i << ": " << *looper << endl;
        val = *clooper;
        if ( mismatch( val, gridy->value(i) ) ) {
          cerr << "GridFieldProfile const_iterator returned wrong value: " << i << ": " << val << " vs. " << i << endl;
          exit(1);
        }
    } 
    clooper = gridy->begin();
    for ( i=1; i<=13; i++ ) {
       clooper++;
    }
    // ========================= const_iterator.indices()
    clooper.indices(&i);
    if ( i != 13 ) {
       cerr << "GridFieldProfile const_iterator.indices failed: ( " << i <<  " )"<< endl;
       exit(1);
    }
    
       
    // =========================  method setPgroup (deferred to test_GridFieldProfile_serial,test_GridFieldProfile_MPI) 
    // =========================  method svr_start (deferred to test_GridFieldProfile_serial,test_GridFieldProfile_MPI) 
    // =========================  method svr_listen (deferred to test_GridFieldProfile_serial,test_GridFieldProfile_MPI) 
    // =========================  method svr_done (deferred to test_GridFieldProfile_serial,test_GridFieldProfile_MPI) 
    // =========================  method svr_send_vals (deferred to test_GridFieldProfile_serial,test_GridFieldProfile_MPI) 
    // =========================  method svr_send_meta (deferred to test_GridFieldProfile_serial,test_GridFieldProfile_MPI) 
    // =========================  method receive_meta (deferred to test_GridFieldProfile_serial,test_GridFieldProfile_MPI) 
      
    exit(0);

}
