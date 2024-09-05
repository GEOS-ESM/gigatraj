
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


#include "config.h"

#include "gigatraj/Parcel.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/trace.hh"


using namespace gigatraj;

template<class Flockof> void trace(Flockof& parcels, double dt) 
{
   Parcel *px;
   
   typename Flockof::iterator it = parcels.begin();
   while ( it != parcels.end() ) {
      it->advance(dt);
      ++it;
   }   

};

template void trace<Flock>(Flock& parcels,                             double dt);
template void trace<Swarm>(Swarm& parcels,                             double dt);
template void trace<std::vector<Parcel>>(std::vector<Parcel>& parcels, double dt);
template void trace<std::deque<Parcel>>(std::deque<Parcel>& parcels,   double dt);
template void trace<std::list<Parcel>>(std::list<Parcel>& parcels,     double dt);


template<class Flockof> void trace(Flockof* parcels, double dt)
{
   typename Flockof::iterator it = parcels->begin();
   while ( it != parcels->end() ) {
      it->advance(dt);
      it++;
   }   

}

template void trace<Flock>(Flock* parcels,                             double dt);
template void trace<Swarm>(Swarm* parcels,                             double dt);
template void trace<std::vector<Parcel>>(std::vector<Parcel>* parcels, double dt);
template void trace<std::deque<Parcel>>(std::deque<Parcel>* parcels,   double dt);
template void trace<std::list<Parcel>>(std::list<Parcel>* parcels,     double dt);

template<class Flockof> void trace(Flockof& parcels, double &time, double dt)
{
   Parcel *px;
   double ptime;
   double pdt;
   
   typename Flockof::iterator it = parcels.begin();
   while ( it != parcels.end() ) {
      ptime = it->getTime();
      pdt = dt;
      if ( dt > 0 ) {
         // forward traj
         
         // we don't trace parcels from the future
         if ( ptime <= time ) {
         
            // bring the parcel up-to-date is needed
            while ( ptime < time ) {
               if ( (time - ptime) < pdt ) {
                  pdt = time - ptime;
               }
               it->advance(pdt);
               ptime = ptime + pdt;
            }
            
            // now move the particle forward to the new time
            it->advance(dt);
         }   
      } else {
         // back traj
         
         // Do everything above, except backwards. 
         if ( ptime >= time ) {
            while ( ptime > time ) {
                if ( (time - ptime) > pdt ) {
                   pdt = time - ptime;
                }
                it->advance(pdt);
                ptime = ptime + pdt;
            }
            
            // now move the particle forward to the new time
            it->advance(dt);
         } 
      }
        
      // next parcel
      ++it;
   }   

}

template void trace<Flock>(Flock& parcels,                             double& time, double dt);
template void trace<Swarm>(Swarm& parcels,                             double& time, double dt);
template void trace<std::vector<Parcel>>(std::vector<Parcel>& parcels, double& time, double dt);
template void trace<std::deque<Parcel>>(std::deque<Parcel>& parcels,   double& time, double dt);
template void trace<std::list<Parcel>>(std::list<Parcel>& parcels,     double& time, double dt);


template<class Flockof> void trace(Flockof* parcels, double &time, double dt)
{
   Parcel *px;
   double ptime;
   double pdt;
   
   typename Flockof::iterator it = parcels->begin();
   while ( it != parcels->end() ) {
      ptime = it->getTime();
      pdt = dt;
      if ( dt > 0 ) {
         // forward traj
         
         // we don't trace parcels from the future
         if ( ptime <= time ) {
         
            // bring the parcel up-to-date is needed
            while ( ptime < time ) {
               if ( (time - ptime) < pdt ) {
                  pdt = time - ptime;
               }
               it->advance(pdt);
               ptime = ptime + pdt;
            }
            
            // now move the particle forward to the new time
            it->advance(dt);
         }   
      } else {
         // back traj
         
         // Do everything above, except backwards. 
         if ( ptime >= time ) {
            while ( ptime > time ) {
                if ( (time - ptime) > pdt ) {
                   pdt = time - ptime;
                }
                it->advance(pdt);
                ptime = ptime + pdt;
            }
            
            // now move the particle forward to the new time
            it->advance(dt);
         } 
      }
        
      // next parcel
      ++it;
   }   

}

template void trace<Flock>(Flock* parcels,                             double& time, double dt);
template void trace<Swarm>(Swarm* parcels,                             double& time, double dt);
template void trace<std::vector<Parcel>>(std::vector<Parcel>* parcels, double& time, double dt);
template void trace<std::deque<Parcel>>(std::deque<Parcel>* parcels,   double& time, double dt);
template void trace<std::list<Parcel>>(std::list<Parcel>* parcels,     double& time, double dt);

