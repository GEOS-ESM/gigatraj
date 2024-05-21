
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


#include "gigatraj/PGenRndDisc.hh"

using namespace gigatraj;

PGenRndDisc::PGenRndDisc()
{
   lon0 = 0.0;
   lat0 = 0.0;
   rad = 50.0;
   thk = 1e-10;
      
   rnd.seed();
}

PGenRndDisc::PGenRndDisc( const real lon, const real lat, const real level, const real r, const real thickness )
{
   lon0 = lon;
   lat0 = lat;
   rad = r;
   thk = thickness;
   z0 = level;

   rnd.seed();
}

const void PGenRndDisc::center( real &lon, real &lat, real &level )   
{
   lon = lon0;
   lat = lat0;
   level = z0;
}


void PGenRndDisc::set_center( const real lon, const real lat, const real level ) 
{
   lon0 = lon;
   lat0 = lat;
   z0 = level;
}


const void PGenRndDisc::size( real &r, real &thickness )
{
   r = rad;
   thickness = thk;
}


void PGenRndDisc::set_size( const real r, const real thickness )
{
   rad = r;
   thk = thickness;
}


void PGenRndDisc::seed( const unsigned int *saw ) 
{
    rnd.seed(saw);
}

template< template<class U, class = std::allocator<U> > class Seq>
void PGenRndDisc :: init( Seq<Parcel>* seq
                    )                          
{
     PlanetNav *nav;
     real lon, lat, z;
     
     if ( seq->size() <= 0 ) {
        throw  (ParcelGenerator :: badparcelcount());
     }

     try {                                       
         typename Seq<Parcel>::iterator it;     
         it = seq->begin();
         while ( it != seq->end() ) {
             real lon, lat, lev, r, ang;
             
             nav = it->getNav();
             
             r = rad * SQRT( rnd.uniform(0.0,1.0) );
             ang =  360.0*rnd.uniform(0.0, 360.0);
             nav->displace( lon0,lat0, r,ang, lon,lat);
             
             z = z0 + thk/2.0*rnd.uniform(-1.0,1.0);

             it->setPos( lon, lat );
             it->setZ( z );
          
             it++;
         }
    } catch (...) {
         throw (ParcelGenerator :: badgeneration());
    }
                                
     

};

Parcel * PGenRndDisc :: create_array(Parcel parcel, int n 
                                ) 
{
    Parcel* pa;
    PlanetNav *nav;
     
    
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
     
    nav = parcel.getNav();
    
    try {

       pa = new Parcel[n];
       // initialize the parcel value
       for (int i=0; i<n; i++ ) {
          real lon, lat, z, r, ang;

          r = rad * SQRT( rnd.uniform(0.0,1.0) );
          ang =  360.0*rnd.uniform(0.0, 360.0);
          nav->displace( lon0,lat0, r,ang, lon,lat);
          
          z = z0 + thk/2.0*rnd.uniform(-1.0,1.0);             

          pa[i] = parcel;
          pa[i].setPos( lon, lat );
          pa[i].setZ( z );
       }
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     

    return pa;


}; 

std::vector<Parcel>* PGenRndDisc :: create_vector(Parcel parcel, int n 
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
       
       PGenRndDisc::init(bunch);

    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};

std::list<Parcel>* PGenRndDisc :: create_list(Parcel parcel, int n 
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

       PGenRndDisc::init(bunch);

    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};

std::deque<Parcel>* PGenRndDisc :: create_deque(Parcel parcel, int n 
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

       PGenRndDisc::init(bunch);

    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};


Flock* PGenRndDisc :: create_Flock(const Parcel& p, int n
                   , ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Flock *flock;
     Flock::iterator ip;         
     real lon,lat,z, rx, ang;
     int status = 0;
     PlanetNav *nav;
     

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

              nav = ip->getNav();
    
              rx = rad * SQRT( rnd.uniform(0.0,1.0) );
              ang =  360.0*rnd.uniform(0.0, 360.0);
              nav->displace( lon0,lat0, rx,ang, lon,lat);
               
              z = z0 + thk/2.0*rnd.uniform(-1.0,1.0);

               // load the parcel location into the parcel
              ip->setPos( lon, lat );
              ip->setZ( z );

            } catch (std::ios::failure) {
               throw (ParcelGenerator :: badgeneration());
            }   
        }   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return flock;

};

Swarm* PGenRndDisc :: create_Swarm(const Parcel& p, int n
                   , ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Swarm *swarm;
     Swarm::iterator ip;         
     real lon,lat,z, rx, ang;
     int status = 0;
     PlanetNav *nav;
     

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

              nav = ip->getNav();
    
              rx = rad * SQRT( rnd.uniform(0.0,1.0) );
              ang =  360.0*rnd.uniform(0.0, 360.0);
              nav->displace( lon0,lat0, rx,ang, lon,lat);
               
              z = z0 + thk/2.0*rnd.uniform(-1.0,1.0);

               // load the parcel location into the parcel
              ip->setPos( lon, lat );
              ip->setZ( z );

            } catch (std::ios::failure) {
               throw (ParcelGenerator :: badgeneration());
            }   
        }   
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return swarm;

};
