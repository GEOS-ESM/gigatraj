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
#include "gigatraj/Catalog.hh"

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
   Catalog* catlog;
   std::string cfgfile;
   std::string teststring;
   bool ok;
   int i;
   std::vector<Catalog::DataSource> destlist; 
   std::string datdir;
   
   datdir = datadir("srcdir"); 

   // first, test how confLocators are handled
   catlog = new Catalog();
   teststring = catlog->confLocator();
   if ( teststring != "" ) {
      cerr << "confLocator of a new Catalog is not null: " << teststring << endl;
      exit(1);
   }
   
   // can we find out config file?
   catlog->confLocator( "test_Catalog" );
   teststring = catlog->confLocator();
   if ( teststring != "test_Catalog" ) {
      cerr << "confLocator of a new Catalog is : " << teststring << endl;
      exit(1);   
   }
   
   cfgfile = catlog->findConfig();
   i = cfgfile.find_last_of('/');
   if ( i != std::string::npos ) {
      i = i + 1;
   } else {
      i = 0;
   }
   if ( cfgfile.substr(i) != "test_Catalog.cat" ) {
      cerr << "findCatalog() returned " << cfgfile << endl;
      exit(1);
   }

   // test that we can do debugging output
   catlog->debug( 100 );
   if ( catlog->debug() != 100 ) {
      cerr << "setting debug level to 100 failed: " << catlog->debug() << endl;
      exit(1);   
   }
   catlog->debug( 0 );

   //catlog->debug( 100 );

   // load the config file
   catlog->load( cfgfile );
   
   // check the names of the attributes
   teststring = catlog->attrName( 0 );
   if ( teststring != "attr1" ) {
      cerr << "got the wrong attribute name for #0: " << teststring << endl;
      exit(1);   
   }
   teststring = catlog->attrName( 2 );
   if ( teststring != "attr3" ) {
      cerr << "got the wrong attribute name for #2: " << teststring << endl;
      exit(1);   
   }
   
   i = catlog->attrIndex( "attr2" );
   if ( i != 1 ) {
      cerr << "got the wrong attribute id for attr2: " << i << endl;
      exit(1);   
   }
 
   // can we get variables with single literal values? 
   ok = catlog->variableValue( "litI01", teststring );
   if ( ! ok || ( teststring != "123" ) ) {
      cerr << "variableValue of litI01 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   teststring = "uhoh";
   ok = catlog->variableValue( "litI01", teststring );
   if ( ! ok || ( teststring != "123" ) ) {
      cerr << "2nd variableValue of litI01 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "litS01", teststring );
   if ( ! ok || ( teststring != "stringA" ) ) {
      cerr << "variableValue of litS01 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }

   // can get get variables with single references to literal values?
   ok = catlog->variableValue( "refI01", teststring );
   if ( ! ok || ( teststring != "123" ) ) {
      cerr << "variableValue of refI01 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   // (with formating?)
   ok = catlog->variableValue( "refI02", teststring );
   if ( ! ok || ( teststring != "00123" ) ) {
      cerr << "variableValue of refI02 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "refF03", teststring );
   if ( ! ok || ( teststring != " -123.46" ) ) {
      cerr << "variableValue of refF03 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "refS04", teststring );
   if ( ! ok || ( teststring != "ring" ) ) {
      cerr << "variableValue of refS04 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "refD05", teststring );
   if ( ! ok || ( teststring != "21-11" ) ) {
      cerr << "variableValue of refD05 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   // nested reference
   ok = catlog->variableValue( "refI06", teststring );
   if ( ! ok || ( teststring != "123" ) ) {
      cerr << "variableValue of refI06 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "refD07", teststring );
   if ( ! ok || ( teststring != "2017-02-17T01:30:00" ) ) {
      cerr << "variableValue of refD07 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "refS08", teststring );
   if ( ! ok || ( teststring != "123_bridge_123.456" ) ) {
      cerr << "variableValue of refS08 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }


   // full-out expressions
   ok = catlog->variableValue( "expI01", teststring );
   if ( ! ok || ( teststring != "53" ) ) {
      cerr << "variableValue of expI01 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "expI02", teststring );
   if ( ! ok || ( teststring != "35" ) ) {
      cerr << "variableValue of expI02 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "expI03", teststring );
   if ( ! ok || ( teststring != "247" ) ) {
      cerr << "variableValue of expI03 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "expF04", teststring );
   if ( ! ok || ( teststring != "53.645" ) ) {
      cerr << "variableValue of expF04 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "expI05", teststring );
   if ( ! ok || ( teststring != "57" ) ) {
      cerr << "variableValue of expI05 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "expS06", teststring );
   if ( ! ok || ( teststring != "foobar" ) ) {
      cerr << "variableValue of expS06 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "expD07", teststring );
   if ( ! ok || ( teststring != "2021-11-17T05:23:12" ) ) {
      cerr << "variableValue of expD07 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   
   // multiply-defined variables
   ok = catlog->variableValue( "varS01", teststring );
   if ( ! ok || ( teststring != "hello" ) ) {
      cerr << "variableValue of varS01 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "varS02", teststring );
   if ( ! ok || ( teststring != "right" ) ) {
      cerr << "variableValue of varS02 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }
   ok = catlog->variableValue( "varS03", teststring );
   if ( ! ok || ( teststring != "right" ) ) {
      cerr << "variableValue of varS03 failed ";
      if ( ok ) {
         cerr << ", returning '" << teststring << "'";
      }
      cerr << endl;
      exit(1);     
   }


   /// try lookups
//catlog->dump();
   ok = catlog->query( "stuff1", "2021-07-15T10:34", destlist, "20210714_00" );
   if ( ok  && destlist.size() != 2 ) {
      cerr << "lookup of stuff1 returned " << destlist.size() << " items" << std::endl;
      exit(1);     
   }
   if ( destlist[0].pre != "pattern_for_t01" ) {
      cerr << "lookup of stuff1 returned " << destlist[0].pre  << " as its first 'pre'" << std::endl;
      exit(1);        
   }
   if ( destlist[1].pre != "pattern_for_t02" ) {
      cerr << "lookup of stuff1 returned " << destlist[1].pre  << " as its second 'pre'" << std::endl;
      exit(1);        
   }
   if ( destlist[0].description != "this is desc01" ) {
      cerr << "lookup of stuff1 dest # 0 std name = " << destlist[0].description  << std::endl;
      exit(1);                 
   }
   if ( destlist[0].dims != 3 ) {
      cerr << "lookup of stuff1 dest # 0 dimensionality = " << destlist[0].dims  << std::endl;
      exit(1);                 
   }
   teststring = catlog->getAttr( destlist[0], "attr2" );
   if ( teststring != "at2valt01" ) {
      cerr << "lookup of stuff1 dest # 0 attribute 'attr2' = " << teststring  << std::endl;
      exit(1);           
   }

   // change the order of preference
   catlog->desired( "attr2", "at2valt04" );
   
   std::string qq01;
   std::string dd01;
   std::string mm01;
   qq01 = "stuff3";
   dd01 = "2021-07-15T10:34";
   mm01 = "20210714_00";
   ok = catlog->query( qq01, dd01, destlist, mm01 );
   //ok = catlog->query( "stuff3", "2021-07-15T10:34", destlist, "20210714_00" );
   if ( ok && destlist.size() != 2 ) {
      cerr << "lookup of stuff3 returned " << destlist.size() << " items" << std::endl;
      exit(1);     
   }
   if ( destlist[1].pre != "stuff3_2021071510_New_THIRD_20210714_00.data" ) {
      cerr << "lookup of stuff3 returned " << destlist[1].pre  << " as its first 'pre'" << std::endl;
      exit(1);        
   }
   if ( destlist[1].post != "stuff3_2021071511_New_THIRD_20210714_00.data" ) {
      cerr << "lookup of stuff3 returned " << destlist[1].post  << " as its first 'post'" << std::endl;
      exit(1);        
   }
   if ( destlist[1].dims != 2 ) {
      cerr << "lookup of stuff3 dest # 0 dimensionality = " << destlist[1].dims  << std::endl;
      exit(1);                 
   }
   if ( destlist[0].pre != "stuff3_2021071506_New_FOURTH.data" ) {
      cerr << "lookup of stuff3 returned " << destlist[0].pre  << " as its second 'pre'" << std::endl;
      exit(1);        
   }
   if ( destlist[0].post != "stuff3_2021071512_New_FOURTH.data" ) {
      cerr << "lookup of stuff3 returned " << destlist[0].post  << " as its second 'post'" << std::endl;
      exit(1);        
   }

catlog->debug( 100 );
   exit(0);

}
