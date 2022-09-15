
#ifndef STREAMLOAD_H
#define STREAMLOAD_H

#include <istream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/ParcelInitializer.hh"

namespace gigatraj {

/*!
\ingroup parcelinitializers

\brief loads Parcel state information from an istream

The StreamLoad class reads information 
from an input stream and loads it into a Parcel
object or collection of Parcel objects.  Note that
the Parcel or collection of Parcels must already
exist.

StreamLoad works as a counterpart to the StreamDump class,
to make it easy to save and restore Parcels.
It uses the Parcel class's ">>" operator
to pull state information from input stream. This means
that the information in the stream is limited to the time, longitude,
latitude, status, and flags; all these must be present.


*/

class StreamLoad : public ParcelInitializer {

   private:
   
      // the stream from which parcel states are to be read
      std::istream *is;
      
      // 0 for a load from text, 1 for a binary load
      int mode;
      
   public:
   
      /// An exception for some kind of problem with the input
      class badinitstream: public ParcelInitializer::badinit {};

      /// Default constructor using cin
      /*! With the default StreamLoad constructor, the standard input stream cin 
          is used as the source of the Parcel state data to be loaded.
          
          \param format determines the format of the input: 0=text (the default), 1=binary.

      */
      StreamLoad( int format=0 );
      
      /// Alternate constructor using a specified istream
      /*! With this alternate constructor, the input stream is specified.
      
          \param input the input stream 

          \param format determines the format of the input: 0=text (the default), 1=binary.
          
      */
      StreamLoad( std::istream& input, int format=0  );      
   
      /// destructor
      /*!
          This is the destructor for the StreamLoad class
      */
      ~StreamLoad();

      /// Returns/set the load format
      /*! This method returns (and optionally sets) the format of the load from either text
          or binary.
          
          \param fmt Set this to 0 for text loads, 1 for binary loads.
                     If negative (the default), the format will not be set form
                     this parameter, anbd the currentl value will be returned.
          
          \return the current format (0=text, 1=binary)
      */
      int format( int fmt = -1 );

      /// sets/returns the input stream   
      /*! this methos sets and.or returns the input stream being used for the load.
      
          \param input a pointer ot the new stream. If NULL, then the current stream
                 is retained unchanged.
          
          \return a pointer ot the input stream
      */           
      std::istream* stream( std::istream* input=NULL );

      /// Initialize a single Parcel
      /*! This method initializes a single Parcel object.
      
          \param p the Parcel object to be initialized

      */
      void apply( Parcel& p ); 

      /// Initialize an array of Parcels
      /*! This method initializes an array of Parcels.
      
          \param p the array of Parcel objects to be initialized
           
          \param n the number of Parcel objects to be initialized

      */
      void apply( Parcel * p, const int n ); 

      /// Initialize a vector of parcels
      /*! This method initializes a vector of Parcels
      
          \param p the vector of Parcel objects to be initialized
           
      */
      void apply( std::vector<Parcel>& p ); 

      /// Initialize a list of parcels
      /*! This method initializes a list of Parcels
      
          \param p the list of Parcel objects to be initialized
           
      */
      void apply( std::list<Parcel>& p ); 

      /// Initialize a deque of parcels
      /*! This method initializes a deque of Parcels
      
          \param p the deque of Parcel objects to be initialized
           
      */
      void apply( std::deque<Parcel>& p ); 


      /// Initialize a Flock of parcels
      /*! This method initializes a Flock of Parcels
      
          \param p the Flock of Parcel objects to be initialized
           
      */
      void apply( Flock& p ); 


      /// Initialize a Swarm of parcels
      /*! This method initializes a Swarm of Parcels
      
          \param p the Swarm of Parcel objects to be initialized
           
      */
      void apply( Swarm& p ); 



      /*! \brief override >> operator for single Parcels
          
          This operator override permits using the 
          common "stream >> object" syntax with StreamLoad objects
          as is used with a standard input stream object. 
          
          \param p a single Parcel to be read
          
      */
      StreamLoad& operator>>( Parcel& p )
      {
         apply( p );
         return *this;  
      }

      /*! \brief override >> operator for vectors of Parcels
          
          This operator override permits using the 
          common "stream >> object" syntax with StreamLoad objects
          as is used with a standard input stream object. 
          
          \param p a vector of Parcels to be Read
          
      */
      StreamLoad& operator>>( std::vector<Parcel>& p )
      {
         apply( p );
         return *this;  
      }

      /*! \brief override >> operator for lists of Parcels
          
          This operator override permits using the 
          common "stream >> object" syntax with StreamLoad objects
          as is used with a standard input stream object. 
          
          \param p a list of Parcels to be Read
          
      */
      StreamLoad& operator>>( std::list<Parcel>& p )
      {
         apply( p );
         return *this;  
      }

      /*! \brief override >> operator for deques of Parcels
          
          This operator override permits using the 
          common "stream >> object" syntax with StreamLoad objects
          as is used with a standard input stream object. 
          
          \param p a deque of Parcels to be Read
          
      */
      StreamLoad& operator>>( std::deque<Parcel>& p )
      {
         apply( p );
         return *this;  
      }

      /*! \brief override >> operator for Flocks of Parcels
          
          This operator override permits using the 
          common "stream >> object" syntax with StreamLoad objects
          as is used with a standard input stream object. 
          
          \param p a Flock of Parcels to be Read
          
      */
      StreamLoad& operator>>( Flock& p )
      {
         apply( p );
         return *this;  
      }

      /*! \brief override >> operator for Swarms of Parcels
          
          This operator override permits using the 
          common "stream >> object" syntax with StreamLoad objects
          as is used with a standard input stream object. 
          
          \param p a Swarm of Parcels to be Read
          
      */
      StreamLoad& operator>>( Swarm& p )
      {
         apply( p );
         return *this;  
      }


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
