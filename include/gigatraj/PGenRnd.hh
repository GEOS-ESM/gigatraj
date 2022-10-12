#ifndef PGENRND_H
#define PGENRND_H

#include <cstdlib>
#include <deque>
#include <list>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/RandomSrc.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/ParcelGenerator.hh"

namespace gigatraj {

/*!
\ingroup parcelgenerators

\brief generates collections of parcels uniformly distributed around the globe

The PGenRnd class generates collections of parcels that are 
randomly distributed horizontally over the entire globe.
The distribution of points takes into account the convergence
of longitude lines at the pole.

Aside from the horizontal position, all other characteristics
of the Parcels are copied from an input parcel. This includes the vertical coordinate position.

*/

class PGenRnd : public ParcelGenerator {


   public:
   
      /// Constructor
      /*! This is the constructor for the PGenRnd class
          This sets us a rectangle in lat-lon space in which the parcels will be distirbuted.
          By default, the rectangle covers the globe.
          
      \param LLlat the latitude of the southern westernmost corner of the rectangle 
      \param LLlon the longitude of the southern westernmost corner of the rectangle
      \param URlat the latitude of the northern easternmost corner of the rectangle
      \param URlon the longitude of the northern easternmost corner of the rectangle
      */
      PGenRnd( float LLlat=-90.0, float LLlon=0.0, float URlat=90.0, float URlon=359.999 ); 

      /// sets the longitude.latitude rectangular domain
      /*!
          This sets us a rectangle in lat-lon space in which the parcels will be distirbuted.
          By default, the rectangle covers the globe.
          
      \param LLlat the latitude of the southern westernmost corner of the rectangle 
      \param LLlon the longitude of the southern westernmost corner of the rectangle
      \param URlat the latitude of the northern easternmost corner of the rectangle
      \param URlon the longitude of the northern easternmost corner of the rectangle
      
      */
      void setBox( float LLlat, float LLlon, float URlat, float URlon );

      /// Sets vertical boundaries
      /*! This sets the vertical bounaries of a PGenRnd object.
             
         \param zlow the lower vertical boundary
         \param zhigh the upper vertical boundary
      */
      void setZ( float zlow=0.0, float zhigh=1.0 ); 
   
      /// Create an array of Parcels 
      /*! This method creates an array of randomly-distributed Parcels. 
      
         \param parcel copy this parcel
         \param n the number of parcels to create
         
         \return a Parcel pointer that points to an array of Parcel objects.
      */
      Parcel * create_array(Parcel parcel, int n );   

      /// Create an vector Container of Parcel objects 
      /*! This method creates a vector Container of randomly-distributed Parcel objects. 
      
        \param parcel copy this parcel
        \param n  the size of the vector object; the number of parcels to create
         
         \return a pointer to a vector of Parcel objects; the calling routine must delete this vector once it is no longer needed.
      */
       std::vector<Parcel>* create_vector(Parcel parcel, int n );
      
      /// Create an list Container of Parcel objects 
      /*! this method creates a list Container of randomly-distributed Parcel objects.
      
        \param parcel copy this parcel
        \param n  the size of the list object; the number of parcels to create
         
         \return a pointer to a list of Parcel obejcts; the calling routine must delete this list once it is no longer needed.
      */
       std::list<Parcel>* create_list(Parcel parcel, int n );
      
      /// Create an deque Container of Parcel objects 
      /*! This method creates a deque Container of randomly-distributed Parcel objects 
      
        \param parcel copy this parcel
        \param n  the size of the deque object; the number of parcels to create
         
         \return a pointer to a deque of Parcel objects; the calling routine must delete this deque once it is no longer needed.
      */
       std::deque<Parcel>* create_deque(Parcel parcel, int n );
         
      
      /// read locations randomly into a Flock container of Parcels
      /*! This method creates a Flock container of Parcels and initializes their locations randomly (horizontally) at a given level.
      
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
      
      /// read locations randomly into a Swarm pseudo-container of Parcels
      /*! This method creates a Swarm pseudo-container of Parcels and initializes their locations randomly (horizontally) at a given level.
      
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
      
      
      /// Seed the random number generator
      /*! This method seeds the random number generator for parcel location generation.
      
         \param saw a pointer to a seed for the random number generator.
                    If NULL, then an internal seed generator is used.
      */
      void seed( unsigned const int *saw=NULL );

   
   private:
     
       RandomSrc rnd; 
       
       /// westernmost longoitude
       float lon0;
       /// easternmost longitude
       float lon1;
       /// sine of the starting latitude
       float slat0;
       /// sine of the ending latitude
       float slat1;
       /// flag: use vertical levels
       int use_z;
       /// lower vertical boundary
       float z0;
       /// upper verticla boundary
       float z1;
       

      /// initialize parcels positions in a sequence container
      /*! initialize parcels positions in a sequence container
      
         \param seq the sequence container that holds our parcels
         \param p the input parcel whose settings we are to copy
      */
      template< template<class U, class = std::allocator<U> > class Seq>
      void init( Seq<Parcel>* seq
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
