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
#include "gigatraj/GridLatLonField3D.hh"
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
    GridLatLonField3D grid, grid2;
    GridLatLonField3D* tmp1;
    GridField3D *gridx;
    GridLatLonFieldSfc sfcgrid;
    GridLatLonFieldSfc* sfcgridx;
    GridLatLonFieldSfc* tmp2;
    std::vector<real> olons, xlons;
    std::vector<real> olats, xlats;
    std::vector<real> opres, xpres;
    std::vector<real> odata, xdata;
    int i, j, k, idx;
    int n1, n2, n3;
    real val, val2;
    int ival, ival2;
    double tyme;
    string str;
    int ia[3], ja[3], ka[3];
    real da[3];
    GridLatLonField3D const *gridy;
    GridField3D::iterator looper;
    GridField3D::const_iterator clooper;
    GridField3D::profileIterator plooper;
    GridField3D::const_profileIterator cplooper;
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
    opres.reserve(18);
    opres.push_back( 1000.0);
    opres.push_back(  850.0);
    opres.push_back(  700.0);
    opres.push_back(  500.0);
    opres.push_back(  400.0);
    opres.push_back(  300.0);
    opres.push_back(  250.0);
    opres.push_back(  200.0);
    opres.push_back(  150.0);
    opres.push_back(  100.0);
    opres.push_back(   70.0);
    opres.push_back(   50.0);
    opres.push_back(   30.0);
    opres.push_back(   10.0);
    opres.push_back(    5.0);
    opres.push_back(    2.0);
    opres.push_back(    1.0);
    opres.push_back(    0.4);
    odata.reserve(72*37*18);
    for (i=0; i<72*37*18; i++ ) {
       odata.push_back( (real) i );
    }   

  

    
    // =========================  method status
    // should be invalid
    if ( grid.status() == 0 ) {
       cerr << " Uninitialized GridLatLonField3D object returned 0 status." << endl;
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
       cerr << " Partially initialized(1) GridLatLonField3D object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_units
    grid.set_units("K");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(2) GridLatLonField3D object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_fillval
    grid.set_fillval(-1234.0);
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(3) GridLatLonField3D object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_vertical
    grid.set_vertical("air pressure");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(4) GridLatLonField3D object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_vunits
    grid.set_vunits("mb");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(5) GridLatLonField3D object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method set_time
    grid.set_time( 3.14159, "Grue 34, year of the kumquat");
    // should still be invalid
    if ( grid.status() == 0 ) {
       cerr << " Partially initialized(6) GridLatLonField3D object returned 0 status." << endl;
       exit(1);    
    }
    // =========================  method hasdata
    if ( grid.hasdata() ) {
       cerr << " Dataless GridLatLonField3D object returned true status " << endl;
       exit(1);    
    }

    // =========================  method load-1
    grid.load( olons, olats, opres, odata );
    // NOW it should be valid!
    if ( grid.status() != 0 ) {
       cerr << " Fully initialized GridLatLonField3D object returned non-0 status: " << grid.status() << endl;
       exit(1);    
    }
    if ( ! grid.hasdata() ) {
       cerr << " Datafull GridLatLonField3D object returned false status "  << endl;
       exit(1);    
    }

    // now check the loaded values
    // =========================  method quantity
    str = grid.quantity(); 
    if ( str != "air temperature" ) {
       cerr << " bad quantity <<" << str << ">>"  << endl;
       exit(1);           
    }
    // =========================  method units
    str = grid.units(); 
    if ( str != "K" ) {
       cerr << " bad units <<" << str << ">>"  << endl;
       exit(1);           
    }
    // =========================  method vertical
    str = grid.vertical(); 
    if ( str != "air pressure" ) {
       cerr << " bad vertical quantity <<" << str << ">>"  << endl;
       exit(1);           
    }
    // =========================  method vunits
    str = grid.vunits(); 
    if ( str != "mb" ) {
       cerr << " bad vertical units <<" << str << ">>" << endl;
       exit(1);           
    }
    // =========================  method fillval
    val = grid.fillval(); 
    if ( mismatch( val, -1234.0 ) ) {
       cerr << " bad fill value :" << val << endl;
       exit(1);           
    }
    // =========================  method time
    tyme = grid.time(); 
    if ( mismatch( tyme, 3.14159 ) ) {
       cerr << " bad time value :" << tyme << endl;
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
    if ( str != "GridLatLonField3D" ) {
       cerr << " bad gridID <<" << str << ">>"  << endl;
       exit(1);           
    }
    
    
    // =========================  method dataSize
    if ( grid.dataSize() != 72*37*18 ) {
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
       cerr << " Uninitialized second GridLatLonField3D object returned 0 status: " << grid2.status() << endl;
       exit(1);    
    }
    // =========================  method (assignment operator)
    grid2 = grid;
    if ( grid2.status() != 0 ) {
       cerr << " Copied GridLatLonField3D object returned non-0 status: " << grid2.status() << endl;
       exit(1);    
    }
    
    // =========================  method dims
    grid2.dims( &n1, &n2, &n3 );
    if ( n1 != 72 || n2 != 37 || n3 != 18 ) {
       cerr << " Bad dimensions: " << n1 << ", " << n2 << ", " << n3 << endl;
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
       val = grid.latitude(i);
       if ( mismatch( olats[i], val ) ) {
          cerr << " Mismatched Lat " << i << ": " << olats[i] << " vs. " << val << endl;
          exit(1);
       }   
    }
    // =========================  method levels
    xpres = grid2.levels();
    for ( i=0; i<n3; i++ ) {
       if ( mismatch( opres[i], xpres[i] ) ) {
          cerr << " Mismatched pres " << i << ": " << opres[i] << " vs. " << xpres[i] << endl;
          exit(1);
       }   
    }
    // =========================  method level
    for ( i=0; i<n3; i++ ) {
       val = grid.level(i);
       if ( mismatch( opres[i], val ) ) {
          cerr << " Mismatched Pres " << i << ": " << opres[i] << " vs. " << val << endl;
          exit(1);
       }   
    }
        
    idx = -1;
    for ( k=0; k<n3; k++ ) {
      for ( j=0; j<n2; j++ ) {
         for ( i=0; i<n1; i++ ) {
            idx++;
            // =========================  method value
            val = grid.value(i,j,k);
            if ( mismatch(val, odata[idx] ) ) {
               cerr << " Mismatched data value (" << i << ", " << j << ", " << k << ") : "
                   << odata[idx] << " vs. " << val << endl;
               exit(1);
            }
            // =========================  method operator()
            val = grid(i,j,k);
            if ( mismatch(val, odata[idx] ) ) {
               cerr << " Mismatched Data Value (" << i << ", " << j << ", " << k << ") : "
                   << odata[idx] << " vs. " << val << endl;
               exit(1);
            }
         }
      } 
    }
    
    //========================== test independence of the two grid data contents
    val = grid(5,6,7);
    grid(5,6,7) = val * 2.0 + 100.0;
    if ( grid(5,6,7) == grid2(5,6,7) ) {
       cerr << "grid and grid2 are NOT independent! " << grid(5,6,7) << " vs. " << grid2(5,6,7) << " vs. " << val << endl;
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
    if ( ! grid.match(grid2) || grid(3,4,5) != grid2(3,4,5) ) {
       cerr << "grid and grid2 do not match after reloading" << endl;
       exit(1);
    }

    // =========================  method &operator()
    // can we assign using the indexing format?
    grid(20,10,3) = -8765.0;
    val = grid.value(20,10,3);
    if ( mismatch( val, -8765.0) ) {
       cerr << " Mismatched Assigned Value (" << 20 << ", " << 10 << ", " << 3 << ") : "
           << -8765.0 << " vs. " << val << endl;
       exit(1);
    }
    
    // check comparisons
    // =========================  method compatible-1
    if ( ! grid.compatible( grid2 ) ) {
       cerr << " grid copy is not compatible with original! " << endl;
       exit(1);
    }
    olons[45] = olons[45]-0.5;
    grid.load( olons, olats, opres, odata );
    if ( grid.compatible( grid2 ) ) {
       cerr << " altered grid copy(1) is compatible with original! " << endl;
       exit(1);
    }
    if ( ! grid.compatible( grid2, METCOMPAT_VERT | METCOMPAT_TIME ) ) {
       cerr << " altered grid copy(1) is (v-t) incompatible with original! " << endl;
       exit(1);
    }
    opres[5] = opres[5]-0.5;
    grid.load( olons, olats, opres, odata );
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
    opres[5] = opres[5]-0.5;
    grid.load( xlons, xlats, opres, odata );
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
    grid.load(olons,olats,opres);
    if ( grid.hasdata() ) {
       cerr << " Dataless loaded GridLatLonField3D object returned true status: " << endl;
       exit(1);    
    }

    // =========================  method extractSfc
    // reset for next test
    grid = grid2;
    // can we extract a 2D surface?
    sfcgridx = dynamic_cast<GridLatLonFieldSfc*>( grid.extractSurface( 5 ) );
    if ( sfcgridx->status() != 0 ) {
       cerr << " extracted sfcgridx is not well-initialized:  " << sfcgridx->status() << endl;
       exit(1);    
    }
    // =========================  method compatible-2
    if ( ! grid.compatible( *sfcgridx, METCOMPAT_HORIZ | METCOMPAT_TIME ) ) {
       cerr << " extract sfcgridx is (h-t) incompatible with 3D original! " << endl;
       exit(1);
    }
    for ( j=0; j<n2; j++ ) {
       for ( i=0; i<n1; i++ ) {
          val = sfcgridx->value(i,j);

          if ( mismatch(val, grid.value(i,j,5) ) ) {
             cerr << " Mismatched data subgrid value (" << i << ", " << j << ", " << 5 << ") : "
                 << grid.value(i,j,5) << " vs. " << val << endl;
             exit(1);
          }
       }
    }
    delete sfcgridx;
    
    // =========================  method replaceLevel
    grid = grid2;
    sfcgridx = dynamic_cast<GridLatLonFieldSfc*>( grid.extractSurface( 8 ) );
    for ( j=0; j<n2; j++ ) {
       for ( i=0; i<n1; i++ ) {
          sfcgridx->valueref(i,j) = sfcgridx->value(i,j) * 2.0 + 100.0;
       }
    }
    grid.replaceLevel( *sfcgridx, 8 );
    for ( k=0; k<n3; k++ ) {
        for ( j=0; j<n2; j++ ) {
           for ( i=0; i<n1; i++ ) {
              val = grid2.value(i,j,k); 
              if ( k == 8 ) {
                 val = val * 2.0 + 100.0;
              }
              if ( mismatch(val, grid.value(i,j,k) ) ) {
                 cerr << " Mismatched data replaceLevel value (" << i << ", " << j << ", " << k << ") : "
                     << grid.value(i,j,k) << " vs. " << val << endl;
                 exit(1);
              }
           }
        }
    }
    
    delete sfcgridx;


    // =========================  method transform
    // reset for next test
    grid = grid2;
   // cerr << "grid 2 units :" << grid2.units() << ": " << grid2.mksScale << ", " << grid2.mksOffset << endl;
    grid.transform("fakeunits", 0.369, 1.345 );
    for ( k=0; k<n3; k++ ) {
       for ( j=0; j<n2; j++ ) {
          for ( i=0; i<n1; i++ ) {
              val = ( grid2(i,j,k) - 1.345) / 0.369; // convert mks to imposed units
              if ( mismatch(val, grid(i,j,k) ) ) {
                 cerr << " Mismatched transformed data value (" << i << ", " << j << ", " << k << ") : "
                 << grid.value(i,j,k) << " vs. " << val << endl;
                 exit(1);
              }
          }
       }
    }          
    

    // =========================  method extractSfc
    // reset for next test
    grid = grid2;
    // can we extract a 2D surface?
    tmp2 = grid.extractSfc( 5 );
    sfcgrid = *tmp2;
    delete tmp2;
    if ( sfcgrid.status() != 0 ) {
       cerr << " extracted sfcgrid is not well-initialized:  " << sfcgrid.status() << endl;
       exit(1);    
    }
    // =========================  method compatible-2
    if ( ! grid.compatible( sfcgrid, METCOMPAT_HORIZ | METCOMPAT_TIME ) ) {
       cerr << " extract sfcgrid is (h-t) incompatible with 3D original! " << endl;
       exit(1);
    }
    for ( j=0; j<n2; j++ ) {
       for ( i=0; i<n1; i++ ) {
          val = sfcgrid.value(i,j);

          if ( mismatch(val, grid.value(i,j,5) ) ) {
             cerr << " Mismatched data subgrid value (" << i << ", " << j << ", " << 5 << ") : "
                 << grid.value(i,j,5) << " vs. " << val << endl;
             exit(1);
          }
       }
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
    
    // =========================  method zindex
    grid.zindex( 550.0, &ival, &ival2 ) ;
    //if ( ival != 3 || ival2 != 2 ) {
    if ( ival != 2 || ival2 != 3 ) {
        cerr << " zindex 550.0 -> (2,3) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    grid.zindex( 0.4, &ival, &ival2 ) ;
    //if ( ival != 17 || ival2 != 16 ) {
    if ( ival != 16 || ival2 != 17 ) {
        cerr << " zindex 0.4 -> (16,17) failed:  " << ival << ", " << ival2 << endl;
        exit(1);    
    }
    grid.zindex( 1000.0, &ival, &ival2 ) ;
    //if ( ival != 1 || ival2 != 0 ) {
    if ( ival != 0 || ival2 != 1 ) {
        cerr << " zindex 1000.0 -> (0,1) failed:  " << ival << ", " << ival2 << endl;
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
    val = grid.value(-3,5,3);
    if ( mismatch( val, grid.value(69,5,3) ) ) {
        cerr << " value[-3,5,3] failed:  " << val << " vs " << grid.value(69,5,3) << endl;
        exit(1);    
    }
    val = grid.value(78,5,3);
    if ( mismatch( val, grid.value(6,5,3) ) ) {
        cerr << " value[78,5,3] failed:  " << val << " vs " << grid.value(6,5,3) << endl;
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
    ka[0] = 5;
    ka[1] = 3;
    ka[2] = 0;
    grid.gridpoints( 3, ia, ja, ka, da);
    if ( da[0] != grid( ia[0],ja[0], ka[0] ) 
     ||  da[1] != grid( ia[1],ja[1], ka[1] ) 
     ||  da[2] != grid( ia[2],ja[2], ka[2] ) ) {
       cerr << " gridpoints results do not match value results" << endl;
       exit(1);    
    
    }

    // =========================  method (copy constructor)
    GridLatLonField3D grid3 = grid;
    if ( grid3.status() != 0 ) {
       cerr << " copy-constructed GridLatLonField3D returned non-zero status: " << grid3.status() << endl;
       exit(1);    
    }
    grid3.dims( &n1, &n2, &n3 );
    if ( n1 != 72 || n2 != 37 || n3 != 18 ) {
       cerr << " Bad dimensions: " << n1 << ", " << n2 << ", " << n3 << endl;
       exit(1);
    }
    if ( grid3.longitude(4) != grid.longitude(4) 
    ||  grid3.latitude(12) != grid.latitude(12)
    ||  grid3.level(6) != grid.level(6)
    ||  grid3(7,10,16) != grid(7,10,16) ) {
       cerr << " copy-constructed grid values do not match original " << endl;
       exit(1);    
    } 

    
    
    // =========================  method <<
    // dump 
    ostringstream os;
    try {
       os << grid;
    } catch (...) {
       cerr << "GridLatLonField3D output failed " << endl;
       exit(1);  
    }   

    grid.clear(); 
    if ( grid.status() == 0 ) {
       cerr << " Formerly-valid but now cleared GridLatLonField3D object returned 0 status." << endl;
       exit(1);    
    }
    
    // =========================  method >>
    // restore 
    string pstate = os.str();
    istringstream is( pstate );
    try {
       is >> grid;
    } catch (...) {
       cerr << "GridLatLonField3D input failed: " << pstate << endl;
       exit(1);  
    }
    if ( ! grid3.match(grid) ) {
       cerr << "GridLatLonField3D restored grid fails to match original " << endl;
       exit(1);    
    }
    
    // now try it with a file
    outfile = new std::ofstream("test_GridLatLonField3D.dat", std::ios::out | std::ios::binary | std::ios::trunc );
    *outfile << grid3;
    outfile->close();
    delete outfile;
    grid.clear();
    infile = new std::ifstream("test_GridLatLonField3D.dat", std::ios::in | std::ios::binary );
    *infile >> grid;
    infile->close();
    delete infile;
    if ( ! grid3.match(grid) ) {
       cerr << "GridLatLonField3D restored from file grid fails to match original " << endl;
       exit(1);    
    }
    (void) remove("test_GridLatLonField3D.dat");

    // ======================= method duplicate
    gridx = grid.duplicate();
    val = (*gridx)(1,2,3);
    if ( val != grid(1,2,3) ) {
       cerr << "GridLatLonField3D duplicated grid fails to match original: " << val << " vs " << grid(1,2,3)<< endl;
       exit(1);    
    }   
    
    
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
     || grid3.value(4,5,5) != gridx->value(4,5,5)
     ) {
       cerr << "GridLatLonField3D* restored grid fails to match original " << endl;
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
      
    
    // ========================= iterator
    // ========================= begin()
    // ========================= end()
    // ========================= assign *
    for ( i=0, looper=grid.begin(); looper != grid.end(); i++, looper++ ) {
        //cerr << i << ": " << *looper << endl;
        val = *looper;
        if ( mismatch( val, static_cast<real>(i) ) ) {
          cerr << "GridLatLonField3D iterator returned wrong value: " << val << " vs. " << i << endl;
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
    for ( i=1; i<=8975; i++ ) {
       looper++;
    }
    // ========================= iterator.indices()
    looper.indices(&i,&j, &k);
    if ( i != 47 || j != 13 || k != 3 ) {
       cerr << "GridLatLonField3D iterator.indices failed: ( " << i << ", " << j << ", " << k << " )"<< endl;
       exit(1);
    }
    val2 = *looper;
    *looper = val;
    if ( mismatch(grid(47,13,3), val) ) {
      cerr << "GridLatLonField3D assignment to iterator failed: " << val << " vs. " << grid(47,13,3) << endl;
      exit(1);
    }
    *looper = val2;

    // ========================= begin(k)
    for ( i=0, looper=grid.begin(5); looper != grid.end(5); i++, looper++ ) {
        val = *looper;
        ival = i + 72*37*5;
        if ( mismatch( val, static_cast<real>(ival) ) ) {
          cerr << "GridLatLonField3D level iterator returned wrong value: " << val << " vs. " << ival << endl;
          exit(1);
        }
    }    
    val2 = grid(0,0,4);
    val = 123.0;
    looper = grid.begin(4);
    *looper = val;
    if ( mismatch(grid(0,0,4), val) ) {
      cerr << "GridLatLonField3D assignment to level iterator failed: " << val << " vs. " << grid(0,0,4) << endl;
      exit(1);
    }
    *looper = val2;
    // ========================= iterator.area()
    area = 0.0;
    for ( looper = grid.begin(); looper != grid.end(); looper++ ) {
        area += looper.area();
    }
    if ( mismatch( 18*4.0*PI, area, 0.02 ) ) {
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
          cerr << "GridLatLonField3D const_iterator returned wrong value: " << val << " vs. " << i << endl;
          exit(1);
        }
    } 
    clooper = gridy->begin();
    for ( i=1; i<=8975; i++ ) {
       clooper++;
    }
    // ========================= const_iterator.indices()
    clooper.indices(&i,&j, &k);
    if ( i != 47 || j != 13 || k != 3 ) {
       cerr << "GridLatLonField3D const_iterator.indices failed: ( " << i << ", " << j << ", " << k << " )"<< endl;
       exit(1);
    }
    // ========================= const_iterator.area()
    area = 0.0;
    for ( clooper = gridy->begin(); clooper != gridy->end(); clooper++ ) {
        area += clooper.area();
    }
    if ( mismatch( 18*4.0*PI, area, 0.02 ) ) {
        cerr << " const_iterator total area failed:  " << area << endl;
        exit(1);    
    }

    
    // ========================= profileIterator
    // ========================= profileBegin()
    // ========================= profileEnd()
    std::vector<real>* prfile;
    i = 0;  
    for ( plooper=grid.profileBegin(); plooper != grid.profileEnd(); plooper++ ) {
        prfile = *plooper;
        for (k=0; k<18; k++ ) {
            val = grid( i % 72, i/72, k );
            if ( val != (*prfile)[k] ) {
               cerr << "profileIterator failed at (" << i%72 << ", " << i/72 << ", " << k << "): " 
               << (*prfile)[k] << " vs. " << val << endl;
               exit(1);
            }           
        }
        delete prfile;
    
        i++;
    }   
    // ========================= profileIterator.indices()
    plooper=grid.profileBegin();
    for ( i=1; i<=983; i++ ) {
       plooper++;
    }
    // ========================= profileIterator.indices()
    plooper.indices(&i,&j);
    if ( i != 47 || j != 13 ) {
       cerr << "GridLatLonField3D profileIterator.indices failed: ( " << i << ", " << j << " )"<< endl;
       exit(1);
    }
    
    grid2 = grid;
    // ========================= profileBegin(int,int)
    // ========================= profileIterator.assign()
    plooper=grid.profileBegin(14,20);
    prfile = new std::vector<real>;
    for ( int k=0; k<18; k++ ) {
         prfile->push_back( static_cast<real>(200+k) );
    }
    plooper.assign(*prfile);
    for (k=0; k<18; k++ ) {
        val = grid( 14,20, k );
        if ( val != (*prfile)[k] ) {
           cerr << "profileIterator assign failed at (" << 14 << ", " << 20 << ", " << k << "): " 
           << (*prfile)[k] << " vs. " << val << endl;
           exit(1);
        }           
    }
    delete prfile;
    grid = grid2;
    // ========================= profileIterator.area()
    area = 0.0;
    for ( plooper = grid.profileBegin(); plooper != grid.profileEnd(); plooper++ ) {
        area += plooper.area();
    }
    if ( mismatch( 4.0*PI, area ) ) {
        cerr << " profileIterator total area failed:  " << area << endl;
        exit(1);    
    }
    
    
    // ========================= const_profileIterator
    // ========================= profileBegin() const
    // ========================= profileEnd() const
    gridy = &grid;
    i = 0;  
    for ( cplooper=gridy->profileBegin(); cplooper != gridy->profileEnd(); cplooper++ ) {
        prfile = *cplooper;
        for (k=0; k<18; k++ ) {
            val = grid( i % 72, i/72, k );
            if ( val != (*prfile)[k] ) {
               cerr << "const_profileIterator failed at (" << i%72 << ", " << i/72 << ", " << k << "): " 
               << (*prfile)[k] << " vs. " << val << endl;
               exit(1);
            }           
        }
        delete prfile;
    
        i++;
    }   
    // ========================= profileIterator.indices()
    cplooper=gridy->profileBegin();
    for ( i=1; i<=983; i++ ) {
       cplooper++;
    }
    // ========================= const_profileIterator.indices()
    cplooper.indices(&i,&j);
    if ( i != 47 || j != 13 ) {
       cerr << "GridLatLonField3D const_profileIterator.indices failed: ( " << i << ", " << j << " )"<< endl;
       exit(1);
    }

    // ========================= profileBegin(int,int) const
    cplooper=gridy->profileBegin(13,16);
    prfile = *cplooper;
    for (k=0; k<18; k++ ) {
        val = grid( 13,16, k );
        if ( val != (*prfile)[k] ) {
           cerr << "profileIterator const failed at (" << 13 << ", " << 16 << ", " << k << "): " 
           << (*prfile)[k] << " vs. " << val << endl;
           exit(1);
        }           
    }
    delete prfile;
    // ========================= profileIterator.area()
    area = 0.0;
    for ( cplooper = gridy->profileBegin(); cplooper != gridy->profileEnd(); cplooper++ ) {
        area += cplooper.area();
    }
    if ( mismatch( 4.0*PI, area ) ) {
        cerr << " const_profileIterator total area failed:  " << area << endl;
        exit(1);    
    }


    //========================== generateVertical
    tmp1 = dynamic_cast<GridLatLonField3D*>(grid.generateVertical());
    grid3 = *tmp1;
    delete tmp1;
    val = grid3(0,0,4);
    if ( val != grid.level(4) ) {
       cerr << "GridLatLonField3D generateVertical failed: " << val << " vs. " << grid.level(4) << endl;
       exit(1);
    }
    if ( grid3.quantity() != grid.vertical() ) {
       cerr << "GridLatLonField3D generateVertical quant failed: " 
       << grid3.quantity() << " vs. " << grid.vertical() << endl;
       exit(1);
    }
    if ( grid3.units() != grid.vunits() ) {
       cerr << "GridLatLonField3D generateVertical units failed: " 
       << grid3.units() << " vs. " << grid.vunits() << endl;
       exit(1);
    }
    
    
    
    // =========================  method setPgroup (deferred to test_GridLatLonField3D_serial,test_GridLatLonField3D_MPI) 
    // =========================  method svr_start (deferred to test_GridLatLonField3D_serial,test_GridLatLonField3D_MPI) 
    // =========================  method svr_listen (deferred to test_GridLatLonField3D_serial,test_GridLatLonField3D_MPI) 
    // =========================  method svr_done (deferred to test_GridLatLonField3D_serial,test_GridLatLonField3D_MPI) 
    // =========================  method svr_send_vals (deferred to test_GridLatLonField3D_serial,test_GridLatLonField3D_MPI) 
    // =========================  method svr_send_meta (deferred to test_GridLatLonField3D_serial,test_GridLatLonField3D_MPI) 
    // =========================  method receive_meta (deferred to test_GridLatLonField3D_serial,test_GridLatLonField3D_MPI) 
      
    

    exit(0);

}
