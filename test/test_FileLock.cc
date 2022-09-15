/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/




#include <iostream>
#include <sstream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/FileLock.hh"

#include <fcntl.h>

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::operator<<;
using std::operator>>;

int main() 
{
    FileLock locker, locker2;
    std::string fname, lockname;
    std::ofstream* out;
    std::ofstream* out2;
    std::ifstream* in;
    std::ifstream* in2;
    int status;
    
    fname = "ThisIsALockTest.data";
    
    lockname = fname + ".lock";
    
    remove(fname.c_str());
    remove(lockname.c_str());
    
    //====================  method openw
    out = locker.openw(fname);
    *out << "foo";

    // does it really lock?
    status = 0;
    try {
       out2 = locker2.openw(fname);
    } catch ( FileLock::badLockExists  err ) {
       status = 1;
    }
    if ( status == 0 ) {
       cerr << "Second (write) lock succeeded when it should have failed" << endl;
       exit(1);
    }   
    status = 0;
    try {
       out2 = locker2.openw(fname, -1);
    } catch ( FileLock::badLockExists  err ) {
       status = 1;
    }
    if ( status == 0 ) {
       cerr << "Third (write) lock succeeded when it should have failed" << endl;
       exit(1);
    }   

    //====================  method openr
    status = 0;
    try {
       in = locker2.openr(fname);
    } catch ( FileLock::badLockExists  err ) {
       status = 1;
    }
    if ( status == 0 ) {
       cerr << "Second (read) lock succeeded when it should have failed" << endl;
       exit(1);
    } 
    
    // openr on nonexistent file
    status = 0;
    try { 
       in = locker2.openr("thisfiledoesnotexist");
    } catch ( FileLock::badNotReadable err ) {
       status = 1;
    }
    if ( status == 0 ) {
       cerr << "read open did not fail on nonexistent file" << endl;
       exit(1);
    }   
          

    //====================  method closer
    locker.closer(out);    
    
    
    
    // now try reading
    in = locker.openr(fname, 0);
    status = 0;
    try {
       in2 = locker2.openr(fname);
    } catch ( FileLock::badLockExists  err ) {
       status = 1;
    }
    if ( status == 0 ) {
       cerr << "Read lock 1 succeeded when it should have failed" << endl;
       exit(1);
    }      
    locker.closer(in);    
    
    // now try it with more permissiveness
    in = locker.openr(fname, -1);
    status = 0;
    try {
       in2 = locker2.openr(fname,-1);
    } catch ( FileLock::badLockExists  err ) {
       status = 1;
    }
    if ( status != 0 ) {
       cerr << "Permissive reading did not work" << endl;
       exit(1);
    }      
    locker.closer(in2);    
    locker.closer(in);    
    
    remove(fname.c_str());
    
    
    exit(0);
}
    
