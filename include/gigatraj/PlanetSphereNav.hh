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

      /// applies longitudinal and latitudinal distance displacements to multiple locations
      /*! This function adds an array of longitudinal distances and a latitudinal distances to a 
          set of longitude and latitude
          position. The new positions overwrite the old.
          
          \param n the number of positions to change
          \param longitudes a pointer to an array of the longitudes (input and output)
          \param latitudes a pointer to an array of latitudes (input and output)
          \param deltax a pointer to an array of the change in longitudinal position, in km
          \param deltay a pointer to an array of the change in meridional position, in km
          \param factor an optional multiplicatove factor to be applied to deltax and deltay
      */
      void deltaxy( int n, real *longitudes, real *latitudes, const real *deltax, const real *deltay, real factor=1.0 );


      /// calculates a great-circle distance between two locations.
      /*! This function calculates the great-circle distance between two locations.
      
         \return the distance between the two locations, in kilometers(km)
       
         \param lon1 longitude of the first point
         \param lat1 latitude of the first point
         \param lon2 longitude of the second point
         \param lat2 latitude of the second point
      */
      real distance( real lon1, real lat1, real lon2, real lat2);   
       
      /// calculates a great-circle bearing between two locations.
      /*! This function calculates the great-circle bearing between two locations.
      
         \return the bearing between the two locations, in degrees clockwise from north
       
         \param lon1 longitude of the first point
         \param lat1 longitude of the first point
         \param lon2 longitude of the second point
         \param lat2 longitude of the second point
      */
      real bearing( real lon1, real lat1, real lon2, real lat2);   

      /// calculates great-circle distances between two sets of locations.
      /*! This function calculates the great-circle distances between two sets of locations.
      
         \param n the number of locations 
         \param lon1 a pointer to the longitudes of the first set of points
         \param lat1 a pointer to the latitudes of the first set of points
         \param lon2 a pointer to the longitudes of the second set of points
         \param lat2 a pointer to the longitudes of the second set of points
         \param d a pointer to the the output distances
      */
      void distance( int n, const real *lon1, const real *lat1, const real *lon2, const real *lat2, real *d);   
       
      /// calculates a position a given distance and bearing from a given position
      /*! This function calculates the longitude and latitude of a position
          a given distance and bearing from a starting position.
          
          \param clon the initial longitude
          \param clat the initial latitude
          \param d the distance away from the initial position
          \param bearng the bearing (angles clockwise from north) to the new position
          \param lon (output) the new longitude
          \param lat (output) the new latitude
      */
      void displace( const real clon, const real clat, const real d, const real bearng, real &lon, real &lat );    
       
      /// calculates positions a given distance and bearing from an array of given positions
      /*! This function calculates the longitude and latitude of an array of positions
          a given set of distances and bearings from an array of starting positions.
          
          \param n the number of locations to displace
          \param clon a pointer to an array of the initial longitudes
          \param clat a pointer to an array of the initial latitudes
          \param d a pointer to an array of the distances away from the initial positions
          \param bearng a pointer to an array of the bearings (angles clockwise from north) to the new positions
          \param lon a pointer to an array of output new longitudes
          \param lat a pointer to an array of output new latitudes
      */
      void displace( int n, const real *clon, const real *clat, const real *d, const real *bearng, real *lon, real *lat );    
     
      /// relocates a vector to a new location
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
         
         \param newlon the longitude of the relocation point
         \param newlat the latitude of the relocation point
         \param lon0 the longitude of the input vector
         \param lat0 the latitude of the input vector
         \param u a pointer to the east-west vector component, which will be replaced by the relocated value
         \param v a pointer to the north-south vector component, which will be replaced by the relocated value
      
      */
      void vRelocate( real newlon, real newlat, real lon0, real lat0, real *u, real *v ); 
     
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
      void vRelocate( int n, const real *newlon, const real *newlat, const real *lon0, const real *lat0, real *u, real *v ); 
       
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
