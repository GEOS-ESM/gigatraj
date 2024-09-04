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
#include <sstream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonFieldSfc.hh"
#include "gigatraj/LinearVinterp.hh"
#include "gigatraj/BilinearHinterp.hh"

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
    GridLatLonField3D grid3d, regrid3d, grid3d2;
    GridLatLonField3D *tmp1;
    GridLatLonFieldSfc gridsfc, regridsfc, gridsfc2;
    GridLatLonFieldSfc *tmp2;
    std::vector<real> olons, relons;
    std::vector<real> olats, relats;
    std::vector<real> opres;
    std::vector<real> odata;
    std::vector<real> vlons;
    std::vector<real> vlats;
    std::vector<real> vzs;
    std::vector<real> *vdata;
    int i,j,k;
    real val, val2;
    real tstlon, tstlat;
    real tstlons[3], tstlats[3], tstzs[3];
    real *tstvals, tstvals2[3], tstvals3[3];
    HLatLonInterp *interp;
    Vinterp *vin;


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

    for (k=0; k<18; k++ ) {
    for (j=0; j<37; j++ ) {
    for (i=0; i<72; i++ ) {
       val =  COS(2.0*PI*static_cast<real>(j)/37.0 )
            * SIN(2.0*PI*static_cast<real>(i)/72.0*3.0 )
            * static_cast<real>(k) ;
       odata.push_back( val );
    }   
    }   
    }   

    relons.reserve(37);
    for (i=0; i<37; i++ ) {
       relons.push_back(i*10.0 + 1.5);
    }   
    relats.reserve(180);
    for (i=0; i<180; i++ ) {
       relats.push_back( i*1.0-89.5 );
    }   



    // set up the 3D grid variable
    grid3d.set_quantity("air temperature");
    grid3d.set_units("K");
    grid3d.set_fillval(-1234.0);
    grid3d.set_vertical("air pressure");
    grid3d.set_vunits("mb");
    grid3d.set_time( 3.14159, "Grue 34, year of the kumquat");
    grid3d.load( olons, olats, opres, odata );

    // set up the surface grid variable
    tmp2 = grid3d.extractSfc(5);
    gridsfc = *tmp2;
    delete tmp2;
    
    
    interp = new BilinearHinterp();

    // =========================  member id
    if ( interp->id() != std::string("BilinearHinterp") ) {
       cerr << "Object id is not BilinearHinterp: " << interp->id() << endl;
       exit(1);
    }
    
    // =========================  method calc(lon, lat, grid3D, k)
    tstlon = -76.3;
    tstlat = 39.7;
    val = interp->calc( tstlon, tstlat, grid3d, 5 );
    if ( mismatch( val, -1.13112) ) {
       cerr << " Mismatched Single GridLatLonField3D interpolated value: " 
           << -1.13112 << " vs. " << val << endl;
       exit(1);
    }
    val = interp->calc( 0.0, 90.0, grid3d, 5 );
    if ( mismatch( val, 0.0 ) ) {
       cerr << " Mismatched Single GridLatLonField3Dx interpolated value: " 
           << 0.0 << " vs. " << val << endl;
       exit(1);
    }

    // =========================  method calc(n, lons, lats, grid3D, k)
    tstlons[0] = -76.3;
    tstlats[0] = 39.7;
    tstlons[1] = 0.5;
    tstlats[1] = -35.0;
    tstlons[2] = 359.5;
    tstlats[2] = 90.0;
    tstvals = interp->calc( 3, &(tstlons[0]), &(tstlats[0]), grid3d, 5 );
    if ( mismatch( tstvals[0], -1.13112 ) 
    || mismatch( tstvals[1], -0.0378941 ) 
    || mismatch( tstvals[2], -0.127548 ) ) {
       cerr << " Mismatched Array GridLatLonField3D interpolated value: " 
           << -1.13112 << " vs. " << tstvals[0] 
           << ", " << -0.0378941 << " vs. " << tstvals[1] 
           << ", " << -0.127548 << " vs. " << tstvals[2] << endl;
       exit(1);
    }
    delete[] tstvals;
    

    // =========================  method calc(vlons, vlat, grid3D, k)
    vlons.clear();
    vlats.clear();
    vlons.push_back( -76.3 );
    vlats.push_back( 39.7 );
    vlons.push_back(  0.5 );
    vlats.push_back( -35.0 );
    vlons.push_back(  359.5 );
    vlats.push_back( 90.0 );
    vdata = interp->calc( vlons, vlats, grid3d, 5 );
    if ( mismatch( (*vdata)[0], -1.13112 ) 
    || mismatch( (*vdata)[1], -0.0378941 ) 
    || mismatch( (*vdata)[2], -0.127548 ) ) {
       cerr << " Mismatched Array GridLatLonFieldSfc interpolated value: " 
           << -1.13112 << " vs. " << (*vdata)[0] 
           << ", " << -0.0378941 << " vs. " << (*vdata)[1] 
           << ", " << -0.127548 << " vs. " << (*vdata)[2] << endl;
       exit(1);
    }
    delete vdata;

    // =========================  method calc(lon, lat, gridSfc )
    val = interp->calc( tstlon, tstlat, gridsfc );
    if ( mismatch( val, -1.13112) ) {
       cerr << " Mismatched Single GridLatLonFieldSfc interpolated value: " 
           << -1.13112 << " vs. " << val << endl;
       exit(1);
    }
    
    // =========================  method calc(n, lons, lats, gridSfc )
    tstlons[0] = -76.3;
    tstlats[0] = 39.7;
    tstlons[1] = 0.5;
    tstlats[1] = -35.0;
    tstlons[2] = 359.5;
    tstlats[2] = 90.0;
    tstvals = interp->calc( 3, &(tstlons[0]), &(tstlats[0]), gridsfc );
    if ( mismatch( tstvals[0], -1.13112 ) 
    || mismatch( tstvals[1], -0.0378941 ) 
    || mismatch( tstvals[2], -0.127548 ) ) {
       cerr << " Mismatched Array GridLatLonFieldSfc interpolated value: " 
           << -1.13112 << " vs. " << tstvals[0] 
           << ", " << -0.0378941 << " vs. " << tstvals[1] 
           << ", " << -0.127548 << " vs. " << tstvals[2] << endl;
       exit(1);
    }
    // (note: tstvals deleted below)
    
    // =========================  vector method calc(n, lons, lats, gridSfc )
    interp->calc( 3, tstlons, tstlats, tstvals2, gridsfc );
    if ( mismatch( tstvals2[0], tstvals[0] ) 
    || mismatch( tstvals2[1], tstvals[1] ) 
    || mismatch( tstvals2[2], tstvals[2] ) ) {
       cerr << " Mismatched Array GridLatLonFieldSfc interpolated value: " 
           << tstvals[0] << " vs. " << tstvals2[0] 
           << ", " << tstvals[1] << " vs. " << tstvals2[1] 
           << ", " << tstvals[2] << " vs. " << tstvals2[2] << endl;
       exit(1);
    }
    
    delete[] tstvals; // (see above)
    
    
    // =========================  method calc(vlons, vlats, gridSfc )
    vlons.clear();
    vlats.clear();
    vlons.push_back( -76.3 );
    vlats.push_back( 39.7 );
    vlons.push_back(  0.5 );
    vlats.push_back( -35.0 );
    vlons.push_back(  359.5 );
    vlats.push_back( 90.0 );
    vdata = interp->calc( vlons, vlats, gridsfc );
    if ( mismatch( (*vdata)[0], -1.13112 ) 
    || mismatch( (*vdata)[1], -0.0378941 ) 
    || mismatch( (*vdata)[2], -0.127548 ) ) {
       cerr << " Mismatched Array GridLatLonFieldSfc interpolated value: " 
           << -1.13112 << " vs. " << (*vdata)[0] 
           << ", " << -0.0378941 << " vs. " << (*vdata)[1] 
           << ", " << -0.127548 << " vs. " << (*vdata)[2] << endl;
       exit(1);
    }
    delete vdata;

    // =========================  method regrid( gridsfc, newlons, newlats );
    tmp2 = interp->regrid( gridsfc, relons, relats );
    regridsfc = *tmp2;
    delete tmp2;
    if ( ( gridsfc.quantity() != regridsfc.quantity() )
      || (    gridsfc.units() != regridsfc.units() )
      || (  gridsfc.fillval() != regridsfc.fillval()  )
      || (  gridsfc.surface() != regridsfc.surface() )
      || (     gridsfc.time() != regridsfc.time() )
      || ( gridsfc.met_time() != regridsfc.met_time() ) ) {
         cerr << " Sfc regridding failed to replicate metadata!" << endl;
         exit(1);
    }
    if ( relats[4] != regridsfc.latitude(4)
      || relons[5] != regridsfc.longitude(5) ) {
         cerr << " Sfc regridding failed to use new dimensions!" << endl;
         exit(1);
    }
    if ( mismatch( regridsfc(4,5), 4.01540 ) ) {
         cerr << " Sfc regridding bad interp: " << regridsfc(4,5)
         << " vs. " << 4.01540 << endl;
         exit(1);
    }
     
    
    // =========================  method regrid( grid3D, newlons, newlats );
    tmp1 = interp->regrid( grid3d, relons, relats );
    regrid3d = *tmp1;
    delete tmp1;
    if ( ( grid3d.quantity() != regrid3d.quantity() )
      || (    grid3d.units() != regrid3d.units() )
      || (  grid3d.fillval() != regrid3d.fillval()  )
      || ( grid3d.vertical() != regrid3d.vertical() )
      || (   grid3d.vunits() != regrid3d.vunits() )
      || (     grid3d.time() != regrid3d.time() )
      || ( grid3d.met_time() != regrid3d.met_time() ) ) {
         cerr << " 3d regridding failed to replicate metadata!" << endl;
         exit(1);
    }
    if ( relats[4] != regrid3d.latitude(4)
      || relons[5] != regrid3d.longitude(5) ) {
         cerr << " 3d regridding failed to use new dimensions!" << endl;
         exit(1);
    }
    if ( mismatch( regrid3d(4,5,5), 4.01540 ) ) {
         cerr << " 3d regridding bad interp: " << regrid3d(4,5,5)
         << " vs. " << 4.01540 << endl;
         exit(1);
    }
    
    // =========================  method vinterp(lon, lat, z, grid, vin )
    tstlons[0] = -76.3;
    tstlats[0] = 39.7;
    tstzs[0] = 225.0;
    vin = new LinearVinterp();
    val = interp->vinterp( tstlons[0], tstlats[0], tstzs[0], grid3d, *vin );
    if ( mismatch( val, -1.47045 )  ) {
       cerr << " Mismatched Array GridLatLonField3D interpolated value 0: " 
           << -1.47045 << " vs. " << val  << endl;
       exit(1);
    }
    // another point
    val = interp->vinterp( 0.0, 90.0, 200.0, grid3d, *vin );
    if ( mismatch( val, 0 )  ) {
       cerr << " Mismatched Array GridLatLonField3D interpolated value 1: " 
           << 0.0 << " vs. " << val  << endl;
       exit(1);
    }
    // try to extrapolate to pressure 0.0
    val = interp->vinterp( 0.0, 90.0, 0.0, grid3d, *vin );
    if ( mismatch( val, grid3d.fillval() )  ) {
       cerr << " Mismatched Array GridLatLonField3D interpolated value 2: " 
           << grid3d.fillval() << " vs. " << val  << endl;
       exit(1);
    }
        
    // =========================  method vinterp(n, lons, lats, zs, grid, vin )
    tstlons[0] = -76.3;
    tstlats[0] = 39.7;
    tstzs[0] = 225.0;
    tstlons[1] = 0.5;
    tstlats[1] = -35.0;
    tstzs[1] = 100.0;
    tstlons[2] = 359.5;
    tstlats[2] = 90.0;
    tstzs[2] = 80.0;
    tstvals = interp->vinterp(3, tstlons, tstlats, tstzs, grid3d, *vin );
    if ( mismatch( tstvals[0], -1.47045 ) 
    || mismatch( tstvals[1], -0.0682093 ) 
    || mismatch( tstvals[2], -0.246592 ) ) {
       cerr << " Mismatched Array GridLatLonField3D interpolated value: " 
           << -1.47045 << " vs. " << tstvals[0] 
           << ", " << -0.0682093 << " vs. " << tstvals[1] 
           << ", " << -0.246592 << " vs. " << tstvals[2] << endl;
       exit(1);
    }
    // (note: tstvals deleted below)
    
    
    // ========================= vector vinterp
    interp->vinterp(3, tstlons, tstlats, tstzs, tstvals2, grid3d, *vin );
    if ( mismatch( tstvals2[0], tstvals[0] ) 
    || mismatch( tstvals2[1], tstvals[1] ) 
    || mismatch( tstvals2[2], tstvals[2] ) ) {
       cerr << " Mismatched Array GridLatLonField3D interpolated value: " 
           << tstvals[0] << " vs. " << tstvals2[0] 
           << ", " << tstvals[1] << " vs. " << tstvals2[1] 
           << ", " << tstvals[2] << " vs. " << tstvals2[2] << endl;
       exit(1);
    }
    
    
    delete[] tstvals; // (see above)
        
    // =========================  method vinterp(lons, lats, zs, grid, vin )
    vzs.clear();
    vzs.push_back( 225.0 );
    vzs.push_back( 100.0 );
    vzs.push_back( 80.0 );
    vdata = interp->vinterp(vlons,vlats, vzs, grid3d, *vin );
    if ( mismatch( (*vdata)[0], -1.47045 ) 
    || mismatch( (*vdata)[1], -0.0682093 ) 
    || mismatch( (*vdata)[2], -0.246592 ) ) {
       cerr << " Mismatched Array GridLatLonField3D interpolated value: " 
           << -1.47045 << " vs. " << (*vdata)[0] 
           << ", " << -0.0682093 << " vs. " << (*vdata)[1] 
           << ", " << -0.246592 << " vs. " << (*vdata)[2] << endl;
       exit(1);
    }
    delete vdata;


    //=========================== interpolating vectors of vector quantities
    grid3d2 = grid3d;
    grid3d2.transform("foo", 2.0, 0.5);
    gridsfc2 = gridsfc;
    gridsfc2.transform("foo", 2.0, 0.5);

    tstlons[0] = -76.3;
    tstlats[0] = 39.7;
    tstzs[0] = 225.0;

    interp->vinterpVector( tstlons[0], tstlats[0], tstzs[0], val, val2, grid3d, grid3d2, *vin );
    if ( mismatch( val, -1.47045 ) 
       || mismatch( val2, (-1.47045 - 0.5)/2 ) ) {
       cerr << " Mismatched Vector GridLatLonField3D interpolated value 0: " 
           << -1.47045 << " vs. " << val  << ", "
           << (-1.47045 - 0.5)/2 + 0.5 << " vs. " << val2  << endl;
       exit(1);
    }
    
    interp->calc( tstlons[0], tstlats[0], val, val2, gridsfc, gridsfc2 );
    if ( mismatch( val, -1.13112 ) 
       || mismatch( val2, (-1.13112 - 0.5)/2 ) ) {
       cerr << " Mismatched Vector GridLatLonField3D interpolated value 0: " 
           << -1.13112 << " vs. " << val  << ", "
           << (-1.13112 - 0.5)/2 + 0.5 << " vs. " << val2  << endl;
       exit(1);
    }

    tstlons[0] = -76.3;
    tstlats[0] = 39.7;
    tstzs[0] = 225.0;
    tstlons[1] = 0.5;
    tstlats[1] = -35.0;
    tstzs[1] = 100.0;
    tstlons[2] = 359.5;
    tstlats[2] = 90.0;
    tstzs[2] = 80.0;
    interp->vinterpVector( 3, tstlons, tstlats, tstzs, tstvals2, tstvals3, grid3d, grid3d2, *vin );
    if ( mismatch( tstvals2[0], -1.47045 ) 
      || mismatch( tstvals2[1], -0.0682093 ) 
      || mismatch( tstvals2[2], -0.246592 ) 
      || mismatch( tstvals3[0], (-1.47045 - 0.5)/2 ) 
      || mismatch( tstvals3[1], (-0.0682093 - 0.5)/2 ) 
      || mismatch( tstvals3[2], (-0.246592 - 0.5)/2 ) ) {
       cerr << " Mismatched Array vector GridLatLonField3D interpolated value: " 
           << -1.47045 << " vs. " << tstvals2[0] 
           << ", " << -0.0682093 << " vs. " << tstvals2[1] 
           << ", " << -0.246592 << " vs. " << tstvals2[2] << "; "
           << (-1.47045 - 0.5)/2 << " vs. " << tstvals3[0] 
           << ", " << (-0.0682093 - 0.5)/2 << " vs. " << tstvals3[1] 
           << ", " << (-0.246592 - 0.5)/2 << " vs. " << tstvals3[2] << endl;
       exit(1);
    }

    interp->calc( 3, tstlons, tstlats, tstvals2, tstvals3, gridsfc, gridsfc2 );
    if ( mismatch( tstvals2[0],-1.13112  ) 
      || mismatch( tstvals2[1],-0.0378941  ) 
      || mismatch( tstvals2[2],-0.127548   ) 
      || mismatch( tstvals3[0], (-1.13112  - 0.5)/2 ) 
      || mismatch( tstvals3[1], (-0.0378941  - 0.5)/2 ) 
      || mismatch( tstvals3[2], (-0.127548  - 0.5)/2 ) ) {
       cerr << " Mismatched Array vector GridLatLonFieldSfc interpolated value: " 
           <<  -1.13112   << " vs. " << tstvals2[0] 
           << ", " << -0.0378941 << " vs. " << tstvals2[1] 
           << ", " << -0.127548 << " vs. " << tstvals2[2] << "; "
           << (-1.13112  - 0.5)/2 << " vs. " << tstvals3[0] 
           << ", " << (-0.0378941  - 0.5)/2 << " vs. " << tstvals3[1] 
           << ", " << (-0.127548  - 0.5)/2 << " vs. " << tstvals3[2] << endl;
       exit(1);
    }


    delete vin;
    delete interp;

}
