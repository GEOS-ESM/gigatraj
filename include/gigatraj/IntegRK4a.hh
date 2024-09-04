
#ifndef GIGATRAJ_INTEGRK4A_H
#define GIGATRAJ_INTEGRK4A_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Integrator.hh"

namespace gigatraj {

/*!

\ingroup integrators

\brief implements fourth-order Runge-Kutta integration of parcel trajectories

The IntegRK4a class provides a method for integrating trajectories
using the fourth-order Runga-Kutte method. In this method,
the wind components u and v are used to increment the
longitude and latitude positions independently.

*/


class IntegRK4a : public Integrator {

  private:
    
    // This is the threshold of the latitude poleward of which
    // we apply the adjustment
    static const real NEARPOLE;


  public:
    
    /// the type of object this is
    static const string id;
 
    /// The basic constructor
    /*! 
          This is the basic constructor for a new IntegRK4a object.
    */
    IntegRK4a();
  
    /// performs the integration over a time step
    /*! 
        This function performs the integration over a single time step
    
      \param lon the parcel longitude
      \param lat the parcel latitude
      \param z the parcel vertical coordinate
      \param t the time, in internal model time
      \param metsrc the source of the meteorological data (winds)
      \param nav the planetary navigation object
      \param dt the time step, in internal model time
       
    */
    void go( real &lon, real &lat, real &z, double &t, MetData *metsrc, PlanetNav *nav, double dt );

    /// performs the integration over a time step, for an array of positions
    /*! 
        This function performs the integration over a single time step, for an array of positions.
    
      \param n the number of positions (length of lons, lats, and zs)
      \param lons a pointer to the array of parcel longitudes
      \param lats a pointer to the array of parcel latitudes
      \param zs a pointer to the array of parcel vertical coordinates
      \param flags a pointer to an array of flags indicating conditions (e.g., missing data) that prevent tracing
      \param t the time, in internal model time units
      \param metsrc the source of the meteorological data (winds)
      \param nav the planetary navigation object
      \param dt the time step, in internal model time units
       
    */
    void go( int n, real *lons, real *lats, real *zs, int *flags, double &t, MetData *metsrc, PlanetNav *nav, double dt );


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
