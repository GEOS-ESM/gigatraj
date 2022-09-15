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
    GridLatLonField3D t_grid, p_grid;
    GridFieldSfc *t_sfc, *p_sfc;
    GridLatLonField3D *theta_1, *theta_2;
    GridLatLonFieldSfc *theta_3;
    GridLatLonField3D *tmp1;
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
       val = 230.0 + rnd.uniform(-5.0,5.0);
       odata.push_back( val );
    }   


    // =========================  set up the temperature grid on pressure surfaces
    t_grid.set_quantity("air_temperature");
    t_grid.set_units("Kelvin");
    t_grid.set_fillval(-1234.0);
    t_grid.set_vertical("air_pressure");
    t_grid.set_vunits("millibar");
    t_grid.mksVScale = 100.0; // (mb->Pa)
    t_grid.set_time( 3.14159, "Grue 34, year of the kumquat");
    t_grid.load( olons, olats, opres, odata );
    
    // derive a pressure grid from the T-grid's vertical coords
    tmp1 = dynamic_cast<GridLatLonField3D*>( t_grid.generateVertical() );
    p_grid = *tmp1;
    delete tmp1;
      
    val = thet.calc( 220.0, 70.0*100.0 ); // the 100.0 is to convert from mb to Pa
    val2 = 220.0 * POW( 1000.0/70.0, 2./7. );
    if ( mismatch( val, val2 ) ) {
       cerr << "single value ThetsOTF mismatch: " << val << " vs " << val2 << endl;
       exit(1);
    }

    // calculate theta
    theta_1 = dynamic_cast<GridLatLonField3D*>( thet.calc( t_grid, p_grid ) );
    val = t_grid(3,4,5) * POW( 1000.0/p_grid(3,4,5), 2./7. );
    if ( mismatch( (*theta_1)(3,4,5), val ) ) {
       cerr << "ThetaOTF 1 bad calc: " << (*theta_1)(3,4,5) << " vs " << val << endl;
       exit(1);
    }
    
    // calculate it without explicit pressures
    theta_2 = dynamic_cast<GridLatLonField3D*>( thet.calc( t_grid ) );
    val = t_grid(3,4,5) * POW( 1000.0/t_grid.level(5), 2./7. );
    if ( mismatch( (*theta_2)(3,4,5), val ) ) {
       cerr << "t=" << t_grid(3,4,5) << ", p=" << t_grid.level(5) << ", Th=" << (*theta_1)(3,4,5) << endl;
       cerr << "tx=" << t_grid(0,0,5) << ",ty=" << t_grid(3,4,0) << endl;
       cerr << "ThetaOTF 2 bad calc: " << (*theta_2)(3,4,5) << " vs " << val << endl;
       exit(1);
    }
    

    // set up surfaces
    t_sfc = t_grid.extractSfc(5);
    p_sfc = p_grid.extractSfc(5);
    // calc theta on this surface
    theta_3 = dynamic_cast<GridLatLonFieldSfc*>( thet.calc( *t_sfc, *p_sfc ) );
    val = (*t_sfc)(3,4) * POW( 1000.0/(*p_sfc)(3,4), 2./7. );
    if ( mismatch( (*theta_3)(3,4), val ) ) {
       cerr << "ThetaOTF 3 bad calc: " << (*theta_3)(3,4) << " vs " << val << endl;
       exit(1);
    }
    delete t_sfc;
    delete p_sfc;
    delete theta_1;
    delete theta_2;
    delete theta_3;
    
    exit(0);

}
