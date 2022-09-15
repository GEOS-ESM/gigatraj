#ifndef GIGATRAJ_PLANETSPHERENAV_H
#define GIGATRAJ_PLANETSPHERENAV_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/PlanetNav.hh"

#include <math.h>


namespace gigatraj {

/*!

\ingroup Navigation

\brief a base class for doing navigation on a spherical planet

\details

The PlanetSphereNav class is a base class for dealing with navigating on a planet:
longitudes, latitudes, distances, and so on.
This class represents a planet as a perfect sphere
(i.e, not an ellipsoid).



*/   

class PlanetSphereNav: public PlanetNav {

   public:

      /// the base constructor
      PlanetSphereNav();
      
      /// destructor
      ~PlanetSphereNav() {};
      
      /// returns the radius of the planet
      /*! This function returns the radius of the planet that the object represents.
      
          \return the planetary radius, in kilometers (km)
      */
      real radius();

  
      /// applies longitudinal and latitudinal distance displacements to a location
      /*! This function adds a longitudinal distance and a latitudinal distance to a longitude, latitude
          position.
          
          \param longitude the input and output longitude
          \param latitude the input and output latitude
          \param deltax the change in longitudinal position, in km
          \param deltay the change in meridional position, in km
      */
      void deltaxy( real *longitude, real *latitude, real deltax, real deltay);


      /// calculates a great-circle distance between two locations.
      /*! This function calculates the great-circle distance between two locations.
      
         \return the distance between the two locations, in kilometers(km)
       
         \param lon1 longitude of the first point
         \param lat1 longitude of the first point
         \param lon2 longitude of the second point
         \param lat2 longitude of the second point
      */
      real distance( real lon1, real lat1, real lon2, real lat2);   
       
      /// calculates a position a given distance and bearing from a given position
      /*! This function calculates the longitude and latitude of a position
          a given distance andbearing from a starting position.
          
          \param clon the initial longitude
          \param clat the initial latitude
          \param d the distance away from the initial position
          \param bearing the bearing (angles clockwise from north) to the new position
          \param lon (output) the new longitude
          \param lat (output) the new latitude
      */
      void displace( const real clon, const real clat, const real d, const real bearing, real &lon, real &lat );    
       
      /// the type of object this is
      static const string id;

   protected:

      /// The radius of the spherical planet.
      real r;



};
}

#endif



/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/
