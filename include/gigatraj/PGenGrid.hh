#ifndef PGENGRID_H
#define PGENGRID_H

#include <deque>
#include <list>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Earth.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/ParcelGenerator.hh"

namespace gigatraj {


/*!
\ingroup parcelgenerators

\brief generates collections of parcels on a 3D grid

The PGenGrid class generates collections of parcels whose positions
are initialized at regularly-spaced gridpoints in three dimensions.
(All times are initialized to 0.0.)

Although Parcels are created in three dimensions, they are stored in
one-dimensional arrays or containers. They are stored in row-major
(i.e., Fortran) order: longitudes first, then latitudes, then vertical
coordinates.

*/

class PGenGrid : public ParcelGenerator {

   private:
   
      /// calculate the number of gridpoints needed
      /*! This method calculates the number of gridpoints needed.
      
         \param beglon the beginning longitude value
         \param endlon the ending longitude value
         \param deltalon the longitude increment
         \param beglat the beginning latitude value
         \param endlat  the ending latitude value
         \param deltalat the latitude increment
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param deltaz the vertical coordinate increment
         
         \return the number of gridpoints
      */
      int count_gridpoints( real beglon, real endlon, real deltalon
                           , real beglat, real endlat, real deltalat
                           , real begz, real endz, real deltaz 
                           );
      

      /*! initialize the gridpoints in a sequence container
      
         \param seq the sequence container that holds our parcels
         \param p the input parcel whose settings we are to copy
         \param beglon the beginning longitude value
         \param endlon the ending longitude value
         \param deltalon the longitude increment
         \param beglat the beginning latitude value
         \param endlat  the ending latitude value
         \param deltalat the latitude increment
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param deltaz the vertical coordinate increment
      */
      template< template<class U, class = std::allocator<U> > class Seq>
      void initgrid( Seq<Parcel>* seq, const Parcel& p
                         , real beglon, real endlon, real deltalon
                         , real beglat, real endlat, real deltalat
                         , real begz, real endz, real deltaz  
                         );


   public:
   
      /// Create an array of Parcels on a grid
      /*! This method creates an array of Parcels on a grid, in row-major order.
      
         \param p the input parcel whose settings we are to copy
         \param np the number of parcels is returned in this variable
         \param beglon the beginning longitude value
         \param endlon the ending longitude value
         \param deltalon the longitude increment
         \param beglat the beginning latitude value
         \param endlat  the ending latitude value
         \param deltalat the latitude increment
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param deltaz the vertical coordinate increment
         
         \return a Parcel pointer that points to an array of Parcel objects.
      */
      Parcel * create_array(const Parcel& p, int *np
                           , real beglon, real endlon, real deltalon
                           , real beglat, real endlat, real deltalat
                           , real begz, real endz, real deltaz 
                           );


      /// Create a vector Container of Parcel objects 
      /*! This method creates a vector Container of Parcel objects. 
      
         \param p the input parcel whose settings we are to copy
         \param beglon the beginning longitude value
         \param endlon the ending longitude value
         \param deltalon the longitude increment
         \param beglat the beginning latitude value
         \param endlat  the ending latitude value
         \param deltalat the latitude increment
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param deltaz the vertical coordinate increment
         
         \return a pointer to a vector of Parcel objects; the calling routine must delete this vector once it is no longer needed.
      */
       std::vector<Parcel>* create_vector(const Parcel& p
                           , real beglon, real endlon, real deltalon
                           , real beglat, real endlat, real deltalat
                           , real begz, real endz, real deltaz 
                           );
      
      /// Create a list Container of Parcel objects 
      /*! This method create a List container of Parcel objects.
      
         \param p the input parcel whose settings we are to copy
         \param beglon the beginning longitude value
         \param endlon the ending longitude value
         \param deltalon the longitude increment
         \param beglat the beginning latitude value
         \param endlat  the ending latitude value
         \param deltalat the latitude increment
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param deltaz the vertical coordinate increment
         
         \return a pointer to a list of Parcel obejcts; the calling routine must delete this list once it is no longer needed.
      */
       std::list<Parcel>* create_list(const Parcel& p
                           , real beglon, real endlon, real deltalon
                           , real beglat, real endlat, real deltalat
                           , real begz, real endz, real deltaz 
                           );
      
      /// Create a deque Container of Parcel objects 
      /*! This method creates a deque Container of Parcel objects.
      
         \param p the input parcel whose settings we are to copy
         \param beglon the beginning longitude value
         \param endlon the ending longitude value
         \param deltalon the longitude increment
         \param beglat the beginning latitude value
         \param endlat  the ending latitude value
         \param deltalat the latitude increment
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param deltaz the vertical coordinate increment
         
         \return a pointer to a deque of Parcel objects; the calling routine must delete this deque once it is no longer needed.
      */
       std::deque<Parcel>* create_deque(const Parcel& p
                           , real beglon, real endlon, real deltalon
                           , real beglat, real endlat, real deltalat
                           , real begz, real endz, real deltaz 
                           );
      
      
      /// create a Flock container of Parcels
      /*! This method creates a Flock container of Parcels and initializes their locations from the givne Parcel
      
         \param parcel the input parcel whose settings we are to copy
         \param beglon the beginning longitude value
         \param endlon the ending longitude value
         \param deltalon the longitude increment
         \param beglat the beginning latitude value
         \param endlat  the ending latitude value
         \param deltalat the latitude increment
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param deltaz the vertical coordinate increment
         \param pgrp a process-group object that is used for parallel processing
         \param r    the ratio of meteorological-data processors to parcel-tracing processors.
                     (For example, if r=3 then there will be one met processor for
                     every 3 parcel-tracing processors) 
         
         \return a pointer to a Flock of Parcel objects; the calling routine must delete this Flock once it is no longer needed.
      */
       Flock* create_Flock(const Parcel& parcel
                           , real beglon, real endlon, real deltalon
                           , real beglat, real endlat, real deltalat
                           , real begz, real endz, real deltaz 
                           , ProcessGrp* pgrp=NULLPTR, int r=0
                           );
      
      
      /// create a Swarm pseudo-container of Parcels
      /*! This method creates a Swarm pseudo-container of Parcels and initializes their locations from the givne Parcel
      
         \param parcel the input parcel whose settings we are to copy
         \param beglon the beginning longitude value
         \param endlon the ending longitude value
         \param deltalon the longitude increment
         \param beglat the beginning latitude value
         \param endlat  the ending latitude value
         \param deltalat the latitude increment
         \param begz the beginning vertical coordinate value
         \param endz the ending vertical coordinate value
         \param deltaz the vertical coordinate increment
         \param pgrp a process-group object that is used for parallel processing
         \param r    the ratio of meteorological-data processors to parcel-tracing processors.
                     (For example, if r=3 then there will be one met processor for
                     every 3 parcel-tracing processors) 
         
         \return a pointer to a Swarm of Parcel objects; the calling routine must delete this Swarm once it is no longer needed.
      */
       Swarm* create_Swarm(const Parcel& parcel
                           , real beglon, real endlon, real deltalon
                           , real beglat, real endlat, real deltalat
                           , real begz, real endz, real deltaz 
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
