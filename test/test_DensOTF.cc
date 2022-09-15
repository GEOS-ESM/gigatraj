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
#include "gigatraj/ThetaOTF.hh"
#include "gigatraj/PressOTF.hh"
#include "gigatraj/DensOTF.hh"

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
    GridLatLonField3D t_grid, p_grid, theta_grid;
    GridLatLonField3D *tmp1;
    GridLatLonField3D rho_grid, rho_grid2;
    GridLatLonFieldSfc t_sfc, p_sfc, theta_sfc;
    GridLatLonFieldSfc rho_sfc, rho_sfc2;
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
    GridFieldSfc::iterator slooper;
    ThetaOTF thet;
    PressOTF press;
    DensOTF dens;
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
    
    // derive a pressure grid from temperature using PressOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( press.calc(t_grid) );
    p_grid = *tmp1;
    delete tmp1;

    // derive a pressure grid from temperature using ThetaOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( thet.calc(t_grid) );
    theta_grid = *tmp1;
    delete tmp1;
    
    
    // derive a density grid from T on P surfaces using DensOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( dens.calc(t_grid) );
    rho_grid = *tmp1;
    delete tmp1;    
    if ( ! t_grid.compatible( rho_grid ) ) {
       cerr << "T grid is not grid-compatible with rho grid " << endl;
       exit(1);
    }   
    if ( rho_grid.quantity() != "air_density" ) {
       cerr << "rho_grid quantity is not density: "  << rho_grid.quantity() << endl;
       exit(1);
    }   
    if ( rho_grid.units() != "kg/m^3" ) {
       cerr << "rho_grid units are not kg/m^2: "  << rho_grid.units() << endl;
       exit(1);
    } 
    val = t_grid.level(9) * 100.0 / 287.04 / t_grid(36,18,9);
    if ( mismatch( rho_grid(36,18,9), val ) ) {
       cerr << "mismatch at (36,18,9): "  <<  rho_grid(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }
    
    // derive a density grid from Theta on pressure surfaces using DensOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( dens.calc(theta_grid) ); 
    rho_grid2 = *tmp1;
    delete tmp1;   
    if ( ! rho_grid.compatible( rho_grid2 ) ) {
       cerr << "1: rho grid #2 is not grid-compatible with rho grid " << endl;
       exit(1);
    }   
    if ( rho_grid2.quantity() != rho_grid.quantity() ) {
       cerr << "1: rho_grid #2 quantity is not density: "  << rho_grid2.quantity() << endl;
       exit(1);
    }   
    if ( rho_grid2.units() != rho_grid.units() ) {
       cerr << "1: rho_grid #2 units are not kg/m^2: "  << rho_grid2.units() << endl;
       exit(1);
    } 
    val = rho_grid(36,18,9);
    if ( mismatch( rho_grid2(36,18,9), val ) ) {
       cerr << "1: mismatch at (36,18,9): "  <<  rho_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }
    
    // derive a density grid from temperature and Theta using DensOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( dens.calc(t_grid, theta_grid) );    
    rho_grid2 = *tmp1;
    delete tmp1;
    if ( ! rho_grid.compatible( rho_grid2 ) ) {
       cerr << "2: rho grid #2 is not grid-compatible with rho grid " << endl;
       exit(1);
    }   
    if ( rho_grid2.quantity() != rho_grid.quantity() ) {
       cerr << "2: rho_grid #2 quantity is not density: "  << rho_grid2.quantity() << endl;
       exit(1);
    }   
    if ( rho_grid2.units() != rho_grid.units() ) {
       cerr << "2: rho_grid #2 units are not kg/m^2: "  << rho_grid2.units() << endl;
       exit(1);
    } 
    val = rho_grid(36,18,9);
    if ( mismatch( rho_grid2(36,18,9), val ) ) {
       cerr << "2: mismatch at (36,18,9): "  <<  rho_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }
    
    // derive a density grid from pressure and Theta using DensOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( dens.calc(theta_grid, p_grid) );    
    rho_grid2 = *tmp1;
    delete tmp1;
    if ( ! rho_grid.compatible( rho_grid2 ) ) {
       cerr << "3: rho grid #2 is not grid-compatible with rho grid " << endl;
       exit(1);
    }   
    if ( rho_grid2.quantity() != rho_grid.quantity() ) {
       cerr << "3: rho_grid #2 quantity is not density: "  << rho_grid2.quantity() << endl;
       exit(1);
    }   
    if ( rho_grid2.units() != rho_grid.units() ) {
       cerr << "3: rho_grid #2 units are not kg/m^2: "  << rho_grid2.units() << endl;
       exit(1);
    } 
    val = rho_grid(36,18,9);
    if ( mismatch( rho_grid2(36,18,9), val ) ) {
       cerr << "3: mismatch at (36,18,9): "  <<  rho_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // now check for things that we should not be able to do
    t_grid.set_quantity("bad stuff");
    ok = 0;
    try {
       tmp1 = dynamic_cast<GridLatLonField3D*>( dens.calc(t_grid) );
       rho_grid2 = *tmp1;
       delete tmp1;
    } catch (...) {
       ok = 1;
    }  
    if ( ok == 0 ) {
       cerr << "1: failed to catch bad T name" << endl;
    }
    ok = 0;
    try {
       tmp1 = dynamic_cast<GridLatLonField3D*>( dens.calc(t_grid, theta_grid) );
       rho_grid2 = *tmp1;
       delete tmp1;
    } catch (...) {
       ok = 1;
    }  
    if ( ok == 0 ) {
       cerr << "2: failed to catch bad T name" << endl;
    }
    t_grid.set_quantity("air_temperature");


    // set up surfaces
    tmp2 = t_grid.extractSfc(5);
    t_sfc = *tmp2;
    delete tmp2;
    tmp2 = theta_grid.extractSfc(5);
    theta_sfc = *tmp2;
    delete tmp2;
    tmp2 = p_grid.extractSfc(5);
    p_sfc = *tmp2;
    delete tmp2;
    tmp2 = rho_grid.extractSfc(5);
    rho_sfc = *tmp2;
    delete tmp2;
    
    val = rho_grid(20,18,5);
    val2 = rho_sfc(20,18);
    if ( mismatch( val, val2 ) ) {
       cerr << "rho sfc extraction failed "  <<  val << " vs. " <<  val2 <<  endl;
       exit(1);    
    }
    // calc density on this surface, using T and P, T and theta, and P and theta
    tmp2 = dynamic_cast<GridLatLonFieldSfc*>( dens.calc( t_sfc, theta_sfc ) );
    rho_sfc2 = *tmp2;
    delete tmp2;
    if ( rho_sfc2.quantity() != rho_grid.quantity() ) {
       cerr << "10: rho_sfc and rho_grid quantities do not match: <<" << rho_grid.quantity() << ">> vs <<" <<  rho_sfc.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( rho_sfc.units() != rho_sfc2.units() ) {
       cerr << "10: rho_sfc units do not match: " << rho_sfc2.units() << " vs " <<  rho_sfc.units() << endl;
       exit(1);
    }
    val = rho_sfc(20,18);
    val2 = rho_sfc2(20,18);
    if ( mismatch( val, val2 ) ) {
       cerr << "10: dens sfc calc failed "  <<  val << " vs. " <<  val2 <<  endl;
       exit(1);    
    }
    




    
    exit(0);

}
