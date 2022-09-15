
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"


#include "gigatraj/PGenRnd.hh"

using namespace gigatraj;


void PGenRnd::seed( const unsigned int *saw ) 
{
    rnd.seed(saw);
}


template< template<class U, class = std::allocator<U> > class Seq>
void PGenRnd :: init( Seq<Parcel>* seq
                    )                          
{
     seed();
     
     if ( seq->size() <= 0 ) {
        throw  (ParcelGenerator :: badparcelcount());
     }

     try {                                       
         typename Seq<Parcel>::iterator it;     
         it = seq->begin();
         while ( it != seq->end() ) {
             real lon, lat;
             lon = rnd.uniform(0.0, 359.9999);
             lat = ASIN( rnd.uniform(-1.0, 1.0) ) /RCONV;
             it->setPos( lon, lat );
          
             it++;
         }
    } catch (...) {
         throw (ParcelGenerator :: badgeneration());
    }
                                
     

};

Parcel * PGenRnd :: create_array(Parcel parcel, int n 
                                ) 
{
    Parcel* pa;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       seed();
       pa = new Parcel[n];
       // initialize the parcel value
       for (int i=0; i<n; i++ ) {
          real lon, lat;
          lon = (real)(rand() % 32768)/32767.0 * 359.9999;
          lat = ASIN( 2.0*(real)(rand() % 32768)/32767.0 - 1.0 ) /RCONV;
          pa[i] = parcel;
          pa[i].setPos( lon, lat );
       }
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     

    return pa;


}; 

std::vector<Parcel>* PGenRnd :: create_vector(Parcel parcel, int n 
                                             )
{
    std::vector<Parcel>*bunch;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::vector<Parcel>;
       bunch->reserve(n);
    
       for (int i=0; i<n; i++ ) {
          bunch->push_back(parcel);
       }
       
       PGenRnd::init(bunch);

    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};

std::list<Parcel>* PGenRnd :: create_list(Parcel parcel, int n 
                                         )
{
    std::list<Parcel>*bunch;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::list<Parcel>;
    
       for (int i=0; i<n; i++ ) {
          bunch->push_back(parcel);
       }

       PGenRnd::init(bunch);

    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};

std::deque<Parcel>* PGenRnd :: create_deque(Parcel parcel, int n 
                                           )
{
    std::deque<Parcel>*bunch;
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::deque<Parcel>;
    
       for (int i=0; i<n; i++ ) {
          bunch->push_back(parcel);
       }

       PGenRnd::init(bunch);

    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};


Flock* PGenRnd :: create_Flock(const Parcel& p, int n
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
        flock = new Flock( p, pgrp, n, r);

        // sync all the processors before we start loading
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }

        for ( ip=flock->begin(); ip != flock->end(); ip++ ) {
            try {
               // generate a random position
              lon = rnd.uniform(0.0, 359.9999);
              lat = ASIN( rnd.uniform(-1.0, 1.0) ) /RCONV;
               // load the parcel location into the parcel
              ip->setPos( lon, lat );

            } catch (std::ios::failure) {
               throw (ParcelGenerator :: badgeneration());
            }   
        }   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return flock;

};

Swarm* PGenRnd :: create_Swarm(const Parcel& p, int n
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
        swarm = new Swarm( p, pgrp, n, r);

        // sync all the processors before we start loading
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }

        for ( ip=swarm->begin(); ip != swarm->end(); ip++ ) {
            try {
               // generate a random position
              lon = rnd.uniform(0.0, 359.9999);
              lat = ASIN( rnd.uniform(-1.0, 1.0) ) /RCONV;
               // load the parcel location into the parcel
              ip->setPos( lon, lat );

            } catch (std::ios::failure) {
               throw (ParcelGenerator :: badgeneration());
            }   
        }   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return swarm;

};
