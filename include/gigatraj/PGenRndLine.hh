#ifndef PGENRNDLINE_H
#define PGENRNDLINE_H


#include <deque>
#include <list>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/RandomSrc.hh"
#include "gigatraj/Earth.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/ParcelGenerator.hh"

namespace gigatraj {


/*!
\ingroup parcelgenerators

\brief generates collections of parcels randomly distributed along a great-circle line

The PGenRndLine class generates collections of parcels whose positions
are initialized at random locations along a great-circle line
between two locations.  Parcels are also distributed in the vertical, forming
a sort of ribbon along the great-circle line.
(All times are initialized to 0.0.)

Although Parcels are created in two dimensions (along-line and vertical), they are stored in
one-dimensional arrays or containers. They are stored in row-major
(i.e., Fortran) order: along-line first, then vertical
coordinates.

*/

class PGenRndLine : public ParcelGenerator {

   private:
      
       // source of random numbers
       RandomSrc rnd; 

      /*! initialize the gridpoints in a sequence container
      
         \param seq the sequence container that holds our parcels
         \param p the input parcel whose settings we are to copy
         \param beglon the beginning longitude value
         \param beglat the beginning latitude value
         \param endlon the ending longitude value
         \param endlat  the ending latitude value
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
      */
      template< template<class U, class = std::allocator<U> > class Seq>
      void init( Seq<Parcel>* seq, const Parcel& p
                           , real beglon, real beglat
                           , real endlon, real endlat
                           , real begz, real endz
                         );


   public:
      
      /// constructor
      /*! This is the consturctor for the PGenRndLine class.
      
      */
      PGenRndLine();
      
      /// Create an array of Parcels on a grid
      /*! This method creates an array of Parcels on a grid, in row-major order.
      
         \param p the input parcel whose settings we are to copy
         \param np the number of parcels to be created
         \param beglon the beginning longitude value
         \param beglat the beginning latitude value
         \param endlon the ending longitude value
         \param endlat  the ending latitude value
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         
         \return a Parcel pointer that points to an array of Parcel objects.
      */
      Parcel * create_array(const Parcel& p, int np
                           , real beglon, real beglat
                           , real endlon, real endlat
                           , real begz, real endz
                           );


      /// Create a vector Container of Parcel objects 
      /*! This method creates a vector Container of Parcel objects. 
      
         \param p the input parcel whose settings we are to copy
         \param np the number of parcels to be created
         \param beglon the beginning longitude value
         \param beglat the beginning latitude value
         \param endlon the ending longitude value
         \param endlat  the ending latitude value
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         
         \return a pointer to a vector of Parcel objects; the calling routine must delete this vector once it is no longer needed.
      */
       std::vector<Parcel>* create_vector(const Parcel& p, int np
                           , real beglon, real beglat
                           , real endlon, real endlat
                           , real begz, real endz
                           );
      
      /// Create a list Container of Parcel objects 
      /*! This method create a List container of Parcel objects.
      
         \param p the input parcel whose settings we are to copy
         \param np the number of parcels to be created
         \param beglon the beginning longitude value
         \param beglat the beginning latitude value
         \param endlon the ending longitude value
         \param endlat  the ending latitude value
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         
         \return a pointer to a list of Parcel obejcts; the calling routine must delete this list once it is no longer needed.
      */
       std::list<Parcel>* create_list(const Parcel& p, int np
                           , real beglon, real beglat
                           , real endlon, real endlat
                           , real begz, real endz
                           );
      
      /// Create a deque Container of Parcel objects 
      /*! This method creates a deque Container of Parcel objects.
      
         \param p the input parcel whose settings we are to copy
         \param np the number of parcels to be created
         \param beglon the beginning longitude value
         \param beglat the beginning latitude value
         \param endlon the ending longitude value
         \param endlat  the ending latitude value
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         
         \return a pointer to a deque of Parcel objects; the calling routine must delete this deque once it is no longer needed.
      */
       std::deque<Parcel>* create_deque(const Parcel& p, int np
                           , real beglon, real beglat
                           , real endlon, real endlat
                           , real begz, real endz
                           );
      
      
      /// create a Flock container of Parcels
      /*! This method creates a Flock container of Parcels and initializes their locations from the givne Parcel
      
         \param parcel the input parcel whose settings we are to copy
         \param np the number of parcels to be created
         \param beglon the beginning longitude value
         \param beglat the beginning latitude value
         \param endlon the ending longitude value
         \param endlat  the ending latitude value
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param pgrp a process-group object that is used for parallel processing
         \param r    the ratio of meteorological-data processors to parcel-tracing processors.
                     (For example, if r=3 then there will be one met processor for
                     every 3 parcel-tracing processors) 
         
         \return a pointer to a Flock of Parcel objects; the calling routine must delete this Flock once it is no longer needed.
      */
       Flock* create_Flock(const Parcel& parcel, int np
                           , real beglon, real beglat
                           , real endlon, real endlat
                           , real begz, real endz
                           , ProcessGrp* pgrp=NULLPTR, int r=0
                           );
      
      
      /// create a Swarm pseudo-container of Parcels
      /*! This method creates a Swarm pseudo-container of Parcels and initializes their locations from the givne Parcel
      
         \param parcel the input parcel whose settings we are to copy
         \param np the number of parcels to be created
         \param beglon the beginning longitude value
         \param beglat the beginning latitude value
         \param endlon the ending longitude value
         \param endlat  the ending latitude value
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param pgrp a process-group object that is used for parallel processing
         \param r    the ratio of meteorological-data processors to parcel-tracing processors.
                     (For example, if r=3 then there will be one met processor for
                     every 3 parcel-tracing processors) 
         
         \return a pointer to a Swarm of Parcel objects; the calling routine must delete this Swarm once it is no longer needed.
      */
       Swarm* create_Swarm(const Parcel& parcel, int np
                           , real beglon, real beglat
                           , real endlon, real endlat
                           , real begz, real endz
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
