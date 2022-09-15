#ifndef GIGATRAJ_TRACE_H
#define GIGATRAJ_TRACE_H


#include <iterator>
#include <deque>
#include <list>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/Swarm.hh"



///  Moves an iterable container full of parcels one step forward
/*!  \ingroup parcels
     Given an iterable container that holds Parcel objects,
     this method is used to advance each Parcel's trajectory by one time step.
     
     \param parcels the container object of parcels
     \param dt the time step 

*/
template<class Flockof> void trace(Flockof& parcels, double dt); 

///  Moves an iterable container full of parcels one step forward
/*!  \ingroup parcels
     Given an interable container that holds Parcel objects,
     this method is used to advance each Parcel's trajectory by one time step.
     
     \param parcels a pointer to the container object of parcels
     \param dt the time step 

*/
template<class Flockof> void trace(Flockof* parcels, double dt);

///  Moves an iterable container full of parcels one step forward, incrementing model time
/*!  \ingroup parcels
     Given an iterable container that holds Parcel objects,
     this method is used to advance each Parcel's trajectory by one time step.
     This version of trace() takes a model time as an argument.
     In this way, parcels may be initialized with different times,
     and they will not be traced until the model time matches their
     internal time.
     
     \param parcels the container object of parcels
     \param time the model time. this gets incmrented by dt after the step is done.
     \param dt the time step 

*/
template<class Flockof> void trace(Flockof& parcels, double &time, double dt);

///  Moves an iterable container full of parcels one step forward, incrementing model time
/*!  \ingroup parcels
     Given an iterable container that holds Parcel objects,
     this method is used to advance each Parcel's trajectory by one time step.
     This version of trace() takes a model time as an argument.
     In this way, parcels may be initialized with different times,
     and they will not be traced until the model time matches their
     internal time.
     
     \param parcels a pointer to the container object of parcels
     \param time the model time. this gets incmrented by dt after the step is done.
     \param dt the time step 

*/
template<class Flockof> void trace(Flockof* parcels, double &time, double dt);

#endif

/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/
