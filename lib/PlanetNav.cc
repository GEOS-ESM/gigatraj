
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

#include "config.h"

#include "gigatraj/PlanetNav.hh"

using namespace gigatraj;


/*! 
The constructor initializes planet-related constants:

If WRAP_0 is set, then the longitudes will range from 0 to 360.
If WRAP_180 is set, then the longitudes will range from -180 to 180.
If neither is set, then WRAP_180 is assumed.

*/
PlanetNav :: PlanetNav()
{

#ifdef WRAP_0
   wraplon = 0.0;
#endif
#ifdef WRAP_180
   wraplon = -180.0;
#endif

   confml = 1;
   
   quality = 0;

}


PlanetNav :: ~PlanetNav()
{

}


void PlanetNav :: wrappingLongitude( real limit )
{
     wraplon = limit;
}

real PlanetNav :: wrappingLongitude() const
{
     return wraplon;
}


real PlanetNav :: wrap( real longitude, real wraplimit ) 
{

   if ( wraplimit < -900 ) {
      wraplimit = wraplon;
   }
   
   wrap( &longitude, 1, wraplimit );
   
   return longitude;
}


void PlanetNav :: wrap( real* longitudes, int n, real wraplimit )
{
     if ( wraplimit < -900 ) {
        wraplimit = wraplon;
     }
     if ( longitudes != NULLPTR && n > 0 ) {
        for (int i=0; i<n; i++ ) {
          if ( FINITE(longitudes[i]) ) {
             while ( longitudes[i] < wraplimit ) {
                 longitudes[i] += fullcircle;
             }
             while ( longitudes[i] >= (wraplimit + fullcircle) ) {   
                 longitudes[i] -= fullcircle;
             }    
          }
        }
     }
}


void PlanetNav :: checkpos( real longitude, real latitude )
{
    if ( ! FINITE(latitude) || ! FINITE(longitude) ) {
       throw badlocation();   
    }

    if ( latitude < -90.0 || latitude > 90.0 ) {
       throw badlocation();   
    }
    
}


void PlanetNav :: deltapos( real *longitude, real *latitude, real deltalon, real deltalat, real factor )
{

   PlanetNav :: deltapos( 1, longitude, latitude, &deltalon, &deltalat, factor );
   
}

void PlanetNav :: deltapos( int n, real *longitude, real *latitude, const real *deltalon, const real *deltalat, real factor)
{   
   for ( int i=0; i<n; i++ ) { 

       try {
   
          if ( FINITE(deltalat[i]) && FINITE(deltalon[i]) ) {
             latitude[i] += deltalat[i]*factor;
             if ( latitude[i] > 90.0 ) {
                // adjust for displacement over the North pole
                latitude[i] = 90.0 - (latitude[i] - 90.0);
                longitude[i] += 180.0;   
             } else {
                if ( latitude[i] < -90.0 ) {
                // adjust for displacement over the South pole
                   latitude[i] = -90.0 - (latitude[i] + 90.0);
                   longitude[i] += 180.0;   
                }
             }
             checkpos(longitude[i], latitude[i]);

             longitude[i] = PlanetNav::wrap( longitude[i] + deltalon[i]*factor );
          }

       } catch ( badlocation badloc ) {
           throw badincrement();
       }
   }

}
  
void PlanetNav :: conformal( int mode )
{
    confml = mode;
} 

int PlanetNav :: conformal() const
{
    return confml;
}

  
void PlanetNav :: approximate( int value )
{
    quality = value;
} 

int PlanetNav :: approximate() const
{
    return quality;
}

