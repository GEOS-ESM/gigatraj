
#ifndef GIGATRAJ_INTEGRK4A_H
#define GIGATRAJ_INTEGRK4A_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Integrator.hh"

namespace gigatraj {

/*!

\ingroup integrators

\brief implements fourth-order Rugge-Kutta integration of parcel trajectories

The IntegRK4a class provides a method for integrating trajectories
using the fourth-order Runga-Kutte method. In this method,
the wind components u and v are used to increment the
longitude and latitude positions independently, with special
adjustments applied poleward of 88 degrees latitude (see the
discussion in the documentation fo rthe Integrator class).

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
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/
