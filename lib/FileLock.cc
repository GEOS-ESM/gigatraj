

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


#include "config.h"

#include "gigatraj/FileLock.hh"

#include <ctime>

#if OSTYPE == UNIX

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif

#endif

#include <cerrno>
#include <cstdio>

using namespace gigatraj;

FileLock::FileLock()
{
   lockname = "";
   
#if OSTYPE == UNIX
   lockid = -1;
#endif

   debug = 0;
   id = 0;
   rnd = NULLPTR;

   init(0);

}

FileLock::FileLock(int seed)
{
   lockname = "";
   
#if OSTYPE == UNIX
   lockid = -1;
#endif

   debug = 0;
   id = 0;

   init(seed);

}

FileLock::~FileLock()
{
   if ( rnd != NULLPTR ) {
      delete rnd;
   }
}

void FileLock::init( int seed ) 
{


   if  (seed == 0 ) {
      // don't bother drawing down the entropy pool
      // just for the simple, non-critical pseudorandom
      // numbers we need.  Just create a seed from the time,
      // but xor that with the (bit-shifted) ID for good measure.
      seed = static_cast<unsigned int>( time(NULL) ) ^ (id << 16);
   }

   //std::cerr << "seed = " << seed << std::endl;
   if ( rnd != NULLPTR ) {
      delete rnd;
   }
   rnd = new RandomSrc( seed );
   
}

std::ofstream* FileLock::openw( const std::string filename, int flag )
{
   // the iostream object for the file
   std::ofstream* fyle;
   // status of the lock:
   //    0 = failed for some other reason than locked
   //    1 = succeeded
   //   -1 = giving up after failing (for being locked) for too many tries
   // -999 = the file is locked, but we are still trying
   int ok;
   // status of the open
   bool fine;
   // maximum number of times to try to open the file
   int maxtries;
   // counter for the number of tries to open the file
   int trynumber;
   
   // todo: split the file name up into path and base names,
   // then prepend ".__" to the base before reassembling.
   // for now, just append this suffix
   lockname = filename + "._lock";

   ok = -999;
   
   // how many times do we want to try the open?
   maxtries = 1;
   if ( flag < 0 ) {
      maxtries = - flag;
   }   
   if ( flag > 0 ) {
      maxtries = flag;
   }
   
   // begin
   trynumber = 1;
#if OSTYPE == UNIX
   /// while we have not yet succeeded....
   while ( ok == -999 ) {
      // try to open the lock file with a Unix-supplied
      // exclusive lock
      lockid=open(lockname.c_str()
                 , O_WRONLY | O_CREAT | O_EXCL
                 , S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);  
      if ( lockid < 0 && errno == EEXIST ) {
         // opened failed.
         
         if ( trynumber < maxtries ) {
            // wait a bit and try again
            wayt(); 
                       
            trynumber ++;
         } else {
            // give up
            ok = -1;
         }
      } else if ( lockid < 0 ) {
         // the opened failed, but not because the file was locked
         ok = 0;
      } else {
         // the open was successful
         ok= 1;
         // now change permissions to none
         // so that no one else can open this lock file.
         fchmod( lockid, 0 );
      }
   }
#endif

   // if the lock file was opened successfully....
   if ( ok == 1 ) {
      fine = true;
      // now open the output file
      try {
         fyle = new std::ofstream(filename.c_str(), std::ios::out | std::ios::binary );
      } catch (...) {
         fine = false;
      }
      
      // Did the open fail?
      if ( ! fine || fyle->fail() ) {   
         
         // release the lock
         if ( remove(lockname.c_str()) != 0 ) {
            throw (badLockRmFailed());
         }
         if (close(lockid)) {
            throw (badLockIOErr());
         }
         lockid = -1;
         throw (badNotWriteable());
      }
   } else if ( ok == -1 ) {    
      // file is already locked
      throw (badLockExists());
   } else { 
      // some other error
      throw (badLockFailed_A());
   }


   return fyle;

}


std::ifstream* FileLock::openr( const std::string filename, int flag )
{
   // the iostream object for the file
   std::ifstream* fyle;
   // status of the lock:
   //    0 = failed for some other reason than locked
   //    1 = succeeded
   //   -1 = giving up after failing (for being locked) for too many tries
   // -999 = the file is locked, but we are still trying
   int ok;
   // status of the open
   bool fine;
   // maximum number of times to try to open the file
   int maxtries;
   // counter for the number of tries to open the file
   int trynumber;
   // to hold the file stat
   struct stat filestats;

   
   // todo: split the file name up into path and base names,
   // then prepend ".__" to the base before reassembling.
   // for now, just append this suffix
   lockname = filename + "._lock";

   ok = -999;
   
   // how many times do we want to try the open?
   maxtries = 1;
   if ( flag < 0 ) {
      maxtries = - flag;
   }
   if ( flag > 0 ) {
      maxtries = flag;
   }   

   // begin
   trynumber = 1;
#if OSTYPE == UNIX
   // check that the fle exists
   if ( stat( filename.c_str(), &filestats ) == 0 ) {
      /// while we have not yet succeeded....
      while ( ok == -999 ) {
         if ( debug ) {
            std::cerr << id << ": trying read-lock, attempt " 
            << trynumber << " of " << maxtries << std::endl;
         }
         // try to open the lock file with an exclusive lock.
         lockid=open(lockname.c_str()
                 , O_WRONLY | O_CREAT | O_EXCL
                 , S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);  
         if ( lockid < 0 && errno == EEXIST ) {
            // lock exists.
            
            // try again later?
            if ( trynumber < maxtries ) {

               wayt();
                           
               trynumber ++;
            } else {
               // no more tries; we give up
               if ( debug ) {
                   std::cerr << ": lock file failed for already exiting. Giving up " << std::endl; 
               }
               ok = -1;
            }
         } else if ( lockid < 0 ) {
            // open failed for some other reason
            ok = 0;
               if ( debug ) {
                  std::cerr << ": lock file failed to be created for some weird reason  " << errno << std::endl; 
               }
         } else {
            // lock formed OK
               if ( debug ) {
                  std::cerr << ": lock file was created OK " << std::endl; 
               }
            ok= 1;
         }
      }
   } else {
      if ( debug ) {
         std::cerr << id << ": cache file does not exist " << std::endl; 
      }
      ok= -2;
   }
#endif
   if ( debug ) {
      std::cerr << id << ": read-lock final status is " 
      << ok << std::endl; 
   }

   // if the lock file was opened successfully....
   if ( ok == 1 ) {
      // open the input file
      fine = true;
      if ( debug ) {
         std::cerr << id << ": attempting to open the data file " << std::endl; 
      }
      try {
         fyle = new std::ifstream(filename.c_str(), std::ios::in | std::ios::binary );
      } catch (...) {
         fine = false;
         std::cerr << id << ": FAILED to open the data file " << std::endl; 
      }      
      
      // Did the open fail?
      if ( ! fine || fyle->fail() ) {   
         // release the lock
         if ( remove(lockname.c_str()) != 0 ) {
            throw (badLockRmFailed());
         }
         if (close(lockid)) {
            throw (badLockIOErr());
         }
         lockid = -1;
         if ( debug ) {
            std::cerr << id << ": failed to open the file: " << fine << "/"
             << fyle->fail() << "/" << fyle->eof() << "/" << fyle->good() 
             << std::endl; 
         }
         delete fyle;
         throw (badNotReadable());
      }
   } else if ( ok == -1 ) {    
      // file is already locked
      if ( debug ) {
         std::cerr << "FileLock::openr: file is already locked" << std::endl;
      }
      throw (badLockExists());
   } else if ( ok == -2 ) {    
      // file is not readable
      if ( debug ) {
         std::cerr << "FileLock::openr: file is not readable" << std::endl;
      }
      throw (badNotReadable());
   } else { 
      // some other error
      if ( debug ) {
         std::cerr << "FileLock::openr: file is has some weird error" << std::endl;
      }
      throw (badLockFailed_B());
   }

   if ( flag < 0 ) {
      if ( debug ) {
         std::cerr << id << ": file open for reading; removing the lock " << std::endl; 
      }
      if ( remove(lockname.c_str()) != 0 ) {
         if ( debug ) {
            std::cerr << "FileLock::openr: Could not remove lock on file " << std::endl;
         }
         throw (badLockRmFailed());
      }
      if (close(lockid)) {
         if ( debug ) {
            std::cerr << "FileLock::openr: Could not close lock on file " << std::endl;
         }
         throw (badLockIOErr());
      }
      lockid = -1;
   }

   return fyle;

}

void FileLock::closer(std::ofstream* fyle)
{
    // close the iostream
    fyle->close();
    // and delete it
    delete fyle;
    
#if OSTYPE == UNIX
    // valid open lock file handle?
    if ( lockid >= 0 ) {
       // remove the lock file
       if ( remove(lockname.c_str()) != 0 ) {
          throw (badLockRmFailed());
       }
       // and close it
       if (close(lockid)) {
          throw (badLockIOErr());
       }
       lockid = -1;
    }

#endif

}


void FileLock::closer(std::ifstream* fyle)
{
    // close the iostream
    fyle->close();
    // and delete it
    delete fyle;
    
#if OSTYPE == UNIX
    // valid open lock file handle?
    if ( lockid >= 0 ) {
       // remove the lock file
       if ( remove(lockname.c_str()) != 0 ) {
          throw (badLockRmFailed());
       }
       // and close it
       if (close(lockid)) {
          throw (badLockIOErr());
       }
       lockid = -1;
    }
       
#endif

}

void FileLock::wayt()
{
   // timespec comes from the time.h header
#ifdef HAVE_TIME_H
   // the time to wait
   timespec waittime;
   // unused placeholder
   timespec junktime;
   // return code from naosleep (ignored)
   int slpcode;
#else
#ifdef HAVE_UNISTD_H
   unsigned int waittime;
#endif
#endif

     
#ifdef HAVE_TIME_H
     // set the time-to-sleep from random number, bit-masked
     // to reduce the range of possible sleep times
     waittime.tv_sec = 0;
     waittime.tv_nsec = rnd->raw() & 0x0fffffff; 
     if ( debug ) {
        std::cerr << id << ": lock exists; sleeping for  " 
        << waittime.tv_nsec << " ns " << std::endl;
     }
     // sleep for this random time
     slpcode = nanosleep( &waittime, &junktime );
#else
#ifdef HAVE_UNISTD_H
     /* This is the fallback, if nanosleep is not available
        but usleep is.
     */   
     // generate a random number of milliseconds. 
     waittime = rnd->raw() & 0x03ffff;
     if ( debug ) {
        std::cerr << id << ": read-lock failed. Sleeping for " 
        << waittime << std::endl; 
     }
     // sleep for this random time    
     (void) usleep( waittime );

#else

     /* This is the fallback to the fallback, it neither
        nanosleep nor usleep is available
        BUT: this will be very slow, since it waits
        between 0 and 7 *seconds*!
     */      
    
     // generate a random number of seconds between 0 and 7. 
     slptime = rnd->raw() & 0x07;
     if ( debug ) {
        std::cerr << id << ": read-lock failed. Sleeping for " 
        << slptime << std::endl; 
     }
     // sleep for this random time    
     (void) sleep( slptime );
#endif
#endif

}


    
