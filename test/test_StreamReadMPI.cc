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
#include <fstream>
#include <sstream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/MPIGrp.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/StreamRead.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ostringstream;
using std::istringstream;
using std::operator<<;
using std::operator>>;

int main(int argc, char* argv[]) 
{

    Parcel p;
    MPIGrp *pgrp;
    int my_id;
    int nprocs;
    int n;
    Flock *flk;
    Flock::iterator iter;
    Swarm *swm;
    Swarm::iterator siter;
    int k;
    ostringstream ooo;
    real lon;
    real lat;
    real z;
    std::string pdata;
    StreamRead *ss;
    istringstream sss;
    
    real time;
    double tag;
    int status;
    static const int SZ = 500;
    char junk[SZ];
    double dx;
    std::string dexpr;
    int spos;
    std::string fmt;
    
    
    pgrp = new MPIGrp(argc, argv);
    
    my_id = pgrp->id();
    nprocs = pgrp->size();

    // initialize the sample parcel
    p.setPos( 30.2345768, 43.4568709 );
    p.setTime(234550.0);
    p.setZ(12.3457869);
    
    n = 104;
    
    pgrp->sync();

    // put the parcel initialization data into a string, but 
    // only for the root process
    for ( k=0; k<n; k++ ) {
        
        
        if ( my_id == 0 ) {
           
           z = 300.0+k;
           lon = k*10.0;
           lat = 80.0 - k*1.0;
           
           ooo << lon << "  " << lat << " " << z << std::endl;
        }
        
    }
    pdata = "";
    if ( my_id == 0 ) {
       pdata = ooo.str();
    }
    sss.str( pdata );
    
    pgrp->sync();
    
    // make an input stream from the parcel data string
    ss = new StreamRead(sss);
    // set its input format
    ss->format("%o %a %v");
    
    pgrp->sync();



    // create a Flock of n parcels
    flk = new Flock( p, pgrp, n, 0);


    // try to read the parcels in from the stream
    // If a non-root parcel attempts a read, it will fail.
    // All parcels should be read in by the root process only.
    ss->apply( *flk );


    delete ss;

    // re-init the string input
    sss.str( pdata );
    
    // make an input stream from the parcel data string
    ss = new StreamRead(sss);
    // set its input format
    ss->format("%o %a %v");
    
    pgrp->sync();

    // create a Swarm of n parcels
    swm = new Swarm( p, pgrp, n, 0);


    // try to read the parcels in from the stream
    // If a non-root parcel attempts a read, it will fail.
    // All parcels should be read in by the root process only.
    ss->apply( *swm );

    delete ss;


    /* Shut down MPI */
    //MPI_Finalize();
    pgrp->shutdown();

    // if we got this far, everything is OK
    return 0;

}
