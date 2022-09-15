#ifndef RANDOMSRC_H
#define RANDOMSRC_H


#include "gigatraj/gigatraj.hh"

namespace gigatraj {

/*!
\ingroup misc
   \brief source of random numbers 
   
 An object of the RandomSrc class generates random numbers.
 
*/

class RandomSrc {

   public:
   
       /*! default constructor.
           The seed is initialized internally
       */
       RandomSrc();
       
       /*! constructor with seed initialization.
       \param seed the seed for initializing the random number generator
       */
       RandomSrc( unsigned int seed );
       
       /*! constructor with seed device.
       \param dev the name of the device fomr which the initial random seed is obtained
                  (The default is "/dev/random".)
       */
       RandomSrc( const std::string dev );
       
       /// destructor
       ~RandomSrc();
       
       /*! returns a uniformly distributed random number
          \param min the number returned will be no less than this
          \param max the number returned will be no greater than this
       */    
       real uniform( real min=0.0, real max=1.0 );
       

       /*! returns a raw integer random number directly
           from the random number generator
       */
       int raw(); 
       
       /*! generate a seed for the random number generator
       */
       unsigned int genSeed();
       
       /*! seed the random number generator
       
           \param saw a pointer to a seed. If NULL, an internal generator is used.
       */
       void seed( const unsigned int *saw=NULL);   

   private:
       
       /// the name of a device from which random numbers may be read. The default is /dev/random.
       std::string rdev;

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
