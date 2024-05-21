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
#include "gigatraj/MetGridSBRot.hh"
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
    MetSBRot *metsrc0;
    MetGridSBRot *metsrc;
    real lon;
    real lat;
    real u,v,w;
    real u0,v0,w0;
    real val, val0;
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
    metsrc0 = new MetSBRot(20.0, 30.0);
    metsrc = new MetGridSBRot;

    if ( grp->size() > 2 ) {
       metsrc->setPgroup(grp, 1);
    } else {
       metsrc->setPgroup(grp);    
    }

    //metsrc->dbug = 1;
    
    if ( metsrc->useMet() ) {
    
        /* sample the wind at 45 S */
       metsrc0->get_uvw( 0.0, 0.0, -45.0, 0.0, &u0, &v0, &w0);  
       metsrc->get_uvw( 0.0, 0.0, -45.0, 0.0, &u, &v, &w);  
       if ( mismatch(u, u0) || mismatch(v,v0) || mismatch(w, w0) ) {
          cerr << "Bad equ. wind val 0-3 : (" << u0 << ", " << v0  << ", " << w0 << ")"
          << " vs.  (" << u << ", " << v  << ", " << w << ")" << endl;
          metsrc->signalMetDone();
          grp->shutdown();
          exit(1);  
       } 
    
    
        /* sample the temperature */
        //metsrc->dbug = 1;
        val0 = metsrc0->getData( "t", 3.0, 0.0, 45.0, 0.0 );
        val = metsrc->getData( "t", 3.0, 0.0, 45.0, 0.0 );
        if ( mismatch(val0, val) ) {
           cerr << "Bad! 45-deg temp value: " << val0 << " vs. " << val << endl;
           metsrc->signalMetDone();
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
