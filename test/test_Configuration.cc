/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
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

   
