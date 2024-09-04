
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

#include "gigatraj/PGenGrid.hh"
#include "gigatraj/Earth.hh"

using namespace gigatraj;


// Use Earth as the default PlanetNav;
// (We are using this only for longitude wrapping calculations,
// so it should not matter is we end up using a different
// planets in the actual model run.
static Earth e;


int PGenGrid :: count_gridpoints( real beglon, real endlon, real deltalon
                           , real beglat, real endlat, real deltalat
                           , real begz, real endz, real deltaz 
                           )
{
     
     // sanity checking //
     if ( (  ( ( deltaz > 0.0 ) && ( begz <= endz ) ) 
          || ( ( deltaz < 0.0 ) && ( begz >= endz ) )
          || ( ( deltaz == 0.0) && ( begz == endz ) )
          ) and 
          (  ( ( deltalat > 0.0 ) && ( beglat <= endlat ) ) 
          || ( ( deltalat < 0.0 ) && ( beglat >= endlat ) )
          || ( ( deltalat == 0.0) && ( beglat == endlat ) ) 
          ) and
          (    ( deltalon != 0.0 ) 
          || ( ( deltalon == 0.0 ) && ( beglon == endlon ) ) 
          ) )  {
          
          // longitudes will always be OK, if they wrap
          real lon0 = e.wrap(beglon);
          real lon1 = e.wrap(endlon);
          while ( (deltalon > 0.0 ) && ( lon0 > lon1 ) ) {
             lon1 += e.fullcircle;
          }   
          while ( (deltalon < 0.0 ) && ( lon0 < lon1 ) ) {
             lon1 -= e.fullcircle;
          }   
          
          // now count the gridpoints
          int nz;
          if ( deltaz != 0.0 ) {
             nz = (int) ( (endz-begz)/deltaz ) + 1;
          } else {
             nz = 1;
          }
          int nlat;
          if ( deltalat != 0.0 ) {
             nlat = (int) ( (endlat-beglat)/deltalat ) + 1;
          } else {
             nlat = 1;
          }
          int nlon;
          if ( deltalon != 0.0 ) {
             nlon = (int) ( (lon1-lon0)/deltalon ) + 1;
          } else {
             nlon = 1;
          }
          return nz*nlat*nlon;          

     }
     
     return 0;

};



template< template<class U, class = std::allocator<U> > class Seq>
void PGenGrid :: initgrid( Seq<Parcel>* seq, const Parcel& p
                   , real beglon, real endlon, real deltalon
                   , real beglat, real endlat, real deltalat
                   , real begz, real endz, real deltaz  
                   )
{
     real z, lon, lat;
     int nz;
     int nlat;
     int nlon;
     real dlon, dlat, dz;
     
     // sanity checking //
     if ( (  ( ( deltaz > 0.0 ) && ( begz <= endz ) ) 
          || ( ( deltaz < 0.0 ) && ( begz >= endz ) )
          || ( ( deltaz == 0.0) && ( begz == endz ) )
          ) and 
          (  ( ( deltalat > 0.0 ) && ( beglat <= endlat ) ) 
          || ( ( deltalat < 0.0 ) && ( beglat >= endlat ) )
          || ( ( deltalat == 0.0) && ( beglat == endlat ) ) 
          ) and
          (    ( deltalon != 0.0 ) 
          || ( ( deltalon == 0.0 ) && ( beglon == endlon ) ) 
          ) )  {
          
          // longitudes will always be OK, if they wrap
          real lon0 = e.wrap(beglon);
          real lon1 = e.wrap(endlon);

          while ( (deltalon > 0.0 ) && ( lon0 > lon1 ) ) {
             lon1 += e.fullcircle;
          }   
          while ( (deltalon < 0.0 ) && ( lon0 < lon1 ) ) {
             lon1 -= e.fullcircle;
          } 
          
          // now count the gridpoints
          if ( deltaz != 0.0 ) {
             nz = (int) ( (endz-begz)/deltaz ) + 1;
             if ( nz < 2 ) {
                nz = 2;
             }
             dz = (endz - begz)/(nz - 1.0);
          } else {
             nz = 1;
             dz = 0.0;
          }
          if ( deltalat != 0.0 ) {
             nlat = (int) ( (endlat-beglat)/deltalat ) + 1;
             if ( nlat < 2 ) {
                nlat = 2;
             }
             dlat = (endlat - beglat)/(nlat - 1.0);
          } else {
             nlat = 1;
             dlat = 0.0;
          }
          if ( deltalon != 0.0 ) {
             nlon = (int) ( (lon1-lon0)/deltalon ) + 1;
             if ( nlon < 2 ) {
                nlon = 2;
             }
             dlon = (endlon - beglon)/(nlon - 1.0);
          } else {
             nlon = 1;
             dlon = 0.0;
          }
            
          try {
               typename Seq<Parcel>::iterator it;
               it = seq->begin();
               for ( int iz=0; iz < nz; iz++ ) {
                  z = begz + iz*dz;
                  for ( int ilat=0; ilat < nlat; ilat++ ) {
                     lat = beglat + ilat*dlat;
                     for ( int ilon=0; ilon < nlon; ilon++ ) {
                        lon = beglon + ilon*dlon;

                         *it = p;  // copy the input parcel's settings
                         it->setPos( lon, lat);  // set the horizontal position
                         it->setZ( z );  // set the vertical position
                         it++;  // next parcel                         
                     }
                  }
               }
          } catch (...) {
              throw (ParcelGenerator :: badgeneration());
          }
     } else {
         throw(ParcelGenerator :: badparcelcount()); 
     }
    

};



Parcel * PGenGrid :: create_array(const Parcel& p, int *np
                     , real beglon, real endlon, real deltalon
                     , real beglat, real endlat, real deltalat
                     , real begz, real endz, real deltaz 
                   )
{                                                                              
    Parcel* pa;                                                                
     real z, lon, lat;
     int nz;
     int nlat;
     int nlon;
     real dlon, dlat, dz;
                                                                               
    *np = -1;                                                                  
                                                                               
    int n = PGenGrid::count_gridpoints(beglon,endlon,deltalon                  
                                      ,beglat,endlat,deltalat                  
                                      ,begz,endz,deltaz);                      

    if ( n <= 0 ) {                                                            
       throw (ParcelGenerator :: badparcelcount());                            
    };                                                                         
                                                                               
    try {                                                                      
       pa = new Parcel[n];                                                     
       // initialize the parcel value                                          

       // longitudes will always be OK, if they wrap                           
       real lon0 = e.wrap(beglon);                                             
       real lon1 = e.wrap(endlon);                                             
       while ( (deltalon > 0.0 ) && ( lon0 > lon1 ) ) {                        
          lon1 += e.fullcircle;                                                
       }                                                                       
       while ( (deltalon < 0.0 ) && ( lon0 < lon1 ) ) {                        
          lon1 -= e.fullcircle;                                                
       }                                                                       
          
       // now count the gridpoints
       if ( deltaz != 0.0 ) {
          nz = (int) ( (endz-begz)/deltaz ) + 1;
          if ( nz < 2 ) {
             nz = 2;
          }
          dz = (endz - begz)/(nz - 1.0);
       } else {
          nz = 1;
          dz = 0.0;
       }
       if ( deltalat != 0.0 ) {
          nlat = (int) ( (endlat-beglat)/deltalat ) + 1;
          if ( nlat < 2 ) {
             nlat = 2;
          }
          dlat = (endlat - beglat)/(nlat - 1.0);
       } else {
          nlat = 1;
          dlat = 0.0;
       }
       if ( deltalon != 0.0 ) {
          nlon = (int) ( (lon1-lon0)/deltalon ) + 1;
          if ( nlon < 2 ) {
             nlon = 2;
          }
          dlon = (endlon - beglon)/(nlon - 1.0);
       } else {
          nlon = 1;
          dlon = 0.0;
       }
                                                                               
       try {                                                                   
            int i = 0;                                                         
            for ( int iz=0; iz < nz; iz++ ) {
               z = begz + iz*dz;
               for ( int ilat=0; ilat < nlat; ilat++ ) {
                  lat = beglat + ilat*dlat;
                  for ( int ilon=0; ilon < nlon; ilon++ ) {
                      lon = beglon + ilon*dlon;
                      pa[i] = p;  // copy the input parcel's settings          
                      pa[i].setPos( lon, lat);  // set the horizontal position 
                      pa[i].setZ( z );  // set the vertical position           
                      i++;  // next parcel                                     
                  }                                                            
               }                                                               
            }                                                                  
            *np = n;                                                           
       } catch (...) {                                                         
           throw (ParcelGenerator :: badgeneration());                         
       }                                                                       


    } catch(...) {                                                             
       throw ( ParcelGenerator :: badgeneration() );                           
    };                                                                         
    return pa;                                                                 


};                                                                             



std::vector<Parcel>* PGenGrid :: create_vector(const Parcel& p
                    , real beglon, real endlon, real deltalon
                    , real beglat, real endlat, real deltalat
                    , real begz, real endz, real deltaz 
                   )
{
    std::vector<Parcel> *bunch;

    int n = PGenGrid::count_gridpoints(beglon,endlon,deltalon
                                      ,beglat,endlat,deltalat
                                      ,begz,endz,deltaz);
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::vector<Parcel>;
       bunch->reserve(n);
    
       for (int i=0; i<n; i++ ) {
          bunch->push_back(p);
       }
       
       PGenGrid::initgrid( bunch, p
                         ,beglon,endlon,deltalon
                         ,beglat,endlat,deltalat
                         ,begz,endz,deltaz ); 
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};



std::list<Parcel>* PGenGrid :: create_list(const Parcel& p
                    , real beglon, real endlon, real deltalon
                    , real beglat, real endlat, real deltalat
                    , real begz, real endz, real deltaz 
                   )
{
    std::list<Parcel>*bunch;
    
    int n = PGenGrid::count_gridpoints(beglon,endlon,deltalon
                                      ,beglat,endlat,deltalat
                                      ,begz,endz,deltaz);
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::list<Parcel>;
    
       for (int i=0; i<n; i++ ) {
          bunch->push_back(p);
       }
       PGenGrid::initgrid( bunch, p
                         ,beglon,endlon,deltalon
                         ,beglat,endlat,deltalat
                         ,begz,endz,deltaz ); 
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};



std::deque<Parcel>* PGenGrid :: create_deque(const Parcel& p
                    , real beglon, real endlon, real deltalon
                    , real beglat, real endlat, real deltalat
                    , real begz, real endz, real deltaz 
                   )
{
    std::deque<Parcel>*bunch;
    
    int n = PGenGrid::count_gridpoints(beglon,endlon,deltalon
                                      ,beglat,endlat,deltalat
                                      ,begz,endz,deltaz);
    if ( n <= 0 ) {
       throw (ParcelGenerator :: badparcelcount());
    };  
    
    try {
       bunch = new std::deque<Parcel>;
    
       for (int i=0; i<n; i++ ) {
          bunch->push_back(p);
       }
       PGenGrid::initgrid( bunch, p
                         ,beglon,endlon,deltalon
                         ,beglat,endlat,deltalat
                         ,begz,endz,deltaz ); 
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     
    
    return bunch;

};


Flock* PGenGrid :: create_Flock(const Parcel& p
                   , real beglon, real endlon, real deltalon
                   , real beglat, real endlat, real deltalat
                   , real begz, real endz, real deltaz 
                   , ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Flock *flock;
     real lon,lat,z;
     int status = 0;
     int n;
     Parcel *pcl;
     int nz;
     int nlat;
     int nlon;
     real dlon, dlat, dz;

     n = PGenGrid::count_gridpoints(beglon,endlon,deltalon
                                  ,beglat,endlat,deltalat
                                  ,begz,endz,deltaz);
     if ( n <= 0 ) {
        throw (ParcelGenerator :: badparcelcount());
     };  

     try {
        // now create a Flock os that many parcels
        flock = new Flock( p, pgrp, n, r);

        // sanity checking //
        if ( (  ( ( deltaz > 0.0 ) && ( begz <= endz ) ) 
             || ( ( deltaz < 0.0 ) && ( begz >= endz ) )
             || ( ( deltaz == 0.0) && ( begz == endz ) )
             ) and 
             (  ( ( deltalat > 0.0 ) && ( beglat <= endlat ) ) 
             || ( ( deltalat < 0.0 ) && ( beglat >= endlat ) )
             || ( ( deltalat == 0.0) && ( beglat == endlat ) ) 
             ) and
             (    ( deltalon != 0.0 ) 
             || ( ( deltalon == 0.0 ) && ( beglon == endlon ) ) 
             ) )  {
             
             // longitudes will always be OK, if they wrap
             real lon0 = e.wrap(beglon);
             real lon1 = e.wrap(endlon);
             while ( (deltalon > 0.0 ) && ( lon0 > lon1 ) ) {
                lon1 += e.fullcircle;
             }   
             while ( (deltalon < 0.0 ) && ( lon0 < lon1 ) ) {
                lon1 -= e.fullcircle;
             } 
             // now count the gridpoints
             if ( deltaz != 0.0 ) {
                nz = (int) ( (endz-begz)/deltaz ) + 1;
                if ( nz < 2 ) {
                   nz = 2;
                }
                dz = (endz - begz)/(nz - 1.0);
             } else {
                nz = 1;
                dz = 0.0;
             }
             if ( deltalat != 0.0 ) {
                nlat = (int) ( (endlat-beglat)/deltalat ) + 1;
                if ( nlat < 2 ) {
                   nlat = 2;
                }
                dlat = (endlat - beglat)/(nlat - 1.0);
             } else {
                nlat = 1;
                dlat = 0.0;
             }
             if ( deltalon != 0.0 ) {
                nlon = (int) ( (lon1-lon0)/deltalon ) + 1;
                if ( nlon < 2 ) {
                   nlon = 2;
                }
                dlon = (endlon - beglon)/(nlon - 1.0);
             } else {
                nlon = 1;
                dlon = 0.0;
             }

             // sync all the processors before we start loading
             if ( pgrp != NULLPTR ) {
                pgrp->sync();
             }
             
             pcl = p.copy();
               
             try {
                  int i=0;
                  for ( int iz=0; iz < nz; iz++ ) {
                     z = begz + iz*dz;
                     for ( int ilat=0; ilat < nlat; ilat++ ) {
                        lat = beglat + ilat*dlat;
                        for ( int ilon=0; ilon < nlon; ilon++ ) {
                            lon = beglon + ilon*dlon;
                            
                            flock->sync();
                            
                            pcl->setPos( lon, lat);  // set the horizontal position
                            pcl->setZ( z );  // set the vertical position
                            
                            // If we are the root processor, the parcel pcl
                            // is valid, and it is sent to the processor
                            // to which it belongs.
                            // If we are not the root processor, then
                            // then pcl has no valid value, but the
                            // processor will ignore it and receive its
                            // parcel value from the root processor.
                            flock->set( i, *pcl, 0 );
                            
                            i++;
                            
                        }
                     }
                  }
             } catch (...) {
                 throw (ParcelGenerator :: badgeneration());
             }
             
             delete pcl;
             
        } else {
            throw(ParcelGenerator :: badparcelcount()); 
        }
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return flock;

};


Swarm* PGenGrid :: create_Swarm(const Parcel& p
                   , real beglon, real endlon, real deltalon
                   , real beglat, real endlat, real deltalat
                   , real begz, real endz, real deltaz 
                   , ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Swarm *swarm;
     real lon,lat,z;
     int status = 0;
     int n;
     Parcel *pcl;
     int nz;
     int nlat;
     int nlon;
     real dlon, dlat, dz;

     n = PGenGrid::count_gridpoints(beglon,endlon,deltalon
                                  ,beglat,endlat,deltalat
                                  ,begz,endz,deltaz);
     if ( n <= 0 ) {
        throw (ParcelGenerator :: badparcelcount());
     };  

     try {
        // now create a Swarm os that many parcels
        swarm = new Swarm( p, pgrp, n, r);

        // sanity checking //
        if ( (  ( ( deltaz > 0.0 ) && ( begz <= endz ) ) 
             || ( ( deltaz < 0.0 ) && ( begz >= endz ) )
             || ( ( deltaz == 0.0) && ( begz == endz ) )
             ) and 
             (  ( ( deltalat > 0.0 ) && ( beglat <= endlat ) ) 
             || ( ( deltalat < 0.0 ) && ( beglat >= endlat ) )
             || ( ( deltalat == 0.0) && ( beglat == endlat ) ) 
             ) and
             (    ( deltalon != 0.0 ) 
             || ( ( deltalon == 0.0 ) && ( beglon == endlon ) ) 
             ) )  {
             
             // longitudes will always be OK, if they wrap
             real lon0 = e.wrap(beglon);
             real lon1 = e.wrap(endlon);
             while ( (deltalon > 0.0 ) && ( lon0 > lon1 ) ) {
                lon1 += e.fullcircle;
             }   
             while ( (deltalon < 0.0 ) && ( lon0 < lon1 ) ) {
                lon1 -= e.fullcircle;
             } 
             // now count the gridpoints
             if ( deltaz != 0.0 ) {
                nz = (int) ( (endz-begz)/deltaz ) + 1;
                if ( nz < 2 ) {
                   nz = 2;
                }
                dz = (endz - begz)/(nz - 1.0);
             } else {
                nz = 1;
                dz = 0.0;
             }
             if ( deltalat != 0.0 ) {
                nlat = (int) ( (endlat-beglat)/deltalat ) + 1;
                if ( nlat < 2 ) {
                   nlat = 2;
                }
                dlat = (endlat - beglat)/(nlat - 1.0);
             } else {
                nlat = 1;
                dlat = 0.0;
             }
             if ( deltalon != 0.0 ) {
                nlon = (int) ( (lon1-lon0)/deltalon ) + 1;
                if ( nlon < 2 ) {
                   nlon = 2;
                }
                dlon = (endlon - beglon)/(nlon - 1.0);
             } else {
                nlon = 1;
                dlon = 0.0;
             }

             // sync all the processors before we start loading
             if ( pgrp != NULLPTR ) {
                pgrp->sync();
             }
             
             pcl = p.copy();
               
             try {
                  int i=0;
                  for ( int iz=0; iz < nz; iz++ ) {
                     z = begz + iz*dz;
                     for ( int ilat=0; ilat < nlat; ilat++ ) {
                        lat = beglat + ilat*dlat;
                        for ( int ilon=0; ilon < nlon; ilon++ ) {
                            lon = beglon + ilon*dlon;
                            
                            swarm->sync();
                            
                            pcl->setPos( lon, lat);  // set the horizontal position
                            pcl->setZ( z );  // set the vertical position
                            
                            // If we are the root processor, the parcel pcl
                            // is valid, and it is sent to the processor
                            // to which it belongs.
                            // If we are not the root processor, then
                            // then pcl has no valid value, but the
                            // processor will ignore it and receive its
                            // parcel value from the root processor.
                            swarm->set( i, *pcl, 0 );
                            
                            i++;
                            
                        }
                     }
                  }
             } catch (...) {
                 throw (ParcelGenerator :: badgeneration());
             }
             
             delete pcl;
             
        } else {
            throw(ParcelGenerator :: badparcelcount()); 
        }
     
    } catch(...) {
       throw ( ParcelGenerator :: badgeneration() );
    };     


    return swarm;

};
