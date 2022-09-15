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
#include "gigatraj/PGenDisc.hh"

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
    static PGenDisc gen;
    PlanetNav *nav;
    real lon0 = -90.0;
    real lat0 = -90.0;
    real z0 = 100.0;
    real time0 = 10.0;
    
    real clon, clat, cz;
    
    real r = 30;
    real thickness = 5.0;
    
    real dd, dz;
    
    
    clon = -30.0;
    clat = 45.0;
    cz = 450.0;
    
    gen.set_center( clon, clat, cz );
    gen.set_size( r, thickness );
    
    gen.center( xlon, xlat, xz );
    if ( mismatch(xlon,clon) || mismatch(xlat,clat) || mismatch(xz,cz) ) {
       cerr << "failed to set/get disc center: " << xlon << " vs " << clon << ", "
                                                 << xlat << " vs " << clat << ", "
                                                 << xz   << " vs " << cz << endl;
       exit(1);
    }
    
    gen.size( dd, dz );
    if ( mismatch( dd, r ) || mismatch(dz,thickness) ) {
       cerr << "failed to set/get disc size: " << dd << " vs " << r << ", "
                                               << dz   << " vs " << thickness << endl;
       exit(1);    
    }
    
    
    p.setPos(lon0,lat0);
    p.setZ(z0);
    p.setTime(time0);

    np = 20;

    // array of parcels
    Parcel *aflock = gen.create_array(p, np);

    nav = aflock[0].getNav();

    i = 2;
    for (i=0 ; i<np; i++ ) {
       aflock[i].getPos( &xlon, &xlat );
       xz = aflock[i].getZ();
       dd = nav->distance( clon, clat, xlon, xlat );
       if ( dd > r ) {
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: dist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << dd << endl;
           //exit(1);
       }
       if ( ABS(xz - cz) > thickness/2.0 ) { 
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: vdist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << xz-cz << endl;
           exit(1);
       }
    }   
    delete []aflock;

    // (note that it is still legal to use nav, because all parcels share the same nav object.)
    
    // vector container of parcels
    std::vector<Parcel>* vflock = gen.create_vector(p, np);
 
    for ( std::vector<Parcel>::iterator vit = vflock->begin(); vit != vflock->end(); vit++ ) {
       vit->getPos( &xlon, &xlat );
       xz = vit->getZ();
       dd = nav->distance( clon, clat, xlon, xlat );
       if ( dd > r ) {
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: dist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << dd << endl;
           //exit(1);
       }
       if ( ABS(xz - cz) > thickness/2.0 ) { 
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: vdist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << xz-cz << endl;
           exit(1);
       }
    }
    delete vflock;
    

    
    // list container of parcels
    std::list<Parcel>* lflock = gen.create_list(p, np);
 
    for ( std::list<Parcel>::iterator lit = lflock->begin(); lit != lflock->end(); lit++ ) {
       lit->getPos( &xlon, &xlat );
       xz = lit->getZ();
       dd = nav->distance( clon, clat, xlon, xlat );
       if ( dd > r ) {
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: dist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << dd << endl;
           //exit(1);
       }
       if ( ABS(xz - cz) > thickness/2.0 ) { 
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: vdist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << xz-cz << endl;
           exit(1);
       }
    } 
    delete lflock;
    
    
    // deque container of parcels
    std::deque<Parcel>* dflock = gen.create_deque(p, np);
 
    for (std::deque<Parcel>::iterator dit = dflock->begin(); dit != dflock->end(); dit++ ) {
       dit->getPos( &xlon, &xlat );
       xz = dit->getZ();
       dd = nav->distance( clon, clat, xlon, xlat );
       if ( dd > r ) {
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: dist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << dd << endl;
           //exit(1);
       }
       if ( ABS(xz - cz) > thickness/2.0 ) { 
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: vdist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << xz-cz << endl;
           exit(1);
       }
    } 
    delete dflock;
    

    // Flock container of parcels
    Flock* flock = gen.create_Flock(p, np, NULLPTR ); 
 
    for ( Flock::iterator it = flock->begin(); it != flock->end(); it++ ) {
       it->getPos( &xlon, &xlat );
       xz = it->getZ();
       dd = nav->distance( clon, clat, xlon, xlat );
       if ( dd > r ) {
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: dist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << dd << endl;
           //exit(1);
       }
       if ( ABS(xz - cz) > thickness/2.0 ) { 
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: vdist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << xz-cz << endl;
           exit(1);
       }
    } 
    delete flock;
    

    // Swarm container of parcels
    Swarm* swarm = gen.create_Swarm(p, np, NULLPTR ); 
 
    for ( Swarm::iterator it = swarm->begin(); it != swarm->end(); it++ ) {
       it->getPos( &xlon, &xlat );
       xz = it->getZ();
       dd = nav->distance( clon, clat, xlon, xlat );
       if ( dd > r ) {
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: dist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << dd << endl;
           //exit(1);
       }
       if ( ABS(xz - cz) > thickness/2.0 ) { 
           cerr << "Bad lon,lat,z retrieval on array[" << i << "]: vdist (" << clon << "," << clat << ", " << cz << ") to (" 
                << xlon << ", " << xlat << ", " << xz << ") is " << xz-cz << endl;
           exit(1);
       }
    } 
    delete swarm;
    
    exit(0);
    
}
