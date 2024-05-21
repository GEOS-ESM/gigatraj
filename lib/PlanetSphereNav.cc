
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/PlanetSphereNav.hh"

using namespace gigatraj;

#ifndef POLAR_LIMIT
//#define POLAR_LIMIT 65.0
#define POLAR_LIMIT 75.0
#endif

PlanetSphereNav :: PlanetSphereNav() : PlanetNav()
{
   polar_lat = POLAR_LIMIT;
}


real PlanetSphereNav :: radius() 
{
    return r;
}

void PlanetSphereNav :: polar_limit( real limit )
{
     polar_lat = limit;
}

real PlanetSphereNav :: polar_limit()
{
     return polar_lat;
}

void PlanetSphereNav :: deltaxy( real *longitude, real *latitude, real deltax, real deltay, real factor, int approx )
{

   PlanetSphereNav::deltaxy( 1, longitude, latitude, &deltax, &deltay, factor, approx);

}

void PlanetSphereNav :: deltaxy( int n, real *longitudes, real *latitudes, const real *deltax, const real *deltay, real factor, int approx) 
{
   if ( approx == -999 ) {
      approx = quality;
   }
   
   if ( approx == 0 ) {
      deltaxy_approx( n, longitudes, latitudes, deltax, deltay, factor);
   } else if ( approx < 0 ) {
      deltaxy_exact( n, longitudes, latitudes, deltax, deltay, factor);   
   } else {
      deltaxy_crude( n, longitudes, latitudes, deltax, deltay, factor);   
   }

}

void PlanetSphereNav :: deltaxy_exact( int n, real *longitudes, real *latitudes, const real *deltax, const real *deltay, real factor) 
{
   real ds, dx, dy;
   real longitude, latitude;
   real cosBB, sinBB;
   real a, cosA, sinA;
   real c, sinC, cosC;
   real b, cosB, sinB;   
   real dlon;
   real sinAA, cosAA, AA;
   
   for ( int i=0; i<n; i++ ) {
   
       if ( FINITE(deltax[i]) && FINITE(deltay[i]) && FINITE(longitudes[i]) && FINITE(latitudes[i]) ) {
       
          longitude = longitudes[i];
          latitude = latitudes[i];
       
          dx = deltax[i];
          dy = deltay[i];
          ds = SQRT( dx*dx + dy*dy );
          
          /*
               We consider an oblique spehrical triangle with one vertex at the North pole,
               one vertex at the starting point,and the thrid virtex at the ending point.
               "c" is the colatitude of the starting point.
               "b" is the colatitude of the ending point
               "a" is the (angular) distance travelled from the starting to the ending point
               "A", opposite "a", is the change in longitude.
               "B", opposite "b", is the bearing of the path form start to end
               "C" is the angle opposite side "c"
          */
       
          // eliminate pathological cases
          if ( ds > 0 ) {
             // only change if the distance is nonzero
          
             if ( latitude <= 90.0 ) {
                if ( latitude >= -90.0 ) {
                   // all nice and sanity-checked at this point
                   
                   sinBB = dx/ds;
                   cosBB = dy/ds;
                   
                   c = (90.0 - latitude)*RCONV;
                   cosC = COS(c);
                   sinC = SIN(c);
                   
                   a = ds/r*factor;
                   cosA = COS(a);
                   sinA = SIN(a);
                                      
                   cosB = cosC*cosA + sinC*sinA*cosBB;
                   b = ACOS( cosB );
                   sinB = SIN( b );
                   // avoid blowing up at the poles
                   if ( sinB != 0.0 ) {
                   
                      sinAA = sinA/sinB*sinBB;
                      cosAA = ( cosA - cosB*cosC )/sinB/sinC;
                      
                      dlon =  ATAN2( sinAA, cosAA )/RCONV;

                   } else {

                      if ( b == 0.0 ) {
                         // north pole
                         b = 1.0e-9;
                      } else {
                         // south pole
                         b = b - 1.0e-9;
                      }
                      sinB = SIN( b );
                      cosB = COS( b );
                   
                      sinAA = sinA/sinB*sinBB;
                      cosAA = ( cosA - cosB*cosC )/sinB/sinC;

                      // put it on just the other side of the pole,
                      // so that we don;t keep hitting the pole repeatedly
                      // when tracing out a path
                      dlon =  ATAN2( sinAA, cosAA )/RCONV + 180.0;
                   }
                   
                   latitude  = 90.0 - b/RCONV;
                   longitude = longitude + dlon;

                } else {
                    // input latitude is at the south pole
                   
                    latitude = latitude + ds/r/RCONV;
                    
                    // Note: bearing is measured clockwise from the top, so 
                    // the angle here is indeed ATAN( dx, dy )
                    longitude = longitude + ATAN2( dx, dy )/RCONV + 180.0;
                    
                }
          
          
             } else {
                // input latitude is at the north pole
                
                latitude = latitude - ds/r/RCONV;
                
                // Note: bearing is measured clockwise from the top, so 
                // the angle here is indeed ATAN( dx, dy )
                longitude = longitude - ATAN2( dx, dy )/RCONV + 180.0;
             } 
          }

          checkpos(longitude, latitude);

          longitude = PlanetSphereNav::wrap(longitude );

          longitudes[i] = longitude;
          latitudes[i]  = latitude;

       }
   }
}

void PlanetSphereNav :: deltaxy_approx( int n, real *longitudes, real *latitudes, const real *deltax, const real *deltay, real factor) 
{
   real ds, dx, dy;
   real longitude, latitude;
   real cosBB, sinBB;
   real a, cosA, sinA, a2;
   real c, sinC, cosC, c2;
   real b, cosB, sinB, b2;   
   real dlon;
   real sinAA, cosAA, AA;
   real newLon, newLat;
   real rlat;
   real eta, zeta;
   
   for ( int i=0; i<n; i++ ) {
   
       if ( FINITE(deltax[i]) && FINITE(deltay[i]) && FINITE(longitudes[i]) && FINITE(latitudes[i]) ) {
       
          longitude = longitudes[i];
          latitude = latitudes[i];
       
          dx = deltax[i];
          dy = deltay[i];
          ds = SQRT( dx*dx + dy*dy );
          
          /*
               We consider an oblique spehrical triangle with one vertex at the North pole,
               one vertex at the starting point,and the thrid virtex at the ending point.
               "c" is the colatitude of the starting point.
               "b" is the colatitude of the ending point
               "a" is the (angular) distance travelled from the starting to the ending point
               "A", opposite "a", is the change in longitude.
               "B", opposite "b", is the bearing of the path form start to end
               "C" is the angle opposite side "c"
          */
       
          // eliminate pathological cases
          if ( ds > 0 ) {
             // only change if the distance is nonzero
          
             if ( latitude <= 90.0 ) {
                if ( latitude >= -90.0 ) {
                   // all nice and sanity-checked at this point
                   
                   rlat = latitude*RCONV;
                   
                   a = ds/r*factor;
                   a2 = a*a;
                   //cosA = COS(a);
                   //sinA = SIN(a);
                   if ( a < 0.01 ) {
                      // small displacement, so change in latitude will be small
                      if ( ABS(latitude) < polar_lat ) {
                         newLat = latitude + dy/r/RCONV;
                         newLon = longitude + dx/r/COS(rlat)/RCONV;
                      } else {
                         // too close to the pole to do the simple approximmation
                         
                         sinA = a;
                         cosA = 1.0 - a2/2.0;
                         
                         sinBB = dx/ds;
                         cosBB = dy/ds;
                   
                         c = (90.0 - latitude)*RCONV;
                         c2 = c*c;
                         //cosC = COS(c);
                         //sinC = SIN(c); 
                         // got up to 180 degrees                  
                         cosC = 1.0 + c2*( - 1.0/2. + c2*( 1.0/24.0 + c2*( -1.0/720.0 + c2*( 1.0/40320.0 - 1.0/3628800.0*c2) ) ) );
                         sinC = c*( 1.0 + c2*( -1.0/6.0 + c2*( 1.0/120.0 + c2*( -1.0/5040 + c2*1.0/362880.0 )))); 
                   
                         if ( latitude >=  polar_lat ) {
                            b2 = c2 + a2 - 2.0*c*a*cosBB;
                            b = SQRT( b2 );
                            if ( b > 0.0 ) {
                               // did not reach the pole
                               sinB = b;
                               cosB = 1.0 - b2/2.0;
                               
                               sinAA = sinA/sinB*sinBB;
                               cosAA = ( b2 + c2 - a2)/2/b/c;
                               dlon =  ATAN2( sinAA, cosAA )/RCONV;
                           } else {
                               // hit the north pole exactly
                               b = 1.0e-9;
                               b2 = b*b;
                               sinB = b;
                               cosB = 1.0 - b2/2.0;

                               sinAA = sinA/sinB*sinBB;
                               cosAA = ( b2 + c2 - a2)/2/b/c;
                               dlon =  ATAN2( sinAA, cosAA )/RCONV + 180.0;
                           }     
                         } else if ( latitude <= - polar_lat ) {
                           eta = PI - c;
                           zeta = SQRT( eta*eta + a2 +2.0*eta*a*cosBB );
                           b = PI - zeta;
                           b2 = b*b;
                           sinAA = a/eta*sinBB;
                           cosAA = (eta*eta + zeta*zeta - a2)/2/eta/zeta;
                           dlon =  ATAN2( sinAA, cosAA )/RCONV;
                         }
                      
                         newLat = 90.0 - b/RCONV;
                         newLon = longitude + dlon;
                         
                      }
                   } else {
                   
                      // displacement is NOT small
                      
                      // good up to around 180 degrees
                      cosA = 1.0 + a2*( - 1.0/2. + a2*( 1.0/24.0 + a2*( -1.0/720.0 + a2*( 1.0/40320.0 - 1.0/3628800.0*a2) ) ) );
                      sinA = a*( 1.0 + a2*( -1.0/6.0 + a2*( 1.0/120.0 + a2*( -1.0/5040 + a2*1.0/362880.0 ))));
                         
                      sinBB = dx/ds;
                      cosBB = dy/ds;
                   
                      c = (90.0 - latitude)*RCONV;
                      c2 = c*c;
                      //cosC = COS(c);
                      //sinC = SIN(c); 
                      // got up to 180 degrees                  
                      cosC = 1.0 + c2*( - 1.0/2. + c2*( 1.0/24.0 + c2*( -1.0/720.0 + c2*( 1.0/40320.0 - 1.0/3628800.0*c2) ) ) );
                      sinC = c*( 1.0 + c2*( -1.0/6.0 + c2*( 1.0/120.0 + c2*( -1.0/5040 + c2*1.0/362880.0 )))); 

                      cosB = cosC*cosA + sinC*sinA*cosBB;
                      b = ACOS( cosB );
                      sinB = SIN( b );                   
                      // avoid blowing up at the poles                                       
                      if ( sinB != 0.0 ) {
                   
                         sinAA = sinA/sinB*sinBB;
                         cosAA = ( cosA - cosB*cosC )/sinB/sinC;
                         
                         dlon =  ATAN2( sinAA, cosAA )/RCONV;
                   
                      } else {
                   
                         if ( b == 0.0 ) {
                            // north pole
                            b = 1.0e-9;
                         } else {
                            // south pole
                            b = b - 1.0e-9;
                         }
                         sinB = SIN( b );
                         cosB = COS( b );
                   
                         sinAA = sinA/sinB*sinBB;
                         cosAA = ( cosA - cosB*cosC )/sinB/sinC;
                   
                         // put it on just the other side of the pole,
                         // so that we don;t keep hitting the pole repeatedly
                         // when tracing out a path
                         dlon =  ATAN2( sinAA, cosAA )/RCONV + 180.0;
                      }
                   
                      newLat  = 90.0 - b/RCONV;
                      newLon = longitude + dlon;
                   
                   }
                } else {
                    // input latitude is at the south pole
                   
                    newLat = latitude + ds/r/RCONV;
                    
                    // Note: bearing is measured clockwise from the top, so 
                    // the angle here is indeed ATAN( dx, dy )
                    newLon = longitude + ATAN2( dx, dy )/RCONV + 180.0;
                    
                }
          
          
             } else {
                // input latitude is at the north pole
                
                newLat = latitude - ds/r/RCONV;
                
                // Note: bearing is measured clockwise from the top, so 
                // the angle here is indeed ATAN( dx, dy )
                newLon = longitude - ATAN2( dx, dy )/RCONV + 180.0;
             } 
          } else {
             newLat = latitude;
             newLon = longitude;
          }

          checkpos(newLon, newLat);

          newLon = PlanetSphereNav::wrap(newLon );

          longitudes[i] = newLon;
          latitudes[i]  = newLat;

       }
   }
}

void PlanetSphereNav :: deltaxy_crude( int n, real *longitudes, real *latitudes, const real *deltax, const real *deltay, real factor) 
{
   real dlat, dlon;
   real midlat, midlon;
   real longitude, latitude;
   real ds, cosBB, sinBB;
   real a, cosA, sinA;
   real colat, cosColat, sinColat;
   real sinAA, cosAA, AA;
   real sinC, cosC;
   real b, cosB, sinB;
   
   for ( int i=0; i<n; i++ ) {
   
       if ( FINITE(deltax[i]) && FINITE(deltay[i]) && FINITE(longitudes[i]) && FINITE(latitudes[i]) ) {
       
          longitude = longitudes[i];
          latitude = latitudes[i];
       
          ds = SQRT( deltax[i]*deltax[i] + deltay[i]*deltay[i] );
       
          // eliminate pathological cases
          if ( ds > 0 ) {
             // only change if the distance is nonzero
          
             if ( latitude <= 90.0 ) {
                if ( latitude >= -90.0 ) {
                   // all nice and sanity-checked at this point
                   
                      // First, we convert x and y to dlon and dlat
                      dlat = deltay[i]*factor / r /RCONV;

                      midlat = latitude + dlat/2.0;
                      midlon = longitude;
                      if ( midlat > 90.0 ) {
                         midlat = 90.0 - (midlat - 90.0);
                         midlon += 180.0;
                      } else {
                         if ( midlat < -90.0 ) {
                            midlat = -90.0 - (midlat + 90.0);
                            midlon += 180.0;
                         }
                      }
                         
                      dlon = deltax[i]*factor/r/COS(midlat*RCONV)/RCONV; ;
   
                      latitude += dlat;
                      if ( latitude > 90.0 ) {
                         latitude = 90.0 - (latitude - 90.0);
                         longitude += 180.0;   
                      } else {
                         if ( latitude < -90.0 ) {
                            latitude = -90.0 - (latitude + 90.0);
                            longitude += 180.0;   
                         }
                      }
                      longitude += dlon ;

                } else {
                    // input latitude is at the south pole
                   
                    latitude = latitude + ds/r/RCONV;
                    
                    // Note: bearing is measured clockwise from the top, so 
                    // the angle here is indeed ATAN( dx, dy )
                    longitude = longitude + ATAN2( deltax[i], deltay[i] );
                    
                }
          
          
             } else {
                // input latitude is at the north pole
                
                latitude = latitude - ds/r/RCONV;
                
                // Note: bearing is measured clockwise from the top, so 
                // the angle here is indeed ATAN( dx, dy )
                longitude = longitude - ATAN2( deltax[i], deltay[i] ) + 180.0;
             } 
          }

          checkpos(longitude, latitude);

          longitude = PlanetSphereNav::wrap(longitude );

          longitudes[i] = longitude;
          latitudes[i]  = latitude;

       }
   }
}

real PlanetSphereNav :: distance( real lon1, real lat1, real lon2, real lat2)
{
    real result;
    
    PlanetSphereNav::distance( 1, &lon1, &lat1, &lon2, &lat2, &result);

    return result;

}

void PlanetSphereNav :: distance( int n, const real *lon1, const real *lat1, const real *lon2, const real *lat2, real *d)
{
    real dlon;
    real clat1, slat1, clat2, slat2, cdlon, sdlon;
    real a, b, darc;

    /*
         We use the Vincenty formula, as referenced in
         http://en.wikipedia.org/wiki/Great-circle_distance
    */
   
    for ( int i=0; i<n; i++ ) { 
    
        cdlon = COS( (lon2[i] - lon1[i])*RCONV );
        sdlon = SIN( (lon2[i] - lon1[i])*RCONV );
        clat1 = COS( lat1[i]*RCONV );
        slat1 = SIN( lat1[i]*RCONV );
        clat2 = COS( lat2[i]*RCONV );
        slat2 = SIN( lat2[i]*RCONV );
    
        a = SQRT( (clat2*sdlon)*(clat2*sdlon) 
                + (clat1*slat2-slat1*clat2*cdlon)*(clat1*slat2-slat1*clat2*cdlon)  );
        b = slat1*slat2 + clat1*clat2*cdlon;
    
        darc = ATAN2( a, b );
    
        d[i] = darc * r;
    }


}   

real PlanetSphereNav :: bearing( real lon1, real lat1, real lon2, real lat2)
{
    real angles;
    real slat1,clat1, slat2,clat2, slons,clons;
    real result;
    
    angles = lat1*RCONV;
    slat1 = SIN(angles);
    clat1 = COS(angles);
    angles = lat2*RCONV;
    slat2 = SIN(angles);
    clat2 = COS(angles);
    angles = (lon2 - lon1)*RCONV;
    slons = SIN(angles);
    clons = COS(angles);

    result = 0.0;

    // Is at least one point at one of the poles?
    if ( ABS(clat2) > 1e-15 ) {
       // not at a pole
       
       // is this a line between two antipoles?
       if ( clons < -1.0 || lat1 != -lat2 ) {
          // no antipoles
       
          // this is the most common case
          result = ATAN2( slons, (clat1*slat2/clat2 - slat1*clons ) );
       
       } else {
          // antipode case
          
          // Any bearing will take you to the antipod point
          // along a great-circle path
          // We'll choose this one.
          result = PI/2.0;
       }
       
    } else {
       // at a pole
       // which one?
       if ( slat2 > 0 ) {
          // north pole
          result = PI;
       } else {
          // south pole
          result = - PI;
       }   
       
    }

    return result/RCONV;

}
  
void PlanetSphereNav :: displace( const real clon, const real clat, const real d, const real bearing, real &lon, real &lat )
{
    
    if ( d > 0.0 ) {
    
       if ( clat < 90.0 ) {
       
          if ( clat > -90.0 ) {
             // wrap the input bearing
             real ang = bearing;
             while ( ang < 0.0 ) {
                ang = ang + 360.0;
             }
             while ( ang >= 360.0 ) {
                ang = ang - 360.0;
             }   
             ang = ang*RCONV;
    
             real sinrb = SIN(ang);
             real cosrb = COS(ang);
    
             real dr = d / r;
             real cosdr = COS(dr);
             real sindr = SIN(dr);
    
             real colat = ( 90.0 - clat )*RCONV;
             real coscolat = COS(colat);
             real sincolat = SIN(colat);
    
             real cosb = coscolat*cosdr + sincolat*sindr*cosrb;
             real b = ACOS(cosb);
             real sinb = SIN(b);
    
             lat = 90.0 - b/RCONV;
    
             real sina = sindr/sinb*sinrb;
             real cosa = ( cosdr - cosb*coscolat )/sinb/sincolat;
             real a = ATAN2( sina, cosa );
    
             lon = wrap( a/RCONV + clon );     
          } else {
               // south pole
               lat = clat + d/r/RCONV;
               lon = wrap( clon + bearing );
          }   
       } else {
         // north pole
         lat = clat - d/r/RCONV;
         lon = wrap( clon - bearing + 180.0 );
       }   
    
    } else {
       // zero distance
       lat = clat;
       lon = clon;
    }   
}

void PlanetSphereNav :: displace( int n, const real *clon, const real *clat, const real *d, const real *bearing, real *lon, real *lat )
{

    for ( int i=0; i<n; i++ ) {
    
        if ( d[i] > 0.0 ) {
    
           if ( clat[i] < 90.0 ) {
           
              if ( clat[i] > -90.0 ) {

                 // wrap the input bearing
                 real ang = bearing[i];
                 while ( ang < 0.0 ) {
                    ang = ang + 360.0;
                 }
                 while ( ang >= 360.0 ) {
                    ang = ang - 360.0;
                 }   
                 ang = ang*RCONV;
    
                 real sinrb = SIN(ang);
                 real cosrb = COS(ang);
    
                 real dr = d[i] / r;
                 real cosdr = COS(dr);
                 real sindr = SIN(dr);
    
                 real colat = ( 90.0 - clat[i] )*RCONV;
                 real coscolat = COS(colat);
                 real sincolat = SIN(colat);
    
                 real cosb = coscolat*cosdr + sincolat*sindr*cosrb;
                 real b = ACOS(cosb);
                 real sinb = SIN(b);
    
                 lat[i] = 90.0 - b/RCONV;
    
                 real sina = sindr/sinb*sinrb;
                 real cosa = ( cosdr - cosb*coscolat )/sinb/sincolat;
                 real a = ATAN2( sina, cosa );
    
                 lon[i] = wrap( a/RCONV + clon[i] );     
           } else {
                // south pole
                lat[i] = clat[i] + d[i]/r/RCONV;
                lon[i] = wrap( clon[i] + bearing[i] );
           }   
         } else {
           // north pole
           lat[i] = clat[i] - d[i]/r/RCONV;
           lon[i] = wrap( clon[i] - bearing[i] + 180.0 );
         }   
    
       } else {
          // zero distance
          lat[i] = clat[i];
          lon[i] = clon[i];
       }   
    }
}

void PlanetSphereNav :: vRelocate( real newlon, real newlat, real lon0, real lat0, real *u, real *v, int approx )
{
     
     vRelocate( 1, &newlon, &newlat, &lon0, &lat0, u, v, approx );
     
}


void PlanetSphereNav :: vRelocate( int n, const real *newlon, const real *newlat, const real *lon0, const real *lat0, real *u, real *v, int approx )
{
     real tmp_u;
     real tmp_v;
     real dlon;
     real dlon2;
     real cosDlon, sinDlon;

     if ( approx == -999 ) {
        approx = quality;
     }
     
     if ( confml == 1 ) { 
        if ( approx == 0 ) {
           for ( int i=0; i<n; i++ ) {
              // we apply the transform only if we are within 15 degrees latitude of the pole
              if ( lat0[i] >= polar_lat || lat0[i] <= -polar_lat || newlat[i] >= polar_lat || newlat[i] <= -polar_lat ) {
     
                 dlon = wrap(newlon[i] - lon0[i], -180.0)*RCONV;
                 if ( lat0[i] < 0.0 ) {
                    dlon = - dlon;
                 }
     
                 dlon2 = dlon*dlon;

                 if ( ABS(dlon) <= 0.001 ) {
                    cosDlon = 1.0 - dlon*dlon/2.0;
                    sinDlon = dlon;
                 } else {
                    // good to ~ 180.0 degrees
                    cosDlon = 1.0 + dlon2*( - 1./2. + dlon2*( 1./24.0 + dlon2*( -1./720.0 + dlon2*( 1./40320.0 - 1./3628800.0*dlon2) ) ) );
                    sinDlon = dlon*( 1.0 + dlon2*( -1./6. + dlon2*( 1./120.0 + dlon2*( -1./5040 + dlon2*1./362880. ))));
                 }
     
                 tmp_u = u[i]*cosDlon - v[i]*sinDlon;
                 tmp_v = u[i]*sinDlon + v[i]*cosDlon;
     
                 u[i] = tmp_u;
                 v[i] = tmp_v;
     
              }
           }
        } else if ( approx < 0 ) {
           for ( int i=0; i<n; i++ ) {
              if ( lat0[i] >= polar_lat || lat0[i] <= -polar_lat || newlat[i] >= polar_lat || newlat[i] <= -polar_lat ) {
                 dlon = (newlon[i] - lon0[i])*RCONV;
                 if ( lat0[i] < 0.0 ) {
                    dlon = - dlon;
                 }     
     
                 cosDlon = COS(dlon);
                 sinDlon = SIN(dlon);
     
                 tmp_u = u[i]*cosDlon - v[i]*sinDlon;
                 tmp_v = u[i]*sinDlon + v[i]*cosDlon;
     
                 u[i] = tmp_u;
                 v[i] = tmp_v;
              }
           }
        
        } else {
           for ( int i=0; i<n; i++ ) {
              // we apply the transform only if we are within 15 degrees latitude of the pole
              if ( lat0[i] >= polar_lat || lat0[i] <= -polar_lat || newlat[i] >= polar_lat || newlat[i] <= -polar_lat ) {
     
                 dlon = (newlon[i] - lon0[i])*RCONV;
                 if ( lat0[i] < 0.0 ) {
                    dlon = - dlon;
                 }
     
     
                 tmp_u = u[i]*COS(dlon) - v[i]*SIN(dlon);
                 tmp_v = u[i]*SIN(dlon) + v[i]*COS(dlon);
     
                 u[i] = tmp_u;
                 v[i] = tmp_v;
     
              }
           }
        
        }
     }

}
