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
