#ifndef GIGATRAJ_INTERPOLATOR_H
#define GIGATRAJ_INTERPOLATOR_H

namespace gigatraj {

/*! @name Interpolation Flags
   Interpolation in space and time is so prevalent in gigatraj that these bitwise GT_* flag values
   to control how it is done are made global to the gigatraj namespace.

The various interpolation methods of these classes take an optional flags 
paremeter.  The flag constant values, which have names of the form GT_*, 
are defined in the gigatraj namespace.

*/
//@{
/// Interpolation flag: Do the interpolation locally, ignoring any multiprocessing
const int GT_INTRP_LOCAL = 0x0001;
//@}


/*! \defgroup interpolators Interpolators

    Given a grid of meteorological field values, Interpolator classes
    provide an estimate of the field at a desired point.
    
*/

/*!
   \brief abstract class for interpolating data from some kind of grid
   
   There are very few characteristics or methods that are common
   to all kinds of interpolation.  Chiefly these seem to be
   error conditions.  The Interpolator class exists in order
   to define a set of exceptions that are widely applicable
   to interpolation.
    
*/
class Interpolator {

   public:
      
      /// Error: Point is out-of-domain of data
      class badoutofdomain {};
      
      /// Error: interpolation over incompatible grids
      class badincompatible {};
      
      /// Error: not enough data for interpolation
      class badnodata {};

         
      // Default constructor
      Interpolator() {};

      /// Default destructor
      virtual ~Interpolator() {};


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
