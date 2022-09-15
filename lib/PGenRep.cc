
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/PGenRep.hh"

using namespace gigatraj;


Parcel * PGenRep :: create_array(Parcel parcel, int n ) 
{
    Parcel* pa;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       pa = new Parcel[n];
       // initialize the parcel value
       for (int i=0; i<n; i++ ) {
          pa[i] = parcel;
       }
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     

    return pa;


}; 

std::vector<Parcel>* PGenRep :: create_vector(Parcel parcel, int n )
{
    std::vector<Parcel>*flock;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       flock = new std::vector<Parcel>;
       flock->reserve(n);
    
       for (int i=0; i<n; i++ ) {
          flock->push_back(parcel);
       }
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return flock;

};

std::list<Parcel>* PGenRep :: create_list(Parcel parcel, int n )
{
    std::list<Parcel>*flock;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       flock = new std::list<Parcel>;
    
       for (int i=0; i<n; i++ ) {
          flock->push_back(parcel);
       }
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return flock;

};

std::deque<Parcel>* PGenRep :: create_deque(Parcel parcel, int n )
{
    std::deque<Parcel>*flock;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       flock = new std::deque<Parcel>;
    
       for (int i=0; i<n; i++ ) {
          flock->push_back(parcel);
       }
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return flock;

};



Flock* PGenRep :: create_Flock(const Parcel& p, int n
                   , ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Flock *flock;
     Flock::iterator ip;         
     real lon,lat,z;
     int status = 0;

     if ( n <= 0 ) {
        throw (ParcelGenerator :: badparcelcount());
     };  
    
     try {
     
        // now create a Flock os that many parels
        // this automatically replicates the example Parcel into the members of the Flock
        flock = new Flock( p, pgrp, n, r);

        // sync all the processors
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }
   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return flock;

};


Swarm* PGenRep :: create_Swarm(const Parcel& p, int n
                   , ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Swarm *swarm;
     Swarm::iterator ip;         
     real lon,lat,z;
     int status = 0;

     if ( n <= 0 ) {
        throw (ParcelGenerator :: badparcelcount());
     };  
    
     try {
     
        // now create a Swarm os that many parels
        // this automatically replicates the example Parcel into the members of the Swarm
        swarm = new Swarm( p, pgrp, n, r);

        // sync all the processors
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }
   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return swarm;

};
