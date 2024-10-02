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
#include "gigatraj/GridFieldDim.hh"

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
    GridFieldDim grid, grid2;
    GridFieldDim const *gridy;
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
    GridFieldDim::iterator looper;
    GridFieldDim::const_iterator clooper;
    std::ofstream *outfile;
    std::ifstream *infile;
    GridFieldDim *arr;
    real area;
    
    odata.reserve(71);
    for (i=0; i<71; i++ ) {
       odata.push_back( i );
    }   

  
    // =========================  method status
    // should be invalid
    if ( grid.status() == 0 ) {
       cerr << " Uninitialized GridFieldDim object returned 0 status." << endl;
       exit(1);    
    }
    // =========================  method clear
    grid.clear(); // should have no effect
    if ( grid.status() == 0 ) {
       cerr << " Cleared GridFieldDim object returned 0 status." << endl;
       exit(1);    
    }

    // =========================  method set_quantity
    grid.set_quantity("altitude");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(1) GridFieldDim object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_units
    grid.set_units("km");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(2) GridFieldDim object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method hasdata
    if ( grid.hasdata() ) {
       cerr << " Dataless GridFieldDim object returned true status" << endl;
       exit(1);    
    }

    // =========================  method load-1
    grid.load( odata );
    // NOW it should be valid!
    if ( grid.status() != 0 ) {
       cerr << " Fully initialized GridFieldDim object returned non-0 status: " << grid.status() << endl;
       exit(1);    
    }
    if ( ! grid.hasdata() ) {
       cerr << " Datafull GridFieldDim object returned false status" << endl;
       exit(1);    
    }

    // =========================  method quantity
    // now check the loaded values
    str = grid.quantity(); 
    if ( str != "altitude" ) {
       cerr << " bad quantity <<" << str << ">>"  << endl;
       exit(1);           
    }
    // =========================  method units
    str = grid.units(); 
    if ( str != "km" ) {
       cerr << " bad units <<" << str << ">>" << endl;
       exit(1);           
    }
    // =========================  type identification
    str = grid.id(); 
    if ( str != "GridFieldDim" ) {
       cerr << " bad gridID <<" << str << ">>"  << endl;
       exit(1);           
    }
    
    // =========================  method dataSize
    if ( grid.dataSize() != 71 ) {
       cerr << " bad dataSize : " << grid.size()   << endl;
       exit(1);               
    }
    if ( grid.size() != 71 ) {
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
       cerr << " Uninitialized second GridFieldDim object returned 0 status: " << grid2.status() << endl;
       exit(1);    
    }
    // =========================  method (assignment operator)
    grid2 = grid;
    if ( grid2.status() != 0 ) {
       cerr << " Copied GridFieldDim object returned non-0 status: " << grid2.status() << endl;
       exit(1);    
    }
    
    // =========================  method dims
    n1 = grid2.size( );
    if ( n1 != 71  ) {
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
    grid.set_value(20,  20.1);
    val = grid.value(20);
    if ( mismatch( val, 20.1) ) {
       cerr << " Mismatched Assigned Value (" << 20 << ", " << 10 << ") : "
           << -8765.0 << " vs. " << val << endl;
       exit(1);
    }
    grid.set_value(20, 20);
    
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
    val = ozs[70];
    grid.index( val, &ival, &ival2 ) ;
    if ( ival != 69 || ival2 != 70 ) {
        cerr << " zindex " << val << " -> (69,70) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    val = 0.0;
    grid.index( val, &ival, &ival2 ) ;
    if ( ival != 0 || ival2 != 1 ) {
        cerr << " zindex " << val << " -> (0,1) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }

   
    if ( grid.status() != 0 ) {
       cerr << "Valid GridFieldDim object returned non-zero status." << endl;
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
    GridFieldDim grid3 = grid;
    if ( grid3.status() != 0 ) {
       cerr << " copy-constructed GridFieldDim returned non-zero status: " << grid3.status() << endl;
       exit(1);    
    }
    n1 = grid3.size();
    if ( n1 != 71  ) {
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
       cerr << "GridFieldDim output failed " << endl;
       exit(1);  
    }   

    grid.clear(); 
    if ( grid.status() == 0 ) {
       cerr << " Formerly-valid but now cleared GridFieldDim object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method >>
    // restore 
    string pstate = os.str();
    istringstream is( pstate );
    try {
       is >> grid;
    } catch (...) {
       cerr << "GridFieldDim input failed: " << pstate << endl;
       exit(1);  
    }
    if ( ! grid3.match(grid) ) {
       cerr << "GridFieldDim restored grid fails to match original " << endl;
       exit(1);    
    }   
    // now try it with a file
    outfile = new std::ofstream("test_GridFieldDim.dat", std::ios::out | std::ios::binary | std::ios::trunc );
    *outfile << grid3;
    outfile->close();
    delete outfile;
    grid.clear();
    infile = new std::ifstream("test_GridFieldDim.dat", std::ios::in | std::ios::binary );
    *infile >> grid;
    infile->close();
    delete infile;
    if ( ! grid3.match(grid) ) {
       cerr << "GridFieldDim restored from file grid fails to match original " << endl;
       exit(1);    
    }
    (void) remove("test_GridFieldDim.dat");


    
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
       cerr << "GridFieldDim* restored grid fails to match original " << endl;
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
          cerr << "GridFieldDim iterator returned wrong value: " << i << ": " << val << " vs. " << odata[i] << endl;
          exit(1);
        }
        try {
           looper.assign( 3.14159 );
           if ( i != 3 ) {
              cerr << "GridFieldDim iterator should not have been able to set element " << i << " to:" << 3.14159 << endl;
              exit(1);       
           }
           if ( mismatch( *looper, 3.14159 ) ) {
             cerr << "GridFieldDim iterator set wrong value: " << i << ": " << val << " vs. " << 3.14159 << endl;
             exit(1);
           }
        } catch ( GridField::badNonmonotonic ) {
           if ( i == 3 ) {
              cerr << "GridFieldDim iterator failed to set element " << i << " to:" << 3.14159 << endl;
              exit(1);       
           }
        } catch (...) {
            cerr << "Unknown failed in seeting element " << i << " to " << 3.14159 << endl;
            exit(1);
        };   
        try {
            looper.assign( 2.71828 );
            if ( i != 2 ) {
               cerr << "GridFieldDim iterator should not have been able to set element " << i << " to:" << 2.71828 << endl;
               exit(1);       
            }
            if ( mismatch( *looper, 2.71828 ) ) {
              cerr << "GridFieldDim iterator assigned wrong value:" << i << ": " << *looper << " vs. " << 2.71828 << endl;
              exit(1);
            }
        } catch ( GridField::badNonmonotonic ) {
           if ( i == 2 ) {
              cerr << "GridFieldDim iterator failed to set element " << i << " to:" << 2.71828<< endl;
              exit(1);       
           }
        } catch (...) {
            cerr <<  "Unknown failed in seeting element " << i << " to " << 2.71828 << endl;
            exit(1);
        }
           
    }    
    val = 13.25;
    looper = grid.begin();
    for ( i=1; i<=13; i++ ) {
       looper++;
    }
    // ========================= iterator.indices()
    looper.indices(&i);
    if ( i != 13  ) {
       cerr << "GridFieldDim iterator.indices failed: ( " << i << " )"<< endl;
       exit(1);
    }
    val2 = *looper;
    looper.assign(val);
    if ( mismatch(grid(13), val) ) {
      cerr << "GridFieldDim assignment to iterator failed: " << val << " vs. " << grid(13) << endl;
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
          cerr << "GridFieldDim const_iterator returned wrong value: " << i << ": " << val << " vs. " << i << endl;
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
       cerr << "GridFieldDim const_iterator.indices failed: ( " << i <<  " )"<< endl;
       exit(1);
    }
    
       
    // =========================  method setPgroup (deferred to test_GridFieldDim_serial,test_GridFieldDim_MPI) 
    // =========================  method svr_start (deferred to test_GridFieldDim_serial,test_GridFieldDim_MPI) 
    // =========================  method svr_listen (deferred to test_GridFieldDim_serial,test_GridFieldDim_MPI) 
    // =========================  method svr_done (deferred to test_GridFieldDim_serial,test_GridFieldDim_MPI) 
    // =========================  method svr_send_vals (deferred to test_GridFieldDim_serial,test_GridFieldDim_MPI) 
    // =========================  method svr_send_meta (deferred to test_GridFieldDim_serial,test_GridFieldDim_MPI) 
    // =========================  method receive_meta (deferred to test_GridFieldDim_serial,test_GridFieldDim_MPI) 
      
    exit(0);

}
