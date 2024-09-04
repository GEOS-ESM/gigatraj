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
     Test program for the MetSelector class
*/
     
#include <iostream>
#include <math.h>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetSelector.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;


int main() 
{
    MetSelector maker;
    std::vector<std::string> nameset;
    int flags;
    std::string desc;
    MetData *msrc;
    bool good;
    
    
    nameset = maker.list();
    //for ( int i=0; i<nameset.size(); i++ ) {
    //    cerr << nameset[i] << endl;
    //}
    if ( nameset.size() < 2 ) {
       cerr << "list() returned fewer than two sources" << endl;
       exit(1);
    }
    
    nameset = maker.list( METCAP_AFCN );
    //for ( int i=0; i<nameset.size(); i++ ) {
    //    cerr << nameset[i] << endl;
    //}
    if ( nameset.size() < 1 ) {
       cerr << "list(MET_AFCN) returned fewer than one source" << endl;
       exit(1);
    }
    
    nameset = maker.list( METCAP_GRID );
    //for ( int i=0; i<nameset.size(); i++ ) {
    //    cerr << nameset[i] << endl;
    //}
    if ( nameset.size() < 1 ) {
       cerr << "list(MET_GRID) returned fewer than one source" << endl;
       exit(1);
    }




    flags = maker.characterization("MetSBRot");
    if ( flags != (METCAP_AFCN) ) {
       cerr << "characterization(MetSBRot) fai;ed: " << flags << endl;
       exit(1);    
    }
    flags = maker.characterization("MetGridSBRot");
    if ( flags != (METCAP_GRID | METCAP_LATLON) ) {
       cerr << "characterization(MetGridSBRot) fai;ed: " << flags << endl;
       exit(1);    
    }

    good = false;
    try {
        flags = maker.characterization("Does Not Exist");
    } catch (MetSelector::badUnknownSource) {
       good = true;
    }
    if ( ! good ) {   
       cerr << "Flags characterization of unknown source succeeded: " << flags << endl;
       exit(1);  
    }   




    desc = maker.description("MetGridSBRot");
    if ( desc.size() < 10 ) {
       cerr << "description(MetGridSBRot) fai;ed: " << desc << endl;
       exit(1);        
    }

    good = false;
    try {
        desc = maker.description("Does Not Exist");
    } catch (MetSelector::badUnknownSource) {
       good = true;
    }
    if ( ! good ) {   
       cerr << "Description unknown source succeeded: " << desc << endl;
       exit(1);  
    }   




   msrc = maker.source("MetGridSBRot");
   desc = "MetGridSBRot";
   if ( msrc->id() != desc ) {
      cerr << "generation of MetGridSBRot returned wrong type: " << msrc->id() << " vs " << desc << endl;
      exit(1);
   }
   
   delete msrc;

    good = false;
    try {
        msrc = maker.source("Does Not Exist");
    } catch (MetSelector::badUnknownSource) {
       good = true;
    }
    if ( ! good ) {   
       cerr << "generation of  unknown source succeeded: " << msrc->id() << endl;
       exit(1);  
    }   




    // if we got this far, everything is OK
    exit(0);
    
}
