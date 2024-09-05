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
#include "gigatraj/GridLatLonFieldSfc.hh"

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
    GridLatLonFieldSfc grid, grid2;
    GridFieldSfc *gridx;
    GridLatLonFieldSfc const *gridy;
    std::vector<real> olons, xlons;
    std::vector<real> olats, xlats;
    std::vector<real> odata, xdata;
    int i, j, idx;
    int n1, n2;
    real val, val2;
    int ival, ival2;
    double tyme;
    string str;
    int ia[3], ja[3];
    real da[3];
    GridFieldSfc::iterator looper;
    GridFieldSfc::const_iterator clooper;
    std::ofstream *outfile;
    std::ifstream *infile;
    GridLatLonFieldSfc *arr;
    real area;
    
    olons.reserve(72);
    for (i=0; i<72; i++ ) {
       olons.push_back(i*5.0);
    }   
    olats.reserve(37);
    for (i=0; i<37; i++ ) {
       olats.push_back( i*5.0-90.0 );
    }   
    odata.reserve(72*37);
    for (i=0; i<72*37; i++ ) {
       odata.push_back( (real) i );
    }   

  



  
    // =========================  method status
    // should be invalid
    if ( grid.status() == 0 ) {
       cerr << " Uninitialized GridLatLonFieldSfc object returned 0 status." << endl;
       exit(1);    
    }
    // =========================  method clear
    grid.clear(); // should have no effect
    if ( grid.status() == 0 ) {
       cerr << " Cleared GridLatLonFieldSfc object returned 0 status." << endl;
       exit(1);    
    }

    // =========================  method set_quantity
    grid.set_quantity("air temperature");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(1) GridLatLonFieldSfc object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_units
    grid.set_units("K");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(2) GridLatLonFieldSfc object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_fillval
    grid.set_fillval(-1234.0);
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(3) GridLatLonFieldSfc object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_surface
    grid.set_surface("tropopause");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(4) GridLatLonFieldSfc object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_time
    grid.set_time( 3.14159, "Grue 34, year of the kumquat");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(5) GridLatLonFieldSfc object returned 0 status." << endl;
       exit(1);    
    }

    // =========================  method hasdata
    if ( grid.hasdata() ) {
       cerr << " Dataless GridLatLonFieldSfc object returned true status" << endl;
       exit(1);    
    }

    // =========================  method load-1
    grid.load( olons, olats, odata );
    // NOW it should be valid!
    if ( grid.status() != 0 ) {
       cerr << " Fully initialized GridLatLonFieldSfc object returned non-0 status: " << grid.status() << endl;
       exit(1);    
    }
    if ( ! grid.hasdata() ) {
       cerr << " Datafull GridLatLonFieldSfc object returned false status" << endl;
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
    str = grid.surface(); 
    if ( str != "tropopause" ) {
       cerr << " bad surface <<" << str << ">>" << endl;
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
    if ( str != "GridLatLonFieldSfc" ) {
       cerr << " bad gridID <<" << str << ">>"  << endl;
       exit(1);           
    }
    
    // =========================  method dataSize
    if ( grid.dataSize() != 72*37 ) {
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

    // =========================  method doeswrap
    if ( grid.doeswrap() == 0  ) {
       cerr << "Grid is said not to wrap, when it does wrap." <<  endl;
       exit(1);
    }
    
    if ( grid2.status() == 0 ) {
       cerr << " Uninitialized second GridLatLonFieldSfc object returned 0 status: " << grid2.status() << endl;
       exit(1);    
    }
    // =========================  method (assignment operator)
    grid2 = grid;
    if ( grid2.status() != 0 ) {
       cerr << " Copied GridLatLonFieldSfc object returned non-0 status: " << grid2.status() << endl;
       exit(1);    
    }
    
    // =========================  method dims
    grid2.dims( &n1, &n2 );
    if ( n1 != 72 || n2 != 37  ) {
       cerr << " Bad dimensions: " << n1 << ", " << n2 <<  endl;
       exit(1);
    }

    // =========================  method longitudes
    xlons = grid2.longitudes();
    for ( i=0; i<n1; i++ ) {
       if ( mismatch( olons[i], xlons[i] ) ) {
          cerr << " Mismatched lon " << i << ": " << olons[i] << " vs. " << xlons[i] << endl;
          exit(1);
       }   
    }
    // =========================  method longitude
    for ( i=0; i<n1; i++ ) {
       val = grid2.longitude(i);
       if ( mismatch( olons[i], val ) ) {
          cerr << " Mismatched Lon " << i << ": " << olons[i] << " vs. " << val << endl;
          exit(1);
       }   
    }
    // =========================  method latitudes
    xlats = grid2.latitudes();
    for ( i=0; i<n2; i++ ) {
       if ( mismatch( olats[i], xlats[i] ) ) {
          cerr << " Mismatched lat " << i << ": " << olats[i] << " vs. " << xlats[i] << endl;
          exit(1);
       }   
    }
    // =========================  method latitude
    for ( i=0; i<n2; i++ ) {
       val = grid2.latitude(i);
       if ( mismatch( olats[i], val ) ) {
          cerr << " Mismatched Lat " << i << ": " << olats[i] << " vs. " << val << endl;
          exit(1);
       }   
    }
        
    idx = -1;
    for ( j=0; j<n2; j++ ) {
       for ( i=0; i<n1; i++ ) {
          idx++;
          // =========================  method value
          val = grid.value(i,j);
          if ( mismatch(val, odata[idx] ) ) {
             cerr << " Mismatched data value (" << i << ", " << j << ") : "
                 << odata[idx] << " vs. " << val << endl;
             exit(1);
          }
          // =========================  method operator()
          val = grid(i,j);
          if ( mismatch(val, odata[idx] ) ) {
             cerr << " Mismatched Data Value (" << i << ", " << j << ") : "
                 << odata[idx] << " vs. " << val << endl;
             exit(1);
          }
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
    if ( ! grid.match(grid2) || grid(3,4) != grid2(3,4) ) {
       cerr << "grid and grid2 do not match after reloading" << endl;
       exit(1);
    }

    // =========================  method &operator()
    grid(20,10) = -8765.0;
    val = grid.value(20,10);
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
    olons[45] = olons[45]-0.5;
    grid.load( olons, olats, odata );
    if ( grid.compatible( grid2 ) ) {
       cerr << " altered grid copy(1) is compatible with original! " << endl;
       exit(1);
    }
    if ( ! grid.compatible( grid2, METCOMPAT_VERT | METCOMPAT_TIME ) ) {
       cerr << " altered grid copy(1) is (v-t) incompatible with original! " << endl;
       exit(1);
    }
    if ( grid.compatible( grid2 ) ) {
       cerr << " altered grid copy(2) is compatible with original! " << endl;
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
    grid.set_surface( "surface" );
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
    grid.load(olons,olats);
    if ( grid.hasdata() ) {
       cerr << " Dataless loaded GridLatLonFieldSfc object returned true status " << endl;
       exit(1);    
    }
    

    // reset for next test
    grid = grid2;
    // =========================  method wrap
    // test longitude wrapping
    val = grid.wrap(45.0);
    if ( mismatch( 45.0, val ) ) {
        cerr << " Wrap 45->45 failed:  " << val << endl;
        exit(1);    
    }
    val = grid.wrap(-45.0);
    if ( mismatch( 315.0, val ) ) {
        cerr << " Wrap -45 -> 315 failed:  " << val << endl;
        exit(1);    
    }
    val = grid.wrap( 0.0);
    if ( mismatch( 0.0, val ) ) {
        cerr << " Wrap 0 -> 0 failed:  " << val << endl;
        exit(1);    
    }
    val = grid.wrap( 355.0);
    if ( mismatch( 355.0, val ) ) {
        cerr << " Wrap 355 -> 355 failed:  " << val << endl;
        exit(1);    
    }
    val = grid.wrap( 357.0);
    if ( mismatch( 357.0, val ) ) {
        cerr << " Wrap 357 -> 357 failed:  " << val << endl;
        exit(1);    
    }
    val = grid.wrap( -2.0);
    if ( mismatch( 358.0, val ) ) {
        cerr << " Wrap -2 -> 358 failed:  " << val << endl;
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
    

    // =========================  method iwrap
    ival = grid.iwrap( 3 );
    if ( ival != 3 ) {
        cerr << " iWrap 3 -> 3 failed:  " << ival << endl;
        exit(1);    
    }
    ival = grid.iwrap( 75 );
    if ( ival != 3 ) {
        cerr << " iWrap 75 -> 4 failed:  " << ival << endl;
        exit(1);    
    }
    ival = grid.iwrap( -1 );
    if ( ival != 71 ) {
        cerr << " iWrap -1 -> 71 failed:  " << ival << endl;
        exit(1);    
    }
    
    
    // =========================  method lonindex
    // test indexing functions
    grid.lonindex( 10.0, &ival, &ival2 ) ;
    if ( ival != 1 || ival2 != 2 ) {
        cerr << " lonindex 10.0 -> (1,2) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    grid.lonindex( 357.0, &ival, &ival2 ) ;
    if ( ival != 71 || ival2 != 72 ) {
        cerr << " lonindex 357.0 -> (71,72) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    grid.lonindex( -2.0, &ival, &ival2 ) ;
    if ( ival != 71 || ival2 != 72 ) {
        cerr << " lonindex -2.0 -> (71,72) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }

    // =========================  method latindex
    grid.latindex( -2.0, &ival, &ival2 ) ;
    if ( ival != 17 || ival2 != 18 ) {
        cerr << " latindex -2.0 -> (17,18) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    grid.latindex( -90.0, &ival, &ival2 ) ;
    if ( ival != 0 || ival2 != 1 ) {
        cerr << " latindex -90.0 -> (0,1) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    grid.latindex(  90.0, &ival, &ival2 ) ;
    if ( ival != 35 || ival2 != 36 ) {
        cerr << " latindex 90.0 -> (35,36) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    grid.latindex( -35.0, &ival, &ival2 ) ;
    if ( ival != 10 || ival2 != 11 ) {
        cerr << " latindex -35.0 -> (10,11) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    
    // test values wraparound
    val = grid.longitude(-2);
    if ( mismatch( 350.0, val ) ) {
        cerr << " longitude[-2] failed:  " << val << " vs " << 350.0 << endl;
        exit(1);    
    }
    val = grid.longitude(73);
    if ( mismatch( 5.0, val ) ) {
        cerr << " longitude[73] failed:  " << val << " vs " << 5.0 << endl;
        exit(1);    
    }
    val = grid.value(-3,5);
    if ( mismatch( val, grid.value(69,5) ) ) {
        cerr << " value[-3,5] failed:  " << val << " vs " << grid.value(69,5) << endl;
        exit(1);    
    }
    val = grid.value(78,5);
    if ( mismatch( val, grid.value(6,5) ) ) {
        cerr << " value[78,5] failed:  " << val << " vs " << grid.value(6,5) << endl;
        exit(1);    
    }
    
    if ( grid.status() != 0 ) {
       cerr << "Valid GridLatLonFieldSfc object returned non-zero status." << endl;
       exit(1);    
    }
    // =========================  method gridpoints
    ia[0] = 2;
    ia[1] = 32;
    ia[2] = 7;
    ja[0] = 12;
    ja[1] = 25;
    ja[2] = 17;
    grid.gridpoints( 3, ia, ja, da);
    if ( da[0] != grid( ia[0],ja[0] ) 
     ||  da[1] != grid( ia[1],ja[1] ) 
     ||  da[2] != grid( ia[2],ja[2] ) ) {
       cerr << " gridpoints results do not match value results" << endl;
       exit(1);    
    
    }

    // =========================  method (copy constructor)
    GridLatLonFieldSfc grid3 = grid;
    if ( grid3.status() != 0 ) {
       cerr << " copy-constructed GridLatLonFieldSfc returned non-zero status: " << grid3.status() << endl;
       exit(1);    
    }
    grid3.dims( &n1, &n2 );
    if ( n1 != 72 || n2 != 37  ) {
       cerr << " Bad dimensions: " << n1 << ", " << n2 <<  endl;
       exit(1);
    }
    if ( grid3.longitude(4) != grid.longitude(4) 
    ||  grid3.latitude(12) != grid.latitude(12)
    ||  grid3(7,10) != grid(7,10) ) {
       cerr << " copy-constructed grid values do not match original " << endl;
       exit(1);    
    } 


    // =========================  method <<
    // dump 
    ostringstream os;
    try {
       os << grid;
    } catch (...) {
       cerr << "GridLatLonFieldSfc output failed " << endl;
       exit(1);  
    }   

    grid.clear(); 
    if ( grid.status() == 0 ) {
       cerr << " Formerly-valid but now cleared GridLatLonFieldSfc object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method >>
    // restore 
    string pstate = os.str();
    istringstream is( pstate );
    try {
       is >> grid;
    } catch (...) {
       cerr << "GridLatLonFieldSfc input failed: " << pstate << endl;
       exit(1);  
    }
    if ( ! grid3.match(grid) ) {
       cerr << "GridLatLonFieldSfc restored grid fails to match original " << endl;
       exit(1);    
    }   
    // now try it with a file
    outfile = new std::ofstream("test_GridLatLonFieldSfc.dat", std::ios::out | std::ios::binary | std::ios::trunc );
    *outfile << grid3;
    outfile->close();
    delete outfile;
    grid.clear();
    infile = new std::ifstream("test_GridLatLonFieldSfc.dat", std::ios::in | std::ios::binary );
    *infile >> grid;
    infile->close();
    delete infile;
    if ( ! grid3.match(grid) ) {
       cerr << "GridLatLonFieldSfc restored from file grid fails to match original " << endl;
       exit(1);    
    }
    (void) remove("test_GridLatLonFieldSfc.dat");


    
    // ======================= method duplicate
    gridx = grid.duplicate();
    val = (*gridx)(1,2);
    if ( val != grid(1,2) ) {
       cerr << "GridLatLonFieldSfc duplicated grid fails to match original: " << val << " vs " << grid(1,2)<< endl;
       exit(1);    
    }   
    
    // ======================= method areas
    arr = dynamic_cast<GridLatLonFieldSfc*>(grid.areas());
    val = arr->value(34,23);
    if ( mismatch( 0.0068997, val ) ) {
        cerr << " area(34,23) failed:  " << val << endl;
        exit(1);    
    }
    area = 0.0;
    for ( j=0; j<n2; j++ ) {
       for ( i=0; i<n1; i++ ) {
          area += arr->value(i,j);
       }
    }   
    if ( mismatch( 4.0*PI, area ) ) {
        cerr << " total area failed:  " << area << endl;
        exit(1);    
    }
    delete arr;  
      
    
    // ======================= method serialize
    // ======================= method deserialize
    //gridx = &grid;
    ostringstream os2;
    gridx->serialize(os2);
    string pstate2 = os2.str();
    istringstream is2( pstate2 );
    grid3.clear();
    grid3.deserialize(is2);
    if ( grid3.quantity() != gridx->quantity()
     || grid3.value(4,5) != gridx->value(4,5)
     ) {
       cerr << "GridLatLonFieldSfc* restored grid fails to match original " << endl;
       exit(1);    
    }   
    
    // ========================= iterator
    // ========================= begin()
    // ========================= end()
    // ========================= assign *
    for ( i=0, looper=grid.begin(); looper != grid.end(); i++, looper++ ) {
        //cerr << i << ": " << *looper << endl;
        val = *looper;
        if ( mismatch( val, static_cast<real>(i) ) ) {
          cerr << "GridLatLonFieldSfc iterator returned wrong value: " << val << " vs. " << i << endl;
          exit(1);
        }
        *looper = 3.14159;
        if ( mismatch( *looper, 3.14159 ) ) {
          cerr << "GridLatLonField3D iterator set wrong value: " << val << " vs. " << 3.14159 << endl;
          exit(1);
        }
        looper.assign( 2.71828 );
        if ( mismatch( *looper, 2.71828 ) ) {
          cerr << "GridLatLonField3D iterator assigned wrong value: " << val << " vs. " << 2.71828 << endl;
          exit(1);
        }
        *looper = val;
    }    
    val = 123.0;
    looper = grid.begin();
    for ( i=1; i<=983; i++ ) {
       looper++;
    }
    // ========================= iterator.indices()
    looper.indices(&i,&j);
    if ( i != 47 || j != 13 ) {
       cerr << "GridLatLonFieldSfc iterator.indices failed: ( " << i << ", " << j << " )"<< endl;
       exit(1);
    }
    val2 = *looper;
    *looper = val;
    if ( mismatch(grid(47,13), val) ) {
      cerr << "GridLatLonFieldSfc assignment to iterator failed: " << val << " vs. " << grid(47,13) << endl;
      exit(1);
    }
    *looper = val2;
    // ========================= iterator.area()
    area = 0.0;
    for ( looper = grid.begin(); looper != grid.end(); looper++ ) {
        area += looper.area();
    }
    if ( mismatch( 4.0*PI, area ) ) {
        cerr << " iterator total area failed:  " << area << endl;
        exit(1);    
    }
    
    // ========================= const_iterator
    // ========================= begin() const
    // ========================= end() const
    gridy = &grid;
    for ( i=0, clooper=gridy->begin(); clooper != gridy->end(); i++, clooper++ ) {
        //cerr << i << ": " << *looper << endl;
        val = *clooper;
        if ( mismatch( val, static_cast<real>(i) ) ) {
          cerr << "GridLatLonFieldSfc const_iterator returned wrong value: " << val << " vs. " << i << endl;
          exit(1);
        }
    } 
    clooper = gridy->begin();
    for ( i=1; i<=983; i++ ) {
       clooper++;
    }
    // ========================= const_iterator.indices()
    clooper.indices(&i,&j);
    if ( i != 47 || j != 13 ) {
       cerr << "GridLatLonFieldSfc const_iterator.indices failed: ( " << i << ", " << j << " )"<< endl;
       exit(1);
    }
    // ========================= iterator.area()
    area = 0.0;
    for ( clooper = gridy->begin(); clooper != gridy->end(); clooper++ ) {
        area += clooper.area();
    }
    if ( mismatch( 4.0*PI, area ) ) {
        cerr << " const_iterator total area failed:  " << area << endl;
        exit(1);    
    }
    
       
    // =========================  method setPgroup (deferred to test_GridLatLonFieldSfc_serial,test_GridLatLonFieldSfc_MPI) 
    // =========================  method svr_start (deferred to test_GridLatLonFieldSfc_serial,test_GridLatLonFieldSfc_MPI) 
    // =========================  method svr_listen (deferred to test_GridLatLonFieldSfc_serial,test_GridLatLonFieldSfc_MPI) 
    // =========================  method svr_done (deferred to test_GridLatLonFieldSfc_serial,test_GridLatLonFieldSfc_MPI) 
    // =========================  method svr_send_vals (deferred to test_GridLatLonFieldSfc_serial,test_GridLatLonFieldSfc_MPI) 
    // =========================  method svr_send_meta (deferred to test_GridLatLonFieldSfc_serial,test_GridLatLonFieldSfc_MPI) 
    // =========================  method receive_meta (deferred to test_GridLatLonFieldSfc_serial,test_GridLatLonFieldSfc_MPI) 
      
    exit(0);

}
