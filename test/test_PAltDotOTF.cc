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
#include "gigatraj/LinearVinterp.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonFieldSfc.hh"
#include "gigatraj/ThetaOTF.hh"
#include "gigatraj/PressOTF.hh"
#include "gigatraj/PAltDotOTF.hh"

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
    GridLatLonField3D t_grid, p_grid, theta_grid, tdot_grid, omega_grid, palt_grid;
    GridLatLonField3D t_onh, p_onh, theta_onh, tdot_onh, omega_onh, palt_onh;
    GridLatLonField3D paltdot_grid, paltdot_grid2;
    GridLatLonField3D *tmp1;
    GridLatLonFieldSfc t_sfc, p_sfc, theta_sfc, palt_sfc, omega_sfc;
    GridLatLonFieldSfc paltdot_sfc, paltdot_sfc2;
    GridLatLonFieldSfc *tmp2;
    std::vector<real> olons;
    std::vector<real> olats;
    std::vector<real> opres;
    std::vector<real> odata;
    std::vector<real> oz;
    std::vector<real> odzdp;
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
    PAltOTF palt;
    PAltDotOTF paltdot;
    int ok;
    real start;
    real x1, x2, x3, y1, y2, y3, a, b, c;
    // pointer to the vertical interpolator used
    Vinterp *vin;
    // theta coordinate values
    std::vector<real> thvals;
    
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
    // convert pressure to altitude
    oz.reserve(18);
    for ( i=0; i<18; i++ ) {
        oz.push_back( palt.calc( opres[i]*100.0 ) );
    }
    // compute d(palt)/d(p)
    odzdp.reserve(18);
    // do the 0th point
    y1 = oz[0];
    y2 = oz[1];
    y3 = oz[2];
    x1 = opres[0];
    x2 = opres[1];
    x3 = opres[2];
    c = (((y1 - y2)/(x2 - x1)) + ((y3 - y2)/(x3 - x2)))/(x3 - x1);
    b = ((x2*x2*(y1 - y3)) + (x3*x3*(y2 - y1)) + (x1*x1*(y3 - y2)))/((x2 - x1)*(x3 - x1)*(x3 - x2));
    odzdp.push_back( b + 2*c*x1 );
    // do points 1 through 16
    for (i=1; i<17; i++ ) {
       y1 = oz[i-1];
       y2 = oz[i];
       y3 = oz[i+1];
       x1 = opres[i-1];
       x2 = opres[i];
       x3 = opres[i+1];
       c = (((y1 - y2)/(x2 - x1)) + ((y3 - y2)/(x3 - x2)))/(x3 - x1);
       b = ((x2*x2*(y1 - y3)) + (x3*x3*(y2 - y1)) + (x1*x1*(y3 - y2)))/((x2 - x1)*(x3 - x1)*(x3 - x2));
       odzdp.push_back( b + 2*c*x2 );    
    }
    // do the last point
    y1 = oz[15];
    y2 = oz[16];
    y3 = oz[17];
    x1 = opres[15];
    x2 = opres[16];
    x3 = opres[17];
    c = (((y1 - y2)/(x2 - x1)) + ((y3 - y2)/(x3 - x2)))/(x3 - x1);
    b = ((x2*x2*(y1 - y3)) + (x3*x3*(y2 - y1)) + (x1*x1*(y3 - y2)))/((x2 - x1)*(x3 - x1)*(x3 - x2));
    odzdp.push_back( b + 2*c*x3 );
    
    // test the derivative
    i = 5;
    val = paltdot.get_dzdp( oz[i] ); // km/Pa
    if ( mismatch( odzdp[i]*100.0, val, 0.05 ) ) {
       cerr << "mismatch deriv at " << i << ": "  <<  odzdp[i]*100.0 << " vs. " <<  val <<  endl;
       exit(1);    
    }
    // now replace those dpdz values with the object's
    for ( i=0; i<18; i++ ) {
        odzdp[i] = 1000.0 * paltdot.get_dzdp( oz[i] );  // km/Pa -> m/Pa
    }
    
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

    // derive a pressure altitude grid from temperature using PAltOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( palt.calc(t_grid) );
    palt_grid = *tmp1;
    delete tmp1;

    // derive a theta grid from temperature using ThetaOTF
    tmp1 = dynamic_cast<GridLatLonField3D*>( thet.calc(t_grid) );
    theta_grid = *tmp1;
    delete tmp1;
    
    
    // =========================  set up the pressure tendency grid on pressure surfaces
    omega_grid.set_quantity("lagrangian_tendency_of_air_pressure");
    omega_grid.set_units("Pa/s");
    omega_grid.set_fillval(-1234.0);
    omega_grid.set_vertical("air_pressure");
    omega_grid.set_vunits("hPa");
    omega_grid.mksVScale = 100.0; // (mb->Pa)
    omega_grid.set_time( 3.14159, "Grue 34, year of the kumquat");
    for (k=0; k<18; k++ ) {
    for (j=0; j<37; j++ ) {
    for (i=0; i<72; i++ ) {       
       val = rnd.uniform(-0.1,0.1) * opres[k];
       odata[i+j*72+k*72*37] = val;
    }   
    }
    }
    omega_grid.load( olons, olats, opres, odata );



    // set up theta coordinates
    thvals.reserve(9);
    thvals.push_back(    290.0); 
    thvals.push_back(    325.0); 
    thvals.push_back(    380.0); 
    thvals.push_back(    500.0); 
    thvals.push_back(    600.0); 
    thvals.push_back(    760.0); 
    thvals.push_back(    885.0); 
    thvals.push_back(   1085.0); 
    thvals.push_back(   1450.0); 

    // get pressure on theta surfaces
    vin = new LinearVinterp();
    tmp1 = dynamic_cast<GridLatLonField3D*>( vin->invert( thvals, theta_grid ) );
    p_onh = *tmp1;
    delete tmp1;
    
    // create a grid of theta on theta surfaces (for use below)
    tmp1 = dynamic_cast<GridLatLonField3D*>( p_onh.generateVertical() );
    theta_onh = *tmp1;
    delete tmp1;
    // compute temperature from theta, on theta surfaces
    t_onh = p_onh;
    t_onh.set_quantity( t_grid.quantity() ); 
    t_onh.set_units( "K" );
    for (k=0; k<9; k++ ) {
    for (j=0; j<37; j++ ) {
    for (i=0; i<72; i++ ) {       
       t_onh(i,j,k) = theta_onh(i,j,k) * POW( 1000.0/p_onh(i,j,k), -2./7. );
    }   
    }
    }
    // now put omega on theta surfaces
    tmp1 = dynamic_cast<GridLatLonField3D*>( vin->reProfile( omega_grid, p_onh ) ); 
    omega_onh = *tmp1;
    delete tmp1;
    delete vin;

    // get palt on theta surfaces
    tmp1 = dynamic_cast<GridLatLonField3D*>( palt.calc(p_onh) );
    palt_onh = *tmp1;
    delete tmp1;


    ////////////////////// 
    val =  omega_grid(36,18,9) * odzdp[9];  // (note: mks units)

    // derive a altitude tendency grid from omega on P surfaces 
    tmp1 = dynamic_cast<GridLatLonField3D*>( paltdot.calc(omega_grid) );
    paltdot_grid = *tmp1;  
    delete tmp1;
    if ( ! t_grid.compatible( paltdot_grid ) ) {
       cerr << "T grid is not grid-compatible with paltdot grid " << endl;
       exit(1);
    }   
    if ( paltdot_grid.quantity() != "tendency_of_pressure_altitude" ) {
       cerr << "paltdot_grid quantity is not density: "  << paltdot_grid.quantity() << endl;
       exit(1);
    }   
    if ( paltdot_grid.units() != "m/s" ) {
       cerr << "paltdot_grid units are not m/s: "  << paltdot_grid.units() << endl;
       exit(1);
    } 
    if ( mismatch( paltdot_grid(36,18,9), val ) ) {
       cerr << "mismatch 01 at (36,18,9): "  <<  paltdot_grid(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }
    tmp1 = dynamic_cast<GridLatLonField3D*>( paltdot.calc(omega_grid, OTF_MKS) );
    paltdot_grid = *tmp1;  
    delete tmp1;
    if ( mismatch( paltdot_grid(36,18,9), val ) ) {
       cerr << "mismatch 01b at (36,18,9): "  <<  paltdot_grid(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // sanity check
    for (k=0; k<9; k++ ) {
    for (j=0; j<37; j++ ) {
    for (i=0; i<72; i++ ) {       
       if ( p_onh(i,j,k) < 0.0 ) {
          cerr << " p(" << i << ", " << j << ", " << k << ") = " << p_onh(i,j,k) << endl;
          exit(1);
       }
    }   
    }
    }
  
  
    
    // derive a altitude tendency grid from omega on theta surfaces 
    val =  palt_onh(36,18,4);
    tmp1 = dynamic_cast<GridLatLonField3D*>( paltdot.calc(omega_onh, palt_onh) );
    paltdot_grid2 = *tmp1;  
    delete tmp1;
    if ( ! t_grid.compatible( paltdot_grid ) ) {
       cerr << "T grid is not grid-compatible with paltdot grid " << endl;
       exit(1);
    }   
    if ( paltdot_grid.quantity() != "tendency_of_pressure_altitude" ) {
       cerr << "paltdot_grid quantity is not density: "  << paltdot_grid.quantity() << endl;
       exit(1);
    }   
    if ( paltdot_grid.units() != "m/s" ) {
       cerr << "paltdot_grid units are not m/s: "  << paltdot_grid.units() << endl;
       exit(1);
    } 
    val = omega_onh(36,18,4) * paltdot.get_dzdp( val );  // note: km/s  
    if ( mismatch( paltdot_grid2(36,18,4), val ) ) {
       cerr << "mismatch 02 at (36,18,4): "  <<  paltdot_grid2(36,18,4) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    tmp1 = dynamic_cast<GridLatLonField3D*>( paltdot.calc(omega_onh, palt_onh, OTF_MKS) );
    paltdot_grid2 = *tmp1;  
    delete tmp1;
    if ( mismatch( paltdot_grid2(36,18,4), val*1000.0 ) ) {
       cerr << "mismatch 02b at (36,18,4): "  <<  paltdot_grid2(36,18,4) << " vs. " <<  val*1000.0 <<  endl;
       exit(1);    
    }

    // now check for things that we should not be able to do
    omega_grid.set_quantity("bad stuff");
    ok = 0;
    try {
       tmp1 = dynamic_cast<GridLatLonField3D*>( paltdot.calc(omega_grid) );
    } catch (...) {
       ok = 1;
    }  
    if ( ok == 0 ) {
       cerr << "1: failed to catch bad Omega name" << endl;
    }
    ok = 0;
    omega_grid.set_quantity("lagrangian_tendency_of_air_pressure");

    

    // set up surfaces
    tmp2 = omega_grid.extractSfc(5);
    omega_sfc = *(tmp2);
    delete tmp2;
    tmp2 = palt_grid.extractSfc(5);
    palt_sfc = *(tmp2);
    delete tmp2;
    tmp2 = paltdot_grid.extractSfc(5);
    paltdot_sfc = *(tmp2);
    delete tmp2;
    tmp2 = dynamic_cast<GridLatLonFieldSfc*>( paltdot.calc( omega_sfc, palt_sfc, OTF_MKS ) );
    paltdot_sfc2 = *(tmp2);
    delete tmp2;
    if ( paltdot_sfc2.quantity() != paltdot_grid.quantity() ) {
       cerr << "10: paltdot_sfc and paltdot_grid quantities do not match: <<" << paltdot_grid.quantity() << ">> vs <<" <<  paltdot_sfc.quantity() << ">>" << endl;
       exit(1);
    }   
    if ( paltdot_sfc.units() != paltdot_sfc2.units() ) {
       cerr << "10: paltdot_sfc units do not match: " << paltdot_sfc2.units() << " vs " <<  paltdot_sfc.units() << endl;
       exit(1);
    }
    val = paltdot_sfc(20,18);
    val2 = paltdot_sfc2(20,18);
    if ( mismatch( val, val2 ) ) {
       cerr << "10: paltdot sfc calc failed "  <<  val << " vs. " <<  val2 <<  endl;
       exit(1);    
    }

/*    


    // calc density on this surface, using T and P, T and theta, and P and theta
    rho_sfc2 = dynamic_cast<GridLatLonFieldSfc&>( dens.calc( t_sfc, theta_sfc ) );
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
    

*/



    
    exit(0);

}
