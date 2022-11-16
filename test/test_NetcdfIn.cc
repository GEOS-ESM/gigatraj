/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


// Unfinished


#include <iostream>
#include <fstream>
#include <sstream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/NetcdfIn.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::cout;
using std::endl;
using std::string;


int main() 
{
    NetcdfIn input;
    string fyle;
    string vert;
    string basetime;
    double t0;
    size_t np;
    Parcel p;
    Parcel* ps;
    std::vector<Parcel> vps;
    Flock *fps;
    real lat,lon,z;
    int nxt;
    int i;
    
    
    
    
    //input.debug( 100 );
    
    input.filename("test_NetcdfIn_data_01.nc4");
    fyle = input.filename();
    
    if ( fyle != "test_NetcdfIn_data_01.nc4" ) {
       cerr << "failed to set file name failed " << endl;
       exit(1);      
    }
    
    vert = input.vertical();
    if ( vert != "" ) {
       cerr << "the default verticak coordinate is not the null string:" << vert << endl;
       exit(1);      
    }
    
    input.open();
    
    basetime = input.cal();
    if ( basetime != "2022-07-15T00:00:00" ) {
       cerr << "the basetime is " << basetime << " instead of 2022-07-15T00:00:00" << endl;
       exit(1);          
    }

    t0 = input.time();
    if ( t0 != 0.0 ) {
       cerr << "the parcel time is " << t0 << " instead of 0" << endl;
       exit(1);          
    }
    
    np = input.n_parcels();
    if ( np != 10 ) {
       cerr << "the number of pcercels is " << np << " instead of 10" << endl;
       exit(1);          
    }
    
    vert = input.vertical();
    if ( vert != "pressure" ) {
       cerr << "the vertical coordinate is " << vert << " instead of 'pressure'" << endl;
       exit(1);              
    }
    
    for ( i=0; i < np; i++ ) {
        
        nxt = input.next_parcel();
        if ( nxt != i ) {
          cerr << "the next parcel index is " << nxt << " instead of " << i << endl;
          exit(1);                         
        }
        
        input.apply( p );
    
        p.getPos( &lon, &lat );
        z = p.getZ();
        
        if ( i == 0 ) {
           if ( mismatch( lon, 117.0 ) 
             || mismatch( lat, 0.0 )
             || mismatch( z, 70.0 ) ) {
             
              cerr << " misread parcel at i=" << i << ": "
              << "( " << lon << ", " << lat << ", " << z << " ) "
              << " instead of ( 117, 0, 70 )" << endl;
      
              exit(1);
             
           }   
        }
        if ( i == 1 ) {
           if ( mismatch( lon, 109.981 ) 
             || mismatch( lat, 4.44444 )
             || mismatch( z, 70.0 ) ) {
             
              cerr << " misread parcel at i=" << i << ": "
              << "( " << lon << ", " << lat << ", " << z << " ) "
              << " instead of ( 109.981, 4.44444, 70 )" << endl;
      
              exit(1);
             
           }   
        }
        if ( i == 9 ) {
           if ( mismatch( lon, 117.0 ) 
             || mismatch( lat, 40.0 )
             || mismatch( z, 70.0 ) ) {
             
              cerr << " misread parcel at i=" << i << ": "
              << "( " << lon << ", " << lat << ", " << z << " ) "
              << " instead of ( 117, 0, 70 )" << endl;
      
              exit(1);
             
           }   
        }
    
    }
    
    nxt = input.next_parcel();
    if ( nxt != -1 ) {
      cerr << "the no-more-parcels idnex is " << nxt << " instead of " << -1 << endl;
      exit(1);
    }
    
    input.reset();
    nxt = input.next_parcel();
    if ( nxt != 0 ) {
      cerr << "the reset parcel idnex is " << nxt << " instead of " << 0 << endl;
      exit(1);
    }

    // Now try arrays
    ps = new Parcel[np];
    input.apply( ps, 2 );
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
    
    // Now try a vector
    input.reset();
    
    input.apply( vps );
    i = 1;
    vps[i].getPos( &lon, &lat );
    z = vps[i].getZ();
    if ( mismatch( lon, 109.981 ) 
      || mismatch( lat, 4.44444 )
      || mismatch( z, 70.0 ) ) {
      
       cerr << " misread vector parcel[" << i << "]: "
       << "( " << lon << ", " << lat << ", " << z << " ) "
       << " instead of ( 109.981, 4.44444, 70 )" << endl;
    
       exit(1);
      
    }   
    
    // Now try a Flock
    input.reset();
    
    fps = new Flock(p, np);
    input.apply( *fps );
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
    
    
    input.close();

}
