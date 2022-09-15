/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


/*!
     Test program for the RandomSrc class
*/

#include <stdlib.h>     
#include <iostream>
#include <math.h>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/RandomSrc.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;


int main() 
{
   int bins[10];
   int i;
   int ntrials;
   int n;
   RandomSrc rnd;
   real val;
   real prob;
   
   
   ntrials = 10000;
   
   // make sure this works
   i = rnd.raw();
   n = rnd.raw();
   if ( i == n ) {
      cerr << "Two raw() calls in a row matched.  Unlikely" << endl;
      exit(1);
   }   
   

   // test the uniform distribution
   n = 0;
   for ( i=0; i<10; i++ ) {
      bins[i] = 0;
   }   
   for (i=1; i<=ntrials; i++ ) {
       val  = rnd.uniform(0.0,10.0);
       
       if ( val < 0.0 || val > 10.0 ) {
          cerr << "illegal random value: " << val << endl;
          exit;
       }
       if ( val < 10.0 ) {
          bins[ static_cast<int>(val) ] ++;
          n++;
       }
          
   } 
   // now calculate the probability for the number of items in each bin
   for ( i=0; i<10; i++ ) {
      // the numbers here were obtained by use of the IDL BINOMIAL() function.
      if ( bins[i] < 900 || bins[i] > 1094 ) {
         if ( bins[i] < 890 || bins[i] > 1114 ) {
            cerr << "Error: random (outside 99.99% prob) " << i << ": " << bins[i] << endl;
            exit(1);
         } else {   
            cerr << "Warning: random (outside 99.9% prob) " << i << ": " << bins[i] << endl;
         }
      }
   }
   
   exit(0);

}

   
