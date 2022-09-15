#ifndef STREAMDUMP_H
#define STREAMDUMP_H

#include <ostream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/ParcelReporter.hh"

namespace gigatraj {

/*!
\ingroup parcelreporters

\brief outputs Parcel state to an ostream

The StreamDump class dumps parcel states to an ostream.

If has a counterpart for loading parcels states from an istream, 
the StreamLoad class.

*/

class StreamDump : public ParcelReporter {

   private:
   
      // the stream to which parcel states are to be dumped
      std::ostream *os;
      
      // 0 for a text dump, 1 for a binary dump
      int mode;
      
   public:
   

      /// An exception for some kind of problem with the output
      class badstreamdump: public ParcelReporter::badreport {};


      /// Default constructor using cout
      /*! With the default constructor, the standard output stream cout
          is used as the destination of the Parcel state data being dumped.
          
          \param format determines the format of the input: 0=text (the default), 1=binary.
      
      */
      StreamDump( int format=0 );
      
      /// Alternate constructor using a specified ostream
      /*! With this alternate constructor, the output stream is specified.
      
         \param output the output stream 
         
         \param format determines the format of the input: 0=text (the default), 1=binary.
      
      */
      StreamDump( std::ostream& output, int format=0 );      
   
      /// Returns/set the dump format
      /*! This method returns (and optionally sets) the format of the dump to either text
          or binary.
          
          \param fmt Set this to 0 for text dumps, 1 for binary dumps.
                     If negative (the default), the format will not be set form
                     this parameter, anbd the currentl value will be returned.
          
          \return the current format (0=text, 1=binary)
      */
      int format( int fmt = -1 );
   
      /// Dump a single Parcel
      /*! This method dumps a single Parcel object.
      
          \param p the Parcel object to be dumped

      */
      void apply( Parcel& p ); 

      /// Dump an array of Parcels
      /*! This method dumps an array of Parcels.
      
          \param p the array of Parcel objects to be dumped

          \param n the number of Parcel objects to be operated on

      */
      void apply( Parcel * const p, const int n ); 


      /// dump a vector of parcels
      /*! This method dumps a vector of Parcels
      
          \param p the vector of Parcel objects to be dumped
           
      */
      void apply( std::vector<Parcel>& p ); 

      /// dump a list of parcels
      /*! This method dumps a list of Parcels
      
          \param p the list of Parcel objects to be dumped
           
      */
      void apply( std::list<Parcel>& p ); 

      /// dump a deque of parcels
      /*! This method dumps a deque of Parcels
      
          \param p the deque of Parcel objects to be dumped
           
      */
      void apply( std::deque<Parcel>& p ); 


      /// dump a Flock of parcels
      /*! This method dumps a Flock of Parcels
      
          \param p the Flock of Parcel objects to be dumped
           
      */
      void apply( Flock& p ); 

      /// dump a Swarm of parcels
      /*! This method dumps a Swarm of Parcels
      
          \param p the Swarm of Parcel objects to be dumped
           
      */
      void apply( Swarm& p ); 


      /*! \brief override << operator for single Parcels
          
          This operator override permits using the 
          common "stream << object" syntax with StreamDump objects
          as is used with a standard output stream object. 
          
          \param p a single Parcel to be read
          
      */
      StreamDump& operator<<( Parcel& p )
      {
         apply( p );
         return *this;  
      }
      /*! \brief override << operator for vectors of Parcels
          
          This operator override permits using the 
          common "stream << object" syntax with StreamDump objects
          as is used with a standard output stream object. 
          
          \param p a vector of Parcels to be Read
          
      */
      StreamDump& operator<<( std::vector<Parcel>& p )
      {
         apply( p );
         return *this;  
      }
      /*! \brief override << operator for lists of Parcels
          
          This operator override permits using the 
          common "stream << object" syntax with StreamDump objects
          as is used with a standard output stream object. 
          
          \param p a list of Parcels to be Read
          
      */
      StreamDump& operator<<( std::list<Parcel>& p )
      {
         apply( p );
         return *this;  
      }
      /*! \brief override << operator for deques of Parcels
          
          This operator override permits using the 
          common "stream << object" syntax with StreamDump objects
          as is used with a standard output stream object. 
          
          \param p a deque of Parcels to be Read
          
      */
      StreamDump& operator<<( std::deque<Parcel>& p )
      {
         apply( p );
         return *this;  
      }

      /*! \brief override << operator for Flocks of Parcels
          
          This operator override permits using the 
          common "stream << object" syntax with StreamDump objects
          as is used with a standard output stream object. 
          
          \param p a Flock of Parcels to be Read
          
      */
      StreamDump& operator<<( Flock& p )
      {
         apply( p );
         return *this;  
      }

      /*! \brief override << operator for Swarms of Parcels
          
          This operator override permits using the 
          common "stream << object" syntax with StreamDump objects
          as is used with a standard output stream object. 
          
          \param p a Swarm of Parcels to be Read
          
      */
      StreamDump& operator<<( Swarm& p )
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
