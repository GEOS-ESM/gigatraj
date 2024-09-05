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

   
