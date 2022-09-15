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
     Test program for the ChangeVertical filter using the MetGridSBRot source
*/
     
#include <iostream>
#include <math.h>

#include <stdlib.h>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetGridSBRot.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/ChangeVertical.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;


int main() 
{

    MetGridSBRot *metsrc;
    real lon;
    real lat;
    real z;
    real val;
    real val0;
    real val2;
    int status;
    double time0;
    double tt;
    std::vector<real> thetas;
    real thet;
    int i;
    std::string fname;
    Parcel p;
    Parcel p2;
    ChangeVertical vc;
    ChangeVertical vc2;
    
    /* create an object with the default values (40 m/s, no tilt) */

    // Gridded SBRot source
    metsrc = new MetGridSBRot;
    
    //metsrc->debug = 1;
    
    // set the parcel position and time
    lat = 12.3;
    lon = 45.6;
    time0 = 5.5;
    z = 15.0;
    
    //cerr << "vert coord " << metsrc->vertical() << endl;
    
    p.setMet( *metsrc);
    p.setTime( time0 );
    p.setPos( lon, lat );
    p.setZ(z);
    
    val0 = metsrc->getData( "theta", time0, lon, lat, z );
    
    p2 = p;
    vc.to( "theta" );
    
    vc.apply(p2);
    val = p2.getZ();
    
    if ( mismatch(val, val0) ) {
       cerr << "bad p  value: " << val << " vs. " << val0 << endl;
       exit(1);  
    } 
    
    vc2.from( "theta" );
    vc2.apply(p2);
    val2 = p2.getZ();
    if ( mismatch(val2, z, 0.01) ) {
       cerr << "bad alt  value: " << val2 << " vs. " << z << endl;
       exit(1);  
    } 
    
    delete metsrc;

    //------------------------------------------------------------------

    // if we got this far, everything is OK
    exit(0);
    
}
