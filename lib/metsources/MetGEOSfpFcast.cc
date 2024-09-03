
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

#include "gigatraj/MetGEOSfpFcast.hh"

using namespace gigatraj;

const std::string MetGEOSfpFcast::iam = "MetGEOSfpFcast";





// default constructor
MetGEOSfpFcast::MetGEOSfpFcast() : MetMyGEOS() 
{    
     metTag("GEOSfpFcast"); 
}

MetGEOSfpFcast::MetGEOSfpFcast( std::string& date ) : MetMyGEOS() 
{
    
    metTag("GEOSfpFcast"); 
    basetime = cal.day1900( date );

}


// destructor
MetGEOSfpFcast::~MetGEOSfpFcast() 
{
    
}



// copy constructor
MetGEOSfpFcast::MetGEOSfpFcast( const MetGEOSfpFcast&  src) : MetMyGEOS(src)
{
}    

void MetGEOSfpFcast::assign(const MetGEOSfpFcast& src)
{
     MetMyGEOS::assign(src);
}    

MetData *MetGEOSfpFcast::genericCopy()
{
    MetGEOSfpFcast *result;
    
    result = new MetGEOSfpFcast;
    result->assign(*this);
    
    return dynamic_cast<MetData*>( result );
}

/// assignment operator
MetGEOSfpFcast& MetGEOSfpFcast::operator=(const MetGEOSfpFcast& src)
{
    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }
    
    assign(src);

    return *this;
}

MetGridData* MetGEOSfpFcast::MetGridCopy()
{
     MetGEOSfpFcast* newthing;
     
     newthing = new MetGEOSfpFcast();
     newthing->assign( *this );
     
     return dynamic_cast<MetGridData*> (newthing);
}



