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
#include "gigatraj/Flock.hh"
#include "gigatraj/PGenNetcdf.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::cout;
using std::endl;
using std::string;


int main() 
{
    PGenNetcdf pgen;
    string fyle;
    string vert;
    string basetime;
    double t0;
    size_t np;
    Parcel p;
    Parcel* ps;
    std::vector<Parcel> *vps;
    Flock *fps;
    real lat,lon,z;
    int nxt;
    int i;
    std::string fname;
    
    
    
    
    //pgen.debug( 100 );

    fname = datadir("srcdir") + "test_NetcdfIn_data_01.nc4";
    
    vert = pgen.vertical();
    if ( vert != "" ) {
       cerr << "the default verticak coordinate is not the null string:" << vert << endl;
       exit(1);      
    }
    
    fyle = pgen.filename();    
    if ( fyle != "" ) {
       cerr << "Initial filename not empty: " << fyle << endl;
       exit(1);      
    }


    // Now try arrays
    ps = pgen.create_array( p, &np, fname );
    i = 1;
    ps[i].getPos( &lon, &lat );
    z = ps[i].getZ();
    if ( mismatch( lon, 109.981 ) 
      || mismatch( lat, 4.44444 )
      || mismatch( z, 70.0 ) ) {
      
       cerr << " misread array parcel[" << i << "]: "
       << "( " << lon << ", " << lat << ", " << z << " ) "
       << " instead of ( 109.981, 4.44444, 70 )" << endl;
    
       exit(1);
      
    }   
    delete[] ps;

    fyle = pgen.filename();    
    if ( fyle != fname ) {
       cerr << "failed to set file name failed: " << fyle << endl;
       exit(1);      
    }
    basetime = pgen.cal();
    if ( basetime != "2022-07-15T00:00:00" ) {
       cerr << "the basetime is " << basetime << " instead of 2022-07-15T00:00:00" << endl;
       exit(1);          
    }

    t0 = pgen.time();
    if ( t0 != 0.0 ) {
       cerr << "the parcel time is " << t0 << " instead of 0" << endl;
       exit(1);          
    }
    
    np = pgen.n_parcels();
    if ( np != 10 ) {
       cerr << "the number of pcercels is " << np << " instead of 10" << endl;
       exit(1);          
    }
    
    vert = pgen.vertical();
    if ( vert != "pressure" ) {
       cerr << "the vertical coordinate is " << vert << " instead of 'pressure'" << endl;
       exit(1);              
    }
    

    // Now try a vector
    vps = pgen.create_vector( p, fname );
    i = 1;
    (*vps)[i].getPos( &lon, &lat );
    z = (*vps)[i].getZ();
    if ( mismatch( lon, 109.981 ) 
      || mismatch( lat, 4.44444 )
      || mismatch( z, 70.0 ) ) {
      
       cerr << " misread vector parcel[" << i << "]: "
       << "( " << lon << ", " << lat << ", " << z << " ) "
       << " instead of ( 109.981, 4.44444, 70 )" << endl;
    
       exit(1);
      
    }   
    
    delete vps;
    
    fps = pgen.create_Flock(  p, fname );   
    i = 1;
    p = fps->get(i);
    p.getPos( &lon, &lat );
    z = p.getZ();
    if ( mismatch( lon, 109.981 ) 
      || mismatch( lat, 4.44444 )
      || mismatch( z, 70.0 ) ) {
      
       cerr << " misread vector parcel[" << i << "]: "
       << "( " << lon << ", " << lat << ", " << z << " ) "
       << " instead of ( 109.981, 4.44444, 70 )" << endl;
    
       exit(1);
      
    }   
    delete fps;

}
