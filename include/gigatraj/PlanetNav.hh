#ifndef GIGATRAJ_PLANETNAV_H
#define GIGATRAJ_PLANETNAV_H

#include "gigatraj/gigatraj.hh"

#include <string>

#include <math.h>


namespace gigatraj {

/*!
   \defgroup Navigation  Planetary Navigation Aids

The Planetary navigation classes provide a means of
handling longitudes and latitudes over the surface of a
planet.  This includes calculating displacements
between locations, as well as various error-checking
functions.

These classes open up the possibility of creating trajectory
models that work with other planets, either modeled or real.
Simply implement a subclass of the PlanetNav class.
To use a specific PlanetNav object, create a ne winstance
of it and load it into any  Parcel object with the Parcel's 
\b setNav() method.  Of course, you will also need to 
load a meteorological data source that is applicable to that planet,
using the Parcel's \b setMet() method.


*/


/*!

\ingroup Navigation

\brief an abstract class for navigating on a planet

The PlanetNav class is an abstract class for dealing with 
navigating on a planet, dealing with 
longitudes, latitudes, distances, and so on.


*/   

class PlanetNav {

   public:

      /// An exception for improper locations
      class badlocation {};

      /// An exception for improper changes in location
      class badincrement {};

      /// The base constructor.
      PlanetNav();
   
      /// The destructor
      ~PlanetNav(); 

      /// wraps longitude values
      /*! This function returns a longitude value that has been wrapped by adding 
          and subtracting 360 degrees until it falls within the
          standard longitude range.   For example, if the 
          range is [0, 360), then a value of -155 would be 
          wrapped to 205 ( -155 + 360).

         \return the wrapped longitude value
         \param longitude the input longitude whose value to be be wrapped
      */
      real wrap( real longitude );
   
      /// verifies a longitude/latitude pair
      /*! This function tests a given latitude and longitude for legality.
          Throws an exception is the latitude is out of range or
          if either the longitude or latitude is non-finite.
         
         \param longitude the longitude of the position to be tested
         \param latitude the latitude of the position to be tested
      */
      void checkpos( real longitude, real latitude );

      /// applies a delta-longitude and -latitude to a location
      /*! This function adds a delta longitude and delta latitude to a longitude, latitude
          position.  This includes making the necessary corrections if
          the displacement take the locztion over one of the poles.
          
          \param longitude the input and output longitude
          \param latitude the input and output latitude
          \param deltalon the change in longitude
          \param deltalat the change in latitude
      */
      void deltapos( real *longitude, real *latitude, real deltalon, real deltalat);

      /// applies longitudinal and latitudinal distance displacements to a location
      /*! This function adds a longitudinal distance and a latitudinal distance to a longitude, latitude
          position.
          
          \param longitude the input and output longitude
          \param latitude the input and output latitude
          \param deltax the change in longitudinal position, in km
          \param deltay the change in meridional position, in km
      */
      virtual void deltaxy( real *longitude, real *latitude, real deltax, real deltay) = 0;

      /// calculates a great-circle distance between two locations.
      /*! This function calculates the great-circle distance between two locations.
      
         \return the distance between the two locations, in kilometers(km)
       
         \param lon1 longitude of the first point
         \param lat1 longitude of the first point
         \param lon2 longitude of the second point
         \param lat2 longitude of the second point
      */
      virtual real distance( real lon1, real lat1, real lon2, real lat2) = 0;   
       
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
      virtual void displace( const real clon, const real clat, const real d, const real bearing, real &lon, real &lat ) = 0;    
       
          
      /*! the number of degrees of longitude around a latitude circle
      */
      static constexpr real fullcircle = 360.0;

      /// the type of object this is
      static const string id;

   private:

      /*! wraplon
      
      The lowest allowed value of longitude.  Longitudes lower than this
      will be wrapped around by adding 360 degrees.  Longitudes greater 
      than or equal to this value plus 360 degrees will be wrapped by 
      subtracting 360 degrees.
      */   
      real wraplon;


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
