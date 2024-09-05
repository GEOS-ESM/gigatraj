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
***     L. R. Lait (NASA Ames Research Center, Code SG) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
*** 
***  Copyright (c) 2023 United States Government as represented by the Administrator of the National Aeronautics and Space Administration.  All Rights Reserved. 
*** 
*** Disclaimer:
*** No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS." 
*** Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS AGREEMENT. 
***  (Please see the NOSA_19110.pdf file for more information.) 
*** 
********************************************************************************/
