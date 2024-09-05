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
    
