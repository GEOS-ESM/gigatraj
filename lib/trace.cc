
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
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

