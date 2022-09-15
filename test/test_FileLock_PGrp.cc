
#include <fcntl.h>

#ifdef USING_MPI
#include "mpi.h"
#endif

#include "gigatraj/gigatraj.hh"
#ifdef USING_MPI
#include "gigatraj/MPIGrp.hh"
#else
#include "gigatraj/SerialGrp.hh"
#endif
#include "gigatraj/FileLock.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::operator<<;
using std::operator>>;

int main(int argc, char* argv[]) 
{
    FileLock locker, locker2;
    std::string fname, lockname;
    std::ofstream* out;
    std::ofstream* out2;
    std::ifstream* in;
    std::ifstream* in2;
    int status;
    ProcessGrp *grp;
    int me;
    std::string str;
    
    /* start up MPI */
#ifdef USING_MPI
    grp = new MPIGrp(argc, argv);
#else
    grp = new SerialGrp();
#endif

    me = grp->id();
    //cerr << "This is process " << me << " of " << grp->size() << endl;

    fname = "ThisIsALockTest.data";
    
    lockname = fname + ".lock";
    
    remove(fname.c_str());
    remove(lockname.c_str());
    
    // sync here, so that the root proc's lock 
    // is not removed by some other proc executing
    // the above
    grp->sync();
    
    locker.id = me;
    locker2.id = me*10;
    //locker.debug = 1;
    //locker2.debug = 1;
    
    //===================== method init()
    locker.init(0);
    locker2.init(0);
    
    if ( me == 0 ) {
       // this is the root process
       // open the file for writing with locking
       
       out = locker.openw(fname);
       *out << "foo";

       // note that we have not closed the file yet!
       //cerr << me << ": file opened for writing with locking" << endl;

       // root processor syncs here, after the open 
       //cerr << me << ": pre-sync" << endl;
       grp->sync();
       //cerr << me << ": post-sync" << endl;
       
    } else {      

       // all other processors sync here, before their opens,
       // so that the root process has the chance to do its open.
       //cerr << me << ": pre-sync" << endl;
       grp->sync();
       //cerr << me << ": post-sync" << endl;
       
       // does it really lock?
       status = 0;
       try {
          out2 = locker2.openw(fname);
       } catch ( FileLock::badLockExists  err ) {
          status = 1;
       }
       if ( status == 0 ) {
          cerr << me << ": Second (write) lock succeeded when it should have failed" << endl;
          grp->shutdown();
          exit(1);
       }
       status = 0;
       try {
          out2 = locker2.openw(fname, -1);
       } catch ( FileLock::badLockExists  err ) {
          status = 1;
       }
       if ( status == 0 ) {
          cerr << me << ": Third (write) lock succeeded when it should have failed" << endl;
          grp->shutdown();
          exit(1);
       }   
    
       status = 0;
       try {
          in = locker2.openr(fname);
       } catch ( FileLock::badLockExists  err ) {
          status = 1;
       }
       if ( status == 0 ) {
          cerr << me << ": Second (read) lock succeeded when it should have failed" << endl;
          grp->shutdown();
          exit(1);
       } 
    
    }

    // everybody syncs here again, to
    // give the root proc a chance to close and unlock the file 
    //cerr << me << ": pre-sync" << endl;
    grp->sync();
    //cerr << me << ": post-sync" << endl;

    if ( me == 0 ) {    
        // the root process closes the file and removes the lock
        locker.closer(out);    
        //cerr << me << ": written file now closed and lock released" << endl;    
    } 

    grp->sync();

    //cerr << "Everybody tries to read now" << endl;
    
    
    // now everybody tries to read the file
    try {
       in = locker.openr(fname, -grp->size()*2);
    } catch (...) {
       cerr << me << ": Read lock 1 failed" << endl;
       grp->shutdown();
       exit(1);
    }
    *in >> str;
    locker.closer(in);    
    
    if ( str != "foo" ) {  
       cerr << me << ": Read failed to read the correct string" << endl;
       grp->shutdown();
       exit(1);
    }
    

    // sync here, so that no one tries to read the file
    // after the root proc has removed it
    grp->sync();
    
    // only the root process tries to remove the file
    if ( me == 0 ) {
       remove(fname.c_str());
    }
    

    /* Shut down parallel processing */
    grp->shutdown();

    exit(0);
}
    
