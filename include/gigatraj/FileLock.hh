
#ifndef GIGATRAJ_FILELOCK_H
#define GIGATRAJ_FILELOCK_H

#include <string>
#include <istream>
#include <fstream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/RandomSrc.hh"

// assume Unix (or Unix-flavored) unless otherwise specified
#ifndef OSTYPE
#define OSTYPE UNIX
#endif

namespace gigatraj {

/*!
\ingroup misc
\brief FileLock provides a simple facility for file-locking

Especially in a multiprocessing environment, 
there are cases in which one process will attempt to open a file for reading or writing
at the same time that other processes want to open the same file.
To prevent such processes from interfering with each other, some
kind of file locking mechanism is desired.  The FileLock class is a simple
implementation of such a mechanism.


*/

class FileLock {


   public:

      /// Error: file is already locked
      class badLockExists {};

      /// Error: failed to lock file
      class badLockFailed {};
      
      /// Error: failed to lock file
      class badLockFailed_A {};
      
      /// Error: failed to lock file
      class badLockFailed_B {};
      
      /// Error: failed to remove lock
      class badLockRmFailed {};
      
      /// Error: file not writeable
      class badNotWriteable {};

      /// Error: file not readable
      class badNotReadable {};

      /// Error: lock not readable
      class badLockIOErr {};

      /// basic constructor
      /*! This is the basic constructor for the FileLock class.
      */
      FileLock();

      /// constructor with a seed for the pseudorandom number generator
      /*! This version of the constructor permits setting the pseudorandom number
          generator.

           \param seed if nonzero, this is used as the seed,  If zero, 
                  an internal seed is generated
      */    
      FileLock( int seed );

      /// destructor
      /*! This is the destructor for the FileLock class.
      */
      ~FileLock();
      
      /// initialize the pseudorandom number generator
      /*! FileLock uses pseudorandom numbers
          to help resolve conflicts with simultaneous access attempts.
          This function initializes that generator.
          
           \param seed if nonzero, this is used as the seed,  If zero, 
                       an internal seed is generated
      */
      void init( int seed=0 );
      
      /// attempts to open a file for writing
      /*! This function opens a file for writing with an exclusive lock.
          The locking method is a simple but portable creation of a lock file
          (an empty file that is created by opening it for writing with locking).
          This is far from foolproof, but it should suffice for our purposes.
          Note, howver, that the lock file is in the same path as the file being
          opened; it just has the string "._lock" appended; this the user must
          has write access to the directory in which the file resides.
      
          \param filename the name of the file to be opened
          \param flag  if 0, then the method attempts an exclusive lock 
                       and throws an exception if it fails to do so.
                       If >0, then the method attempts an exclusive lock
                       and keeps trying at most \p flag times until it gets one.
                       If -1, then the method opens the file without any locking,
                       but only if the file is not already locked.
      */
      std::ofstream* openw( const std::string filename, int flag = 0);    

      /// attempts to open a file for reading
      /*! This function opens a file for reading, subject to file locking.
      
          \return a pointer to a new ifstream object from which data may be read.
                  This pointer should be specified as the argument in the closer() method,
                  where the ifstream will be closed and deleted.  
          \param filename the name of the file to be opened
          \param flag  if 0, then the method attempts an exclusive lock 
                       and throws an exception if it fails to do so.
                       If >0, then the method attempts an exclusive lock
                       and keeps trying at most \p flag times until it gets one.
                       If -1, then the method locks the file, opens it, and
                       then releases the lock immediately.  This is useful
                       only for files that are written once but then read
                       by many processes.
      */
      std::ifstream* openr( const std::string filename, int flag = 0 );    

      /// closes an output file stream, releasing any lock that was created
      /*! This function closes the file and frees the lock. It also deletes 
          the ofstream object.
      */
      void closer(std::ofstream* fyle);

      /// closes an input file stream, releasing any lock
      /*! This function closes the file and frees the lock. 
          It also deletes the ifstream object.
      */
      void closer(std::ifstream* fyle);


      /// holds an id number
      int id;
      
      /// turns on debugging messages
      int debug;

   private:
      /// the name of the lock file
      std::string lockname;
#if OSTYPE == UNIX
      /// the file handle of the lock
      int lockid;
      /// random number generator
      RandomSrc* rnd;
#endif
      /// waits for period of time before a retry
      void wayt();
      
      
      /// 

};

};

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
