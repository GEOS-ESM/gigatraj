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
#include "gigatraj/MetMERRA.hh"
#include "gigatraj/LogLinearVinterp.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;


int main() 
{

    MERRA_Directory MDir;
    MetMERRA *metsrc0;
    string s1, s2;
    int status;
    int hgrid,vgrid,tspace,tavg,tbase, ndims;
    string *longname, *units, *url, *url2;
    string basedate, date2;
    real dd, dd2, dd3;
    real d0;
    double tyme, tyme2, tyme3;
    GridLatLonField3D *grid3d;
    GridLatLonFieldSfc *grid2d;
    float lon0,lat0,p0, c0;
    int nx,ny,nz;
    int i;

    /////////////////// Test the MERRA_Directory class here

    // get CF name from MERRA name
    s1 = MDir.CFName("TQV");
    if ( s1 != string("total_precipitable_water_vapor") ) {
       cerr << "Bad CFName Lookup:" << s1 << endl;
       exit(1);
    }
    // get MERRA name from CF name
    s2 = MDir.MERRAName(s1);
    if ( s2 != string("TQV") ) {
       cerr << "Bad MERAName Lookup:" << s2 << endl;
       exit(1);    
    }
    // try a nonexistent name
    status = 1;
    try {
       s1 = MDir.CFName("George Washington");
    } catch (MERRA_Directory::badName err) {
       status = 0;
    } 
    if ( status ) {
       cerr << "Failed to catch bad MERRAName" << endl;
       exit(1);
    }   
    status = 1;
    try {
       s1 = MDir.MERRAName("Thomas Jefferson");
    } catch (MERRA_Directory::badName err) {
       status = 0;
    } 
    if ( status ) {
       cerr << "Failed to catch bad CFName" << endl;
       exit(1);
    }   

    // Now look up zonal wind, with lax requirements
    status = MDir.LookUp( "U", "2001-07-15", -1, -1, -1, -1
             , &longname, &units, &ndims, &hgrid, &vgrid, &tspace, &tavg, &tbase, &url );

    if ( status ) {
       cerr << "Failed LookUp for U: not found" << endl;
       exit(1);
    }   
    if ( *longname != "Eastward wind component") {
       cerr << "Failed LookUp for U: long name : " << *longname << endl;
       exit(1);    
    } 
    if ( *units != "m/s" ) {
       cerr << "Failed LookUp for U: units : " << *units << endl;
       exit(1);        
    }
    if ( ndims != 3 ) {
       cerr << "Failed LookUp for U: ndims : " << ndims << endl;
       exit(1);        
    }
    if ( *url != "https://goldsmr3.gesdisc.eosdis.nasa.gov/opendap/MERRA/MAI6NPANA.5.2.0/2001/07/MERRA300.prod.assim.inst6_3d_ana_Np.20010715.hdf" ) {
       cerr << "Failed Look--Up for U: url : " << *url << endl;
       exit(1);        
    }
    delete longname;
    delete units;
    
    status = MDir.LookUp( "U", "2001-07-15", 2, 0, 6, 0
             , &longname, &units, &ndims, &hgrid, &vgrid, &tspace, &tavg, &tbase, &url2 );

    if ( status ) {
       cerr << "Failed LookUp for U with specifics: not found" << endl;
       exit(1);
    }   
    if ( *url != *url2 ) {
       cerr << "URLs for lax and strict Lookups do not match" << endl;
       exit(1);
    }   
    delete url;
    delete url2;

    // Now try it without retrieving any other information
    status = MDir.LookUp( "U", "2001-07-15", 2, 1, 3, 0
            , NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
    if ( status ) {
       cerr << "Failed LookUp for U with nulls: not found" << endl;
       exit(1);
    }
    
    
    //------------------------------------------------------------------
    
    // create a MERRA data object
    metsrc0 = new MetMERRA();
    //metsrc0->debug = 10;
    
    basedate = "2010-03-15T12:13";
    
    metsrc0->set_BaseTime(basedate);
    date2 = metsrc0->BaseTime();
    if ( basedate != date2 ) {
       cerr << "set_BaseTime failed:" << date2 << endl;
       exit(1);    
    } 
    delete metsrc0;
    
    metsrc0 = new MetMERRA(basedate);
    date2 = metsrc0->BaseTime();
    if ( basedate != date2 ) {
       cerr << "new MetMERRA(date) failed:" << date2 << endl;
       exit(1);    
    } 
    
    // test time conversion routines 
    tyme = metsrc0->cal2Time( "2010-03-17T00:13" );
    if ( mismatch(tyme, 1.5) ) {
       cerr << "Bad cal->time conversion: " << tyme << " vs. " << 1.5 << endl;
       exit(1);  
    }
    date2 = metsrc0->time2Cal( tyme );
    if ( date2 !=  "2010-03-17T00:13" ) {
       cerr << "Bad time->cal conversion: " << date2 << " vs.  2010-03-17T00:13" << endl;
       exit(1);  
    }
    tyme = metsrc0->cal2Time( "2010-03-17T00:00" );
    if ( mismatch(tyme, 1.49097) ) {
       cerr << "Bad cal->time conversion 2: " << tyme << " vs. " << 1.49097 << endl;
       exit(1);  
    }
    date2 = metsrc0->time2Cal( tyme );
    if ( date2 !=  "2010-03-17T00:00" ) {
       cerr << "Bad time->cal conversion 2: " << date2 << " vs.  2010-03-17T00:00" << endl;
       exit(1);  
    }
    //cerr << "xtime = " << tyme << endl;
  
    tyme = metsrc0->cal2Time( "2010-03-17T06:00" );

    //*************  Sfc-reading tests *******************************

    //metsrc0->debug = 10;

    // test sample values for a 2D field
    grid2d = metsrc0->GetSfc( "PS", "2010-03-17T06:00" );

    grid2d->dims( &nx, &ny );
    if ( nx != 540 || ny != 361 ) {
       cerr << "Bad grid2d dimensions: longitude " << nx << " vs. " << 540 << endl;
       cerr << "                     :  latitude " << ny << " vs. " << 361 << endl;
       exit(1);  
    }
    
    //for ( i=0; i < nx ; i++ ) {
    //    cerr << "lon[" << i << "] = " << grid2d->longitude(i) << endl;
    //}
    //for ( i=0; i < ny ; i++ ) {
    //    cerr << "lat[" << i << "] = " << grid2d->latitude(i) << endl;
    //}
    dd = grid2d->longitude(271);
    d0 = 0.66667;
    if ( mismatch(dd, d0,0.0001) ) {
       cerr << "Bad grid2d - longitude: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    dd = grid2d->latitude(181);
    d0 = 0.500000;
    if ( mismatch(dd, d0) ) {
       cerr << "Bad grid2d latitude: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }

    // std::cerr         << (*grid2d)(   0,   0)
    //      << ", " << (*grid2d)(nx-1,ny-1)
    //      << ", " << (*grid2d)(nx-1,   0)
    //      << ", " << (*grid2d)(   0,ny-1)
    //      << ", " << (*grid2d)(nx/2,ny/2)
    //      << std::endl;

    // check values
    d0 = 69467.0;
    dd = (*grid2d)(0,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D PS[" << 0 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = 102075.0;
    dd = (*grid2d)(nx-1,ny-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D PS[" << nx-1 << "," << ny-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 =   69467.0;
    dd = (*grid2d)(nx-1,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D PS[" << nx-1 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = 102075.0;
    dd = (*grid2d)(0,ny-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D PS[" << 0 << "," << ny-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = 100987.0;
    dd = (*grid2d)(nx/2,ny/2);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 2D PS[" << nx/2 << "," << ny/2 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    
    //metsrc0->debug = 0;
    
    // test direct access
    dd = metsrc0->getData( "PS", tyme, grid2d->longitude(nx/2), grid2d->latitude(ny/2), -999.0   );
    d0 = 100987.0;
    if ( mismatch(dd, d0) ) {
       cerr << "Bad getdata( " << grid2d->longitude(nx/2) << "," << grid2d->latitude(ny/2)<< ") PS value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    
    delete grid2d;

    //*************  3D-reading tests *******************************

    //metsrc0->debug = 1;
    
    grid3d = metsrc0->Get3D( "T", "2010-03-17T06:00" );

    
    // check the grid    
    grid3d->dims( &nx, &ny, &nz );
    if ( nx != 540 || ny != 361 || nz != 42 ) {
       cerr << "Bad grid3d dimensions: longitude " << nx << " vs. " << 540 << endl;
       cerr << "                     :  latitude " << ny << " vs. " << 361 << endl;
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

    dd = grid3d->longitude(271);
    d0 = 0.66667;
    if ( mismatch(dd, d0, 0.0001) ) {
       cerr << "Bad grid3d longitude: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    dd = grid3d->latitude(181);
    d0 = 0.500000;
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

    //std::cerr    << (*grid3d)(   0,   0,   0)
    //     << ", " << (*grid3d)(nx-1,ny-1,nz-1)
    //     << ", " << (*grid3d)(nx-1,   0,   0)
    //     << ", " << (*grid3d)(   0,ny-1,   0)
    //     << ", " << (*grid3d)(   0,   0,nz-1)
    //     << ", " << (*grid3d)(nx-1,ny-1,   0)
    //     << ", " << (*grid3d)(nx-1,   0,nz-1)
    //     << ", " << (*grid3d)(   0,ny-1,nz-1)
    //     << ", " << (*grid3d)(nx/2,ny/2,nz/2)
    //     << std::endl;

    d0 = grid3d->fillval();
    dd = (*grid3d)(0,0,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << 0 << "," << 0 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = 259.925;
    dd = (*grid3d)(nx-1,ny-1,nz-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << ny-1 << "," << nz-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = grid3d->fillval();
    dd = (*grid3d)(nx-1,0,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << 0 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = 245.794;
    dd = (*grid3d)(0,ny-1,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << 0 << "," << ny-1 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = 254.596;
    dd = (*grid3d)(0,0,nz-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << 0 << "," << 0 << "," << nz-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = 245.794;
    dd = (*grid3d)(nx-1,ny-1,0);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << ny-1 << "," << 0 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = 254.596;
    dd = (*grid3d)(nx-1,0,nz-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx-1 << "," << 0 << "," << nz-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    d0 = 259.925;
    dd = (*grid3d)(0,ny-1,nz-1);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << 0 << "," << ny-1 << "," << nz-1 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }
    
    d0 = 232.634;
    dd = (*grid3d)(nx/2,ny/2,nz/2);
    if ( mismatch(dd, d0) ) {
       cerr << "Bad 3D T[" << nx/2 << "," << ny/2 << "," << nz/2 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }

    //metsrc0->debug = 0;
    
    // test a quantity that is calculated on the fly
    d0 = (*grid3d)(nx/3,ny/4,nz/2) * POW( 1000.0/grid3d->level(nz/2), 2./7.);
    delete grid3d;
    grid3d = metsrc0->Get3D( "air_potential_temperature", "2010-03-17T06:00" );
    dd = (*grid3d)(nx/3,ny/4,nz/2);
    if ( mismatch(dd, d0, 0.01) ) {
       cerr << "Bad 3D  Theta[" << nx/2 << "," << ny/2 << "," << nz/2 << "] value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }


    lon0 = grid3d->longitude(271);
    lat0 = grid3d->latitude(181);
    p0   = grid3d->level(20);
    //cerr << " grid points "  << lon0 << ", " << lat0 << ", " << p0 << endl;
    
    // test direct access
    tyme = metsrc0->cal2Time( "2010-03-17T06:00" );
    c0   = grid3d->longitude(1);
    
    dd = metsrc0->getData( "T", tyme, lon0,lat0,p0   );
    d0 = 243.297;
    if ( mismatch(dd, d0) ) {
       cerr << "Bad getdata( " << lon0 << ", " << lat0 << ", " << p0 << ") T value: " << dd << " vs. " << d0 << endl;
       exit(1);  
    }

    // Now get a second time
    tyme2 = metsrc0->cal2Time( "2010-03-17T12:00" );
    dd2 = metsrc0->getData( "T", tyme2, lon0,lat0,p0    );
    d0 = 242.003;
    if ( mismatch(dd2, d0) ) {
       cerr << "Bad getdata 2(" << lon0 << ", " << lat0 << ", " << p0 << ") T value: " << dd2 << " vs. " << d0 << endl;
       exit(1);  
    }

    // Now do a time interpolation
    tyme3 = metsrc0->cal2Time( "2010-03-17T07:45" );
    dd3 = metsrc0->getData( "T", tyme3, lon0,lat0,p0    );
    d0 = (tyme3 - tyme)/(tyme2 - tyme)*(dd2-dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata 3(" << lon0 << ", " << lat0 << ", " << p0 << ") T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << tyme  << ", " << dd << endl;
       cerr << " 2: " << tyme2 << ", " << dd2 << endl;
       cerr << " 3: " << tyme3 << ", " << dd3 << endl;       
       exit(1);  
    }
    
    
    // get a second gridpoint at tyme, one longitude over
    c0 = grid3d->longitude(272);
    //cerr << "c0 = " << c0 << endl;
    dd2 = metsrc0->getData( "T", tyme, c0, lat0, p0   );
    d0 = 243.078;
    if ( mismatch(dd2, d0) ) {
       cerr << "Bad getdata Lo(" << c0 << ", " << lat0 << ", " << p0 << ") T value: " << dd2 << " vs. " << d0 << endl;
       exit(1);  
    }
    // do longitude interpolation
    dd3 = metsrc0->getData( "T", tyme, 1.000, lat0, p0   );
    d0 = (1.0 - lon0)/(c0 - lon0)*(dd2-dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata LoInt( 1.00, " << lat0 << ", " << p0 << ") T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << lon0 << ", " << dd << endl;
       cerr << " 2: " << c0 << ", " << dd2 << endl;
       cerr << " 3: " << 1.0000 << ", " << dd3 << endl;       
       exit(1);  
    }


    // get another second gridpoint at tyme, one latitude over
    c0 = grid3d->latitude(182);
    //cerr << "c0 = " << c0 << endl;
    dd2 = metsrc0->getData( "T", tyme, lon0, c0, p0   );
    d0 = 243.594;
    if ( mismatch(dd2, d0) ) {
       cerr << "Bad getdata La("  << lon0 << ",  " << c0 << ", " << p0 << ") T value: " << dd2 << " vs. " << d0 << endl;
       exit(1);  
    }
    // do latitude interpolation
    dd3 = metsrc0->getData( "T", tyme,lon0, 1.000,  p0   );
    d0 = (1.0 - lat0)/(c0 - lat0)*(dd2-dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata LaInt("  << lon0 << ",  " << " 1.0, " << p0 << ") T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << lat0 << ", " << dd << endl;
       cerr << " 2: " << c0 << ", " << dd2 << endl;
       cerr << " 3: " << 1.0000 << ", " << dd3 << endl;       
       exit(1);  
    }

    // get another second gridpoint at tyme, one pressure level under
    c0 = grid3d->level(19);
    //cerr << "c0 = " << c0 << endl;
    dd2 = metsrc0->getData( "T", tyme, lon0, lat0, c0 );
    d0 = 251.253;
    if ( mismatch(dd2, d0) ) {
       cerr << "Bad getdata Pr(" << lon0 << ", " << lat0 << ", " << c0 << ") T value: " << dd2 << " vs. " << d0 << endl;
       exit(1);  
    }
    // do pressure interpolation (linear)
    dd3 = metsrc0->getData( "T", tyme, lon0, lat0, 340.00000   );
    d0 = (340.0 - p0)/(c0 - p0)*(dd2 - dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata PrInt(" << lon0 << ", " << lat0 << ", 340.00000) T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << p0 << ", " << dd << endl;
       cerr << " 2: " << c0 << ", " << dd2 << endl;
       cerr << " 3: " << 340.0 << ", " << dd3 << endl;       
       exit(1);  
    }
    // do pressure interpolation (log-linear)
    metsrc0->set_vinterp( new LogLinearVinterp() );
    dd3 = metsrc0->getData( "T", tyme,  lon0, lat0, 340.00000   );
    d0 = (LOG(340.0) - LOG(p0))/(LOG(c0) - LOG(p0))*(dd2 - dd) + dd;
    if ( mismatch(dd3, d0) ) {
       cerr << "Bad getdata PrLogInt(" << lon0 << ", " << lat0 << ",  340.00000) T value: " << dd3 << " vs. " << d0 << endl;
       cerr << " 1: " << 300.0 << ", " << dd << endl;
       cerr << " 2: " << 350.0 << ", " << dd2 << endl;
       cerr << " 3: " << 340.0 << ", " << dd3 << endl;       
       exit(1);  
    }
    

/*
    
    // try to read temperatures
    //dd = metsrc0->getData( "T", 1.5*3600.0*24.0, 0.0, 45.0, 0.0 );
    //dd = metsrc0->getData( "T", 172125.0, 0.0, 45.0, 0.0 );
    //dd = metsrc0->getData( "T", tyme, 0.0, 45.0, 0.0 );
*/
    
    
    delete grid3d;
    delete metsrc0;
    

    //------------------------------------------------------------------

    // if we got this far, everything is OK
    exit(0);
    
}
