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
#include "gigatraj/FilePath.hh"

#include <stdlib.h>

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::istringstream;
using std::operator<<;
using std::operator>>;

int main() 
{

    FilePath fp1, fp2;
    std::string tval, s1;
    int n, tn;

    // =========================  append(string)
    fp1.append("This");
    
    // =========================  fullpath
    s1 = fp1.fullpath();
    tval = "This";
    if ( s1 != tval ) {
       cerr << "Full path failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }

    fp1.append("is/a/test/");
    s1 = fp1.fullpath();
    tval = "This/is/a/test";
    if ( s1 != tval ) {
       cerr << "Full path 2 failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    
    // =========================  assignment
    fp2 = fp1;
    // =========================  setTop
    fp2.setTop("");
    s1 = fp2.fullpath();
    tval = "/This/is/a/test";
    if ( s1 != tval ) {
       cerr << "Full path failed: " << tval << " vs " << s1 << endl; 
       exit(1);
    }
    
    
    // =========================  directory
    // fp1 = "./" + "This/is/a/test"
    s1 = fp1.directory();
    tval = "This/is/a";
    if ( s1 != tval ) {
       cerr << "directory failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(1);
    tval = "This";
    if ( s1 != tval ) {
       cerr << "directory(1) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(2);
    tval = "This/is";
    if ( s1 != tval ) {
       cerr << "directory(2) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(3);
    tval = "This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(3) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(4);
    tval = "This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(4) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(5);
    tval = "This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(5) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(25);
    tval = "This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(25) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-1);
    tval = "a";
    if ( s1 != tval ) {
       cerr << "directory(-1) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-2);
    tval = "is/a";
    if ( s1 != tval ) {
       cerr << "directory(-2) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-3);
    tval = "This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(-3) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-4);
    tval = "./This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(-4) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-5);
    tval = "./This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(-5) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-25);
    tval = "./This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(-25) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }

    // =========================  clear
    fp1.setTop("wring");
    fp1.clear();
    s1 = fp1.fullpath();
    tval = ".";
    if ( s1 != tval ) {
       cerr << "Cleared Full path failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    
    // more directory tests
    
    // test with a short file name
    fp1.clear();
    fp1.append("This");
    // fp1 = "./" + "This"
    s1 = fp1.directory();
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(1);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(1) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(2);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(2) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(3);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(3) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(4);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(4) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(5);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(5) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(25);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(25) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-1);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(-1) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-2);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(-2) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-3);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(-3) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-4);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(-4) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-5);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(-5) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-25);
    tval = "./";
    if ( s1 != tval ) {
       cerr << "s directory(-25) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }

    // fp2 = "/" + "This/is/a/test"
    s1 = fp2.directory();
    tval = "/This/is/a";
    if ( s1 != tval ) {
       cerr << "/ directory failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(1);
    tval = "/This";
    if ( s1 != tval ) {
       cerr << "directory(1) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(2);
    tval = "/This/is";
    if ( s1 != tval ) {
       cerr << "directory(2) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(3);
    tval = "/This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(3) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(4);
    tval = "/This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(4) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(5);
    tval = "/This/is/a";
    if ( s1 != tval ) {
       cerr << "directory(5) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(25);
    tval = "/This/is/a";
    if ( s1 != tval ) {
       cerr << "/ directory(25) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(-1);
    tval = "a";
    if ( s1 != tval ) {
       cerr << "/ directory(-1) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(-2);
    tval = "is/a";
    if ( s1 != tval ) {
       cerr << "/ directory(-2) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(-3);
    tval = "/This/is/a";
    if ( s1 != tval ) {
       cerr << "/ directory(-3) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(-4);
    tval = "/This/is/a";
    if ( s1 != tval ) {
       cerr << "/ directory(-4) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(-5);
    tval = "/This/is/a";
    if ( s1 != tval ) {
       cerr << "/ directory(-5) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp2.directory(-25);
    tval = "/This/is/a";
    if ( s1 != tval ) {
       cerr << "/ directory(-25) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }

    fp1.setTop("");
    // fp1 = "/" + "This"
    s1 = fp1.directory();
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(1);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(1) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(2);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(2) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(3);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(3) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(4);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(4) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(5);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(5) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(25);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(25) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-2);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(-2) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-1);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(-1) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-2);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s ddirectory(-2) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-3);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(-3) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-4);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(-4) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-5);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(-5) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    s1 = fp1.directory(-25);
    tval = "/";
    if ( s1 != tval ) {
       cerr << "/s directory(-25) failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }

    
    // =========================  basename
    s1 = fp2.basename();
    tval = "test";
    if ( s1 != tval ) {
       cerr << "basename failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }

    
    // =========================  append(FilePath)
    fp1.append("at/end");
    fp2.append(fp1);
    s1 = fp2.fullpath();
    tval = "/This/is/a/test/This/at/end"; 
    if ( s1 != tval ) {
       cerr << "Cleared Full path failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }

    // =========================  prepend(string)
    fp1.clear();
    fp1.prepend("a");
    fp1.prepend("b");
    s1 = fp1.fullpath();
    tval = "b/a"; 
    if ( s1 != tval ) {
       cerr << "prepend path failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    fp1.append("c");
    fp1.prepend("d");
    fp1.append("e");
    s1 = fp1.fullpath();
    tval = "d/b/a/c/e"; 
    if ( s1 != tval ) {
       cerr << "prepend2 path failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    
    // =========================  prepend(FilePath)
    fp1.prepend(fp2);
    s1 = fp1.fullpath();
    tval = "/This/is/a/test/This/at/end/d/b/a/c/e"; 
    if ( s1 != tval ) {
       cerr << "prepend3 path failed: " << tval << " vs " << s1 << endl;
       exit(1);
    }
    
    // =========================  count
    n = fp1.count();
    tn = 12;
    if ( n != tn ) {
       cerr << "pcount failed: " << tn << " vs " << n << endl;
       exit(1);    
    }
    
    // =========================  find(string)
    n = fp1.find("This");
    tn = 0;
    if ( n != tn ) {
       cerr << "find-1 failed: " << tn << " vs " << n << endl;
       exit(1);    
    }
    n = fp1.find("This",2);
    tn = 4;
    if ( n != tn ) {
       cerr << "find-2 failed: " << tn << " vs " << n << endl;
       exit(1);    
    }
    n = fp1.find("This",-2);
    tn = -1;
    if ( n != tn ) {
       cerr << "find-3 failed: " << tn << " vs " << n << endl;
       exit(1);    
    }
    n = fp1.find("This",40);
    tn = -1;
    if ( n != tn ) {
       cerr << "find-4 failed: " << tn << " vs " << n << endl;
       exit(1);    
    }
    n = fp1.find("Adolf");
    tn = -1;
    if ( n != tn ) {
       cerr << "find-5 failed: " << tn << " vs " << n << endl;
       exit(1);    
    }

    // =========================  find(FilePath)
    fp2.clear();
    fp2.append("b/a/c");
    n = fp1.find(fp2);
    tn = 8;
    if ( n != tn ) {
       cerr << "find-3 failed: " << tn << " vs " << n << endl;
       exit(1);    
    }

    // =========================  equality operator
    if ( fp1 == fp2 ) {
       cerr << "== failed (a)" << endl;
       exit(1);    
    }
    // =========================  inequality operator
    if ( ! (fp1 != fp2) ) {
       cerr << "!= failed (a)" << endl;
       exit(1);    
    }
    fp2 = fp1;
    if ( fp1 != fp2 ) {
       cerr << "!= failed (b)" << endl;
       exit(1);    
    }
    if ( ! (fp1 == fp2) ) {
       cerr << "== failed (b)" << endl;
       exit(1);    
    }
    
    // =========================  copy constructor
    FilePath fp3 = fp1;
    if ( fp1 != fp3 ) {
       cerr << "copy constructor failed" << endl;
       exit(1);    
    }

    // =========================  makedir
    fp3.clear();
    fp3.append("test_FilePath_dir");
    fp3.append("testfile");
    // should create it
    fp3.makedir();
    // even if it exists
    fp3.makedir();
    //  create subdirs
    fp3.clear();
    fp3.append("test_FilePath_dir");
    fp3.append("a");
    fp3.append("b");
    fp3.append("testfile");
    fp3.makedir();
    

    int junk = system("/bin/rm -rf test_FilePath_dir");


    exit(0);
}
    
