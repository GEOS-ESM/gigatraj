
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/ParcelGenerator.hh"

using namespace gigatraj;

/*!
creates an array of parcels
*/
Parcel * ParcelGenerator :: create_array(int n)
{
    Parcel* pa;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       pa = new Parcel[n];
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     

    return pa;

};


/*!
creates an vector container of parcels
*/
std::vector<Parcel>* ParcelGenerator :: create_vector( int n )
{
    std::vector<Parcel>*bunch;
    Parcel parcel;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::vector<Parcel>;
       bunch->reserve(n);
    
       for (int i=0; i<n; i++ ) {
          bunch->push_back(parcel);
       }
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};

/*!
creates an list container of parcels
*/
std::list<Parcel>* ParcelGenerator :: create_list( int n )
{
    std::list<Parcel>*bunch;
    Parcel parcel;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::list<Parcel>;
    
       for (int i=0; i<n; i++ ) {
          bunch->push_back(parcel);
       }
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};

/*!
creates an deque container of parcels
*/
std::deque<Parcel>* ParcelGenerator :: create_deque( int n )
{
    std::deque<Parcel>*bunch;
    Parcel parcel;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::deque<Parcel>;
    
       for (int i=0; i<n; i++ ) {
          bunch->push_back(parcel);
       }
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};



Flock* ParcelGenerator :: create_Flock( int n
                   , ProcessGrp* pgrp, int r
                   )                        
{
     Parcel p;
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
        pgrp->sync();
   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return flock;

};



Swarm* ParcelGenerator :: create_Swarm( int n
                   , ProcessGrp* pgrp, int r
                   )                        
{
     Parcel p;
     // the parcel container
     Swarm *swarm;
     Swarm::iterator ip;         
     real lon,lat,z;
     int status = 0;

     if ( n <= 0 ) {
        throw (ParcelGenerator :: badparcelcount());
     };  
    
     try {
     
        // now create a Swarm of that many parcels
        // this automatically replicates the example Parcel into the members of the Swarm
        swarm = new Swarm( p, pgrp, n, r);

        // sync all the processors 
        pgrp->sync();
   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return swarm;

};
