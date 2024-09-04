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





#include <cstdlib>
#include <iostream>
#include <sstream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/RandomSrc.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonFieldSfc.hh"
#include "gigatraj/ThetaOTF.hh"
#include "gigatraj/PressOTF.hh"

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
    RandomSrc rnd(1234);
    GridLatLonField3D t_grid, p_grid, p_grid2;
    GridLatLonField3D *tmp1;
    GridLatLonFieldSfc t_sfc, p_sfc, theta_sfc, rho_sfc;
    GridLatLonField3D theta_1, theta_2, rho_grid, pdel_grid;
    GridLatLonFieldSfc *tmp2;
    std::vector<real> olons;
    std::vector<real> olats;
    std::vector<real> opres;
    std::vector<real> odata;
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
    ThetaOTF thet;
    PressOTF press;
    int ok;
    real start;
    
    
    // set up our grid coordinates
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
    // set up our data array of temperatures
    odata.reserve(72*37*18);
    for (i=0; i<72*37*18; i++ ) {
       val = 230.0 + rnd.uniform(-5.0,5.0);
       odata.push_back( val );
    }   


    // =========================  set up the temperature grid on pressure surfaces
    t_grid.set_quantity("air_temperature");
    t_grid.set_units("Kelvin");
    t_grid.set_fillval(-1234.0);
    t_grid.set_vertical("air_pressure");
    t_grid.set_vunits("hPa");
    t_grid.mksVScale = 100.0; // (mb->Pa)
    t_grid.set_time( 3.14159, "Grue 34, year of the kumquat");
    t_grid.load( olons, olats, opres, odata );
    

    // derive a pressure grid from this grid using PressOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc(t_grid) );
    p_grid = *tmp1;
    delete tmp1;
    
    if ( t_grid.vertical() != p_grid.quantity() ) {
       cerr << "1: p_grid quantities do not match:" << t_grid.vertical() << " vs " <<  p_grid.quantity() << endl;
       exit(1);
    }   
    if ( t_grid.vunits() != p_grid.units() ) {
       cerr << "1: p_grid units do not match" << t_grid.vunits() << " vs " <<  p_grid.units() << endl;
       exit(1);
    } 
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid.level(k), t_grid.level(k)) ) {       
          cerr << "1: PressOTF levels mismatch at : " << k << p_grid.level(k) << " vs " <<  t_grid.level(k) << endl;
          exit(1);
        }  
    }
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid( 0, 0,k), p_grid.level(k)) 
          || mismatch( p_grid( 0,36,k), p_grid.level(k))
          || mismatch( p_grid(71,36,k), p_grid.level(k))
          || mismatch( p_grid(71, 0,k), p_grid.level(k)) ) {
       
          cerr << "1: corner PressOTF value mismatch at : " << k << endl;
          cerr << "  0,  0, k: " << p_grid(  0, 0,k) << " vs " << p_grid.level(k) << endl;
          cerr << "  0, 36, k: " << p_grid(  0,36,k) << " vs " << p_grid.level(k) << endl;
          cerr << " 71, 36, k: " << p_grid( 71,36,k) << " vs " << p_grid.level(k) << endl;
          cerr << " 71,  0, k: " << p_grid( 71, 0,k) << " vs " << p_grid.level(k) << endl;
          exit(1);
        }  
    }


    // derive an array of thetas using ThetaOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( thet.calc( t_grid ) );
    theta_1 = *tmp1;
    delete tmp1;
    // use this and the temperatures to get pressure
    tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc(t_grid, theta_1) );
    p_grid2 = *tmp1;
    delete tmp1;
    if ( t_grid.vertical() != p_grid2.quantity() ) {
       cerr << "2: p_grid quantities do not match: <<" << t_grid.vertical() << ">> vs <<" <<  p_grid2.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( t_grid.vunits() != p_grid2.units() ) {
       cerr << "2: p_grid units do not match: " << t_grid.vunits() << " vs " <<  p_grid2.units() << endl;
       exit(1);
    } 
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2.level(k), t_grid.level(k)) ) {       
          cerr << "2: PressOTF levels mismatch at : " << k << p_grid2.level(k) << " vs " <<  t_grid.level(k) << endl;
          exit(1);
        }  
    }
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2( 0, 0,k), p_grid2.level(k)) 
          || mismatch( p_grid2( 0,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71, 0,k), p_grid2.level(k)) ) {
       
          cerr << "2: corner PressOTF value mismatch at k = " << k << endl;
          cerr << "  0,  0, k: " << p_grid2(  0, 0,k) << " vs " << p_grid2.level(k) << endl;
          cerr << "  0, 36, k: " << p_grid2(  0,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71, 36, k: " << p_grid2( 71,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71,  0, k: " << p_grid2( 71, 0,k) << " vs " << p_grid2.level(k) << endl;
          exit(1); 
        }  
    }
    // do this again, reversed 
    tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc(theta_1, t_grid) );
    p_grid2 = *tmp1;
    delete tmp1;
    if ( t_grid.vertical() != p_grid2.quantity() ) {
       cerr << "3: p_grid quantities do not match: <<" << t_grid.vertical() << ">> vs <<" <<  p_grid2.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( t_grid.vunits() != p_grid2.units() ) {
       cerr << "3: p_grid units do not match: " << t_grid.vunits() << " vs " <<  p_grid2.units() << endl;
       exit(1);
    } 
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2.level(k), t_grid.level(k)) ) {       
          cerr << "3: PressOTF levels mismatch at : " << k << p_grid2.level(k) << " vs " <<  t_grid.level(k) << endl;
          exit(1);
        }  
    }
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2( 0, 0,k), p_grid2.level(k)) 
          || mismatch( p_grid2( 0,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71, 0,k), p_grid2.level(k)) ) {
       
          cerr << "3: corner PressOTF value mismatch at k = " << k << endl;
          cerr << "  0,  0, k: " << p_grid2(  0, 0,k) << " vs " << p_grid2.level(k) << endl;
          cerr << "  0, 36, k: " << p_grid2(  0,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71, 36, k: " << p_grid2( 71,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71,  0, k: " << p_grid2( 71, 0,k) << " vs " << p_grid2.level(k) << endl;
          exit(1); 
        }  
    }
    
    
    // turn the temperatures into densities
    rho_grid = t_grid;
    rho_grid.set_quantity("air_density");
    rho_grid.set_units("kg/m^3");
    for ( k=0; k<18; k++ ) {
        val = rho_grid.level(k);
        for ( j=0; j<37; j++ ) {
            for ( i=0; i<72; i++ ) {
                rho_grid(i,j,k) = (val*100.0)/287.04/t_grid(i,j,k);
            }
        }
    }            
    // use this and the temperatures to get pressure
    tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc(t_grid, rho_grid) );
    p_grid2 = *tmp1;
    delete tmp1;
    if ( t_grid.vertical() != p_grid2.quantity() ) {
       cerr << "4: p_grid quantities do not match: <<" << t_grid.vertical() << ">> vs <<" <<  p_grid2.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( t_grid.vunits() != p_grid2.units() ) {
       cerr << "4: p_grid units do not match: " << t_grid.vunits() << " vs " <<  p_grid2.units() << endl;
       exit(1);
    } 
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2.level(k), t_grid.level(k)) ) {       
          cerr << "4: PressOTF levels mismatch at : " << k << p_grid2.level(k) << " vs " <<  t_grid.level(k) << endl;
          exit(1);
        }  
    }
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2( 0, 0,k), p_grid2.level(k)) 
          || mismatch( p_grid2( 0,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71, 0,k), p_grid2.level(k)) ) {
       
          cerr << "4: corner PressOTF value mismatch at k = " << k << endl;
          cerr << "  0,  0, k: " << p_grid2(  0, 0,k) << " vs " << p_grid2.level(k) << endl;
          cerr << "  0, 36, k: " << p_grid2(  0,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71, 36, k: " << p_grid2( 71,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71,  0, k: " << p_grid2( 71, 0,k) << " vs " << p_grid2.level(k) << endl;
          exit(1); 
        }  
    }
    // do it again with the args reversed
    tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc(rho_grid, t_grid) );
    p_grid2 = *tmp1;
    delete tmp1;
    if ( t_grid.vertical() != p_grid2.quantity() ) {
       cerr << "5: p_grid quantities do not match: <<" << t_grid.vertical() << ">> vs <<" <<  p_grid2.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( t_grid.vunits() != p_grid2.units() ) {
       cerr << "5: p_grid units do not match: " << t_grid.vunits() << " vs " <<  p_grid2.units() << endl;
       exit(1);
    } 
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2.level(k), t_grid.level(k)) ) {       
          cerr << "5: PressOTF levels mismatch at : " << k << p_grid2.level(k) << " vs " <<  t_grid.level(k) << endl;
          exit(1);
        }  
    }
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2( 0, 0,k), p_grid2.level(k)) 
          || mismatch( p_grid2( 0,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71, 0,k), p_grid2.level(k)) ) {
       
          cerr << "5: corner PressOTF value mismatch at k = " << k << endl;
          cerr << "  0,  0, k: " << p_grid2(  0, 0,k) << " vs " << p_grid2.level(k) << endl;
          cerr << "  0, 36, k: " << p_grid2(  0,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71, 36, k: " << p_grid2( 71,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71,  0, k: " << p_grid2( 71, 0,k) << " vs " << p_grid2.level(k) << endl;
          exit(1); 
        }  
    }
    

    // add the test of the thicknesses integration calculation here


    // construct pressure layer thicknesses
    start = 0.1;
    pdel_grid = t_grid;
    pdel_grid.set_quantity("air_layer_pressure_thickness");
    pdel_grid.set_units("hPa");
    val = 2.0*( pdel_grid.level(17) - start ); 
    for ( j=0; j<37; j++ ) {
        for ( i=0; i<72; i++ ) {
            pdel_grid(i,j,17) = val;
        }
    }
    for ( k=16; k>=0; k-- ) {
        val = 2.0*( pdel_grid.level(k) - pdel_grid.level(k+1) ) - pdel_grid(0,0,k+1);
        for ( j=0; j<37; j++ ) {
            for ( i=0; i<72; i++ ) {
                pdel_grid(i,j,k) = val;
            }
        }
    }            
    // use this and the temperatures to get pressure
    tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc( pdel_grid, start ) );
    p_grid2 = *tmp1;
    delete tmp1;
    if ( pdel_grid.vertical() != p_grid2.quantity() ) {
       cerr << "6: p_grid quantities do not match: <<" << t_grid.vertical() << ">> vs <<" <<  p_grid2.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( pdel_grid.vunits() != p_grid2.units() ) {
       cerr << "6: p_grid units do not match: " << t_grid.vunits() << " vs " <<  p_grid2.units() << endl;
       exit(1);
    } 
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2.level(k), t_grid.level(k)) ) {       
          cerr << "6: PressOTF levels mismatch at : " << k << p_grid2.level(k) << " vs " <<  t_grid.level(k) << endl;
          exit(1);
        }  
    }
    for ( k=0; k<18; k++ ) {
        if ( mismatch( p_grid2( 0, 0,k), p_grid2.level(k)) 
          || mismatch( p_grid2( 0,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71,36,k), p_grid2.level(k))
          || mismatch( p_grid2(71, 0,k), p_grid2.level(k)) ) {
       
          cerr << "6: corner PressOTF value mismatch at k = " << k << endl;
          cerr << "  0,  0, k: " << p_grid2(  0, 0,k) << " vs " << p_grid2.level(k) << endl;
          cerr << "  0, 36, k: " << p_grid2(  0,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71, 36, k: " << p_grid2( 71,36,k) << " vs " << p_grid2.level(k) << endl;
          cerr << " 71,  0, k: " << p_grid2( 71, 0,k) << " vs " << p_grid2.level(k) << endl;
          exit(1); 
        }  
    }






    
    
    // now check for things that we should not be able to do
    t_grid.set_quantity("bad stuff");
    ok = 0;
    try {
       tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc(t_grid, rho_grid) );
       p_grid2 = *tmp1;
       delete tmp1;
    } catch (...) {
       ok = 1;
    }
    if ( ok == 0 ) {
       cerr << "1: failed to catch bad T name" << endl;
    }
    t_grid.set_quantity("air_temperature");
    rho_grid.set_quantity("bad stuff");
    ok = 0;
    try {
       tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc(t_grid, rho_grid) );
       p_grid2 = *tmp1;
       delete tmp1;
    } catch (...) {
       ok = 1;
    }  
    if ( ok == 0 ) {
       cerr << "2: failed to catch bad T name" << endl;
    }
    rho_grid.set_quantity("air_density");
    
    

    // set up surfaces
    tmp2 = t_grid.extractSfc(5);
    t_sfc = *tmp2;
    delete tmp2;
    tmp2 = theta_1.extractSfc(5);
    theta_sfc = *tmp2;
    delete tmp2;
    tmp2 = rho_grid.extractSfc(5);
    rho_sfc = *tmp2;
    delete tmp2;
    val = t_grid.level(5);
    // calc pressure on this surface, using T and theta
    tmp2 = dynamic_cast<GridLatLonFieldSfc*>( press.calc( t_sfc, theta_sfc ) );
    p_sfc = *tmp2;
    delete tmp2;
    if ( t_grid.vertical() != p_sfc.quantity() ) {
       cerr << "10: p_sfc quantities do not match: <<" << t_grid.vertical() << ">> vs <<" <<  p_sfc.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( t_grid.vunits() != p_sfc.units() ) {
       cerr << "10: p_sfc units do not match: " << t_grid.vunits() << " vs " <<  p_sfc.units() << endl;
       exit(1);
    } 
    if ( mismatch( p_sfc( 0, 0), val) 
      || mismatch( p_sfc( 0,36), val)
      || mismatch( p_sfc(71,36), val)
      || mismatch( p_sfc(71, 0), val) ) {
    
      cerr << "10: corner PressOTF value mismatch: " << endl;
      cerr << "  0,  0: " << p_sfc(  0, 0) << " vs " << val << endl;
      cerr << "  0, 36: " << p_sfc(  0,36) << " vs " << val << endl;
      cerr << " 71, 36: " << p_sfc( 71,36) << " vs " << val << endl;
      cerr << " 71,  0: " << p_sfc( 71, 0) << " vs " << val << endl;
      exit(1); 
    }  
    // calc pressure on this surface, using T and density
    tmp2 = dynamic_cast<GridLatLonFieldSfc*>( press.calc( t_sfc, rho_sfc ) );
    p_sfc = *tmp2;
    delete tmp2;
    if ( t_grid.vertical() != p_sfc.quantity() ) {
       cerr << "20: p_sfc quantities do not match: <<" << t_grid.vertical() << ">> vs <<" <<  p_sfc.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( t_grid.vunits() != p_sfc.units() ) {
       cerr << "20: p_sfc units do not match: " << t_grid.vunits() << " vs " <<  p_sfc.units() << endl;
       exit(1);
    } 
    if ( mismatch( p_sfc( 0, 0), val) 
      || mismatch( p_sfc( 0,36), val)
      || mismatch( p_sfc(71,36), val)
      || mismatch( p_sfc(71, 0), val) ) {
    
      cerr << "20: corner PressOTF value mismatch: " << endl;
      cerr << "  0,  0: " << p_sfc(  0, 0) << " vs " << val << endl;
      cerr << "  0, 36: " << p_sfc(  0,36) << " vs " << val << endl;
      cerr << " 71, 36: " << p_sfc( 71,36) << " vs " << val << endl;
      cerr << " 71,  0: " << p_sfc( 71, 0) << " vs " << val << endl;
      exit(1); 
    }  

    // now check for things that we should not be able to do
    t_sfc.set_quantity("bad stuff");
    ok = 0;
    try {
       tmp2 = dynamic_cast<GridLatLonFieldSfc*>( press.calc(t_sfc, rho_sfc) );
       p_sfc = *tmp2;
       delete tmp2;
    } catch (...) {
       ok = 1;
    }  
    if ( ok == 0 ) {
       cerr << "30: failed to catch bad T name" << endl;
    }
    
    exit(0);

}
