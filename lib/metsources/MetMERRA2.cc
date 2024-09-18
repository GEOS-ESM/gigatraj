
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

#include "config.h"

#include "math.h"

#include "gigatraj/MetMERRA2.hh"

using namespace gigatraj;

const std::string MetMERRA2::iam = "MetMERRA2";





// default constructor
MetMERRA2::MetMERRA2() : MetMyGEOS() 
{    
     metTag("MetMERRA2"); 
}

MetMERRA2::MetMERRA2( std::string& date ) : MetMyGEOS() 
{
    
    metTag("MetMERRA2"); 
    basetime = cal.day1900( date );

}


// destructor
MetMERRA2::~MetMERRA2() 
{
    
}



// copy constructor
MetMERRA2::MetMERRA2( const MetMERRA2&  src) : MetMyGEOS(src)
{
}    

void MetMERRA2::assign(const MetMERRA2& src)
{
     MetMyGEOS::assign(src);
}    

MetData *MetMERRA2::genericCopy()
{
    MetMERRA2 *result;
    
    result = new MetMERRA2;
    result->assign(*this);
    
    return dynamic_cast<MetData*>( result );
}

/// assignment operator
MetMERRA2& MetMERRA2::operator=(const MetMERRA2& src)
{
    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }
    
    assign(src);

    return *this;
}

MetGridData* MetMERRA2::MetGridCopy()
{
     MetMERRA2* newthing;
     
     newthing = new MetMERRA2();
     newthing->assign( *this );
     
     return dynamic_cast<MetGridData*> (newthing);
}



