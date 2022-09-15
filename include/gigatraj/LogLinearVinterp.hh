#ifndef GIGATRAJ_LOGLINEARVINTERP_H
#define GIGATRAJ_LOGLINEARVINTERP_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Vinterp.hh"

#include <vector>

#include <math.h>

namespace gigatraj {

/*!
\ingroup vinterpolators
\brief class for log-linearly interpolating one or more vertical profiles to a specific vertical level

An object of this class provides means of interpolating meteorological
fields in their vertical coordinates.  Interpolation is done linearly in the
log of the interpolating variable, rather than the variable itself.

The various interpolation methods of this class take an optional \c flags 
paremeter.  The flag constant values, which have names of the form GT_*, 
are defined in the gigatraj namespace.



*/

class LogLinearVinterp : public Vinterp {

   public:
   
   
      /// constructor
      LogLinearVinterp();

      /// destructor
      ~LogLinearVinterp();

      /// returns a copy of this Vinterp object
      LogLinearVinterp* copy() const;

      /// returns the kind of interpolator this is
      std::string id(); 

      /// interpolates to a particular level at a given horizontal grid point.
      /*! Given \p i and \p j indices into the horizontal grid of a GridField3D object,
          this function interpolates to a particular vertical level at the \p (i,j) horizontal grid point.
          
           \returns the interpolated value
           \param z the level to interpolate to
           \param grid a 3D data grid
           \param i the first index into the horizontal grid of the point which is to be interpolated
           \param j the second index into the horizontal grid of the point which is to be interpolated
           \param flags flag values affecting the interpolation
      */
      real calc( real z, const GridField3D& grid, int i, int j, int flags=0 ) const;      

      /// interpolates to a vector of vertical levels at a given horizontal grid point.
      /*! Given \p i and \p j indices into the horizontal grid of a GridField3D object,
          this function interpolates to a vector of vertical levels at the \p (i,j) horizontal grid point.
      
          \return a new vector containing the interpolated values.
                  It is the calling routine's responsibility to delete this returned
                  variable when it is no longer needed.
           \param zs the level to interpolate to
           \param grid a 3D data grid
           \param i the first index into the horizontal grid of the point which is to be interpolated
           \param j the second index into the horizontal grid of the point which is to be interpolated
           \param flags flag values affecting the interpolation
      */
      std::vector<real>*  calc( const std::vector<real>& zs, const GridField3D& grid, int i, int j, int flags=0 ) const;      

      /// interpolates to an array of vertical levels at a given horizontal grid point.
      /*! Given \p i and \p j indices into the horizontal grid of a GridField3D object,
          this function interpolates to an array of vertical levels at the \p (i,j) horizontal grid point.
      
           \return a pointer to a newly-allocated array that contains the interpolated values.
                  It is the calling routine's responsibility to delete this returned
                  variable when it is no longer needed.
           \param n the number of levels to interpolate to
           \param zs an array of levels to interpolate to
           \param grid a 3D data grid
           \param i the first index into the horizontal grid of the point which is to be interpolated
           \param j the second index into the horizontal grid of the point which is to be interpolated
           \param flags flag values affecting the interpolation
      */
      real* calc( int n, const real* zs, const GridField3D& grid, int i, int j, int flags=0 ) const;      

      /// interpolates a 3D gridded field to a particular surface of the vertical coordinate
      /*! This function interpolates a 3D data grid to a given value of its vertical coordinate.
          The result is a 2D surface grid object that holds the vertically-interpolated
          values at each horizontal grid point of the 3D grid.
      
           \return a pointer to a new GridFieldSfc field that holds the interpolated values.  Note that the
                   calling routine will usually need to cast this to an appropriate
                   sub-class of GridFieldSfc. The calling routine is responsible for
                   deleting the new object when it is no longer needed.
           \param z the surface to interpolate to.  This will be the same physical quantity
                    as in the vertical cooridnate of the \p grid parameter.
           \param grid the 3D grid to interpolate
           \param flags flag values affecting the interpolation
      */            
      GridFieldSfc* surface( const real z, const GridField3D& grid, int flags=0 ) const;
   

   protected:

      /// inline function to do the weighted average for linear interpolation
      inline real minicalc( real z, real z1, real z2, real d1, real d2 ) const
      {
          real result;
          
          result = ( z - z1 ) / (z2 - z1) * (d2 - d1 ) + d1;
      
          return result;
      }
      
      
      /// interpolates a vector of data and a cooridnate to a single coordinate value
      /*! This function performs an interpolation, given a vector of dependent and independent
          variables.  It takes into account bad or missing data in both variables.
          
          \return the interpolated value
          \param griddata the input dependent variable
          \param vgriddata the input independent variable
          \param z the independent varibale value to interpolate to
          \param bad the bad-or-missing-data fill value used in \p griddata
          \param vbad the bad-or-missing-data fill value used in \p vgriddata
          \param dir specified the direction of the independent variable: +1 if increasing, -1 if decreasing
          \param debug set to 0 for quiet output, otherwide for debugging output.
      */    
      real interp( const std::vector<real>* griddata, const std::vector<real>* vgriddata, real z, real bad, real vbad, int dir, int debug=0 ) const; 


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
