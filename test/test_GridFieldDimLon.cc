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
#include "gigatraj/GridFieldDimLon.hh"

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
    GridFieldDimLon grid, grid2;
    GridFieldDimLon const *gridy;
    std::vector<real> ozs, xzs;
    std::vector<real> odata, xdata;
    int i, j, idx;
    int n1, n2;
    real val, val2, val0;
    int ival, ival2;
    double tyme;
    string str;
    int ia[3], ja[3];
    real da[3];
    GridFieldDimLon::iterator looper;
    GridFieldDimLon::const_iterator clooper;
    std::ofstream *outfile;
    std::ifstream *infile;
    GridFieldDimLon *arr;
    real area;
    
    odata.reserve(72);
    for (i=0; i<72; i++ ) {
       odata.push_back( i*5.0 );
    }   

  
    // =========================  method status
    // should be invalid
    if ( grid.status() == 0 ) {
       cerr << " Uninitialized GridFieldDimLon object returned 0 status." << endl;
       exit(1);    
    }
    // =========================  method clear
    grid.clear(); // should have no effect
    if ( grid.status() == 0 ) {
       cerr << " Cleared GridFieldDimLon object returned 0 status." << endl;
       exit(1);    
    }

    // =========================  method set_quantity
    grid.set_quantity("longitude");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(1) GridFieldDimLon object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_units
    grid.set_units("degrees_east");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(2) GridFieldDimLon object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method hasdata
    if ( grid.hasdata() ) {
       cerr << " Dataless GridFieldDimLon object returned true status" << endl;
       exit(1);    
    }

    // =========================  method load-1
    grid.load( odata );
    // NOW it should be valid!
    if ( grid.status() != 0 ) {
       cerr << " Fully initialized GridFieldDimLon object returned non-0 status: " << grid.status() << endl;
       exit(1);    
    }
    if ( ! grid.hasdata() ) {
       cerr << " Datafull GridFieldDimLon object returned false status" << endl;
       exit(1);    
    }

    // =========================  method quantity
    // now check the loaded values
    str = grid.quantity(); 
    if ( str != "longitude" ) {
       cerr << " bad quantity <<" << str << ">>"  << endl;
       exit(1);           
    }
    // =========================  method units
    str = grid.units(); 
    if ( str != "degrees_east" ) {
       cerr << " bad units <<" << str << ">>" << endl;
       exit(1);           
    }
    // =========================  type identification
    str = grid.id(); 
    if ( str != "GridFieldDimLon" ) {
       cerr << " bad gridID <<" << str << ">>"  << endl;
       exit(1);           
    }
    
    // =========================  method dataSize
    if ( grid.dataSize() != 72 ) {
       cerr << " bad dataSize : " << grid.size()   << endl;
       exit(1);               
    }
    if ( grid.size() != 72 ) {
       cerr << " bad size : " << grid.size()   << endl;
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
       cerr << " Uninitialized second GridFieldDimLon object returned 0 status: " << grid2.status() << endl;
       exit(1);    
    }
    // =========================  method (assignment operator)
    grid2 = grid;
    if ( grid2.status() != 0 ) {
       cerr << " Copied GridFieldDimLon object returned non-0 status: " << grid2.status() << endl;
       exit(1);    
    }
    
    // =========================  method dims
    n1 = grid2.size( );
    if ( n1 != 72  ) {
       cerr << " Bad dimension: " << n1  <<  endl;
       exit(1);
    }

        
    for ( i=0; i<n1; i++ ) {
       // =========================  method value
       val = grid.value(i);
       if ( mismatch(val, odata[i] ) ) {
          cerr << " Mismatched data value (" << i << ", " << j << ") : "
              << odata[i] << " vs. " << val << endl;
          exit(1);
       }
       // =========================  method operator()
       val = grid(i);
       if ( mismatch(val, odata[i] ) ) {
          cerr << " Mismatched Data Value (" << i << ", " << j << ") : "
              << odata[i] << " vs. " << val << endl;
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
    val0 = 20*5.0 + 0.1;
    grid.set_value(20, val0 );
    val = grid.value(20);
    if ( mismatch( val, val0) ) {
       cerr << " Mismatched Assigned Value (" << 20 << ") : "
           << val0 << " vs. " << val << endl;
       exit(1);
    }
    // reste the value
    grid.set_value(20, 20*5.0);
    

    // =========================  method compatible
    // check comparisons
    if ( ! grid.compatible( grid2 ) ) {
       cerr << " grid copy is not compatible with original! " << endl;
       exit(1);
    }
    ozs = odata;
    ozs[45] = ozs[45]-0.5;
    grid.load( ozs );
    if ( grid.compatible( grid2 ) ) {
       cerr << " altered grid copy(1) is compatible with original! " << endl;
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
    
    
    // =========================  method index
    // test indexing functions
    val = ozs[9]+0.5;
    grid.index( val, &ival, &ival2 ) ;
    if ( ival != 9 || ival2 != 10) {
        cerr << " zindex " << val << " -> (9,10) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    val = ozs[71];
    grid.index( val, &ival, &ival2 ) ;
    if ( ival != 70 || ival2 != 71 ) {
        cerr << " zindex " << val << " -> (70,71) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    val = 0.0;
    grid.index( val, &ival, &ival2 ) ;
    if ( ival != 0 || ival2 != 1 ) {
        cerr << " zindex " << val << " -> (0,1) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    val = ozs[71] + 0.5;
    grid.index( val, &ival, &ival2 ) ;
    if ( ival != 71 || ival2 != 72 ) {
        cerr << " zindex " << val << " -> (71,72) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    val = 0.0 - 0.1;
    grid.index( val, &ival, &ival2 ) ;
    if ( ival != n1-1 || ival2 != n1 ) {
        cerr << " zindex " << val << " -> (71,72) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }

   
    if ( grid.status() != 0 ) {
       cerr << "Valid GridFieldDimLon object returned non-zero status." << endl;
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
    GridFieldDimLon grid3 = grid;
    if ( grid3.status() != 0 ) {
       cerr << " copy-constructed GridFieldDimLon returned non-zero status: " << grid3.status() << endl;
       exit(1);    
    }
    n2 = grid.size();
    n1 = grid3.size();
    if ( n1 != n2  ) {
       cerr << " Bad dimension: " << n1 << ", " << n2 <<  endl;
       exit(1);
    }
    if ( grid3.value(4) != grid.value(4) 
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
       cerr << "GridFieldDimLon output failed " << endl;
       exit(1);  
    }   

    grid.clear(); 
    if ( grid.status() == 0 ) {
       cerr << " Formerly-valid but now cleared GridFieldDimLon object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method >>
    // restore 
    string pstate = os.str();
    istringstream is( pstate );
    try {
       is >> grid;
    } catch (...) {
       cerr << "GridFieldDimLon input failed: " << pstate << endl;
       exit(1);  
    }
    if ( ! grid3.match(grid) ) {
       cerr << "GridFieldDimLon restored grid fails to match original " << endl;
       exit(1);    
    }   
    // now try it with a file
    outfile = new std::ofstream("test_GridFieldDimLon.dat", std::ios::out | std::ios::binary | std::ios::trunc );
    *outfile << grid3;
    outfile->close();
    delete outfile;
    grid.clear();
    infile = new std::ifstream("test_GridFieldDimLon.dat", std::ios::in | std::ios::binary );
    *infile >> grid;
    infile->close();
    delete infile;
    if ( ! grid3.match(grid) ) {
       cerr << "GridFieldDimLon restored from file grid fails to match original " << endl;
       exit(1);    
    }
    (void) remove("test_GridFieldDimLon.dat");


    
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
       cerr << "GridFieldDimLon* restored grid fails to match original " << endl;
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
          cerr << "GridFieldDimLon iterator returned wrong value: " << i << ": " << val << " vs. " << odata[i] << endl;
          exit(1);
        }
        try {
           looper.assign( 13.14159 );
           if ( i != 2 ) {
              cerr << "GridFieldDimLon iterator should not have been able to set element " << i << " to:" << 3.14159 << endl;
              exit(1);       
           }
           if ( mismatch( *looper, 13.14159 ) ) {
             cerr << "GridFieldDimLon iterator set wrong value: " << i << ": " << val << " vs. " << 3.14159 << endl;
             exit(1);
           }
        } catch ( GridField::badNonmonotonic ) {
           if ( i == 2 ) {
              cerr << "GridFieldDimLon iterator failed to set element " << i << " to:" << 13.14159 << endl;
              exit(1);       
           }
        } catch (...) {
            cerr << "Unknown failed in seeting element " << i << " to " << 13.14159 << endl;
            exit(1);
        };   
    }  
    // restore the changed value
    grid.set_value(2, 10.0);  

    val = 13*5.0 + 0.25;
    looper = grid.begin();
    for ( i=1; i<=13; i++ ) {
       looper++;
    }
    // ========================= iterator.indices()
    looper.indices(&i);
    if ( i != 13  ) {
       cerr << "GridFieldDimLon iterator.indices failed: ( " << i << " )"<< endl;
       exit(1);
    }
    val2 = *looper;
    looper.assign(val);
    if ( mismatch(grid(13), val) ) {
      cerr << "GridFieldDimLon assignment to iterator failed: " << val << " vs. " << grid(13) << endl;
      exit(1);
    }
    looper.assign( val2 );
    
    // ========================= const_iterator
    // ========================= begin() const
    // ========================= end() const
    gridy = &grid;
    for ( i=0, clooper=gridy->begin(); clooper != gridy->end(); i++, clooper++ ) {
        //cerr << i << ": " << *looper << endl;
        val = *clooper;
        if ( mismatch( val, gridy->value(i) ) ) {
          cerr << "GridFieldDimLon const_iterator returned wrong value: " << i << ": " << val << " vs. " << i << endl;
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
       cerr << "GridFieldDimLon const_iterator.indices failed: ( " << i <<  " )"<< endl;
       exit(1);
    }


// add tests for decreasing longitudfes
// add tests for non-wrapping longitudes

       
    // =========================  method setPgroup (deferred to test_GridFieldDimLon_serial,test_GridFieldDimLon_MPI) 
    // =========================  method svr_start (deferred to test_GridFieldDimLon_serial,test_GridFieldDimLon_MPI) 
    // =========================  method svr_listen (deferred to test_GridFieldDimLon_serial,test_GridFieldDimLon_MPI) 
    // =========================  method svr_done (deferred to test_GridFieldDimLon_serial,test_GridFieldDimLon_MPI) 
    // =========================  method svr_send_vals (deferred to test_GridFieldDimLon_serial,test_GridFieldDimLon_MPI) 
    // =========================  method svr_send_meta (deferred to test_GridFieldDimLon_serial,test_GridFieldDimLon_MPI) 
    // =========================  method receive_meta (deferred to test_GridFieldDimLon_serial,test_GridFieldDimLon_MPI) 
      
    exit(0);

}
