
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"
#include <iomanip>
#include "math.h"
#include "mpi.h"
#include "gigatraj/MetGEOSDistributedData.hh"

using namespace gigatraj;

const gigatraj::CalGregorian MetGEOSDistributedData::cal;

// default constructor
MetGEOSDistributedData::MetGEOSDistributedData() 
{

}

// destructor
MetGEOSDistributedData::~MetGEOSDistributedData() 
{
   //delete x3D;
   //delete xSfc;
}

// copy constructor
MetGEOSDistributedData::MetGEOSDistributedData( const MetGEOSDistributedData&  src) 
{

}    


MetGEOSDistributedData & MetGEOSDistributedData::operator=(const MetGEOSDistributedData& src)
{
}

std::string MetGEOSDistributedData::time2Cal( double time, int format )
{
    return cal.date1900(time+basetime, format );
}

void MetGEOSDistributedData::defineCal( std::string caldate, double time )
{

     // convert cal to a time and set basetime to the offset between that and time
     basetime = ( cal.day1900( caldate ) - time );

}

double MetGEOSDistributedData ::cal2Time( std::string date )
{
    return (cal.day1900( date ) - basetime);
}

