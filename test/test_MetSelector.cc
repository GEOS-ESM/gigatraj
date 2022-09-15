/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
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
