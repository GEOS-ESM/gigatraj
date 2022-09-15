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
#include "gigatraj/PGenGrid.hh"

#include "test_utils.hh"


using namespace gigatraj;
using std::cout;
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
    real xlon;
    real xlat;
    real xz;
    real time;
    int status;
    int i;
    int np;
    static PGenGrid gen;
    const real lat0 = 23.45;
    const real lon0 = -45.678;
    const real z0 = 456.5;
    const double time0 = 1000.0;
    
    p.setPos(lon0,lat0);
    p.setZ(z0);
    p.setTime(time0);

    // array of parcels
    Parcel *aflock = gen.create_array(p, &np
                                 ,   0.0, 355.0, 5.0 
                                 , -90.0,  90.0, 5.0
                                 , 320.0, 450.0,10.0
                                 );
    i = 0; 
    for ( real z = 320.0; z<= 450.0; z += 10.0 ) {
    for ( real lat = -90.0; lat <= 90.0; lat += 5.0 ) {
    for ( real lon =  0.0; lon <= 355.0; lon += 5.0 ) {
        aflock[i].getPos( &xlon, &xlat );
        xz = aflock[i].getZ();
        time = aflock[i].getTime();
        if ( mismatch( xlat, lat ) || mismatch( xlon, lon ) ||  mismatch( xz, z) ) {
           cerr << "Bad lon,lat,z retrieval on array[" << i << ": (" << lon << "," << lat << ", " << z << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
        } 
        
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on deque: (" << lon << "," << lat << z << ") :" << time0 << " != (" << time << ")" << endl;
           exit(1);          
        }  

        i++;
    }
    }
    }
    if ( np != i ) {
       cerr << "Wrong number of array elements: " << np << " :" << i << endl;
       exit(1);    
    }
    delete []aflock;


    // vector container of parcels
    std::vector<Parcel>* vflock = gen.create_vector(p 
                                 ,   0.0, 355.0, 5.0 
                                 , -90.0,  90.0, 5.0
                                 , 320.0, 450.0,10.0
                                 );
 
    std::vector<Parcel>::iterator vit = vflock->begin();
    
    
    for ( real z = 320.0; z<= 450.0; z += 10.0 ) {
    for ( real lat = -90.0; lat <= 90.0; lat += 5.0 ) {
    for ( real lon =  0.0; lon <= 355.0; lon += 5.0 ) {
        vit->getPos( &xlon, &xlat );
        xz = vit->getZ();
        time = vit->getTime();
        if ( mismatch( xlat, lat ) || mismatch( xlon, lon ) ||  mismatch( xz, z) ) {
           cerr << "Bad lon,lat,z retrieval on vector: (" << lon << "," << lat << ", " << z << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
        } 
        
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on vector: (" << lon << "," << lat << z << ") :" << time0 << " != (" << time << ")" << endl;
           exit(1);          
        }  

        ++vit;
    }
    }
    }

    
    // list container of parcels
    std::list<Parcel>* lflock = gen.create_list(p 
                                 ,   0.0, 355.0, 5.0 
                                 , -90.0,  90.0, 5.0
                                 , 320.0, 450.0,10.0
                                 );
 
    std::list<Parcel>::iterator lit = lflock->begin();
    
    for ( real z = 320.0; z<= 450.0; z += 10.0 ) {
    for ( real lat = -90.0; lat <= 90.0; lat += 5.0 ) {
    for ( real lon =  0.0; lon <= 355.0; lon += 5.0 ) {
        lit->getPos( &xlon, &xlat );
        xz = lit->getZ();
        time = lit->getTime();
        if ( mismatch( xlat, lat ) || mismatch( xlon, lon ) ||  mismatch( xz, z) ) {
           cerr << "Bad lon,lat,z retrieval on list: (" << lon << "," << lat << ", " << z << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
        } 
        
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on list: (" << lon << "," << lat << z << ") :" << time0 << " != (" << time << ")" << endl;
           exit(1);          
        }  

        ++lit;
    }
    }
    }


    
    // deque container of parcels
    std::deque<Parcel>* dflock = gen.create_deque(p 
                                 ,   0.0, 355.0, 5.0 
                                 , -90.0,  90.0, 5.0
                                 , 320.0, 450.0,10.0
                                 );
 
    std::deque<Parcel>::iterator dit = dflock->begin();
    
    for ( real z = 320.0; z<= 450.0; z += 10.0 ) {
    for ( real lat = -90.0; lat <= 90.0; lat += 5.0 ) {
    for ( real lon =  0.0; lon <= 355.0; lon += 5.0 ) {
        dit->getPos( &xlon, &xlat );
        xz = dit->getZ();
        time = dit->getTime();
        if ( mismatch( xlat, lat ) || mismatch( xlon, lon ) ||  mismatch( xz, z) ) {
           cerr << "Bad lon,lat,z retrieval on deque: (" << lon << "," << lat << ", " << z << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
        } 
        
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on deque: (" << lon << "," << lat << z << ") :" << time0 << " != (" << time << ")" << endl;
           exit(1);          
        }  

        ++dit;
    }
    }
    }

    // Flock container of parcels
    Flock* flock = gen.create_Flock(p 
                                 ,   0.0, 355.0, 5.0 
                                 , -90.0,  90.0, 5.0
                                 , 320.0, 450.0,10.0
                                 );
 
    Flock::iterator fit = flock->begin();
    for ( real z = 320.0; z<= 450.0; z += 10.0 ) {
    for ( real lat = -90.0; lat <= 90.0; lat += 5.0 ) {
    for ( real lon =  0.0; lon <= 355.0; lon += 5.0 ) {
        fit->getPos( &xlon, &xlat );
        xz = fit->getZ();
        time = fit->getTime();
        if ( mismatch( xlat, lat ) || mismatch( xlon, lon ) ||  mismatch( xz, z) ) {
           cerr << "Bad lon,lat,z retrieval on Flockl: (" << lon << "," << lat << ", " << z << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
        } 
        
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on Flock: (" << lon << "," << lat << z << ") :" << time0 << " != (" << time << ")" << endl;
           exit(1);          
        }  

        ++fit;
    }
    }
    }    
    delete flock;


    // Swarm container of parcels
    Swarm* swarm = gen.create_Swarm(p 
                                 ,   0.0, 355.0, 5.0 
                                 , -90.0,  90.0, 5.0
                                 , 320.0, 450.0,10.0
                                 );
 
    Swarm::iterator sit = swarm->begin();
    for ( real z = 320.0; z<= 450.0; z += 10.0 ) {
    for ( real lat = -90.0; lat <= 90.0; lat += 5.0 ) {
    for ( real lon =  0.0; lon <= 355.0; lon += 5.0 ) {
        sit->getPos( &xlon, &xlat );
        xz = sit->getZ();
        time = sit->getTime();
        if ( mismatch( xlat, lat ) || mismatch( xlon, lon ) ||  mismatch( xz, z) ) {
           cerr << "Bad lon,lat,z retrieval on Swarml: (" << lon << "," << lat << ", " << z << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
        } 
        
        if ( mismatch( time, time0 ) ) {
           cerr << "Bad time retrieval on Swarm: (" << lon << "," << lat << z << ") :" << time0 << " != (" << time << ")" << endl;
           exit(1);          
        }  

        ++sit;
    }
    }
    }    
    delete swarm;


    
    exit(0);
    
}
