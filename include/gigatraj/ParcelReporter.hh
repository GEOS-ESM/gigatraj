
#ifndef PARCELREPORTER_H
#define PARCELREPORTER_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/ParcelFilter.hh"

namespace gigatraj {

/*! \defgroup parcelreporters Parcel Reporting Filters
\ingroup parcelfilters

\brief report on Parcels

This is a superclass for classes that extract state information from Parcels
and (possibly) do something with it.

*/

/*!
\ingroup parcelreporters

\brief an abstract class for extracting state from Parcels

The ParcelReporter class is an abstract class for extracting state information from
a Parcel object or a collection of Parcel objects. 
The basic idea is that the Parcel states are left unchanged, but the information
gained is used to some purpose.
The purpose may be almost
anything, from a creating a written file dump of each parcel's state,
to a computing a single statistic compiled from the ensemble of all parcels.


*/

class ParcelReporter : public ParcelFilter {

   public:
   

      /// An exception for some kind of problem with the filter
      class badreport: public ParcelFilter::badfilter {};

      /// virtual destructor
      virtual ~ParcelReporter() {};

      /// Report on a single Parcel
      /*! This method reports on a single Parcel.
      
          \param p the Parcel object about which a report is to be made

      */
      virtual void apply( Parcel& p ) = 0; 

      /// Report on an array of Parcels
      /*! This method reports on an array of Parcels.
      
          \param p the array of Parcel objects about which a report is to be made
    
          \param n the number of Parcel objects to be operated on

      */
      virtual void apply( Parcel * const p, const int n ) = 0; 

      /// dump a vector of parcels
      /*! This method dumps a vector of Parcels
      
          \param p the vector of Parcel objects to be dumpd
           
      */
      virtual void apply( std::vector<Parcel>& p ) = 0; 

      /// dump a list of parcels
      /*! This method dumps a list of Parcels
      
          \param p the list of Parcel objects to be dumpd
           
      */
      virtual void apply( std::list<Parcel>& p ) = 0; 

      /// dump a deque of parcels
      /*! This method dumps a deque of Parcels
      
          \param p the deque of Parcel objects to be dumpd
           
      */
      virtual void apply( std::deque<Parcel>& p ) = 0; 


      /// dump a Flock of parcels
      /*! This method dumps a Flock of Parcels
      
          \param p the Flock of Parcel objects to be dumpd
           
      */
      virtual void apply( Flock& p ) = 0; 

      /// dump a Swarm of parcels
      /*! This method dumps a Swarm of Parcels
      
          \param p the Swarm of Parcel objects to be dumpd
           
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
