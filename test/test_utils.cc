/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


// Unfinished

/*!
     Test utilities for gigatraj
*/

#include <stdlib.h>
#include "test_utils.hh"

using namespace gigatraj;

std::string datadir( std::string dir )
{
    std::string result;
    char *envdir;

    result = "";
    if ( dir != "" ) {
       envdir = getenv( dir.c_str() );
       if ( envdir != NULL ) {
          result = std::string( envdir ) + "/";
       }
    }

    return result;
}

   
int mismatch( gigatraj::real a, gigatraj::real b) 
{

    gigatraj::real diff;

    if ( ! (ISOK(a) && ISOK(b))  ) {
       return 1;
    }   
    
    diff = a - b;
    if (diff < 0.0 ) 
    {
       diff = - diff;
    }   

    if ( b < 0.0 )
    {
       b = - b;
    }
    if ( b > 0.0 ) 
    {   
       if ( diff/b*100.0 > 0.001 ) {
          return 1;
       } 
    } else {
       if ( diff > 0.0001 ) {
          return 1;
       } 
    }
   

    return 0;   

}

int mismatch( gigatraj::real a, gigatraj::real b, gigatraj::real tol ) 
{

    gigatraj::real diff;
    
    if ( ! (ISOK(a) && ISOK(b))  ) {
       return 1;
    }   
    
    diff = a - b;
    if (diff < 0.0 ) 
    {
       diff = - diff;
    }   

    if ( diff > tol ) {
       return 1;
    } 
    
    return 0;   

}


void cmp_pos(std::vector<gigatraj::Parcel> &vflock0, std::vector<gigatraj::Parcel> &vflock
     , gigatraj::real* max_d, gigatraj::real* min_d, gigatraj::real *avg_d, gigatraj::real *med_d, gigatraj::real *sprd_d )
{

   real minx,maxx,tot,var;
   real median;
   int n = vflock0.size();

   // first, compare the sizes of the two vectors
   if ( n != vflock.size() ) {
      std::cerr << "Mismatch on vector sizes!" << std::endl;
      exit(1);
   } 
   

   minx =  1.0e15;
   maxx = -1.0e15;
   tot =  0.0;
   var = 0.0;
   median = 0.0;
   
   std::vector<Parcel>::iterator vit = vflock.begin();
   std::vector<Parcel>::iterator vit0 = vflock0.begin();
   
   // get the planet
   PlanetNav *e = vit0->getNav();
   
   // set up a vector for the differences
   std::list<real> difs;
     
   while ( vit0 != vflock0.end() ) {
      real lat,lon,lat0,lon0;
      
      vit0->getPos(&lon0,&lat0);
      vit->getPos(&lon,&lat);
   
      real dst = e->distance(lon0,lat0, lon,lat);
      
      if ( maxx < dst ) {
         maxx = dst;
      }   
      if ( minx > dst ) {
         minx = dst;
      }   
   
      tot = tot + dst;

      difs.push_back(dst);
   
      vit0++;
      vit++;
   }
   
   difs.sort();
   
   tot = tot / n;
   
   std::list<real>::iterator lit = difs.begin();
   int i=0;
   while ( lit != difs.end() ) {
   
      if ( i == (n/2) ) {
         median = *lit;
      }
      if ( ((n % 2) == 1) & ( i == (n/2+1) ) ) {
         median = ( median + *lit )/2.0;
      }       
   
      var = var + ( *lit - tot )*( *lit - tot );
   
      lit++;
      i++;
   }
   
   *max_d = maxx;
   *min_d = minx;
   *avg_d = tot;
   *med_d = median;
   *sprd_d = SQRT( var / (n-1) );

} 

