
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

#include "gigatraj/MetGEOSfpAssim.hh"

using namespace gigatraj;

const std::string MetGEOSfpAssim::iam = "MetGEOSfpAssim";





// default constructor
MetGEOSfpAssim::MetGEOSfpAssim() : MetMyGEOS() 
{    
     metTag("GEOSfpAssim"); 
}

MetGEOSfpAssim::MetGEOSfpAssim( std::string& date ) : MetMyGEOS() 
{
    
    metTag("GEOSfpAssim"); 
    basetime = cal.day1900( date );

}


// destructor
MetGEOSfpAssim::~MetGEOSfpAssim() 
{
    
}



// copy constructor
MetGEOSfpAssim::MetGEOSfpAssim( const MetGEOSfpAssim&  src) : MetMyGEOS(src)
{
}    

void MetGEOSfpAssim::assign(const MetGEOSfpAssim& src)
{
     MetMyGEOS::assign(src);
}    

MetData *MetGEOSfpAssim::genericCopy()
{
    MetGEOSfpAssim *result;
    
    result = new MetGEOSfpAssim;
    result->assign(*this);
    
    return dynamic_cast<MetData*>( result );
}

/// assignment operator
MetGEOSfpAssim& MetGEOSfpAssim::operator=(const MetGEOSfpAssim& src)
{
    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }
    
    assign(src);

    return *this;
}

MetGridData* MetGEOSfpAssim::MetGridCopy()
{
     MetGEOSfpAssim* newthing;
     
     newthing = new MetGEOSfpAssim();
     newthing->assign( *this );
     
     return dynamic_cast<MetGridData*> (newthing);
}



