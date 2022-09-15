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
#include "gigatraj/MetSBRot.hh"
#ifdef USING_MPI
#include "gigatraj/MPIGrp.hh"
#else
#include "gigatraj/SerialGrp.hh"
#endif

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::istringstream;
using std::operator<<;
using std::operator>>;

int main(int argc, char* argv[]) 
{
    MetSBRot *metsrc;
    real lon;
    real lat;
    real u,v,w;
    real val, valx;
    int status;
    string time1, time2;
    double time0;
    int i;
    ProcessGrp *grp;
    
    /* start up MPI */
#ifdef USING_MPI
    grp = new MPIGrp(argc, argv);
#else
    grp = new SerialGrp();
#endif

    /* create an object with the default values (40 m/s, no tilt) */
    metsrc = new MetSBRot;

    if ( grp->size() > 2 ) {
       metsrc->setPgroup(grp, 1);
    } else {
       metsrc->setPgroup(grp);    
    }

//cerr << "Begin" << endl;
    if ( metsrc->useMet() ) {
    
        /* sample the wind at 45 S */
        metsrc->get_uvw( 0.0, 0.0, -45.0, 0.0, &u, &v, &w);  
        if ( mismatch(u, 28.28427125) || mismatch(v,0.0) || mismatch(w, 0.0) ) {
           cerr << "Bad equ. wind val 0-3 : (" << u << ", " << v  << ", " << w << ")" << endl;
           grp->shutdown();
           exit(1);  
        } 
    
    
        /* sample the temperature */
        valx = 288.15;
        val = metsrc->getData( "t", 0.0, 0.0, 45.0, 0.0 );
        if ( mismatch(valx, val) ) {
           cerr << "Bad! 45-deg temp value: " << valx << " vs. " << val << endl;
           grp->shutdown();
           exit(1);  
        } 
        
    
        metsrc->signalMetDone();
    }
//cerr << "End" << endl;
    

    delete metsrc;

    /* Shut down MPI */
    grp->shutdown();

    // if we got this far, everything is OK
    exit(0);
    
}
