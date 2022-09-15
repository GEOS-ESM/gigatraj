/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/



/*!
     Test program for the MpiGrp parallel processing class
*/


#include <stdlib.h>     
#include <math.h>
#include <iostream>
#include <string>

#include "mpi.h"

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MPIGrp.hh"
#include "gigatraj/Parcel.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;

int main(int argc, char* argv[]) 
{
    ProcessGrp *grp_a, *grp_b, *grp_c, *grp_d;
    MPIGrp *grp_mpi;
    real rv1, rv2;
    int  iv1, iv2;
    double dv1, dv2;
    int status;
    int i;
    int imroot ;
    int grpsize;
    int me;
    ProcessGrp::ProcessRole ptype;
    Parcel* p1;
    Parcel* p2;
    real lon, lat, z;
    int pscpy;
    int list[3];
    MPI_Comm mpi_comm;
    MPI_Group mpi_group;
    int mpi_flags;
    string str;
    
    /* start up MPI */
    MPI_Init(&argc, &argv);
   

    /* test MPI functions */
    grp_mpi = new MPIGrp();
    
    // =========================  method MPIcomm
    mpi_comm = grp_mpi->MPIcomm();
    
    // =========================  method MPIgroup
    mpi_group = grp_mpi->MPIgroup();

    // =========================  method getflags
    mpi_flags = grp_mpi->getflags();
    
    delete grp_mpi;

    /* make a group of all the processors */
    grp_a = new MPIGrp();
    
    // =========================  method size
    grpsize = grp_a->size();
    if ( grpsize <= 0 ) {
       cerr << "[" << me << "] Base group size: " << grpsize  << endl;
       MPI_Finalize();
       exit(1);
    }
    
    // =========================  method is_root
    // =========================  method id
    // =========================  method root_id
    imroot = grp_a->is_root(); 
    me = grp_a->id();    
    if ( imroot && me != grp_a->root_id() ) {
       cerr << "[" << me << "] Root ID does not match: " << me << " vs. " << grp_a->root_id() << endl;
       MPI_Finalize();
       exit(1);
    } 
    if ( ! imroot && me == grp_a->root_id() ) {
       cerr << "[" << me << "] Root ID matches when it shouldn't: " << me << endl;
       MPI_Finalize();
       exit(1);
    } 
    
    // =========================  method setType
    // =========================  method sync
    // =========================  method send_reals
    // =========================  method send_doubles
    // =========================  method send_ints
    // =========================  method send_string
    // =========================  method receive_reals
    // =========================  method receive_doubles
    // =========================  method receive_ints
    // =========================  method receive_string
    // test transmission of numeric values from one
    // processor to another
    if ( imroot ) {
       // these values will be transmitted between processes
       rv1 = 345.6;
       dv1 = 23425.43;
       iv1 = 42;
       str = "Just testing";
       
       grp_a->setType( me, ProcessGrp::PGrpRole_Coordinator );
    
       //cerr << "I am root (" << me << ")" << endl;

       grp_a->sync();
       
       for ( int i = 0; i < grp_a->size(); i++ ) {
          if ( me != i ) {
             
             grp_a->send_reals( i, 1, &rv1 );
             grp_a->send_doubles( i, 1, &dv1 );
             grp_a->send_ints( i, 1, &iv1 );
             grp_a->send_string( i, str );
             
          }
       }
    } else {
       rv1 = -1.0;
       dv1 = -1.0;
       iv1 = -1;
       str = "nuthin";       
       grp_a->setType( me, ProcessGrp::PGrpRole_Tracer );

       //cerr << "I am NOT root (" << me << ")" << endl;
       
       grp_a->sync();
       
       grp_a->receive_reals( 0, 1, &rv1 );
       grp_a->receive_doubles( 0, 1, &dv1 );
       grp_a->receive_ints( 0, 1, &iv1 );
       grp_a->receive_string( 0, &str );
       
       
    }    
    //cerr << "rv1=" << rv1 << ", dv1=" << dv1 << ", iv1=" << iv1 << endl;
    if ( mismatch(rv1, 345.6) ) {
       cerr << "[" << me << "] Bad rv1 transmission: 345.6 != " << rv1 << endl;
       MPI_Finalize();
       exit(1);
    } 
    if ( mismatch(dv1, 23425.43 ) ) {
       cerr << "[" << me << "] Bad dv1 transmission:  != " << dv1 << endl;
       MPI_Finalize();
       exit(1);
    }     
    if ( mismatch(iv1, 42 ) ) {
       cerr << "[" << me << "] Bad iv1 transmission:  != " << iv1 << endl;
       MPI_Finalize();
       exit(1);
    } 
    if ( str.compare("Just testing") != 0 ) {
       cerr << "[" << me << "] Bad str transmission:  != " << iv1 << endl;
       exit(1);
    }
    

    // =========================  method type
    ptype = grp_a->type();
    if ( imroot && ptype != ProcessGrp::PGrpRole_Coordinator ) {
       cerr << "[" << me << "] Root process Type is wrong" << endl;
       MPI_Finalize();
       exit(1);
    } 
    if ( ! imroot && ptype != ProcessGrp::PGrpRole_Tracer ) {
       cerr << "[" << me << "] Non-Root process Type is wrong" << endl;
       MPI_Finalize();
       exit(1);
    } 
    

    // =========================  method subgroup
    // =========================  method belongs
    // test creating a subgroup
    grp_b = grp_a->subgroup(2);
    if ( grp_b->belongs() ) {
    
       if ( me >= 2 ) { 
          cerr << "[" << me << "] is in subgroup b when it should NOT be!" << endl;
          MPI_Finalize();
          exit(1);
       } 

       // test communicating through this subgroup
       if ( grp_b->is_root() ) {
          iv1 = 56;
          for ( int i=0; i < grp_b->size(); i++ ) {
              if ( i != grp_b->id() ) {
                 grp_b->send_ints( i, 1, &iv1 );
              }
          }    
       
       } else {
          
          grp_b->receive_ints( 0, 1, &iv1 );

       }
       if ( mismatch(iv1, 56 ) ) {
          cerr << "[" << me << "] Bad iv1 transmission in subgroup b:  != " << iv1 << endl;
          MPI_Finalize();
          exit(1);
       } 
       
       
    } else {
    
       if ( me < 2 ) { 
          cerr << "[" << me << "] is NOT in subgroup b when it should be!" << endl;
          MPI_Finalize();
          exit(1);
       } 
    
    }

    delete grp_b;


    grp_a->sync();
    
    // test creating a subgroup again, this time with an offset
    if ( grp_a->size() >= 3 ) {
       grp_b = grp_a->subgroup(2,0,1);
       if ( grp_b->belongs() ) {
    
          if ( me !=1 && me != 2 ) { 
             cerr << "[" << me << "]offset is in subgroup b when it should NOT be!" << endl;
             MPI_Finalize();
             exit(1);
          } 

          // test communicating through this subgroup
          if ( grp_b->is_root() ) {
             iv1 = 729;
             for ( int i=0; i < grp_b->size(); i++ ) {
                 if ( i != grp_b->id() ) {
                    grp_b->send_ints( i, 1, &iv1 );
                 }
             }    
          
          } else {
             
             grp_b->receive_ints( 0, 1, &iv1 );

          }
          if ( mismatch(iv1, 729 ) ) {
             cerr << "[" << me << "]offset Bad iv1 transmission in subgroup b:  != " << iv1 << endl;
             MPI_Finalize();
             exit(1);
          } 
          
          
       } else {
    
          if ( me == 1 || me == 2 ) { 
             cerr << "[" << me << "]offset is NOT in subgroup b when it should be!" << endl;
             MPI_Finalize();
             exit(1);
          } 
    
       }

       delete grp_b;
    }

    grp_a->sync();

    // =========================  method subgroup #2
    // test creating a subgroup again, this time with an explicit list
    if ( grp_a->size() >= 3 ) {
    
       list[0] = 2;
       list[1] = 0;
    
       grp_b = grp_a->subgroup(2,list);
       if ( grp_b->belongs() ) {
    
          if ( me !=0 && me != 2 ) { 
             cerr << "[" << me << "]list is in subgroup b when it should NOT be!" << endl;
             MPI_Finalize();
             exit(1);
          } 

          // test communicating through this subgroup
          if ( grp_b->is_root() ) {
             iv1 = 1385;
             for ( int i=0; i < grp_b->size(); i++ ) {
                 if ( i != grp_b->id() ) {
                    grp_b->send_ints( i, 1, &iv1 );
                 }
             }    
          
          } else {
             
             grp_b->receive_ints( 0, 1, &iv1 );

          }
          if ( mismatch(iv1, 1385 ) ) {
             cerr << "[" << me << "]list Bad iv1 transmission in subgroup b:  != " << iv1 << endl;
             MPI_Finalize();
             exit(1);
          } 
          
          
       } else {
    
          if ( me == 0 || me == 2 ) { 
             cerr << "[" << me << "]list is NOT in subgroup b when it should be!" << endl;
             MPI_Finalize();
             exit(1);
          } 
          
          // not in group.  Can we still get its size?
          if ( grp_b->size() != 2 ) {
             cerr << "[" << me << "] non-member cannot get size of subgroup b!" << endl;
             MPI_Finalize();
             exit(1);          
          }
    
       }




    
       // test copy 
       grp_c = grp_b->copy();
       if ( grp_b->size() != grp_c->size() ) {
          cerr << "[" << me << "] copy b->c failed!" << endl;
          MPI_Finalize();
          exit(1);
       }

       delete grp_c;
       delete grp_b;
    }

    grp_a->sync();
    
    

    // =========================  method copy
    // test copy 
    grp_b = grp_a->subgroup(2);
    grp_c = grp_b->copy();
    if ( grp_b->size() != grp_c->size() ) {
       cerr << "[" << me << "] copy b->c failed!!" << grp_b->size() << " vs. " << grp_c->size()<< endl;
       MPI_Finalize();
       exit(1);
    }
    delete grp_c;
    delete grp_b;

    // =========================  method sync #2
    grp_a->sync(0);

    
    
    // subgroup again, this time asking for too many processes
    grp_b = grp_a->subgroup( grp_a->size()+1 );
    if ( grp_a->size() != grp_b->size() ) {
       cerr << "[" << me << "] too-big subgroup b created with wrong size!" << endl;
       MPI_Finalize();
       exit(1);
    }
    delete grp_b;

    // subgroup again, again time asking for too many processes, but with strictness this time
    status = 1;
    try {
       grp_b = grp_a->subgroup( grp_a->size()+1, PG_STRICT );
    } catch ( ProcessGrp::badgroupsize err) {
       status = 0;
    }
    if ( status ) {
       cerr << "[" << me << "] too-big subgroup b, but created anyway!" << endl;
       MPI_Finalize();
       exit(1);
    }




    // =========================  method split
    // split the "a" group in two, 
    grp_a->split( 3, &grp_b, &grp_c );
    if ( grp_b->belongs() ) {
       if ( me > 2 ) {
          cerr << "[" << me << "] was split into subgroup b when it should NOT be!" << endl;
          MPI_Finalize();
          exit(1);
       }
    
    }
    if ( grp_c->belongs() ) {
    
       if ( me <= 2 ) {
          cerr << "[" << me << "] was split into subgroup c when it should NOT be!" << endl;
          MPI_Finalize();
          exit(1);
       }
    
    }    
    if ( grp_b->size() != 3 ) {
          cerr << "[" << me << "] split group b has the wrong size: " << grp_b->size() << endl;
          MPI_Finalize();
          exit(1);    
    }
    if ( grp_c->size() != (grp_a->size() - 3) ) {
          cerr << "[" << me << "] split group c has the wrong size: " << grp_c->size() << endl;
          MPI_Finalize();
          exit(1);    
    }
    
    delete grp_b;
    delete grp_c;
    
    // split the "a" group in two again, this time keeping root in both subgroups
    grp_a->split( 3, &grp_b, &grp_c, PG_KEEPROOT );
    if ( grp_b->belongs() ) {
       if ( me > 2 ) {
          cerr << "[" << me << "] was split into subgroup b when it should NOT be (r)!" << endl;
          MPI_Finalize();
          exit(1);
       }
    
    }
    if ( grp_c->belongs() ) {
    
       if ( me <= 2 && me != 0 ) {
          cerr << "[" << me << "] was split into subgroup c when it should NOT be (r)!" << endl;
          MPI_Finalize();
          exit(1);
       }
    
    }    
    if ( grp_b->size() != 3 ) {
          cerr << "[" << me << "] split group b (r) has the wrong size: " << grp_b->size() << endl;
          MPI_Finalize();
          exit(1);    
    }
    if ( grp_c->size() != (grp_a->size() - 3 + 1) ) {
          cerr << "[" << me << "] split group c (r) has the wrong size: " << grp_c->size() << endl;
          MPI_Finalize();
          exit(1);    
    }
    
    delete grp_b;
    delete grp_c;


    // split the "a" group in two again, this time asking for the first group to be too big
    grp_a->split( grp_a->size()+1, &grp_b, &grp_c );
    if ( grp_c != NULLPTR ) {
       cerr << "[" << me << "] too-big split group c is non-NULL!" << endl;
       MPI_Finalize();
       exit(1);    
    }
    delete grp_b;

    status = 1;
    try {
       grp_a->split( grp_a->size()+1, &grp_b, &grp_c, PG_KEEPROOT );
    } catch ( ProcessGrp::badgroupsize err) {
       status = 0;
    }
    if ( status ) {
       cerr << "[" << me << "] too-big split group b, but created anyway!" << endl;
       MPI_Finalize();
       exit(1);
    }



    // test interprocess send/receive of parcels
    p1 = new Parcel;
    p1->setPos( 11.0, 22.0);
    p1->setZ(33.0);
    p2 = new Parcel;
    p2->setPos( 44.0, 55.0);
    p2->setZ(66.0);

    pscpy = 0;
    // =========================  method Parcel::send (deferred from test_Parcel)
    // =========================  method Parcel::receive (deferred from test_Parcel)
    if ( me == 0 ) {
       // root: send the p2 to p1
       p2->send( grp_a, 1 );
    } else {
       if ( me == 1 ) {
          // receive the new value from root
          p1->receive( grp_a, 0 );
          pscpy = 1; 
       }
    }  
    
    z = p1->getZ();
    p1->getPos( &lon, &lat );
    if ( pscpy == 0 ) {    
       if ( mismatch( lon, 11.0 ) || mismatch( lat, 22.0 ) || mismatch(z, 33.0) ) {
          cerr << "Bad p1 non-copy: " << lat << " " << lon << " " << z << endl;
          exit(1);
       }
    } else {
       if ( mismatch( lon, 44.0 ) || mismatch( lat, 55.0 ) || mismatch(z, 66.0) ) {
          cerr << "Bad p1 copy: " << lat << " " << lon << " " << z << endl;
          exit(1);
       }
    }
    
    z = p2->getZ();
    p2->getPos( &lon, &lat );
    if ( mismatch( lon, 44.0 ) || mismatch( lat, 55.0 ) || mismatch(z, 66.0) ) {
       cerr << "Bad p2 non-copy: " << lat << " " << lon << " " << z << endl;
       exit(1);
    }
    delete p1;
    delete p2;


    delete grp_a;

    /* Shut down MPI */
    MPI_Finalize();

    // if we got this far, everything is OK
    exit(0);

}
    
