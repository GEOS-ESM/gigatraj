
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/MetData.hh"

using namespace gigatraj;

// constructor
MetData::MetData()
{ 
     my_pgroup = NULLPTR;
     my_metproc = -1;
     now = time(NULL);
     wfctr = 1.0;
     flags = 0;
     debug = 0;
};

// copy constructor
MetData::MetData( const MetData& src )
{

   my_pgroup  = src.my_pgroup;
   my_metproc = src.my_metproc;
   now = time(NULL);
   wfctr = src.wfctr;
   flags = 0;
   debug = src.debug;

}


void MetData::assign( const MetData& src )
{

   my_pgroup  = src.my_pgroup;
   my_metproc = src.my_metproc;
   now = src.now;
   wfctr = src.wfctr;
   flags = src.flags;
   debug = src.debug;

}

void MetData::get_uvw( double time, int n, real* lons, real* lats, real* zs
      , real *u, real *v, real *w)
{
    for (int i=0; i<n; i++ ) {
        u[i] = 0.0;
        v[i] = 0.0;
        w[i] = 0.0;
    }
}

void MetData::getData( string quantity, double time, int n, real* lons, real* lats, real* zs, real* values, int flags )
{
    
    for ( int i=0; i< n; i++ ) {      
        values[i] = getData( quantity, time, lons[i], lats[i], zs[i], flags);
    }
}

ProcessGrp* MetData::getPgroup( int* met) const
{
    if ( met != NULLPTR ) {
       *met = my_metproc;
    }
    
    return my_pgroup;   

}

void MetData::setPgroup( ProcessGrp *pg, int met )
{

   my_pgroup = pg;
   my_metproc = met;

}

void MetData::sync( int mode )
{
    
    if ( my_pgroup != NULLPTR ) {
       
       my_pgroup->sync();
       
    }
}

void MetData::delay()
{
    // do nothing
}

int MetData::receive_svr_status()
{
   int result;
   
   result = PGR_STATUS_OK;
   
   if ( isMetClient() ) {
       my_pgroup->send_ints( my_metproc, 1, &result, PGR_TAG_STATUS );
   }

   return result;
}
  
void  MetData::send_svr_status( int status, int client )
{
   if ( ! isMetClient() ) {
       my_pgroup->send_ints( client, 1, &status, PGR_TAG_STATUS );
   }

} 


void MetData::signalMetDone()
{
   int client_req = PGR_CMD_DONE;

   if ( isMetClient() ) {
      // we are not a dedicated met processor.
      // send a "Done" message to the dedicated met processor
      if ( debug >= 5 ) {
         std::cerr << "MetData::signalMetDone: " << my_pgroup->id() << "/" << my_pgroup->group_id()
                   << " signaling DONE to met proc " << my_metproc << std::endl;
      }
      my_pgroup->send_ints( my_metproc, 1, &client_req, PGR_TAG_REQ );
   }
   
   // everybody sync at this point
   if ( my_pgroup != NULLPTR ) {
      //my_pgroup->sync("MetData::signalMetDone");
      my_pgroup->sync();
   }

}


int MetData::useMet()
{
    
    if ( my_pgroup != NULLPTR ) {
       //my_pgroup->sync("MetData::useMet begins");
       my_pgroup->sync();
    }   

    if ( isMetServer() ) {
        // This is a dedicated met processor
        
        // listen for requests from other processors for our data
        serveMet();
    
        // and return 0, so that the caller can know that
        // data access statements are NOT to be executed
        return 0;
    } else {
        // This is NOT a dedicated met processor;
        // return 1 so that the caller can know if 
        // data access statements should be executed
        return 1;
    }   

}

void MetData::setIsoVertical()
{
     flags = flags | MET_ISO;
}

void MetData::clearIsoVertical() 
{
     flags = flags & ( ~ MET_ISO );
}

std::string MetData::u_wind( std::string *units ) const
{
    if ( units != NULL ) {
       *units = "";
    }
    return "";   
}

std::string MetData::v_wind( std::string *units ) const
{
    if ( units != NULL ) {
       *units = "";
    }
    return "";   
}

std::string MetData::w_wind( std::string *units ) const
{
    if ( units != NULL ) {
       *units = "";
    }
    return "";   
}

std::string MetData::vertical( std::string *units ) const
{
    if ( units != NULL ) {
       *units = "";
    }
    return "";   
}

std::string MetData::vunits() const
{
    return "";   
}
