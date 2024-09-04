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
