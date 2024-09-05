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
#include "gigatraj/PGenRnd.hh"

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
    static PGenRnd gen;
    real lon0 = -90.0;
    real lat0 = -90.0;
    real z0 = 100.0;
    real time0 = 10.0;
    
    p.setPos(lon0,lat0);
    p.setZ(z0);
    p.setTime(time0);

    np = 10;

    // array of parcels
    Parcel *aflock = gen.create_array(p, np);
    i = 2;

    aflock[i].getPos( &xlon, &xlat );
    xz = aflock[i].getZ();
    time = aflock[i].getTime();
    if ( ! mismatch( xlat, lat0 ) && ! mismatch( xlon, lon0 ) &&  ! mismatch( xz, z0) ) {
           cerr << "Bad lon,lat,z retrieval on array[" << i << ": (" << lon0 << "," << lat0 << ", " << z0 << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
    } 
    delete []aflock;

    // vector container of parcels
    std::vector<Parcel>* vflock = gen.create_vector(p, np);
 
    std::vector<Parcel>::iterator vit = vflock->begin();
    vit++;
    vit++;
    vit->getPos( &xlon, &xlat );
    xz = vit->getZ();
    if ( ! mismatch( xlat, lat0 ) && ! mismatch( xlon, lon0 ) &&  ! mismatch( xz, z0) ) {
           cerr << "Bad lon,lat,z retrieval on vector: (" << lon0 << "," << lat0 << ", " << z0 << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
    } 
    delete vflock;
    
    
    // list container of parcels
    std::list<Parcel>* lflock = gen.create_list(p, np);
 
    std::list<Parcel>::iterator lit = lflock->begin();
    lit++;
    lit++;
    lit->getPos( &xlon, &xlat );
    xz = lit->getZ();
    if ( ! mismatch( xlat, lat0 ) && ! mismatch( xlon, lon0 ) &&  ! mismatch( xz, z0) ) {
           cerr << "Bad lon,lat,z retrieval on list: (" << lon0 << "," << lat0 << ", " << z0 << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
    } 
    delete lflock;
    
    
    // deque container of parcels
    std::deque<Parcel>* dflock = gen.create_deque(p, np);
 
    std::deque<Parcel>::iterator dit = dflock->begin();
    dit++;
    dit++;
    dit->getPos( &xlon, &xlat );
    xz = dit->getZ();
    if ( ! mismatch( xlat, lat0 ) && ! mismatch( xlon, lon0 ) &&  ! mismatch( xz, z0) ) {
           cerr << "Bad lon,lat,z retrieval on deque: (" << lon0 << "," << lat0 << ", " << z0 << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
    } 
    delete dflock;
    


    // Flock container of parcels
    Flock* flock = gen.create_Flock(p, np, NULLPTR ); 
 
    Flock::iterator it = flock->begin();
    it++;
    it++;
    it->getPos( &xlon, &xlat );
    xz = it->getZ();
    if ( ! mismatch( xlat, lat0 ) && ! mismatch( xlon, lon0 ) &&  ! mismatch( xz, z0) ) {
           cerr << "Bad lon,lat,z retrieval on Flock " << i << ": (" << lon << "," << lat << ", " << z << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
    } 
    delete flock;
    

    // Swarm container of parcels
    Swarm* swarm = gen.create_Swarm(p, np, NULLPTR ); 
 
    Swarm::iterator sit = swarm->begin();
    sit++;
    sit++;
    sit->getPos( &xlon, &xlat );
    xz = sit->getZ();
    if ( ! mismatch( xlat, lat0 ) && ! mismatch( xlon, lon0 ) &&  ! mismatch( xz, z0) ) {
           cerr << "Bad lon,lat,z retrieval on Swarm " << i << ": (" << lon << "," << lat << ", " << z << ") != (" 
                << xlon << ", " << xlat << ", " << xz << ")" << endl;
           exit(1);
    } 
    delete swarm;
    
    exit(0);
    
}
