
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

#include "gigatraj/MetGEOSfp.hh"

using namespace gigatraj;

const std::string MetGEOSfp::iam = "MetGEOSfp";





// default constructor
MetGEOSfp::MetGEOSfp() : MetMyGEOS() 
{    
     metTag("GEOSfp"); 
}

MetGEOSfp::MetGEOSfp( std::string& date ) : MetMyGEOS() 
{
    
    metTag("GEOSfp"); 
    basetime = cal.day1900( date );

}


// destructor
MetGEOSfp::~MetGEOSfp() 
{
    
}



// copy constructor
MetGEOSfp::MetGEOSfp( const MetGEOSfp&  src) : MetMyGEOS(src)
{
}    

void MetGEOSfp::assign(const MetGEOSfp& src)
{
     MetMyGEOS::assign(src);
}    

MetData *MetGEOSfp::genericCopy()
{
    MetGEOSfp *result;
    
    result = new MetGEOSfp;
    result->assign(*this);
    
    return dynamic_cast<MetData*>( result );
}

/// assignment operator
MetGEOSfp& MetGEOSfp::operator=(const MetGEOSfp& src)
{
    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }
    
    assign(src);

    return *this;
}

MetGridData* MetGEOSfp::MetGridCopy()
{
     MetGEOSfp* newthing;
     
     newthing = new MetGEOSfp();
     newthing->assign( *this );
     
     return dynamic_cast<MetGridData*> (newthing);
}



