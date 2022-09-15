
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

const gigatraj::GEOSfpFcast_Directory MetGEOSfp::fdir;
const gigatraj::GEOSfpAssim_Directory MetGEOSfp::adir;



// default constructor
MetGEOSfp::MetGEOSfp() : MetGEOSPortal() 
{    
    
    setup_vars();   
        
    legalDims.push_back("lon");
    legalDims.push_back("lat");
    legalDims.push_back("lev");
    legalDims.push_back("time");

    setup_gridSpecs();   
    
    reset();   

}

MetGEOSfp::MetGEOSfp( std::string& date ) : MetGEOSPortal() 
{
    setup_vars();   
        
    legalDims.push_back("lon");
    legalDims.push_back("lat");
    legalDims.push_back("lev");
    legalDims.push_back("time");
     
    setup_gridSpecs();
   
    reset();
   
    basetime = cal.day1900( date );

}


MetGEOSfp::MetGEOSfp( std::string& date, std::string& mrun ) : MetGEOSPortal() 
{
    
   setup_vars();   

   legalDims.push_back("lon");
   legalDims.push_back("lat");
   legalDims.push_back("lev");
   legalDims.push_back("time");
    
   setup_gridSpecs();
   
   reset();
   
   basetime = cal.day1900( date );

   modelRun = mrun;

}

// destructor
MetGEOSfp::~MetGEOSfp() 
{
    
}

void MetGEOSfp::setup_vars() 
{
    vWindStuff vw;
    
    altitude_name = "z";
    pressure_name = "p";
    palt_name = "PAlt";
    pottemp_name = "Theta";
    
      vw.quantity = "w";
      vw.units = "m/s";
      vw.MKSscale = 1.0;
      vw.MKSoffset = 0.0;
      vertwind_quants[ altitude_name ] = vw;
    
      vw.quantity = "omega";
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
    gettheta.setTemperatureName("t");
    gettheta.setPressureName(pressure_name);
    
    getpress.set_quantity(pressure_name);
    getpress.setTemperatureName("t");
    getpress.setPotentialTemperatureName(pottemp_name);
    getpress.setPressureThicknessName("delp");
    getpress.setDensityName("Density");

    getthetadot.set_quantity((vertwind_quants[ pottemp_name ]).quantity);
    getthetadot.setTemperatureDotName("dtdttot"); // note: this is not present in all data streams on the server
    getthetadot.setTemperatureName("t");
    getthetadot.setPressureName(pressure_name);
    getthetadot.setPotentialTemperatureName(pottemp_name);
    getthetadot.setPressureDotName((vertwind_quants[ pressure_name ]).quantity);
    
    getpalt.set_quantity(palt_name);
    getpalt.setPressureName( pressure_name );

    getpaltdot.set_quantity((vertwind_quants[ palt_name ]).quantity);
    getpaltdot.setPressureAltitudeName(palt_name);
    getpaltdot.setPressureName(pressure_name);
    getpaltdot.setPressureDotName("omega");

}

void MetGEOSfp::setup_gridSpecs()
{
    HGridSpec hspec;
    VGridSpec vspec;
    int i;
    
    hspec.code = 'N';
    hspec.id   = 0;
    hspec.startLon = -180.0;
    hspec.endLon = 179.6875;
    hspec.nLons = 1152;
    hspec.deltaLon = (hspec.endLon - hspec.startLon)/(hspec.nLons - 1);
    hspec.startLat = -90.0;
    hspec.endLat = 90.0;
    hspec.nLats = 721;
    hspec.deltaLat = (hspec.endLat - hspec.startLat)/(hspec.nLats - 1);
    hspecs.push_back( hspec );

    hspec.code = 'C';
    hspec.id   = 1;
    hspec.startLon = -180.0;
    hspec.endLon = 179.3750;
    hspec.nLons = 576;
    hspec.deltaLon = (hspec.endLon - hspec.startLon)/(hspec.nLons - 1);
    hspec.startLat = -90.0;
    hspec.endLat = 90.0;
    hspec.nLats = 361;
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
   vspec.quant = "Model Levels";
   vspec.units = "";
   vspec.nLevs = 72;
   vspec.levs.clear();
   for ( i=1; i<=72; i++ ) {
       vspec.levs.push_back(  static_cast<real>(i) ); 
   }
   vspecs.push_back( vspec );
   
   vspec.code = 'p';
   vspec.id = 2;
   vspec.quant = "p";
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
   vspec.nLevs = 73;
   vspec.levs.clear();
   for ( i=0; i<73; i++ ) {
       vspec.levs.push_back(  static_cast<real>(i) ); 
   }
   vspecs.push_back( vspec );

}

double MetGEOSfp::Portal_time_nativeTo1900( double nativeTime )
{
      return nativeTime - 693596.00;
};

double MetGEOSfp::Portal_deltime_nativeTo1900( double nativeDelTime )
{
      return nativeDelTime;
};


void MetGEOSfp::reset()
{
   std::string vunits;

   MetGEOSPortal::reset();
      
   modelRun = "";
   
   whichSrc = 0;
   forceSrc = 0;

   hgrid = 0;
   set_hgrid(hgrid, lons, lats);
   nlons = lons.size();
   nlats = lats.size();

   // Native GEOSfp variable names
   //  zonal wind
   wind_ew_name = "u";
   //  meridional wind
   wind_ns_name = "v";
   // vertical "wind" will depend on the vertical coordinate system
   // and is set by set_vertical()

   pgrid = 2;
   set_vgrid(pgrid, native_zs, native_vquant, vunits); 
   native_nzs = native_zs.size();
   set_vertical(native_vquant, vunits, &native_zs );   

}

// copy constructor
MetGEOSfp::MetGEOSfp( const MetGEOSfp&  src) : MetGEOSPortal(src)
{
     modelRun = src.modelRun;
     whichSrc = src.whichSrc;
     forceSrc = src.forceSrc;
     
}    

void MetGEOSfp::assign(const MetGEOSfp& src)
{
     MetGEOSPortal::assign(src);

     modelRun = src.modelRun;
     whichSrc = src.whichSrc;
     forceSrc = src.forceSrc;

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

std::string MetGEOSfp::model_run()
{
    return modelRun;
}

std::string MetGEOSfp::model_run_cal()
{ 
    std::string result;

    result = "";
    // modelrun should be in 'YYYYMMDD_HH" format
    //                        01234567890
    if ( modelRun.length() == 11 ) {
       result = modelRun.substr(0,4)
              + "-" + modelRun.substr(4,2)
              + "-" + modelRun.substr(6,2)
              + "T" + modelRun.substr(9,2)
              + ":00";
   } 
   
   return result;
}

void MetGEOSfp::set_model_run(std::string &mrun)
{
    modelRun = mrun;
}



int MetGEOSfp::which_src()
{
    return whichSrc;
}

int MetGEOSfp::which_forced_src()
{
    return forceSrc;
}

void MetGEOSfp::set_forced_src( int which )
{
    forceSrc = which;
}


int MetGEOSfp::queryQuantity( const std::string quantity
                , std::string **longName, std::string **units, int *ndims
                , int *actualVertGrid, int *actualHorizGrid, int *actualTimeSpace, int *actualTimeAvg, int *actualBaseTime
                , std::string **url )
{
    int status;
    int use_vgrid;
    int use_hgrid;
    int use_tspace;
    int use_tavg;
    
    // start out being flexible on everything
    use_vgrid = -1;
    use_hgrid = -1;
    use_tspace = -1;
    use_tavg = -1;

    // now be as strict as required    
    if ( horizStrictness() ) {
       use_hgrid = hgrid;
    }   
    if ( vertStrictness() ) {
       use_vgrid = pgrid;
    }   
    if ( tspaceStrictness() ) {
       use_tspace = tspace;
    }   
    if ( tavgStrictness() ) {
       use_tavg = tave;
    }   
    

    if ( forceSrc <= 0 ) {
       // try assimilation first
       try {
           status = adir.LookUp( quantity, use_hgrid, use_vgrid, use_tspace, use_tavg
                , longName, units
                , ndims, actualVertGrid, actualHorizGrid, actualTimeSpace, actualTimeAvg, actualBaseTime
                , url );
    
       } catch (GEOSfpAssim_Directory::badName err) {
          status = 1;
       } catch (GEOSfpAssim_Directory::badNotFound err) {
          status = 1;
       }
    } else {
        status = 1;
    } 
    if ( status == 1 && forceSrc >= 0 ) {
       // try the forecast  
       try {
           status = fdir.LookUp( quantity, use_hgrid, use_vgrid, use_tspace, use_tavg
                , longName, units
                , ndims, actualVertGrid, actualHorizGrid, actualTimeSpace, actualTimeAvg, actualBaseTime
                , url, modelRun );    
       } catch (GEOSfpFcast_Directory::badName err) {
          status = 1;
       } catch (GEOSfpFcast_Directory::badNotFound err) {
          status = 1;
       }
    }   

    return status;
}
       
bool MetGEOSfp::legalQuantity( const std::string quantity )
{
     bool result;
     int status;
     std::vector<string> *testquants;
     std::vector<string>::iterator q;

     // start by assuming the quantity if illegal
     result = false;
     testquants = new_testQuantity( quantity );
     
     status = 0;
     for ( q = testquants->begin(); q != testquants->end() && status == 0; q++ ) {

        status = status & queryQuantity( *q
                                       , NULL, NULL, NULL
                                       , NULL, NULL, NULL, NULL, NULL
                                       , NULL );
     }

     if ( status == 0 ) {     
        result = true;        
     } else {                 
        if ( quantity == altitude_name 
          || quantity == pressure_name
          || quantity == palt_name  
        ) {                   
           result = true;     
        }                     
     }                        
     
     delete testquants;
     
     //std::cerr << "tested quantity " << quantity << " and got " << result << std::endl;
     
     return result;
}



void MetGEOSfp::setOption( const std::string &name, const std::string &value )
{
}

void MetGEOSfp::setOption( const std::string &name, int value )
{
    int tmp;
    
    if ( name == "HorizontalGridThinning" ) {
        set_thinning(value);
    } else if ( name == "HorizontalGridOffset" ) {
        tmp = thinning();
        set_thinning( tmp, value );
    } else if ( name == "ForecastOnly" ) {
        if ( value == 1 ) {
           set_forced_src(1);
        }
    } else if ( name == "AnalysisOnly" ) {
        if ( value == 1 ) {
           set_forced_src(-1);
        }
    } else if ( name == "AnalysisAndForecast" ) {
        if ( value == 1 ) {
           set_forced_src(0);
        }
    } else if ( name == "Delay" ) {
        setWaitOpen(value);
    }
    
}

void MetGEOSfp::setOption( const std::string &name, float value )
{
}

void MetGEOSfp::setOption( const std::string &name, double value )
{
}

bool MetGEOSfp::getOption( const std::string &name, std::string &value )
{
   value = "";
   return false;
}


bool MetGEOSfp::getOption( const std::string &name, int &value )
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
    } else if ( name == "ForecastOnly" ) {
        tmp = which_forced_src();
        ( tmp > 0 ) ? value = 1 : value = 0;
        result = true;
    } else if ( name == "AnalysisOnly" ) {
        tmp = which_forced_src();
        ( tmp < 0 ) ? value = 1 : value = 0;
        result = true;
    } else if ( name == "AnalysisAndForecast" ) {
        tmp = which_forced_src();
        ( tmp == 0 ) ? value = 1 : value = 0;
        result = true;
    }
    return result;
}


bool MetGEOSfp::getOption( const std::string &name, float &value )
{
    bool result = false;
    value = 0.0;
    return result;
}


bool MetGEOSfp::getOption( const std::string &name, double &value )
{
    bool result = false;
    value = 0.0;
    return result;
}



FilePath* MetGEOSfp::cachefile( const GridFieldSfc* item ) const
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
       
       // note: Assim and Fcast cache files have the same name,
       // so that they can be used interchangeably.
       // The expiration date in the cache file determines
       // whether the fle is a forecast or not.
       fname = "MetGEOSfp_"
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


FilePath* MetGEOSfp::cachefile( const GridField3D* item ) const
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
       
       
       // note: Assim and Fcast cache files have the same name,
       // so that they can be used interchangeably.
       // The expiration date in the cache file determines
       // whether the fle is a forecast or not.
       fname = "MetGEOSfp_"
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


std::string MetGEOSfp::quantityName_cf2geos(const std::string &quantity ) const
{
     
     return fdir.GEOSfpFcastName( quantity );        

}


std::string MetGEOSfp::quantityName_geos2cf(const std::string &quantity ) const
{
     std::string result;
     
     result = fdir.CFName( quantity );
     
     return result;        

}



int MetGEOSfp::setup(  const std::string quantity, const double time )
{
    std::string caltime;
    std::string subquant;

    // convert model time to calendar time
    caltime = time2Cal(time);

    if ( quantity == pottemp_name || quantity == "air_potential_temperature" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return setup( "t", caltime );

    } else if ( quantity == "ThetaDot" || quantity == "net_heating_rate" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return setup( "dtdttot", caltime ) & setup( "t", caltime ) ;

    } else if ( quantity == "w" || quantity == "upward_air_velocity" ) {
       
       return setup( "omega", caltime );

    } else if (quantity == "p" || quantity == "air_pressure" ) {

       return setup( "u", caltime );
       
    } else if ( quantity == palt_name || quantity == "pressure_altitude" ) {
       
       // same as for pressure, above
       return setup( "u", caltime );

    } else if (quantity == altitude_name ) {

       return setup( "u", caltime );
       
    } else {
       // just ask for the quantity directly
       return setup( quantity, caltime ); 
    }


}    


int MetGEOSfp::setup(  const std::string quantity, const std::string &time )
{
    std::string caltime;
    int status;
    std::string *newUrl;
    int use_hgrid = -1;
    int use_vgrid = -1;
    int use_tspace = -1;
    int use_tavg = -1;
    int use_tbase = 0;
    std::string tmp_modelrun;
    std::string base_day;
    std::string base_run;
    double bday;
    double leadtime;
    double junktime;
    double xday, yday;
    double final_assim_day;

    // convert model time to calendar time
    caltime = time;
    
    caltime = caltime.substr(0,10);  // use just the date part
    xday = cal2Time( time ); // internal model time
    yday = xday + basetime; // day1900

    //std::cerr << " in setup w/ quant " << quantity << " at time " << time << std::endl;
    
    // do we need to find the basic attributes of this quantity?
    if ( quantity != test_quant || caltime != test_date ) {

       if ( debug >= 3 ) {
         std::cerr << "MetGEOSfp::setup: testing for " << quantity << " @ " << caltime << std::endl;
         std::cerr << "MetGEOSfp::setup: the model run is <<" << modelRun << ">>" << std::endl; 
       }
       test_quant = quantity;
       test_date = caltime;
       
      status = queryQuantity( quantity
               , NULL, NULL, &test_ndims
               , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, NULL );
       
       if ( status == 0 ) {
          
          test_url = "";
          if ( debug >= 3 ) {
             std::cerr << "MetGEOSfp::setup:  testing was successful.  ndims is " << test_ndims << std::endl;
          }
          
       } else {
          whichSrc = 0;
          
          throw(badDataNotFound());
       }
    }
      
    // std::cerr << " setup returns " << test_ndims << std::endl;
    
    
    return test_ndims;

}    


time_t MetGEOSfp::expiration( const std::string &valid_at )
{
     // the expiration timestamp
     time_t result;
     // expiration, in day1900 format
     double etime;
     // the valid_at time, in day1900 form
     double vtime;
     std::string idate;
     // the model initialization time, in day1900 format
     double itime;
     //  forecast lead time
     double lead_time;
     // the model run type
     std::string run;
     // expiration hours
     double ehours;
     
     // are the data from assimilation or from forecasts?
     if ( whichSrc < 0 ) {
        // assimilation--never expires
        result = 0; 
     } else {
        // forecast

        // we start with a value that guarntees that the adta
        // will always be expired. (0 means it never expires.)
        result = 1;
        // convert the valid_at time to day1900
        vtime = cal2Time( valid_at );
        // get the model run initialization time
        idate = model_run_cal();
        if ( idate != "" ) {
           // use the model run time
           itime = cal2Time( idate );  
        } else if ( cur_tgrid.n  > 0 ) {
           // the model run is "" (i.e., ".latest"),
           // but maybe we have some data available
           // for which we have times
           itime = cur_tgrid.start - basetime;
           //std::cerr << "MetGEOSfp::expiration: itime = " << itime << std::endl;
        } else {
           // Ok. The model run is "", and there are no data.
           // Nothing more we can do here except leave
           // the expiration as 1.
           itime = 0.0;
        }
     
        if ( itime > 0 ) {
           lead_time = (vtime - itime)*24.0; // converting to hours

           /* This diagram explains the logic below

               TABLE OF FORECAST LEAD TIMES, BY MODEL RUN

       valid-at    00Z   06Z  12Z  18Z   00Z  06Z  12Z  18Z

          01T00     0
          01T06     6     0
          01T12    12     6    0 
          01T18    18    12    6    0
          02T00    24    18   12    6     0
          02T06    30    24   18   12     6     0
          02T12    36    30   24   18    12     6    0 
          02T18    42         30   24    18    12    6    0    
          03T00    48         36   30    24    18   12    6    
          03T06    54         42         30    24   18   12
          03T12    60         48         36    30   24   18
          03T18    66         54         42         30   24
          04T00    72         60         48         36   30
          04T06    78         66         54         42     
          04T12    84         72         60         48     
          04T18    90         78         66         54     
          05T00    96         84         72         60     
          05T06   102         90         78         66 
          05T12   108         96         84         72 
          05T18   114        102         90         78 
          06T00   120        108         96         84 
          06T06   126        114        102         90 
          06T12   132        120        108         96 
          06T18   138                   114        102 
          07T00   144                   120        108 
          07T06   150                   126        114 
          07T12   156                   132        120 
          07T18   162                   138
          08T00   168                   144
          08T06   174                   150
          08T12   180                   156
          08T18   186                   162
          09T00   192                   168
          09T06   198                   174
          09T12   204                   180
          09T18   210                   186
          10T00   216                   192
          10T06   222                   198
          10T12   228                   204
          10T18   234                   210
          11T00   240                   216
          11T06                         222
          11T12                         228
          11T18                         234
                                        240



           */
           
           // extract the hour to see what run this is
           run = idate.substr( 11,2);
           if ( run == "00" ) {
              if ( lead_time <= 36.0 ) {
                 ehours = 6.0;
              } else if ( lead_time <= 132.0 ) {
                 ehours = 12.0;
              } else {
                 ehours = 24.0;
              }
           } else if ( run == "06" ) {
              ehours = 6.0;
           } else if ( run == "12" ) {
              if ( lead_time <= 36.0 ) {
                 ehours = 6.0;
              } else {
                 ehours = 12.0;
              }   
           } else if ( run == "18" ) {
              ehours = 6.0;
           } else {
                throw(badModelRun());    
           }
     
           // this is the time the data should expire, in day1900 form
           // Add an extra 8 hours to account for GMAO processing delays)
           etime = itime + (ehours + 8.0)/24.0;
     
           // now convert to time_t format (seconds elapsed since 1970-01-01T00 GMT)
           result = (etime - 25568.00)*24.0*3600.0; 
        }
     }
     
     return result;
}


MetGEOSfp* MetGEOSfp::myNew()
{
   MetGEOSfp *dup;
   
   dup = new MetGEOSfp;
   
   dup->debug = debug;
   dup->setPgroup(my_pgroup, my_metproc);
   dup->defineCal( time2Cal(0), 0.0 );
   dup->maxsnaps = this->maxsnaps;
   dup->setCacheDir( this->diskcachedir );
   dup->modelRun = modelRun;
   dup->openwait = this->openwait;
   
   return dup;
}

bool MetGEOSfp::bracket( const std::string &quantity, const std::string &time, std::string *t1, std::string *t2)
{
    double wanted, prev, next;
    bool result;

    wanted = cal2Time(time);
    
    if ( debug > 5 ) {
       std::cerr << "MetGEOSfp::bracket*: Bracketing cal time " << time << std::endl;
    }
    result = bracket( quantity, wanted, &prev, &next );
    
    *t1 = time2Cal(prev);
    *t2 = time2Cal(next);

    return result;
}

bool MetGEOSfp::bracket( const std::string &quantity, double time, double *t1, double *t2)
{
    double prev, next;
    std::string caltime;
    int true_hgrid, true_vgrid, true_tspace, true_tave, true_tbase;
    int status;
    double xtime, ytime, ptime, ztime;
    double tbase=0.0;
    double tspace=24;
    std::vector<std::string> *testquants;
    bool sametime;
    
    if ( debug > 5 ) {
       std::cerr << "MetGEOSfp::bracket: Bracketing time " << time << " against base " << basetime 
       << ": " << time2Cal(time, 4) << std::endl;
    }
    
    // note: for on-the-fly calculations, we need
    // to get the bracket of the quantities that the OTF calculaiton
    // depends on. Here, we will just be using the 0th such quantity.
    // But if the component quantities are on different time ticks
    // this may not be sufficient.
    testquants = new_testQuantity( quantity );
    
    // find the variable we are looking for
    status = queryQuantity( (*testquants)[0]
                , NULL, NULL, NULL
                , &true_vgrid, &true_hgrid, &true_tspace, &true_tave, &true_tbase, NULL );

    delete testquants;
    
    if ( status == 0 ) {
       // use the default from the data source
       tbase = true_tbase/60.0; // convert min->hours
       // use the default from the data source
       tspace = true_tspace;
    } else {
       time = 0.0;
    }   

    if ( override_tbase > 0 ) {
       // use the user-imposed base hour
       tbase = override_tbase;
    }      
    if ( override_tspace > 0 ) {
       // use the user-imposed time spacing
       tspace = override_tspace;
    }
    if ( debug > 50 ) {
       std::cerr << "MetGEOSfp::bracket: tbase = " << tbase << " tspace = " << tspace << std::endl;
    }
    
    // convert from model seconds to day1900
    ztime = time + basetime;
    // get the day1900 date
    xtime = floor( ztime );
    // convert the time part to intervals/indices
    ytime = (( ztime - xtime )*24.0 - tbase)/tspace; 
    if ( debug > 50 ) {
       std::cerr << "MetGEOSfp::bracket:  ztime = " << ztime << ", xtime= " << xtime << " , ytime=" << ytime  << std::endl;
    }
    
    // get the datestamp of the previous integral time index
    prev = (floor(ytime)*tspace + tbase)/24.0 + xtime;
    // get the datestamp of the next integral time index
    next = (ceil(ytime)*tspace + tbase)/24.0 + xtime;

    if ( abs( ztime - prev ) > 15.0/3600.0/24.0 ) {
       // the desired time is not the same as the previous time tick
       if ( abs( next - ztime ) > 15.0/3600.0/24.0 ) {
          // the desired time is not the same as the next time tick
          sametime = false;
       } else {
          sametime = true;
          prev = next;
       }   
    } else {
       sametime = true;
       next = prev;
    }

    if ( debug > 5 ) {
       std::cerr << "MetGEOSfp::bracket:   Found times " << prev << " and " << next << " using interval " << tspace << std::endl;
    }
   
    *t1 = prev - basetime;
    *t2 = next - basetime;

    if ( debug > 5 ) {
       std::cerr << "MetGEOSfp::bracket:   Translated bracket times to  " << *t1 << " (" << time2Cal(*t1)
       << ") and " << *t2  << " (" << time2Cal(*t2) << ")" << std::endl;
    }

    return sametime;
}




int MetGEOSfp::prep(  const std::string quantity, const double time )
{
    std::string caltime;
    std::string subquant;

    // convert model time to calendar time
    caltime = time2Cal(time);

    if ( quantity == pottemp_name || quantity == "air_potential_temperature" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return prep( "t", caltime );

    } else if ( quantity == "ThetaDot" || quantity == "net_heating_rate" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return prep( "dtdttot", caltime ) & prep( "t", caltime ) ;

    } else if ( quantity == "w" || quantity == "upward_air_velocity" ) {
       
       return prep( "omega", caltime );

    } else if (quantity == "p" || quantity == "air_pressure" ) {

       return prep( "t", caltime );
       
     } else if ( quantity == palt_name || quantity == "pressure_altitude" ) {
        // same as for pressure, above
       return prep( "t", caltime );

    } else if (quantity == altitude_name ) {

       return prep( "t", caltime );
       
    } else {
       // just ask for the quantity directly
       return prep( quantity, caltime ); 
    }


}    


int MetGEOSfp::prep(  const std::string quantity, const std::string &time )
{
    std::string caltime;
    int status;
    std::string *newUrl;
    int use_hgrid = -1;
    int use_vgrid = -1;
    int use_tspace = -1;
    int use_tavg = -1;
    int use_tbase = 0;
    std::string tmp_modelrun;
    std::string base_day;
    std::string base_run;
    double bday;
    double leadtime;
    double junktime;
    double xday, yday;
    double final_assim_day;

    // convert model time to calendar time
    caltime = time;
    
    caltime = caltime.substr(0,10);  // use just the date part
    xday = cal2Time( time ); // internal model time
    yday = xday + basetime; // day1900

    //std::cerr << " in prep w/ quant " << quantity << " at time " << time << std::endl;
    
    // do we need to find the basic attributes of this quantity?
    if ( test_url == "" || quantity != test_quant || caltime != test_date ) {

       if ( debug >= 3 ) {
         std::cerr << "MetGEOSfp::prep: testing for " << quantity << " @ " << caltime << std::endl;
         std::cerr << "MetGEOSfp::prep: the model run is <<" << modelRun << ">>" << std::endl; 
       }
       test_quant = quantity;
       test_date = caltime;
       
       // where should we look: assimilation or forecast?
       whichSrc = forceSrc;
       
       status = 1; // assume failure
       final_assim_day = 0.0; // assume there are no assimilation data
       if ( forceSrc <= 0 ) {
          // try assimilation
          
          // find the variable we are looking for
          status = adir.LookUp( test_quant, hgrid, pgrid, tspace, tave
               , NULL, NULL, &test_ndims
               , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl );
          if ( status && (strict != 0x07) ) {
             if ( horizStrictness() ) {
                use_hgrid = hgrid;
             }   
             if ( vertStrictness() ) {
                use_vgrid = pgrid;
             }   
             if ( tspaceStrictness() ) {
                use_tspace = tspace;
             }   
             if ( tavgStrictness() ) {
                use_tavg = tave;
             }   
             status = adir.LookUp( test_quant, use_hgrid, use_vgrid, use_tspace, use_tavg
               , NULL, NULL, &test_ndims
               , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl );
          }
          if ( status == 0 ) {
             // got assimilation specs.
             
             if ( debug >= 3 ) {
                std::cerr << "MetGEOSfp::prep:  Examining Assim URL <<"  << *newUrl << ">>" << std::endl;
             }
             
             /* find when the assimilation leaves off.
                Note: this involves opening a connection ot the server!
             */   
             final_assim_day = get_last_time1900( *newUrl );
             
             
             if ( debug >= 3 ) {
                std::cerr << "MetGEOSfp::prep:  final time in that URL is "  << final_assim_day << std::endl;
             }
             
             // if the desired time is below this, then we are done
             if ( yday <= final_assim_day ) {
                whichSrc = -1;
             } else {
                // set the status to failed
                status = 1;
             }       
          }
             
       }
       
       if ( status != 0 && forceSrc >= 0 ) {
          // Either there is no assimilation for this variable, or
          // we did not try for an assimilation.
          
          // Either way, we need to check for forecasts
       
          if ( modelRun == "" ) {
             // no model run has been specified.
             // So we need to figure out which model file to open.
       
             if ( final_assim_day == 0.0 ) {
                // We either could not or would not get a final time from the assimilation file.
                
                // Construct a URL for the ".latest" forecast file (since modelRun is "")

                if ( debug >= 3 ) {
                   std::cerr << "MetGEOSfp::prep: Trying to get URL for '.latest' forecast " << std::endl;
                }
                status = fdir.LookUp( test_quant, hgrid, pgrid, tspace, tave
                     , NULL, NULL, &test_ndims
                     , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl, modelRun );
                if ( status && (strict != 0x07) ) {
                   if ( horizStrictness() ) {
                      use_hgrid = hgrid;
                   }   
                   if ( vertStrictness() ) {
                      use_vgrid = pgrid;
                   }   
                   if ( tspaceStrictness() ) {
                      use_tspace = tspace;
                   }   
                   if ( tavgStrictness() ) {
                      use_tavg = tave;
                   }   
                   status = fdir.LookUp( test_quant, use_hgrid, use_vgrid, use_tspace, use_tavg
                     , NULL, NULL, &test_ndims
                     , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl, modelRun );
                }
                
                if ( status == 0 ) {
                   // Now use this ".latest" URL to open forecast file and get the
                   // first (initialization) time from that.
                   if ( debug >= 3 ) {
                      std::cerr << "MetGEOSfp::prep:  Examining Forecast URL <<"  << *newUrl << ">>" << std::endl;
                   }
                   final_assim_day = get_first_time1900( *newUrl );      
                   if ( debug >= 3 ) {
                      std::cerr << "MetGEOSfp::prep:  final time in that URL is "  << final_assim_day << std::endl;
                   }
                }
          
             } 
             
             // truncate to get 00Z on the last day for which there are real data
             final_assim_day = floor( final_assim_day );
             
             // We try these offsets to that day, looking for an appropriate forecast model run file
             for ( int dayOffset=0; dayOffset > -2; dayOffset-- ) {
             
                 bday = final_assim_day + dayOffset  - basetime; // internal model time, same as xday
                 
                 base_day = time2Cal(bday); // again, internal model time
                 // transform YYYY-MM-DD to YYYYMMDD_
                 base_run = base_day.substr(0,4) + base_day.substr(5,2) + base_day.substr(8,2) + "_";
                 
                 // how many hours ahead of the assimilation 00Z time is the desired time?
                 leadtime = (xday - bday)*24.0;
                 
                 // Use this to guess what forecast model run is wanted
                 // But remember that the forecast file might not actually exist yet,
                 // and that is why we are looping with dayOffset--to catch the preivous
                 // day's forecasts if today's are not ready.
                 if ( leadtime <= 6.0 ) {
                    tmp_modelrun = base_run + "06";
                 } else if ( leadtime <= 12.0 ) {
                    tmp_modelrun = base_run + "12";
                 } else if ( leadtime <= 18.0 ) {
                    tmp_modelrun = base_run + "18";
                 } else {
                    tmp_modelrun = base_run + "00";
                 }
                 
                 // tmp_modelrun = tmp_modelrun + ".test.nonexistent";
                 
                 if ( debug >= 3 ) {
                    std::cerr << "MetGEOSfp::prep:  Testing leadtime " << leadtime << " yielding model run " << tmp_modelrun << std::endl;
                 }
                 
                 // construct the URL for the forecast file
                 status = fdir.LookUp( test_quant, hgrid, pgrid, tspace, tave
                      , NULL, NULL, &test_ndims
                      , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl, tmp_modelrun );
                 if ( status && (strict != 0x07) ) {
                    if ( horizStrictness() ) {
                       use_hgrid = hgrid;
                    }   
                    if ( vertStrictness() ) {
                       use_vgrid = pgrid;
                    }   
                    if ( tspaceStrictness() ) {
                       use_tspace = tspace;
                    }   
                    if ( tavgStrictness() ) {
                       use_tavg = tave;
                    }   
                    status = fdir.LookUp( test_quant, use_hgrid, use_vgrid, use_tspace, use_tavg
                      , NULL, NULL, &test_ndims
                      , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl, tmp_modelrun );
                 }
                 
                 if ( status == 0 ) {
                    // got a URL.
                    
                    // now check that the forecast run is actually out there, but
                    // trying to open the file and read its times.
                    
                    // The get_last_time() call above was done on the assimilation file, which should always be there.
                    // The get_first_time() call above was done on the ".latest" forecast file, which should always be there.
                    // But this forecast file might not be there, so we need to trap the error it might throw.
                    try {
                        
                        if ( debug >= 3 ) {
                           std::cerr << "MetGEOSfp::prep:  Examining forecast URL <<"  << *newUrl << ">>" << std::endl;
                        }
                        junktime = get_first_time1900( *newUrl );
                        
                        if ( junktime > 0.0 ) {
                           // leave the dayOffset loop--we have our URL
                           if ( debug >= 3 ) {
                              std::cerr << "MetGEOSfp::prep:  Success!" << std::endl;
                           }
                           dayOffset = -999;
                           whichSrc = 1;
                        }
                    } catch (badNetcdfOpen) {
                       // fall through to the next loop iteration
                    } catch (badNetcdfError) {
                       // fall through to the next loop iteration
                    }
                 }
                 
                 
             }             
          } else {
             // a model run was specified, so use that             

            status = fdir.LookUp( test_quant, hgrid, pgrid, tspace, tave
                 , NULL, NULL, &test_ndims
                 , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl, modelRun );
            if ( status && (strict != 0x07) ) {
               if ( horizStrictness() ) {
                  use_hgrid = hgrid;
               }   
               if ( vertStrictness() ) {
                  use_vgrid = pgrid;
               }   
               if ( tspaceStrictness() ) {
                  use_tspace = tspace;
               }   
               if ( tavgStrictness() ) {
                  use_tavg = tave;
               }   
               status = fdir.LookUp( test_quant, use_hgrid, use_vgrid, use_tspace, use_tavg
                 , NULL, NULL, &test_ndims
                 , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl, modelRun );
            }
            whichSrc = 1;
          }
          
          // Note that we do not need to check for the existence of the URL here.
          // That wil come later, when we try to open it.
          // The user is presumed to have known what he or she was doing by specifying a modelRun.
          
       }  

       if ( status == 0 ) {
          
          test_url = *newUrl;
          if ( debug >= 3 ) {
             std::cerr << "MetGEOSfp::prep:  testing was successful.  ndims is " << test_ndims << ", url=<<" << test_url << ">>" << std::endl;
          }
          
          delete newUrl;
       
       } else {
          whichSrc = 0;
          
          throw(badDataNotFound());
       }
    }
      
    // std::cerr << " prep returns " << test_ndims << std::endl;
    
    
    return test_ndims;

}    





std::vector<string> *MetGEOSfp::new_testQuantity( string quantity )
{
     std::vector<string> *result;
     int status;
     
     result = new std::vector<string>;
     
     if ( quantity == pottemp_name || quantity == "air_potential_temperature" ) {
        // for theta, we test for T
        result->push_back("t");
     } else if (quantity == "ThetaDot" || quantity == "net_heating_rate" ) {
        // for theta-dot (net heating rate), we test for T and DTDTTOT
        result->push_back("t");
        result->push_back("dtdttot");
     } else if (quantity == "w" ) {
        result->push_back("omega");
     } else if (quantity == pressure_name || quantity == "air_pressure" ) {
        // for pressure, we test for T
        result->push_back("t");
     } else if ( quantity == palt_name || quantity == "pressure_altitude" ) {
        // same as for pressure, above
        result->push_back("t");
     } else if (quantity == altitude_name ) {
        result->push_back("t");
     } else {
        // all else, we test for the quantity itself
        result->push_back(quantity);
     }
     
     //std::cerr << "tested quantity " << quantity << " and got " << result << std::endl;
     
     return result;
}

void MetGEOSfp::set_vertical( const std::string quantity, const std::string units )
{
     set_vertical( quantity, units, NULLPTR );
}

void MetGEOSfp::set_vertical( const std::string quantity, const std::string units, const std::vector<real>* levels )
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
    
     if ( my_quant == altitude_name || my_quant == "altitude" ) {
        my_units = "km";
        my_wind_vert_name = "w";
        wfctr = 0.001; // w gets scaled from m/s to km/s
     } else if ( my_quant == pressure_name || my_quant == "air_pressure" ) {
        my_units = "hPa";
        my_wind_vert_name = "omega";;
        wfctr = 0.01; // omega get scaled from Pa/s to hPa/s
     } else if ( my_quant == pottemp_name || my_quant == "air_potential_temperature" ) {
        my_units = "K";
        my_wind_vert_name = "ThetaDot";
        wfctr = 1.0;
     } else if ( my_quant == palt_name || my_quant == "pressure_altitude" ) {
        my_units = "km";
        my_wind_vert_name = "w";;
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
 

GridLatLonField3D* MetGEOSfp::new_directGrid3D( const std::string quantity, const std::string time )
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
    MetGEOSfp *defaultThis;
    time_t zeep;

    //std::cerr << "Entering new_directGrid3D" << std::endl;

    if ( quantity == "nodataload" ) {
       throw (MetGridData::baddataload());
    }   

    // std::cerr << "*** TIME = " << time << std::endl;
    
    //std::cerr << "reading " << quantity << " @ " << time << std::endl;

    // translate the time    
    mtime = cal2Time( time );

    (void) prep( quantity, mtime );


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
       component_1 = defaultThis->Get3D("t", time);
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

       (void) this->prep("u", time);
       if ( test_vgrid == 2 ) {
          // data are on pressure levels; just grab the vert coord
          component_1 = defaultThis->Get3D("u", time);
          tmp1 = dynamic_cast<GridLatLonField3D*>(getpress.calc( *component_1 ));
          *grid3d = *tmp1;
          delete tmp1;
          defaultThis->remove( component_1 );
       } else if ( test_vgrid == 1 ) {
          // data are on model levels--read the layer thicknesses
          // and calculate pressure from them
          component_1 = defaultThis->Get3D("delp", time);
          // the "1.0" is the pressure (in Pa) imposed at the top GMAO model level
          tmp1 = dynamic_cast<GridLatLonField3D*>(getpress.calc( *component_1, 1.0 ));
          *grid3d = *tmp1;
          delete tmp1;
          defaultThis->remove( component_1 );
          // Note: pressure data are in Pa; convert to hPa (mb)
          grid3d->transform("hPa", 0.01);
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
       component_1 = defaultThis->Get3D("dtdttot", time);
       component_2 = defaultThis->Get3D(pottemp_name, time);
       if ( ! component_2->compatible( *component_1 ) ) {
          // make sure the two fields are on the same spatial grid.
          // DTDTTOT is usually on a coarser grid.
          component_3 = reconcile( *component_1, *component_2 );
          // replace the bad-grid dtttot with the new-grid version
          defaultThis->remove( component_1 );
          component_1 = component_3;
       }
       tmp1 = dynamic_cast<GridLatLonField3D*>(getthetadot.calc( *component_1, *component_2 ));
       *grid3d = *tmp1;
       delete tmp1;
       // put code to do the mass balance adjustment here
       defaultThis->remove( component_2 ); 
       defaultThis->remove( component_1 );
       delete defaultThis;
    
    } else if (  quantity == "w" || quantity == "upward_air_velocity" ) {

       defaultThis = myNew();
       // set in the above call: defaultThis->diskcaching = this->diskcaching;
       component_1 = defaultThis->Get3D("omega", time);
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

GridLatLonFieldSfc* MetGEOSfp::new_directGridSfc( const std::string quantity, const std::string time )
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

void MetGEOSfp::delay()
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

