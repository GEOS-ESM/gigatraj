
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

const gigatraj::MERRA_Directory MetMERRA::dir;



// default constructor
MetMERRA::MetMERRA() : MetGEOSPortal() 
{    
    
    setup_vars();   
    
    setup_gridSpecs();   
    
    legalDims.push_back("XDim");
    legalDims.push_back("YDim");
    legalDims.push_back("Height");
    legalDims.push_back("TIME");
    
    reset();   

}

MetMERRA::MetMERRA( std::string& date ) : MetGEOSPortal() 
{
    
    setup_vars();   
    
    legalDims.push_back("XDim");
    legalDims.push_back("YDim");
    legalDims.push_back("Height");
    legalDims.push_back("TIME");

    setup_gridSpecs();
   
    reset();
   
    basetime = cal.day1900( date );

}


// destructor
MetMERRA::~MetMERRA() 
{
    
}

void MetMERRA::setup_vars()
{
    vWindStuff vw;

    altitude_name = "Z";
    pressure_name = "P";
    palt_name = "PAlt";
    pottemp_name = "Theta";
    
      vw.quantity = "W";
      vw.units = "m/s";
      vw.MKSscale = 1.0;
      vw.MKSoffset = 0.0;
      vertwind_quants[ altitude_name ] = vw;
    
      vw.quantity = "OMEGA";
      vw.units = "Pa/s";
      vw.MKSscale = 1.0;
      vw.MKSoffset = 0.0;
      vertwind_quants[ pressure_name ] = vw;
    
      vw.quantity = "ThetaDot";
      vw.units = "K/s";
      vw.MKSscale = 1.0;
      vw.MKSoffset = 0.0;
      vertwind_quants[ pottemp_name ] = vw;
    
    gettheta.set_quantity(pottemp_name);
    gettheta.setTemperatureName("T");
    gettheta.setPressureName(pressure_name);
    
    getpress.set_quantity(pressure_name);
    getpress.setTemperatureName("T");
    getpress.setPotentialTemperatureName(pottemp_name);
    getpress.setPressureThicknessName("DELP");
    getpress.setDensityName("Density");

    getthetadot.set_quantity((vertwind_quants[ pottemp_name ]).quantity);
    getthetadot.setTemperatureDotName("DTDTTOT"); // note: this is not present in all data streams on the server
    getthetadot.setTemperatureName("T");
    getthetadot.setPressureName(pressure_name);
    getthetadot.setPotentialTemperatureName(pottemp_name);
    getthetadot.setPressureDotName((vertwind_quants[ pressure_name ]).quantity);
    
    getpalt.set_quantity(palt_name);
    getpalt.setPressureName( pressure_name );

    getpaltdot.set_quantity((vertwind_quants[ palt_name ]).quantity);
    getpaltdot.setPressureAltitudeName(altitude_name);
    getpaltdot.setPressureName(pressure_name);
    getpaltdot.setPressureDotName((vertwind_quants[ pressure_name ]).quantity);
    

}

void MetMERRA::setup_gridSpecs()
{
    HGridSpec hspec;
    VGridSpec vspec;
    int i;
    
    hspec.code = 'N';
    hspec.id   = 0;
    hspec.startLon = -180.0;
    hspec.endLon = -180.0 + 539*2./3.;
    hspec.nLons = 540;
    hspec.deltaLon = (hspec.endLon - hspec.startLon)/(hspec.nLons - 1);
    hspec.startLat = -90.0;
    hspec.endLat = 90.0;
    hspec.nLats = 361;
    hspec.deltaLat = (hspec.endLat - hspec.startLat)/(hspec.nLats - 1);
    hspecs.push_back( hspec );

    hspec.code = 'C';
    hspec.id   = 1;
    hspec.startLon = -179.3750;
    hspec.endLon = 179.3750;
    hspec.nLons = 288;
    hspec.deltaLon = (hspec.endLon - hspec.startLon)/(hspec.nLons - 1);
    hspec.startLat = -90.0;
    hspec.endLat = 90.0;
    hspec.nLats = 144;
    hspec.deltaLat = (hspec.endLat - hspec.startLat)/(hspec.nLats - 1);
    hspecs.push_back( hspec );

    hspec.code = 'F';
    hspec.id   = 2;
    hspec.startLon = -180.0;
    hspec.endLon = 178.750;
    hspec.nLons = 288;
    hspec.deltaLon = (hspec.endLon - hspec.startLon)/(hspec.nLons - 1);
    hspec.startLat = -90.0;
    hspec.endLat = 90.0;
    hspec.nLats = 181;
    hspec.deltaLat = (hspec.endLat - hspec.startLat)/(hspec.nLats - 1);
    hspecs.push_back( hspec );
    
   vspec.code = 'x';
   vspec.id   = 0;
   vspec.quant = "<quasi-horizontal surface>";
   vspec.units = "";
   vspec.nLevs = 0;
   vspec.levs.clear();
   vspecs.push_back( vspec );
   
   vspec.code = 'n';
   vspec.id   = 1;
   vspec.quant = "Model-Levels";
   vspec.units = "";
   vspec.nLevs = 71;
   vspec.levs.clear();
   for ( i=1; i<=72; i++ ) {
       vspec.levs.push_back(  static_cast<real>(i) ); 
   }
   vspecs.push_back( vspec );
   
   vspec.code = 'p';
   vspec.id = 2;
   vspec.quant = "P";
   vspec.units = "hPa";
   vspec.nLevs = 42;
   vspec.levs.clear();
   vspec.levs.push_back( 1000.  );
   vspec.levs.push_back(  975.  );
   vspec.levs.push_back(  950.  );
   vspec.levs.push_back(  925.  );
   vspec.levs.push_back(  900.  );
   vspec.levs.push_back(  875.  );
   vspec.levs.push_back(  850.  );
   vspec.levs.push_back(  825.  );
   vspec.levs.push_back(  800.  );
   vspec.levs.push_back(  775.  );
   vspec.levs.push_back(  750.  );
   vspec.levs.push_back(  725.  );
   vspec.levs.push_back(  700.  );
   vspec.levs.push_back(  650.  );
   vspec.levs.push_back(  600.  );
   vspec.levs.push_back(  550.  );
   vspec.levs.push_back(  500.  );
   vspec.levs.push_back(  450.  );
   vspec.levs.push_back(  400.  );
   vspec.levs.push_back(  350.  );
   vspec.levs.push_back(  300.  );
   vspec.levs.push_back(  250.  );
   vspec.levs.push_back(  200.  );
   vspec.levs.push_back(  150.  );
   vspec.levs.push_back(  100.  );
   vspec.levs.push_back(   70.  );
   vspec.levs.push_back(   50.  );
   vspec.levs.push_back(   40.  );
   vspec.levs.push_back(   30.  );
   vspec.levs.push_back(   20.  );
   vspec.levs.push_back(   10.  );
   vspec.levs.push_back(    7.  );
   vspec.levs.push_back(    5.  );
   vspec.levs.push_back(    4.  );
   vspec.levs.push_back(    3.  );
   vspec.levs.push_back(    2.  );
   vspec.levs.push_back(    1.  );
   vspec.levs.push_back(    0.7 );
   vspec.levs.push_back(    0.5 );
   vspec.levs.push_back(    0.4 );
   vspec.levs.push_back(    0.3 );
   vspec.levs.push_back(    0.1 );
   vspecs.push_back( vspec );
   
   vspec.code = 'e';
   vspec.id = 3;
   vspec.quant = "Model Edges";
   vspec.units = "";
   vspec.nLevs = 72;
   vspec.levs.clear();
   for ( i=0; i<73; i++ ) {
       vspec.levs.push_back(  static_cast<real>(i) ); 
   }
   vspecs.push_back( vspec );

}

double MetMERRA::Portal_time_nativeTo1900( double nativeTime )
{
      //return nativeTime/24.0/3600.0  + 33969.000;
      return nativeTime/24.0/60.0  + time_zero;
};

double MetMERRA::Portal_deltime_nativeTo1900( double nativeDelTime )
{
      return nativeDelTime/24.0/60.0;
};

void MetMERRA::reset()
{
   std::string vunits;

   MetGEOSPortal::reset();
      

   set_hgrid( 0 );

   // Native GEOSfp variable names
   //  zonal wind
   wind_ew_name = "U";
   //  meridional wind
   wind_ns_name = "V";
   // vertical "wind" will depend on the vertical coordinate system
   // and is set by set_vertical()

   set_vgrid(2); 
   set_vertical(native_vquant, native_vuu, &native_zs );   

}

// copy constructor
MetMERRA::MetMERRA( const MetMERRA&  src) : MetGEOSPortal(src)
{
}    

void MetMERRA::assign(const MetMERRA& src)
{
     MetGEOSPortal::assign(src);
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



       
bool MetMERRA::legalQuantity( const std::string quantity )
{
     bool result;
     int status;
     std::vector<string> *testquants;
     std::vector<string>::iterator q;
     std::string dayt;
     
     // use a day that we know is in the data set
     dayt = "2010-01-01T00:00:00";

     // start by assuming the quantity if illegal
     result = false;
     testquants = new_testQuantity( quantity );
     
     status = 0;
     for ( q = testquants->begin(); q != testquants->end() && status == 0; q++ ) {
         try {
             status = dir.LookUp( *q, dayt, -1, -1, -1, -1
                                , NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
     
         } catch (MERRA_Directory::badName err) {
            status = 1;
         } catch (MERRA_Directory::badNotFound err) {
            status = 1;
         }
     }

     if ( status == 0 ) {     
        result = true;        
     } else {                 
        if ( quantity == altitude_name 
          || quantity == pressure_name 
          || quantity == palt_name  
          || quantity == "Model-Levels"
        ) {                   
           result = true;     
        }                     
     }                        
     
     delete testquants;
     
     //std::cerr << "tested quantity " << quantity << " and got " << result << std::endl;
     
     return result;
}



void MetMERRA::setOption( const std::string &name, const std::string &value )
{
}

void MetMERRA::setOption( const std::string &name, int value )
{
    int tmp;
    
    if ( name == "HorizontalGridThinning" ) {
        set_thinning(value);
    } else if ( name == "HorizontalGridOffset" ) {
        tmp = thinning();
        set_thinning( tmp, value );
    } else if ( name == "Delay" ) {
        setWaitOpen(value);
    }
    
}

void MetMERRA::setOption( const std::string &name, float value )
{
}

void MetMERRA::setOption( const std::string &name, double value )
{
}

bool MetMERRA::getOption( const std::string &name, std::string &value )
{
   value = "";
   return false;
}


bool MetMERRA::getOption( const std::string &name, int &value )
{
    bool result = false;
    int tmp;
    
    if ( name == "HorizontalGridThinning" ) {
        value = thinning();
        result = true;
    } else if ( name == "HorizontalGridOffset" ) {
        (void) thinning(&tmp);
        value = tmp;
        result = true;
    }
    return result;
}


bool MetMERRA::getOption( const std::string &name, float &value )
{
    bool result = false;
    value = 0.0;
    return result;
}


bool MetMERRA::getOption( const std::string &name, double &value )
{
    bool result = false;
    value = 0.0;
    return result;
}



FilePath* MetMERRA::cachefile( const GridFieldSfc* item ) const
{
    FilePath* filepath;
    std::string fname;
    std::string date;
    std::string type;
    std::string flags;
    std::string month, year, day;
    std::string attr;
    const GridLatLonFieldSfc* actualItem;
    std::stringstream ss;
    
    actualItem = dynamic_cast<const GridLatLonFieldSfc*>(item);

    filepath = NULLPTR;

    if ( item->cacheable() ) {

       filepath = new FilePath;
       *filepath = *diskcachedir;
       
       flags = "";
       try {
          attr = actualItem->attribute("MASS_BALANCE");
       } catch ( GridField::badMissingAttr ) {
          attr = "";
       } 
       if ( attr != "" ) {
          flags = flags + "B";
       }
       if ( skip > 1 ) {
          ss.flush();
          ss << "R:" << skip;
          if ( skoff > 0 ) {
             ss << ":" << skoff;
          }
          flags = flags + ss.str();
       }
       
       date = actualItem->met_time();
       
       fname = "MetMERRA_"
               + actualItem->quantity()
               + "_" + actualItem->met_time()
               + "_" + flags
               + "_Sfc" + actualItem->surface()
               + ".cache";
       
       // now set up subdirectories
       year = date.substr(0,4);
       month = date.substr(5,2);
       day = date.substr(8,2);
       filepath->append( "Y"+year  );
       filepath->append( "M"+month );
       filepath->append( "D"+day   );

       
       // put the file name at the end of the path
       filepath->append( fname );
       
    }
    
    return filepath;  
    
}


FilePath* MetMERRA::cachefile( const GridField3D* item ) const
{
    FilePath* filepath;
    std::string fname;
    std::string date;
    std::string type;
    std::string flags;
    std::string month, year, day;
    std::string attr;
    const GridLatLonField3D* actualItem;
    std::stringstream ss;
    
    actualItem = dynamic_cast<const GridLatLonField3D*>(item);

    filepath = NULLPTR;

    if ( item->cacheable() ) {

       filepath = new FilePath;
       *filepath = *diskcachedir;
       
       flags = "";
       try {
          attr = actualItem->attribute("MASS_BALANCE");
       } catch ( GridField::badMissingAttr ) {
          attr = "";
       } 
       if ( attr != "" ) {
          flags = flags + "B";
       }
       if ( skip > 1 ) {
          ss.flush();
          ss << "R:" << skip;
          if ( skoff > 0 ) {
             ss << ":" << skoff;
          }
          flags = flags + ss.str();
       }
       
       date = actualItem->met_time();
       
       
       
       fname = "MetMERRA_"
               + actualItem->quantity()
               + "_" + date
               + "_" + flags
               + "_3D_" + actualItem->vertical()
               + ".cache";
       
       // now set up subdirectories
       year = date.substr(0,4);
       month = date.substr(5,2);
       day = date.substr(8,2);
       filepath->append( "Y"+year  );
       filepath->append( "M"+month );
       filepath->append( "D"+day   );

       // put the file name at the end of the path
       filepath->append( fname );
       
    }
    
    return filepath;    
    
}


std::string MetMERRA::quantityName_cf2geos(const std::string &quantity ) const
{
     
     return dir.MERRAName( quantity );        

}


std::string MetMERRA::quantityName_geos2cf(const std::string &quantity ) const
{
     std::string result;
     
     result = dir.CFName( quantity );
     
     return result;        

}



int MetMERRA::setup(  const std::string quantity, const double time )
{
    std::string caltime;
    std::string subquant;

    // convert model time to calendar time
    caltime = time2Cal(time);

    if ( quantity == pottemp_name || quantity == "air_potential_temperature" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return setup( "T", caltime );

    } else if ( quantity == "ThetaDot" || quantity == "net_heating_rate" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return setup( "DTDTTOT", caltime ) & setup( "T", caltime ) ;

    } else if ( quantity == "W" || quantity == "upward_air_velocity" ) {
       
       return setup( "OMEGA", caltime );

    } else if (quantity == "P" || quantity == "air_pressure" ) {

       return setup( "U", caltime );
       
    } else if ( quantity == palt_name || quantity == "pressure_altitude" ) {
       
       // same as for pressure, above
       return setup( "U", caltime );

    } else if (quantity == altitude_name ) {

       return setup( "U", caltime );
       
    } else {
       // just ask for the quantity directly
       return setup( quantity, caltime ); 
    }


}    


int MetMERRA::setup(  const std::string quantity, const std::string &time )
{
    std::string caltime;
    int status;
    std::string *newUrl;
    int use_hgrid = -1;
    int use_vgrid = -1;
    int use_tspace = -1;
    int use_tavg = -1;
    int use_tbase = 0;

    // convert model time to calendar time
    caltime = time;
    
    caltime = caltime.substr(0,10);  // use just the date part

    //std::cerr << " in setup w/ quant " << quantity << " at time " << time << std::endl;
    
    // do we need to find the basic attributes of this quantity?
    if ( quantity != test_quant || caltime != test_date ) {

       //std::cerr << " testing " << std::endl;
    
       test_quant = quantity;
       test_date = caltime;
       
       // find the variable we are looking for
       status = dir.LookUp( test_quant, test_date, desired_vgrid_id, desired_hgrid_id, desired_tspace, desired_tave
            , NULL, NULL, &test_ndims
            , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl );
       if ( status && (strict != StrictAboutGridding) ) {
          if ( horizStrictness() ) {
             use_hgrid = desired_hgrid_id;
          }   
          if ( vertStrictness() ) {
             use_vgrid = desired_vgrid_id;
          }   
          if ( tspaceStrictness() ) {
             use_tspace = desired_tspace;
          }   
          if ( tavgStrictness() ) {
             use_tavg = desired_tave;
          }   
          status = dir.LookUp( test_quant, test_date, use_vgrid, use_hgrid, use_tspace, use_tavg
            , NULL, NULL, &test_ndims
            , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl );
       }
       
       if ( status == 0 ) {
          
          // std::cerr << " testing was successful.  ndims is " << test_ndims << std::endl;
          test_url = *newUrl;
          
          // use the date to set the time_zero
          time_zero = cal2Time(test_date) + basetime + test_tbase/60.0/24.0;
          
          delete newUrl;
       
       } else {
          throw(badDataNotFound());
       }
    }
      
    // std::cerr << " setup returns " << test_ndims << std::endl;
    
    
    return test_ndims;

}    


time_t MetMERRA::expiration( const std::string &valid_at )
{     
     return 0;
}


MetMERRA* MetMERRA::myNew()
{
   MetMERRA *dup;
   std::string dayt;
   
   dup = new MetMERRA;
   
   dup->debug = debug;
   dup->setPgroup(my_pgroup, my_metproc);
   dayt = time2Cal(0);
   dup->defineCal( dayt, 0.0 );
   dup->maxsnaps = this->maxsnaps;
   dup->setCacheDir( this->diskcachedir );
   dup->openwait = this->openwait;
   
   return dup;
}

bool MetMERRA::bracket( const std::string &quantity, const std::string &time, std::string *t1, std::string *t2)
{
    double prev, next;
    int true_hgrid, true_vgrid, true_tspace, true_tave, true_tbase;
    int status;
    double xtime, ytime, ptime;
    double mtime;
    double tbase=0;
    double tspace = 24;
    std::vector<std::string> *testquants;
    bool sametime;
    
    if ( debug > 5 ) {
       std::cerr << "MetMERRA::bracket: Bracketing time " << time << " against base " << basetime << std::endl;
    }
    
    // note: for on-the-fly calculations, we need
    // to get the bracket of the quantities that the OTF calculaiton
    // depends on. Here, we will just be using the 0th such quantity.
    // But if the component quantities are on different time ticks
    // this may not be sufficient.
    testquants = new_testQuantity( quantity );
    
    mtime = cal2Time( time );

    // find the variable we are looking for
    status = dir.LookUp( (*testquants)[0], time.substr(0,10), desired_vgrid_id, desired_hgrid_id, tspace, desired_tave
         , NULL, NULL, NULL 
         , &true_vgrid, &true_hgrid, &true_tspace, &true_tave, &true_tbase, NULL );
    if ( status && ! strict ) {
       status = dir.LookUp( (*testquants)[0], time.substr(0,10), -1, -1, -1, -1 
         , NULL, NULL, NULL 
         , &true_vgrid, &true_hgrid, &true_tspace, &true_tave, &true_tbase, NULL );
    }

    delete testquants;

    if ( status == 0 ) {
          // use the default from the data source
          tbase = true_tbase/60.0; // convert min->hours
          // use the default from the data source
          tspace = true_tspace;
    } else {
       mtime = 0.0;
    }   
    
    if ( override_tbase > 0 ) {
       // use the user-imposed base hour
       tbase = override_tbase;
    }      
    if ( override_tspace > 0 ) {
       // use the user-imposed time spacing
       tspace = override_tspace;
    }
    //std::cerr << "MetMERRA::bracket:  time=" << time << " (" << caltime << "), tspace=" << tspace << ", tbase = " << tbase << std::endl;
       
    // convert from model time to day1900
    ptime = mtime + basetime;
    // get the day1900 date
    xtime = floor( ptime );

    // convert the time part to intervals/indices
    ytime = (( ptime - xtime)*24.0 - tbase)/ tspace; 
    
    // get the datestamp of the previous integral time index
    prev = (floor(ytime)*tspace + tbase)/24.0 + xtime;
    
    //std::cerr << "MetMERRA::bracket:  ptime=" << ptime << " (" << time2Cal((ptime-basetime)) << "), xtime=" << xtime << ", ytime=" << ytime << std::endl;
    //std::cerr << "MetMERRA::bracket:  prev=" << prev << ", ptime - prev = " << ptime - prev << std::endl;
    //std::cerr << "MetMERRA::bracket:  floor(ytime)=" << floor(ytime) << ", ceil(ytime)=" << ceil(ytime) << ", ytime - floor(ytime) = " << ytime - floor(ytime) << std::endl;
    if ( ABS( prev - ptime )/tspace*24.0 > 15.0/3600.0/24.0 ) {
       // get the datestamp of the next integral time index
       next = (ceil(ytime)*tspace + tbase)/24.0 + xtime;
       sametime = false;
    } else {
       // no interpolation needed.
       //std::cerr << "BANG: bracketed times are indentical!" << std::endl;
       next = ptime;
       sametime = true;
    }
    //std::cerr << "MetMERRA::bracket:  next=" << next << std::endl;

    if ( debug > 5 ) {
       std::cerr << "MetMERRA::bracket:   Found times " << prev << " and " << next << " using interval " << true_tspace << std::endl;
    }
   
    *t1 = time2Cal((prev-basetime));
    *t2 = time2Cal((next-basetime));

    if ( debug > 5 ) {
       std::cerr << "MetMERRA::bracket:   Translated bracket times to  " << *t1 << " and " << *t2  << std::endl;
    }
 
    return sametime;
}

bool MetMERRA::bracket( const std::string &quantity, double time, double *t1, double *t2)
{
     std::string wanted, prev, next;
     bool result;
     
     wanted = time2Cal( time, 3 );
     result = bracket( quantity, wanted, &prev, &next );
     
     *t1 = cal2Time( prev );
     *t2 = cal2Time( next );
     
     return result;
}




std::vector<string> *MetMERRA::new_testQuantity( string quantity )
{
     std::vector<string> *result;
     int status;
     
     result = new std::vector<string>;
     
     if ( quantity == pottemp_name || quantity == "air_potential_temperature" ) {
        // for theta, we test for T
        result->push_back("T");
     } else if (quantity == "ThetaDot" || quantity == "net_heating_rate" ) {
        // for theta-dot (net heating rate), we test for T and DTDTTOT
        result->push_back("T");
        result->push_back("DTDTTOT");
     } else if (quantity == "W" || quantity == "upward_air_velocity" ) {
        result->push_back("OMEGA");
     } else if (quantity == pressure_name || quantity == "air_pressure" ) {
        // for pressure, we test for T
        result->push_back("T");
     } else if ( quantity == palt_name || quantity == "pressure_altitude" ) {
        // same as for pressure, above
        result->push_back("T");
     } else if (quantity == altitude_name ) {
        result->push_back("T");
     } else {
        // all else, we test for the quantity itself
        result->push_back(quantity);
     }
     
     //std::cerr << "tested quantity " << quantity << " and got " << result << std::endl;
     
     return result;
}

void MetMERRA::set_vertical( const std::string quantity, const std::string units )
{
     set_vertical( quantity, units, NULLPTR );
}
void MetMERRA::set_vertical( const std::string quantity, const std::string units, const std::vector<real>* levels )
{
     std::string my_units;
     std::string my_wind_vert_name;
     std::string my_quant;
     std::vector<real>* levs;
     real myScale, myOffset;
     
     my_quant = pressure_name;
     if ( quantity != "" ) {
        my_quant = quantity;
     }   
     // note: MERRA does not actually define variable names for
     //   altitude, air_pressure, Theta, upward_air_velocity,
     //   or net heating rate.  So we leave those as lowercase

     if ( my_quant == altitude_name || my_quant == "altitude" ) {
        my_units = "km";
        my_wind_vert_name = "W";
        wfctr = 0.001; // w gets scaled from m/s to km/s
     } else if ( my_quant == pressure_name || my_quant == "air_pressure" ) {
        my_units = "hPa";
        my_wind_vert_name = "OMEGA";;
        wfctr = 0.01; // omega get scaled from Pa/s to hPa/s
     } else if ( my_quant == pottemp_name || my_quant == "air_potential_temperature" ) {
        my_units = "K";
        my_wind_vert_name = "ThetaDot";
        wfctr = 1.0;
     } else if ( my_quant == palt_name || my_quant == "pressure_altitude" ) {
        my_units = "km";
        my_wind_vert_name = "W";;
        wfctr = 0.001; // w gets scaled from m/s to km/s
     } else {
        throw(badVerticalCoord());
     }         
    
     if ( units != "" ) {
        my_units = units;
     }

     vMKS( my_quant, my_units, &myScale, &myOffset );
    
     MetGridData::set_verticalBase( quantity, my_units, NULLPTR, myScale, myOffset );
     wind_vert_name = my_wind_vert_name;
    
     if ( levels == NULLPTR ) {
       // use the default set of vertical values for this coord system
        levs = vcoords( &my_quant );
        zs = *levs;
        delete levs;
     } else {
        // use what was provided
        zs = *levels;     
     }

}
 

GridLatLonField3D* MetMERRA::new_directGrid3D( const std::string quantity, const std::string time )
{
    int i,j,k;
    real lon, lat, z;
    GridLatLonField3D* grid3d;
    GridLatLonField3D* tmp1;
    GridLatLonField3D* component_1;
    GridLatLonField3D* component_2;
    GridLatLonField3D* component_3;
    double mtime;
    real val;
    std::vector<real> data;
    MetMERRA *defaultThis;
    time_t zeep;
    std::vector<real> *tmpvals;

    //std::cerr << "Entering new_directGrid3D" << std::endl;

    if ( quantity == "nodataload" ) {
       throw (MetGridData::baddataload());
    }   

    // std::cerr << "*** TIME = " << time << std::endl;
    
    //std::cerr << "reading " << quantity << " @ " << time << std::endl;

    // translate the time    
    mtime = cal2Time( time );


    /// 3D gridded field
    
    grid3d = new GridLatLonField3D();
    grid3d->setPgroup( my_pgroup, my_metproc );

    // Some quantities do not exist in files that can be read in,
    // so we need to calculate them on the fly, from quantities
    // that we can read in.
    if ( quantity == pottemp_name || quantity == "air_potential_temperature" ) {
       // We need to read in other quantities and calculate the desired quantity
       // from them.  We would also like to take advantage of any caching
       // being done, since for example in this case we would be reading
       // T in twice: once explicitly and once as part of "reading" P.
       // Thus, we want to call the Grid3D method instead of Obtain3D.
       // However, Grid3D will do any horizontal and/or vertical interpolation
       // that is needed for our final desired grid, and we do not want that at
       // this stage.  So we use a local instance of the metGEOSfp object,
       // with default settings (except for caching), and we use that to
       // read in our component quantities.
       defaultThis = myNew();
       // set in the above call: defaultThis->diskcaching = this->diskcaching;
       component_1 = defaultThis->Get3D("T", time);
       component_2 = defaultThis->Get3D(pressure_name, time);
       tmp1 = dynamic_cast<GridLatLonField3D*>(gettheta.calc( *component_1, *component_2 ));
       *grid3d = *tmp1;
       delete tmp1;
       defaultThis->remove( component_2 ); 
       defaultThis->remove( component_1 );
       delete defaultThis;
    } else if ( quantity == pressure_name || quantity == "air_pressure" ) {
       defaultThis = myNew();
       // set in the above call: defaultThis->diskcaching = this->diskcaching;

       (void) this->setup("U", time);
       if ( test_vgrid == 2 ) {
          // data are on pressure levels; just grab the vert coord
          component_1 = defaultThis->Get3D("U", time);
          tmp1 = dynamic_cast<GridLatLonField3D*>(getpress.calc( *component_1 ));
          *grid3d = *tmp1;
          delete tmp1;
          defaultThis->remove( component_1 );
       } else if ( test_vgrid == 1 ) {
          // data are on model levels--read the layer thicknesses
          // and calculate pressure from them
          // (Note that PL is not available for MERRA, so we must use DELP.)
          component_1 = defaultThis->Get3D("DELP", time);
          // the "1.0" is the pressure (in Pa) imposed at the top GMAO model level
          tmp1 = dynamic_cast<GridLatLonField3D*>(getpress.calc( *component_1, 1.0 ));
          *grid3d = *tmp1;
          delete tmp1;
          defaultThis->remove( component_1 );
          // Note: pressure data are in Pa; convert to hPa (mb)
          grid3d->transform("hPa", 100.0, 0.0);
       } else {
          throw (badDataNotFound());
       }   
       
       delete defaultThis;

    } else if ( quantity == "ThetaDot" || quantity == "net_heating_rate" ) {
    
       defaultThis = myNew();
       // set in the above call: defaultThis->diskcaching = this->diskcaching;
       /* for now, we will build our ThetaDot based on dT/dt and theta.
          But we ought to use the OMEGA field in the calculation
          as well.
       */   
       component_1 = defaultThis->Get3D("DTDTTOT", time);
       component_2 = defaultThis->Get3D(pottemp_name, time);
       if ( ! component_2->compatible( *component_1 ) ) {
          // make sure the two fields are on the same spatial grid.
          // DTDTTOT is usually on a coarser grid.
          component_3 = reconcile( *component_1, *component_2 );
          // replace the bad-grid dtttot with the new-grid version
          defaultThis->remove( component_1 );
          component_1 = component_3;
       }
       // compute ThetaDot
       tmp1 = dynamic_cast<GridLatLonField3D*>(getthetadot.calc( *component_1, *component_2 ));
       *grid3d = *tmp1;
       delete tmp1;
       // put code to do the mass balance adjustment here
       defaultThis->remove( component_2 ); 
       defaultThis->remove( component_1 );
       delete defaultThis;
    
    } else if (  quantity == "W" || quantity == "upward_air_velocity" ) {

       defaultThis = myNew();
       // set in the above call: defaultThis->diskcaching = this->diskcaching;
       component_1 = defaultThis->Get3D("OMEGA", time);
       component_2 = defaultThis->Get3D(pressure_name, time);
       tmp1 = dynamic_cast<GridLatLonField3D*>(getpaltdot.calc( *component_1, *component_2 ));
       *grid3d = *tmp1;
       delete tmp1;
       defaultThis->remove( component_2 );
       defaultThis->remove( component_1 );
       delete defaultThis;    
    
    } else if ( quantity == palt_name || quantity == "pressure_altitude" ) {

       defaultThis = myNew();
       component_1 = defaultThis->Get3D(pressure_name, time);
       tmp1 = dynamic_cast<GridLatLonField3D*>(getpalt.calc( *component_1 ));
       *grid3d = *tmp1;

       defaultThis->remove( component_1 );
       delete defaultThis;    
    
    } else if ( quantity == "air_density" ) {
    
    } else {

       readPortal( quantity, time, grid3d );

    }
    
    grid3d->setPgroup( my_pgroup, my_metproc );
    
    // set the expiration time
    zeep = expiration( time );
    grid3d->set_expires( zeep );
    
    //std::cerr << "leaving new_directGrid3D" << std::endl;

    return grid3d;
    

}

GridLatLonFieldSfc* MetMERRA::new_directGridSfc( const std::string quantity, const std::string time )
{
    int i,j;
    real lon, lat;
    GridLatLonFieldSfc* gridsfc;
    GridLatLonFieldSfc* desiredsfc;
    GridLatLonField3D* grid3D;
    GridLatLonField3D* desired3D;
    double mtime;
    std::vector<real> data;
    std::string sfcname;
    std::string quantname;
    size_t pos;
    time_t zeep;

    if ( quantity == "nodataload" ) {
       throw (MetGridData::baddataload());
    }   

    // split the quantity name into quantity and surface
    pos = quantity.find("@");
    if ( pos != string::npos ) {
       quantname = quantity.substr(0, pos);
       sfcname = quantity.substr(pos+1);
    } else {
       //todo:  put in switch block here
       quantname = quantity;
       sfcname = "sfc";   // note where we are evaluating the quantity, below
    }


    // translate the time    


    // create the output gridded surface
    gridsfc = new GridLatLonFieldSfc();
    gridsfc->setPgroup( my_pgroup, my_metproc );
    

    if ( sfcname == "trop" ) {
       // The surface is the tropopause
       gridsfc->set_quantity(quantity);
       gridsfc->set_units("");
       gridsfc->set_time( mtime, time );
       gridsfc->set_fillval( -9999.0 );
       gridsfc->set_surface(sfcname);
    
       // get the 3D temperatures on our desired vertical coordinate
       grid3D = new_directGrid3D("t", time); // get temperature on altitude
       // compute tropopause from temp on the desired vertical coordinate
       desiredsfc = new GridLatLonFieldSfc();
//       *desiredsfc = dynamic_cast<GridLatLonFieldSfc&>(tropgen.wmo( *grid3D ));
// FIX THIS!

       // get the desired quantity on destied vertical coord
       desired3D = new_directGrid3D(quantname, time); 
       // interpolate the desired quantity onto the desired surface
       delete gridsfc;
       gridsfc = dynamic_cast<GridLatLonFieldSfc*>(vin->surface( *desired3D, *desiredsfc ));
       remove( desired3D );
       delete desiredsfc;
       remove( grid3D );
    } else if ( sfcname == "sfc" ) { 
       //std::cerr << "About to read " << quantity << " from sfc" << std::endl;
       readPortal( quantity, time, gridsfc );
       //std::cerr << "Back from reading " << quantity << " from sfc" << std::endl;

    } else {
      delete gridsfc;
      throw (badsurface());
    } 
    
    gridsfc->setPgroup( my_pgroup, my_metproc );

    // set the expiration time
    zeep = expiration( time );
    gridsfc->set_expires( zeep );

    return gridsfc;
    

}


void MetMERRA::delay()
{
    float wayt;
    int w;
    int c1;
    
    if ( my_pgroup->id() >= 0 ) {
        
       c1 = my_pgroup->numberOfProcessors() ;
       
       // max of three concurrent connections to the opendap server
       // allow about 6 seconds for each
       if ( c1 > 3 ) {
          wayt =   ( c1/3.0*6.0 );
       
          w = ( my_pgroup->random()*wayt + 0.5);
          
          if ( debug > 5 ) {
             std::cerr << "MetMERRA2::delay: sleeping for " << w 
                       << " sec from a max of " << wayt 
                       << " w/ " << c1 <<  " processors"
                       << std::endl;
          }
          (void) sleep( w );          
       }
    }
}


