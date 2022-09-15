
#ifndef PARCELFILTER_H
#define PARCELFILTER_H

#include <iterator>
#include <deque>
#include <list>
#include <vector>


#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/Swarm.hh"

namespace gigatraj {

/*! \defgroup parcelfilters Parcel filters
\ingroup parcelstuff

\brief filters that create, initialize, destroy, report on, or otherwise deal with Parcels

This is a superclass from which every class that manipulates Parcel objects inherits.
Any object that creates or destroys Parcels, or which alters their state in some
what that is either impossible or inappropriate for the Parcels to do for themselves,
is a member of the ParcelFilter class.

*/

/*!
\ingroup parcelfilters

\brief an abstract class for operating on parcels

\details

The ParcelFilter class is an abstract class for performing a transformation on a collection
of Parcel objects.  This transformation may be a purely reporting function
(e.g., dumping the parcels' states to a file), or it may set parcels' states (e.g.,
a Parcel initializer). A ParcelFilter may be a source of Parcels (i.e., a member of
the ParcelGenerator class), or a sink for Parcels.


*/

class ParcelFilter {

   public:
   

      /// An exception for some kind of problem with the filter
      class badfilter {};

      /// An exception for a bad number of parcels
      class badparcelnum {};

      /// virtual destructor
      virtual ~ParcelFilter() {};

      /// Virtual method for applying the filter to a single Parcel
      /*! Apply the filter to a single parcel
      
      \param p the Parcel object to which the filter is to be applied

      */
      virtual void apply( Parcel& p ) = 0; 

      /// Virtual method for applying applying the filter to an array of Parcels
      /*! Apply the filter to an array of parcels
      
      \param p the array of Parcel objects to which the filter is to be applied
    
      \param n the number of Parcel objects in the array

      */
      virtual void apply( Parcel * const p, const int n ) = 0; 



};


/// Method for applying a filter to an iterable container of Parcels. 
/*!  Applies a filter to an iterable container full of parcels

     \param parcels the container object of parcels
     \param fil the filter object to be applied to each parcel in the container 

*/
template<class Bunch, class Filter>
void sweep(Bunch& parcels, Filter fil) 
{  
   typename Bunch::iterator it = parcels.begin();
   while ( it != parcels.end() ) {
      (*fil) (*it);  // apply the filter 
      ++it;
   }   

}




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

