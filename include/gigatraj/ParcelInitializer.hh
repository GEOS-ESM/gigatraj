
#ifndef PARCELINITIALIZER_H
#define PARCELINITIALIZER_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/ParcelFilter.hh"

namespace gigatraj {

/*! \defgroup parcelinitializers Parcel Initialization Filters
\ingroup parcelfilters

\brief initialize Parcels

This is a superclass for classes that loads state information
into Parcel objects.

*/


/*!
\ingroup parcelinitializers

\brief an abstract base class for loading state into Parcels

The ParcelInitializer class is an abstract class for loading state information into a Parcel
object or collection of Parcel objects.  Note that an
object of this class does not create new Parcel objects;
the Parcel or collection of Parcels must already
exist.


*/

class ParcelInitializer : public ParcelFilter {

   public:
   

      /// An exception for some kind of problem with the filter
      class badinit: public ParcelFilter::badfilter {};

      /// virtual destructor
      virtual ~ParcelInitializer() {};

      /// Initialize a single Parcel
      /*! This method initializes a single Parcel object.
      
          \param p the Parcel object to be initialized

      */
      virtual void apply( Parcel& p ) = 0; 

      /// Initialize an array of parcels
      /*! This method initializes an array of Parcels
      
          \param p the array of Parcel objects to be initialized
           
          \param n the number of Parcel objects to be initialized

      */
      virtual void apply( Parcel * const p, const int n ) = 0; 

      /// Initialize a vector of parcels
      /*! This method initializes a vector of Parcels
      
          \param p the vector of Parcel objects to be initialized
           
      */
      virtual void apply( std::vector<Parcel>& p ) = 0; 

      /// Initialize a list of parcels
      /*! This method initializes a list of Parcels
      
          \param p the list of Parcel objects to be initialized
           
      */
      virtual void apply( std::list<Parcel>& p ) = 0; 

      /// Initialize a deque of parcels
      /*! This method initializes a deque of Parcels
      
          \param p the deque of Parcel objects to be initialized
           
      */
      virtual void apply( std::deque<Parcel>& p ) = 0; 

      /// Initialize a Flock of parcels
      /*! This method initializes a Flock of Parcels
      
          \param p the Flock of Parcel objects to be initialized
           
      */
      virtual void apply( Flock& p ) = 0; 

      /// Initialize a Swarm of parcels
      /*! This method initializes a Swarm of Parcels
      
          \param p the Swarm of Parcel objects to be initialized
           
      */
      virtual void apply( Swarm& p ) = 0; 

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

