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

      /// sets the longitude at which longitudes are to be wrapped
      /*! This method sets the longitude wrapping limit.
      
          Because the earth is round and winds blow roughly west to east,
          parcel trajectories will often follow some generally circumpolar path,
          around and around. It is convenient to restrict the value of the 
          parcel longitudes to some preferred range by adding or subtracting 
          multiples of 360 degrees until the longitude fits within the range.
          
          however, there are two main schools of thought regarding what
          is the proper range: 0 to 360, and -180 to 180. Or more precisely,
          [0, 360) and [-180, 180).  With the [0,360) range, for example, a longitude 
          of 359.999 would remani unchanged, but a value of 360.000 would be remapped to 0.000.
          This remapping is called "wrapping".
          
          This method sets the limit at and beyond which longitudinal values will be
          wrapped.  Once set, the range of allowed longitudes becomes [value, value+360).
          Note that any value is allowed here. The most common and widely-accepted values,
          however, are the two mentioned above. The default value is set at library compile time,
          depending on whether the DO_WRAP0 or the DO_WRAP180 macros are defined, so that 
          an institutional preference can be set site-wide.
          
          \param limit the value of the longitudinal wrapping limit
          
      */
      void wrappingLongitude( real limit );
      
      /// returns the current longitude at which longitudes are wrapped.
      /*! This method returns the current value of the longitude wrapping limit.
          The wrapping limit marks the interval [value, value+360) into which all longitudes
          may be wrapped with the wrap() method.
          
          \return the value of the longitude wrapping limit
      
      */
      real wrappingLongitude() const;

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
          \param factor an optional multiplicatove factor to be applied to deltax and deltay
      */
      void deltapos( real *longitude, real *latitude, real deltalon, real deltalat, real factor=1.0);

      /// applies arrays of delta-longitudes and -latitudes to an array of locations
      /*! This function adds a delta longitude and delta latitude from arrays
          to a longitude, latitude
          position from arrays.  This includes making the necessary corrections if
          the displacement take a locztion over one of the poles.
          
          \param n the number of points
          \param longitude a pointer to an array of the position longitudes (overwritten by the output)
          \param latitude a pointer to an array of the position latitudes (overwritten by the output)
          \param deltalon a pointer to an array of the changes in longitude
          \param deltalat a pointer to an array of the changes in latitude
          \param factor an optional multiplicatove factor to be applied to deltax and deltay
      */
      void deltapos( int n, real *longitude, real *latitude, const real *deltalon, const real *deltalat, real factor=1.0 );

      /// applies longitudinal and latitudinal distance displacements to a location
      /*! This function adds a longitudinal distance and a latitudinal distance to a longitude, latitude
          position.
          
          \param longitude the input and output longitude
          \param latitude the input and output latitude
          \param deltax the change in longitudinal position, in km
          \param deltay the change in meridional position, in km
      */
      virtual void deltaxy( real *longitude, real *latitude, real deltax, real deltay) = 0;

      /// applies longitudinal and latitudinal distance displacements to multiple locations
      /*! This function adds an array of longitudinal distances and a latitudinal distances to a 
          set of longitude and latitude
          position. The new positions overwrite the old.
          
          \param n the number of positions to change
          \param longitude a pointer to an array of the longitudes (input and output)
          \param latitude a pointer to an array of latitudes (input and output)
          \param deltax a pointer to an array of the change in longitudinal position, in km
          \param deltay a pointer to an array of the change in meridional position, in km
          \param factor an optional multiplicatove factor to be applied to deltax and deltay
      */
      virtual void deltaxy( int n, real *longitude, real *latitude, const real *deltax, const real *deltay, real factor=0.0) = 0;

      /// calculates a great-circle distance between two locations.
      /*! This function calculates the great-circle distance between two locations.
      
         \return the distance between the two locations, in kilometers(km)
       
         \param lon1 longitude of the first point
         \param lat1 longitude of the first point
         \param lon2 longitude of the second point
         \param lat2 longitude of the second point
      */
      virtual real distance( real lon1, real lat1, real lon2, real lat2) = 0;   

      /// calculates great-circle distances between two sets of locations.
      /*! This function calculates the great-circle distances between two sets of locations.
      
         \param n the number of locations 
         \param lon1 a pointer to the longitudes of the first set of points
         \param lat1 a pointer to the latitudes of the first set of points
         \param lon2 a pointer to the longitudes of the second set of points
         \param lat2 a pointer to the longitudes of the second set of points
         \param d a pointer to the the output distances
      */
      virtual void distance( int n, const real *lon1, const real *lat1, const real *lon2, const real *lat2, real *d) = 0;   
       
      /// calculates a great-circle bearing between two locations.
      /*! This function calculates the great-circle bearing between two locations.
      
         \return the bearing between the two locations, in degrees clockwise from north
       
         \param lon1 longitude of the first point
         \param lat1 longitude of the first point
         \param lon2 longitude of the second point
         \param lat2 longitude of the second point
      */
      virtual real bearing( real lon1, real lat1, real lon2, real lat2) = 0;   

      /// calculates a position a given distance and bearing from a given position
      /*! This function calculates the longitude and latitude of a position
          a given distance and bearing from a starting position.
          
          \param clon the initial longitude
          \param clat the initial latitude
          \param d the distance away from the initial position
          \param bearing the bearing (angles clockwise from north) to the new position
          \param lon (output) the new longitude
          \param lat (output) the new latitude
      */
      virtual void displace( const real clon, const real clat, const real d, const real bearing, real &lon, real &lat ) = 0;    
       
      /// calculates positions a given distance and bearing from an array of given positions
      /*! This function calculates the longitude and latitude of an array of positions
          a given set of distances and bearings from an array of starting positions.
          
          \param n the number of locations to displace
          \param clon a pointer to an array of the initial longitudes
          \param clat a pointer to an array of the initial latitudes
          \param d a pointer to an array of the distances away from the initial positions
          \param bearing a pointer to an array of the bearings (angles clockwise from north) to the new positions
          \param lon a pointer to an array of output new longitudes
          \param lat a pointer to an array of output new latitudes
      */
      virtual void displace( int n, const real *clon, const real *clat, const real *d, const real *bearing, real *lon, real *lat ) = 0;    
       

    /// determines how vectors are interpolated near the poles of the sphere
    /*!
        This function sets how horizontal wind vectors behave near the poles.
        Components of a vector field are not scalar fields. The same wind vector at
        different locations may have quite different compoents.
        
        Consider for example a wind vector at longitude=0, latitude=89.999, which we will
        denote as (0,89.999).  Suppose the wind vector there is purely northward with
        a windspeed of W. Expresssed as components (u-component, v-component), this vector
        is [0,W].  But just a short distance away, on the other side of the north pole,
        at (180,89.999), the same wind vector points south: [0, -W]. Likewise, the same vector
        at (90,89.999) is [W,0], and at (-90,89.999) it is [-W,0]. Now try to interpolate
        this wind field to the location (45, 89.9999). Simply interpolating the components 
        independently of each other will yield a wind vector that is close to [0,0], which is dead wrong.
        Before performing a weighted average of several vectors for the interpolation,
        we first need to translate those vectors to a single location. 
        
        The vRelocatate() methods perform this translation, but it needs to know
        which of several possible methods it should use. The conformal() method
        sets this.
        
        Away from the poles, or with small-magnitude, closely-spaced vectors near the poles, 
        you can get away with omitting the translation. This is mode 0, useful 
        for improving efficiency when it does not harm the accuracy too much.
        
        The next easiest method to apply is a conformal rotation about
        the difference in longitudes, applied to vectors with latitudes gearter than 88 N or 88 S.
        This gives must better accuracy in situations where mode 0 does not suffice,
        but it comes with the price of having to compute several trigonometric functions.
        
        
        \param mode sets the kind of adjustment to be applied.
                    A value of 0 means that no adjustment will be applied.
                    A value of 1 means that a polar conformal rotaton is applied.

    */
    void conformal( int mode ); 

    /// returns how vectors are interpolated near the poles of the sphere
    /*!
        This function returns how horizontal wind vectors behave near the poles.
        Components of a vector field are not scalar fields.
        When the zonal and meridional winds are
        interpolated to a parcel position as part of the time integration,
        they must be adjusted to avoid introducing anomalies into
        the trajectories.
        
        This method returns the method that is being used to accomplish that.
        
        \return mode the kind of adjustment being applied.
                    A value of 0 means that no adjustment will be applied.
                    A value of 1 means that a polar stereographic rotaton is applied near the poles

    */
    int conformal(); 

      /// relocates a vector to a new location
      /*! Vectors on a quasi-spherical planet have different orientations in the planetary coordinate
          system, depending on where they are located. Consequently, to 
          two vectors can be added together only if they are colocated. This effect is
          very small at the equator, but it can be quite large near the poles.
          
          To fix this problem, the vectors must be relocated to the point at which they are
          applied. The spatial orientation of a relocated vector will not change, but its components
          will be adjusted to maitain the proper orientation at the relocation point.
          
          The vRelocate method accomplishes such a relocation, using a simple conformal transformation
          that applies only near the poles.
         
         \param newlon the longitude of the relocation point
         \param newlat the latitude of the relocation point
         \param lon0 the longitude of the input vector
         \param lat0 the latitude of the input vector
         \param u a pointer to the east-west vector component, which will be replaced by the relocated value
         \param v a pointer to the north-south vector component, which will be replaced by the relocated value
      
      */
      virtual void vRelocate( real newlon, real newlat, real lon0, real lat0, real *u, real *v ) = 0; 
     
      /// relocates an array of vectors to an array of new locations
      /*! Vectors on a sphere have different orientations in the spherical coordinate
          system, depending on where they are located. Consequently, to 
          two vectors can be added together only if they are colocated. This effect is
          very small at the equator, but it can be quite large near the poles.
          
          As an example, consider a wind vector of magnitude U that crosses the pole, heading from longitude 0
          to longitude 180.  At longitude 0 and latitude 89.9999, which we denote as (0, 89.9999),
          the wind components are (0,U): the wind is blowing northward towards the pole.
          On the other side of the pole at (180,89.9999), however, the wind is blowing southward
          and has components (0, -U). At (90, 89.9999), the wind is blowing towards the east with
          components (U,0). And at (-90,89.999), the wind is blowing westard with components (-U,0).
          If we try to estimate the wind components at, say, (45,89.999999) by bilinear interpolation
          of the components, we obtain wind components very nearly (0,0).
          
          To fix this problem, the vectors must be relocated to the point at which they are
          applied. The spatial orientation of a relocated vector will not change, but its components
          will be adjusted to maitain the proper orientation at the relocation point.
          
          The vRelocate method accomplishes such a relocation, using a simple conformal transformation
          that applies only near the poles.
         
         \param n the number of vectors to transform
         \param newlon a pointer to and array of the longitudes of the relocation points
         \param newlat a pointer to and array of the latitudes of the relocation points
         \param lon0 a pointer to and array of the longitudes of the input vectors
         \param lat0 a pointer to and array of the latitudes of the input vectors
         \param u a pointer to and array of the east-west vector components, which will be replaced by the relocated values
         \param v a pointer to and array of the north-south vector components, which will be replaced by the relocated values
      
      */
      virtual void vRelocate( int n, const real *newlon, const real *newlat, const real *lon0, const real *lat0, real *u, real *v ) = 0; 
          
      /*! the number of degrees of longitude around a latitude circle
      */
      static constexpr real fullcircle = 360.0;

      /// the type of object this is
      static const string id;

   protected:
   
      /*! confml
      
      A flag that determines whether vectors are adjusted during interpolations
      and time integrations to account for the curvature of the planet.
      We call this "conformal (angle-preserving) adjustment"
      This is an integer rather than a boolean, to allow for potentially
      several different kinds of conformal adjustment.
      
      Setting this to anything other than zero will likely add a number of trigonometric calculations
      to the trajectory tracing. This will decrease efficiency but add accuracy near
      the polar regions.
      */
      int confml;

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
