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
