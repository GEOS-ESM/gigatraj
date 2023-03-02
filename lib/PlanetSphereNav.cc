
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

PlanetSphereNav :: PlanetSphereNav() : PlanetNav()
{
   // nothing to be done here
}


real PlanetSphereNav :: radius() 
{
    return r;
}

#ifndef POLAR_LIMIT
#define POLAR_LIMIT 65.0
#endif

void PlanetSphereNav :: deltaxy( real *longitude, real *latitude, real deltax, real deltay )
{
   real dlat, dlon;
   real midlat, midlon;
   real ds, cosBB, sinBB;
   real a, cosA, sinA;
   real colat, cosColat, sinColat;
   real sinAA, cosAA, AA;
   real sinC, cosC;
   real b, cosB, sinB;

   if ( FINITE(deltax) && FINITE(deltay) && FINITE(*longitude) && FINITE(*latitude) ) {
   
      ds = SQRT( deltax*deltax + deltay*deltay );
   
      // eliminate pathological cases
      if ( ds > 0 ) {
         // only change if the distance is nonzero
      
         if ( *latitude <= 90.0 ) {
            if ( *latitude >= -90.0 ) {
               // all nice and sanity-checked at this point
               
               // but are we doing the full-out great circle route,
               // or going with an approximation?               
               if ( ( confml == 0 ) && ( ABS(*latitude) < POLAR_LIMIT ) ) {
                  // more efficient, if less accurate
                  
                  // First, we convert x and y to dlon and dlat
                  dlat = deltay / r /RCONV;
                  dlon = deltax;

                  try {
                     midlat = *latitude + dlat/2.0;
                     if ( midlat > 90.0 ) {
                        midlat = 90.0 - (midlat - 90.0);
                     } else {
                        if ( midlat < -90.0 ) {
                           midlat = -90.0 - (midlat + 90.0);
                        }
                     }
                     
                     dlon = dlon/ ( 2.0 * PI * r * COS(midlat*RCONV) ) * 360.0;
   
                     *latitude += dlat;
                     if ( *latitude > 90.0 ) {
                        *latitude = 90.0 - (*latitude - 90.0);
                        *longitude += 180.0;   
                     } else {
                        if ( *latitude < -90.0 ) {
                           *latitude = -90.0 - (*latitude + 90.0);
                           *longitude += 180.0;   
                        }
                     }
                     *longitude += dlon ;

                  } catch ( badlocation badloc ) {
                      throw badincrement();
                  }
               
               
               } else {
                  
                  // traces the path over a great-circle route.
                  
                  // but we need to deal with the special case of deltay == 0.0,
                  // which generally means that the intention is to 
                  // move along a latitude circle, not along a greta-circle path
                  
                  if ( deltay != 0 ) {
               
                     // the general case
               
                     a = ds/r;
                     sinA = SIN(a);
                     cosA = COS(a);
                
                     colat = ( 90.0 - *latitude)*RCONV;
                     sinC = SIN(colat);
                     cosC = COS(colat);
                
                     sinBB = deltax/ds;
                     cosBB = deltay/ds;
                
                     cosB = cosC*cosA + sinC*sinA*cosBB;
                     b = ACOS( cosB );
                     sinB = SIN( b );
                
                     sinAA = sinA/sinB*sinBB;
                     cosAA = ( cosA - cosB*cosC )/sinB/sinC;
                
                     *latitude = 90.0 - b/RCONV;
                     *longitude = *longitude + ATAN2( sinAA, cosAA )/RCONV;
                
                  } else {
                     
                     // No change in latitude
                                        
                     dlon = deltax/ ( r * COS(*latitude*RCONV) ) /RCONV;
                     
                     *longitude = *longitude + dlon;
                     
                
                  }
               }
            
            } else {
                // input latitude is at the south pole
               
                *latitude = *latitude + ds/r/RCONV;
                
                // Note: bearing is measured clockwise from the top, so 
                // the angle here is indeed ATAN( dx, dy )
                *longitude = *longitude + ATAN2( deltax, deltay );
                
            }
      
      
         } else {
            // input latitude is at the north pole
            
            *latitude = *latitude - ds/r/RCONV;
            
            // Note: bearing is measured clockwise from the top, so 
            // the angle here is indeed ATAN( dx, dy )
            *longitude = *longitude - ATAN2( deltax, deltay ) + 180.0;
         } 
         
         checkpos(*longitude, *latitude);

         *longitude = PlanetSphereNav::wrap( *longitude );

      }
   }


}

void PlanetSphereNav :: deltaxy( int n, real *longitudes, real *latitudes, const real *deltax, const real *deltay, real factor) 
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
                   
                   // but are we doing the full-out great circle route,
                   // or going with an approximation?               
                   if ( ( confml == 0 ) && ( ABS(latitude) < POLAR_LIMIT ) ) {
                      // more efficient, if less accurate
               
                      // First, we convert x and y to dlon and dlat
                      dlat = deltay[i]*factor / r /RCONV;
                      dlon = deltax[i]*factor;

                      try {
                         midlat = latitude + dlat/2.0;
                         if ( midlat > 90.0 ) {
                            midlat = 90.0 - (midlat - 90.0);
                         } else {
                            if ( midlat < -90.0 ) {
                               midlat = -90.0 - (midlat + 90.0);
                            }
                         }
                         
                         dlon = dlon/ ( 2.0 * PI * r * COS(midlat*RCONV) ) * 360.0;
   
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


                      } catch ( badlocation badloc ) {
                          throw badincrement();
                      }
               

                   } else {   
                      
                      // traces the path over a great-circle route.
                      
                      // but we need to deal with the special case of deltay == 0.0,
                      // which generally means that the intention is to 
                      // move along a latitude circle, not along a greta-circle path
                      
                      if ( deltay[i] != 0 ) {
               
                         a = ds/r;
                          // the general case
                         sinA = SIN(a);
                         cosA = COS(a);
                        
                         colat = ( 90.0 - latitude)*RCONV;
                         sinC = SIN(colat);
                         cosC = COS(colat);
                        
                         sinBB = deltax[i]/ds;
                         cosBB = deltay[i]/ds;
                        
                         cosB = cosC*cosA + sinC*sinA*cosBB;
                         b = ACOS( cosB );
                         sinB = SIN( b );
                        
                         sinAA = sinA/sinB*sinBB;
                         cosAA = ( cosA - cosB*cosC )/sinB/sinC;
                        
                         latitude  = 90.0 - b/RCONV;
                         longitude = longitude + ATAN2( sinAA, cosAA )/RCONV;
                      
                      } else {
                         
                         // No change in latitude
                                            
                         dlon = deltax[i]/ ( r * COS(latitude*RCONV) ) /RCONV;
                         
                         longitude = longitude + dlon;
                         
                
                      }
                    
                   }
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
    real dlon;
    real clat1, slat1, clat2, slat2, cdlon, sdlon;
    real a, b, darc;

    /*
         We use the Vincenty formula, as referenced in
         http://en.wikipedia.org/wiki/Great-circle_distance
    */
    
    cdlon = COS( (lon2-lon1)*RCONV );
    sdlon = SIN( (lon2-lon1)*RCONV );
    clat1 = COS( lat1*RCONV );
    slat1 = SIN( lat1*RCONV );
    clat2 = COS( lat2*RCONV );
    slat2 = SIN( lat2*RCONV );
    
    a = SQRT( (clat2*sdlon)*(clat2*sdlon) 
            + (clat1*slat2-slat1*clat2*cdlon)*(clat1*slat2-slat1*clat2*cdlon)  );
    b = slat1*slat2 + clat1*clat2*cdlon;

    darc = ATAN2( a, b );

    return darc * r;

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

#ifndef NEARPOLE
#define NEARPOLE 88.0
#endif
 
void PlanetSphereNav :: vRelocate( real newlon, real newlat, real lon0, real lat0, real *u, real *v )
{
     real tmp_u;
     real tmp_v;
     real dlon;
     
     if ( confml == 1 ) { 
        // we apply the transform only if we are within 15 degrees latitude of the pole
        if ( lat0 >= NEARPOLE || lat0 <= -NEARPOLE || newlat >= NEARPOLE || newlat <= -NEARPOLE ) {
     
           dlon = (lon0 - newlon)*RCONV;
           
           if ( lat0 < 0.0 ) {
              dlon = - dlon;
           }
     
           tmp_u = (*u)*COS(dlon) - (*v)*SIN(dlon);
           tmp_v = (*u)*SIN(dlon) + (*v)*COS(dlon);
     
           *u = tmp_u;
           *v = tmp_v;
     
        }
     }

}


void PlanetSphereNav :: vRelocate( int n, const real *newlon, const real *newlat, const real *lon0, const real *lat0, real *u, real *v )
{
     real tmp_u;
     real tmp_v;
     real dlon;

     if ( confml == 1 ) { 
        for ( int i=0; i<n; i++ ) {
           // we apply the transform only if we are within 15 degrees latitude of the pole
           if ( lat0[i] >= NEARPOLE || lat0[i] <= -NEARPOLE || newlat[i] >= NEARPOLE || newlat[i] <= -NEARPOLE ) {
     
              dlon = (lon0[i] - newlon[i])*RCONV;
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
