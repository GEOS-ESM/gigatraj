
#ifndef GIGATRAJ_INTEGRATOR_H
#define GIGATRAJ_INTEGRATOR_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/PlanetNav.hh"
#include "gigatraj/MetData.hh"

namespace gigatraj {

/*! \defgroup integrators  Time Integrators

\brief integrates the winds seen by a parcel to obtain its trajectory

Integrators provide a means of tracing 
parcel trajectories.  As such, they form the core of the 
gigatraj trajectory model.  They work by getting the 
the winds (obtained from a MetData object) at the parcel's location
and then applying a time integration scheme to get the parcel
location at a subsequent moment in time.


*/

/*!
\ingroup integrators

\brief abstract class for integration of parcel trajectories

The Integrator class is an abstract class for the numerical time integration 
for tracing trajectories.    

One topic that deserves special mention here is the spatial
translation vector quantities.  It is critical to understand that
a vector quantity such as the wind is not simply the collection
of its scalar components.  

Consider the basic equations of motion of a parcel in longitude (\f$\lambda\f$)
and latitude(\f$\phi\f$):

\f[ \frac{d\lambda}{dt} = u \f]
\f[ \frac{d\phi}{dt} = v \f]

To obtain the parcel position \f$(\lambda_1,\phi_1)\f$ at time \f$t_1\f$, 
given the position  \f$(\lambda_0,\phi_0)\f$ at time \f$t_0\f$, we
integrate:

\f[ \lambda_1 = \lambda_0 + u ( t_1 - t_0 ) \f]
\f[ \phi_1 = \phi_0 + v ( t_1 - t_0 ) \f]

For \f$u\f$ and \f$v\f$, we could use the wind at
the parcel's original location: \f$u(\lambda_0,\phi_0)\f$
and \f$v(\lambda_0,\phi_0)\f$.  However, this implies that
the winds remain constant over the spatial interval 
that the pracel traverses during time interval \f$(t_1 - t_0)\f$.
This is rarely the case, and so higher accuracy may be found with some
sort of iterative procedure in which initial estimates of 
\f$\lambda_1\f$ and \f$\phi_1\f$ are estimated as above, then
the \f$u\f$ and \f$v\f$ wind components are interpolated to 
this new location and combined in a weighted average 
with the wind components at the original location.  For example:


\f[ \lambda_1 = \lambda_0 + (u(\lambda_1,\phi_1) + u(\lambda_0,\phi_0))/2 ( t_1 - t_0 ) \f]

\f[ \phi_1 = \phi_0 + (v(\lambda_1,\phi_1) + v(\lambda_0,\phi_0)))/2 ( t_1 - t_0 ) \f]


In this way, time integration frequently involves
applying the vector wind from one location ("A") to a parcel at a 
slightly different location ("B")--in other words, wind vectors 
must often be translated in space.

Frequently, as with the example above, 
the scalar components of the wind at "A" are simply applied to the 
parcel at location "B".  But consider the situation in which
the (latitude,longitude) of the parcel at "A" is (89.999,0.0), with
the wind at that point being due north at 10 m/s.  If location "B" 
is just over the pole at (89.999,180.0), then the wind at "B" will
also be very nearly 10 m/s, but it will be due south.  The (u,v) 
components at "A" are expressed as (0,-10) and those at "B" as (0,10).
The two wind vectors are physically almost exactly the same
in magnitude and direction, but their vector components are
expressed in opposite terms that will cause 
problems when combining them arithmetically.  It is for this
reason that some integration schemes, naively applied,  
will caused parcels passing over the poles to become stuck there.

The effect is of course smaller and frequently negligible 
at low latitudes.  But anyone seeking to implement an integrator
for a trajectory model must be aware of the problem and take
steps to mitigate it.  Tests using the wind fields from the 
tilted solid-body earth rotation meteorological data sources, 
MetSBRot or MetGridSBRot, are strongly recommended, as these force
parcels in a circular path that traverses the poles.

*/

class Integrator {

  private:
 
  protected:
 
    /// 0 = no adjustment near poles on the sphere
    int confml;

  public:
    
    /// virtual destructor
    virtual ~Integrator() {};
    
    /// the kind of integrator this is
    static const string id;
 
    /// performs the integration over a time step
    /*! 
        This function performs the integration over a single time step
    
      \param lon the parcel longitude
      \param lat the parcel latitude
      \param z the parcel vertical coordinate
      \param t the time, in internal model time units
      \param metsrc the source of the meteorological data (winds)
      \param nav the planetary navigation object
      \param dt the time step, in internal model time units
       
    */
    virtual void go( real &lon, real &lat, real &z, double &t, MetData *metsrc, PlanetNav *nav, double dt ) = 0;

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
    virtual void go( int n, real *lons, real *lats, real *zs, int *flags, double &t, MetData *metsrc, PlanetNav *nav, double dt ) = 0;

    /// determines how vectors are interpolated near the poles of the sphere
    /*!
        This function sets how horizontal wind vectors are interpolated
        near the poles. Components of a vector field are not scalar fields.
        Near the poles. When the zonal and meridional winds are
        interpolated to a parcel position as part of the time integration,
        they must be adjusted to avoid introducing anomalies into
        the trajectories.
        
        \param mode sets the kind of adjustment to be applied.
                    A value of zero means that no adjustment will be applied.
                    The effect of other values depends on the particular
                    kind of integrator being used.  Note that the default
                    value is not necessary zero.

    */
    void conformal( int mode ); 

    /// returns how vectors are interpolated near the poles of the sphere
    /*!
        This function returns how horizontal wind vectors are interpolated
        near the poles. Components of a vector field are not scalar fields.
        Near the poles. When the zonal and meridional winds are
        interpolated to a parcel position as part of the time integration,
        they must be adjusted to avoid introducing anomalies into
        the trajectories.
        
        \return mode the kind of adjustment to be applied.
                    A value of zero means that no adjustment will be applied.
                    The effect of other values depends on the particular
                    kind of integrator being used.  

    */
    int conformal(); 


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
