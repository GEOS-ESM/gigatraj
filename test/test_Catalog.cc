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
   std::vector<float> *dvals;
   std::string dquant;
   std::string dunits;
   
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
   
   // findConfig() looks in the current directory first,
   // then in $GIGATRAJ_CATALOGS,
   // and then in the directory denoted by GTCONFIGDIR in the code.
   // Thus, the cfgfile returns might have a directory prepsneded to the
   // base file name. If so, remove it for the check.
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

   // load the config file
   catlog->load( cfgfile );
   
   // check the first and last dates
   if ( catlog->first_date != "1900-01-01T00:00" ) {
      cerr << "got the wrong first date: '" << catlog->first_date << "'" << endl;
      exit(1);      
   }
   if ( catlog->final_date != "2023-01-01T00:00" ) {
      cerr << "got the wrong final date: '" << catlog->final_date << "'" << endl;
      exit(1);      
   }
   
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
   ok = catlog->variableValue( "expS09", teststring );
   if ( ! ok || ( teststring != "obag27" ) ) {
      cerr << "variableValue of expS09 failed ";
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

   // dimensions
   if ( catlog->dimensionValues( "nonexistent", dquant, dunits, &dvals ) ) {
      cerr << "undefined dimension appears to be defined. ";
      exit(1);
   }
   if ( catlog->dimensionValues( "dim01", dquant, dunits, &dvals ) ) {
      if ( dquant != "lon" ) {
         cerr << "dim01 quant is " << dquant<< " vs. 'lon'" << endl;
         exit(1);
      }
      if ( dunits != "degrees_east" ) {
         cerr << "dim01 units are " << dunits << " vs. 'degrees_east'" << endl;
         exit(1);
      }
      if ( dvals->size() != 360 ) {
         cerr << "dim01 values are the wrong size: " << dvals->size() << " vs " << 360 << endl;
         exit(1);     
      }
      if ( mismatch( (*dvals)[0], -180.0) ) {
         cerr << "dim01 value[0]: " << (*dvals)[0] << " vs " << -180.0 << endl;
         exit(1);     
      }
      if ( mismatch( (*dvals)[1] - (*dvals)[0], 1.0) ) {
         cerr << "dim01 delta: " << (*dvals)[1] - (*dvals)[0] << " vs " << 1.0 << endl;
         exit(1);     
      }
      
      delete dvals;
      
   } else {
      cerr << "defined dimension appears to be undefined. ";
      exit(1);
   }
   if ( catlog->dimensionValues( "dim02", dquant, dunits, &dvals ) ) {
      if ( dquant != "lat" ) {
         cerr << "dim02 quant is " << dquant<< " vs. 'lat'" << endl;
         exit(1);
      }
      if ( dunits != "degrees_north" ) {
         cerr << "dim02 units are " << dunits << " vs. 'degrees_north'" << endl;
         exit(1);
      }
      if ( dvals->size() != 181 ) {
         cerr << "dim02 values are the wrong size: " << dvals->size() << " vs " << 181 << endl;
         exit(1);     
      }
      if ( mismatch( (*dvals)[0], -90.0) ) {
         cerr << "dim02 value[0]: " << (*dvals)[0] << " vs " << -90.0 << endl;
         exit(1);     
      }
      if ( mismatch( (*dvals)[dvals->size() - 1], 90.0) ) {
         cerr << "dim02 last: " << (*dvals)[dvals->size() - 1] << " vs " << 90.0 << endl;
         exit(1);     
      }
      
      delete dvals;
      
   } else {
      cerr << "defined dimension appears to be undefined. ";
      exit(1);
   }
   if ( catlog->dimensionValues( "dim03", dquant, dunits, &dvals ) ) {
      if ( dquant != "P" ) {
         cerr << "dim03 quant is " << dquant<< " vs. 'P'" << endl;
         exit(1);
      }
      if ( dunits != "hPa" ) {
         cerr << "dim03 units are " << dunits << " vs. 'hPa'" << endl;
         exit(1);
      }
      if ( dvals->size() != 4 ) {
         cerr << "dim03 values are the wrong size: " << dvals->size() << " vs " << 4 << endl;
         exit(1);     
      }
      if ( mismatch( (*dvals)[0], 1000.0) ) {
         cerr << "dim03 value[0]: " << (*dvals)[0] << " vs " << 1000.0 << endl;
         exit(1);     
      }
      if ( mismatch( (*dvals)[dvals->size() - 1], 20.0) ) {
         cerr << "dim03 last: " << (*dvals)[dvals->size() - 1] << " vs " << 20.0 << endl;
         exit(1);     
      }
      
      delete dvals;
      
   } else {
      cerr << "defined dimension appears to be undefined. ";
      exit(1);
   }
   // make sure we can do this twice
   if ( catlog->dimensionValues( "dim03", dquant, dunits, &dvals ) ) {
      if ( dquant != "P" ) {
         cerr << "dim03 quant is " << dquant<< " vs. 'P'" << endl;
         exit(1);
      }
      if ( dunits != "hPa" ) {
         cerr << "dim03 units are " << dunits << " vs. 'hPa'" << endl;
         exit(1);
      }
      if ( dvals->size() != 4 ) {
         cerr << "dim03 values are the wrong size: " << dvals->size() << " vs " << 4 << endl;
         exit(1);     
      }
      if ( mismatch( (*dvals)[0], 1000.0) ) {
         cerr << "dim03 value[0]: " << (*dvals)[0] << " vs " << 1000.0 << endl;
         exit(1);     
      }
      if ( mismatch( (*dvals)[dvals->size() - 1], 20.0) ) {
         cerr << "dim03 last: " << (*dvals)[dvals->size() - 1] << " vs " << 20.0 << endl;
         exit(1);     
      }
      
      delete dvals;
      
   } else {
      cerr << "defined dimension appears to be undefined. ";
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


   // check the weird quantity name
   ok = catlog->query( "stuff$7(93)5m@_+-&^end", "2021-07-15T10:34", destlist, "20210714_00" );
   if ( ok  && destlist.size() != 1 ) {
      cerr << "lookup of s returned " << destlist.size() << " items" << std::endl;
      exit(1);     
   }
   if ( destlist[0].pre != "pattern_for_t02" ) {
      cerr << "lookup of stuff$7(93)5m@_+-&^end returned " << destlist[0].pre  << " as its first 'pre'" << std::endl;
      exit(1);        
   }
   if ( destlist[0].dims != 2 ) {
      cerr << "lookup of stuff$7(93)5m@_+-&^end dest # 0 dimensionality = " << destlist[0].dims  << std::endl;
      exit(1);                 
   }
   teststring = catlog->getAttr( destlist[0], "attr2" );
   if ( teststring != "at2valt02" ) {
      cerr << "lookup of stuff$7(93)5m@_+-&^end dest # 0 attribute 'attr2' = " << teststring  << std::endl;
      exit(1);           
   }


catlog->debug( 100 );
   exit(0);

}
