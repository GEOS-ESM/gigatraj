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
#include <time.h>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/MetGEOSfp.hh"
#include "gigatraj/LogLinearVinterp.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;


string t2d( time_t tyme ) 
{
    string result;
    struct tm *time_parts;
    CalGregorian cal;

    // convert to calendar components
    time_parts = gmtime( &tyme );
    result = cal.buildDate( time_parts->tm_year + 1900
                        , time_parts->tm_mon + 1
                        , time_parts->tm_mday
                        , time_parts->tm_hour
                        , time_parts->tm_min
                        );

    return result;

}

int main() 
{

    MetGEOSfp *metsrc0;
    string modelrun, modelrun0;
    string dayt1, dayt2;
    string dayt;
    string s1, s2;
    int status;
    int hgrid,vgrid,tspace,tavg,tbase, ndims;
    string *longname, *units, *url, *url2;
    string basedate, date2;
    real dd, dd2, dd3;
    real d0;
    real badval;
    double tyme, tyme2, tyme3;
    GridLatLonField3D *grid3d, *grid3d_b;
    GridLatLonFieldSfc *grid2d, *grid2d_b;
    int nx,ny,nz;
    int nx2,ny2,nz2;
    int i;
    time_t systime;
    float lon0, lat0, p0, c0;
    time_t expiring;


    /////////////////// Determine a model run that should be present
    
    // get the current time
    systime = time(0);
    
    // try 5 days ago, to ensure that it comes from the assimiltion
    dayt1 = t2d( systime - 5*24*3600 );
    dayt1 = dayt1.substr(0,10) + "T06:00";
    // try 2 days from now, to ensure that it comes from the forecasts
    dayt2 = t2d( systime + 2*24*3600 );
    dayt2 = dayt2.substr(0,10) + "T12:00";
    
    
    // we will use this as the base time for the met source
    basedate = t2d( systime );
    basedate = basedate.substr(0,10) + "T05:00";
    
    cerr << " basedate = " << basedate << endl;
    cerr << " dayt1 = " << dayt1 << endl;
    cerr << " dayt2 = " << dayt2 << endl;

 
    //------------------------------------------------------------------

    // create a GEOSfp data object
    metsrc0 = new MetGEOSfp(basedate);
    
    //metsrc0->dbug = 10;

    date2 = metsrc0->BaseTime();
    if ( basedate != date2 ) {
       cerr << "set_BaseTime failed:" << date2 << endl;
       exit(1);    
    }    

    // test time conversion routines 
    tyme = metsrc0->cal2Time( dayt2 );
    if ( mismatch(tyme,2.29167 ) ) {
       cerr << "Bad cal->time conversion: " << tyme << " vs. " << 2.29167 << endl;
       exit(1);  
    }
    date2 = metsrc0->time2Cal( tyme );
    if ( date2 !=  dayt2 ) {
       cerr << "Bad time->cal conversion: " << date2 << " vs. " << dayt1 << endl;
       exit(1);  
    }
  
    //*************  Sfc-reading tests *******************************

    //metsrc0->dbug = 10;


    // test sample values for a forecast 2D field
    grid2d_b = metsrc0->GetSfc( "ps", dayt2 );
delete grid2d_b;
delete metsrc0;
exit(0);
    
    // check that the expiration time is non-zero
    expiring = grid2d_b->expires();
    if ( expiring == 0 ) {
       cerr << "zero expiration date for 2D forecast" << endl;
       exit(1);  
    }
    
    delete grid2d_b;

    tyme = metsrc0->cal2Time(dayt1);
    
    // test sample values for an assimilation 2D field
    grid2d = metsrc0->GetSfc( "ps", dayt1 );
    
    // check that the expiration time is non-zero
    expiring = grid2d->expires();
    if ( expiring != 0 ) {
       cerr << "non-zero expiration date for 2D assimilation" << endl;
       exit(1);  
    }
    
    
    // check the grid    
    grid2d->dims( &nx, &ny );
    if ( nx != 1152 || ny != 721 ) {
       cerr << "Bad grid2d dimensions: longitude " << nx << " vs. " << 1152 << endl;
       cerr << "                     :  latitude " << ny << " vs. " << 721 << endl;
       exit(1);  
    }
        
    //for ( i=0; i < nx ; i++ ) {
    //    cerr << "lon[" << i << "] = " << grid2d->longitude(i) << endl;
    //}
    //for ( i=0; i < ny ; i++ ) {
    //    cerr << "lat[" << i << "] = " << grid2d->latitude(i) << endl;
    //}
    dd = grid2d->longitude(577);
    d0 = 0.31250;
    if ( mismatch(dd, d0) ) {
       cerr << "Bad grid2d longitude: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    dd = grid2d->latitude(361);
    d0 = 0.250000;
    if ( mismatch(dd, d0) ) {
       cerr << "Bad grid2d latitude: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    
    // Cannot check precise values, since the forecasts will
    // change from day to day. but we can check for
    // physical reasonableness.
    // (Note: surface pressure ps is in Pa, not hPa)
    dd = (*grid2d)(0,0);
    if ( dd < 50000.0 || dd > 150000.0 ) {
       cerr << "Bad 2D [" << 0 << "," << 0 << "] PS value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid2d)(nx-1,ny-1);
    if ( dd < 50000.0 || dd > 150000.0 ) {
       cerr << "Bad 2D [" << nx-1 << "," << ny-1 << "] PS value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid2d)(nx-1,0);
    if ( dd < 50000.0 || dd > 150000.0 ) {
       cerr << "Bad 2D [" << nx-1 << "," << 0 << "] PS value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid2d)(0,ny-1);
    if ( dd < 50000.0 || dd > 150000.0 ) {
       cerr << "Bad 2D [" << 0 << "," << ny-1 << "] PS value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid2d)(nx/2,ny/2);
    if ( dd < 50000.0 || dd > 150000.0 ) {
       cerr << "Bad 2D [" << nx/2 << "," << ny/2 << "] PS value: " << dd << " out of range " << endl;
       exit(1);  
    }
    
    //metsrc0->dbug = 0;

    // test direct access
    dd = metsrc0->getData( "ps", tyme, grid2d->longitude(nx/2+1), grid2d->latitude(ny/2+1), -999.0   );
    d0 = (*grid2d)( nx/2+1, ny/2+1 );
    if ( mismatch(dd, d0) ) {
       cerr << "Bad getdata( " << grid2d->longitude(nx/2) << "," << grid2d->latitude(ny/2)<< ") PS value: " << dd << " vs " << d0 << endl;
       exit(1);  
    }
    
    // test thinned-out grid
    //metsrc0->
    //grid2d_b = 
    
    // now look at thinned-out data
    
    metsrc0->set_thinning( 4 );
    
    grid2d_b = metsrc0->GetSfc( "ps", dayt1 );
    grid2d_b->dims( &nx2, &ny2 );
    if ( nx2 != 288 || ny2 != 181 ) {
       cerr << "Bad thinned grid2d dimensions: longitude " << nx2 << " vs. " << 288 << endl;
       cerr << "                     :  latitude " << ny2 << " vs. " << 181 << endl;
       exit(1);  
    }

    delete grid2d_b;
    delete grid2d;


    // restore to full resolution.
    metsrc0->set_thinning( 0 );
    

    //*************  3D-reading tests *******************************

    //metsrc0->dbug = 10;
    

    // test sample values for a forecast 3D field
    grid3d_b = metsrc0->Get3D( "t", dayt2 );
    
    // check that the expiration time is non-zero
    expiring = grid3d_b->expires();
    if ( expiring == 0 ) {
       cerr << "zero expiration date for 3D forecast" << endl;
       exit(1);  
    }
    
    delete grid3d_b;
    
    tyme = metsrc0->cal2Time(dayt1);
    
    // test sample values for an assimilation 2D field
    grid3d = metsrc0->Get3D( "t", dayt1 );
    
    // check that the expiration time is zero
    expiring = grid3d->expires();
    if ( expiring != 0 ) {
       cerr << "zero expiration date for 3D assimilation" << endl;
       exit(1);  
    }
    
    // check the grid    
    grid3d->dims( &nx, &ny, &nz );
    if ( nx != 1152 || ny != 721 || nz != 42 ) {
       cerr << "Bad grid2d dimensions: longitude " << nx << " vs. " << 1152 << endl;
       cerr << "                     :  latitude " << ny << " vs. " << 721 << endl;
       cerr << "                     :  vertical " << nz << " vs. " << 42 << endl;
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

    dd = grid3d->longitude(577);
    d0 = 0.3125000;
    if ( mismatch(dd, d0) ) {
       cerr << "Bad grid3d longitude: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    dd = grid3d->latitude(361);
    d0 = 0.2500000;
    if ( mismatch(dd, d0) ) {
       cerr << "Bad grid3d latitude: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    dd = grid3d->level(21);
    d0 = 250.0;
    if ( mismatch(dd, d0) ) {
       cerr << "Bad grid3d level: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }


    // some of the values we test may be bad-fill values
    badval = grid3d->fillval();
    
    dd = (*grid3d)(0,0,0);
    if ( ( dd != badval ) && ( dd < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad 3D T[" << 0 << "," << 0 << "," << 0 << "] value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid3d)(nx-1,ny-1,nz-1);
    if ( ( dd != badval ) && ( dd < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << ny-1 << "," << nz-1 << "] value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid3d)(nx-1,0,0);
    if ( ( dd != badval ) && ( dd < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << 0 << "," << 0 << "] value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid3d)(0,ny-1,0);
    if ( ( dd != badval ) && ( dd < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad 3D T[" << 0 << "," << ny-1 << "," << 0 << "] value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid3d)(0,0,nz-1);
    if ( ( dd != badval ) && ( dd < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad 3D T[" << 0 << "," << 0 << "," << nz-1 << "] value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid3d)(nx-1,ny-1,0);
    if ( ( dd != badval ) && ( dd < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << ny-1 << "," << 0 << "] value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid3d)(nx-1,0,nz-1);
    if ( ( dd != badval ) && ( dd < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << 0 << "," << nz-1 << "] value: " << dd << " out of range " << endl;
       exit(1);  
    }
    dd = (*grid3d)(0,ny-1,nz-1);
    if ( ( dd != badval ) && ( dd < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad 3D T[" << 0 << "," << ny-1 << "," << nz-1 << "] value: " << dd << " out of range " << endl;
       exit(1);  
    }    
    dd = (*grid3d)(nx/2,ny/2,nz/2);
    if ( ( dd != badval ) && ( dd < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad 3D T[" << nx/2 << "," << ny/2 << "," << nz/2 << "] value: " << dd << " out of range " << endl;
       exit(1);  
    }
    
    delete grid3d;
    

    // test sample values for an assimilation 2D field
    grid3d = metsrc0->Get3D( "t", dayt1 );
    
    // test thinning
    metsrc0->set_thinning( 4 );
    
    grid3d_b = metsrc0->Get3D( "t", dayt1 );
    grid3d_b->dims( &nx2, &ny2, &nz2 );
    if ( nx2 != 288 || ny2 != 181 || nz2 != 42 ) {
       cerr << "Bad thinned grid2d dimensions: longitude " << nx2 << " vs. " << 288 << endl;
       cerr << "                     :  latitude " << ny2 << " vs. " << 181 << endl;
       cerr << "                     :  vertical " << nz2 << " vs. " << 42 << endl;
       exit(1);  
    }
    
    
    delete grid3d_b;
    
    metsrc0->set_thinning( 0 );


    //metsrc0->dbug = 0;
    //metsrc0->dbug = 10;
    
    // test a quantity that is calculated on the fly
    d0 = (*grid3d)(nx/3,ny/4,nz/2) * POW( 1000.0/grid3d->level(nz/2), 2./7.);
    delete grid3d;
    grid3d = metsrc0->Get3D( "air_potential_temperature", dayt1 );
    dd = (*grid3d)(nx/3,ny/4,nz/2);
    if ( mismatch(dd, d0, 0.01) ) {
       cerr << "Bad 3D  Theta[" << nx/2 << "," << ny/2 << "," << nz/2 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }

    lon0 = grid3d->longitude(577);
    lat0 = grid3d->latitude(361);
    p0 = grid3d->level(20); 

    delete grid3d;
   
    grid3d = metsrc0->Get3D( "t", dayt1 );
        
    // test direct access
    tyme = metsrc0->cal2Time( dayt1 );
    dd = metsrc0->getData( "t", tyme, lon0, lat0, p0   );
    d0 = (*grid3d)(577,361,20);
    if ( mismatch(dd,d0) ) {
       cerr << "Bad getdata(" << lon0 << "," << lat0 << "," << p0 << ") value: " << dd << " vs " << d0 << endl;
       exit(1);  
    }


    // Now get a second time
    tyme2 = tyme + 3.0/24.0;
    dd2 = metsrc0->getData( "t", tyme2, lon0, lat0, p0  );
    if ( ( dd2 != badval ) && ( dd2 < 130.0 || dd2 > 400.0  ) ) {
       cerr << "Bad getdata(" << lon0 << "," << lat0 << "," << p0 << ") value: " << dd2 << " out of range " << endl;
       exit(1);  
    }
    
    // Now do a time interpolation
    tyme3 = tyme + 1.2345/24.0;
    dd3 = metsrc0->getData( "t", tyme3, lon0, lat0, p0  );
    d0 = (tyme3 - tyme)/(tyme2 - tyme)*(dd2-dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata 3(" << lon0 << "," << lat0 << "," << p0 << ") T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << tyme  << ", " << dd  << endl;
       cerr << " 2: " << tyme2 << ", " << dd2 << endl;
       cerr << " 3: " << tyme3 << ", " << dd3 << endl;       
       exit(1);  
    }
    
    
    // get a second gridpoint at tyme, one longitude over
    c0 = grid3d->longitude(578);
    dd2 = metsrc0->getData( "t", tyme, c0, lat0, p0   );
    if ( ( dd2 != badval ) && ( dd2 < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad getdata Lo(" << c0 << "," << lat0 << "," << p0 << ") T value: " << dd2  << " out of range " << endl;
       exit(1);  
    }
    // do longitude interpolation
    dd3 = metsrc0->getData( "t", tyme, 0.5, lat0, p0   );
    d0 = (0.5 - lon0)/(c0 - lon0)*(dd2-dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata LoInt( 0.5, " << lat0 << ", " << p0 << " ) T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << lon0 << ", " << dd << endl;
       cerr << " 2: " << c0 << ", "   << dd2 << endl;
       cerr << " 3: " << 0.5 << ", " << dd3 << endl;       
       exit(1);  
    }

    // get another second gridpoint at tyme, one latitude over
    c0 = grid3d->latitude(362);
    dd2 = metsrc0->getData( "t", tyme, lon0, c0, p0   );
    if ( ( dd2 != badval ) && ( dd2 < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad getdata La(" << lon0 << "," << c0 << "," << p0 << ") T value: " << dd2  << " out of range " << endl;
       exit(1);  
    }
    // do latitude interpolation
    dd3 = metsrc0->getData( "t", tyme,lon0, 0.47, p0   );
    d0 = (0.47 - lat0)/(c0 -lat0)*(dd2-dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata LaInt( " << lon0 << "," << 0.47 << "," << p0 << " ) T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << lat0 << ", " << dd << endl;
       cerr << " 2: " << c0   << ", " << dd2 << endl;
       cerr << " 3: " << 0.47 << ", " << dd3 << endl;       
       exit(1);  
    }


    // get another second gridpoint at tyme, one pressure level under
    c0 = grid3d->level(19);
    dd2 = metsrc0->getData( "t", tyme, lon0, lat0, c0   );
    if ( ( dd2 != badval ) && ( dd2 < 130.0 || dd > 400.0  ) ) {
       cerr << "Bad getdata Pr(" << lon0 << "," << lat0 << "," << c0 << ") T value: " << dd2  << " out of range " << endl;
       exit(1);  
    }
    // do pressure interpolation (linear)
    dd3 = metsrc0->getData( "t", tyme, lon0, lat0,  340.00000   );
    d0 = (340.0 - p0)/(c0 - p0)*(dd2 - dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata PrInt( " << lon0 << ", " << lat0 << ", 340.00000) T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << p0 << ", "    << dd << endl;
       cerr << " 2: " << c0 << ", "    << dd2 << endl;
       cerr << " 3: " << 340.0 << ", " << dd3 << endl;       
       exit(1);  
    }
    // do pressure interpolation (log-linear)
    metsrc0->set_vinterp( new LogLinearVinterp(), true );
    dd3 = metsrc0->getData( "t", tyme, lon0, lat0,  340.00000   );
    d0 = (LOG(340.0) - LOG(p0))/(LOG(c0) - LOG(p0))*(dd2 - dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata PrLogInt( " << lon0 << ", " << lat0 << ", 340.00000) T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << p0    << ", " << dd << endl;
       cerr << " 2: " << c0    << ", " << dd2 << endl;
       cerr << " 3: " << 340.0 << ", " << dd3 << endl;       
       exit(1);  
    }


    delete grid3d;

    
    delete metsrc0;
    

    //------------------------------------------------------------------

    // if we got this far, everything is OK
    exit(0);
    
}
