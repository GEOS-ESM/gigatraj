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
#include "gigatraj/DensOTF.hh"
#include "gigatraj/BalanceThetaDot1OTF.hh"

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
    GridLatLonField3D dens_grid, thdot_grid, bal_thdot_grid;
    GridLatLonFieldSfc t_sfc, p_sfc, theta_sfc;
    GridLatLonFieldSfc thdot_sfc, thdot_sfc2;
    GridLatLonFieldSfc *area;
    GridLatLonField3D *tmp1;
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
    DensOTF dens;
    ThetaOTF thet;
    PressOTF press;
    ThetaDotOTF thdot;
    BalanceThetaDot1OTF bal_thdot;
    int ok;
    real start;
    real unbal_massflux, bal_massflux;
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
    delete vin;


    // get density on theta surfaces
    tmp1 = dynamic_cast<GridLatLonField3D*>( dens.calc( t_onh ));
    dens_grid = *tmp1;
    delete tmp1;

    //==============  on theta surfaces
    
    // derive a theta tendency grid from Tdot and T on theta surfaces 
    tmp1 = dynamic_cast<GridLatLonField3D*>( thdot.calc(tdot_onh, t_onh) );
    thdot_grid = *tmp1;
    delete tmp1;    
    if ( ! thdot_grid.compatible( tdot_onh ) ) {
       cerr << "thdot grid is not grid-compatible with tdot_onh grid 51" << endl;
       exit(1);
    }   

    // mass-balance the theta tendency grid
    tmp1 = dynamic_cast<GridLatLonField3D*>( bal_thdot.calc( thdot_grid, t_onh ));
    bal_thdot_grid = *tmp1;
    delete tmp1;
    if ( ! bal_thdot_grid.compatible( tdot_onh ) ) {
       cerr << "bal_thdot grid is not grid-compatible with tdot_onh grid 51" << endl;
       exit(1);
    }  

    // now check the integration on the 500K surface
    k = 3; // 500 K
    tmp2 =  thdot_grid.extractSfc( k );
    thdot_sfc = *tmp2;
    delete tmp2;
    tmp2 = bal_thdot_grid.extractSfc( k );
    thdot_sfc2 = *tmp2;
    delete tmp2;
    area = dynamic_cast<GridLatLonFieldSfc*>(thdot_sfc.areas());
    bal_massflux = 0.0;
    unbal_massflux = 0.0;
    for (j=0; j<37; j++ ) {
    for (i=0; i<72; i++ ) {
        unbal_massflux = unbal_massflux + thdot_sfc.value(i,j)*dens_grid.value(i,j,k)*area->value(i,j);
        bal_massflux = bal_massflux +  thdot_sfc2.value(i,j)*dens_grid.value(i,j,k)*area->value(i,j);   
    }   
    }
    if ( mismatch( bal_massflux, 0.0 ) ) {
       cerr << "balanced flux is not balanced: "  <<  bal_massflux << " from " << unbal_massflux <<  endl;
       exit(1);    
    }
    delete area;
    
    
    
    exit(0);

}
