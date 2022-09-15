/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


// Unfinished

#include <iostream>
#include <sstream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/SerialGrp.hh"
#include "gigatraj/Swarm.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::istringstream;
using std::operator<<;
using std::operator>>;

int main() 
{

    Parcel p;
    Parcel *pc;
    real lon;
    real lat;
    real z;
    int status;
    Swarm *swm;
    Swarm::iterator iter;
    SerialGrp *pgrp;
    int k;

    // create a process group (serial, of course)
    pgrp = new SerialGrp();

    // create a Swarm of 100 parcels
    swm = new Swarm( p, pgrp, 100, 0);

    if ( swm->size() != 100 ) {
       cerr << "Swarm size not set properly: " << swm->size() << endl;
       exit(1);
    }
    
    // set all parcels in the Swarm
    //  Note: the proper way to set parcels by their index value
    //  is by the set() method.  Using the iterator here works
    //  only for serial processing.  This test is merely to
    //  check that the iterator works.
    k=0;
    for ( iter=swm->begin(); iter!=swm->end(); iter++ ) {
       //cerr << "iter " << k << endl;
       
       lon = k*10.0;
       lat = 80.0 - k*1.0;
       iter->setPos(lon,lat);
       
       k++;
    }
    // set the height of each parcel, this time
    // using the set() method
    for ( k=0; k<100; k++ ) {
        
        z = 300.0+k;
        
        p = swm->get(k);
        lon = -999.0;
        lat = -999.0;
        p.getPos(&lon,&lat);
        if ( mismatch( lat, 80.0 - k*1.0 ) || mismatch( lon, k*10.0 ) ) {
           cerr << "Bad lon/lat retrieval on " << k << ": ("
           << k*10.0 << ", " << 80-k*1.0 << ") != (" 
           << lon << ", " << lat << ")" << endl;
           exit(1);
        }   
        
        p.setZ(z);
        swm->set(k, p);

    }
    // check each parcel in the Swarm
    //  Note: the proper way to get parcels by their index value
    //  is by the get() method.  Using the iterator here works
    //  only for serial processing.  This test is merely to
    //  check that the iterator works.
    k=0;
    for ( iter=swm->begin(); iter!=swm->end(); iter++ ) {
       
       lon = -999.0;
       lat = -999.0;
       z = -999.0;
       iter->getPos(&lon,&lat);
       z = iter->getZ();
       if ( mismatch( lat, 80.0 - k*1.0 ) || mismatch( lon, k*10.0 ) 
       || mismatch( z, 300.0+k) ) {
          cerr << "Bad lon,lat,z retrieval on " << k << ":"
          << "(" << k*10.0 << ", " << 80-k*1.0 << ", " << 300.0+k << ") != "
          << "(" << lon << ", " << lat << ", " << z << ")" << endl;
          exit(1);
       }   
       
       
       k++;
    }

    // test [] syntax
    for ( k=0; k<swm->size(); k++ ) {
       lon = -999.0;
       lat = -999.0;
       z = -999.0;
       p = (*swm)[k];
       p.getPos(&lon,&lat);
       z = p.getZ();
       if ( mismatch( lat, 80.0 - k*1.0 ) || mismatch( lon, k*10.0 ) 
       || mismatch( z, 300.0+k) ) {
          cerr << "Bad lon,lat,z retrieval on " << k << ":"
          << "(" << k*10.0 << ", " << 80-k*1.0 << ", " << 300.0+k << ") != "
          << "(" << lon << ", " << lat << ", " << z << ")" << endl;
          exit(1);
       }   
    
    }
    
    // add a new parcel to the Swarm
    swm->add(p);
    if ( swm->size() != 101 ) {
       cerr << "Swarm size incremented improperly: " << swm->size() << endl;
       exit(1);
    }
    

    delete swm;

    exit(0);
}
