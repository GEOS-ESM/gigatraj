/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include <iostream>
#include <math.h>

#include <stdlib.h>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetMyGEOS.hh"
#include "gigatraj/LogLinearVinterp.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;


int main() 
{

    MetMyGEOS *metsrc0;
    string s1, s2;
    int status;
    int hgrid,vgrid,tspace,tavg,tbase, ndims;
    string *longname, *units, *url, *url2;
    real dd, dd2, dd3;
    real d0;
    double tyme, tyme2, tyme3;
    GridLatLonField3D *grid3d;
    GridLatLonFieldSfc *grid2d;
    int nx,ny,nz;
    int i;
    std::string stry;
    std::string test_date;

    // the met catalog to be used
    std::string metCatalog = "test_MetMyGEOS";
    // corresponds to the zero mode time
    std::string basedate   = "2000-03-15T12:13";
    // 1.5 days ahead of the basetime
    std::string baseOffset = "2000-03-17T00:13";
   
    // the valid-at time to work with, mainly
    std::string date0 = "2000-04-15T03:00";
    // the next timestamp after date0
    std::string date1 = "2000-04-15T04:00";
    // a timestamp between date0 and date1
    std::string datem = "2000-04-15T03:43";
    
    // the expected number of longitudes
    int eNlons = 720;
    // the expected number of latitudes
    int eNlats = 361;
    // the expeted number of vertical levels fo r3D quantities
    int eNvert = 48;
    
    // index for testing a longitude value
    int iLon0 = 362;
    // the longitude corresponding to that index
    real eLon0 = 1.0;
    
    // index for testig a latitude value
    int iLat0 = 182;
    // the latitude value corresponding to that index
    real eLat0 = 1.0;
    
    // index for texting a vertical level value
    int iVrt0 = 21;
    // the vertical level value corresponding to that index
    real eVert0 = 250.0;
    
    // local marker for bad-or-missing data
    real bad = -999.99;
    
    // the 2D quantity to test
    std::string quant2d = "PS";
    // the units of that quantity
    std::string units2d = "Pa";
    // the 2D data value at [0,0]
    real  eDat2dLL = 67862.34; 
    // the 2D data value at [eNlons-1, eNlats-1]
    real  eDat2dHH = 101982.0; 
    // the 2D data value at [eNlons-1, 0]
    real  eDat2dHL = 67862.34; 
    // the 2D data value at [0, eNlats-1]
    real  eDat2dLH = 101982.0; 
    // the 2D data value at [eNlons/2, eNlats/2]
    real  eDat2dMM = 100771.8; 
    // the 2D data value at (eNlons/2+1, eNLats/2+1]
    real  eDat2dM1M1 = 100770.2; 
    
    // the 3D quantity to test
    std::string quant3d = "T";
    // its units
    std::string units3d = "K";
    // the verticla coord
    std::string vquant3d = "P";
    // the vertical coord units
    std::string vunits3d = "hPa";
    // the 3D data value at [0,0,0]
    real  eDat3dLLL = bad; 
    // the 3D data value at [eNlons-1, eNlats-1, eNvert-1]
    real  eDat3dHHH = 218.0784; 
    // the 3D data value at [eNlons-1, 0, 0]
    real  eDat3dHLL = bad; 
    // the 3D data value at [0, eNlats-1, 0]
    real  eDat3dLHL = 251.0733; 
    // the 3D data value at [0, 0, eNvert-1]
    real  eDat3dLLH = 224.9851; 
    // the 3D data value at [eNlons-1, eNlats-1, 0]
    real  eDat3dHHL = 251.0733; 
    // the 3D data value at [eNlons-1, 0, eNvert-1]
    real  eDat3dHLH = 224.9851; 
    // the 3D data value at [0, eNlats-1, eNvert-1]
    real  eDat3dLHH = 218.0784; 
    // the 3D data value at [eNlons/2, eNlats/2, eNvert/2]
    real  eDat3dMMM = 190.2171; 

    // indices for OTf test
    int iLon1 = eNlons/3;
    int iLat1 = eNlats/4;
    int iVrt1 = eNvert/2;

    // coordinates for direct access test
    real eLon2 = 1.0;
    real eLat2 = 0.5;
    real eVrt2 = 300.0;
    real eDat3d2 = 243.6409;
    
    // direct access test at the next time step
    // same coords as for test2, but for date1 instead of date0
    real eDat3d3 = 243.6399;
    
    // coordinates for longitudinal interp test
    real eLon4 = eLon2 + 0.5;
    real eLon4a = eLon2 + 0.5/2.0;
    real eLat4 = eLat2;
    real eVrt4 = eVrt2;
    real eDat3d4 = 243.5765;
    
    // coordinates for latitudinal interp test
    real eLon5 = eLon2;
    real eLat5 = eLat2 + 0.5;
    real eLat5a = eLat2 + 0.5/2.0;
    real eVrt5 = eVrt2;
    real eDat3d5 = 243.7912;
    
    // coordinates for vertical interp test
    real eLon6 = eLon2;
    real eLat6 = eLat2;
    real eVrt6 = 350.0;
    real eVrt6a = 340.0;
    real eDat3d6 = 250.7633;
    
    /////////////////// Test the MyGEOS_Directory class here


    // create a MyGEOS data object
    metsrc0 = new MetMyGEOS();
    //metsrc0->debug( 100 );

    
    
    metsrc0->set_BaseTime(basedate);
    test_date = metsrc0->BaseTime();
    if ( basedate != test_date ) {
       cerr << "set_BaseTime failed:" << test_date << endl;
       exit(1);    
    } 
    delete metsrc0;
    
    metsrc0 = new MetMyGEOS(basedate);
    test_date = metsrc0->BaseTime();
    if ( basedate != test_date ) {
       cerr << "new MetMyGEOS(date) failed:" << test_date << endl;
       exit(1);    
    } 
    
    // test time conversion routines 
    tyme = metsrc0->cal2Time( baseOffset );
    if ( mismatch(tyme, 1.5) ) {
       cerr << "Bad cal->time conversion: " << tyme << " vs. " << 1.5 << endl;
       exit(1);  
    }
    test_date = metsrc0->time2Cal( tyme );
    if ( test_date !=  baseOffset ) {
       cerr << "Bad time->cal conversion: " << test_date << " vs. " << baseOffset << endl;
       exit(1);  
    }


    //*************  Catalog tests *******************************

    //metsrc0->debug( 100 );
    
    metsrc0->metTag( metCatalog );
    stry = metsrc0->metTag();
    if ( stry != metCatalog ) {
       cerr << "Met tag is " << stry << " instead of " << metCatalog << endl;
       exit(1);
    }

    //*************  Sfc-reading tests *******************************


    // test sample values for a 2D field

    grid2d = metsrc0->GetSfc( quant2d, date0 );
    
    // check the grid    
    grid2d->dims( &nx, &ny );
    if ( nx != eNlons || ny != eNlats ) {
       cerr << "Bad grid2d dimensions: longitude " << nx << " vs. " << eNlons << endl;
       cerr << "                     :  latitude " << ny << " vs. " << eNlats << endl;
       exit(1);  
    }

        
    //for ( i=0; i < nx ; i++ ) {
    //    cerr << "lon[" << i << "] = " << grid2d->longitude(i) << endl;
    //}
    //for ( i=0; i < ny ; i++ ) {
    //    cerr << "lat[" << i << "] = " << grid2d->latitude(i) << endl;
    //}
    dd = grid2d->longitude(iLon0);
    if ( mismatch(dd, eLon0) ) {
       cerr << "Bad grid2d longitude: " << dd << " vs. " << eLon0 << endl;
       exit(1);  
    }
    dd = grid2d->latitude(iLat0);
    if ( mismatch(dd, eLat0) ) {
       cerr << "Bad grid2d latitude: " << dd << " vs. " << eLat0 << endl;
       exit(1);  
    }


    // check quantities and units
    stry = grid2d->quantity();
    if ( stry != quant2d ) {
       cerr << "Bad grid2d quantity: " << stry << " vs. " << quant2d << endl;
       exit(1);  
    }
    stry = grid2d->units();
    if ( stry != units2d ) {
       cerr << "Bad grid2d units: " << stry << " vs. " << units2d << endl;
       exit(1);  
    }


    // check data values

    if ( eDat2dLL != bad ) {
       d0 = eDat2dLL;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid2d)(0,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D [" << 0 << "," << 0 << "] PS value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    
    if ( eDat2dHH != bad ) {
       d0 = eDat2dHH;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid2d)(nx-1,ny-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D [" << nx-1 << "," << ny-1 << "] PS value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat2dHL != bad ) {
       d0 = eDat2dHL;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid2d)(nx-1,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D [" << nx-1 << "," << 0 << "] PS value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat2dLH != bad ) {
       d0 = eDat2dLH;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid2d)(0,ny-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D [" << 0 << "," << ny-1 << "] PS value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat2dMM != bad ) {
       d0 = eDat2dMM;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid2d)(nx/2,ny/2);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D [" << nx/2 << "," << ny/2 << "] PS value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    
    //metsrc0->debug( 0 );

    tyme = metsrc0->cal2Time( date0 );
    if ( eDat2dM1M1 != bad ) {
       d0 = eDat2dM1M1;
    } else {
       d0 = grid3d->fillval();
    }   
    // test direct access
    dd = metsrc0->getData( quant2d, tyme, grid2d->longitude(nx/2+1), grid2d->latitude(ny/2+1), -999.0   );
    if ( mismatch(dd, d0) ) {
       cerr << "Bad getdata( " << grid2d->longitude(nx/2) << "," << grid2d->latitude(ny/2)
       << ") PS value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }

    delete grid2d;

    //*************  3D-reading tests *******************************

    //metsrc0->debug( 1 );
    
    grid3d = metsrc0->Get3D( quant3d, date0 );
    
    // check the grid    
    grid3d->dims( &nx, &ny, &nz );
    if ( nx != eNlons || ny != eNlats || nz != eNvert ) {
       cerr << "Bad grid2d dimensions: longitude " << nx << " vs. " << eNlons << endl;
       cerr << "                     :  latitude " << ny << " vs. " << eNlats << endl;
       cerr << "                     :  vertical " << nz << " vs. " << eNvert << endl;
       exit(1);  
    }
        

    //for ( i=0; i < nx ; i++ ) {
    //    cerr << "lon[" << i << "] = " << grid3d->longitude(i) << endl;
    //}
    //for ( i=0; i < ny ; i++ ) {
    //    cerr << "lat[" << i << "] = " << grid3d->latitude(i) << endl;
    //}
    //for ( i=0; i < nz ; i++ ) {
    //    cerr << "z[" << i << "] = " << grid3d->level(i) << endl;
    //}

    dd = grid3d->longitude(iLon0);
    if ( mismatch(dd, eLon0) ) {
       cerr << "Bad grid3d longitude: " << dd << " vs. " << eLon0 << endl;
       exit(1);  
    }
    dd = grid3d->latitude(iLat0);
    if ( mismatch(dd, eLat0) ) {
       cerr << "Bad grid3d latitude: " << dd << " vs. " << eLat0 << endl;
       exit(1);  
    }
    dd = grid3d->level(iVrt0);
    if ( mismatch(dd, eVert0) ) {
       cerr << "Bad grid3d level: " << dd << " vs. " << eVert0 << endl;
       exit(1);  
    }

    // check quantities and units
    stry = grid3d->quantity();
    if ( stry != quant3d ) {
       cerr << "Bad grid3d quantity: " << stry << " vs. " << quant3d << endl;
       exit(1);  
    }
    stry = grid3d->units();
    if ( stry != units3d ) {
       cerr << "Bad grid3d units: " << stry << " vs. " << units3d << endl;
       exit(1);  
    }
    stry = grid3d->vertical();
    if ( stry != vquant3d ) {
       cerr << "Bad grid3d vertical: " << stry << " vs. " << vquant3d << endl;
       exit(1);  
    }
    stry = grid3d->vunits();
    if ( stry != vunits3d ) {
       cerr << "Bad grid3d vertical units: " << stry << " vs. " << vunits3d << endl;
       exit(1);  
    }


    if ( eDat3dLLL != bad ) {
       d0 = eDat3dLLL;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid3d)(0,0,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << 0 << "," << 0 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat3dHHH != bad ) {
       d0 = eDat3dHHH;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid3d)(nx-1,ny-1,nz-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << ny-1 << "," << nz-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat3dHLL != bad ) {
       d0 = eDat3dHLL;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid3d)(nx-1,0,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << 0 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat3dLHL != bad ) {
       d0 = eDat3dLHL;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid3d)(0,ny-1,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << 0 << "," << ny-1 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat3dLLH != bad ) {
       d0 = eDat3dLLH;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid3d)(0,0,nz-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << 0 << "," << 0 << "," << nz-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat3dHHL != bad ) {
       d0 = eDat3dHHL;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid3d)(nx-1,ny-1,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << ny-1 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat3dHLH != bad ) {
       d0 = eDat3dHLH;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid3d)(nx-1,0,nz-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << 0 << "," << nz-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    if ( eDat3dLHH != bad ) {
       d0 = eDat3dLHH;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid3d)(0,ny-1,nz-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << 0 << "," << ny-1 << "," << nz-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }    
    if ( eDat3dMMM != bad ) {
       d0 = eDat3dMMM;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = (*grid3d)(nx/2,ny/2,nz/2);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx/2 << "," << ny/2 << "," << nz/2 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }

    //metsrc0->debug( 0 );
    
    // test a quantity that is calculated on the fly
    d0 = (*grid3d)(iLon1,iLat1,iVrt1);
    dd = grid3d->level(iVrt1);
    d0 = (*grid3d)(iLon1,iLat1,iVrt1) * POW( 1000.0/grid3d->level(iVrt1), 2./7.);
    delete grid3d;
    grid3d = metsrc0->Get3D( "air_potential_temperature", date0 );
    dd = (*grid3d)(iLon1,iLat1,iVrt1);
    if ( mismatch(dd, d0, 0.01) ) {
       cerr << "Bad 3D  Theta[" << iLon1 << "," << iLat1 << "," << iVrt1 << "] value: " 
            << dd << " vs. " << d0 << endl;
       exit(1);  
    }

    delete grid3d;
    
    // test direct access
    tyme = metsrc0->cal2Time( date0 );
    if ( eDat3d2 != bad ) {
       d0 = eDat3d2;
    } else {
       d0 = grid3d->fillval();
    }   
    dd = metsrc0->getData( "T", tyme, eLon2, eLat2, eVrt2   );
    if ( mismatch(dd, d0) ) {
       cerr << "Bad getdata( " << eLon2 << ", " << eLat2 << ", " << eVrt2 << ") T value: " 
       << dd << " vs. " << d0 << endl;
       exit(1);  
    }


    // Now get a second time
    tyme2 = metsrc0->cal2Time( date1 );
    dd2 = metsrc0->getData( "T", tyme2, eLon2, eLat2, eVrt2  );
    if ( eDat3d3 != bad ) {
       d0 = eDat3d3;
    } else {
       d0 = grid3d->fillval();
    }   
    if ( mismatch(dd2, d0) ) {
       cerr << "Bad getdata 2( " << eLon2 << ", " << eLat2 << ", " << eVrt2 << ") T value: " 
       << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    
    // Now do a time interpolation
    tyme3 = metsrc0->cal2Time( datem );
    dd3 = metsrc0->getData( "T", tyme3, eLon2, eLat2, eVrt2 );
    d0 = (tyme3 - tyme)/(tyme2 - tyme)*(dd2-dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata 3( " << eLon2 << ", " << eLat2 << ", " << eVrt2 << ") T value: " 
       << dd << " vs. " << d0 << endl;
       cerr << " 1: " << tyme  << ", " << dd  << endl;
       cerr << " 2: " << tyme2 << ", " << dd2 << endl;
       cerr << " 3: " << tyme3 << ", " << dd3 << endl;       
       exit(1);  
    }
    
    
    // get a second gridpoint at tyme, one longitude over
    dd2 = metsrc0->getData( "T", tyme, eLon4, eLat4, eVrt4   );
    d0 = eDat3d4;
    if ( mismatch(dd2, d0) ) {
       cerr << "Bad getdata Lo( " << eLon4 << ", " << eLat4 << ", " << eVrt4 << ") T value: " 
       << dd2 << " vs. " << d0 << endl;
       exit(1);  
    }
    // do longitude interpolation
    dd3 = metsrc0->getData( "T", tyme, eLon4a, eLat4, eVrt4   );
    d0 = (eLon4a - eLon2)/(eLon4 - eLon2)*(dd2-dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata LoInt( " << eLon4a << ", " << eLat4 << ", " << eVrt4 << ") T value: " 
       << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << eLon2 << ", " << dd << endl;
       cerr << " 2: " << eLon4 << ", " << dd2 << endl;
       cerr << " 3: " << eLon4a << ", " << dd3 << endl;       
       exit(1);  
    }


    // get another second gridpoint at tyme, one latitude over
    dd2 = metsrc0->getData( "T", tyme, eLon5, eLat5, eVrt5   );
    d0 = eDat3d5;
    if ( mismatch(dd2, d0) ) {
       cerr << "Bad getdata La(" << eLon5 << ", " << eLat5 << ", " << eVrt5 << ") T value: " 
       << dd2 << " vs. " << d0 << endl;
       exit(1);  
    }
    // do latitude interpolation
    dd3 = metsrc0->getData( "T", tyme,eLon5, eLat5a,  eVrt5   );
    d0 = (eLat5a - eLat2)/(eLat5 - eLat2)*(dd2-dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata LaInt(" << eLon5 << ", " << eLat5a << ", " << eVrt5 << ") T value: " 
       << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << eLat2 << ", " << dd << endl;
       cerr << " 2: " << eLat5 << ", " << dd2 << endl;
       cerr << " 3: " << eLat5a << ", " << dd3 << endl;       
       exit(1);  
    }

    // get another second gridpoint at tyme, one pressure level under
    dd2 = metsrc0->getData( "T", tyme, eLon6, eLat6, eVrt6   );
    d0 = eDat3d6;
    if ( mismatch(dd2, d0) ) {
       cerr << "Bad getdata Pr(" << eLon6 << ", " << eLat6 << ", " << eVrt6 << ") T value: " 
       << dd2 << " vs. " << d0 << endl;
       exit(1);  
    }
    // do pressure interpolation (linear)
    dd3 = metsrc0->getData( "T", tyme, eLon6, eLat6, eVrt6a  );
    d0 = (eVrt6a - eVrt2)/(eVrt6 - eVrt2)*(dd2 - dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata PrInt(" << eLon6 << ", " << eLat6 << ", " << eVrt6a << ") T value: " 
       << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << eVrt2 << ", " << dd << endl;
       cerr << " 2: " << eVrt6 << ", " << dd2 << endl;
       cerr << " 3: " << eVrt6a << ", " << dd3 << endl;       
       exit(1);  
    }

    // do pressure interpolation (log-linear)
    metsrc0->set_vinterp( new LogLinearVinterp(), true );
    dd3 = metsrc0->getData( "T", tyme, eLon6, eLat6, eVrt6a  );
    d0 = (LOG(eVrt6a) - LOG(eVrt2))/(LOG(eVrt6) - LOG(eVrt2))*(dd2 - dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata PrLogInt(" << eLon6 << ", " << eLat6 << ", " << eVrt6a << ") T value: " 
       << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << eVrt2 << ", " << dd << endl;
       cerr << " 2: " << eVrt6 << ", " << dd2 << endl;
       cerr << " 3: " << eVrt6a << ", " << dd3 << endl;       
       exit(1);  
    }
    
    
    delete metsrc0;
    

    //------------------------------------------------------------------

    // if we got this far, everything is OK
    exit(0);
    
}
