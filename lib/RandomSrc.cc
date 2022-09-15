
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include <fstream>
#include <ctime>
#include <cstdlib>

#include "gigatraj/RandomSrc.hh"

using namespace gigatraj;


RandomSrc::RandomSrc() 
{
    unsigned int saw;
    
    rdev = "/dev/random";
    
    saw = genSeed();
    
    srand(saw);
    
};

RandomSrc::RandomSrc( unsigned int saw )
{
   rdev = "/dev/random";
    
   srand(saw);
}

RandomSrc::RandomSrc( const std::string dev )
{
    unsigned int saw;
    
    rdev = dev;
    
    saw = genSeed();
    
    srand(saw);

}


RandomSrc::~RandomSrc() {

}


real RandomSrc::uniform( real min, real max ) 
{
    real result;
    
    
    result = (real)(rand() % 32768)/32767.0;
    result = result * (max-min) + min;

    return result;
}       

int RandomSrc::raw()
{
   return rand();
}   

unsigned int RandomSrc::genSeed()
{
    unsigned int saw;

    try {
       // first, try to read the seed in from /dev/random
       
       // make a char array the size of "saw"
       char cc[sizeof(saw)+1];
       // open /dev/random
       std::ifstream r(rdev);
       // read those N bytes of randomness
       r.get(cc, sizeof(saw));
       r.close();
       // play with pointers to transfer the bytes just read to the seed
       char *cin = cc;
       char *cout = (char *)(&saw);
       for (int i=0; i<sizeof(saw); i++ ) {
           *cout = *cin;
           cin++;
           cout++;
       }
    } catch (...) {
       // or just get it from the time
       std::cerr << "no " << rdev << " access" << std::endl;
       saw = (unsigned int) time(NULLPTR);
    }
    
    return saw;

}

void RandomSrc::seed( const unsigned int *saw )
{
     unsigned int saw2;
     
     if ( saw == NULL ) {
        saw2 = genSeed();
     } else {
        saw2 = *saw;
     }   

     srand(saw2);

}

