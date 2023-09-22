
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

#include "gigatraj/MetGEOSPortal.hh"

using namespace gigatraj;


const gigatraj::CalGregorian MetGEOSPortal::cal;

//*********************  PUBLIC METHODS *********************

// constructor
MetGEOSPortal::HGridSpec::HGridSpec()
{
   clear();
}

// destructor
MetGEOSPortal::HGridSpec::~HGridSpec()
{
}

// clears the spec
void MetGEOSPortal::HGridSpec::clear()
{
    code = ' ';
    id = 0;
    startLon = 0.0;
    endLon = 0.0;
    deltaLon = 1.0;
     nLons = 0;
    startLat = 0.0;
    endLat = 0.0;
    deltaLat = 1.0;
     nLats = 0;
    thin = 1; 
}

// constructor
MetGEOSPortal::VGridSpec::VGridSpec()
{
   clear();
}

// destructor
MetGEOSPortal::VGridSpec::~VGridSpec()
{
}

// clears the spec
void MetGEOSPortal::VGridSpec::clear()
{
   code = ' ';
   id = -1;
   quant = "";
   units = "";
   levs.clear();
   nLevs = 0;

}

// constructor
MetGEOSPortal::TGridSpec::TGridSpec()
{
   clear();
}

// destructor
MetGEOSPortal::TGridSpec::~TGridSpec()
{
}

// clears the spec
void MetGEOSPortal::TGridSpec::clear()
{
   tave = -1;
   tbase = 0;
   tspace = -1;
   start = 0.0;
   end = 0.0;
   delta = 1.0;
   n = 0;
}






// default constructor
MetGEOSPortal::MetGEOSPortal() : MetGridLatLonData() 
{
    // note: the sub-class needs to set the names
    // for gettheta, getpress, and getthetadot
    
    reset();   

}

MetGEOSPortal::MetGEOSPortal( std::string& date ) : MetGridLatLonData() 
{
    // note: the sub-class needs to set the names
    // for gettheta, getpress, and getthetadot
    
    reset();
   
    basetime = cal.day1900( date );

}



// destructor
MetGEOSPortal::~MetGEOSPortal() 
{
    
   if ( isOpen ) { 
      Portal_close();
   }
   
    
}


// copy constructor
MetGEOSPortal::MetGEOSPortal( const MetGEOSPortal&  src) : MetGridLatLonData(src)
{
     desired_hgrid_id = src.desired_hgrid_id;
     desired_vgrid_id = src.desired_vgrid_id;
     desired_tspace = src.desired_tspace;
     desired_tave = src.desired_tave;
     desired_tbase = src.desired_tbase;
     strict = src.strict;
     skip = src.skip;
     skoff = src.skoff;
     ntries = src.ntries;
     waittry = src.waittry;
     openwait = src.openwait;
     basetime = src.basetime;
     
}    

/// assignment operator
MetGEOSPortal& MetGEOSPortal::operator=(const MetGEOSPortal& src)
{
    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }
    
    assign(src);

    return *this;
}

MetGridLatLonData& MetGEOSPortal::operator=(const MetGridLatLonData& src)
{
    const MetGEOSPortal* ssrc;
    
    ssrc = dynamic_cast<const MetGEOSPortal*>(&src);
    // handle assignment to self
    if ( this == ssrc ) {
       return *this;
    }
    
    assign(*ssrc);

    return *this;
}



std::string MetGEOSPortal::time2Cal( double time, int format )
{
    return cal.date1900( basetime + time, format );
}


double MetGEOSPortal::cal2Time( std::string date )
{
    return (cal.day1900( date ) - basetime);

}

void MetGEOSPortal::defineCal( string caldate, double time )
{
     double oldbase;
     
     oldbase = basetime;
     
     // convert cal to a time and set basetime to the offset between that and time
     basetime = ( cal.day1900( caldate ) - time );

}

int MetGEOSPortal::thinning( int *offset )
{
    if ( offset != NULL ) {
       *offset = skoff;
    }
    
    return skip;
}

void MetGEOSPortal::set_thinning( int thin, int offset )
{
    if ( thin > 1 ) {
       skip = thin;
    } else {
       skip = 1;
    } 
    if ( offset >= 0 ) {
       skoff = offset;
    } else {
       skoff = 0;
    }
    
    update_hgrid();
}


void MetGEOSPortal::set_BaseTime( std::string& date ) 
{
    basetime = cal.day1900(date);

}        
std::string MetGEOSPortal::BaseTime( ) const
{
    //std::string *result;
    std::string result;
   // result = new std::string;
    
    result = cal.date1900( basetime );
    
    return result;
}           


GridLatLonField3D* MetGEOSPortal::Get3D( const std::string quantity, const std::string time )
{
    return new_directGrid3D( quantity, time );
}         

GridLatLonFieldSfc* MetGEOSPortal::GetSfc( const std::string quantity, const std::string time )
{
    return new_directGridSfc( quantity, time );
}         


double MetGEOSPortal::get_first_time1900( std::string &url )
{
        
        // do we still have a URL open from the last read? 
        if ( cur_url != url && isOpen ) {
           // then close it
           Portal_close();        
        } 
     
        // is the url that we need open?
        if ( isOpen == 0 ) {
           // No? then tke care of that now
           
           //- std::cerr << "about to open " << url << std::endl;
           Portal_open( url );        
       }

       return cur_tgrid.start;

}

double MetGEOSPortal::get_last_time1900( std::string &url )
{
        
        // do we still have a URL open from the last read? 
        if ( cur_url != url && isOpen ) {
           // then close it
           Portal_close();        
        } 
     
        // is the url that we need open?
        if ( isOpen == 0 ) {
           // No? then tke care of that now
           
           //- std::cerr << "about to open " << url << std::endl;
           Portal_open( url );        
           
       }
       
       return cur_tgrid.end;
}


//*********************  PROTECTED METHODS *********************

void MetGEOSPortal::assign(const MetGEOSPortal& src)
{
    MetGridLatLonData::assign(src);

     desired_hgrid_id = src.desired_hgrid_id;
     desired_vgrid_id = src.desired_vgrid_id;
     desired_tspace = src.desired_tspace;
     desired_tave = src.desired_tave;
     desired_tbase = src.desired_tbase;
     strict = src.strict;
     skip = src.skip;
     ntries = src.ntries;
     waittry = src.waittry;
     openwait = src.openwait;
     basetime = src.basetime;

}    

void MetGEOSPortal::reset()
{
   
   strict = StrictAboutNothing;
   skip = 0;
   skoff = 0;
   basetime = 0.0;
   ntries = 5;
   waittry = 40;
   openwait = 1;
   isOpen = 0;
   cur_url = "";
   
   desired_hgrid_id = -1;
   desired_vgrid_id = -1;
   desired_tspace = -1;
   desired_tave = -1;
   desired_tbase = -1;
   
   test_vgrid = -1;
   test_ndims = -1;
   test_hgrid = -1;
   test_vgrid = -1;
   test_tspace = -1;
   test_tave = -1;
   test_tbase = -1;
   
   override_tbase = -1;
   override_tspace = -1;

   // reset the open-file dimensions
   cur_tgrid.clear();
   cur_hgrid.clear();
   cur_vgrid.clear();
   
   time_zero = 0.0;
      
}

std::vector<real>* MetGEOSPortal::vcoords( const std::string *coordSys ) const
{
    std::vector<real> *result;
    const std::string *vc;
    
    result = new std::vector<real>;

    vc = &vquant;
    if ( coordSys != NULLPTR ) {
       vc = coordSys;
    }
    
    if ( *vc == pressure_name || *vc == "air_pressure" ) {

       result->push_back( 1000.  );     
       result->push_back(  975.  );     
       result->push_back(  950.  );     
       result->push_back(  925.  );     
       result->push_back(  900.  );     
       result->push_back(  875.  );     
       result->push_back(  850.  );     
       result->push_back(  825.  );     
       result->push_back(  800.  );     
       result->push_back(  775.  );     
       result->push_back(  750.  );     
       result->push_back(  725.  );     
       result->push_back(  700.  );     
       result->push_back(  650.  );     
       result->push_back(  600.  );     
       result->push_back(  550.  );     
       result->push_back(  500.  );     
       result->push_back(  450.  );     
       result->push_back(  400.  );     
       result->push_back(  350.  );     
       result->push_back(  300.  );     
       result->push_back(  250.  );     
       result->push_back(  200.  );     
       result->push_back(  150.  );     
       result->push_back(  100.  );     
       result->push_back(   70.  );     
       result->push_back(   50.  );     
       result->push_back(   40.  );     
       result->push_back(   30.  );     
       result->push_back(   20.  );     
       result->push_back(   10.  );     
       result->push_back(    7.  );     
       result->push_back(    5.  );     
       result->push_back(    4.  );     
       result->push_back(    3.  );     
       result->push_back(    2.  );     
       result->push_back(    1.  );     
       result->push_back(    0.7 );     
       result->push_back(    0.5 );     
       result->push_back(    0.4 );     
       result->push_back(    0.3 );     
       result->push_back(    0.1 );     
    
    } else if ( *vc == pottemp_name || *vc == "air_potential_temperature" ) {

       result->push_back(  287.0 );     
       result->push_back(  288.0 );     
       result->push_back(  289.0 );     
       result->push_back(  290.0 );     
       result->push_back(  291.0 );     
       result->push_back(  293.0 );     
       result->push_back(  294.0 );     
       result->push_back(  296.0 );     
       result->push_back(  297.0 );     
       result->push_back(  298.0 );     
       result->push_back(  299.0 );     
       result->push_back(  300.0 );     
       result->push_back(  302.0 );     
       result->push_back(  304.0 );     
       result->push_back(  307.0 );     
       result->push_back(  310.0 );     
       result->push_back(  313.0 );     
       result->push_back(  316.0 );     
       result->push_back(  319.0 );     
       result->push_back(  322.0 );     
       result->push_back(  328.0 );     
       result->push_back(  341.0 );     
       result->push_back(  349.0 );     
       result->push_back(  360.0 );     
       result->push_back(  373.0 );     
       result->push_back(  393.0 );     
       result->push_back(  414.0 );     
       result->push_back(  437.0 );     
       result->push_back(  459.0 );     
       result->push_back(  485.0 );     
       result->push_back(  501.0 );     
       result->push_back(  519.0 );     
       result->push_back(  540.0 );     
       result->push_back(  567.0 );     
       result->push_back(  601.0 );     
       result->push_back(  644.0 );     
       result->push_back(  691.0 );     
       result->push_back(  745.0 );     
       result->push_back(  805.0 );     
       result->push_back(  854.0 );     
       result->push_back(  909.0 );     
       result->push_back(  963.0 );     
       result->push_back( 1022.0 );     
       result->push_back( 1063.0 );     
       result->push_back( 1108.0 );     
       result->push_back( 1167.0 );     
       result->push_back( 1231.0 );     
       result->push_back( 1327.0 );     
       result->push_back( 1426.0 );     
       result->push_back( 1510.0 );     
       result->push_back( 1600.0 );     
       result->push_back( 1708.0 );     
       result->push_back( 1784.0 );     
       result->push_back( 1877.0 );     
       result->push_back( 1969.0 );     
       result->push_back( 2048.0 );     
       result->push_back( 2130.0 );     
       result->push_back( 2178.0 );     
       result->push_back( 2232.0 );     
       result->push_back( 2293.0 );     
       result->push_back( 2356.0 );     
       result->push_back( 2441.0 );     
       result->push_back( 2500.0 );     
       result->push_back( 2600.0 );     
       result->push_back( 2764.0 );     
       result->push_back( 2967.0 );     
       result->push_back( 3100.0 );     
    
    } else if ( *vc == palt_name || *vc == "pressure_altitude" ) {

       result->push_back( getpalt.calc( 1000.  *100.0) );     
       result->push_back( getpalt.calc(  975.  *100.0) );     
       result->push_back( getpalt.calc(  950.  *100.0) );     
       result->push_back( getpalt.calc(  925.  *100.0) );     
       result->push_back( getpalt.calc(  900.  *100.0) );     
       result->push_back( getpalt.calc(  875.  *100.0) );     
       result->push_back( getpalt.calc(  850.  *100.0) );     
       result->push_back( getpalt.calc(  825.  *100.0) );     
       result->push_back( getpalt.calc(  800.  *100.0) );     
       result->push_back( getpalt.calc(  775.  *100.0) );     
       result->push_back( getpalt.calc(  750.  *100.0) );     
       result->push_back( getpalt.calc(  725.  *100.0) );     
       result->push_back( getpalt.calc(  700.  *100.0) );     
       result->push_back( getpalt.calc(  650.  *100.0) );     
       result->push_back( getpalt.calc(  600.  *100.0) );     
       result->push_back( getpalt.calc(  550.  *100.0) );     
       result->push_back( getpalt.calc(  500.  *100.0) );     
       result->push_back( getpalt.calc(  450.  *100.0) );     
       result->push_back( getpalt.calc(  400.  *100.0) );     
       result->push_back( getpalt.calc(  350.  *100.0) );     
       result->push_back( getpalt.calc(  300.  *100.0) );     
       result->push_back( getpalt.calc(  250.  *100.0) );     
       result->push_back( getpalt.calc(  200.  *100.0) );     
       result->push_back( getpalt.calc(  150.  *100.0) );     
       result->push_back( getpalt.calc(  100.  *100.0) );     
       result->push_back( getpalt.calc(   70.  *100.0) );     
       result->push_back( getpalt.calc(   50.  *100.0) );     
       result->push_back( getpalt.calc(   40.  *100.0) );     
       result->push_back( getpalt.calc(   30.  *100.0) );     
       result->push_back( getpalt.calc(   20.  *100.0) );     
       result->push_back( getpalt.calc(   10.  *100.0) );     
       result->push_back( getpalt.calc(    7.  *100.0) );     
       result->push_back( getpalt.calc(    5.  *100.0) );     
       result->push_back( getpalt.calc(    4.  *100.0) );     
       result->push_back( getpalt.calc(    3.  *100.0) );     
       result->push_back( getpalt.calc(    2.  *100.0) );     
//       result->push_back( getpalt.calc(    1.  *100.0) );     
//       result->push_back( getpalt.calc(    0.7 *100.0) );     
//       result->push_back( getpalt.calc(    0.5 *100.0) );     
//       result->push_back( getpalt.calc(    0.4 *100.0) );     
//       result->push_back( getpalt.calc(    0.3 *100.0) );     
//       result->push_back( getpalt.calc(    0.1 *100.0) );     
    

    } else if ( *vc == altitude_name || *vc == "altitude" ) {

       result->push_back(  0.109767 );  
       result->push_back(  0.320891 );  
       result->push_back(  0.537503 );  
       result->push_back(  0.759888 );  
       result->push_back(  0.988369 );  
       result->push_back(  1.21819  ); 
       result->push_back(  1.45441  ); 
       result->push_back(  1.69767  ); 
       result->push_back(  1.94842  ); 
       result->push_back(  2.20230  ); 
       result->push_back(  2.46325  ); 
       result->push_back(  2.73306  ); 
       result->push_back(  3.01204  ); 
       result->push_back(  3.58772  ); 
       result->push_back(  4.20438  ); 
       result->push_back(  4.86374  ); 
       result->push_back(  5.57129  ); 
       result->push_back(  6.34065  ); 
       result->push_back(  7.18340  ); 
       result->push_back(  8.11584  ); 
       result->push_back(  9.16200  ); 
       result->push_back( 10.3595   );
       result->push_back( 11.7840   );
       result->push_back( 13.6084   );
       result->push_back( 16.1797   );
       result->push_back( 18.4416   );
       result->push_back( 20.5767   );
       result->push_back( 21.9997   );
       result->push_back( 23.8489   );
       result->push_back( 26.4818   );
       result->push_back( 31.0547   );
       result->push_back( 33.4541   );
       result->push_back( 35.7776   );
       result->push_back( 37.3548   );
       result->push_back( 39.4309   );
       result->push_back( 42.4412   );
       result->push_back( 47.8201   );
       result->push_back( 50.6458   );
       result->push_back( 53.2829   );
       result->push_back( 54.9943   );
       result->push_back( 57.1530   );
       result->push_back( 64.9467   );
    
    } else {
   
       throw(badVerticalCoord());
    
    }

    return result;

}


void MetGEOSPortal::query_hgrid( int qhgrid, std::vector<real>& lons, std::vector<real>& lats ) 
{
   int nlons, nlats;
   real delta;
   real begval;
   
   if ( qhgrid < 0 ) {
      qhgrid = desired_hgrid_id;
   }
   
   nlons = hspecs[qhgrid].nLons;
   begval =  hspecs[qhgrid].startLon;
   delta = hspecs[qhgrid].deltaLon;
   lons.clear();
   lons.reserve(nlons);
   for (int i=0; i<nlons; i++ ) {
       lons.push_back( i*delta +  begval );
   }

   nlats = hspecs[qhgrid].nLats;
   begval =  hspecs[qhgrid].startLat;
   delta = hspecs[qhgrid].deltaLat;
   lats.clear();
   lats.reserve(nlats);
   for (int i=0; i<nlats; i++ ) {
       lats.push_back( i*delta +  begval );
   }
   
}
void MetGEOSPortal::set_hgrid( const int hgrid0 ) 
{
   int nlons, nlats;
   real delta;
   real begval;
   
   if ( hgrid0 >= 0 && hgrid0 <= 2 ) {
      desired_hgrid_id = hgrid0;
   
      query_hgrid( desired_hgrid_id, lons, lats ); 
      nlons = lons.size();
      nlats = lats.size();
   
   }      
   
}


void MetGEOSPortal::query_vgrid( int qvgrid, std::vector<real>& zs, std::string& vquant, std::string& vunits ) 
{
   int nzs;
   
   if ( qvgrid < 0 ) {
      qvgrid = desired_vgrid_id;
   }   
   
   
   for ( int i=0; i < vspecs.size(); i++ ) {
      if ( vspecs[i].id == qvgrid ) {
          vquant = vspecs[i].quant;
          vunits = vspecs[i].units;
          zs = vspecs[i].levs;
          return;
      }      
   }
   
   throw(badVerticalCoord());
}


void MetGEOSPortal::set_vgrid( const int vgrid0 ) 
{
   int nzs;
   
   if ( vgrid0 >= 0 && vgrid0 <= 2 ) {
      
      desired_vgrid_id = vgrid0;
      query_vgrid( desired_vgrid_id, native_zs, native_vquant, native_vuu );
      native_nzs = native_zs.size();
      
   } else {  
      throw(badVerticalCoord());
   }
}

void MetGEOSPortal::setStrictness( const bool horiz, const bool vert, const bool tspace, const bool tavg )
{
    if ( horiz ) { 
       strict = strict | StrictAboutHgrid ;
    } else {
       strict = strict & ( ~ StrictAboutHgrid) ;  
    }
    if ( vert ) { 
       strict = strict | StrictAboutVgrid ;
    } else {
       strict = strict & ( ~ StrictAboutVgrid) ;  
    }
    if ( tspace ) { 
       strict = strict | StrictAboutTgrid ;
    } else {
       strict = strict & ( ~ StrictAboutTgrid) ;  
    }
    if ( tavg ) { 
       strict = strict | StrictAboutTavg ;
    } else {
       strict = strict & ( ~ StrictAboutTavg) ;  
    }
    
}

bool MetGEOSPortal::horizStrictness()     
{
    return (( strict & StrictAboutHgrid) != 0 );
}

bool MetGEOSPortal::vertStrictness()      
{
    return (( strict & StrictAboutVgrid) != 0 );
}

bool MetGEOSPortal::tspaceStrictness()
{
    return (( strict & StrictAboutTgrid) != 0 );
}

bool MetGEOSPortal::tavgStrictness()     
{
    return (( strict & StrictAboutTavg) != 0 );
}

void  MetGEOSPortal::setNTries( const int n)
{
    if ( n > 0 ) {
       ntries = n;
    }
}

int  MetGEOSPortal::nTries()
{
    return ntries;
}

void  MetGEOSPortal::setWaitTries( const unsigned int interval)
{
     waittry = interval;
}
 
unsigned int  MetGEOSPortal::waitTries()
{
     return waittry;
}


void  MetGEOSPortal::setWaitOpen( const unsigned int interval)
{
     openwait = interval;
}
 
unsigned int  MetGEOSPortal::waitOpen()
{
     return openwait;
}



bool MetGEOSPortal::try_again( const int error, int &trial ) const
{
    bool result;
    
    if ( error == NC_NOERR ) {
       // operation succeeded.
       // We should not try again
       result = false;
    } else {
       // go to the next trial
      trial++;
    
      // But should we?
      if ( trial < ntries ) {
         // yes. But wait first.
         // The waiting period grows as the number of trials.
//         if ( dbug > 1 ) {
            std::cerr << " --- netcdf failure (" << error << "), trial " << trial 
                      << " of " << ntries << "; waiting " <<  waittry*trial << " seconds"
                      << std::endl;
//         }
         (void) sleep( waittry*trial );
         result = true;
      } else {
         // No more trials.
//       if ( dbug > 1 ) {
          std::cerr << " --- netcdf failure (" << error << "), trial " << trial 
                    << " of " << ntries << "; giving up now."
                    << std::endl;
//       }
         result = false;
      }
    }

    return result;
}

int MetGEOSPortal::prep(  const std::string quantity, const double time )
{
    // the generic verison is to just call setup()
    return setup( quantity, time );
}  
  
int MetGEOSPortal::prep(  const std::string quantity, const std::string &time )
{
    // the generic version is to just call setup()
    return setup( quantity, time );
}   

void MetGEOSPortal::Portal_open(const std::string& url )
{
     int err;
     char attr_name[NC_MAX_NAME+1];
     char attr_cval[NC_MAX_NAME+1];
     int attr_type;
     size_t  attr_size;
     std::string name;
     std::string s_value;
     float f_value;
     std::string attr_val_str;
     std::vector<float>  attr_val_f;
     std::vector<char>   attr_val_c;
     std::vector<int>    attr_val_i;
     std::vector<double> attr_val_d;
     int trial;
     int waittime;
     
     // now open the new
     if ( dbug > 2 ) {
        std::cerr << "MetGEOSPortal::Portal_open: attempting nc_open of: <<" << url  << ">>" << std::endl;
     }

     // but first, sleep some time between opens, to avoid being obnoxious to the server
     if ( openwait > 0 ) {
        if ( dbug > 2 ) {
           std::cerr << "************* About to wait " << openwait << " seconds on proc ";
           if ( my_pgroup != NULLPTR ) {
              std::cerr << my_pgroup->id() << " group " << my_pgroup->group_id();
           }
           std::cerr << std::endl;
        }
        (void) sleep( openwait ); // sleep this time between opens, to avoid being obnoxious to the server
     }

     trial = 0;
     do {
        //- std::cerr << " nc_opening url " << url << std::endl;
        err = nc_open( url.c_str(), NC_NOWRITE, &ncid);     
        //- std::cerr << " nc_opened url " << url << std::endl;
     } while ( try_again( err, trial ) );
     if ( err != NC_NOERR ) {
        std::cerr << "MetGEOSPortal::Portal_open: Bad url: " << url << std::endl;
        throw(badNetcdfOpen(err));
     }
     isOpen = 1;
     cur_url = url;                                
     if ( dbug >= 2 ) {
        std::cerr << "MetGEOSPortal::Portal_open: ----- opened " << url << std::endl;         
     }  
     if ( dbug > 2 ) {
        std::cerr << "MetGEOSPortal::Portal_open: nc_open success!" << std::endl;
     }

     // do the initial inquiries to get basic sizes and shapes
     trial = 0;
     do {
        err = nc_inq(ncid, &ndimens, &nvars, &ngatts, &unlimdim_idx);
     } while( try_again( err, trial ) );
     if ( err != NC_NOERR ) {
        throw(badNetcdfError(err));
     }
     
     if ( dbug > 2 ) {
        std::cerr << "MetGEOSPortal::Portal_open: initial inq: " << ndimens << ", " << nvars << ", " << ngatts << ", " << unlimdim_idx << std::endl;
     }
         
     // go through the global attributes, one by one
     for (int i=0; i<ngatts; i++) {
         trial = 0;
         do {
            err = nc_inq_attname(ncid, NC_GLOBAL, i, attr_name );
         } while ( try_again( err, trial ) );   
         if ( err != NC_NOERR ) {
            throw(badNetcdfError(err));
         }
         
         name.assign(attr_name);
         
         trial = 0;
         do {
            err = nc_inq_att(ncid, NC_GLOBAL, attr_name, &attr_type, &attr_size );
         } while ( try_again( err, trial ) );   
         if ( err != NC_NOERR ) {
            throw(badNetcdfError(err));
         }
         
         if ( dbug > 4 ) {
            std::cerr << "MetGEOSPortal::Portal_open: reading global attr " << i << ": " << name 
            << "(" << attr_type << " x " << attr_size << ") ";
         }
         
         switch (attr_type) {
         case NC_CHAR: 
             // character-based metadata consists of human-readable information
             // that is useful to know
             //- std::cerr << " [NC_CHAR] ";
             
             // (but omit "ArchivedMetadata.0", since that does not work)
             if ( name != "ArchivedMetadata.0" ) {
             
                Portal_read_attr( attr_val_str, attr_name, NC_GLOBAL, attr_size );
                if ( dbug > 4  ) {
                   std::cerr << "= " << gattr_strings[name];
                }
                gattr_strings[name] = attr_val_str;
                
             }
             break;
         case NC_FLOAT:
             //- std::cerr << " [NC_FLOAT] ";
             // a float attribute may be an array of floats.
             Portal_read_attr( attr_val_f, attr_name, NC_GLOBAL, attr_size );
             if ( dbug > 4  ) {
                std::cerr << "= " << gattr_reals[name];
             }
             // use only the first value; toss the rest
             gattr_reals[name] = attr_val_f[0];
             attr_val_f.clear();
             
             break;
         case NC_DOUBLE:
             //- std::cerr << " [NC_DOUBLE] ";
             // a double attribute may be an array of doubles.
             Portal_read_attr( attr_val_d, attr_name, NC_GLOBAL, attr_size );
             if ( dbug > 4  ) {
                std::cerr << "= " << gattr_reals[name];
             }
             gattr_reals[name] = attr_val_d[0];
             break;
         default:
             // ignore 
             break;    
         }
          
         if ( dbug > 4  ) {
            std::cerr << std::endl;
         }

     }                                                         

     Portal_read_all_dims();

} 

void MetGEOSPortal::Portal_close()
{
    int err;
    int trial;
    
    if ( isOpen ) {
       trial = 0;
       do {
          //- std::cerr << "nc_closing url" << std::endl;
          err = nc_close(ncid);
          //- std::cerr << "nc_closed url" << std::endl;
       } while ( try_again( err, trial ) );   
       if ( err != NC_NOERR ) {
          throw(badNetcdfError(err));
       } 
       isOpen = 0; 
       if ( dbug > 2 ) {
          std::cerr << "MetGEOSPortal::Portal_close: nc_close success!" << std::endl;        
       }
    }
    
    // reset the open-file dimensions 
    cur_tgrid.clear();
    cur_hgrid.clear();
    cur_vgrid.clear();

} 


void MetGEOSPortal::Portal_checkdims(const int nvdims, const int*dimids )
{
     int err;
     // the dimension id
     int dim_id;
     // name of the dimension
     char dim_name[NC_MAX_NAME+1];
     // number of elements in the dimension
     size_t dim_size;
     // the name of the dimension
     std::string name;
     // legal dimension name?
     bool illegal;
     // trial number
     int trial;
     
     // get dimensional information
     for ( int i=0; i<nvdims; i++ ) {
         dim_id = dimids[i];
         trial = 0;
         do {
            err = nc_inq_dim(ncid, dim_id, dim_name, &dim_size );
         } while ( try_again( err, trial ) );   
         if ( err != NC_NOERR ) {
            throw(badNetcdfError(err));
         }
            
         name.assign(dim_name);
                     
         if ( dbug > 5 ) {
            std::cerr << "MetGEOSPortal::Portal_checkdims:     dim " << i << " is " << name << " of size " << dim_size << std::endl;
         }
                       
         // check for unknown names
         
         illegal = true;
         for ( int i=0; i<legalDims.size(); i++ ) {
             if ( name == legalDims[i] ) {
                illegal = false;
                break;
             }
         }
         if ( illegal ) {      
               std::cerr << "MetGEOSPortal::Portal_checkdims: Unknown dimension named " << name << std::endl;
               throw(badUnknownDim());
         }
            
     }

}


int MetGEOSPortal::Portal_findtime( const double desired_time ) const
{
    double diff;
    int result;
    int idx;
    int nt;
    double itime;
    double xtime;
    
    
    result = -1;
    
    //  time values are evenly-spaced, so we can calculate
    //  which one is closest, and if it is close enough
    
    nt = cur_tgrid.n;
    
    if ( dbug > 4 ) {
       std::cerr << "MetGEOSPortal::Portal_findtime: Looking for time " << desired_time << " among " << nt << " times " << std::endl;
    }
    
    if ( nt > 0 ) {
    
       if ( dbug > 4 ) {
          std::cerr << "MetGEOSPortal::Portal_findtime:  time range is " << cur_tgrid.start << " to " << cur_tgrid.end << std::endl;
          std::cerr << "MetGEOSPortal::Portal_findtime:  base time is " << basetime << std::endl;
       }    

       if ( nt > 1 ) {
          itime = (desired_time - (cur_tgrid.start - basetime))/cur_tgrid.delta + 0.5;
          idx =  itime ;
       } else {
          idx = 0;
       }

       xtime = cur_tgrid.start + idx*cur_tgrid.delta - basetime;
       if ( dbug > 4 ) {
          std::cerr << "MetGEOSPortal::Portal_findtime:  determined index " << idx << " for " << xtime << std::endl;
       }    
       
       if ( idx >= 0 && idx < nt ) {
          // should be within 2 minutes
          if ( ABS(xtime - desired_time) < (2.0/60.0/24.0) ) {
             result = idx;
          } else {
            if ( dbug > 5 ) {
               double xt2 = desired_time;
               double xt1 = xtime;
               std::string st1 =  cal.date1900( basetime + xt1 );
               std::string st2 =  cal.date1900( basetime + xt2 );
               std::cerr << "MetGEOSPortal::Portal_findtime:  outside time tolerance  " 
                         << xtime  << "(" << st1 << ")"
                         << " vs " 
                         << desired_time   << "(" << st2  << ")"
                         << " diff = " << ABS(xtime - desired_time) 
                         << std::endl;
            }
          }   
       }    
    }
    
    if ( dbug > 4 ) {
       std::cerr << "MetGEOSPortal::Portal_findtime:  Returning result " << result << std::endl;
    }    
    return result;
    
}


void MetGEOSPortal::Portal_dim_attrs(const std::string &dname, const int var_id, const int nattrs )
{
    int err;
    // name of the attribute
    char attrname[NC_MAX_NAME+1];
    // attribute type
    nc_type attr_type;
    // number of elements in the attribute
    size_t attr_size;
    // pointers to attribute values of different types
    std::vector<unsigned char> attr_val_b;
    std::vector<short> attr_val_s;
    std::vector<int> attr_val_i;
    std::vector<float> attr_val_f;
    std::vector<double> attr_val_d;
    std::string attr_val_str;
    SpanAttr item;
    std::string aname;
    std::string outval;
    int trial;
    
    if ( dbug > 4 ) {
       std::cerr << "MetGEOSPortal::Portal_get_attrs: " << nattrs << " Attributes:" << std::endl;
    }
    
    
    // for each attribute of this variable...
    for ( int ia=0; ia<nattrs; ia++ ) {
    
        if ( dbug > 5 ) {
           std::cerr << "MetGEOSPortal::Portal_get_attrs:      Attribute " << ia << " of " << nattrs << ":" << std::endl;
        } 
        
        trial = 0;
        do {  
           err = nc_inq_attname( ncid, var_id, ia, attrname);
        } while ( try_again( err, trial ) );   
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        
        // get information about the attribute
        trial = 0;
        do {  
           err = nc_inq_att(ncid, var_id, attrname, &attr_type, &attr_size );
        } while ( try_again( err, trial ) );   
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        
        aname.assign(attrname);
        if ( dbug > 5 ) {
           std::cerr << "MetGEOSPortal::Portal_get_attrs:          attribute name is  " << aname <<  std::endl;
        }   
        
        // clear the output value
        outval.clear();
        
        // how we read in the attribute value depends on the value type
        switch (attr_type) {
        case NC_CHAR:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_get_attrs:          Reading " << attr_size << " characters for attribute " << aname << std::endl;
             }   

             Portal_read_attr( outval, attrname, var_id, attr_size );

             if ( dname == legalDims[3] && attrname == "units" ) {
             
             }
             
           break;
        case NC_BYTE:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_get_attrs:          Reading " << attr_size << " bytes for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_b, attrname, var_id, attr_size );

             attr_val_b.clear();

           break;
        case NC_SHORT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_get_attrs:          Reading " << attr_size << " shorts for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_s, attrname, var_id, attr_size );
             
             attr_val_s.clear();
                
           break;
        case NC_INT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_get_attrs:          Reading " << attr_size << " ints for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_i, attrname, var_id, attr_size );

             attr_val_i.clear();    
           break;
        case NC_FLOAT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_get_attrs:          Reading " << attr_size << " floats for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_f, attrname, var_id, attr_size );

             
             attr_val_f.clear();    

           break;
        case NC_DOUBLE:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_get_attrs:          Reading " << attr_size << " doubles for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_d, attrname, var_id, attr_size );


             attr_val_d.clear();    
           break;
        default:
           if (dbug > 5 ) {
              std::cerr << "MetGEOSPortal::Portal_get_attrs:   unknown attr data type " << attr_type << " for " << attrname << std::endl;
           }
           throw(badNetcdfError(NC_EBADTYPE));   
        } 

        
    
    }
    

}


// 3D
void MetGEOSPortal::Portal_getattrs(const int var_id, const int nattrs, GridLatLonField3D *datagrid )
{
    int err;
    real badval_def = 1.0e15;
    real badval_0 = 0.0;
    real badval_1 = 0.0;
    real badval_2 = 0.0;
    // name of the attribute
    char attrname[NC_MAX_NAME+1];
    // attribute type
    nc_type attr_type;
    // number of elements in the attribute
    size_t attr_size;
    // pointers to attribute values of different types
    std::vector<unsigned char> attr_val_b;
    std::vector<short> attr_val_s;
    std::vector<int> attr_val_i;
    std::vector<float> attr_val_f;
    std::vector<double> attr_val_d;
    std::string attr_val_str;
    // holds a string-form attribute value
    std::string outval;
    std::string aname;
    std::string aval;
    std::ostringstream sconv;
    int trial;
    
    if ( dbug > 4 ) {
       std::cerr << "MetGEOSPortal::Portal_getattrs: (3D) Attributes:" << std::endl;
    }
    
    // for each attribute of this variable...
    for ( int ia=0; ia<nattrs; ia++ ) {
    
        if ( dbug > 5 ) {
           std::cerr << "MetGEOSPortal::Portal_getattrs: (3D)      Attribute " << ia << " of " << nattrs << ":" << std::endl;
        } 
        
        trial = 0;
        do {  
           err = nc_inq_attname( ncid, var_id, ia, attrname);
        } while ( try_again( err, trial ) );   
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        
        // get information about the attribute
        trial = 0;
        do {  
           err = nc_inq_att(ncid, var_id, attrname, &attr_type, &attr_size );
        } while ( try_again( err, trial ) );   
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        
        aname.assign(attrname);
        if ( dbug > 5 ) {
           std::cerr << "MetGEOSPortal::Portal_getattrs:  (3D)        attribute name is  " << aname <<  std::endl;
        }   
        
        // clear the output value
        outval.clear();
        
        sconv.str("");
        
        // how we read in the attribute value depends on the value type
        switch (attr_type) {
        case NC_CHAR:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs: (3D)         Reading " << attr_size << " characters for attribute " << aname << std::endl;
             }   

             Portal_read_attr( outval, attrname, var_id, attr_size );

             if ( aname == "units" ) {
                datagrid->set_units( outval );
             } 
             
             sconv << outval; 
             
           break;
        case NC_BYTE:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs:  (3D)        Reading " << attr_size << " bytes for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_b, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_b[i];
             }
             
             attr_val_b.clear();

           break;
        case NC_SHORT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs: (3D)         Reading " << attr_size << " shorts for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_s, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_s[i];
             } 
             
             attr_val_s.clear();
                
           break;
        case NC_INT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs:  (3D)        Reading " << attr_size << " ints for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_i, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_i[i];
             } 

             attr_val_i.clear();    
           break;
        case NC_FLOAT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs: (3D)         Reading " << attr_size << " floats for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_f, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_f[i];
             }    

             if ( aname == "missing_value" ) {
                badval_0 = attr_val_f[0];
             } 
             if ( aname == "missing_fvalue"  ) {
                badval_1 = attr_val_f[0];
             }
             if ( aname == "_FillValue"  ) {
                badval_2 = attr_val_f[0];
             }
             
             attr_val_f.clear();    

           break;
        case NC_DOUBLE:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs:  (3D)        Reading " << attr_size << " doubles for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_d, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_d[i];
             }    
             
             attr_val_d.clear();    
           break;
        default:
           if (dbug > 5 ) {
              std::cerr << "MetGEOSPortal::Portal_getattrs: (3D)  unknown attr data type " << attr_type << " for " << attrname << std::endl;
           }
           throw(badNetcdfError(NC_EBADTYPE));   
        } 
        
        
        // store the attribute
        if ( dbug > 5 ) {
           std::cerr << "MetGEOSPortal::Portal_getattrs:  (3D)        Setting attribute " << aname << " to:<<" << sconv.str() << ">>" << std::endl;
        }   
        datagrid->set_attribute( aname, sconv.str() ); 
        
    
    }
    
    // set the bad-or-missing-data flag
    if ( badval_0 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetGEOSPortal::Portal_getattrs: (3D)      Setting badval to 0: " << badval_0 << std::endl;
       }   
       datagrid->set_fillval( badval_0 );
    } else if ( badval_1 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetGEOSPortal::Portal_getattrs: (3D)      Setting badval to 1: " << badval_1 << std::endl;
       }   
       datagrid->set_fillval( badval_1 );
    } else if ( badval_2 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << " MetGEOSPortal::Portal_getattrs:  (3D)       Setting badval to 2: " << badval_2 << std::endl;
       }   
       datagrid->set_fillval( badval_1 );
    } else {
       if ( dbug > 5 ) {
          std::cerr << "MetGEOSPortal::Portal_getattrs:  (3D)     Setting badval to def: " << badval_def << std::endl;
       }   
       datagrid->set_fillval( badval_def );
    }         

}

/// Sfc
void MetGEOSPortal::Portal_getattrs(const int var_id, const int nattrs, GridLatLonFieldSfc *datagrid )
{
    int err;
    real badval_def = 1.0e15;
    real badval_0 = 0.0;
    real badval_1 = 0.0;
    real badval_2 = 0.0;
    // name of the attribute
    char attrname[NC_MAX_NAME+1];
    // attribute type
    nc_type attr_type;
    // number of elements in the attribute
    size_t attr_size;
    // pointers to attribute values of different types
    std::vector<unsigned char>attr_val_b;
    std::vector<short>attr_val_s;
    std::vector<int>attr_val_i;
    std::vector<float>attr_val_f;
    std::vector<double>attr_val_d;
    std::string attr_val_str;
    // holds a string-form attribute value
    std::string outval;
    std::string aname;
    std::ostringstream sconv;
    int trial;
    
    if ( dbug > 4) {
       std::cerr << "MetGEOSPortal::Portal_getattrs: (Sfc)  Attributes:" << std::endl;
    }
    
    // for each attribute of this variable...
    for ( int ia=0; ia<nattrs; ia++ ) {
    
        if ( dbug > 5 ) {
           std::cerr << "MetGEOSPortal::Portal_getattrs: (Sfc)     Attribute " << ia << " of " << nattrs << ":" << std::endl;
        } 
        
        trial = 0;
        do {
           err = nc_inq_attname( ncid, var_id, ia, attrname);
        } while ( try_again( err, trial ) );   
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        
        // get information about the attribute
        trial = 0;
        do {
           err = nc_inq_att(ncid, var_id, attrname, &attr_type, &attr_size );
        } while ( try_again( err, trial ) );   
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        
        aname.assign(attrname);
        if ( dbug) {
           std::cerr << "MetGEOSPortal::Portal_getattrs:  (Sfc)       attribute name is  " << aname <<  std::endl;
        }   
        
        // clear the output value
        outval.clear();
        
        sconv.str("");
        
        // how we read in the attribute value depends on the value type
        switch (attr_type) {
        case NC_CHAR:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs: (Sfc)        Reading " << attr_size << " characters for attribute " << aname << std::endl;
             }   

             Portal_read_attr( outval, attrname, var_id, attr_size );

             if ( aname == "units" ) {
                datagrid->set_units( outval );
             } 
             
             sconv << outval; 
             
           break;
        case NC_BYTE:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs: (Sfc)        Reading " << attr_size << " bytes for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_b, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_b[i];
             }
             
             attr_val_b.clear();

           break;
        case NC_SHORT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs:  (Sfc)       Reading " << attr_size << " shorts for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_s, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_s[i];
             } 
             
             attr_val_s.clear();
           break;
        case NC_INT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs: (Sfc)        Reading " << attr_size << " ints for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_i, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_i[i];
             } 

             attr_val_i.clear();    
           break;
        case NC_FLOAT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs:  (Sfc)       Reading " << attr_size << " floats for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_f, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_f[i];
             }    

             if ( aname == "missing_value" ) {
                badval_0 = attr_val_f[0];
             } 
             if ( aname == "missing_fvalue"  ) {
                badval_1 = attr_val_f[0];
             }
             if ( aname == "_FillValue"  ) {
                badval_2 = attr_val_f[0];
             }

             attr_val_f.clear();    

           break;
        case NC_DOUBLE:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getattrs:  (Sfc)       Reading " << attr_size << " doubles for attribute " << aname << std::endl;
             }   

             Portal_read_attr( attr_val_d, attrname, var_id, attr_size );

             for (int i=0; i<attr_size; i++ ) {
                 if ( i > 0 ) {
                    sconv << " ";
                 }   
                 sconv << attr_val_d[i];
             }    
             
             attr_val_d.clear();    
           break;
        default:
           if (dbug > 0 ) {
              std::cerr << "MetGEOSPortal::Portal_getattrs:  (Sfc)unknown attr data type " << attr_type << " for " << attrname << std::endl;
           }
           throw(badNetcdfError(NC_EBADTYPE));   
        } 
        
        
        // store the attribute
        if ( dbug > 5 ) {
           std::cerr << "MetGEOSPortal::Portal_getattrs: (Sfc)        Setting attribute " << aname << " to:<<" << sconv.str() << ">>" << std::endl;
        }   
        datagrid->set_attribute( aname, sconv.str() ); 
        
    
    }
    
    // set the bad-or-missing-data flag
    if ( badval_0 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetGEOSPortal::Portal_getattrs: (Sfc)     Setting badval to 0: " << badval_0 << std::endl;
       }   
       datagrid->set_fillval( badval_0 );
    } else if ( badval_1 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetGEOSPortal::Portal_getattrs:  (Sfc)    Setting badval to 1: " << badval_1 << std::endl;
       }   
       datagrid->set_fillval( badval_1 );
    } else if ( badval_2 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetGEOSPortal::Portal_getattrs:  (Sfc)       Setting badval to 2: " << badval_2 << std::endl;
       }   
       datagrid->set_fillval( badval_1 );
    } else {
       if ( dbug > 5 ) {
          std::cerr << "MetGEOSPortal::Portal_getattrs:  (Sfc)    Setting badval to def: " << badval_def << std::endl;
       }   
       datagrid->set_fillval( badval_def );
    }         

}



void MetGEOSPortal::Portal_read_attr( std::string &result, char *attr_name, int varid, size_t len )
{
    char *val;
    std::string s_value;
    int err;
    int trial;
    
    // Some attributes are string arrays, the total lengths of which
    // exceeds NC_MAX_NAME in length. So we need to
    // allocate space for the string, read it in,
    // copy it into our collection, and then free up the memory
    // that we allocated.
    // Note the +1 on the size, to allow for a terminating NULL
    try{
       val = new char[len + 1];
    } catch(...) {
       throw (badNoMem());
    }  
    trial = 0;
    do { 
       err = nc_get_att_text(ncid, varid, attr_name, val );
    } while ( try_again( err, trial ) );   
    if ( err != NC_NOERR ) {
       throw(badNetcdfError(err));
    }
    // ensure that the C-string is terminated
    val[len] = 0;

    s_value.assign(val, len);

    delete[] val;
   
}


void MetGEOSPortal::Portal_read_attr( std::vector<unsigned char> &result, char *attr_name, int varid, size_t len )
{
    unsigned char *val;
    int err;
    int trial;
    
    // Some attributes are string arrays, the total lengths of which
    // exceeds NC_MAX_NAME in length. So we need to
    // allocate space for the string, read it in,
    // copy it into our collection, and then free up the memory
    // that we allocated.
    // Note the +1 on the size, to allow for a terminating NULL
    try {
       val = new unsigned char[len + 1];
    } catch(...) {
       throw (badNoMem());
    }
    trial= 0;
    do {
       err = nc_get_att_uchar(ncid, varid, attr_name, val );
    } while ( try_again( err, trial ) );   
    if ( err != NC_NOERR ) {
       throw(badNetcdfError(err));
    }

    result.clear();
    result.reserve(len);
    
    for (int i=0; i<len; i++ ) {
        result.push_back( val[i] );
    }    
    
    delete val;
   
}

void MetGEOSPortal::Portal_read_attr( std::vector<short> &result, char *attr_name, int varid, size_t len )
{
    short *val;
    int err;
    int trial;
    
    try {
       val = new short[len];
    } catch(...) {
       throw (badNoMem());
    }   
    
    trial = 0;
    do {
       err = nc_get_att_short(ncid, varid, attr_name, val );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       throw(badNetcdfError(err));
    }
    
    result.clear();
    result.reserve(len);
    
    for (int i=0; i<len; i++ ) {
        result.push_back( val[i] );
    }    
    
    delete val;

}


void MetGEOSPortal::Portal_read_attr( std::vector<int> &result, char *attr_name, int varid, size_t len )
{
    int *val;
    int err;
    int trial;
    
    try {
       val = new int[len];
    } catch(...) {
       throw (badNoMem());
    }   
    
    trial = 0;
    do {
       err = nc_get_att_int(ncid, varid, attr_name, val );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       throw(badNetcdfError(err));
    }
    
    result.clear();
    result.reserve(len);
    
    for (int i=0; i<len; i++ ) {
        result.push_back( val[i] );
    }    
    
    delete val;

}


void MetGEOSPortal::Portal_read_attr( std::vector<float> &result, char *attr_name, int varid, size_t len )
{
    float *val;
    int err;
    int trial;
    
    try {
       val = new float[len];
    } catch(...) {
       throw (badNoMem());
    }   
    
    trial = 0;
    do {
       err = nc_get_att_float(ncid, varid, attr_name, val );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       throw(badNetcdfError(err));
    }
    
    result.clear();
    result.reserve(len);
    
    for (int i=0; i<len; i++ ) {
        result.push_back( val[i] );
    }    
    
    delete[] val;

}

void MetGEOSPortal::Portal_read_attr( std::vector<double> &result, char *attr_name, int varid, size_t len )
{
    double *val;
    int err;
    int trial;
    
    try {
       val = new double[len];
    } catch(...) {
       throw (badNoMem());
    }   
    
    trial = 0;
    do {
       err = nc_get_att_double(ncid, varid, attr_name, val );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       throw(badNetcdfError(err));
    }
    
    result.clear();
    result.reserve(len);
    
    for (int i=0; i<len; i++ ) {
        result.push_back( val[i] );
    }    
    
    delete[] val;

}



void MetGEOSPortal::Portal_read_dim( std::string &dim_name, nc_type &dim_type, SpanTriplet &span) 
{
     // name of the dimension
     const char *c_dim_name;
     // the variable ID of the dimension
     int dvar_id;
     // the dim id of the dimension
     int ddim_id;
     // tweaked version of the dimension's name
     char dimvar_name[NC_MAX_NAME+1];
     // characterisitics of the dimension variable
     int dndim;
     int ddims[NC_MAX_VAR_DIMS];
     int dnatt;
     size_t dim_size;
     int err;
     float f_first, f_last, f_delta;
     double d_first, d_last, d_delta;
     int i_first, i_last, i_delta;
     int trial;


     c_dim_name = dim_name.c_str();
     
     if ( dbug >= 3 ) {
        std::cerr << "MetGEOSPortal::Portal_read_dim: attempting to read dimension " << dim_name << std::endl;
     } 
     
     // which dimension is this dimension?
     trial = 0;
     do {
        err = nc_inq_dimid( ncid, c_dim_name, &ddim_id );
        if ( err == NC_EBADDIM ) {
           // good URL, but bad dimension
           // which implies this is the levels dimension 
           // don't bother re-trying anything
           throw(badMissingDim());           
        }
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        throw(badNetcdfError(err));
     }
     trial = 0;
     do {
        err = nc_inq_dim(ncid, ddim_id, dimvar_name, &dim_size );
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        throw(badNetcdfError(err));
     }
     

     // which variable is this dimension?
     trial = 0;
     do {
        err = nc_inq_varid(ncid, c_dim_name, &dvar_id );
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        std::cerr << "MetGEOSPortal::Portal_read_dim: no netdcf varianble id for dimension " << dim_name << std::endl;
        throw(badNetcdfError(err));
     }
     if ( dbug >= 5 ) {
        std::cerr << "MetGEOSPortal::Portal_read_dim: var id =  " << dvar_id << std::endl;
     } 

     // get the characteristics
     trial = 0;
     do {
        err = nc_inq_var(ncid, dvar_id, dimvar_name, &dim_type, &dndim, ddims, &dnatt );
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        std::cerr << "MetGEOSPortal::Portal_read_dim: cannot read characteristics for ncdf var id " << dvar_id << " or dimension " << dim_name << std::endl;
        throw(badNetcdfError(err));
     }
     if ( dndim != 1 ) {
        std::cerr << "MetGEOSPortal::Portal_read_dim: Bad number-of-dimensions for " << dim_name << " : " << dndim << std::endl;        
     }
     if ( dbug >= 5 ) {
        std::cerr << "MetGEOSPortal::Portal_read_dim: dndim =  " << dndim << std::endl;
     }
     
     switch (dim_type) {
     case NC_FLOAT:
        if ( dbug > 5 ) {
            std::cerr << "MetGEOSPortal::Portal_read_dim:    trying to read 2 of " << dim_size << " NC_FLOATs for dim " << dim_name << std::endl;
        }
        Portal_read_dim_floats( f_first, f_last, f_delta, dvar_id, dim_size );
        span.floatSpec.first = f_first;
        span.floatSpec.last = f_last;
        span.floatSpec.delta = f_delta;
        span.floatSpec.size = dim_size;
        break;
     case NC_DOUBLE:
        if ( dbug > 5 ) {
            std::cerr << "MetGEOSPortal::Portal_read_dim:    trying to read 2 of " << dim_size << " NC_DOUBLEs for dim " << dim_name << std::endl;
        }
        Portal_read_dim_doubles( d_first, d_last, d_delta, dvar_id, dim_size );
        span.doubleSpec.first = d_first;
        span.doubleSpec.last = d_last;
        span.doubleSpec.delta = d_delta;
        span.doubleSpec.size = dim_size;
        break;
     case NC_INT:
        if ( dbug > 5 ) {
            std::cerr << "MetGEOSPortal::Portal_read_dim:    trying to read 2 of " << dim_size << " NC_INTs for dim " << dim_name << std::endl;
        }
        Portal_read_dim_ints( i_first, i_last, i_delta, dvar_id, dim_size );
        span.intSpec.first = i_first;
        span.intSpec.last = i_last;
        span.intSpec.delta = i_delta;
        span.intSpec.size = dim_size;
        break;
     default:
        std::cerr << "MetGEOSPortal::Portal_read_dim: Unimplemented dimensional netcdf format: " << dim_type << std::endl;
        throw(badNetcdfError(err));
     }

}


void MetGEOSPortal::Portal_read_dim( std::string &dim_name, std::vector<real> &vals)
{
     // name of the dimension
     const char *c_dim_name;
     // the variable ID of the dimension
     int dvar_id;
     // the dim id of the dimension
     int ddim_id;
     // tweaked version of the dimension's name
     char dimvar_name[NC_MAX_NAME+1];
     // characterisitics of the dimension variable
     int dndim;
     int ddims[NC_MAX_VAR_DIMS];
     int dnatt;
     size_t dim_size;
     int err;
     float f_first, f_last, f_delta;
     double d_first, d_last, d_delta;
     int i_first, i_last, i_delta;
     nc_type dim_type;
     int trial;


     c_dim_name = dim_name.c_str();
     
     // which dimension is this dimension?
     trial = 0;
     do {
        err = nc_inq_dimid( ncid, c_dim_name, &ddim_id );
        if ( err == NC_EBADDIM ) {
           // good URL, but bad dimension
           // which implies this is the levels dimension 
           // don't bother re-trying anything
           throw(badMissingDim());           
        }
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        throw(badNetcdfError(err));
     }
     trial = 0;
     do {
        err = nc_inq_dim(ncid, ddim_id, dimvar_name, &dim_size );
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        throw(badNetcdfError(err));
     }
     
     // which variable is this dimension?
     trial = 0;
     do {
        err = nc_inq_varid(ncid, c_dim_name, &dvar_id );
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        std::cerr << "MetGEOSPortal::Portal_read_dim: no netdcf varianble id for dimension " << dim_name << std::endl;
        throw(badNetcdfError(err));
     }
     // get the characteristics
     trial = 0;
     do {
        err = nc_inq_var(ncid, dvar_id, dimvar_name, &dim_type, &dndim, ddims, &dnatt );
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        std::cerr << "MetGEOSPortal::Portal_read_dim: cannot read characteristics for ncdf var id " << dvar_id << " or dimension " << dim_name << std::endl;
        throw(badNetcdfError(err));
     }
     if ( dndim != 1 ) {
        std::cerr << "MetGEOSPortal::Portal_read_dim: Bad number-of-dimensions for " << dim_name << " : " << dndim << std::endl;
        
     }
     
     switch (dim_type) {
     case NC_FLOAT:
        if ( dbug > 5 ) {
            std::cerr << "MetGEOSPortal::Portal_read_dim:    trying to read " << dim_size << " NC_FLOATs for dim " << dim_name << std::endl;
        }
        Portal_read_dim_floats( vals, dvar_id, dim_size );
        break;
     case NC_DOUBLE:
        if ( dbug > 5 ) {
            std::cerr << "MetGEOSPortal::Portal_read_dim:    trying to read " << dim_size << " NC_DOUBLEs for dim " << dim_name << std::endl;
        }
        Portal_read_dim_doubles( vals, dvar_id, dim_size );
        break;
     case NC_INT:
        if ( dbug > 5 ) {
            std::cerr << "MetGEOSPortal::Portal_read_dim:    trying to read " << dim_size << " NC_INTs for dim " << dim_name << std::endl;
        }
        Portal_read_dim_ints( vals, dvar_id, dim_size );
        break;
     default:
        std::cerr << "MetGEOSPortal::Portal_read_dim: Unimplemented dimensional netcdf format: " << dim_type << std::endl;
        throw(badNetcdfError(err));
     }


}

void MetGEOSPortal::update_hgrid()
{
    int newLen;
    
    // try to make sense of what we have read of the current horizontal grid
    cur_hgrid.id = -1;
    for ( int i=0; i<hspecs.size(); i++ ) {
        if ( (hspecs[i].nLons == cur_hgrid.nLons)
          && ( ABS(hspecs[i].deltaLon - cur_hgrid.deltaLon) < 0.001 )
          && (hspecs[i].nLats == cur_hgrid.nLats)
          && ( ABS(hspecs[i].deltaLat - cur_hgrid.deltaLat) < 0.001 ) ) {
        
             cur_hgrid.id = hspecs[i].id;
             
             break;
        } 
    }
    
    cur_hgrid.thin = 1;
    if ( skip > 0 && skip < (cur_hgrid.nLons/2) ) {
       cur_hgrid.thin = skip;
    }
    cur_hgrid.thin_offset = 0;
    if ( skoff >= 0 && skoff < skip ) {
       cur_hgrid.thin_offset = skoff;
    }
    
    newLen = cur_hgrid.nLons/cur_hgrid.thin;
    if ( newLen*cur_hgrid.thin < cur_hgrid.nLons ) {
       newLen++;
    }
    
    // reset the lons and lats
    lons.clear();
    lons.reserve( newLen );
    for ( int i=0; i<newLen; i++ ) {
        lons.push_back( cur_hgrid.startLon + cur_hgrid.deltaLon*(cur_hgrid.thin_offset + cur_hgrid.thin*i) );
    }
    nlons = lons.size();
        
    newLen = cur_hgrid.nLats/cur_hgrid.thin;
    if ( newLen*cur_hgrid.thin < cur_hgrid.nLats ) {
       newLen++;
    }

    lats.clear();
    lats.reserve( newLen );
    for ( int i=0; i<newLen; i++ ) {
        lats.push_back( cur_hgrid.startLat + cur_hgrid.deltaLat*cur_hgrid.thin*i );
    }
    nlats = lats.size();

}
void MetGEOSPortal::update_vgrid()
{
    int nz;
    
    // try to make sense of what we just read
    nz = cur_vgrid.levs.size();
    if ( nz == 42 ) {
       cur_vgrid.id = 2;
    } else if ( nz == 72 ) {       
       cur_vgrid.id = 1;
    } else if ( nz == 73 ) {
       cur_vgrid.id = 3;       
    }
    
    // native is what we are using
    native_zs = cur_vgrid.levs;
    native_nzs = cur_vgrid.nLevs;

}

void MetGEOSPortal::update_tgrid()
{

    // try to make sense of what we have read of the current time grid
    cur_tgrid.tbase  = round( ( cur_tgrid.start - trunc( cur_tgrid.start ) )*24.0 );
    cur_tgrid.tspace = cur_tgrid.delta;

}

void MetGEOSPortal::Portal_read_all_dims()
{
    double first_t, last_t, delta_t;
    int    len_t;
    double first_lon, last_lon, delta_lon;
    int len_lon; 
    double first_lat, last_lat, delta_lat;
    int len_lat; 
    std::vector<real> z;
    int len_z;
    std::string dname; 
    SpanTriplet span;
    nc_type dim_type;
    std::vector<real> zvals;
    double xbase, xspace;
    int nz;
    
    
    dname = legalDims[3]; // time
    Portal_read_dim( dname, dim_type, span);
    switch (dim_type) {
    case NC_DOUBLE:
       cur_tgrid.start = Portal_time_nativeTo1900( span.doubleSpec.first );
       cur_tgrid.end   = Portal_time_nativeTo1900( span.doubleSpec.last  );
       cur_tgrid.delta = Portal_deltime_nativeTo1900( span.doubleSpec.delta );
       cur_tgrid.n     = span.doubleSpec.size;
       break;
    case NC_FLOAT:
       cur_tgrid.start = Portal_time_nativeTo1900( static_cast<double>(span.floatSpec.first) );
       cur_tgrid.end   = Portal_time_nativeTo1900( static_cast<double>(span.floatSpec.last)  );
       cur_tgrid.delta = Portal_deltime_nativeTo1900( static_cast<double>(span.floatSpec.delta) );
       cur_tgrid.n     = span.doubleSpec.size;
       break;
    case NC_INT:
       cur_tgrid.start = Portal_time_nativeTo1900( static_cast<double>(span.intSpec.first) );
       cur_tgrid.end   = Portal_time_nativeTo1900( static_cast<double>(span.intSpec.last)  );
       cur_tgrid.delta = Portal_deltime_nativeTo1900( static_cast<double>(span.intSpec.delta) );
       cur_tgrid.n     = span.intSpec.size;
       break;
    default:
       std::cerr << "MetGEOSPortal::Portal_read_all_dims: Unimplemented format for dim " << dname << ": " << dim_type << std::endl;
       throw(badDimsForm());
    }
       
    dname = legalDims[0]; // longitude
    Portal_read_dim( dname, dim_type, span);
    switch (dim_type) {
    case NC_DOUBLE:
       cur_hgrid.startLon = span.doubleSpec.first;
       cur_hgrid.endLon   = span.doubleSpec.last;
       //cur_hgrid.deltaLon = span.doubleSpec.delta;
       cur_hgrid.nLons    = span.doubleSpec.size;
       cur_hgrid.deltaLon = (cur_hgrid.endLon - cur_hgrid.startLon)/(cur_hgrid.nLons - 1);
       break;
    default:
       std::cerr << "MetGEOSPortal::Portal_read_all_dims: Unimplemented format for dim " << dname << ": " << dim_type << std::endl;
       throw(badDimsForm());
    }
       
    dname = legalDims[1];  // latitude
    Portal_read_dim( dname, dim_type, span);
    switch (dim_type) {
    case NC_DOUBLE:
       cur_hgrid.startLat = span.doubleSpec.first;
       cur_hgrid.endLat   = span.doubleSpec.last;
       cur_hgrid.deltaLat = span.doubleSpec.delta;
       cur_hgrid.nLats    = span.doubleSpec.size;
       cur_hgrid.deltaLat = (cur_hgrid.endLat - cur_hgrid.startLat)/(cur_hgrid.nLats - 1);
       break;
    default:
       std::cerr << "MetGEOSPortal::Portal_read_all_dims: Unimplemented format for dim " << dname << ": " << dim_type << std::endl;
       throw(badDimsForm());
    }
    
    update_hgrid();
    update_tgrid();
    
    try {
       dname = legalDims[2]; // vertical level
       Portal_read_dim( dname, zvals);
       
       cur_vgrid.levs = zvals;
       cur_vgrid.nLevs = zvals.size();
       

       
    //- std::cerr << " init tim:" << cur_tgrid.n << " vals from " << cur_tgrid.start 
    //<< " to " << cur_tgrid.end << " via " << cur_tgrid.delta 
    //<< " w/ base " << basetime << std::endl;
       
    } catch (badMissingDim err) {
       // no vertical levels in this file.
       zvals.clear();
       cur_vgrid.id = 0;
       cur_vgrid.levs.clear();
       cur_vgrid.nLevs = 0;
    } 
    
    update_vgrid();   

}




void MetGEOSPortal::Portal_read_dim_doubles( std::vector<real>&vals, int varid, size_t len )
{
    double* buffr;
    int err;
    size_t zero = 0;
    size_t count;
    ptrdiff_t stride;
    double delta;
    double val;
    int trial;
    
    buffr = new double[len];
    count = len;
    stride = 1;

    //- std::cerr << "reading " << len << " double dims " << std::endl;    
    trial = 0;
    do {
       err = nc_get_vars_double( ncid, varid, &zero, &count, &stride, buffr );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       std::cerr << "MetGEOSPortal::Portal_read_dim_doubles: Failed trying to read " << len << " NC_DOUBLEs " << std::endl;
       throw(badNetcdfError(err));
    }
    
    if ( len > 1 ) {
       delta = buffr[1] - buffr[0];
    } else {
       delta = 0;
    }
    
    vals.clear();
    vals.reserve(len);
    for ( int j=0; j<len; j++ ) {
        val = buffr[j];
        if ( (dbug > 5) && (j < 5) ) {
           std::cerr << "MetGEOSPortal::Portal_read_dim_doubles: dim[" << j << "] = " << val << std::endl;
        }
        vals.push_back( val );
    }
    
    delete[] buffr;
    
}

void MetGEOSPortal::Portal_read_dim_doubles( double &first, double &last, double &delta, int varid, size_t len )
{
    double buffr[2];
    int err;
    size_t zero = 0;
    size_t count = 2;
    ptrdiff_t stride;
    int trial;
    
    stride = len - 1;

    //- std::cerr << "Reading " << len << " double dims " << std::endl;    
    trial = 0;
    do {
       err = nc_get_vars_double( ncid, varid, &zero, &count, &stride, buffr );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       std::cerr << "MetGEOSPortal::Portal_read_dim_doubles: Failed trying to read " << len << " NC_DOUBLEs " << std::endl;
       throw(badNetcdfError(err));
    }
    
    first = buffr[0];
    last = buffr[1];
    delta = ( last - first )/(len - 1);

}

void MetGEOSPortal::Portal_read_dim_floats( std::vector<real>&vals, int varid, size_t len )
{
    float* buffr;
    int err;
    size_t zero = 0;
    size_t count;
    ptrdiff_t stride;
    float delta;
    float val;
    int trial;
    
    buffr = new float[len];
    count = len;
    stride = 1;
    
    trial = 0;
    do {
       err = nc_get_vars_float( ncid, varid, &zero, &count, &stride, buffr );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       std::cerr << "MetGEOSPortal::Portal_read_dim_floats: Failed trying to read " << len << " NC_FLOATs " << std::endl;
       throw(badNetcdfError(err));
    }
    
    if ( len > 1 ) {
       delta = buffr[1] - buffr[0] ;
    } else {
       delta = 0;
    }

    vals.clear();
    vals.reserve(len);
    for ( int j=0; j<len; j++ ) {
        val = buffr[j];
        if ( (dbug > 5) && (j < 5) ) {
           std::cerr << "MetGEOSPortal::Portal_read_dim_floats: dim[" << j << "] = " << val << std::endl;
        }
        vals.push_back( val );
    }
    
    delete[] buffr;

}

void MetGEOSPortal::Portal_read_dim_floats( float &first, float &last, float &delta, int varid, size_t len )
{
    float buffr[2];
    int err;
    size_t zero = 0;
    size_t count = 2;
    ptrdiff_t stride;
    int trial;
    
    stride = len - 1;

    //- std::cerr << "Reading " << len << " float dims " << std::endl;    
    trial = 0;
    do {
       err = nc_get_vars_float( ncid, varid, &zero, &count, &stride, buffr );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       std::cerr << "MetGEOSPortal::Portal_read_dim_floats: Failed trying to read " << len << " NC_FLOATs " << std::endl;
       throw(badNetcdfError(err));
    }
    
    first = buffr[0];
    last = buffr[1];
    delta = ( last - first )/(len - 1);

}

void MetGEOSPortal::Portal_read_dim_ints( std::vector<real>&vals, int varid, size_t len )
{
    int *buffr;
    int err;
    size_t zero = 0;
    size_t count = 2;
    ptrdiff_t stride;
    int delta;
    int val;
    int trial;
    
    buffr = new int[len];
    count = len;
    stride = 1;
    
    trial = 0;
    do {
       err = nc_get_vars_int( ncid, varid, &zero, &count, &stride, buffr );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       std::cerr << "MetGEOSPortal::Portal_read_dim_ints: Failed trying to read " << len << " NC_INTs " << std::endl;
       throw(badNetcdfError(err));
    }
    
    if ( len > 1 ) {
       delta = buffr[1] - buffr[0];
    } else {
       delta = 0;
    } 
    
    vals.clear();
    vals.reserve(len);
    for ( int j=0; j<len; j++ ) {
        val = buffr[j];
        if ( (dbug > 5) && (j < 5) ) {
           std::cerr << "MetGEOSPortal::Portal_read_dim_ints: dim[" << j << "] = " << val << std::endl;
        }
        vals.push_back( static_cast<real>(val) );
    }
    
    delete[] buffr;
    
}

void MetGEOSPortal::Portal_read_dim_ints( int &first, int &last, int &delta, int varid, size_t len )
{
    int buffr[2];
    int err;
    size_t zero = 0;
    size_t count = 2;
    ptrdiff_t stride;
    int trial;
    
    stride = len - 1;

    // std::cerr << "Reading " << len << " int dims " << std::endl;    
    trial = 0;
    do {
       err = nc_get_vars_int( ncid, varid, &zero, &count, &stride, buffr );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       std::cerr << "MetGEOSPortal::Portal_read_dim_ints: Failed trying to read " << len << " NC_INTs " << std::endl;
       throw(badNetcdfError(err));
    }
    
    first = buffr[0];
    last = buffr[1];
    delta = ( last - first )/(len - 1);

}

void MetGEOSPortal::Portal_read_data_floats( std::vector<real>&vals, int var_id, int ndims, size_t *starts, size_t *counts, ptrdiff_t *strides )
{
     float *buffr;
     float *bp;
     int totsize;
     int nlons, nlats, nz;
     int err;
     int idx;
     int trial;
     bool all_zeroes;
     size_t my_starts[4];
     size_t my_counts[4];
     ptrdiff_t my_strides[4];
     int nchunks;
     ptrdiff_t chunksize;
     int toread;
     
     my_starts[0] = 0;
     my_starts[1] = 0;
     my_starts[2] = 0;
     my_starts[3] = 0;
     my_counts[0] = 0;
     my_counts[1] = 0;
     my_counts[2] = 0;
     my_counts[3] = 0;
     my_strides[0] = 0;
     my_strides[1] = 0;
     my_strides[2] = 0;
     my_strides[3] = 0;
     
     if ( ndims == 3 ) {
        // three-dimensional surface
        nlons = counts[3];
        nlats = counts[2];
        nz = counts[1];
        totsize = nlons*nlats*nz;
        
        for ( int i=0; i<4 ; i++ ) {
            my_counts[i] = counts[i];
            my_starts[i] = starts[i];
            my_strides[i] = strides[i];
        }
        
        /* Now this may be too big to read in all at once.
           In that case, we will split it up and read the data in
           layer by layer.
        */
        if ( totsize < 9000000 ) {
           // larger-scale grid (fewer gridpoints) we can read in all at once
           nchunks = 1;
           chunksize = 0;
           toread = totsize;
        } else {
           // smaller-scale grid (more gridpoints) we should read in chunks
           nchunks = nz;
           chunksize = nlons*nlats;
           my_counts[1] = 1;
           my_starts[1] = 0;
           my_strides[1] = 1;
           toread = chunksize;
        }
        
     } else {
        // two-dimensional surface
        nlons = counts[2];
        nlats = counts[1];
        nz = 1;
        totsize = nlons*nlats*nz;
        
        for ( int i=0; i<3 ; i++ ) {
            my_counts[i] = counts[i];
            my_starts[i] = starts[i];
            my_strides[i] = strides[i];
        }
        nchunks = 1;
        chunksize = 0;
        toread = totsize;
     }   
     

     try {
        buffr = new float[totsize];
     } catch(...) {
        throw (badNoMem());
     }
     if ( dbug > 5 ) {
        std::cerr << "MetGEOSPortal::Portal_read_data: (" << ndims << "D):  about to read data! " <<  std::endl;
     }

     vals.clear();
     vals.reserve(totsize);


     err = NC_NOERR;
     bp = &(buffr[0]);
     int isample = toread/10;
     for ( int ichunk = 0; (ichunk < nchunks) && (err == NC_NOERR) ; ichunk++ ) {

         trial = 0;
         do {

            if ( dbug > 5 ) {
               std::cerr << "MetGEOSPortal::Portal_read_data: (" << ndims 
                         << "D):  about to read data chunk " << ichunk << " of " << nchunks
                         << " = " << toread << " floats (trial " << trial << ")" <<  std::endl;
            }
            err = nc_get_vars_float( ncid, var_id, my_starts, my_counts, my_strides, bp );

            if ( err == NC_NOERR ) {

               all_zeroes = true;
               for ( int i=0; i<toread; i += isample ) {
                   if ( bp[i] != 0 ) {
                      all_zeroes = false;
                   }                     
               }

               if ( all_zeroes ) {
                  // sometimes we get a "successful" read, but the
                  // values are all zeroes.
                  // Detect this and treat it as a failure.
                  if ( dbug > 0 ) {
                     std::cerr << "MetGEOSPortal::Portal_read_data: (" << ndims << "D):  Read all zeroes! " <<  std::endl;
                  }
                  err = NC_ERANGE;
               }
     
            }
     
         } while ( try_again( err, trial ) );       

         if ( err != NC_NOERR ) {
            delete[] buffr;   
            throw(badNetcdfError(err));
         }

         //- std::cerr << " chunk " << ichunk << " [" << bp-buffr << "] (0)=" << bp[0] << ", (" << nlats*nlons-1 << ")=" << bp[nlons*nlats-1] << std::endl;     
         // ok, we have successfully read in this chunk
         // now set up for the next chunk
         if ( nchunks > 0 ) {
            my_starts[1] = my_starts[1] + strides[1];
            bp = bp + chunksize;         
         }
         
     }
     
     // move the data into place
     for (int k=0; k<nz; k++) {
         for (int j=0; j<nlats; j++ ) {
             for (int i=0; i<nlons; i++ ) {
                 idx = i + j*nlons + k*nlons*nlats;
                 vals.push_back( buffr[idx] ); 
             }
         }
     }

     delete[] buffr;   
}


void MetGEOSPortal::set_verticalBase( const std::string quantity, const std::string units, const std::vector<real>* levels, real scale, real offset )
{
    std::string vunitz;

    MetGridData::set_verticalBase( quantity, units, levels, scale, offset );
    
    if ( quantity == pressure_name || quantity == "air_pressure" ) {
       strict = strict | StrictAboutVgrid;
       set_vgrid(2); 
    } else if ( quantity == "Model-Levels" ) {
       strict = strict | StrictAboutVgrid;
       set_vgrid(1); 
    }
}

void MetGEOSPortal::set_verticalBase( GridField3D *grid )
{
    std::string vunitz;

    MetGridData::set_verticalBase( grid );
    
    if ( vquant == pressure_name || vquant == "air_pressure" ) {
       strict = strict | StrictAboutVgrid;
       set_vgrid(2); 
    } else if ( vquant == "Model-Levels" ) {
       strict = strict | StrictAboutVgrid;
       set_vgrid(1); 
    }

}    


/// 3D
void MetGEOSPortal::Portal_getvar(const std::string quantity, const double time, GridLatLonField3D* grid3d )
{
     int err;
     std::vector<real> *xlons, *xlats, *xzs, *xdata;
     int xnlons, xnlats, xnzs;
     char attr_name[NC_MAX_NAME+1];
     char attr_cval[NC_MAX_NAME+1];
     int attr_type;
     size_t  attr_size;
     std::string name;
     std::string s_value;
     float f_value;
     int var_id;
     // variable name returned on query
     char junkname[NC_MAX_NAME];
     // variable type
     nc_type var_type;
     // variable's number of dimensions
     int nvdims;
     // array of IDs for a variable's dimensions
     int dimids[NC_MAX_VAR_DIMS];
     // number of a variable's attributes
     int var_nattrs;
     // starting point in source array from which we will read
     size_t zero = 0;
     std::string vq, vu;
     int tindex;
     size_t vstarts[4];
     size_t vcounts[4];
     ptrdiff_t vstride[4];
     float *dbuffr;
     int idx;
     real vscale, voffset;
     int trial;
     

        // get the variable ID
        trial = 0;
        do {
           err = nc_inq_varid(ncid, quantity.c_str(), &var_id );
        } while ( try_again( err, trial ) );       
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        if ( dbug > 4 ) {
           std::cerr << "MetGEOSPortal::Portal_getvar: (3D)  variable <<" << quantity << ">> is var_id " << var_id << std::endl;
        }
        // get basic info about this variable
        trial = 0;
        do {
           err = nc_inq_var(ncid,var_id, junkname, &var_type, &nvdims, dimids, &var_nattrs);
        } while ( try_again( err, trial ) );       
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        if ( dbug  > 4) {
           std::cerr << "MetGEOSPortal::Portal_getvar: (3D)   variable id " <<  var_id << " has type=" << var_type << ", " << nvdims << " dims, " 
           << var_nattrs << " attrs" << std::endl;
        }

        if ( dbug > 4 ) {
           std::cerr << "MetGEOSPortal::Portal_getvar: (3D)   variable id " <<  var_id << std::endl;
        }
        
        
        // copy global string attributes into the object (in case it gets cached)
        for ( gs_iter=gattr_strings.begin(); gs_iter !=gattr_strings.end(); gs_iter++ ) {
           std::string aname = gs_iter->first;
           std::string aval = gs_iter->second;
           grid3d->set_attribute( aname, aval ); 
        }
        
        // read the variable attributes here, esp. bad-or-missing-data flag
        Portal_getattrs(var_id, var_nattrs, grid3d);
        
        // get the variable's dimensions
        Portal_checkdims( nvdims, dimids );
        
        // find the index of the time snapshot that we want
        tindex = Portal_findtime( time );
        if ( tindex == -1 ) {
           throw(badTimeNotFound());
        }

        nlons = cur_hgrid.nLons;
        nlats = cur_hgrid.nLats;
        nzs = cur_vgrid.nLevs;
        //- std::cerr << "       counts = " << nlons << ", " << nlats << ", " << nzs  << std::endl;

        // set up the grid dimensions for the data object
        xlons = new std::vector<real>;
        xlats = new std::vector<real>;
        query_hgrid( test_hgrid, *xlons, *xlats );
        xnlons = xlons->size();
        xnlats = xlats->size();
        
        xzs = new std::vector<real>;
        query_vgrid( test_vgrid, *xzs, vq, vu );
        xnzs = xzs->size();
        grid3d->set_vertical(vq);
        if ( vu == "mb" || vu == "hPa" ) {
           vscale = 100.0;
           voffset = 1.0;
        } else {
           vscale = 1.0;
           voffset = 0.0;
        }      
        grid3d->set_vunits(vu, vscale, voffset);
        
        if ( nlons != xnlons || nlats != xnlats || nzs != xnzs ) {
           if ( dbug > 0 ) {
              std::cerr << "MetGEOSPortal::Portal_getvar: (3D)         Dims Mismatch:  " << nlons << "/" << xnlons 
              << ", " << nlats << "/" << xnlats 
              << ", " << nzs << "/" << xnzs  << std::endl;
              std::cerr << "MetGEOSPortal::Portal_getvar: (3D)  zs=";
           }   
           throw (badDimsMismatch());
        }

        // set up for skipping gridpoints
        vstarts[0] = tindex;
        vcounts[0] = 1;
        vstride[0] = 1;

        vstarts[1] = 0;
        vcounts[1] = nzs;
        vstride[1] = 1;

        vstarts[2] = 0;
        vcounts[2] = nlats;
        vstride[2] = 1;

        vstarts[3] = 0;
        vcounts[3] = nlons;
        vstride[3] = 1;
        
        if ( skip > 0 ) {
           // thin out xlons and xlats
           vcounts[2] = lats.size();
           vstride[2] = skip;
           
           vcounts[3] = lons.size();
           vstride[3] = skip;
           vstarts[3] = skoff;
           
           xlons->clear();
           for (int i=0; i<vcounts[3]; i++ ) {
               xlons->push_back( cur_hgrid.startLon + cur_hgrid.deltaLon*(vstarts[3] + i*vstride[3]) );
           }    
           xlats->clear();
           for (int i=0; i<vcounts[2]; i++ ) {
               xlats->push_back( cur_hgrid.startLat + cur_hgrid.deltaLat*(vstarts[2] + i*vstride[2]) );
           }    
           
           nlons = xlons->size();
           nlats = xlats->size();
        } 

        // read the data values here
        // how we read in the attribute value depends on the value type
        switch (var_type) {
        case NC_FLOAT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getvar: (3D)         Reading " << nlons*nlats*nzs  << " floats for variable " << quantity << std::endl;
             }
          
             xdata = new std::vector<real>;
             Portal_read_data_floats( *xdata, var_id, 3, vstarts, vcounts, vstride );
           
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getvar: (3D)   finished reading data! " <<  std::endl;
             }

           break;
        default:
           if (dbug > 0) {
              std::cerr << "MetGEOSPortal::Portal_getvar: (3D)  unexpected var data type " << var_type << " for " << quantity << std::endl;
           }
           throw(badNetcdfError(NC_EBADTYPE));   
        } 

        
        grid3d->load( *xlons, *xlats, *xzs, *xdata );
        
        delete xlons;
        delete xlats;
        delete xzs;
        delete xdata;
        

}

// Sfc
void MetGEOSPortal::Portal_getvar(const std::string quantity, const double time, GridLatLonFieldSfc* grid2d )
{
     int err;
     std::vector<real> *xlons, *xlats, *xdata;
     int xnlons, xnlats, xnzs;
     char attr_name[NC_MAX_NAME+1];
     char attr_cval[NC_MAX_NAME+1];
     int attr_type;
     size_t  attr_size;
     std::string name;
     std::string s_value;
     float f_value;
     int var_id;
     // variable name returned on query
     char junkname[NC_MAX_NAME];
     // variable type
     nc_type var_type;
     // variable's number of dimensions
     int nvdims;
     // array of IDs for a variable's dimensions
     int dimids[NC_MAX_VAR_DIMS];
     // number of a variable's attributes
     int var_nattrs;
     // starting point in source array from which we will read
     size_t zero = 0;
     std::string vq, vu;
     int tindex;
     size_t vstarts[3];
     size_t vcounts[3];
     ptrdiff_t vstride[3];
     float *dbuffr;
     int idx;
     int trial;
     

        // get the variable ID
        trial = 0;
        do {
           err = nc_inq_varid(ncid, quantity.c_str(), &var_id );
        } while ( try_again( err, trial ) );       
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        if ( dbug > 4 ) {
           std::cerr << "MetGEOSPortal::Portal_getvar: (Sfc) variable <<" << quantity << ">> is var_id " << var_id << std::endl;
        }
        // get basic info about this variable
        trial = 0;
        do {
           err = nc_inq_var(ncid,var_id, junkname, &var_type, &nvdims, dimids, &var_nattrs);
        } while ( try_again( err, trial ) );       
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        if ( dbug > 4 ) {
           std::cerr << "MetGEOSPortal::Portal_getvar: (Sfc) variable id " <<  var_id << " has type=" << var_type << ", " << nvdims << " dims, " 
           << var_nattrs << " attrs" << std::endl;
        }

        if ( dbug  > 4) {
           std::cerr << "MetGEOSPortal::Portal_getvar: (Sfc) variable id " <<  var_id << std::endl;
        }
        
        
        // copy global string attributes into the object (in case it gets cached)
        for ( gs_iter=gattr_strings.begin(); gs_iter !=gattr_strings.end(); gs_iter++ ) {
           std::string aname = gs_iter->first;
           std::string aval = gs_iter->second;
           grid2d->set_attribute( aname, aval ); 
        }
        
        // read the variable attributes here, esp. bad-or-missing-data flag
        Portal_getattrs(var_id, var_nattrs, grid2d);
        
        // get the variable's dimensions
        Portal_checkdims( nvdims, dimids );
        
        // find the index of the time snapshot that we want
        tindex = Portal_findtime( time );
        if ( tindex == -1 ) {
           throw(badTimeNotFound());
        }

        nlons = cur_hgrid.nLons;
        nlats = cur_hgrid.nLats;
        nzs = cur_vgrid.nLevs;
        //- std::cerr << "       counts = " << nlons << ", " << nlats << ", " << nzs  << std::endl;

        // set up the grid dimensions for the data object
        xlons = new std::vector<real>;
        xlats = new std::vector<real>;
        query_hgrid( test_hgrid, *xlons, *xlats );
        xnlons = xlons->size();
        xnlats = xlats->size();
        
        if ( nlons != xnlons || nlats != xnlats ) {
           if ( dbug > 0 ) {
              std::cerr << "MetGEOSPortal::Portal_getvar: (Sfc)       Dims Mismatch:  " << nlons << "/" << xnlons 
              << ", " << nlats << "/" << xnlats  << std::endl;
           }   
           throw (badDimsMismatch());
        }

        // set up for skipping gridpoints
        vstarts[0] = tindex;
        vcounts[0] = 1;
        vstride[0] = 1;

        vstarts[1] = 0;
        vcounts[1] = nlats;
        vstride[1] = 1;

        vstarts[2] = 0;
        vcounts[2] = nlons;
        vstride[2] = 1;
        
        if ( skip > 0 ) {
           // thin out xlons and xlats
           vcounts[1] = lats.size();
           vstride[1] = skip;

           vcounts[2] = lons.size();
           vstride[2] = skip;
           vstarts[2] = skoff;
           
           xlons->clear();
           for (int i=0; i<vcounts[2]; i++ ) {
               xlons->push_back( cur_hgrid.startLon + cur_hgrid.deltaLon*(vstarts[2] + i*vstride[2]) );
           }    
           xlats->clear();
           for (int i=0; i<vcounts[1]; i++ ) {
               xlats->push_back( cur_hgrid.startLat + cur_hgrid.deltaLat*(vstarts[1] + i*vstride[1]) );
           }    
           
           nlons = xlons->size();
           nlats = xlats->size();
        } 

        // read the data values here
        // how we read in the attribute value depends on the value type
        switch (var_type) {
        case NC_FLOAT:
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getvar: (Sfc)       Reading " << nlons*nlats*nzs  << " floats for variable " << quantity << std::endl;
             }   
             
             xdata = new std::vector<real>;
             Portal_read_data_floats( *xdata, var_id, 2, vstarts, vcounts, vstride );
             
             if ( dbug > 5 ) {
                std::cerr << "MetGEOSPortal::Portal_getvar: (Sfc) finished reading data! " <<  std::endl;
             }

           break;
        default:
           if (dbug > 5) {
              std::cerr << "MetGEOSPortal::Portal_getvar: (Sfc) unexpected var data type " << var_type << " for " << quantity << std::endl;
           }
           throw(badNetcdfError(NC_EBADTYPE));   
        } 

        
        grid2d->load( *xlons, *xlats, *xdata );
    
        delete xlons;
        delete xlats;
        delete xdata;
        

}


// 3D
void MetGEOSPortal::readPortal( const std::string quantity, const std::string time, GridLatLonField3D* grid3d )
{
     int ndims;
     double timex;
     double preTime, postTime;
     GridLatLonField3D* preData;
     GridLatLonField3D* postData;
     double mtime;
     std::string ctime1, ctime2;
     GridLatLonField3D::iterator src1, src2, dest;
     real bad;
         
     mtime = cal2Time(time);
     grid3d->set_time( mtime, time );
     grid3d->set_quantity(quantity);
    
    
     // subtle point here: bracket() needs to know the data spacing,
     // by looking at what time characteristics are
     // available for this quantity, but NOT by opening
     // a remote file. The prep() call below goes a step further.
     // This is so that we can tell here whether we will need one
     // file read or two (and thus potentially two different prep() calls,
     // if the two times belong to two different remote files/urls.
     if ( bracket( quantity, mtime, &preTime, &postTime ) ) { 
        
        // This sets, among other things, test_url, which is used to
        // open the remote file below. Preferably, no new url
        // will be opened at this pont. But for some data sources,
        // prep() might open the new file in the course of looking
        // for a file that contains the requested time (e.g., when 
        // ultiple forecast files are available on the remote end).
        // prep() throws an error if there is no such quantity
        prep( quantity, time );
     

        if ( dbug > 3 ) {
     
           std::cerr << "MetGEOSPortal::readPortal: (3D) quantity=" << quantity << std::endl;
           std::cerr << "MetGEOSPortal::readPortal: (3D) time=" << time << std::endl;
           std::cerr << "MetGEOSPortal::readPortal: (3D) url=" << test_url << std::endl;
           std::cerr << "MetGEOSPortal::readPortal: (3D) grid=" << test_hgrid << "," << test_vgrid << "," << test_tave << std::endl;
        }
           
        timex = cal2Time( time );
     
        // do we still have a URL open from the last read? 
        if ( test_url != cur_url && isOpen ) {
           // then close it
           Portal_close();          
        } 
     
        // is the url that we need open?
        if ( isOpen == 0 ) {
           // No? then tke care of that now
     
           Portal_open( test_url );        
        }
     
        grid3d->set_fillval( 1.0e15 );
          
        Portal_getvar( quantity, timex,  grid3d );
     
   } else {
       // the desired time is bracketed by two data set snapshot times
       // read in two data fields and interpolate. (Sigh)
       if ( dbug > 4 ) {
          std::cerr << "MetGEOSPortal::readPortal: (3D) bracketted time: getting " << preTime << " to " << postTime << ", or ";
       }
       preData = dynamic_cast<GridLatLonField3D*>(grid3d->duplicate());
       ctime1 =  time2Cal(preTime);
       if ( dbug > 4 ) {
          std::cerr << ctime1 << " to ";
       }
       preData->set_time( preTime, ctime1 );
       readPortal( quantity, ctime1, preData );
       
       postData = dynamic_cast<GridLatLonField3D*>(grid3d->duplicate());
       ctime2 =  time2Cal(postTime);
       if ( dbug > 4 ) {
          std::cerr << ctime2 << std::endl;
       }
       postData->set_time( postTime, ctime2 );
       readPortal( quantity, ctime2, postData );
   
       // grid3D is empty of data, dimensions, and even some metadata
       // preData and postData have all of that
       // so let's give grid3d what it should have read
       *grid3d = *preData;
       // except set the time back to the target time
       grid3d->set_time( mtime, time );

       // now fill grid3d with time-interpolated data
       bad = preData->fillval();
       for ( src1 = preData->begin(), src2 = postData->begin(), dest = grid3d->begin();
             src1 != preData->end();
             src1++, src2++, dest++ ) {
       
             if ( *src1 != bad && *src2 != bad ) {
                // linearly interpolate in time
                *dest = ( *src2 - *src1 )/(postTime - preTime)*( mtime - preTime) + *src1;
             } else {
                *dest = bad;
             }
       
       }      
       std::cerr << " (*** WARNING: " << quantity << " for " << time << " was interpolated from " << ctime1 << " and " << ctime2 << " ***)" <<std::endl;   
       
       remove( preData );
       remove( postData );
   
   }

   // These quantities get automatically transformed 
   // into preferred units.
   // Their default is MKS/SI units.
   if ( grid3d->mksScale == 1.0 ) {
      if ( quantity == altitude_name || quantity == "altitude" ) {
         grid3d->transform("km", 0.0, 1000.0);
      } else if ( quantity == pressure_name || quantity == "air_pressure" ) {
         grid3d->transform("mb", 0.0, 100.0);
      } else {     
         // units should be OK
      }
   }
}

// Sfc
void MetGEOSPortal::readPortal( const std::string quantity, const std::string time, GridLatLonFieldSfc* grid2d )
{
     int ndims;
     double timex;
     double preTime, postTime;
     GridLatLonFieldSfc* preData;
     GridLatLonFieldSfc* postData;
     double mtime;
     std::string ctime1, ctime2;
     GridLatLonFieldSfc::iterator src1, src2, dest;
     real bad;
         
          
     mtime = cal2Time(time);
     grid2d->set_time( mtime, time );
     grid2d->set_quantity(quantity);
    
     if ( bracket( quantity, mtime, &preTime, &postTime ) ) { 
        
        // this sets, among other things, test_url 
        // throws an error if there is no such quantity
        // std::cerr << "about to setup for sfc read" << std::endl;
        ndims = prep( quantity, time );
        //- std::cerr << "done with setup for sfc read" << std::endl;
     
     
        if ( dbug > 3 ) {
           std::cerr << "MetGEOSPortal::readPortal: (Sfc) quantity=" << quantity << std::endl;
           std::cerr << "MetGEOSPortal::readPortal: (Sfc) ctime=" << time << std::endl;
           std::cerr << "MetGEOSPortal::readPortal: (Sfc) mtime=" << mtime << std::endl;
           std::cerr << "MetGEOSPortal::readPortal: (Sfc) url=" << test_url << std::endl;
           std::cerr << "MetGEOSPortal::readPortal: (Sfc) grid=" << test_hgrid << "," << test_vgrid << "," << test_tave << std::endl;
        }
     
        timex = cal2Time( time );
     
        // do we still have a URL open from the last read? 
        if ( test_url != cur_url && isOpen ) {
           // then close it
           Portal_close();        
        } 
     
        // is the url that we need open?
        if ( isOpen == 0 ) {
           // No? then tke care of that now
           
           //- std::cerr << "about to open " << test_url << std::endl;
           Portal_open( test_url );        
           //- std::cerr << "opened url " << std::endl;
        }
     
        // populate some attributes of the grid
        grid2d->set_fillval( 1.0e15 );
     
        //- std::cerr << "about to getvar " << quantity << std::endl;
        Portal_getvar( quantity, timex,  grid2d );
     
     } else {
        // the desired time is bracketed by two data set snapshot times
        // read in two data fields and interpolate. (Sigh)
        if ( dbug > 4 ) {
           std::cerr << "MetGEOSPortal::readPortal: (Sfc) bracketted time: getting " << preTime << " to " << postTime << ", or ";
        }
        preData = dynamic_cast<GridLatLonFieldSfc*>(grid2d->duplicate());
        ctime1 =  time2Cal(preTime);
        if ( dbug > 4 ) {
           std::cerr << ctime1 << " to ";
        }
        preData->set_time( preTime, ctime1 );
        readPortal( quantity, ctime1, preData );
       
        postData = dynamic_cast<GridLatLonFieldSfc*>(grid2d->duplicate());
        ctime2 =  time2Cal(postTime);
        if ( dbug > 4 ) {
           std::cerr << ctime2 << std::endl;
        }
        postData->set_time( postTime, ctime2 );
        readPortal( quantity, ctime2, postData );
   
        // grid2d is empty of data, dimensions, and even some metadata
        // preData and postData have all of that
        // so let's give grid3d what it should have read
        *grid2d = *preData;
        // except set the time back to the target time
        grid2d->set_time( mtime, time );

        // now fill grid3d with time-interpolated data
        bad = preData->fillval();
        for ( src1 = preData->begin(), src2 = postData->begin(), dest = grid2d->begin();
              src1 != preData->end();
              src1++, src2++, dest++ ) {
       
              if ( *src1 != bad && *src2 != bad ) {
                 // linearly interpolate in time
                 *dest = ( *src2 - *src1 )/(postTime - preTime)*( mtime - preTime) + *src1;
              } else {
                 *dest = bad;
              }
       
        } 
        std::cerr << " (*** WARNING: " << quantity << " for " << time << " was interpolated from " << ctime1 << " and " << ctime2 << " ***)" << std::endl;   
       
        remove( preData );
        remove( postData );
   
     }
     
     // These quantities get automatically transformed 
     // into preferred units.
     // Their default is MKS/SI units.
     if ( grid2d->mksScale == 1.0 ) {
        if ( quantity == altitude_name || quantity == "altitude" ) {
           grid2d->transform("km", 0.0, 1000.0);
        } else if ( quantity == pressure_name || quantity == "air_pressure" ) {
           grid2d->transform("mb", 0.0, 100.0);
        } else {     
           // units should be OK
        }
     }
     

}


bool MetGEOSPortal::vConvert( GridField3D *input, std::string quant, std::string units, real scale, real offset )
{
    std::string currvq;
    int nlons, nlats, nzs;
    std::vector<real> vcoord;
    real val;
    
    input->dims( &nlons, &nlats, &nzs );
    
    // get the current vertical quantity
    currvq = input->vertical();

    if ( currvq == pressure_name && quant == palt_name ) {
       vcoord = input->levels();
       if ( dbug > 1 ) {
           std::cerr << "**** converting " << currvq << " to " << quant 
                     << " for " << input->quantity() << " grid " << std::endl;
       }

       if ( units == "" ) { 
          units = "km";
       }
       if (units == "km" ) {
          scale = 1000.0;
       }

       for ( int i=0; i < nzs; i++ ) {
           val =  vcoord[i]*input->mksVScale + input->mksVOffset;
           val = getpalt.calc( val );
           val = ( val*1000.0 - offset)/scale;

           // DON'T do this! we want to convert the GRID's vertical coord,
           // not the METSOURCE's vertical coord.
           //zs[i] = val;

           vcoord[i] = val;
       }
       // replace the old with the new
       input->levels( vcoord );    
       input->set_vertical( quant );
       input->set_vunits( units, scale, offset );
       
       return true;
    } else if ( currvq == palt_name && quant == pressure_name ) {
       vcoord = input->levels();
       if ( dbug > 1 ) {
           std::cerr << "**** converting " << currvq << " to " << quant 
                     << " for " << input->quantity() << " grid " << std::endl;
       }

       if ( units == "" ) { 
          units = "hPa";
       }
       if (units == "mb" || units == "hPa" ) {
          scale = 100.0;
       }

       for ( int i=0; i < nzs; i++ ) {
           val =  vcoord[i]*input->mksVScale + input->mksVOffset;
           val = getpress.calp( val/1000.0 );
           val = ( val - offset)/scale;
           // replace the old with the new
           //zs[i] = val;
           vcoord[i] = val;
       }
    
       input->levels( vcoord );    
       input->set_vertical( quant );
       input->set_vunits( units, scale, offset );
       
       return true;
    }


    return false;
}




//*********************  PRIVATE METHODS *********************






