
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

