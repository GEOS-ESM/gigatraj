
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

void PlanetSphereNav :: deltaxy( real *longitude, real *latitude, real deltax, real deltay )
{
   real dlat, dlon;
   real midlat, midlon;
   
   // First, we convert x and y to dlon and dlat
   dlat = deltay / r /RCONV;
   if ( *latitude != 90.0 and *latitude != -90.0 ) 
   { 
      dlon = deltax;
   } else {
      dlon = 0.0;
   }   

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

      checkpos(*longitude, *latitude);

      *longitude = PlanetSphereNav::wrap( *longitude );

   } catch ( badlocation badloc ) {
       throw badincrement();
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
  
void PlanetSphereNav :: displace( const real clon, const real clat, const real d, const real bearing, real &lon, real &lat )
{
    
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

}
