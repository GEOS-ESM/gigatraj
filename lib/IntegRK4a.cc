
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/IntegRK4a.hh"

using namespace gigatraj;

const std::string IntegRK4a::id = "IntegRK4";

const real gigatraj::IntegRK4a::NEARPOLE = 88.0;

IntegRK4a :: IntegRK4a()
{
    // the default is to apply no conformal adjustment
    confml = 0;
}

void IntegRK4a :: go( real &lon, real &lat, real &z, double &t, MetData *metsrc, PlanetNav *nav, double dt0 )
{
    // coefficients for the Runga-Kutte integration
    real k1u,k1v,k1w, k2u,k2v,k2w, k3u,k3v,k3w, k4u,k4v,k4w;
    // changes in longitude, latitude, and vertical cooridates, respectively
    real dx,dy,dz;
    // intermediate longotude and latitude variables
    real tmplon,tmplat;
    // sine and cosine of the longitude, for tilting the winds near the poles 
    real cdlon,sdlon;
    // adjusted/"rotate" wind components
    real up,vp;
    // time in seconds
    real dt;
    // delta-lons and delta-lats, delta-zs
    real dlon1, dlon2, dlon3, dlon4;
    real dlat1, dlat2, dlat3, dlat4;
    real dz1, dz2, dz3, dz4;
    std::string dyt;
    real r;
    real mg1, mg2;
    int debug;
    
    debug = metsrc->debug;
    //debug = 101;

    // get the planetary radius
    r = 6371.0;
    
    // convert days to seconds
    dt = dt0 * 86400.0;

    //std::cerr << "IntegRK4 BEGIN: time0=" << t << ", dt0=" << dt0 << std::endl;
    // note: in general, z will need to be translated to the met levels
    // but this is handled by the met source, internally
    metsrc->get_uvw(t, lon, lat, z,  &k1u, &k1v, &k1w);
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(t, 3);
       std::cerr << "     IntegRK4a @ (" << t << "/" << dyt << ", " << lon << "," << lat << ", " << z << ", " << t << "): u1=" << k1u << ", v1=" << k1v << std::endl;
    }
    // convert m/s to km/s 
    k1u = k1u/1000.0;
    k1v = k1v/1000.0;

    
    // advance the parcel halfway through the time delta, using the winds
    dlat1 = dt*k1v/r/RCONV;
    tmplon = lon;
    tmplat = lat;
    nav->deltapos( &tmplon, &tmplat, 0.0, dlat1/2.0 );
    if ( ABS(tmplat) != 90.0 ) {
       dlon1 = dt*k1u/r/RCONV/COS(tmplat*RCONV);
    } else {
       dlon1 = 0.0;
    }
    dz1 = dt*k1w;
    if ( debug >= 100 ) {
       std::cerr << "     IntegRK4a: dlon1, dlat1 = " << dlon1 << ", " << dlat1 << std::endl;
    }
    tmplon = lon;
    tmplat = lat;
    nav->deltapos( &tmplon, &tmplat, dlon1/2, dlat1/2 );          
    // get the winds at this intermediate point
    //std::cerr << "IntegRK4: time1=" << t + dt0/2.0 << std::endl;
    metsrc->get_uvw(t + dt0/2.0, tmplon, tmplat, z + dz1/2.0 ,  &k2u, &k2v, &k2w );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(t + dt0/2.0, 3);
       std::cerr << "     IntegRK4a @ (" << t << "/" << dyt << ", "  << tmplon << "," << tmplat << ", " << z+dz1/2.0 << ", " << t+dt0/2.0 << "): u2=" << k2u << ", v2=" << k2v << std::endl;
    }
    k2u = k2u/1000.0;
    k2v = k2v/1000.0;
    // "rotate" the winds if we are too near one of the poles

    if ( confml == 1 ) {
      if ( lat >= NEARPOLE || lat <= -NEARPOLE || tmplat >= NEARPOLE || tmplat <= -NEARPOLE ) 
      {
         cdlon = COS( (tmplon - lon)*RCONV  );
         sdlon = SIN( (tmplon - lon)*RCONV  );
         up =  k2u*cdlon + k2v*sdlon; 
         vp = -k2u*sdlon + k2v*cdlon;
         k2u = up;
         k2v = vp;
         //std::cerr << "hey!" << std::endl;
      }
    }


    // repeat the procedure using the winds at the intermediate point
    dlat2 = dt*k2v/r/RCONV;
    tmplon = lon;
    tmplat = lat;
    nav->deltapos( &tmplon, &tmplat, 0.0, dlat2/2.0 );
    if ( ABS(tmplat) != 90.0 ) {
       dlon2 = dt*k2u/r/RCONV/COS(tmplat*RCONV);
    } else {
       dlon2 = 0.0;
    }
    dz2 = dt*k2w;
    if ( debug >= 100 ) {
       std::cerr << "     IntegRK4a: dlon2, dlat2 = " << dlon2 << ", " << dlat2 << std::endl;
    }
    tmplon = lon;
    tmplat = lat;
    nav->deltapos( &tmplon, &tmplat, dlon2/2.0, dlat2/2.0 );
    //std::cerr << "IntegRK4: time2=" << t + dt0/2.0 << std::endl;
    metsrc->get_uvw(t + dt0/2.0, tmplon, tmplat, z + dz2/2.0, &k3u, &k3v, &k3w );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(t + dt0/2.0, 3);
       std::cerr << "     IntegRK4a @ (" << t << "/" << dyt << ", "  << tmplon << "," << tmplat << ", " << z+dz2/2.0 << ", " << t+dt0/2.0 << "): u3=" << k3u << ", v3=" << k3v << std::endl;
    }
    k3u = k3u/1000.0;
    k3v = k3v/1000.0;
    // "rotate" the winds if we are too near one of the poles
    if ( confml == 1 ) {
      if ( lat >= NEARPOLE || lat <= -NEARPOLE || tmplat >= NEARPOLE || tmplat <= -NEARPOLE ) 
      {
         cdlon = COS( (tmplon - lon)*RCONV  );
         sdlon = SIN( (tmplon - lon)*RCONV  );
         up =  k3u*cdlon + k3v*sdlon; 
         vp = -k3u*sdlon + k3v*cdlon;
         k3u = up;
         k3v = vp; 
      }
    }


    // and once again. using the winds from the previous step 
    dlat3 = dt*k3v/r/RCONV;
    tmplon = lon;
    tmplat = lat;
    nav->deltapos( &tmplon, &tmplat, 0.0, dlat3/2.0 ); // (note the 1/2.0 !)
    if ( ABS(tmplat) != 90.0 ) {
       dlon3 = dt*k3u/r/RCONV/COS(tmplat*RCONV);
    } else {
       dlon3 = 0.0;
    }
    dz3 = dt*k3w;
    if ( debug >= 100 ) {
       std::cerr << "     IntegRK4a: dlon3, dlat3 = " << dlon3 << ", " << dlat3 << std::endl;
    }
    tmplon = lon;
    tmplat = lat;
    nav->deltapos( &tmplon, &tmplat, dlon3, dlat3 );
   //std::cerr << "IntegRK4: time3=" << t + dt0 << std::endl;
    metsrc->get_uvw(t + dt0, tmplon, tmplat, z + dz3,   &k4u, &k4v, &k4w );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(t + dt0, 3);
       std::cerr << "     IntegRK4a @ (" << t << "/" << dyt << ", "  << tmplon << "," << tmplat << ", " << z+dt*k3w/2.0 << ", " << t+dt0 << "): u4=" << k4u << ", v4=" << k4v << std::endl;
    }
    k4u = k4u/1000.0;
    k4v = k4v/1000.0;
    // "rotate" the winds if we are too near one of the poles
    if ( confml == 1 ) {
      if ( lat >= NEARPOLE || lat <= -NEARPOLE || tmplat >= NEARPOLE || tmplat <= -NEARPOLE ) 
      {
       cdlon = COS( (tmplon - lon)*RCONV  );
       sdlon = SIN( (tmplon - lon)*RCONV  );
       up =  k4u*cdlon + k4v*sdlon; 
       vp = -k4u*sdlon + k4v*cdlon;
       k4u = up;
       k4v = vp; 
      }
    }

    // final deltas
    dlat4 = dt*k4v/r/RCONV;
    tmplon = lon;
    tmplat = lat;
    nav->deltapos( &tmplon, &tmplat, 0.0, dlat4/2.0 );
    if ( ABS(tmplat) != 90.0 ) {
       dlon4 = dt*k4u/r/RCONV/COS(tmplat*RCONV);
    } else {
       dlon4 = 0.0;
    }
    dz4 = dt*k4w;
    if ( debug >= 100 ) {
       std::cerr << "     IntegRK4a: dlon4, dlat4 = " << dlon4 << ", " << dlat4 << std::endl;
    }
    
    
    // aply the Runga-Kutte method to get the
    // changes in lon,lat, and z
    dx = ( dlon1 + 2.0*dlon2 + 2.0*dlon3 + dlon4 )/6.0;
    dy = ( dlat1 + 2.0*dlat2 + 2.0*dlat3 + dlat4 )/6.0;
    dz = (   dz1 + 2.0*dz2   + 2.0*dz3   + dz4)/6.0;
    
    //dx =  dt/6.0*(k1u + 2.0*k2u + 2.0*k3u + k4u);
    //dy =  dt/6.0*(k1v + 2.0*k2v + 2.0*k3v + k4v);
    //dz =  dt/6.0*(k1w + 2.0*k2w + 2.0*k3w + k4w);
    
    // update the longitude and latitude
    nav->deltapos( &lon, &lat, dx, dy );
    if ( debug >= 100 ) {
       std::cerr << "     IntegRK4a: dx, dy, lat, lon = " << dx << ", " << dy << ", " << lat << ", " << lon << std::endl;
    }

    // update the vertical coordinate
    z += dz;
    // advance the time
    t += dt0;
    //std::cerr << "IntegRK4 END: time=" << t << ", dt0=" << dt0 << std::endl;


}

void IntegRK4a :: go( int n, real *lons, real *lats, real *zs, int *flags, double &t, MetData *metsrc, PlanetNav *nav, double dt0 )
{
    real lon, lat, z;
    // coefficients for the Runga-Kutte integration
    real k1u,k1v,k1w, k2u,k2v,k2w, k3u,k3v,k3w, k4u,k4v,k4w;
    // changes in longitude, latitude, and vertical cooridates, respectively
    real dx,dy,dz;
    // intermediate longitude and latitude variables
    real tmplon,tmplat,tmpz;
    // sine and cosine of the longitude, for tilting the winds near the poles 
    real cdlon,sdlon;
    // adjusted/"rotate" wind components
    real up,vp;
    // time in seconds
    real dt;
    // delta-lons and delta-lats, delta-zs
    real dlon1, dlon2, dlon3, dlon4;
    real dlat1, dlat2, dlat3, dlat4;
    real dz1, dz2, dz3, dz4;
    std::string dyt;
    real r;
    real mg1, mg2;
    int debug;
    int i;
    int ii;
    int flag;
    const char *nanstr = "";
    int nuse;
    
    debug = metsrc->debug;
    //debug = 101;

    // get the planetary radius
    r = 6371.0;
    
    // convert days to seconds
    dt = dt0 * 86400.0;
    
    int*  const iused = new int[n];

    real* const kus = new real[n];
    real* const kvs = new real[n];
    real* const kws = new real[n];

    real* const dlons1 = new real[n];
    real* const dlats1 = new real[n];
    real* const dzs1 = new real[n];

    real* const dlons2 = new real[n];
    real* const dlats2 = new real[n];
    real* const dzs2 = new real[n];

    real* const dlons3 = new real[n];
    real* const dlats3 = new real[n];
    real* const dzs3 = new real[n];

    real* const tmplons = new real[n];
    real* const tmplats = new real[n];
    real* const tmpzs = new real[n];

    nuse = 0;
    ii = -1;
    for ( i=0; i<n; i++ ) {
        if ( flags[i] == 0 ) {
           nuse++;
           ii++;
           iused[ii] = i;
           //tmplons[ii] = lons[i];
           //tmplats[ii] = lats[i];
           //tmpzs[ii] = zs[i];
        }
    
    }
    
    
    metsrc->get_uvw(t, nuse, lons, lats, zs, kus, kvs, kws);
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(t, 3);
       std::cerr << "     IntegRK4a-vector @ (" << t << "/" << dyt << ", " << lons[0] << "," << lats[0] 
       << ", " << zs[0] << ", " << t << "): u1=" << kus[0] << ", v1=" << kvs[0] << std::endl;
    }

    for ( i=0; i<nuse; i++ ) {
    
        ii = iused[i];
    
        if ( FINITE(kus[i]) && FINITE(kvs[i]) && FINITE(kws[i]) ) {
    
           // convert m/s to km/s 
           k1u = kus[i]/1000.0;
           k1v = kvs[i]/1000.0;
    
           // advance the parcel halfway through the time delta, using the winds
           dlat1 = dt*k1v/r/RCONV;
           tmplon = lons[ii];
           tmplat = lats[ii];
           nav->deltapos( &tmplon, &tmplat, 0.0, dlat1/2.0 );
           if ( ABS(tmplat) != 90.0 ) {
              dlon1 = dt*k1u/r/RCONV/COS(tmplat*RCONV);
           } else {
              dlon1 = 0.0;
           }
           dz1 = dt*kws[i];
           if ( debug >= 100 && i == 0 ) {
              std::cerr << "     IntegRK4a-vector: dlon1, dlat1 = " << dlon1 << ", " << dlat1 << std::endl;
           }
           tmplon = lons[i];
           tmplat = lats[i];
           nav->deltapos( &tmplon, &tmplat, dlon1/2, dlat1/2 );          
           // get the winds at this intermediate point
           //std::cerr << "IntegRK4-vector: time1=" << t + dt0/2.0 << std::endl;

           tmpz =  zs[ii] + dz1/2.0;

        } else {
           dlon1 = RNAN(nanstr);
           dlat1 = RNAN(nanstr);
           dz1 = RNAN(nanstr);
           tmplon = lons[ii];
           tmplat = lats[ii];
           tmpz = zs[ii];
        }
        dlons1[i] = dlon1;
        dlats1[i] = dlat1;
        dzs1[i] = dz1;
        tmplons[i] = tmplon;
        tmplats[i] = tmplat;
        tmpzs[i] = tmpz;
    }

    metsrc->get_uvw(t + dt0/2.0, nuse, tmplons, tmplats, tmpzs, kus, kvs, kws );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(t + dt0/2.0, 3);
       std::cerr << "     IntegRK4a-vector @ (" << t << "/" << dyt << ", "  << tmplons[0] << "," << tmplats[0] 
                 << ", " << tmpzs[0] << ", " << t+dt0/2.0 << "): u2=" << kus[0] << ", v2=" << kvs[0] << std::endl;
    }

    for ( i=0; i<nuse; i++ ) {    
    
        ii = iused[i];
    
        if ( FINITE(kus[i]) && FINITE(kvs[i]) && FINITE(kws[i]) 
          && FINITE(dlons1[i]) && FINITE(dlats1[i]) && FINITE(dzs1[i]) ) {
    
           k2u = kus[i]/1000.0;
           k2v = kvs[i]/1000.0;
           // "rotate" the winds if we are too near one of the poles

           if ( confml == 1 ) {
              if ( lats[ii] >= NEARPOLE || lats[ii] <= -NEARPOLE || tmplats[i] >= NEARPOLE || tmplats[i] <= -NEARPOLE ) 
              {
                 cdlon = COS( (tmplons[i] - lons[ii])*RCONV  );
                 sdlon = SIN( (tmplons[i] - lons[ii])*RCONV  );
                 up =  k2u*cdlon + k2v*sdlon; 
                 vp = -k2u*sdlon + k2v*cdlon;
                 k2u = up;
                 k2v = vp;
                 //std::cerr << "hey!" << std::endl;
              }
           }


           // repeat the procedure using the winds at the intermediate point
           dlat2 = dt*k2v/r/RCONV;
           tmplon = lons[ii];
           tmplat = lats[ii];
           nav->deltapos( &tmplon, &tmplat, 0.0, dlat2/2.0 );
           if ( ABS(tmplat) != 90.0 ) {
              dlon2 = dt*k2u/r/RCONV/COS(tmplat*RCONV);
           } else {
              dlon2 = 0.0;
           }
           dz2 = dt*kws[i];
           if ( debug >= 100 && i == 0 ) {
              std::cerr << "     IntegRK4a-vector: dlon2, dlat2 = " << dlon2 << ", " << dlat2 << std::endl;
           }
           tmplon = lons[ii];
           tmplat = lats[ii];
           nav->deltapos( &tmplon, &tmplat, dlon2/2.0, dlat2/2.0 );
    
           tmpz =  zs[ii] + dz2/2.0;
        } else {
           dlon2 = RNAN(nanstr);
           dlat2 = RNAN(nanstr);
           dz2   = RNAN(nanstr);
           tmplon = lons[ii];
           tmplat = lats[ii];
           tmpz = zs[ii];
        }
        
        dlons2[i] = dlon2;
        dlats2[i] = dlat2;
        dzs2[i] = dz2;
        tmplons[i] = tmplon;
        tmplats[i] = tmplat;
        tmpzs[i] =  tmpz;
    } 

    //std::cerr << "IntegRK4-vector: time2=" << t + dt0/2.0 << std::endl;
    metsrc->get_uvw(t + dt0/2.0, nuse, tmplons, tmplats, tmpzs, kus, kvs, kws );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(t + dt0/2.0, 3);
       std::cerr << "     IntegRK4a-vector @ (" << t << "/" << dyt << ", "  << tmplons[0] << "," << tmplats[0] 
                 << ", " << tmpzs[0] << ", " << t+dt0/2.0 << "): u3=" << kus[0] << ", v3=" << kvs[0] << std::endl;
    }

    for ( i=0; i<nuse; i++ ) {    
    
        ii = iused[i];
    
        if ( FINITE(kus[i]) && FINITE(kvs[i]) && FINITE(kws[i]) 
          && FINITE(dlons1[i]) && FINITE(dlats1[i]) && FINITE(dzs1[i])
          && FINITE(dlons2[i]) && FINITE(dlats2[i]) && FINITE(dzs2[i]) ) {
    
           k3u = kus[i]/1000.0;
           k3v = kvs[i]/1000.0;
           // "rotate" the winds if we are too near one of the poles
           if ( confml == 1 ) {
              if ( lats[ii] >= NEARPOLE || lats[ii] <= -NEARPOLE || tmplats[i] >= NEARPOLE || tmplats[i] <= -NEARPOLE ) 
              {
                 cdlon = COS( (tmplons[i] - lons[ii])*RCONV  );
                 sdlon = SIN( (tmplons[i] - lons[ii])*RCONV  );
                 up =  k3u*cdlon + k3v*sdlon; 
                 vp = -k3u*sdlon + k3v*cdlon;
                 k3u = up;
                 k3v = vp; 
              }
           }

           // and once again. using the winds from the previous step 
           dlat3 = dt*k3v/r/RCONV;
           tmplon = lons[ii];
           tmplat = lats[ii];
           nav->deltapos( &tmplon, &tmplat, 0.0, dlat3/2.0 ); // (note the 1/2.0 !)
           if ( ABS(tmplat) != 90.0 ) {
              dlon3 = dt*k3u/r/RCONV/COS(tmplat*RCONV);
           } else {
              dlon3 = 0.0;
           }
           dz3 = dt*kws[i];
           if ( debug >= 100 && i == 0 ) {
              std::cerr << "     IntegRK4a-vector: dlon3, dlat3 = " << dlon3 << ", " << dlat3 << std::endl;
           }
           tmplon = lons[ii];
           tmplat = lats[ii];
           nav->deltapos( &tmplon, &tmplat, dlon3, dlat3 );
  
           tmpz =  zs[ii] + dz3;
        } else {
           dlon3 = RNAN(nanstr);
           dlat3 = RNAN(nanstr);
           dz3   = RNAN(nanstr);
           tmplon = lons[ii];
           tmplat = lats[ii];
           tmpz = zs[ii];
        }
        
        dlons3[i] = dlon3;
        dlats3[i] = dlat3;
        dzs3[i] = dz3;
        tmplons[i] = tmplon;
        tmplats[i] = tmplat;
        tmpzs[i] =  tmpz;
    } 


    //std::cerr << "IntegRK4-vector: time3=" << t + dt0 << std::endl;
    metsrc->get_uvw(t + dt0, nuse, tmplons, tmplats, tmpzs,  kus, kvs, kws );
    if ( debug >= 100 ) {
       dyt = metsrc->time2Cal(t + dt0, 3);
       std::cerr << "     IntegRK4a-vector @ (" << t << "/" << dyt << ", "  << tmplons[0] << "," << tmplats[0] 
                 << ", " << tmpzs[0] << ", " << t+dt0 << "): u4=" << kus[0] << ", v4=" << kvs[0] << std::endl;
    }

    for ( i=0; i<nuse; i++ ) {    
    
        ii = iused[i];
    
        if ( FINITE(kus[i]) && FINITE(kvs[i]) && FINITE(kws[i]) 
          && FINITE(dlons1[i]) && FINITE(dlats1[i]) && FINITE(dzs1[i])
          && FINITE(dlons2[i]) && FINITE(dlats2[i]) && FINITE(dzs2[i])
          && FINITE(dlons3[i]) && FINITE(dlats3[i]) && FINITE(dzs3[i]) ) {
    
           k4u = kus[i]/1000.0;
           k4v = kvs[i]/1000.0;
           // "rotate" the winds if we are too near one of the poles
           if ( confml == 1 ) {
             if ( lats[ii] >= NEARPOLE || lats[ii] <= -NEARPOLE || tmplats[i] >= NEARPOLE || tmplats[i] <= -NEARPOLE ) 
             {
              cdlon = COS( (tmplons[i] - lons[ii])*RCONV  );
              sdlon = SIN( (tmplons[i] - lons[ii])*RCONV  );
              up =  k4u*cdlon + k4v*sdlon; 
              vp = -k4u*sdlon + k4v*cdlon;
              k4u = up;
              k4v = vp; 
             }
           }

           // final deltas
           dlat4 = dt*k4v/r/RCONV;
           tmplon = lons[ii];
           tmplat = lats[ii];
           nav->deltapos( &tmplon, &tmplat, 0.0, dlat4/2.0 );
           if ( ABS(tmplat) != 90.0 ) {
              dlon4 = dt*k4u/r/RCONV/COS(tmplat*RCONV);
           } else {
              dlon4 = 0.0;
           }
           dz4 = dt*kws[i];
           if ( debug >= 100 && i == 0 ) {
              std::cerr << "     IntegRK4a: dlon4, dlat4 = " << dlon4 << ", " << dlat4 << std::endl;
           }
        
           dlon1 = dlons1[i];
           dlat1 = dlats1[i];
           dz1 = dzs1[i];
           dlon2 = dlons2[i];
           dlat2 = dlats2[i];
           dz2 = dzs2[i];
           dlon3 = dlons3[i];
           dlat3 = dlats3[i];
           dz3 = dzs3[i];
    
           // apply the Runga-Kutte method to get the
           // changes in lon,lat, and z
           dx = ( dlon1 + 2.0*dlon2 + 2.0*dlon3 + dlon4 )/6.0;
           dy = ( dlat1 + 2.0*dlat2 + 2.0*dlat3 + dlat4 )/6.0;
           dz = (   dz1 + 2.0*dz2   + 2.0*dz3   + dz4)/6.0;
    
           // update the longitude and latitude
           nav->deltapos( &(lons[ii]), &(lats[ii]), dx, dy );
           if ( debug >= 100 ) {
              std::cerr << "     IntegRK4a: dx, dy, lat, lon = " << dx << ", " << dy << ", " << lat << ", " << lon << std::endl;
           }
           // update the vertical coordinate
           zs[ii] += dz;
    
        } else {
        
           // disable tracing this parcel 
           flags[ii] = 1;
        
        }

    }
    
    delete tmpzs;
    delete tmplats;
    delete tmplons;
    delete kws;
    delete kvs;
    delete kus;
    delete dzs3;
    delete dlats3;
    delete dlons3;
    delete dzs2;
    delete dlats2;
    delete dlons2;
    delete dzs1;
    delete dlats1;
    delete dlons1;


    // advance the time
    t += dt0;
    //std::cerr << "IntegRK4 END: time=" << t << ", dt0=" << dt0 << std::endl;


}
