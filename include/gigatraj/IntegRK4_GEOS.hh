
#ifndef GIGATRAJ_INTEGRK4_GEOS_H
#define GIGATRAJ_INTEGRK4_GEOS_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/PlanetNav.hh"
#include "gigatraj/MetGEOSDistributedData.hh"

namespace gigatraj {

/*!

\ingroup integrators

\brief implements fourth-order Rugge-Kutta integration of parcel trajectories

The IntegRK4 class provides a method for integrating trajectories
using the fourth-order Runga-Kutte method. In this method,
the wind components u and v are used to increment the
longitude and latitude positions independently.

*/


class IntegRK4_GEOS {

  private:
    
    // This is the threshold of the latitude poleward of which
    // we apply the adjustment
    static const real NEARPOLE;
    
  protected:
    int confml;
  public:
    
    /// the type of object this is
    static const string id;
 
    /// The basic constructor
    /*! 
          This is the basic constructor for a new IntegRK4_GEOS object.
    */
    IntegRK4_GEOS();
  
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
    void go( real &lon, real &lat, real &z, double &t, MetGEOSDistributedData *metsrc, PlanetNav *nav, double dt );

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
    void go( int n, real *lons, real *lats, real *zs, int *flags, double &t, MetGEOSDistributedData *metsrc, PlanetNav *nav, double dt );


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
