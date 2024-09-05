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
     Test program for the Configuration class
*/

#include <stdlib.h>     
#include <iostream>
#include <math.h>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Configuration.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cout;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::istringstream;
using std::operator<<;
using std::operator>>;


int main(int argc, char * const argv[]) 
{
   Configuration conf;
   std::string aaa,bbb,ccc,ddd,eee;
   bool doHelp;
   bool testBool;
   int testInt;
   real testReal;
   int xarg;
   

    conf.add("conf1", cConfig, "test_config_01.cfg", "x", 0, "config file" );
   
    conf.add("conf2", cConfig, "test_config_02.cfg", "y", 0, "extra config file" );
    conf.add("conf3", cConfig, "nonexistent.cfg"   , "z", 0, "optional ocnfig file" );

    // register these parameters with the configuration object
    conf.add("AAA" , cBoolean, "N"      , "a" );
    conf.add("BBB" , cBoolean, "N"      , "b", 0, " something bold" );
    conf.add("CCC" , cInt    , "99"     , "", 0, "something cold"  );
    conf.add("DDD" , cFloat  , "99.99"  , "d", 0, "something to dream" );
    conf.add("EEE" , cString , "gamma"  , "e", 0, "something extreme" );
    conf.add("help", cBoolean , "N"  , "h", 0, "prints help text" );
   
    // test these parameter values now, before we overwrite any
    conf.fetchParam( "BBB", testBool );
    if ( testBool ) {
       cerr << " Bad Boolean retrieval for BBB: True " << endl;
       exit(1);
    }   
    conf.fetchParam( "CCC", testInt );
    if ( testInt != 99 ) {
       cerr << " Bad Int retrieval for CCC: " << testInt << endl;
       exit(1);
    }   
    conf.fetchParam( "DDD", testReal );
    if ( mismatch( testReal, 99.99 ) ) {
       cerr << " Bad Real retrieval for DDD: " << testReal << endl;
       exit(1);
    }   
     
    // load the parameters from files and/or the command line options
    xarg = conf.load(argc,argv);
   
    conf.fetchParam("help", doHelp );
    if ( doHelp ) {
       conf.help( "Help text:", "end of help");
    }
   
   
    aaa = conf.get("AAA");
    bbb = conf.get("BBB");
    ccc = conf.get("CCC");
    ddd = conf.get("DDD");
    eee = conf.get("EEE");
    
    cout << "AAA=" << aaa << endl;
    cout << "BBB=" << bbb << endl;
    cout << "CCC=" << ccc << endl;
    cout << "DDD=" << ddd << endl;
    cout << "EEE=" << eee << endl;
    
    //if ( xarg > 0 ) {
    //   cout << "arg 1: " << argv[xarg] << endl;
    //}
   
   
   conf.clear();
    
   exit(0);

}

   
