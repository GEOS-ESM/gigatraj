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
