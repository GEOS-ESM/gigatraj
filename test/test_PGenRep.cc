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
#include "gigatraj/PGenRep.hh"

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
    real lon;
    real lat;
    real z;
    real time;
    int status;
    int i;
    static PGenRep gen;
    const real lat0 = 23.45;
    const real lon0 = -45.678;
    const real z0 = 456.5;
    const double time0 = 1000.0;
    
    
    p.setPos(lon0,lat0);
    p.setZ(z0);
    p.setTime(time0);
    
    // array of parcels
    Parcel *aflock = gen.create_array(p,10);    
    for ( i=0; i<10 ; i++ ) {
        aflock[i].getPos( &lon, &lat );
        if ( mismatch( lat, lat0 ) || mismatch( lon, lon0 ) ) {
           cerr << "Bad lon,lat retrieval on array element " << i << ": (" << lon0 << "," << lat0 << ") != (" << lon << ", " << lat << ")" << endl;
           exit(1);
        } 
        
        z = aflock[i].getZ();
        if ( mismatch( z, z0 ) ) {
           cerr << "Bad z retrieval on array element " << i << ": (" << z0 << ") != (" << z << ")" << endl;
           exit(1);
        }  
       
        time = aflock[i].getTime();
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on array element " << i << ": (" << time0 << ") != (" << time << ")" << endl;
           exit(1);          
        }  

    }
    delete []aflock;
    
    // vector container of parcels
    std::vector<Parcel>*  vflock = gen.create_vector(p,10);    
    i = 0;
    std::vector<Parcel>::iterator vit = vflock->begin();
    while ( vit != vflock->end() ) {

        vit->getPos( &lon, &lat );
        if ( mismatch( lat, lat0 ) || mismatch( lon, lon0 ) ) {
           cerr << "Bad lon,lat retrieval on vector element " << i << ": (" << lon0 << "," << lat0 << ") != (" << lon << ", " << lat << ")" << endl;
           exit(1);
        } 
        
        z = vit->getZ();
        if ( mismatch( z, z0 ) ) {
           cerr << "Bad z retrieval on vector element " << i << ": (" << z0 << ") != (" << z << ")" << endl;
           exit(1);
        }  
       
        time = vit->getTime();
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on vector element " << i << ": (" << time0 << ") != (" << time << ")" << endl;
           exit(1);          
        }  

        i++;
        ++vit;
    }
    delete vflock;
    
    // list container of parcels
    std::list<Parcel>*  lflock = gen.create_list(p,10);    
    i = 0;
    std::list<Parcel>::iterator lit = lflock->begin();
    while ( lit != lflock->end() ) {

        lit->getPos( &lon, &lat );
        if ( mismatch( lat, lat0 ) || mismatch( lon, lon0 ) ) {
           cerr << "Bad lon,lat retrieval on list element " << i << ": (" << lon0 << "," << lat0 << ") != (" << lon << ", " << lat << ")" << endl;
           exit(1);
        } 
        
        z = lit->getZ();
        if ( mismatch( z, z0 ) ) {
           cerr << "Bad z retrieval on list element " << i << ": (" << z0 << ") != (" << z << ")" << endl;
           exit(1);
        }  
       
        time = lit->getTime();
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on list element " << i << ": (" << time0 << ") != (" << time << ")" << endl;
           exit(1);          
        }  

        i++;
        ++lit;

    }
    delete lflock;

    // deque container of parcels    
    std::deque<Parcel>*  qflock = gen.create_deque(p,10);    
    i = 0;
    std::deque<Parcel>::iterator dit = qflock->begin();
    while ( dit != qflock->end() ) {

        dit->getPos( &lon, &lat );
        if ( mismatch( lat, lat0 ) || mismatch( lon, lon0 ) ) {
           cerr << "Bad lon,lat retrieval on deque element " << i << ": (" << lon0 << "," << lat0 << ") != (" << lon << ", " << lat << ")" << endl;
           exit(1);
        } 
        
        z = dit->getZ();
        if ( mismatch( z, z0 ) ) {
           cerr << "Bad z retrieval on deque element " << i << ": (" << z0 << ") != (" << z << ")" << endl;
           exit(1);
        }  
       
        time = dit->getTime();
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on deque element " << i << ": (" << time0 << ") != (" << time << ")" << endl;
           exit(1);          
        }  

        i++;
        ++dit;

    }
    delete qflock;

    // Flock container of parcels
    Flock* flock = gen.create_Flock(p, 10); 
    i = 0;
    Flock::iterator fit = flock->begin();
    while ( fit != flock->end() ) {

        fit->getPos( &lon, &lat );
        if ( mismatch( lat, lat0 ) || mismatch( lon, lon0 ) ) {
           cerr << "Bad lon,lat retrieval on flock element " << i << ": (" << lon0 << "," << lat0 << ") != (" << lon << ", " << lat << ")" << endl;
           exit(1);
        } 
        
        z = fit->getZ();
        if ( mismatch( z, z0 ) ) {
           cerr << "Bad z retrieval on flock element " << i << ": (" << z0 << ") != (" << z << ")" << endl;
           exit(1);
        }  
       
        time = fit->getTime();
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on flock element " << i << ": (" << time0 << ") != (" << time << ")" << endl;
           exit(1);          
        }  

        i++;
        ++fit;

    }
    delete flock;
    
    // Swarm container of parcels
    Swarm* swarm = gen.create_Swarm(p, 10); 
    i = 0;
    Swarm::iterator sit = swarm->begin();
    while ( sit != swarm->end() ) {

        sit->getPos( &lon, &lat );
        if ( mismatch( lat, lat0 ) || mismatch( lon, lon0 ) ) {
           cerr << "Bad lon,lat retrieval on swarm element " << i << ": (" << lon0 << "," << lat0 << ") != (" << lon << ", " << lat << ")" << endl;
           exit(1);
        } 
        
        z = sit->getZ();
        if ( mismatch( z, z0 ) ) {
           cerr << "Bad z retrieval on swarm element " << i << ": (" << z0 << ") != (" << z << ")" << endl;
           exit(1);
        }  
       
        time = sit->getTime();
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on swarm element " << i << ": (" << time0 << ") != (" << time << ")" << endl;
           exit(1);          
        }  

        i++;
        ++sit;

    }
    delete swarm;
    
    exit(0);
    
}
