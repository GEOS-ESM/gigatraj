
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

#include "gigatraj/MetMERRA.hh"

using namespace gigatraj;

const std::string MetMERRA::iam = "MetMERRA";





// default constructor
MetMERRA::MetMERRA() : MetMyGEOS() 
{    
     metTag("MERRA"); 
}

MetMERRA::MetMERRA( std::string& date ) : MetMyGEOS() 
{
    
    metTag("MERRA"); 
    basetime = cal.day1900( date );

}


// destructor
MetMERRA::~MetMERRA() 
{
    
}



// copy constructor
MetMERRA::MetMERRA( const MetMERRA&  src) : MetMyGEOS(src)
{
}    

void MetMERRA::assign(const MetMERRA& src)
{
     MetMyGEOS::assign(src);
}    

MetData *MetMERRA::genericCopy()
{
    MetMERRA *result;
    
    result = new MetMERRA;
    result->assign(*this);
    
    return dynamic_cast<MetData*>( result );
}

/// assignment operator
MetMERRA& MetMERRA::operator=(const MetMERRA& src)
{
    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }
    
    assign(src);

    return *this;
}

MetGridData* MetMERRA::MetGridCopy()
{
     MetMERRA* newthing;
     
     newthing = new MetMERRA();
     newthing->assign( *this );
     
     return dynamic_cast<MetGridData*> (newthing);
}



