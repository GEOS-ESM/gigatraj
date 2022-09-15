/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


// Tests the trace() function against a vector of Parcels


#include <iostream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/Swarm.hh"
#include "gigatraj/trace.hh"
#include "gigatraj/MetSBRot.hh"
#include "gigatraj/Earth.hh"
#include "gigatraj/PGenFile.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cout;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::istringstream;
using std::operator<<;
using std::operator>>;

int main() 
{

    Parcel p;
    Parcel cp;
    Earth e;
    MetSBRot metsrc(30.0);
    real lon;
    real lat;
    real z;
    real time;
    real dt;
    real flon;
    real duration;
    int status;
    static PGenFile gen;
    
    dt = 0.01 ; // 15-minute time step
    
    // set the meteorological data source
    // (setting this for one parcel sets it for all parcels)
    p.setMet( metsrc );

    // read in a set of parcels for comparison with traced parcels
    std::vector<Parcel>* vbunch0 = gen.create_vector(p, "test_traj001.dat");

    cout.setf(std::ios::fixed);

    // loop through each of these different durations
    // (currently only one: 5 days)
    for ( duration=5.0; duration <= 5.0 ; duration += 1.0 ) {
       real max_d, min_d, avg_d, med_d, sprd_d;
       
       std::cout << "Duration=" << duration << ": " << std::endl;
       

       /////////////////////////////////////////////////////////////////////////

       // initialize a vector container of parcels
       std::vector<Parcel>* vbunch = gen.create_vector(p, "test_traj001.dat");

       // trace them forward
       for ( time=0.0; time<=duration ; time+=dt )
       {
           trace(vbunch, dt );
       }
       // trace them backward
       for ( time=duration; time>=0.0 ; time-=dt )
       {
           trace(vbunch, - dt );
       }
       
       // compare the parcels after the forward/backward trace 
       // with the original parcels 
       cmp_pos( *vbunch0, *vbunch, &max_d, &min_d, &avg_d, &med_d, &sprd_d );

       std::cout << "  vector " << max_d << "  " << min_d << "  " << avg_d << "  " << med_d << "  " << sprd_d  << std::endl;
       
       if ( max_d > 10.0 || avg_d > 0.5 || med_d > 0.5 ) {
          if ( max_d > 10.0 ) {
             std::cerr << "Maximum error is too large: " << max_d << std::endl;
          }
          if ( avg_d > 0.5 ) {
             std::cerr << "Average error is too large: " << avg_d << std::endl;
          }
          if ( med_d > 0.5 ) {
             std::cerr << "Median error is too large: " << med_d << std::endl;
          }
          exit(1);
       }
   
       delete vbunch;


       /////////////////////////////////////////////////////////////////////////
       
       // initialize a list container of parcels
       std::list<Parcel>* Lbunch = gen.create_list(p, "test_traj001.dat");
       // trace them forward
       for ( time=0.0; time<=duration ; time+=dt )
       {
           trace(Lbunch, dt );
       }
       // trace them backward
       for ( time=duration; time>=0.0 ; time-=dt )
       {
           trace(Lbunch, - dt );
       }
       
             
       // copy from the list to a vector, for comparison
       vbunch = new std::vector<Parcel>;
       for ( std::list<Parcel>::iterator Lfrom = Lbunch->begin();
               Lfrom != Lbunch->end();
               Lfrom++ ) {
           vbunch->push_back( *Lfrom );
       }
       
       // compare the parcels after the forward/backward trace 
       // with the original parcels 
       cmp_pos( *vbunch0, *vbunch, &max_d, &min_d, &avg_d, &med_d, &sprd_d );

       std::cout << "    list " << max_d << "  " << min_d << "  " << avg_d << "  " << med_d << "  " << sprd_d  << std::endl;
       
       if ( max_d > 10.0 || avg_d > 0.5 || med_d > 0.5 ) {
          if ( max_d > 10.0 ) {
             std::cerr << "Maximum error is too large: " << max_d << std::endl;
          }
          if ( avg_d > 0.5 ) {
             std::cerr << "Average error is too large: " << avg_d << std::endl;
          }
          if ( med_d > 0.5 ) {
             std::cerr << "Median error is too large: " << med_d << std::endl;
          }
          exit(1);
       }
   
       delete Lbunch;


       /////////////////////////////////////////////////////////////////////////
       
       // initialize a deque container of parcels
       std::deque<Parcel>* Dbunch = gen.create_deque(p, "test_traj001.dat");
       // trace them forward
       for ( time=0.0; time<=duration ; time+=dt )
       {
           trace(Dbunch, dt );
       }
       // trace them backward
       for ( time=duration; time>=0.0 ; time-=dt )
       {
           trace(Dbunch, - dt );
       }
       
             
       // copy from the deque to a vector, for comparison
       vbunch = new std::vector<Parcel>;
       for ( std::deque<Parcel>::iterator Dfrom = Dbunch->begin();
               Dfrom != Dbunch->end();
               Dfrom++ ) {
           vbunch->push_back( *Dfrom );
       }
       
       // compare the parcels after the forward/backward trace 
       // with the original parcels 
       cmp_pos( *vbunch0, *vbunch, &max_d, &min_d, &avg_d, &med_d, &sprd_d );

       std::cout << "   deque " << max_d << "  " << min_d << "  " << avg_d << "  " << med_d << "  " << sprd_d  << std::endl;
       
       if ( max_d > 10.0 || avg_d > 0.5 || med_d > 0.5 ) {
          if ( max_d > 10.0 ) {
             std::cerr << "Maximum error is too large: " << max_d << std::endl;
          }
          if ( avg_d > 0.5 ) {
             std::cerr << "Average error is too large: " << avg_d << std::endl;
          }
          if ( med_d > 0.5 ) {
             std::cerr << "Median error is too large: " << med_d << std::endl;
          }
          exit(1);
       }
   
       delete Dbunch;



       /////////////////////////////////////////////////////////////////////////
       
       // initialize a Flock container of parcels
       Flock* Fbunch = gen.create_Flock(p, "test_traj001.dat");
       // trace them forward
       for ( time=0.0; time<=duration ; time+=dt )
       {
           trace(Fbunch, dt );
       }
       // trace them backward
       for ( time=duration; time>=0.0 ; time-=dt )
       {
           trace(Fbunch, - dt );
       }
       
             
       // copy from the Flock to a vector, for comparison
       vbunch = new std::vector<Parcel>;
       for ( Flock::iterator Ffrom = Fbunch->begin();
               Ffrom != Fbunch->end();
               Ffrom++ ) {
           vbunch->push_back( *Ffrom );
       }
       
       // compare the parcels after the forward/backward trace 
       // with the original parcels 
       cmp_pos( *vbunch0, *vbunch, &max_d, &min_d, &avg_d, &med_d, &sprd_d );

       std::cout << "   Flock " << max_d << "  " << min_d << "  " << avg_d << "  " << med_d << "  " << sprd_d  << std::endl;
       
       if ( max_d > 10.0 || avg_d > 0.5 || med_d > 0.5 ) {
          if ( max_d > 10.0 ) {
             std::cerr << "Maximum error is too large: " << max_d << std::endl;
          }
          if ( avg_d > 0.5 ) {
             std::cerr << "Average error is too large: " << avg_d << std::endl;
          }
          if ( med_d > 0.5 ) {
             std::cerr << "Median error is too large: " << med_d << std::endl;
          }
          exit(1);
       }
   
       delete Fbunch;


       /////////////////////////////////////////////////////////////////////////
       
       // initialize a Swarm pseudo-container of parcels
       Swarm* Sbunch = gen.create_Swarm(p, "test_traj001.dat");
       // trace them forward
       for ( time=0.0; time<=duration ; time+=dt )
       {
           trace(Sbunch, dt );
       }
       // trace them backward
       for ( time=duration; time>=0.0 ; time-=dt )
       {
           trace(Sbunch, - dt );
       }
       
             
       // copy from the Swarm to a vector, for comparison
       vbunch = new std::vector<Parcel>;
       for ( Swarm::iterator Sfrom = Sbunch->begin();
               Sfrom != Sbunch->end();
               Sfrom++ ) {
           cp = *Sfrom;
           vbunch->push_back( cp );
       }
       
       // compare the parcels after the forward/backward trace 
       // with the original parcels 
       cmp_pos( *vbunch0, *vbunch, &max_d, &min_d, &avg_d, &med_d, &sprd_d );

       std::cout << "   Swarm " << max_d << "  " << min_d << "  " << avg_d << "  " << med_d << "  " << sprd_d  << std::endl;
       
       if ( max_d > 10.0 || avg_d > 0.5 || med_d > 0.5 ) {
          if ( max_d > 10.0 ) {
             std::cerr << "Maximum error is too large: " << max_d << std::endl;
          }
          if ( avg_d > 0.5 ) {
             std::cerr << "Average error is too large: " << avg_d << std::endl;
          }
          if ( med_d > 0.5 ) {
             std::cerr << "Median error is too large: " << med_d << std::endl;
          }
          exit(1);
       }
   
       delete Sbunch;

       /////////////////////////////////////////////////////////////////////////
       
    }


    delete vbunch0;
    exit(0);
}
