#ifndef GIGATRAJ_HLATLONINTERP_H
#define GIGATRAJ_HLATLONINTERP_H

#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Hinterp.hh"
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonFieldSfc.hh"

namespace gigatraj {

/*! \defgroup hinterpolators Horizontal Interpolators

\ingroup interpolators
\brief interpolates from a straight longitude-latitude grid

HLatLonInterp class members provide a means of interpolating
gridded data (provided by a GridLatLonFieldSfc or GridLatLonField3D
object) to an arbitrary desired longitude and latitude.

In general, a class in the Horizontal Interpolators
group might not necessarily interpolate from a 
longitude-latitude grid, although that is the
most common case.  Some meteorological data could
be structures with other kinds of horizontal grids.

*/


/*!
\ingroup hinterpolators

\brief abstract class for interpolating a quasi-horizontal surface 
to a specific longitude and latitude

The HLatLonInterp class is an abstract class.
An object of this class provides a means of interpolating a field on 
a quasi-horizontal two-dimensional longitude-latitude gridded surface,
to specific point locations (i.e., longitudes and latitudes) on that surface.

Also, please note that 
the various interpolation methods of these classes take an optional \c flags 
paremeter.  The flag constant values, which have names of the form GT_*, 
are defined in the gigatraj namespace.

*/

class HLatLonInterp : public Hinterp {

   public:
      
      // Default constructor
      HLatLonInterp() {};

      /// Default destructor
      virtual ~HLatLonInterp() {};
      
      /// returns a copy of this Hinterp object
      virtual HLatLonInterp* copy() const = 0;

      /// returns the kind of interpolator this is
      virtual std::string id() = 0; 

      /// interpolates to a particular longitude and latitude
      /*! This function interpolates a level of a 3D fields to a 
          particular longitude and latitude

           \return the interpolated value of the field
           \param lon the longitude to interpolate to
           \param lat the latitude to interpolate to
           \param grid a 3D grid, one of whose layers will provide the surface to interpolate
           \param k the index of the grid vertical level which is to be interpolated
           \param flags flag values affecting the interpolation
      */
      virtual real calc( real lon, real lat, const GridLatLonField3D& grid, const int k, int flags=0 ) const = 0;      

      /// interpolates to a particular longitude and latitude
      /*! This function interpolates quasi-horizontal 2D surface field to 
          a particular longitude and latitude

           \return  the interpolated value of the field
           \param lon the longitude to interpolate to
           \param lat the latitude to interpolate to
           \param grid a 2D quasihorizontal grid of data to be interpolated
           \param flags flag values affecting the interpolation
      */
      virtual real calc( real lon, real lat, const GridLatLonFieldSfc& grid, int flags=0 ) const = 0;      
   
      /// interpolates to a vector of longitudes and latitudes
      /*! This function interpolates a level of a 3D field to a vector 
          of longitudes and latitudes.

          \return a new vector containing the interpolated values.
                  It is the calling routine's responsibility to delete this returned
                  variable when it is no longer needed.
           \param lon a vector of longitudes to interpolate to
           \param lat a vector of latitudes to interpolate to
           \param grid a 3D grid, one of whose layers will provide the surface to interpolate
           \param k the index of the grid vertical level which is to be interpolated
           \param flags flag values affecting the interpolation
      */
      virtual std::vector<real>* calc( const std::vector<real>& lon, const std::vector<real>& lat, const GridLatLonField3D& grid, const int k, int flags=0 ) const = 0;      

      /// interpolates to a vector of longitudes and latitudes
      /*! This function interpolates a quasi-horizontal 2D surface field to a 
          vector of longitudes and latitudes.
          
           \return a new vector containing the interpolated values.
              It is the calling routine's responsibility to delete this returned
              variable when it is no longer needed.
           \param lon a vector of longitudes to interpolate to
           \param lat a vector of latitudes to interpolate to
           \param grid a 2D quasihorizontal grid of data to be interpolated
           \param flags flag values affecting the interpolation
      */
      virtual std::vector<real>* calc( const std::vector<real>& lon, const std::vector<real>& lat, const GridLatLonFieldSfc& grid, int flags=0 ) const = 0;      


      /// interpolates to an array of longitudes and latitudes
      /*! This function interpolates a level of a 3D field to an array 
          of longitudes and latitudes
          
          \return a pointer to a newly-allocated array that contains the interpolated values.
                  It is the calling routine's responsibility to free this returned
                  variable when it is no longer needed.
           \param n the number of coordinates to interpolate to
           \param lons an array of longitudes to interpolate to
           \param lats an array of latitudes to interpolate to
           \param grid a 3D grid, one of whose layers will provide the surface to interpolate
           \param k the index of the grid vertical level which is to be interpolated
           \param flags flag values affecting the interpolation
      */
      virtual real* calc( const int n, const real* lons, const real* lats, const GridLatLonField3D& grid, const int k, int flags=0 ) const = 0;

      /// interpolates to an array of longitudes and latitudes
      /*! This function interpolates a quasi-horizontal 2D surface to 
          an array of longitudes and latitudes
          
          \return a pointer to a newly-allocated array that contains the interpolated values.
                  It is the calling routine's responsibility to free this returned
                  variable when it is no longer needed.
           \param n the number of coordinates to interpolate to
           \param lons an array of longitudes to interpolate to
           \param lats an array of latitudes to interpolate to
           \param grid a 2D quasihorizontal grid of data to be interpolated
           \param flags flag values affecting the interpolation
      */
      virtual real* calc( const int n, const real* lons, const real* lats, const GridLatLonFieldSfc& grid, int flags=0 ) const = 0;

      /// interpolates a GridFieldSfc object to an array of points
      /*!
         This function interpolates a meteorological field on a quasi-horizontal surface 
         to an array of locations.
         
       \param  n the number of coordinates to interpolate to
       \param  lons an array of longitudes to interpolate to
       \param  lats an array of latitudes to interpolate to
       \param  results a pointer to an n-element array that holds the results of the interpolations.
       \param  grid a 2D quasihorizontal grid of data to be interpolated
       \param  flags flag values affecting the interpolation
      */
      virtual void calc( const int n, const real* lons, const real* lats, real* results, const GridLatLonFieldSfc& grid, int flags=0 ) const = 0;


      /// interpolates a vector field held in two GridLatLonField3D objects to an arbitrary location  
      /*!
        This function interpolates a particular level of a 3D gridded field to to a particular longitude and latitude
        
        \param lon the longitude to interpolate to
        \param lat the latitude to interpolate to
        \param xval reference to the result value of the interpolated x vector component
        \param yval reference to the result value of the interpolated y vector component
        \param xgrid a 3D grid for the first vector component, one of whose layers will provide the surface to interpolate
        \param ygrid a 3D grid for the second vector component, one of whose layers will provide the surface to interpolate
        \param k the index of the grid vertical level which is to be interpolated
        \param flags flag values affecting the interpolation
      */
      virtual void calc( real lon, real lat, real &xval, real &yval, const GridLatLonField3D& xgrid, const GridLatLonField3D& ygrid, const int k, int flags=0 ) const = 0;

      /// interpolates a vector field held in two GridLatLonFieldSfc objects to an arbitrary location 
      /*!
        This function interpolates a 2D gridded quasihorizontal field to a particular longitude and latitude

       \param  lon the longitude to interpolate to
       \param  lat the latitude to interpolate to
       \param  xval reference to the result value of the interpolated x vector component
       \param  yval reference to the result value of the interpolated y vector component
       \param  xgrid a 2D quasihorizontal grid of the first vector component to be interpolated
       \param  ygrid a 2D quasihorizontal grid of the second vector component to be interpolated
       \param  flags flag values affecting the interpolation
      */
      virtual void calc( real lon, real lat, real &xval, real &yval, const GridLatLonFieldSfc& xgrid, const GridLatLonFieldSfc& ygrid, int flags=0 ) const = 0;

      /// interpolates a vector field held in two GridLatLonFieldSfc objects to an array of arbitrary locations 
      /*!
        This function interpolates components of a 2D gridded quasihorizontal vector field to an array of longitudes and latitudes

       \param  n the number of positions (e.g., the length of lons, lats, xvals, and yvals)
       \param  lons an array of the longitudes to interpolate to
       \param  lats an array of the latitudes to interpolate to
       \param  xvals an array of the the result values of the interpolated x vector component
       \param  yvals an array of the result values of the interpolated y vector component
       \param  xgrid a 2D quasihorizontal grid of the first vector component to be interpolated
       \param  ygrid a 2D quasihorizontal grid of the second vector component to be interpolated
       \param  flags flag values affecting the interpolation
      */
      virtual void calc( int n, real* lons, real* lats, real* xvals, real* yvals, const GridLatLonFieldSfc& xgrid, const GridLatLonFieldSfc& ygrid, int flags=0 ) const = 0;
      
      /// re-grids a quasi-horizontal 2D surface
      /*! This function interpolates a meteorological field on a quasi-horizontal 2D surface
          to a new set of gridpoints.
          
           \return a pointer to a new GridLatLonFieldSfc object with the new lat-lon grid, full of data interpolated
                   from the old grid.
                The calling routine is responsible for deleting the new object when it is no longer needed.
           \param input the GridLatLonFieldSfc object whose data are to be re-gridded
           \param lons a vector of longitudes to define the new grid
           \param lats a vector of latitudes to define the new grid
           \param  flags flag values affecting the interpolation
      */
      virtual GridLatLonFieldSfc* regrid( const GridLatLonFieldSfc& input, const std::vector<real>& lons, const std::vector<real>& lats, int flags=0 ) const = 0;
      
      /// re-grids a 3D field
      /*! This function interpolates a 3D meteorological field to a new horizontal grid
          by treating it as a stack of quasi-horizontal 2D surfaces.

           \return a pointer to a new GridLatLonField3D object with the new lat-lon grid.
                The calling routine is responsible for deleting the new object when it is no longer needed.
           \param input the GridLatLonField3D object whose data are to be re-gridded
           \param lons a vector of longitudes to define the new grid
           \param lats a vector of latitudes to define the new grid
           \param  flags flag values affecting the interpolation
      */
      virtual GridLatLonField3D* regrid( const GridLatLonField3D& input, const std::vector<real>& lons, const std::vector<real>& lats, int flags=0 ) const = 0;

      /// interpolates a GridLatLonField3D object to a new horizontal and vertical grid
      /*! 
       This function interpolates a meteorological field on a stack of quasi-horizontal surface
       to a new set of gridpoints.
       
       \return  a pointer to a new GridLatLonField3D object with the new lat-lon grid.
                The calling routine is responsible for deleting the new object when it is no longer needed.
       \param  input the GridLatLonField3D object whose data are to be re-gridded
       \param  lons a vector of longitudes to define the new grid
       \param  lats a vector of latitudes to define the new grid
       \param  zs a vector of vertical levels to define the new grid
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
      */
      virtual GridLatLonField3D* regrid( const GridLatLonField3D& input, const std::vector<real>& lons, const std::vector<real>& lats, const std::vector<real>& zs, const Vinterp& vin, int flags=0 ) const = 0;
      
      /// interpolates a 3D field to a particular longitude, latitude, and vertical coordinate
      /*! This function interpolates not only to a point horizontally but also vertically.
          First, each vertical level of the grid field is interpolated horizontally
          to the horizontal coordinates, resulting in a vertical profile.
          These are then interpolated to the actual point level using the vin interpolator.
       
           \return the interpolated value
           \param lon the longitude to interpolate to
           \param lat the latitude to interpolate to
           \param z the vertical level to interpolate to
           \param grid a 3D grid of data to be interpolated
           \param vin a Vinterp object for doing the interpolation to the vertical levels.
           \param flags flag values affecting the interpolation
      
      */
      virtual real vinterp( real lon, real lat, real z, const GridLatLonField3D& grid, const Vinterp& vin, int flags=0 ) const = 0; 

      /// interpolates a 3D field to a vector of longitudes, latitudes, and vertical coordinates
      /*! This function interpolates not only to points horizontally but also vertically.
          First, each vertical level of the grid field is interpolated horizontally
          to the horizontal coordinates, resulting in a set of vertical profiles.
          These are then interpolated to the actual point levels using the vin interpolator.
          
           \return a new vector containing the interpolated values.
                   It is the calling routine's responsibility to delete this returned
                   variable when it is no longer needed.
           \param lons a vector of longitudes to interpolate to
           \param lats a vector of latitudes to interpolate to.  Must be the same length as lons.
           \param zs a vector of vertical levels to interpolate to.  Must be the same length as lons.
           \param grid a 3D grid of data to be interpolated
           \param vin a Vinterp object for doing the interpolation to the vertical levels.
           \param flags flag values affecting the interpolation
      
      */
      virtual std::vector<real>* vinterp( const std::vector<real>& lons, const std::vector<real>& lats, const std::vector<real>& zs, const GridLatLonField3D& grid, const Vinterp& vin, int flags=0 ) const = 0;

      /// interpolates a 3D field to an array of longitudes, latitudes, and vertical coordinates
      /*! This function interpolates not only to points horizontally but also vertically.
          First, each vertical level of the grid field is interpolated horizontally
          to the horizontal coordinates, resulting in a set of vertical profiles.
          These are then interpolated to the actual point levels using the vin interpolator.
         
           \return a pointer to a newly-allocated array that contains the interpolated values.
                   It is the calling routine's responsibility to free this returned
                   variable when it is no longer needed.       
           \param n the number of coordinates to interpolate to
           \param lons an array of longitudes to interpolate to
           \param lats an array of latitudes to interpolate to
           \param zs an array of vertical levels to interpolate to
           \param grid a 3D grid of data to be interpolated
           \param vin a Vinterp object for doing the interpolation to the vertical levels.
           \param flags flag values affecting the interpolation
      
      */
      virtual real* vinterp( const int n, const real* lons, const real* lats, const real* zs, const GridLatLonField3D& grid, const Vinterp& vin, int flags=0 ) const = 0; 

      /// interpolates a 3D field to an array of longitudes, latitudes, and vertical coordinates
      /*! This function interpolates not only to points horizontally but also vertically.
          First, each vertical level of the grid field is interpolated horizontally
          to the horizontal coordinates, resulting in a set of vertical profiles.
          These are then interpolated to the actual point levels using the vin interpolator.

       \param  n the number of coordinates to interpolate to
       \param  lons an array of longitudes to interpolate to
       \param  lats an array of latitudes to interpolate to
       \param  zs an array of vertical levels to interpolate to
       \param  grid a 3D grid of data to be interpolated
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
       \param  results  a pointer to a newly-allocated array that contains the interpolated values.
                It is the calling routine's responsibility to free this returned
                variable when it is no longer needed.
      */
        virtual void vinterp( const int n, const real* lons, const real* lats, const real* zs, real* results, const GridLatLonField3D& grid, const Vinterp& vin, int flags=0 ) const = 0; 

      /// interpolates a vector field to a point horizontally and vertically
      /*!
       
       This function interpolates to a point not only horizontally but also vertically.
       First, each vertical level of the grid field is interpolated horizontally
       to the horizontal coordinates, resulting in a vertical profile.
       These are then interpolated to the actual point level using the vin interpolator.
       
       \param  lon the longitude to interpolate to
       \param  lat the latitude to interpolate to
       \param  z the vertical level to interpolate to
       \param  xval a reference to the interpolated x vector component
       \param  yval a reference to the interpolated y vector component
       \param  xgrid a 3D grid of vector x component data to be interpolated
       \param  ygrid a 3D grid of vector y component data to be interpolated
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
      */
      virtual void vinterpVector( real lon, real lat, real z, real &xval, real &yval, const GridLatLonField3D& xgrid, const GridLatLonField3D& ygrid, const Vinterp& vin, int flags=0 ) const = 0; 


      /// interpolates a vector field to an array of points, horizontally and vertically
      /*!
       
       This function interpolates to an array of points point not only horizontally but also vertically.
       First, each vertical level of the grid field is interpolated horizontally
       to the horizontal coordinates, resulting in a vertical profile.
       These are then interpolated to the actual point level using the vin interpolator.
       
       \param  n the number of points
       \param  lons the array of the longitudes to interpolate to
       \param  lats the array of latitudes to interpolate to
       \param  zs the array of vertical level to interpolate to
       \param  xvals the interpolated x vector components
       \param  yvals the interpolated y vector components
       \param  xgrid a 3D grid of vector x component data to be interpolated
       \param  ygrid a 3D grid of vector y component data to be interpolated
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
      */
      virtual void vinterpVector( int n, const real* lons, const real* lats, const real* zs, real *xvals, real *yvals, const GridLatLonField3D& xgrid, const GridLatLonField3D& ygrid, const Vinterp& vin, int flags=0 ) const = 0; 

   
   protected:

      /*! Interprets the flags value passed to the various calculation routines,
          such that if interpolation is to be done locally (ignoring any
          multiprocessing), then 1 is returns.  Otherwise, 0 is returned.
      */    
      int do_local( int flags ) const
      {
          
          return (flags & ~GT_INTRP_LOCAL)? 1 : 0;
      
      }
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
