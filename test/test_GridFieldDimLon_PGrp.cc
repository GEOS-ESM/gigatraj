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
#include "gigatraj/GridFieldDimLon.hh"
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
    GridFieldDimLon grid, grid2;
    std::vector<real> ozs;
    std::vector<real> odata;
    int i, j, idx;
    int n1, n2, n1a, n2a;
    std::vector<real> dim;
    std::vector<real> dima;
    real val;
    int ival, ival2;
    double tyme;
    string str;
    int ia[3], ja[3];
    real da[3];
    ProcessGrp *grp;
    int done_goal;
    int done_count;
    int client_status;
    int src;
    int n;
    real *vals;
    int *coords;

    // Create a MPI processor group
#ifdef USING_MPI
    grp = new MPIGrp(argc, argv);
#else
    grp = new SerialGrp();
#endif


    
    odata.reserve(71);
    for (i=0; i<71; i++ ) {
       odata.push_back( i/3.0 );
    }   

  
    // set up the grid variable
    grid.set_quantity("altitude");
    grid.set_units("km");
    grid.load( odata );
    
    // =========================  method setPgroup 
    grid.setPgroup(grp, 1);
    
    
    // make a copy
    grid2 = grid;
    if ( ! grid.compatible(grid2) ) {
       cerr << " copied grid 2 is incompatible with grid" << endl;
       grp->shutdown();
       exit(1);              
    }
    
    if ( grp->id() == 1 ) {
       // This is the central met processor
       // listen to and satisfy data requests from other processors
       
       // =========================  method svr_listen
       // =========================  method svr_send_vals
       // =========================  method svr_send_meta
       grid.svr_listen();
    
    } else {
       // make data requests to central met processor
       
#ifdef USING_MPI
       // first, clear the met object's data 
       // (But only if we are actually using MPI!)
       grid.clear(); 
       if ( grid.compatible(grid2) ) {
          cerr << " grid is still compatible with grid2 after being cleared" << endl;
          grid.svr_done();
          grp->shutdown();
          exit(1);              
       }
#endif
       
       // re-load the metadata
       // =========================  method receive_meta
       grid.ask_for_meta();
       grid.receive_meta();
       if ( ! grid.match(grid2) ) {
          cerr << "[" << grp->id() << "] grid metadata does not match grid2's after being re-loaded from remote" << endl;
          grid.svr_done();
          grp->shutdown();
          exit(1);              
       }
       
       
       // Now load the metadata from the remote end
       
       ia[0] = 2;
       ia[1] = 32;
       ia[2] = 7;
       da[0] = -1.0;
       da[1] = -1.0;
       da[2] = -1.0;
       // =========================  method gridpoints
       grid.ask_for_data();
       grid.gridpoints( 3, ia, da);
       if ( da[0] != grid2( ia[0] ) 
        ||  da[1] != grid2( ia[1] ) 
        ||  da[2] != grid2( ia[2] ) ) {
          cerr << " gridpoints results do not match value results" << endl;
          grid.svr_done();
          grp->shutdown();
          exit(1);        
       }
       
       // send the "I am done" signal
       // =========================  method svr_done
       grid.svr_done();

    }

    grid.sync();


    // =========================  method svr_start
    if ( grid.svr_start() ) {
       // met client

       grid.ask_for_meta();
       grid.receive_meta();

       grid.svr_done();

    }
    
    grp->shutdown();
      
    exit(0);

}
