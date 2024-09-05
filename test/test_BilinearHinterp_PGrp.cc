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
#include "gigatraj/GridLatLonField3D.hh"
#include "gigatraj/GridLatLonFieldSfc.hh"
#include "gigatraj/LinearVinterp.hh"
#include "gigatraj/BilinearHinterp.hh"
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
    GridLatLonField3D grid, grid2;
    GridLatLonFieldSfc gridsfc;
    std::vector<real> olons;
    std::vector<real> olats;
    std::vector<real> opres;
    std::vector<real> odata;
    std::vector<real> vlons;
    std::vector<real> vlats;
    std::vector<real> vzs;
    std::vector<real> *vdata;
    HLatLonInterp *interp;
    Vinterp *vin;
    ProcessGrp *grp;
    real val;
    int i,j,k;
    
    /* start up MPI */
#ifdef USING_MPI
    grp = new MPIGrp(argc, argv);
#else
    grp = new SerialGrp();
#endif
   
    //cerr << "This is process " << grp->id() << " of " << grp->size() << endl;

    olons.reserve(72);
    for (i=0; i<72; i++ ) {
       olons.push_back(i*5.0);
    }   
    olats.reserve(37);
    for (i=0; i<37; i++ ) {
       olats.push_back( i*5.0-90.0 );
    }   
    opres.reserve(18);
    opres.push_back( 1000.0);
    opres.push_back(  850.0);
    opres.push_back(  700.0);
    opres.push_back(  500.0);
    opres.push_back(  400.0);
    opres.push_back(  300.0);
    opres.push_back(  250.0);
    opres.push_back(  200.0);
    opres.push_back(  150.0);
    opres.push_back(  100.0);
    opres.push_back(   70.0);
    opres.push_back(   50.0);
    opres.push_back(   30.0);
    opres.push_back(   10.0);
    opres.push_back(    5.0);
    opres.push_back(    2.0);
    opres.push_back(    1.0);
    opres.push_back(    0.4);

    odata.reserve(72*37*18);
    for (k=0; k<18; k++ ) {
    for (j=0; j<37; j++ ) {
    for (i=0; i<72; i++ ) {
       val =  COS(2.0*PI*static_cast<real>(j)/37.0 )
            * SIN(2.0*PI*static_cast<real>(i)/72.0*3.0 )
            * static_cast<real>(k) ;
       odata.push_back( val );
    }   
    }   
    }   

  
    // set up the grid variable
    grid.set_quantity("air temperature");
    grid.set_units("K");
    grid.set_fillval(-1234.0);
    grid.set_vertical("air pressure");
    grid.set_vunits("mb");
    grid.set_time( 3.14159, "Grue 34, year of the kumquat");
    grid.load( olons, olats, opres, odata );
    

    // =========================  method setPgroup 
    grid.setPgroup(grp, 1);

    interp = new BilinearHinterp();
    vin = new LinearVinterp();

    if ( grp->id() == 1 ) {
       // This is the central met processor
       // listen to and satisfy data requests from other processors
       
    
       // =========================  method svr_listen
       // =========================  method svr_send_vals
       // =========================  method svr_send_meta
       grid.svr_listen();
    
    } else {
       // make data requests to central met processor

       vlons.push_back( -76.3 );
       vlats.push_back( 39.7 );
       vlons.push_back(  0.5 );
       vlats.push_back( -35.0 );
       vlons.push_back(  359.5 );
       vlats.push_back( 90.0 );
       //grid.ask_for_data();
       vdata = interp->calc( vlons, vlats, grid, 5 );
       if ( mismatch( (*vdata)[0], -1.13112 ) 
       || mismatch( (*vdata)[1], -0.0378941 ) 
       || mismatch( (*vdata)[2], -0.127548 ) ) {
          cerr << " Mismatched Array GridLatLonFieldSfc interpolated value: " 
              << -1.13112 << " vs. " << (*vdata)[0] 
              << ", " << -0.0378941 << " vs. " << (*vdata)[1] 
              << ", " << -0.127548 << " vs. " << (*vdata)[2] << endl;
          grp->shutdown();
          exit(1);
       }
       delete vdata;

       vzs.push_back( 225.0 );
       vzs.push_back( 100.0 );
       vzs.push_back( 80.0 );
       //grid.ask_for_data();
       vdata = interp->vinterp(vlons,vlats, vzs, grid, *vin );
       if ( mismatch( (*vdata)[0], -1.47045 ) 
       || mismatch( (*vdata)[1], -0.0682093 ) 
       || mismatch( (*vdata)[2], -0.246592 ) ) {
          cerr << " Mismatched Array GridLatLonField3D interpolated value: " 
              << -1.47045 << " vs. " << (*vdata)[0] 
              << ", " << -0.0682093 << " vs. " << (*vdata)[1] 
              << ", " << -0.246592 << " vs. " << (*vdata)[2] << endl;
          exit(1);
       }
       delete vdata;

       // send the "I am done" signal
       // =========================  method svr_done
       grid.svr_done();

    }

    // =========================  method svr_start
    if ( grid.svr_start() ) {
       // met client

       grid.ask_for_meta();
       grid.receive_meta();

       grid.svr_done();

    }
    

    /* Shut down MPI */
    grp->shutdown();

    delete vin;
    delete interp;
      
    exit(0);

}
