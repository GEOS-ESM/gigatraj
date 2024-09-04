#ifndef GIGATRAJ_HINTERP_H
#define GIGATRAJ_HINTERP_H

#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Interpolator.hh"
#include "gigatraj/GridField3D.hh"
#include "gigatraj/GridFieldSfc.hh"
#include "gigatraj/Vinterp.hh"

namespace gigatraj {

/*! \defgroup hinterpolators Horizontal Interpolators

\ingroup interpolators
\brief interpolates from a horizontal spatial grid

Hinterp class members provide a means of interpolating
gridded data (provided by a GridFieldSfc or GridField3D
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

The Hinterp class is an abstract class.
An object of this class provides a means of interpolating a field on 
a quasi-horizontal two-dimensional gridded surface,
to specific point locations (i.e., longitudes and latitudes) on that surface.
Note that the horizontal grid does not necessarily have to be
a rectangular longitude-latitude grid.

The non-virtual sub-classes of Hinterp will need to be able to interpolate  from
a specific kind of GridField object, and in some cases to  generate a specific
kind of GridField object.  There are two approaches in implementing these
sub-classes.  One way would be to have the  classes' methods take a plain
GridField object as an argument. When the method is called with a specific
sub-class GridField object as an argument, that object would be upcast
automatically to  GridField.  Then inside the Hinterp sub-class method, that
GridField would be downcast back to the specific sub-class of GridField  that
the sub-class of Hinterp can deal with.  This way may be a little more pure
from the standpoint of object-oriented programming, but the price
of that purity is a performance penalty in a function that will
be used constantly in a trajectory model.  The other way is to
define, informally, a set of interpolation functions that all
Hinterp sub-classes should implement, but have the methods
as implemented by those sub-classes take actual specific non-virtual
GridField objects as arguments.  This is more efficient, but it
is less flexible; the programmer cannot treat all Hinterp objects
interchangeably when calling their interpolation methods.
In the case of Hinterp objects, this is an acceptable tradeoff,
because they are only ever called from within specific MetData
sub-classes that handle only specific sub-classes of GridField.
(If this is seems confusing, compare the list of function below with
the way their counterparts' declarations in, say, the BilinearHinterp
class.)

We chose both ways.  The hinterp class declares a set of pure virtual functions
that every non-virtual sub-class must implement.  But in addition to these, every
non-virtual sub-class should implement a set of overlaid functions
that use and return non-virtual sub-classes of GridField.  For example, the
BilinearHinterp class implements all of the methods declared here
that use generic GridField3D and GridFieldSfc object arguments, 
but it also provides functions with the same names, but which
take GridLatLonfield3D and GridLatLonFieldSfc object arguments.


Also, please note that 
the various interpolation methods of these classes take an optional \c flags 
paremeter.  The flag constant values, which have names of the form GT_*, 
are defined in the gigatraj namespace.



*/

class Hinterp : public Interpolator {

   public:
   
      /// \brief error in interpolation
      class badhinterp {};
      
      // Default constructor
      Hinterp() {};

      /// Default destructor
      virtual ~Hinterp() {};
      
      /// returns a copy of this Hinterp object
      virtual Hinterp* copy() const = 0;

      /// returns the kind of interpolator this is
      virtual std::string id() = 0; 

      /// interpolates a GridField3D object to an arbitrary location  
      /*!
        This function interpolates a particular level of a 3D gridded field to to a particular longitude and latitude
        
        \return  the interpolated value (type real)
        \param lon the longitude to interpolate to
        \param lat the latitude to interpolate to
        \param grid a 3D grid, one of whose layers will provide the surface to interpolate
        \param k the index of the grid vertical level which is to be interpolated
        \param flags flag values affecting the interpolation
      */
      virtual real calc( real lon, real lat, const GridField3D& grid, const int k, int flags=0 ) const = 0;

      /// interpolates a GridFieldSfc object to an arbitrary location 
      /*!
        This function interpolates a 2D gridded quasihorizontal field to a particular longitude and latitude

       \return  the interpolated value (type real)
       \param  lon the longitude to interpolate to
       \param  lat the latitude to interpolate to
       \param  grid a 2D quasihorizontal grid of data to be interpolated
       \param  flags flag values affecting the interpolation
      */
      virtual real calc( real lon, real lat, const GridFieldSfc& grid, int flags=0 ) const = 0;


      /// interpolates a GridField3D object to a vector of points
      /*!
      This function interpolates a particular level of a 3D gridded field to a vector of particular longitudes and latitudes.

       \return  a new vector of type real containing the interpolated values.
               It is the calling routine's responsibility to delete this returned
               variable when it is no longer needed.
       \param  lons a vector of longitudes to interpolate to
       \param  lats a vector of latitudes to interpolate to
       \param  grid a 3D grid, one of whose layers will provide the surface to interpolate
       \param  k the index of the grid vertical level which is to be interpolated
       \param  flags flag values affecting the interpolation
      */
      virtual  std::vector<real>* calc( const std::vector<real>& lons, const std::vector<real>& lats, const GridField3D& grid, const int k, int flags=0 ) const = 0;      

      /// interpolates a GridFieldSfc object to a vector of points
      /*!
       This function interpolates a 2D gridded quasihorizontal field to a vector of particular longitudes and latitudes.

       \return  a new vector of type real containing the interpolated values.
               It is the calling routine's responsibility to delete this returned
               variable when it is no longer needed.
       \param  lons a vector of longitudes to interpolate to
       \param  lats a vector of latitudes to interpolate to
       \param  grid a 2D quasihorizontal grid of data to be interpolated
       \param  flags flag values affecting the interpolation
      */
      virtual std::vector<real>* calc( const std::vector<real>& lons, const std::vector<real>& lats, const GridFieldSfc& grid, int flags=0 ) const = 0;      


      /// interpolates a GridField3D object to an array of points
      /*!
         This function interpolates a meteorological field on a specified level of a 3D grid
         to an array of locations.
         
       \return  a pointer to a newly-allocated array of type real containing the interpolated values.
               It is the calling routine's responsibility to free this returned
               variable when it is no longer needed.
       \param  n the number of coordinates to interpolate to
       \param  lons an array of longitudes to interpolate to
       \param  lats an array of latitudes to interpolate to
       \param  grid a 3D grid, one of whose layers will provide the surface to interpolate
       \param  k the index of the grid vertical level which is to be interpolated
       \param  flags flag values affecting the interpolation
      */
      virtual real* calc( const int n, const real* lons, const real* lats, const GridField3D& grid, const int k, int flags=0 ) const = 0;

      /// interpolates a GridFieldSfc object to an array of points
      /*!
         This function interpolates a meteorological field on a quasi-horizontal surface 
         to an array of locations.
         
       \return  a pointer to a newly-allocated array that contains the interpolated values.
          It is the calling routine's responsibility to free this returned
          variable when it is no longer needed.
       \param  n the number of coordinates to interpolate to
       \param  lons an array of longitudes to interpolate to
       \param  lats an array of latitudes to interpolate to
       \param  grid a 2D quasihorizontal grid of data to be interpolated
       \param  flags flag values affecting the interpolation
      */
      virtual real* calc( const int n, const real* lons, const real* lats, const GridFieldSfc& grid, int flags=0 ) const = 0;

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
      virtual void calc( const int n, const real* lons, const real* lats, real* results, const GridFieldSfc& grid, int flags=0 ) const = 0;

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
      virtual void calc( real lon, real lat, real &xval, real &yval, const GridField3D& xgrid, const GridField3D& ygrid, const int k, int flags=0 ) const = 0;

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
      virtual void calc( real lon, real lat, real &xval, real &yval, const GridFieldSfc& xgrid, const GridFieldSfc& ygrid, int flags=0 ) const = 0;

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
      virtual void calc( int n,  real* lons, real* lats, real* xvals, real* yvals, const GridFieldSfc& xgrid, const GridFieldSfc& ygrid, int flags=0 ) const = 0;

      /// interpolates a GridFieldSfc object to a new horizontal grid
      /*!
       This function interpolates a meteorological field on a quasi-horizontal surface
       to a new set of gridpoints.
       
       \return  a pointer to a new GridFieldSfc object with the new quasihorizontal grid grid.
                The calling routine is responsible for deleting the new object when it is no longer needed.
       \param  input the GridFieldSfc object whose data are to be re-gridded
       \param  x a vector of x-coordinates to define the new grid
       \param  y a vector of y-coordinates to define the new grid
       \param  flags flag values affecting the interpolation
      */
      virtual GridFieldSfc* regrid( const GridFieldSfc& input, const std::vector<real>& x, const std::vector<real>& y, int flags=0 ) const = 0;

      /// interpolates a GridField3D object to a new horizontal grid
      /*! 
       This function interpolates a meteorological field on a stack of quasi-horizontal surface
       to a new set of gridpoints.
       
       \return  a pointer to a new GridField3D object with the new lat-lon grid.
                The calling routine is responsible for deleting the new object when it is no longer needed.
       \param  input the GridField3D object whose data are to be re-gridded
       \param  x a vector of longitudes to define the new grid
       \param  y a vector of latitudes to define the new grid
       \param  flags flag values affecting the interpolation
      */
      virtual GridField3D* regrid( const GridField3D& input, const std::vector<real>& x, const std::vector<real>& y, int flags=0 ) const = 0;

      /// interpolates a GridField3D object to a new horizontal and vertical grid
      /*! 
       This function interpolates a meteorological field on a stack of quasi-horizontal surface
       to a new set of gridpoints.
       
       \return  a pointer to a new GridField3D object with the new lat-lon grid.
                The calling routine is responsible for deleting the new object when it is no longer needed.
       \param  input the GridField3D object whose data are to be re-gridded
       \param  x a vector of longitudes to define the new grid
       \param  y a vector of latitudes to define the new grid
       \param  z a vector of vertical levels to define the new grid
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
      */
      virtual GridField3D* regrid( const GridField3D& input, const std::vector<real>& x, const std::vector<real>& y, const std::vector<real>& z, const Vinterp& vin, int flags=0 ) const = 0;

      /// interpolates a GridField3D object to a new horizontal and vertical grid
      /*! 
       This function interpolates a meteorological field on a stack of quasi-horizontal surface
       to a new set of gridpoints.
       
       This function interpolates to a point not only horizontally but also vertically.
       First, each vertical level of the grid field is interpolated horizontally
       to the horizontal coordinates, resulting in a vertical profile.
       These are then interpolated to the actual point level using the vin interpolator.
       
       \return  the interpolated value
       \param  lon the longitude to interpolate to
       \param  lat the latitude to interpolate to
       \param  z the vertical level to interpolate to
       \param  grid a 3D grid of data to be interpolated
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
      */
      virtual real vinterp( real lon, real lat, real z, const GridField3D& grid, const Vinterp& vin, int flags=0 ) const = 0; 


      /// interpolates to arrays of points horizontally and vertically
      /*!
       This function interpolates in three dimensions to points input as arrays.
       First, each vertical level of the grid field is interpolated horizontally
       to the horizontal coordinates, resulting in a set of vertical profiles.
       These are then interpolated to the actual point levels using the vin interpolator.

       \return  a new vector containing the interpolated values.
          It is the calling routine's responsibility to delete this returned
          variable when it is no longer needed.   
       \param  lons a vector of longitudes to interpolate to
       \param  lats a vector of latitudes to interpolate to.  Must be the same length as lons.
       \param  zs a vector of vertical levels to interpolate to.  Must be the same length as lons.
       \param  grid a 3D grid of data to be interpolated
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags lag values affecting the interpolation

      */
      virtual std::vector<real>* vinterp( const std::vector<real>& lons, const std::vector<real>& lats, const std::vector<real>& zs, const GridField3D& grid, const Vinterp& vin, int flags=0 ) const = 0;


      /// interpolates to vectors of points horizontally and vertically
      /*!
       This function interpolates in three dimensions to points input as vectors. 
       First, each vertical level of the grid field is interpolated horizontally
       to the horizontal coordinates, resulting in a set of vertical profiles.
       These are then interpolated to the actual point levels using the vin interpolator.

       \return  a pointer to a newly-allocated array that contains the interpolated values.
                It is the calling routine's responsibility to free this returned
                variable when it is no longer needed.
       \param  n the number of coordinates to interpolate to
       \param  lons an array of longitudes to interpolate to
       \param  lats an array of latitudes to interpolate to
       \param  zs an array of vertical levels to interpolate to
       \param  grid a 3D grid of data to be interpolated
       \param  vin a Vinterp object for doing the interpolation to the vertical levels.
       \param  flags flag values affecting the interpolation
      */
      virtual real* vinterp( const int n, const real* lons, const real* lats, const real* zs, const GridField3D& grid, const Vinterp& vin, int flags=0 ) const = 0; 

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
      virtual void vinterp( const int n, const real* lons, const real* lats, const real* zs, real* results, const GridField3D& grid, const Vinterp& vin, int flags=0 ) const = 0; 


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
      virtual void vinterpVector( real lon, real lat, real z, real &xval, real &yval, const GridField3D& xgrid, const GridField3D& ygrid, const Vinterp& vin, int flags=0 ) const = 0; 

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
      virtual void vinterpVector( int n, const real* lons, const real* lats, const real* zs, real *xvals, real *yvals, const GridField3D& xgrid, const GridField3D& ygrid, const Vinterp& vin, int flags=0 ) const = 0; 

    /// determines how vectors are interpolated near the poles of the sphere
    /*!
        This function sets how horizontal wind vectors are interpolated
        near the poles. Components of a vector field are not scalar fields.
        Near the poles. When the zonal and meridional winds are
        interpolated to a location,
        they are best adjusted to avoid introducing anomalies into
        the trajectories. Doing this when interpolating within grids is less important when 
        than doing so when interpolating for time integration, since the grid spacing
        is typically small; however, it may still be helpful.
        
        \param mode sets the kind of adjustment to be applied.
                    A value of zero means that no adjustment will be applied.
                    The effect of other values depends on the particular
                    kind of Hinterp object being used.  Note that the default
                    value is not necessary zero.

    */
    void conformal( int mode ); 

    /// returns how vectors are interpolated near the poles of the sphere
    /*!
        This function returns how horizontal wind vectors are interpolated
        near the poles. Components of a vector field are not scalar fields.
        Near the poles. When the zonal and meridional winds are
        interpolated to a location,
        they are best adjusted to avoid introducing anomalies into
        the trajectories. Doing this when interpolating within grids is less important when 
        than doing so when interpolating for time integration, since the grid spacing
        is typically small; however, it may still be helpful.
        
        \return mode the kind of adjustment to be applied.
                    A value of zero means that no adjustment will be applied.
                    The effect of other values depends on the particular
                    kind of Hinterp object being used.  

    */
    int conformal(); 


   
   protected:
 
      /// 0 = no adjustment near poles on the sphere
      int confml;

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
