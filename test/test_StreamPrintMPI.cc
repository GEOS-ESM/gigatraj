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
#include <math.h>

#include "mpi.h"

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/MPIGrp.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/StreamPrint.hh"

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

    Parcel p;
    Parcel *pc;
    real lon;
    real lat;
    real z;
    real xlon, xlat, xz;
    int status;
    Flock *flk, *flk2;
    Flock::iterator iter;
    Swarm *swm, *swm2;
    Swarm::iterator siter;
    MPIGrp *pgrp;
    int k;
    int n;
    int my_id;
    int nprocs;
    StreamPrint *sp;
    ostringstream ooo;
    std::string sss;
    
    n = 104;
    
    // create a process group (MPI, of course)
    pgrp = new MPIGrp(argc, argv);
    
    my_id = pgrp->id();
    nprocs = pgrp->size();

    // create a Flock of n parcels
    flk = new Flock( p, pgrp, n, 0);
    pgrp->sync();

    // set the height and position of each parcel, 
    // using the set() method as is appropriate for multi-processing
    for ( k=0; k<n; k++ ) {
        
        if ( my_id == 0 ) {
           z = 300.0+k;
           lon = k*10.0;
           lat = 80.0 - k*1.0;
        } else {
           z = -999.0;
           lon = 0.0;
           lat = 0.0;
        }   
        p.setZ(z);
        p.setPos(lon,lat);

        flk->set(k, p);
        
    }
    pgrp->sync();


    // test parallelized printing,

    sp = new StreamPrint(ooo);

    try {
       sp->apply(*flk);
    } catch (...) {
       cerr << "Stream Print failed oj Flock" << endl;
       exit(1);  
    }

    sss = ooo.str();
    //cerr << "sss=<<" << sss << ">>" << endl;
    
    if ( flk->is_root() ) {
       if ( sss == "" ) {
          cerr << "Root output stream is null." << endl;
          exit(1);  
       }
    } else {
       if ( sss != "" ) {
          cerr << "Non-Root output stream is non-null." << endl;
          cerr << sss << endl;
          exit(1);  
       }
    
    }
    
    pgrp->sync();


    delete flk;


    // create a Swarm of n parcels
    swm = new Swarm( p, pgrp, n, 0);
    pgrp->sync();

    // set the height and position of each parcel, 
    // using the set() method as is appropriate for multi-processing
    for ( k=0; k<n; k++ ) {
        
        if ( my_id == 0 ) {
           z = 300.0+k;
           lon = k*10.0;
           lat = 80.0 - k*1.0;
        } else {
           z = -999.0;
           lon = 0.0;
           lat = 0.0;
        }   
        p.setZ(z);
        p.setPos(lon,lat);

        swm->set(k, p);
        
    }
    pgrp->sync();


    // test parallelized printing,

    sp = new StreamPrint(ooo);

    try {
       sp->apply(*swm);
    } catch (...) {
       cerr << "Stream Print failed oj Swarm" << endl;
       exit(1);  
    }

    sss = ooo.str();
    //cerr << "sss=<<" << sss << ">>" << endl;
    
    if ( swm->is_root() ) {
       if ( sss == "" ) {
          cerr << "Root output stream is null." << endl;
          exit(1);  
       }
    } else {
       if ( sss != "" ) {
          cerr << "Non-Root output stream is non-null." << endl;
          cerr << sss << endl;
          exit(1);  
       }
    
    }
    
    pgrp->sync();


    delete swm;

    /* Shut down MPI */
    //MPI_Finalize();
    pgrp->shutdown();

    // if we got this far, everything is OK
    return 0;

}
    
