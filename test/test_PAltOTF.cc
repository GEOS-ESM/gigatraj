/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/





#include <cstdlib>
#include <iostream>
#include <sstream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/RandomSrc.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonFieldSfc.hh"
#include "gigatraj/PAltOTF.hh"
#include "gigatraj/ThetaOTF.hh"
#include "gigatraj/PressOTF.hh"
#include "gigatraj/PressOTF.hh"
#include "gigatraj/LogLinearVinterp.hh"

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
    GridLatLonField3D t_grid, a_grid, p_grid, a_grid2, p_grid2;
    GridLatLonField3D *tmp1;
    GridLatLonFieldSfc t_sfc, a_sfc, p_sfc, theta_sfc, rho_sfc;
    GridLatLonField3D theta_1, theta_2, rho_grid, pdel_grid;
    GridLatLonFieldSfc *tmp2;
    std::vector<real> olons;
    std::vector<real> olats;
    std::vector<real> opres;
    std::vector<real> odata;
    std::vector<real> oalt;
    std::vector<real> otheta;
    int i, j, k, idx;
    int n1, n2, n3;
    real val, val2;
    real bad;
    int ival, ival2;
    double tyme;
    string str;
    int ia[3], ja[3], ka[3];
    real da[3];
    GridLatLonField3D const *gridy;
    GridField3D::iterator looper;
    GridField3D::const_iterator clooper;
    PAltOTF palt;
    ThetaOTF thet;
    PressOTF press;
    int ok;
    real start;
    LogLinearVinterp vin;
    
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
    for (i=0; i<18; i++ ) {
       //cerr << i << ": " << opres[i] << endl;
       val = palt.calc( opres[i]*100.0 );
       //cerr << "      " << val << endl;
       oalt.push_back( val );
    }  
    if ( mismatch( opres[11], 50.0) || mismatch( oalt[11],20.5767) ) {       
      cerr << "PAltOTF pressure level conversion mismatch at 50 mb:" << oalt[11] << " vs. " << 20.5767 << endl;
      exit(1);
    }
    val2 = palt.clac( oalt[11] )/100.0; 
    if ( mismatch( opres[11], val2) ) {       
      cerr << "PAltOTF altitude conversion mismatch at 50 mb:" << val2 << " vs. " << oalt[11] << endl;
      exit(1);
    }
    
    otheta.reserve(11);
    otheta.push_back(380.0);
    otheta.push_back(400.0);
    otheta.push_back(420.0);
    otheta.push_back(460.0);
    otheta.push_back(500.0);
    otheta.push_back(540.0);
    otheta.push_back(600.0);
    otheta.push_back(700.0);
    otheta.push_back(800.0);
    otheta.push_back(1000.0);
    otheta.push_back(20000.0); // this level should end up with fill values
     

    bad = -1234.0;

    // =========================  set up the temperature grid on pressure surfaces
    t_grid.set_quantity("air_temperature");
    t_grid.set_units("Kelvin");
    t_grid.set_fillval(bad);
    t_grid.set_vertical("air_pressure");
    t_grid.set_vunits("hPa");
    t_grid.mksVScale = 100.0; // (mb->Pa)
    t_grid.set_time( 3.14159, "Grue 34, year of the kumquat");
    t_grid.load( olons, olats, opres, odata );
    

    // derive an altitude grid from this grid using PAltOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( palt.calc(t_grid) );
    a_grid = *tmp1;
    delete tmp1;

    if ( a_grid.quantity() != "pressure_altitude" ) {
       cerr << "1: a_grid quantities do not match:pressure_altitude vs " <<  a_grid.quantity() << endl;
       exit(1);
    }   
    if ( a_grid.units() != "km" ) {
       cerr << "1: a_grid units do not match: km vs " <<  a_grid.units() << endl;
       exit(1);
    } 
    if ( a_grid.vertical() != "air_pressure" ) {
       cerr << "1: a_grid vertical coordinate does not match: air_pressure vs " <<  a_grid.vertical() << endl;
       exit(1);
    }   
    for ( k=0; k<18; k++ ) {
        if ( mismatch( a_grid.level(k), t_grid.level(k)) ) {       
          cerr << "1: PressOTF levels mismatch at : " << k << a_grid.level(k) << " vs " <<  t_grid.level(k) << endl;
          exit(1);
        }  
    }
    for ( k=0; k<18; k++ ) {
        if ( mismatch( a_grid( 0, 0,k), oalt[k]) 
          || mismatch( a_grid( 0,36,k), oalt[k])
          || mismatch( a_grid(71,36,k), oalt[k])
          || mismatch( a_grid(71, 0,k), oalt[k]) ) {
       
          cerr << "1: corner PressOTF value mismatch at : " << k << endl;
          cerr << "  0,  0, k: " << a_grid(  0, 0,k) << " vs " << oalt[k] << endl;
          cerr << "  0, 36, k: " << a_grid(  0,36,k) << " vs " << oalt[k] << endl;
          cerr << " 71, 36, k: " << a_grid( 71,36,k) << " vs " << oalt[k] << endl;
          cerr << " 71,  0, k: " << a_grid( 71, 0,k) << " vs " << oalt[k] << endl;
          exit(1);
        }  
    }



    // Now we want to get the pressure altitudes from an array of pressures on theta surfaces
    
    // first, derive the thetas from the temperatures on pressure surfaces
    tmp1 = dynamic_cast<GridLatLonField3D*>( thet.calc( t_grid ) );
    theta_1 = *tmp1;
    delete tmp1;
   
    // next, get the array of pressures on presure surfaces
    tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc(t_grid, theta_1) );
    p_grid = *tmp1;
    delete tmp1;
    
    // now derive the pressures on theta surfaces from the pressures on theta surfaces 
    // and thetas on pressure surfaces
    tmp1 = dynamic_cast<GridLatLonField3D*>(vin.invert( otheta, theta_1 ));
    p_grid2 = *tmp1;
    delete tmp1;
    
    // now get the pressure altitudes on theta surfaces
    tmp1 = dynamic_cast<GridLatLonField3D*>( palt.calc(p_grid2) );
    a_grid2 = *tmp1;
    delete tmp1;
    if ( a_grid2.quantity() != "pressure_altitude" ) {
       cerr << "2: a_grid quantities do not match:pressure_altitude vs " <<  a_grid2.quantity() << endl;
       exit(1);
    }   
    if ( a_grid2.units() != "km" ) {
       cerr << "2: a_grid units do not match: km vs " <<  a_grid2.units() << endl;
       exit(1);
    } 
    if ( a_grid2.vertical() != "air_potential_temperature" ) {
       cerr << "1: a_grid2 vertical coordinate does not match: air_potnetial_temperature vs " <<  a_grid2.vertical() << endl;
       exit(1);
    }   

    a_grid2.dims( &n1, &n2, &n3 );
    for ( k=0; k<n3; k++ ) {

        val = a_grid2( 0, 0,k);
        val2 = p_grid2( 0, 0,k);
        if ( val2 != bad ) {
           val2 =  palt.calc( val2*100.0 );
        } else {
           val2 = bad;
        }
        if ( mismatch( val, val2 ) ) {
          cerr << "  0,  0, k: " << val	 << " vs " << val2 << endl;
          exit(1);
        }  
        val = a_grid2( 0,36,k);
        val2 = p_grid2( 0,36,k);
        if ( val2 != bad ) {
           val2 =  palt.calc( val2*100.0 );
        } else {
           val2 = bad;
        }
        if ( mismatch( val, val2 ) ) {
          cerr << "  0, 36, k: " << val	 << " vs " << val2 << endl;
          exit(1);
        }  
        val = a_grid2(71,36,k);
        val2 = p_grid2(71,36,k);
        if ( val2 != bad ) {
           val2 =  palt.calc( val2*100.0 );
        } else {
           val2 = bad;
        }
        if ( mismatch( val, val2 ) ) {
          cerr << " 71, 36, k: " << val	 << " vs " << val2 << endl;
          exit(1);
        }  
        val = a_grid2(71, 0,k);
        val2 = p_grid2(71, 0,k);
        if ( val2 != bad ) {
           val2 =  palt.calc( val2*100.0 );
        } else {
           val2 = bad;
        }
        if ( mismatch( val, val2 ) ) {
          cerr << " 71,  0, k: " << val	 << " vs " << val2 << endl;
          exit(1);
        }  

    }

    // now check for things that we should not be able to do
    p_grid2.set_quantity("bad stuff");
    ok = 0;
    try {
       tmp1 = dynamic_cast<GridLatLonField3D*>( palt.calc(p_grid2) );
       a_grid2 = *tmp1;
       delete tmp1;
    } catch (...) {
       ok = 1;
    }
    if ( ok == 0 ) {
       cerr << "1: failed to catch bad Press name" << endl;
    }
    p_grid2.set_quantity("air_pressure");


    // set up surfaces
    tmp2 = p_grid.extractSfc(5);
    p_sfc = *tmp2;
    delete tmp2;

    // calc palt on this surface, using press
    tmp2 = dynamic_cast<GridLatLonFieldSfc*>( palt.calc( p_sfc ) );
    a_sfc = *tmp2;
    delete tmp2;
    if ( a_grid.quantity() != a_sfc.quantity() ) {
       cerr << "10: a_sfc quantities do not match: <<" << a_grid.quantity() << ">> vs <<" <<  a_sfc.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( a_grid.units() != a_sfc.units() ) {
       cerr << "10: a_sfc units do not match: " << a_grid.units() << " vs " <<  a_sfc.units() << endl;
       exit(1);
    } 
    val = palt.calc( 100.0*a_grid.level(5) );
    if ( mismatch( a_sfc( 0, 0), val) 
      || mismatch( a_sfc( 0,36), val)
      || mismatch( a_sfc(71,36), val)
      || mismatch( a_sfc(71, 0), val) ) {
    
      cerr << "10: corner PAltOTF surface value mismatch: " << endl;
      cerr << "  0,  0: " << a_sfc(  0, 0) << " vs " << val << endl;
      cerr << "  0, 36: " << a_sfc(  0,36) << " vs " << val << endl;
      cerr << " 71, 36: " << a_sfc( 71,36) << " vs " << val << endl;
      cerr << " 71,  0: " << a_sfc( 71, 0) << " vs " << val << endl;
      exit(1); 
    }  


    // now check for things that we should not be able to do
    p_sfc.set_quantity("bad stuff");
    ok = 0;
    try {
       tmp2 = dynamic_cast<GridLatLonFieldSfc*>( palt.calc(p_sfc) );
       a_sfc = *tmp2;
       delete tmp2;
    } catch (...) {
       ok = 1;
    }  
    if ( ok == 0 ) {
       cerr << "30: failed to catch bad Press name" << endl;
    }
    
    exit(0);

}
