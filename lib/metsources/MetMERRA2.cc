
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "math.h"

#include "gigatraj/MetMERRA2.hh"

using namespace gigatraj;

const std::string MetMERRA2::iam = "MetMERRA2";





// default constructor
MetMERRA2::MetMERRA2() : MetMyGEOS() 
{    
     metTag("MERRA2"); 
}

MetMERRA2::MetMERRA2( std::string& date ) : MetMyGEOS() 
{
    
    metTag("MERRA2"); 
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



