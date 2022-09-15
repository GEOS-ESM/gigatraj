/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/



#include <iostream>
#include <sstream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonFieldSfc.hh"
#include "gigatraj/LinearVinterp.hh"

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
    GridLatLonField3D grid3d, vert3d, idx3d, newgrid, newgrid2;
    GridLatLonField3D *tmp1;
    GridLatLonFieldSfc gridsfc, gridsfc2;
    GridLatLonFieldSfc *tmp2;
    std::vector<real> olons;
    std::vector<real> olats;
    std::vector<real> opres;
    std::vector<real> overt;
    std::vector<real> odata, odata2, odata3;
    std::vector<real> vlevs;
    std::vector<real> *vdata;
    int i,j,k;
    real val;
    real tstlev;
    real tstlevs[3];
    real *tstvals;
    Vinterp *interp;
    std::vector<real> profz, profv;
    //LinearVinterp *interp;


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
    
    overt.reserve(10);
    for (k=0; k<10; k++ ) {
       overt.push_back( static_cast<real>(k) );
    }   

    odata.reserve(72*37*18);
    odata2.reserve(72*37*18);
    for (k=0; k<18; k++ ) {
    for (j=0; j<37; j++ ) {
    for (i=0; i<72; i++ ) {
       val =  COS(2.0*PI*static_cast<real>(j)/37.0 )
            * SIN(2.0*PI*static_cast<real>(i)/72.0*3.0 )
            * static_cast<real>(k) ;
       odata.push_back( val );
       odata2.push_back( opres[k] );
    }   
    }   
    }   
    for (k=0; k<10; k++ ) {
    for (j=0; j<37; j++ ) {
    for (i=0; i<72; i++ ) {
       odata3.push_back( opres[k+6] );
    }   
    }   
    }   

    // set up the 3D grid variable
    // fake (unphysical) temperatures on pressure surfaces
    grid3d.set_quantity("air_temperature");
    grid3d.set_units("K");
    grid3d.set_fillval(-1234.0);
    grid3d.set_vertical("air_pressure");
    grid3d.set_vunits("mb");
    grid3d.set_time( 3.14159, "Grue 34, year of the kumquat");
    grid3d.load( olons, olats, opres, odata );

    // grid of pressures on pressure surfaces
    vert3d = grid3d;
    vert3d.set_quantity( grid3d.vertical() );
    vert3d.set_units( grid3d.vunits() );
    vert3d.load( olons, olats, opres, odata2 );

    // grid of pressures on index surfaces
    idx3d.set_quantity("air_pressure");
    idx3d.set_units("mb");
    idx3d.set_vertical("index");
    idx3d.set_vunits("");
    idx3d.set_time( grid3d.time(), grid3d.met_time() );
    idx3d.load( olons, olats, overt, odata3 );

 
    interp = new LinearVinterp();

    // =========================  member id
    if ( interp->id() != std::string("LinearVinterp") ) {
       cerr << "Object id is not LinearVinterp: " << interp->id() << endl;
       exit(1);
    }

    
    // =========================  method calc(z, grid3D, i,j)
    // get "temperature" on a particular pressure surface at a particular gridpoint
    tstlev = 539.7;
    val = interp->calc( tstlev, grid3d, 35, 27 );
    if ( mismatch( val, -0.0920980) ) {
       cerr << " Mismatched Single GridLatLonField3D interpolated value: " 
           << -0.0920980 << " vs. " << val << endl;
       exit(1);
    }

    // =========================  method calc(n, zs, grid3D, i,j)
    // "temperature" at a gridpoint on an array of pressure surfaces
    tstlevs[0] = 539.700;
    tstlevs[1] = 123.400;
    tstlevs[2] = 34.800;
    
    tstvals = interp->calc( 3, &(tstlevs[0]), grid3d, 35, 27 );
    if ( mismatch( tstvals[0], -0.0920980 ) 
    || mismatch( tstvals[1], -0.280488 ) 
    || mismatch( tstvals[2], -0.386605 ) ) {
       cerr << " Mismatched Array GridLatLonField3D interpolated value: " 
           << -0.0920980 << " vs. " << tstvals[0] 
           << ", " << "-0.280488" << " vs. " << tstvals[1] 
           << ", " << "-0.386605" << " vs. " << tstvals[2] << endl;
       exit(1);
    }
    free(tstvals);
    
    

    // =========================  method calc( vz, grid3D, i,j)
    // "temperatures" at a gridpoint on a vector of pressure surfaces
    vlevs.push_back( 539.70 );
    vlevs.push_back( 123.40 );
    vlevs.push_back( 34.800 );

    vdata = interp->calc( vlevs, grid3d, 35, 27 );
    if ( mismatch( (*vdata)[0], -0.0920980 ) 
    || mismatch( (*vdata)[1], -0.280488 ) 
    || mismatch( (*vdata)[2], -0.386605 ) ) {
       cerr << " Mismatched Array GridLatLonField3D interpolated value: " 
           << -0.0920980 << " vs. " << (*vdata)[0] 
           << ", " << -0.280488 << " vs. " << (*vdata)[1] 
           << ", " << -0.386605 << " vs. " << (*vdata)[2] << endl;
       exit(1);
    }
    delete vdata;

    
    // =========================  method surface( z grid3D)
    // interpolate the 3D "temperature" grid to a 2D pressure surface
    tmp2 = dynamic_cast<GridLatLonFieldSfc*>(interp->surface( 123.40, grid3d ));
    gridsfc = *tmp2;
    delete tmp2;
    val = gridsfc( 35, 27 );
    if ( mismatch( val, -0.280488 ) ) {
       cerr << " Mismatched Surface GridLatLonField3D interpolated value: " 
           << -0.280488 << " vs. " << val << endl;
       exit(1);
    }
    
    // ========================= method surface( z grid3D grid3D)
    // interpolate 3D "temperatures" to a pressure surface using a 3D pressure grid
    tmp2 = dynamic_cast<GridLatLonFieldSfc*>(interp->surface( opres[11], grid3d, vert3d ));
    gridsfc = *tmp2;
    delete tmp2;
    val = gridsfc.value( 15, 10 );
    if ( mismatch( val, grid3d(15,10,11)  )) {
       cerr << " Mismatched Surface GridLatLonField3D vert interpolated value: " 
           <<  grid3d(15,10,11) << " vs. " << val << endl;
       exit(1);
    }

    // ========================= method surface( grid3D gridSfc )
    // interpolate 3D "temperatures" to a 2D surface
    tmp2 = vert3d.extractSfc(11);
    gridsfc2 = *tmp2;
    delete tmp2;
    tmp2 = dynamic_cast<GridLatLonFieldSfc*>(interp->surface( grid3d, gridsfc2 ));
    gridsfc = *tmp2;
    delete tmp2;
    val = gridsfc.value( 15, 10 );
    if ( mismatch( val, grid3d(15,10,11)  )) {
       cerr << " Mismatched Surface GridLatLonField3D/Sfc vert interpolated value: " 
           <<  grid3d(15,10,11) << " vs. " << val << endl;
       exit(1);
    }

    // ========================= method reProfile( n, zs grid3D grid3D)
    // interpolate 3D "temperatures" to new pressure surfaces using a 3D pressure grid
    tmp1 = dynamic_cast<GridLatLonField3D*>(interp->reProfile( 3, tstlevs, grid3d, vert3d ));
    newgrid = *tmp1;
    delete tmp1;
    if ( mismatch( newgrid(35,27,0), -0.0920980 ) 
    || mismatch( newgrid(35,27,1), -0.280488 ) 
    || mismatch( newgrid(35,27,2), -0.386605 ) ) {
       cerr << " Mismatched Array GridLatLonField3D Interpolated value: " 
           << -0.0920980 << " vs. " << newgrid(35,27,0) 
           << ", " << "-0.280488" << " vs. " << newgrid(35,27,1) 
           << ", " << "-0.386605" << " vs. " << newgrid(35,27,2) << endl;
       exit(1);
    }
    newgrid.clear();

    // ========================= method reProfile( zs grid3D grid3D)
    // interpolate 3D "temperatures" to new pressure surfaces using a 3D pressure grid
    tmp1 = dynamic_cast<GridLatLonField3D*>(interp->reProfile( vlevs, grid3d, vert3d ));
    newgrid = *tmp1;
    delete tmp1;
    if ( mismatch( newgrid(35,27,0), -0.0920980 ) 
    || mismatch( newgrid(35,27,1), -0.280488 ) 
    || mismatch( newgrid(35,27,2), -0.386605 ) ) {
       cerr << " Mismatched Array GridLatLonField3D Interpolated Value: " 
           << -0.0920980 << " vs. " << newgrid(35,27,0) 
           << ", " << "-0.280488" << " vs. " << newgrid(35,27,1) 
           << ", " << "-0.386605" << " vs. " << newgrid(35,27,2) << endl;
       exit(1);
    }
    newgrid.clear();
   

    // ========================= method reProfile( grid3D grid3D)
    tmp1 = dynamic_cast<GridLatLonField3D*>(interp->reProfile( grid3d, idx3d ));
    newgrid = *tmp1;
    delete tmp1;
    if ( mismatch( newgrid(35,27,0),  -0.197247 ) 
    || mismatch( newgrid(35,27,1),  -0.230122) 
    || mismatch( newgrid(35,27,2), -0.262996 ) ) {
       cerr << " Mismatched Array GridLatLonField3D Interpolated Value: " 
           << -0.197247  << " vs. " << newgrid(35,27,0) 
           << ", " <<  -0.230122 << " vs. " << newgrid(35,27,1) 
           << ", " << -0.262996 << " vs. " << newgrid(35,27,2) << endl;
       exit(1);
    }
    newgrid.clear();

    // ========================= method invert( vector, grid3D)
    tmp1 = dynamic_cast<GridLatLonField3D*>( interp->invert( opres, idx3d ) );
    newgrid = *tmp1;
    delete tmp1;
    if ( newgrid(35,27,0) != newgrid.fillval() ) {
       cerr << "Bad-value failure on invert: " << newgrid(35,27,0) << endl;
       exit(1);
    }    
    if ( mismatch( newgrid(35,27,8),  2.0 ) )  {
       cerr << " Mismatched Array GridLatLonField3D inverted Value: " 
           << 2.0  << " vs. " << newgrid(35,27,8) << endl;
       exit(1);
    }
    newgrid.clear();




    // ========================= method profile( zs, vals, z, bad )
    profz.push_back( 1.0 );
    profv.push_back( 10.0 );
    profz.push_back( 2.0 );
    profv.push_back( 20.0 );
    profz.push_back( 3.0 );
    profv.push_back( 999.0 );
    profz.push_back( 4.0 );
    profv.push_back( 40.0 );
  
    val = interp->profile( profz, profv, 3.0, 999.0 );
    if ( mismatch( val, 30.0 ) ) {
       cerr << "Mismatched single profile interp value: "
       << val << " vs. " << 30.0 << endl;
    }
    
    delete interp;
  
    
}
