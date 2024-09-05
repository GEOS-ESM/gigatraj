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
