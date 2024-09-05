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
#include "gigatraj/ThetaDotOTF.hh"

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
    GridLatLonField3D t_grid, p_grid, theta_grid, tdot_grid, omega_grid;
    GridLatLonField3D t_onh, p_onh, theta_onh, tdot_onh, omega_onh;
    GridLatLonField3D thdot_grid, thdot_grid2;
    GridLatLonField3D *tmp1;
    GridLatLonFieldSfc t_sfc, p_sfc, theta_sfc;
    GridLatLonFieldSfc thdot_sfc, thdot_sfc2;
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
    ThetaDotOTF thdot;
    int ok;
    real start;
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
    
    // =========================  set up the temperature tendency grid on pressure surfaces
    tdot_grid.set_quantity("tendency_of_air_temperature");
    tdot_grid.set_units("K/s");
    tdot_grid.set_fillval(-1234.0);
    tdot_grid.set_vertical("air_pressure");
    tdot_grid.set_vunits("hPa");
    tdot_grid.mksVScale = 100.0; // (mb->Pa)
    tdot_grid.set_time( 3.14159, "Grue 34, year of the kumquat");
    for (i=0; i<72*37*18; i++ ) {
       val = rnd.uniform(-1.0,1.0);
       odata[i] = val;
    }   
    tdot_grid.load( olons, olats, opres, odata );
    
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
    thvals.reserve(18);
    thvals.push_back(    290.0); 
    thvals.push_back(    325.0); 
    thvals.push_back(    380.0); 
    thvals.push_back(    500.0); 
    thvals.push_back(    600.0); 
    thvals.push_back(    760.0); 
    thvals.push_back(    885.0); 
    thvals.push_back(   1085.0); 
    thvals.push_back(   1450.0); 
    thvals.push_back(   2170.0); 
    thvals.push_back(   3100.0); 
    thvals.push_back(   4350.0); 
    thvals.push_back(   7350.0); 
    thvals.push_back(  22770.0);
    thvals.push_back(  47850.0);
    thvals.push_back( 129000.0);
    thvals.push_back( 270700.0);
    thvals.push_back( 648600.0);
    
    vin = new LinearVinterp();
    tmp1 = dynamic_cast<GridLatLonField3D*>( vin->invert( thvals, theta_grid ) );
    p_onh = *tmp1;
    delete tmp1;
    tmp1 = dynamic_cast<GridLatLonField3D*>( p_onh.generateVertical() );
    theta_onh = *tmp1;
    delete tmp1;
    //tmp1 = dynamic_cast<GridLatLonField3D*>( vin->reProfile( t_grid, p_onh ) ); 
    //t_onh = *tmp1;
    //delete tmp1;
    t_onh = p_onh;
    t_onh.set_quantity( t_grid.quantity() ); 
    t_onh.set_units( "K" );
    for (k=0; k<18; k++ ) {
    for (j=0; j<37; j++ ) {
    for (i=0; i<72; i++ ) {       
       t_onh(i,j,k) = theta_onh(i,j,k) * POW( 1000.0/p_onh(i,j,k), -2./7. );
       //if ( i == 36 && j==18 && k == 4 ) {
       //   cerr << "thx=" << theta_onh(i,j,k) << ", p=" << p_onh(i,j,k) << ", t=" <<  t_onh(i,j,k) << endl;
       //}
    }   
    }
    }
    tmp1 = dynamic_cast<GridLatLonField3D*>( vin->reProfile( tdot_grid, p_onh ) );
    tdot_onh = *tmp1;
    delete tmp1;
    tmp1 = dynamic_cast<GridLatLonField3D*>( vin->reProfile( omega_grid, p_onh ) ); 
    omega_onh = *tmp1;
    delete tmp1;
    delete vin;










    ////////////////////// tests without omega term:
    val =  theta_grid(36,18,9) * tdot_grid(36,18,9) / t_grid(36,18,9);

    // derive a theta tendency grid from Tdot and T on P surfaces 
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, t_grid) );
    thdot_grid = *tmp1;  
    delete tmp1;
    if ( ! t_grid.compatible( thdot_grid ) ) {
       cerr << "T grid is not grid-compatible with thdot grid " << endl;
       exit(1);
    }   
    if ( thdot_grid.quantity() != "tendency_of_air_potential_temperature" ) {
       cerr << "thdot_grid quantity is not density: "  << thdot_grid.quantity() << endl;
       exit(1);
    }   
    if ( thdot_grid.units() != "K/s" ) {
       cerr << "thdot_grid units are not K/s: "  << thdot_grid.units() << endl;
       exit(1);
    } 
    if ( mismatch( thdot_grid(36,18,9), val ) ) {
       cerr << "mismatch 01 at (36,18,9): "  <<  thdot_grid(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // derive a theta tendency grid from Tdot and Theta on P surfaces 
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, theta_grid) ); 
    thdot_grid2 = *tmp1;
    delete tmp1;   
    if ( ! thdot_grid.compatible( thdot_grid2 ) ) {
       cerr << "thdot2 grid is not grid-compatible with thdot grid 02" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,9), val ) ) {
       cerr << "mismatch 02 at (36,18,9): "  <<  thdot_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // derive a theta tendency grid from Tdot, T, pressure
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, t_grid, p_grid) );
    thdot_grid2 = *tmp1;  
    delete tmp1;   
    if ( ! thdot_grid.compatible( thdot_grid2 ) ) {
       cerr << "thdot2 grid is not grid-compatible with thdot grid 03" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,9), val ) ) {
       cerr << "mismatch 03 at (36,18,9): "  <<  thdot_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // derive a theta tendency grid from Tdot, T, theta
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, t_grid, theta_grid) ); 
    thdot_grid2 = *tmp1;   
    delete tmp1;   
    if ( ! thdot_grid.compatible( thdot_grid2 ) ) {
       cerr << "thdot2 grid is not grid-compatible with thdot grid 04" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,9), val ) ) {
       cerr << "mismatch 04 at (36,18,9): "  <<  thdot_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // derive a theta tendency grid from Tdot, theta, p
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, theta_grid, p_grid) );
    thdot_grid2 = *tmp1; 
    delete tmp1;   
    if ( ! thdot_grid.compatible( thdot_grid2 ) ) {
       cerr << "thdot2 grid is not grid-compatible with thdot grid 05" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,9), val ) ) {
       cerr << "mismatch 05 at (36,18,9): "  <<  thdot_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }



    ////////////////////// tests without omega term:
    val =  theta_grid(36,18,9) * ( tdot_grid(36,18,9) / t_grid(36,18,9) - 2./7.*omega_grid(36,18,9)/(p_grid(36,18,9)*100.0) );

    // derive a theta tendency grid from Tdot, T, and omega on P surfaces 
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, t_grid, omega_grid) ); 
    thdot_grid2 = *tmp1; 
    delete tmp1;   
    if ( ! thdot_grid.compatible( thdot_grid2 ) ) {
       cerr << "thdot2 grid is not grid-compatible with thdot grid 11" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,9), val ) ) {
       cerr << "mismatch 11 at (36,18,9): "  <<  thdot_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // derive a theta tendency grid from Tdot, theta, and omega on P surfaces 
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, theta_grid, omega_grid) ); 
    thdot_grid2 = *tmp1;   
    delete tmp1;   
    if ( ! thdot_grid.compatible( thdot_grid2 ) ) {
       cerr << "thdot2 grid is not grid-compatible with thdot grid 12" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,9), val ) ) {
       cerr << "mismatch 12 at (36,18,9): "  <<  thdot_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // derive a theta tendency grid from Tdot, t,  p, and omega
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, t_grid, p_grid, omega_grid) ); 
    thdot_grid2 = *tmp1;   
    delete tmp1;   
    if ( ! thdot_grid.compatible( thdot_grid2 ) ) {
       cerr << "thdot2 grid is not grid-compatible with thdot grid 30" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,9), val ) ) {
       cerr << "mismatch 30 at (36,18,9): "  <<  thdot_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // derive a theta tendency grid from Tdot, t,  theta, and omega
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, t_grid, theta_grid, omega_grid) );
    thdot_grid2 = *tmp1; 
    delete tmp1;   
    if ( ! thdot_grid.compatible( thdot_grid2 ) ) {
       cerr << "thdot2 grid is not grid-compatible with thdot grid 31" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,9), val ) ) {
       cerr << "mismatch 31 at (36,18,9): "  <<  thdot_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // derive a theta tendency grid from Tdot, theta,  p, and omega
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_grid, theta_grid, p_grid, omega_grid) );    
    thdot_grid2 = *tmp1;
    delete tmp1;
    if ( ! thdot_grid.compatible( thdot_grid2 ) ) {
       cerr << "thdot2 grid is not grid-compatible with thdot grid 32" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,9), val ) ) {
       cerr << "mismatch 32 at (36,18,9): "  <<  thdot_grid2(36,18,9) << " vs. " <<  val <<  endl;
       exit(1);    
    }



    //==============  on theta surfaces
    
    ////////////////////// tests without omega term:
    val =  thvals[4] * tdot_onh(36,18,4) / t_onh(36,18,4);

    // derive a theta tendency grid from Tdot and T on theta surfaces 
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_onh, t_onh) );
    thdot_grid = *tmp1;  
    delete tmp1; 
    if ( ! thdot_grid.compatible( tdot_onh ) ) {
       cerr << "thdot grid is not grid-compatible with tdot_onh grid 51" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid(36,18,4), val ) ) {
       cerr << "mismatch 51 at (36,18,4): "  <<  thdot_grid(36,18,4) << " vs. " <<  val <<  endl;
       exit(1);    
    }

    // derive a theta tendency grid from Tdot and p on theta surfaces
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_onh, p_onh) );
    thdot_grid2 = *tmp1;     
    delete tmp1; 
    if ( ! thdot_grid2.compatible( thdot_grid ) ) {
       cerr << "thdot grid 2 is not grid-compatible with thdot grid 52" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,4), val ) ) {
       cerr << "mismatch 52 at (36,18,4): "  <<  thdot_grid2(36,18,4) << " vs. " <<  val <<  endl;
       exit(1);    
    }


    ////////////////////// tests without omega term:
    val =  thvals[4] * ( tdot_onh(36,18,4) / t_onh(36,18,4) - 2./7.*omega_onh(36,18,4)/(p_onh(36,18,4)*100.0) );

    // derive a theta tendency grid from Tdot, T, omega on theta surfaces 
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_onh, t_onh, omega_onh) );  
    thdot_grid2 = *tmp1;   
    delete tmp1; 
    if ( ! thdot_grid2.compatible( thdot_grid ) ) {
       cerr << "thdot grid 2 is not grid-compatible with thdot grid 61" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,4), val ) ) {
       cerr << "mismatch 61 at (36,18,4): "  <<  thdot_grid2(36,18,4) << " vs. " <<  val <<  endl;
       exit(1);    
    }
    
    // derive a theta tendency grid from Tdot and p on theta surfaces 
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_onh, p_onh, omega_onh) );    
    thdot_grid2 = *tmp1;
    delete tmp1;
    if ( ! thdot_grid2.compatible( thdot_grid ) ) {
       cerr << "thdot grid 2 is not grid-compatible with thdot grid 62" << endl;
       exit(1);
    }   
    if ( mismatch( thdot_grid2(36,18,4), val ) ) {
       cerr << "mismatch 62 at (36,18,4): "  <<  thdot_grid2(36,18,4) << " vs. " <<  val <<  endl;
       exit(1);    
    }




/*    
    

    // now check for things that we should not be able to do
    t_grid.set_quantity("bad stuff");
    ok = 0;
    try {
       rho_grid2 = dynamic_cast<GridLatLonField3D&>( dens.calc(t_grid) );
    } catch (...) {
       ok = 1;
    }  
    if ( ok == 0 ) {
       cerr << "1: failed to catch bad T name" << endl;
    }
    ok = 0;
    try {
       rho_grid2 = dynamic_cast<GridLatLonField3D&>( dens.calc(t_grid, theta_grid) );
    } catch (...) {
       ok = 1;
    }  
    if ( ok == 0 ) {
       cerr << "2: failed to catch bad T name" << endl;
    }
    t_grid.set_quantity("air_temperature");


    // set up surfaces
    t_sfc = t_grid.extractSfc(5);
    theta_sfc = theta_grid.extractSfc(5);
    p_sfc = p_grid.extractSfc(5);
    rho_sfc = rho_grid.extractSfc(5);
    val = rho_grid(20,18,5);
    val2 = rho_sfc(20,18);
    if ( mismatch( val, val2 ) ) {
       cerr << "rho sfc extraction failed "  <<  val << " vs. " <<  val2 <<  endl;
       exit(1);    
    }
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
