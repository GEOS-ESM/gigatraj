#ifndef PGENREP_H
#define PGENREP_H

#include <deque>
#include <list>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/ParcelGenerator.hh"

namespace gigatraj {

/*!
\ingroup parcelgenerators

\brief generates collections of parcels from a given parcel

The PGenRep ("Replicate") class generates collections of parcels that are all
copies of a given parcel object.

*/

class PGenRep : public ParcelGenerator {

   public:
   
      /// Create an array of Parcels as duplicates of a given Parcel
      /*! This methods creates an array of Parcels as duplicates of a given Parcel.
      
         \param parcel copy this parcel
         \param n the number of parcels to create
         
         \return a Parcel pointer that points to an array of Parcel objects.
      */
      Parcel * create_array(Parcel parcel, int n );   


      /// Create an vector Container of Parcel objects 
      /*! This method creates a vector Container of Parcel objects.
      
        \param parcel copy this parcel
        \param n  the size of the vector object; the number of parcels to create
         
         \return a pointer to a vector of Parcel objects; the calling routine must delete this vector once it is no longer needed.
      */
       std::vector<Parcel>* create_vector(Parcel parcel, int n );
      
      /// Create an list Container of Parcel objects 
      /*! This method creates a list Container of Parcel objects. 
      
        \param parcel copy this parcel
        \param n  the size of the list object; the number of parcels to create
         
         \return a pointer to a list of Parcel obejcts; the calling routine must delete this list once it is no longer needed.
      */
       std::list<Parcel>* create_list(Parcel parcel, int n );
      
      /// Create an deque Container of Parcel objects 
      /*! this method creates an deque Container of Parcel objects.
      
        \param parcel copy this parcel
        \param n  the size of the deque object; the number of parcels to create
         
         \return a pointer to a deque of Parcel objects; the calling routine must delete this deque once it is no longer needed.
      */
       std::deque<Parcel>* create_deque(Parcel parcel, int n );
      
      
      /// read replicate a Parcel into a Flock container of Parcels
      /*! This method creates a Flock container of Parcels and initializes their locations from the given Parcel
      
         \param parcel the input parcel whose settings we are to copy
         \param n  the size of the Flock object; the number of parcels to create
         \param pgrp a process-group object that is used for parallel processing
         \param r    the ratio of meteorological-data processors to parcel-tracing processors.
                     (For example, if r=3 then there will be one met processor for
                     every 3 parcel-tracing processors) 
         
         \return a pointer to a Flock of Parcel objects; the calling routine must delete this Flock once it is no longer needed.
      */
       Flock* create_Flock(const Parcel& parcel, int n
                           , ProcessGrp* pgrp=NULLPTR, int r=0
                           );
      
      
      /// read replicate a Parcel into a Swarm pseudo-container of Parcels
      /*! This method creates a Swarm pseudo-container of Parcels and initializes their locations from the given Parcel
      
         \param parcel the input parcel whose settings we are to copy
         \param n  the size of the Swarm object; the number of parcels to create
         \param pgrp a process-group object that is used for parallel processing
         \param r    the ratio of meteorological-data processors to parcel-tracing processors.
                     (For example, if r=3 then there will be one met processor for
                     every 3 parcel-tracing processors) 
         
         \return a pointer to a Swarm of Parcel objects; the calling routine must delete this Swarm once it is no longer needed.
      */
       Swarm* create_Swarm(const Parcel& parcel, int n
                           , ProcessGrp* pgrp=NULLPTR, int r=0
                           );
      
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
