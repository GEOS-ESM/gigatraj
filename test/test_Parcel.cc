/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/




#include <iostream>
#include <sstream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"

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
    real z0;
    int status;
    ParcelFlag flgs;
    
    
    if ( p.id != string("Parcel") )
    {
       cerr << "Bad ID" << endl;
       exit(1);
    }   
    
    
    // =========================  method setPos
    // set a horizontal position
    try {
       p.setPos( 30.0, 40.0 );
    } catch ( PlanetNav::badlocation err) {
       cerr << "SetPos to (30,40) failed " << endl;
       exit(1);  
    } 
    // attempt to set a position to an invalid location
    status = 1;
    try {
       p.setPos( -40.0, 91.0 );
    } catch ( PlanetNav::badlocation err) {
       // we should catch the exception, if all works well
       status = 0;
    } 
    if ( status ) {
       cerr << "SetPos should have failed for (-40,91) " << endl;
       exit(1);  
    }


    // =========================  method getLon
    // retrieve a longitude
    lon = p.getLon();
    if ( mismatch( lon, 30.0 ) ) {
       cerr << "Bad lon retrieval: 30 != " << lon << endl;
       exit(1);
    }  


    // =========================  method getLat
    // retrieve a latitude 
    lat = p.getLat();
    if ( mismatch( lat, 40.0 ) ) {
       cerr << "Bad lat retrieval: 40 != " << lat << endl;
       exit(1);
    }   
    


    // =========================  method getPos
    // Retrieve both lat and lon at once
    p.getPos( &lon, &lat );
    if ( mismatch( lat, 40.0 ) || mismatch( lon, 30.0 ) ) {
       cerr << "Bad lon,lat retrieval: (30,40) != (" << lon << ", " << lat << ")" << endl;
       exit(1);
    }   
    

    // =========================  method addPos
    // move the parcel
    p.addPos( 4.0, 6.0 );
    // Check the moved position
    p.getPos( &lon, &lat );
    if ( mismatch( lat, 46.0 ) || mismatch( lon, 34.0 ) ) {
       cerr << "Bad lon,lat retrieval: (34,46) != (" << lon << ", " << lat << ")" << endl;
       exit(1);
    }   
    
    
    // =========================  method setZ
    // set a z value
    p.setZ(350.0);
    
    // =========================  method getZ
    // retrieve the z value
    z = p.getZ();
    if ( mismatch(z, 350.0) ) {
       cerr << "Bad z retrieval: 350 != " << z << endl;
       exit(1);
    } 
    
    
    // =========================  method addZ
    // add to the z value
    p.addZ( 15.3 );
    z = p.getZ();
    if ( mismatch(z, 365.3) ) {
       cerr << "Bad z retrieval: 365 != " << z << endl;
       exit(1);
    } 
    
    // =========================  method setTime
    p.setTime(1234.0);
    
    // =========================  method getTime
    // retrieve the time value
    z = p.getTime();
    if ( mismatch(z, 1234.0) ) {
       cerr << "Bad time retrieval: 1234 != " << z << endl;
       exit(1);
    } 
    
    
    // =========================  method setNoTrace
    // set the notrace flag
    p.setNoTrace();
    if ( p.flags() != 1 ) {
       cerr << "failed to set NoTrace" << endl;
       exit(1);
    }
    
    // =========================  method queryNoTrace
    if ( ! p.queryNoTrace()  ) {
       cerr << "queryNoTrace does not work" << endl;
       exit(1);
    }
    
    // =========================  method clearNoTrace
    // clear the notrace flag
    p.clearNoTrace();
    if ( p.queryNoTrace() ) {
       cerr << "Failed to clear NoTrace" << endl;
       exit(1);
    }
     
    
    
    // =========================  method setHitBdy
    // set the hitbdy status
    p.setHitBdy();
    if ( p.status() != 1 ) {
       cerr << "failed to set HitBdy" << endl;
       exit(1);
    }
    
    // =========================  method queryHitBdy
    if ( ! p.queryHitBdy()  ) {
       cerr << "queryHitBdy does not work" << endl;
       exit(1);
    }
    
    // =========================  method clearHitBdy
    // clear the hitbdy status
    p.clearHitBdy();
    if ( p.queryHitBdy() ) {
       cerr << "Failed to clear HitBdy" << endl;
       exit(1);
    }

    
    // =========================  method setNonVert
    // set the hitbdy status
    p.setNonVert();
    if ( p.status() != 4 ) {
       cerr << "failed to set NonVert" << endl;
       exit(1);
    }
    
    // =========================  method queryNonVert
    if ( ! p.queryNonVert()  ) {
       cerr << "queryNonVert does not work" << endl;
       exit(1);
    }
    
    // =========================  method clearNonVert
    // clear the hitbdy status
    p.clearNonVert();
    if ( p.queryNonVert() ) {
       cerr << "Failed to clear NonVert" << endl;
       exit(1);
    }
    

    // =========================  method setStatus
    p.setStatus(65432);

    // =========================  method status
    if ( p.status() != 65432 ) {
       cerr << "Failed to set status" << endl;
       exit(1);    
    }


    
    // =========================  method setNoTrace
    // set the NoTrace status
    p.setNoTrace();
    if ( p.flags() != 1 ) {
       cerr << "failed to set NoTrace" << endl;
       exit(1);
    }
    
    // =========================  method queryNoTrace
    if ( ! p.queryNoTrace()  ) {
       cerr << "queryNoTrace does not work" << endl;
       exit(1);
    }
    
    // =========================  method clearNoTrace
    // clear the NoTrace status
    p.clearNoTrace();
    if ( p.queryNoTrace() ) {
       cerr << "Failed to clear NoTrace" << endl;
       exit(1);
    }

    // =========================  method setflags
    p.setFlags(54321);

    // =========================  method flags
    if ( p.flags() != 54321 ) {
       cerr << "Failed to set flags" << endl;
       exit(1);    
    }
    
    
    // =========================  method <<
    // dump state
    ostringstream os;
    try {
       os << p;
    } catch (...) {
       cerr << "Parcel output failed " << endl;
       exit(1);  
    }   

    // =========================  method >>
    // restore state
    string pstate = os.str();
    Parcel p2;
    istringstream is( pstate );
    try {
       is >> p2;
    } catch (...) {
       cerr << "Parcel input failed: " << pstate << endl;
       exit(1);  
    }   
    z = p2.getZ();
    p2.getPos( &lon, &lat );
    if ( mismatch( lat, 46.0 ) || mismatch( lon, 34.0 ) || mismatch(z, 365.3) ) {
       cerr << "Bad restore from output parcel" << endl;
       exit(1);
    } 
    

    // =========================  method =
    // assignment operator
    p.setPos( -12.34, 45.67 );
    p.setZ(12.3);
    p.setTime(7000.0);
    try {    
       p2 = p;
    } catch (...) {
       cerr << "Parcel assignment failed " << endl;
       exit(1);  
    }   
    z = p2.getZ();
    p2.getPos( &lon, &lat );
    if ( mismatch( lat, 45.67 ) || mismatch( lon, -12.34 ) || mismatch(z, 12.3) ) {
       cerr << "Bad assignment: " << lat << " " << lon << " " << z << endl;
       exit(1);
    } 
    
    
    // =========================  method copy
    // copy
    pc = p2.copy();
    p2.setPos( 102.34, -23.4); 
    z = pc->getZ();
    pc->getPos( &lon, &lat );
    if ( mismatch( lat, 45.67 ) || mismatch( lon, -12.34 ) || mismatch(z, 12.3) ) {
       cerr << "Bad copy: " << lat << " " << lon << " " << z << endl;
       exit(1);
    }



    // ========================  method field
    z = p.field( "alt" );
    z0 = pc->getZ();
    if ( mismatch(z, z0) ) {
       cerr << "Bad field 'alt' value retrieval: " << z << " vs " << z0 << endl;
       exit(1);
    } 
    


    
    // =========================  method setNav (missing -- todo)
    // =========================  method getNav (missing -- todo)
    // =========================  method setMet (missing -- todo)
    // =========================  method getMet (missing -- todo)
    // =========================  method advance (deferred to test_traj0000)
    // =========================  method send (deferred to test_SerialGrp, test_MPIGrp)
    // =========================  method receive (deferred to test_SerialGrp, test_MPIGrp)
    
    
    exit(0);
}
