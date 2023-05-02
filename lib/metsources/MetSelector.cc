
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

#include "gigatraj/MetSelector.hh"

using namespace gigatraj;


MetSelector::MetInfo::MetInfo() {
   MetDesc item;
   
   // we should need to fill this only once
   if ( mdesc.size() == 0 ) {
       
       item.name = "MetSBRot";
       item.desc = "Tilted solid-body earth rotation, using analytical functions";
       item.caps = METCAP_AFCN;
       mdesc.push_back(item);
       
       item.name = "MetGridSBRot";
       item.desc = "Tilted solid-body earth rotation, using gridded arrays";
       item.caps = METCAP_GRID | METCAP_LATLON;
       mdesc.push_back(item);
       
#ifdef USE_MERRA
       item.name = "MetMERRA";
       item.desc = "NASA GMAO MERRA (1) data";
       item.caps = METCAP_GRID | METCAP_LATLON | METCAP_REMOTE;
       mdesc.push_back(item);
#endif
#ifdef USE_MERRA2
       item.name = "MetMERRA2";
       item.desc = "NASA GMAO MERRA-2 data";
       item.caps = METCAP_GRID | METCAP_LATLON | METCAP_REMOTE;
       mdesc.push_back(item);
#endif
#ifdef USE_GEOSFP
       item.name = "MetGEOSfp";
       item.desc = "NASA GMAO GEOS fp data";
       item.caps = METCAP_GRID | METCAP_LATLON | METCAP_REMOTE | METCAP_FCST;
       mdesc.push_back(item);

       item.name = "MetGEOSfpAssim";
       item.desc = "NASA GMAO GEOS fp data, assimilation only";
       item.caps = METCAP_GRID | METCAP_LATLON | METCAP_REMOTE;
       mdesc.push_back(item);

       item.name = "MetGEOSfpFcast";
       item.desc = "NASA GMAO GEOS fp data, forecast only";
       item.caps = METCAP_GRID | METCAP_LATLON | METCAP_REMOTE | METCAP_FCST;
       mdesc.push_back(item);
#endif
#ifdef USE_MYGEOS
       item.name = "MetMyGEOS";
       item.desc = "NASA GMAO GEOS output data";
       item.caps = METCAP_GRID | METCAP_LATLON | METCAP_REMOTE;
       mdesc.push_back(item);
#endif
   }

}


const MetSelector::MetInfo MetSelector::metinfo;

MetSelector::MetSelector()
{
 
}

MetSelector::~MetSelector()
{
}



std::vector<std::string> MetSelector::list( int flags )
{
    std::vector<std::string> result;
    
    for ( int i=0; i<metinfo.mdesc.size(); i++ ) {
        if ( (flags == -1 ) || ( metinfo.mdesc[i].caps & flags ) ) {
           result.push_back(metinfo.mdesc[i].name);
        }
    }
    
    return result;
}


int MetSelector::characterization(const  std::string &source )
{
    for ( int i=0; i<metinfo.mdesc.size(); i++ ) {
        if ( source == metinfo.mdesc[i].name ) {
           return metinfo.mdesc[i].caps;
        }
    }
    
    throw(badUnknownSource());
    
}

std::string MetSelector::description(const  std::string &source )
{
    for ( int i=0; i<metinfo.mdesc.size(); i++ ) {
        if ( source == metinfo.mdesc[i].name ) {
           return metinfo.mdesc[i].desc;
        }
    }
    
    throw(badUnknownSource());
    
}


MetData* MetSelector::source(const  std::string &source )
{
    MetSBRot *metsbrot;
    MetGridSBRot *metgridsbrot;
    MetData* result;
    
    if ( source == "MetSBRot" ) {
        metsbrot = new MetSBRot;
        metsbrot->set_cal(1); // use the Gregorian calendar
        result = metsbrot;
    } else if ( source == "MetGridSBRot" ) {
        metgridsbrot = new MetGridSBRot;
        metgridsbrot->set_cal(1); // use the Gregorian calendar
        result = metgridsbrot;
#ifdef USE_MERRA
     } else if ( source == "MetMERRA" ) {
        result = new MetMERRA;
#endif
#ifdef USE_MERRA2
     } else if ( source == "MetMERRA2" ) {
        result = new MetMERRA2;
#endif
#ifdef USE_GEOSFP
     } else if ( source == "MetGEOSfp" ) {
        result = new MetGEOSfp;

     } else if ( source == "MetGEOSfpAssim" ) {
        result = new MetGEOSfpAssim;

     } else if ( source == "MetGEOSfpFcast" ) {
        result = new MetGEOSfpFcast;
#endif
#ifdef USE_MYGEOS
     } else if ( source == "MetMyGEOS" ) {
        result = new MetMyGEOS;
#endif
    } else {
       std::cerr << "MetSelector::source(): Unknown data source requested: " << source << std::endl;
       throw(badUnknownSource());
    }
    
    return result;
}

