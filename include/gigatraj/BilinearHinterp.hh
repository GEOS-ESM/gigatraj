#ifndef GIGATRAJ_BILINEARHINTERP_H
#define GIGATRAJ_BILINEARHINTERP_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/HLatLonInterp.hh"
#include "gigatraj/GridCubedSphereField3D.hh"
#include "gigatraj/GridCubedSphereFieldSfc.hh"
#include <vector>
#include <math.h>

namespace gigatraj {

/*!
\ingroup hinterpolators
\brief class for bi-linearly interpolating latitude-longuitude grids

An object of this class interpolates a field on 
a quasi-horizontal two-dimensional longitude-latitude gridded surface,
to specific point locations (i.e., longitudes and latitudes) on that surface.
It does this by using linear interpolation in both longitude and latitude.

The various interpolation methods of these classes take an optional \c flags 
paremeter.  The flag constant values, which have names of the form GT_*, 
are defined in the gigatraj namespace.


*/


class BilinearHinterp : public HLatLonInterp {

   public:
   
   
      /// constructor
      BilinearHinterp();

      /// destructor
      ~BilinearHinterp();

      /// returns a copy of this Hinterp object
      BilinearHinterp* copy() const;

      /// returns the type of interpolator this is
      std::string id();

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
      real calc( real lon, real lat, const GridLatLonField3D& grid, const int k, int flags=0 ) const;      

      /// interpolates to a particular longitude and latitude
      /*! This function interpolates quasi-horizontal 2D surface field to 
          a particular longitude and latitude

           \return  the interpolated value of the field
           \param lon the longitude to interpolate to
           \param lat the latitude to interpolate to
           \param grid a 2D quasihorizontal grid of data to be interpolated
           \param flags flag values affecting the interpolation
      */
      real calc( real lon, real lat, const GridLatLonFieldSfc& grid, int flags=0 ) const;      
   
      /// interpolates to a vector of longitudes and latitudes
      /*! This function interpolates a level of a 3D field to a vector 
          of longitudes and latitudes.

          \return a new vector containing the interpolated values.
                  It is the calling routine's responsibility to delete this returned
                  variable when it is no longer needed.
           \param lons a vector of longitudes to interpolate to
           \param lats a vector of latitudes to interpolate to
           \param grid a 3D grid, one of whose layers will provide the surface to interpolate
           \param k the index of the grid vertical level which is to be interpolated
           \param flags flag values affecting the interpolation
      */
      std::vector<real>* calc( const std::vector<real>& lons, const std::vector<real>& lats, const GridLatLonField3D& grid, const int k, int flags=0 ) const;      

      /// interpolates to a vector of longitudes and latitudes
      /*! This function interpolates a quasi-horizontal 2D surface field to a 
          vector of longitudes and latitudes.
          
           \return a new vector containing the interpolated values.
              It is the calling routine's responsibility to delete this returned
              variable when it is no longer needed.
           \param lons a vector of longitudes to interpolate to
           \param lats a vector of latitudes to interpolate to
           \param grid a 2D quasihorizontal grid of data to be interpolated
           \param flags flag values affecting the interpolation
      */
      std::vector<real>* calc( const std::vector<real>& lons, const std::vector<real>& lats, const GridLatLonFieldSfc& grid, int flags=0 ) const;      

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
      real* calc( const int n, const real* lons, const real* lats, const GridLatLonField3D& grid, const int k, int flags=0 ) const;

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
      real* calc( const int n, const real* lons, const real* lats, const GridLatLonFieldSfc& grid, int flags=0 ) const;

      /// interpolates to an array of longitudes and latitudes
      /*! This function interpolates a quasi-horizontal 2D surface to 
          an array of longitudes and latitudes
          
           \param n the number of coordinates to interpolate to
           \param lons an array of longitudes to interpolate to
           \param lats an array of latitudes to interpolate to
           \param results a pointer to an array that will contain the interpolated values.
           \param grid a 2D quasihorizontal grid of data to be interpolated
           \param flags flag values affecting the interpolation
      */
      void calc( const int n, const real* lons, const real* lats, real* results, const GridLatLonFieldSfc& grid, int flags=0 ) const;


      /// interpolates a vector field held in two GridLatLonField3D objects to an arbitrary location  
      /*!
        This function interpolates a particular level of a 3D gridded vector field to to a particular longitude and latitude
        
        \param lon the longitude to interpolate to
        \param lat the latitude to interpolate to
        \param xval reference to the result value of the interpolated x vector component
        \param yval reference to the result value of the interpolated y vector component
        \param xgrid a 3D grid for the first vector component, one of whose layers will provide the surface to interpolate
        \param ygrid a 3D grid for the second vector component, one of whose layers will provide the surface to interpolate
        \param k the index of the grid vertical level which is to be interpolated
        \param flags flag values affecting the interpolation
      */
      void calc( real lon, real lat, real &xval, real &yval, const GridLatLonField3D& xgrid, const GridLatLonField3D& ygrid, const int k, int flags=0 ) const;

      /// interpolates a vector field held in two GridLatLonFieldSfc objects to an arbitrary location 
      /*!
        This function interpolates components of a 2D gridded quasihorizontal vector field to a particular longitude and latitude

       \param  lon the longitude to interpolate to
       \param  lat the latitude to interpolate to
       \param  xval reference to the result value of the interpolated x vector component
       \param  yval reference to the result value of the interpolated y vector component
       \param  xgrid a 2D quasihorizontal grid of the first vector component to be interpolated
       \param  ygrid a 2D quasihorizontal grid of the second vector component to be interpolated
       \param  flags flag values affecting the interpolation
      */
      void calc( real lon, real lat, real &xval, real &yval, const GridLatLonFieldSfc& xgrid, const GridLatLonFieldSfc& ygrid, int flags=0 ) const;

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
      void calc( int n, real* lons, real* lats, real* xvals, real* yvals, const GridLatLonFieldSfc& xgrid, const GridLatLonFieldSfc& ygrid, int flags=0 ) const;

      /// re-grids a quasi-horizontal 2D surface
      /*! This function interpolates a meteorological field on a quasi-horizontal 2D surface
          to a new set of gridpoints.
          
           \return a pointer to a new GridLatLonFieldSfc object with the new lat-lon grid, full of data interpolated
                   from the old grid. The calling routine is responsible for deleting
                   the new object when it is no longer needed.
           \param input the GridLatLonFieldSfc object whose data are to be re-gridded
           \param lons a vector of longitudes to define the new grid
           \param lats a vector of latitudes to define the new grid
           \param flags flag values affecting the interpolation
      */
      GridLatLonFieldSfc* regrid( const GridLatLonFieldSfc& input, const std::vector<real>& lons, const std::vector<real>& lats, int flags=0 ) const;
      
      /// re-grids a 3D field
      /*! This function interpolates a 3D meteorological field to a new horizontal grid
          by treating it as a stack of quasi-horizontal 2D surfaces.

           \return a pointer to a new GridLatLonField3D object with the new lat-lon grid.
                   The calling routine is responsible for deleting
                   the new object when it is no longer needed.\param input the GridLatLonField3D object whose data are to be re-gridded
           \param lons a vector of longitudes to define the new grid
           \param lats a vector of latitudes to define the new grid
           \param flags flag values affecting the interpolation
      */
      GridLatLonField3D* regrid( const GridLatLonField3D& input, const std::vector<real>& lons, const std::vector<real>& lats, int flags=0 ) const;

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
      GridLatLonField3D* regrid( const GridLatLonField3D& input, const std::vector<real>& lons, const std::vector<real>& lats, const std::vector<real>& zs, const Vinterp& vin, int flags=0 ) const;
      
      /// interpolates a 3D field to a particular longitude, latitude, and vertical coordinate
      /*! This function interpolates not only to a point horizontally but also vertically.
          First, each vertical level of the grid field is interpolated horizontally
          to the horizontal coordinates, resulting in a vertical profile.
          These are then interpolated to the actual point level using the vin interpolator.
       
           \return the interpolated value
           \param lon the longitude to interpolate to
           \param lat the latitude to interpolate to
           \param z the vertical level to interpolate to
           \param grid a 2D quasihorizontal grid of data to be interpolated
           \param vin a Vinterp object for doing the interpolation to the vertical levels.
           \param flags flag values affecting the interpolation
      
      */
      real vinterp( real lon, real lat, real z, const GridLatLonField3D& grid, const Vinterp& vin, int flags=0 ) const; 

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
           \param grid a 2D quasihorizontal grid of data to be interpolated
           \param vin a Vinterp object for doing the interpolation to the vertical levels.
           \param flags flag values affecting the interpolation
      
      */
      std::vector<real>* vinterp( const std::vector<real>& lons, const std::vector<real>& lats, const std::vector<real>& zs, const GridLatLonField3D& grid, const Vinterp& vin, int flags=0 ) const;
      
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
           \param grid a 2D quasihorizontal grid of data to be interpolated
           \param vin a Vinterp object for doing the interpolation to the vertical levels.
           \param flags flag values affecting the interpolation
      
      */
      real* vinterp( const int n, const real* lons, const real* lats, const real* zs, const GridLatLonField3D& grid, const Vinterp& vin, int flags=0 ) const; 


      /// interpolates to vectors of points horizontally and vertically
      /*!
       This function interpolates in three dimensions to points input as vectors. 
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
       void vinterp( const int n, const real* lons, const real* lats, const real* zs, real* results, const GridLatLonField3D& grid, const Vinterp& vin, int flags=0 ) const; 

       void vinterp( const int n, const real* lons, const real* lats, const real* zs, real* results, const GridCubedSphereField3D& grid, const Vinterp& vin, int flags=0 ) const;

      /// interpolates a vector field to an array of points, horizontally and vertically
      /*!
       
       This function interpolates to an array of points point not only horizontally but also vertically.
       First, each vertical level of the grid field is interpolated horizontally
       to the horizontal coordinates, resulting in a vertical profile.
       These are then interpolated to the actual point level using the vin interpolator.
       
       \param  n the number of points
       \param  lons the array of longitudes to interpolate to
       \param  lats the array of latitudes to interpolate to
       \param  zs the array of vertical levels to interpolate to
       \param  xvals the interpolated x vector components
       \param  yvals the interpolated y vector components
       \param  xgrid a 3D grid of vector x component data to be interpolated
       \param  ygrid a 3D grid of vector y component data to be interpolated
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
      */
      void vinterpVector( int n, const real* lons, const real* lats, const real* zs, real *xvals, real *yvals, const GridLatLonField3D& xgrid, const GridLatLonField3D& ygrid, const Vinterp& vin, int flags=0 ) const; 
      void vinterpVector( int n, const real* lons, const real* lats, const real* zs, real* xvals, real* yvals, const GridCubedSphereField3D& xgrid, const GridCubedSphereField3D& ygrid, const Vinterp& vin, int flags=0 ) const;

/*! \name Standard Hinterp methods
 These methods implement the standard methods required by the Hinterp class. 
*/
//@{
      /// interpolates a GridField3D object to an arbitrary location  
      /*!
        This function interpolates a particular level of a 3D gridded field to to a particular longitude and latitude
        
        \param lon the longitude to interpolate to
        \param lat the latitude to interpolate to
        \param grid a 3D grid, one of whose layers will provide the surface to interpolate
        \param k the index of the grid vertical level which is to be interpolated
        \param flags flag values affecting the interpolation
      */
      real calc( real lon, real lat, const GridField3D& grid, const int k, int flags=0 ) const;

      /// interpolates a GridFieldSfc object to an arbitrary location 
      /*!
        This function interpolates a 2D gridded quasihorizontal field to a particular longitude and latitude

       \param  lon the longitude to interpolate to
       \param  lat the latitude to interpolate to
       \param  grid a 2D quasihorizontal grid of data to be interpolated
       \param  flags flag values affecting the interpolation
       \return  the interpolated value (type real)
      */
      real calc( real lon, real lat, const GridFieldSfc& grid, int flags=0 ) const;


      /// interpolates a GridField3D object to a vector of points
      /*!
      This function interpolates a particular level of a 3D gridded field to a vector of particular longitudes and latitudes.

       \param  lon a vector of longitudes to interpolate to
       \param  lat a vector of latitudes to interpolate to
       \param  grid a 3D grid, one of whose layers will provide the surface to interpolate
       \param  k the index of the grid vertical level which is to be interpolated
       \param  flags flag values affecting the interpolation
       \return  a new vector of type real containing the interpolated values.
               It is the calling routine's responsibility to delete this returned
               variable when it is no longer needed.
      */
       std::vector<real>* calc( const std::vector<real>& lon, const std::vector<real>& lat, const GridField3D& grid, const int k, int flags=0 ) const;      

      /// interpolates a GridFieldSfc object to a vector of points
      /*!
       This function interpolates a 2D gridded quasihorizontal field to a vector of particular longitudes and latitudes.

       \param  lon a vector of longitudes to interpolate to
       \param  lat a vector of latitudes to interpolate to
       \param  grid a 2D quasihorizontal grid of data to be interpolated
       \param  flags flag values affecting the interpolation
       \return  a new vector of type real containing the interpolated values.
               It is the calling routine's responsibility to delete this returned
               variable when it is no longer needed.
      */
      std::vector<real>* calc( const std::vector<real>& lon, const std::vector<real>& lat, const GridFieldSfc& grid, int flags=0 ) const;      
      std::vector<real>* calc( const std::vector<real>& lon, const std::vector<real>& lat, const GridCubedSphereFieldSfc& grid, int flags=0 ) const;      

      /// interpolates a GridField3D object to an array of points
      /*!
         This function interpolates a meteorological field on a specified level of a 3D grid
         to an array of locations.
         
       \param  n the number of coordinates to interpolate to
       \param  lons an array of longitudes to interpolate to
       \param  lats an array of latitudes to interpolate to
       \param  grid a 3D grid, one of whose layers will provide the surface to interpolate
       \param  k the index of the grid vertical level which is to be interpolated
       \param  flags flag values affecting the interpolation

       \return  a pointer to a newly-allocated array of type real containing the interpolated values.
               It is the calling routine's responsibility to free this returned
               variable when it is no longer needed.
      */
      real* calc( const int n, const real* lons, const real* lats, const GridField3D& grid, const int k, int flags=0 ) const;

      /// interpolates a GridFieldSfc object to an array of points
      /*!
         This function interpolates a meteorological field on a quasi-horizontal surface 
         to an array of locations.
         
       \param  n the number of coordinates to interpolate to
       \param  lons an array of longitudes to interpolate to
       \param  lats an array of latitudes to interpolate to
       \param  grid a 2D quasihorizontal grid of data to be interpolated
       \param  flags flag values affecting the interpolation
       \return  a pointer to a newly-allocated array that contains the interpolated values.
          It is the calling routine's responsibility to free this returned
          variable when it is no longer needed.
      */
      real* calc( const int n, const real* lons, const real* lats, const GridFieldSfc& grid, int flags=0 ) const;

      /// interpolates a GridFieldSfc object to an array of points
      /*!
         This function interpolates a meteorological field on a quasi-horizontal surface 
         to an array of locations.
         
       \param  n the number of coordinates to interpolate to
       \param  lons an array of longitudes to interpolate to
       \param  lats an array of latitudes to interpolate to
       \param  results  a pointer to an array that contains the interpolated values.
       \param  grid a 2D quasihorizontal grid of data to be interpolated
       \param  flags flag values affecting the interpolation
      */
      void calc( const int n, const real* lons, const real* lats, real* results, const GridFieldSfc& grid, int flags=0 ) const;

      /// interpolates a vector field held in two GridField3D objects to an arbitrary location  
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
      void calc( real lon, real lat, real &xval, real &yval, const GridField3D& xgrid, const GridField3D& ygrid, const int k, int flags=0 ) const;

      /// interpolates a vector field held in two GridFieldSfc objects to an arbitrary location 
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
      void calc( real lon, real lat, real &xval, real &yval, const GridFieldSfc& xgrid, const GridFieldSfc& ygrid, int flags=0 ) const;

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
      void calc( int n,  real* lons, real* lats, real* xvals, real* yvals, const GridFieldSfc& xgrid, const GridFieldSfc& ygrid, int flags=0 ) const;



      /// interpolates a GridFieldSfc object to a new horizontal grid
      /*!
       This function interpolates a meteorological field on a quasi-horizontal surface
       to a new set of gridpoints.
       
       \param  input the GridFieldSfc object whose data are to be re-gridded
       \param  x a vector of x-coordinates to define the new grid
       \param  y a vector of y-coordinates to define the new grid
       \param  flags flag values affecting the interpolation
       \return  a pointer to a new GridFieldSfc object with the new quasihorizontal grid grid.
                The calling routine is responsible for deleting the new object when it is no longer needed.
      */
      GridFieldSfc* regrid( const GridFieldSfc& input, const std::vector<real>& x, const std::vector<real>& y, int flags=0 ) const;

      /// interpolates a GridField3D object to a new horizontal grid
      /*! 
       This function interpolates a meteorological field on a stack of quasi-horizontal surface
       to a new set of gridpoints.
       
       \param  input the GridField3D object whose data are to be re-gridded
       \param  x a vector of longitudes to define the new grid
       \param  y a vector of latitudes to define the new grid
       \param  flags flag values affecting the interpolation
       \return  a pointer to a new GridField3D object with the new lat-lon grid.
                The calling routine is responsible for deleting the new object when it is no longer needed.
      */
      GridField3D* regrid( const GridField3D& input, const std::vector<real>& x, const std::vector<real>& y, int flags=0 ) const;

      /// interpolates a GridField3D object to a new horizontal and vertical grid
      /*! 
       This function interpolates a meteorological field on a stack of quasi-horizontal surface
       to a new set of gridpoints.
       
       \param  input the GridField3D object whose data are to be re-gridded
       \param  x a vector of longitudes to define the new grid
       \param  y a vector of latitudes to define the new grid
       \param  z a vector of vertical levels to define the new grid
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
       \return  a pointer to a new GridField3D object with the new lat-lon grid.
                The calling routine is responsible for deleting the new object when it is no longer needed.
      */
      GridField3D* regrid( const GridField3D& input, const std::vector<real>& x, const std::vector<real>& y, const std::vector<real>& z, const Vinterp& vin, int flags=0 ) const;

      /// interpolates to a point horizontally and vertically
      /*!
       
       This function interpolates to a point not only horizontally but also vertically.
       First, each vertical level of the grid field is interpolated horizontally
       to the horizontal coordinates, resulting in a vertical profile.
       These are then interpolated to the actual point level using the vin interpolator.
       
       \param  lon the longitude to interpolate to
       \param  lat the latitude to interpolate to
       \param  z the vertical level to interpolate to
       \param  grid a 3D grid of data to be interpolated
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
       \return  the interpolated value
      */
      real vinterp( real lon, real lat, real z, const GridField3D& grid, const Vinterp& vin, int flags=0 ) const; 


      /// interpolates to arrays of points horizontally and vertically
      /*!
       This function interpolates in three dimensions to points input as arrays.
       First, each vertical level of the grid field is interpolated horizontally
       to the horizontal coordinates, resulting in a set of vertical profiles.
       These are then interpolated to the actual point levels using the vin interpolator.

       \param  lons a vector of longitudes to interpolate to
       \param  lats a vector of latitudes to interpolate to.  Must be the same length as lons.
       \param  zs a vector of vertical levels to interpolate to.  Must be the same length as lons.
       \param  grid a 3D grid of data to be interpolated
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags lag values affecting the interpolation
       \return  a new vector containing the interpolated values.
          It is the calling routine's responsibility to delete this returned
          variable when it is no longer needed.   

      */
      std::vector<real>* vinterp( const std::vector<real>& lons, const std::vector<real>& lats, const std::vector<real>& zs, const GridField3D& grid, const Vinterp& vin, int flags=0 ) const;

      /// interpolates to vectors of points horizontally and vertically
      /*!
       This function interpolates in three dimensions to points input as vectors. 
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
       \return  a pointer to a newly-allocated array that contains the interpolated values.
                It is the calling routine's responsibility to free this returned
                variable when it is no longer needed.
      */
      real* vinterp( const int n, const real* lons, const real* lats, const real* zs, const GridField3D& grid, const Vinterp& vin, int flags=0 ) const; 

      /// interpolates to vectors of points horizontally and vertically
      /*!
       This function interpolates in three dimensions to points input as vectors. 
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
      void vinterp( const int n, const real* lons, const real* lats, const real* zs, real* results, const GridField3D& grid, const Vinterp& vin, int flags=0 ) const; 

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
      void vinterpVector( real lon, real lat, real z, real &xval, real &yval, const GridLatLonField3D& xgrid, const GridLatLonField3D& ygrid, const Vinterp& vin, int flags=0 ) const; 


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
      void vinterpVector( real lon, real lat, real z, real &xval, real &yval, const GridField3D& xgrid, const GridField3D& ygrid, const Vinterp& vin, int flags=0 ) const; 


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
      void vinterpVector( int n, const real* lons, const real* lats, const real* zs, real *xvals, real *yvals, const GridField3D& xgrid, const GridField3D& ygrid, const Vinterp& vin, int flags=0 ) const; 
//@}

      /*! This is the threshold of the latitude poleward of which
          we apply the simple conformal vector adjustment
      */    
      static const real NEARPOLE;

      real gcdist( real lat1,real long1,real lat2,real long2) const;
      real minicalc( real lon, real lat
                                 , real lon11, real lat11
                                 , real lon21, real lat21
                                 , real lon12, real lat12
                                 , real lon22, real lat22
                                 , real d11, real d21, real d12, real d22 ) const;

   protected:
   
      /// private convenience function
      inline real minicalc( real lon, real lat
                          , real lon1, real lat1
                          , real lon2, real lat2
                          , real d11, real d21, real d12, real d22 ) const
      {
          real result;
          real fx, fx1, fy;
          
          fx = lon2 - lon1;
          if ( fx > 180.0 && lon2 > lon1 ) {
             fx = fx - 360.0;
          }   
          if ( fx < -180.0 && lon2 < lon1 ) {
             fx = fx + 360.0;
          }   
          fx1 = lon - lon1;
          if ( fx1 > 180.0 && lon2 > lon1 ) {
             fx1 = fx1 - 360.0;
          }   
          if ( fx1 < -180.0 && lon2 < lon1 ) {
             fx1 = fx1 + 360.0;
          } 
          fx = fx1 / fx;
          
          fy = ( lat - lat1 ) / ( lat2 - lat1 );  
          
           result = ( d22 - d12 - d21 + d11 )*fx*fy
               + ( d21 - d11 ) * fy
               + ( d12 - d11 ) * fx
               + d11;
      
           return result;
      }

      
   
};
}

#endif

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


