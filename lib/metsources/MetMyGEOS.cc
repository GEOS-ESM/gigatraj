
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

#include "gigatraj/MetMyGEOS.hh"

using namespace gigatraj;

const std::string MetMyGEOS::iam = "MetMyGEOS";



// default constructor
MetMyGEOS::MetMyGEOS() : MetGridLatLonData() 
{    

    ready = false;
    is_open = false;
    fillval = 1.0e15;
    
    reset();   

}

MetMyGEOS::MetMyGEOS( std::string& date ) : MetGridLatLonData() 
{

    ready = false;
    is_open = false;
    
    reset();
   
    basetime = cal.day1900( date );
    
    catTimeOffset = 0.0;
    
}



// destructor
MetMyGEOS::~MetMyGEOS() 
{
    
}

// copy constructor
MetMyGEOS::MetMyGEOS( const MetMyGEOS&  src) : MetGridLatLonData(src)
{

     mettag = src.mettag;
     modelrun = src.modelrun;
     basetime = src.basetime;
     catTimeOffset = src.catTimeOffset;

     altitude_name = src.altitude_name;
     pressure_name = src.pressure_name;
     palt_name = src.palt_name;
     pottemp_name = src.pottemp_name;

     verticalWinds = src.verticalWinds;
     legalDims = src.legalDims;
     otfIDs = src.otfIDs;
     
     skip = src.skip;
     skoff = src.skoff;
     waittry = src.waittry;
     openwait = src.openwait;
     ntries = src.ntries;
     time_zero = src.time_zero;
}    

void MetMyGEOS::assign(const MetMyGEOS& src)
{
     reset();
     
     MetGridLatLonData::assign(src);

     mettag = src.mettag;
     modelrun = src.modelrun;
     basetime = src.basetime;
     catTimeOffset = src.catTimeOffset;

     altitude_name = src.altitude_name;
     pressure_name = src.pressure_name;
     palt_name = src.palt_name;
     pottemp_name = src.pottemp_name;

     verticalWinds = src.verticalWinds;
     legalDims = src.legalDims;
     otfIDs = src.otfIDs;
     
     skip = src.skip;
     skoff = src.skoff;
     waittry = src.waittry;
     openwait = src.openwait;
     ntries = src.ntries;
     time_zero = src.time_zero;
}    

/// assignment operator
MetMyGEOS& MetMyGEOS::operator=(const MetMyGEOS& src)
{
    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }
    
    assign(src);

    return *this;
}

MetGridLatLonData& MetMyGEOS::operator=(const MetGridLatLonData& src)
{
    const MetMyGEOS* ssrc;
    
    if ( src.id() == this->id() ) {
    
       ssrc = dynamic_cast<const MetMyGEOS*>(&src);
       // handle assignment to self
       if ( this == ssrc ) {
          return *this;
       }
    
       assign(*ssrc);

       return *this;

    } else {
       throw (badIncompatibleMetDataClass());
    }
}

std::string MetMyGEOS::time2Cal( double time, int format )
{
    return cal.date1900( basetime + time, format );
}


double MetMyGEOS::cal2Time( std::string date )
{
    return (cal.day1900( date ) - basetime);
}

void MetMyGEOS::defineCal( std::string caldate, double time )
{
     double oldbase;
     
     oldbase = basetime;
     
     // convert cal to a time and set basetime to the offset between that and time
     basetime = ( cal.day1900( caldate ) - time );

}

void MetMyGEOS::setOption( const std::string &name, const std::string &value )
{
     if ( name == "DataSetID" ) {
        metTag( value );
     } else if ( name == "ModelRun" ) {
        modelRun( value );
     }
}

void MetMyGEOS::setOption( const std::string &name, int value )
{
    int tmp;

    if ( name == "HorizontalGridThinning" ) {
        set_thinning(value);
    } else if ( name == "HorizontalGridOffset" ) {
        tmp = thinning();
        set_thinning( tmp, value );
/*
    } else if ( name == "Delay" ) {
        setWaitOpen(value);
*/
    }
    
}

void MetMyGEOS::setOption( const std::string &name, float value )
{
}

void MetMyGEOS::setOption( const std::string &name, double value )
{
}

bool MetMyGEOS::getOption( const std::string &name, std::string &value )
{
   bool result;
   
   value = "";
   result = false;
   
   if ( name == "DataSetID" ) {
      value = metTag();
      result = true;
   } else if ( name == "ModelRun" ) {
      value = modelrun;
      result = true;
   }
   return result;
}


bool MetMyGEOS::getOption( const std::string &name, int &value )
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


bool MetMyGEOS::getOption( const std::string &name, float &value )
{
    bool result = false;
    value = 0.0;
    return result;
}


bool MetMyGEOS::getOption( const std::string &name, double &value )
{
    bool result = false;
    value = 0.0;
    return result;
}


MetData *MetMyGEOS::genericCopy()
{
    MetMyGEOS *result;
    
    result = new MetMyGEOS;
    result->assign(*this);
    
    return dynamic_cast<MetData*>( result );
}



void MetMyGEOS::set_vertical( const std::string quantity, const std::string units )
{
     set_vertical( quantity, units, NULLPTR );
}

void MetMyGEOS::set_vertical( const std::string quantity, const std::string units, const std::vector<real>* levels )
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
    
     // note: GEOS does not (usually) define variable names for
     //   altitude, air_pressure, Theta, upward_air_velocity,
     //   or net heating rate.

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
        my_wind_vert_name = "PAltDot";;
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
 
void MetMyGEOS::impose_times( double otbase, double otspace )
{
       MetGridData::impose_times( otbase, otspace );
       
       catlog.timeSpacing( otspace, otbase );

}


bool MetMyGEOS::legalQuantity( const std::string quantity )
{
     bool result;
     std::string dayt;

     if ( quantity == pressure_name
       || quantity == pottemp_name
       || quantity == palt_name
       || quantity == altitude_name 
       || quantity == "Model-Levels" ) {
       
       // these quantities are legal by definition,
       // since they are built-in 
       result = true;

     } else {
        // has to be looked up
        result = dsInit( quantity );
     }
              
     return result;
}

std::string MetMyGEOS::units( const std::string quantity )
{
    std::string result;
    std::string dayt;
    bool ok;
    
    result = "UNKNOWN";
    
    if ( dsInit( quantity ) ) {
       // extract the units, if present, from that DataSource
       result = ds[test_dsrc].units;
    }   
    
    return result;
}


void MetMyGEOS::delay()
{
    float wayt;
    int w;
    int c1;

    // we have to be multiprocessing, and the current file 
    // (opened or about to be opened) has to be a URL
    if ( my_pgroup->id() >= 0 && is_url ) {
        
       c1 = my_pgroup->numberOfProcessors() ;
       
       // assume a max of three concurrent connections to an opendap server
       // allow about 6 seconds for each
       if ( c1 > 3 ) {
          wayt =   ( c1/3.0*6.0 );
       
          w = ( my_pgroup->random()*wayt + 0.5);
          
          if ( dbug > 5 ) {
             std::cerr << "MetMyGEOS::delay: sleeping for " << w 
                       << " sec from a max of " << wayt 
                       << " w/ " << c1 <<  " processors"
                       << std::endl;
          }
          (void) sleep( w );          
       }
    }

}

void MetMyGEOS::debug( int level ) 
{
     MetData::debug( level );
     catlog.debug( level );
}

MetGridData* MetMyGEOS::MetGridCopy()
{
     MetMyGEOS* newthing;
     
     newthing = new MetMyGEOS();
     newthing->assign( *this );    
     
     return dynamic_cast<MetGridData*> (newthing);
}
       

GridLatLonField3D* MetMyGEOS::new_directGrid3D( const std::string quantity, const std::string time )
{
    double mtime;

    int i,j,k;
    real lon, lat, z;
    GridLatLonField3D* grid3d;
    GridLatLonField3D* tmp1;
    GridLatLonField3D* component_1;
    GridLatLonField3D* component_2;
    GridLatLonField3D* component_3;
    real val;
    std::vector<real> data;
    MetMyGEOS *defaultThis;
    time_t zeep;
    std::string vcode;

    if ( dbug > 20 ) {
       std::cerr << "MetMyGEOS::new_directGrid3D: Entering quantity = " << quantity << " @ " << time << std::endl;
    }

    if ( quantity == "nodataload" ) {
       throw (MetGridData::baddataload());
    }   
    
    // translate the time to internal model time
    mtime = cal2Time( time );


    // create a new 3D gridded field to hold the data we are about to read
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
       component_1 = defaultThis->new_directGrid3D("T", time);
       component_2 = defaultThis->new_directGrid3D(pressure_name, time);
       tmp1 = dynamic_cast<GridLatLonField3D*>(gettheta.calc( *component_1, *component_2 ));
       *grid3d = *tmp1;
       delete tmp1;
       defaultThis->remove( component_2 ); 
       defaultThis->remove( component_1 );
       delete defaultThis;
    } else if ( quantity == pressure_name || quantity == "air_pressure" ) {
       defaultThis = myNew();
       // set in the above call: defaultThis->diskcaching = this->diskcaching;

       (void) this->setup(wind_ew_name, time);
       vcode = queryVertical();
       if ( vcode[0] == 'P' ) {
          // data are on pressure levels; just grab the vert coord
          component_1 = defaultThis->new_directGrid3D(wind_ew_name, time);
          tmp1 = dynamic_cast<GridLatLonField3D*>(getpress.calc( *component_1 ));
          *grid3d = *tmp1;
          delete tmp1;
          defaultThis->remove( component_1 );
       } else if ( vcode[0] == 'L' ) {
          // data are on model levels--read the mid-layer pressures PL
          // (NOT the DELP layer thicknesses!)
          // and calculate pressure from them
          if ( quantity != "PL" ) { 
             component_1 = defaultThis->new_directGrid3D("PL", time);
             *grid3d = *component_1;
             grid3d->set_quantity(quantity);
             defaultThis->remove( component_1 );
          } else {
             readSource( quantity, time, grid3d );   
          }   
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
       component_1 = defaultThis->new_directGrid3D("DTDTTOT", time);
       component_2 = defaultThis->new_directGrid3D(pottemp_name, time);
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
    
    } else if ( quantity == "W" || quantity == "upward_air_velocity" ) {

       defaultThis = myNew();
       // set in the above call: defaultThis->diskcaching = this->diskcaching;
       component_1 = defaultThis->new_directGrid3D("OMEGA", time);
       component_2 = defaultThis->new_directGrid3D(pressure_name, time);
       tmp1 = dynamic_cast<GridLatLonField3D*>(getpaltdot.calc( *component_1, *component_2 ));
       *grid3d = *tmp1;
       delete tmp1;
       defaultThis->remove( component_2 );
       defaultThis->remove( component_1 );
       delete defaultThis;    
    
    } else if ( quantity == palt_name || quantity == "pressure_altitude" ) {

       defaultThis = myNew();
       component_1 = defaultThis->new_directGrid3D(pressure_name, time);
       tmp1 = dynamic_cast<GridLatLonField3D*>(getpalt.calc( *component_1 ));
       *grid3d = *tmp1;

       defaultThis->remove( component_1 );
       delete defaultThis;    
    
    } else if ( quantity == "Model-Levels" ) {
    
          // model data is not to be found on any data surfaces
          //throw (badDataNotFound());
    
          delete grid3d;
          grid3d = NULLPTR;
    
    } else {

       readSource( quantity, time, grid3d );

    }
    
    if ( grid3d != NULLPTR ) {
       grid3d->setPgroup( my_pgroup, my_metproc );
    
       // set the expiration time
       zeep = expiration( time );
       grid3d->set_expires( zeep );
    }
    
    if ( dbug > 20 ) {
       std::cerr << "MetMyGEOS::new_directGrid3D: leaving  quantity=" << quantity << " @ " << time << std::endl;
    }
    
    return grid3d;
    

}

GridLatLonFieldSfc* MetMyGEOS::new_directGridSfc( const std::string quantity, const std::string time )
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
       desiredsfc->setPgroup( my_pgroup, my_metproc );

//       *desiredsfc = dynamic_cast<GridLatLonFieldSfc&>(tropgen.wmo( *grid3D ));
// FIX THIS!

       // get the desired quantity on desired vertical coord
       desired3D = new_directGrid3D(quantname, time); 
       desired3D->setPgroup( my_pgroup, my_metproc );
       // interpolate the desired quantity onto the desired surface
       delete gridsfc;
       gridsfc = dynamic_cast<GridLatLonFieldSfc*>(vin->surface( *desired3D, *desiredsfc ));
       remove( desired3D );
       delete desiredsfc;
       remove( grid3D );
    } else if ( sfcname == "sfc" ) { 
       //std::cerr << "About to read " << quantity << " from sfc" << std::endl;
       readSource( quantity, time, gridsfc );
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


std::vector<real>* MetMyGEOS::vcoords( const std::string *coordSys ) const
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
       result->push_back(    0.2 );     
       result->push_back(    0.1 );     
       result->push_back(    0.07 );     
       result->push_back(    0.05 );     
       result->push_back(    0.04 );     
       result->push_back(    0.03 );     
       result->push_back(    0.02 );
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
    
    } else if ( *vc == "Model-Levels" ) {
       for ( int i=0; i<72; i++ ) {
          result->push_back( i );  
       }
    } else if ( *vc == "Model-Edges" ) {
       for ( int i=0; i<73; i++ ) {
          result->push_back( i );  
       }
    } else {
   
       throw(badVerticalCoord());
    
    }

    return result;

}


FilePath* MetMyGEOS::cachefile( const GridFieldSfc* item ) const
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
       
       fname = mettag + "_"
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


FilePath* MetMyGEOS::cachefile( const GridField3D* item ) const
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
       
       
       
       fname = mettag + "_"
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



bool MetMyGEOS::vConvert( GridField3D *input, std::string quant, std::string units, real scale, real offset )
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


bool MetMyGEOS::bracket( const std::string &quantity, const std::string &time, std::string *t1, std::string *t2)
{
    std::vector<std::string> *testquants;
    bool sametime;
    bool ok;
    int idx;
    double tcal;
    double tcat;
    
    if ( dbug > 5 ) {
       std::cerr << "MetMyGEOS::bracket: Bracketing time " << time << " against base " << basetime << std::endl;
    }
    
    sametime = false;
    
    // note: for on-the-fly calculations, we need
    // to get the bracket of the quantities that the OTF calculaiton
    // depends on. Here, we will just be using the 0th such quantity.
    // But if the component quantities are on different time ticks
    // this may not be sufficient.
    testquants = testQuantity( quantity );
    
    ok = catlog.query( (*testquants)[0], time, ds, modelrun );
    if ( ok ) {
        // find the internal model time from the calendar
        tcal = cal2Time( time );
        // the the day1900 time used by the catalog
        tcat = catlog.date2number( time );
        
        // now get the timebase from the difference btween the two
        catTimeOffset = tcal - tcat;
        
        idx = 0;
        
        *t1 = ds[idx].t0;
        *t2 = ds[idx].t1;
    
        sametime = ( *t1 == time );
    }


    delete testquants;
    
    if ( dbug > 5 ) {
       std::cerr << "MetMyGEOS::bracket:   Translated bracket times to  " << *t1 << " and " << *t2  << std::endl;
    }
 
    return sametime;
}

bool MetMyGEOS::bracket( const std::string &quantity, double time, double *t1, double *t2)
{
     std::string wanted, prev, next;
     bool result;
     
     wanted = time2Cal( time, 3 );
     result = bracket( quantity, wanted, &prev, &next );
     
     *t1 = cal2Time( prev );
     *t2 = cal2Time( next );
     
     return result;
}


int MetMyGEOS::setup(  const std::string quantity, const double time )
{
    std::string caltime;

    init();

    // convert model time to calendar time
    caltime = time2Cal(time);

    if ( quantity == pottemp_name || quantity == "air_potential_temperature" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return prep( "T", caltime );

    } else if ( quantity == verticalWinds[pottemp_name].name || quantity == "net_heating_rate" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return prep( "DTDTTOT", caltime ) & prep( "T", caltime ) ;

    } else if ( quantity == verticalWinds[altitude_name].name || quantity == "upward_air_velocity" ) {
       
       return prep( "OMEGA", caltime );

    } else if (quantity == pressure_name || quantity == "air_pressure" ) {

       return prep( wind_ew_name, caltime );
       
    } else if ( quantity == palt_name || quantity == "pressure_altitude" ) {
       
       // same as for pressure, above
       return prep( wind_ew_name, caltime );

    } else if (quantity == altitude_name ) {

       return prep( wind_ew_name, caltime );
       
    } else {
       // just ask for the quantity directly
       return prep( quantity, caltime ); 
    }


}    

int MetMyGEOS::setup(  const std::string quantity, const std::string &time )
{
    std::string caltime;

    init();

    // convert model time to calendar time
    caltime = time;    

    if ( quantity == pottemp_name || quantity == "air_potential_temperature" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return prep( "T", caltime );

    } else if ( quantity == verticalWinds[pottemp_name].name || quantity == "net_heating_rate" ) {
       // we want theta, but we will be calculating theta 
       // on the fly from temperatures.  So the quantity that
       // we really need to ask for is temperature and pressure
       
       return prep( "DTDTTOT", caltime ) & prep( "T", caltime ) ;

    } else if ( quantity == verticalWinds[altitude_name].name || quantity == "upward_air_velocity" ) {
       
       return prep( "OMEGA", caltime );

    } else if (quantity == pressure_name || quantity == "air_pressure" ) {

       return prep( wind_ew_name, caltime );
       
    } else if ( quantity == palt_name || quantity == "pressure_altitude" ) {
       
       // same as for pressure, above
       return prep( wind_ew_name, caltime );

    } else if (quantity == altitude_name ) {

       return prep( wind_ew_name, caltime );
       
    } else {
       // just ask for the quantity directly
       return prep( quantity, caltime ); 
    }

}

int MetMyGEOS::prep(  const std::string quantity, const std::string& time )
{
    std::string caltime;
    int status;
    bool ok;
    std::string *newUrl;
    double tbase, tinc;
    std::string fileTime;
    std:string tbasestr;
    int nt;
    
    init();

    // convert model time to calendar time
    //std::cerr << " in setup w/ quant " << quantity << " at time " << time << std::endl;
    
    // do we need to find the basic attributes of this quantity?
    if ( dsInit( quantity, time ) ) {
          // get the file start time (string form)
          fileTime = ds[test_dsrc].t0;
          // get the file start time (catalog numeric form)
          tbase = ds[test_dsrc].preStart_t; 
          // Note: this is a (pretty good) guess at what the zero-base time in the file
          // might be. It may well change when we read the metadata from the file
          time_zero = cal2Time( fileTime ) ;
          // For now, though, this is good enough to give us the offset between
          // model time and catalog file.
          catTimeOffset = time_zero - tbase;
    } else {
          throw(badDataNotFound());
    }
          
    // std::cerr << " setup returns " << ds[test_dsrc].dims << std::endl;
    
    return ds[test_dsrc].dims;

}    

/////////////////////////////////////////////

void MetMyGEOS::metTag( const std::string mytag )
{
     mettag = mytag;
     if ( ready ) {
        catlog.clear();
     }
     
     catlog.confLocator( mettag );
     
     ready = false;
}

std::string MetMyGEOS::metTag() const
{
    return mettag;
}

void MetMyGEOS::modelRun( const std::string fcst )
{
     modelrun = fcst;
}

std::string MetMyGEOS::modelRun() const
{
    return modelrun;
}


int MetMyGEOS::thinning( int *offset )
{
    if ( offset != NULL ) {
       *offset = skoff;
    }
    
    return skip;
}

void MetMyGEOS::set_thinning( int thin, int offset )
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
    
}

real MetMyGEOS::defaultBad() const
{
    return fillval;
}

void MetMyGEOS::defaultBad( real defBad )
{
    fillval = defBad;
}

void MetMyGEOS::set_BaseTime( std::string& date ) 
{
    basetime = cal.day1900(date);

}        

std::string MetMyGEOS::BaseTime( ) const
{
    std::string result;
    
    result = cal.date1900( basetime );
    
    return result;
}           


GridLatLonField3D* MetMyGEOS::Get3D( const std::string quantity, const std::string time )
{
    return new_directGrid3D( quantity, time );
}         

GridLatLonFieldSfc* MetMyGEOS::GetSfc( const std::string quantity, const std::string time )
{
    return new_directGridSfc( quantity, time );
}         


/////////////////////////////////////////////

void MetMyGEOS::init()
{
    std::string cfgFile;
    std::string qname;
    
    if ( ! ready ) {
       
       catlog.confLocator( mettag );
       
       cfgFile = catlog.findConfig();
       
       if ( cfgFile != "" ) {
          catlog.load( cfgFile );
          
          if ( catlog.variableValue( "pressure_name", qname ) ) {
             verticalWinds[qname] = verticalWinds[pressure_name];
             pressure_name = qname;
          }
          if ( catlog.variableValue( "pottemp_name", qname ) ) {
             verticalWinds[qname] = verticalWinds[pottemp_name];
             pottemp_name = qname;
          }
          if ( catlog.variableValue( "palt_name", qname ) ) {
             verticalWinds[qname] = verticalWinds[palt_name];
             palt_name = qname;
          }
          if ( catlog.variableValue( "alt_name", qname ) ) {
             verticalWinds[qname] = verticalWinds[altitude_name];
             altitude_name = qname;
          }
          if ( catlog.variableValue( "pressure_w_name", qname ) ) {
             verticalWinds[pressure_name].name = qname;
          }
          if ( catlog.variableValue( "pottemp_w_name", qname ) ) {
             verticalWinds[pottemp_name].name = qname;
          }
          if ( catlog.variableValue( "palt_w_name", qname ) ) {
             verticalWinds[palt_name].name = qname;
          }
          if ( catlog.variableValue( "alt_w_name", qname ) ) {
             verticalWinds[altitude_name].name = qname;
          }
          if ( catlog.variableValue( "wind_ew_name", qname ) ) {
             wind_ew_name = qname;
          }
          if ( catlog.variableValue( "wind_ns_name", qname ) ) {
             wind_ns_name = qname;
          }
 
          
          catlog.timeSpacing( override_tspace, override_tbase );
          ready = true;
       } else {
          std::cerr << "No config file found for '" << mettag << "'" << std::endl;
          throw (badConfig());
       }

       if ( vquant == "none" ) {
          set_vertical( pressure_name, "hPa" );
       }
    }

}


void MetMyGEOS::setup_vars()
{
    // vertical wind
    Catalog::DataSource vw;
    
    // these are legal quantities for input coordinates
    legalDims.push_back("lon");
    legalDims.push_back("lat");
    legalDims.push_back("lev");
    legalDims.push_back("time");

    // set names of possible vertical coorindate quantities for this data set
    altitude_name = "Z";
    pressure_name = "P";
    palt_name = "PAlt";
    pottemp_name = "Theta";
    
    // setup vertical wind descriptions
    vw.name = "W";
    vw.units = "m/s";
    vw.scale = 1.0;
    vw.offset = 0.0;
    verticalWinds[ altitude_name ] = vw;
    
    vw.name = "OMEGA";
    vw.units = "Pa/s";
    vw.scale = 1.0;
    vw.offset = 0.0;
    verticalWinds[ pressure_name ] = vw;
    
    vw.name = "ThetaDot";
    vw.units = "K/s";
    vw.scale = 1.0;
    vw.offset = 0.0;
    verticalWinds[ pottemp_name ] = vw;

    vw.name = "PAltDot";
    vw.units = "m/s";
    vw.scale = 1.0;
    vw.offset = 0.0;
    verticalWinds[ pottemp_name ] = vw;
        
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
    getpaltdot.setPressureAltitudeName(palt_name);
    getpaltdot.setPressureName(pressure_name);
    getpaltdot.setPressureDotName((vertwind_quants[ pressure_name ]).quantity);
    
    
}

void MetMyGEOS::reset()
{

     // close any open files
     if ( is_open ) {
     
     }
     current_file = "";
     is_url = false;
     
     // clear the catalog
     if ( ready ) {
        catlog.clear();
     }
          
     // re-initialize variables
     mettag = "MetMyGEOS";
     modelrun = "";
     basetime = 0;
     catTimeOffset = 0;
     skip = 0;
     skoff = 0;
     waittry = 1;
     openwait = 0;
     ntries = 1;
     time_zero = 0.0;
     
     wind_ew_name = "U";
     wind_ns_name = "V";
     verticalWinds.clear();
     legalDims.clear();
     otfIDs.clear();
     ds.clear();
     test_dsrc = -1;
     opened_dsrc = -1;
     
     setup_vars();

}


time_t MetMyGEOS::expiration( const std::string &valid_at )
{     
     return 0;
}



MetMyGEOS* MetMyGEOS::myNew()
{
   MetMyGEOS *dup;
   
   dup = new MetMyGEOS();
   
   dup->dbug = dbug;
   dup->setPgroup(my_pgroup, my_metproc);
   dup->defineCal( time2Cal(0), 0.0 );
   dup->maxsnaps = this->maxsnaps;
   dup->setCacheDir( this->diskcachedir );

   dup->mettag = this->mettag;
   dup->modelrun = this->modelrun;
   dup->catTimeOffset = this->catTimeOffset;
   dup->basetime = this->basetime;

   dup->altitude_name = this->altitude_name;
   dup->pressure_name = this->pressure_name;
   dup->palt_name = this->palt_name;
   dup->pottemp_name = this->pottemp_name;

   dup->verticalWinds = this->verticalWinds;
   dup->legalDims = this->legalDims;
   dup->otfIDs = this->otfIDs;

   dup->skip = this->skip;
   dup->skoff = this->skoff;
   dup->waittry = this->waittry;
   dup->openwait = this->openwait;
   dup->ntries = this->ntries;
   dup->time_zero = this->time_zero;

   
   return dup;
}

std::vector<std::string> *MetMyGEOS::testQuantity( const std::string& quantity )
{
     std::vector<std::string> *result;
     int status;
     
     result = new std::vector<std::string>;
     
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


bool  MetMyGEOS::dsInit( const std::string& quantity, const std::string& time )
{
     bool result;
     bool doit;
     std::string quant;
     std::string dayt;
     
     
     result = false;
     
     // load the catalog, if necessary
     init();
     
     // use this as the default quantity
     quant = wind_ew_name;
     if ( quantity != "" ) {
        quant = quantity;
     }
     // use this as a test date, if nothing else is available
     dayt = "2010-01-01T00:00:00";
     // but we'd prefer a user-supplied test date
     if ( time != "" ) {
        dayt = time;
     }
     
     // do we have no previous query results?
     doit = (test_dsrc < 0 || ds.size() == 0 );
     if ( ! doit ) {
        // we do have previous query results,
        // but not necessarily for this query
        
        //  are these results sufficient for this query?
        if ( ds[test_dsrc].name != quant ) { 
            if ( quantity == "" ) {
               // re-use the old quantity
               quant = ds[test_dsrc].name;
            } else {
               doit = true;
            }
        } 
     } 
     if ( ! doit ) {
        // the quantites are the same, or the given quantity
        // is the default, meaning that we will use the old
        // quantity.     
        if ( time == "" ) {
           // re-use the old time if the user doesn't care
           dayt = ds[test_dsrc].time;
        } else {
           // but if the user specifes a time that does not match
           // the previous query, then we need to do a new query
           doit = true;
        }       
     }
     if ( doit ) {
     
        result = catlog.query( quant, dayt, ds, modelrun );
        if ( result ) {
           test_dsrc = 0;
           opened_dsrc = -1;
           // TODO: check if the ds is the same as one currently opened, so we don;t have to close and re-open?
        }
     } else {
        // no need for any new query
        result = true;
     }

     return result;
}   

std::string MetMyGEOS::queryQuantity( int index )
{
    std::string result;
    bool ok;
    
    result = "";
    
    ok =  dsInit();
    if ( ok ) {
    
       if ( index < 0 ) {
          index = test_dsrc;
       }
       
       result = ds[test_dsrc].name;

    }

    return result;
}

std::string MetMyGEOS::queryUnits( int index )
{
    std::string result;
    bool ok;
    
    result = "";
    
    ok =  dsInit();
    if ( ok ) {
    
       if ( index < 0 ) {
          index = test_dsrc;
       }
       
       result = ds[test_dsrc].units;

    }

    return result;
}

std::string MetMyGEOS::queryTime( int index, std::string* pre, std::string* post )
{
    std::string result;
    std::string myPre;
    std::string myPost;
    bool ok;
    
    result = "";
    myPre = "";
    myPost = "";
    
    ok =  dsInit();
    if ( ok ) {
    
       if ( index < 0 ) {
          index = test_dsrc;
       }
       
       result = ds[test_dsrc].time;
       myPre = ds[test_dsrc].pre;
       myPost = ds[test_dsrc].post;
       
    }
    
    if ( pre != NULLPTR ) {
       *pre = myPre;
    }
    if ( post != NULLPTR ) {
       *post = myPost;
    }

    return result;

}

int MetMyGEOS::queryDimensionality( int index )
{
    int result;
    bool ok;
    
    result = 0;
    
    ok =  dsInit();
    if ( ok ) {
    
       if ( index < 0 ) {
          index = test_dsrc;
       }
       
       result = ds[test_dsrc].dims;

    }

    return result;
}

std::string MetMyGEOS::queryAttr(std::string& attr,  int index )
{
    std::string result;
    bool ok;
    
    result = "";
    
    ok =  dsInit();
    if ( ok ) {
    
       if ( index < 0 ) {
          index = test_dsrc;
       }
       
       result = catlog.getAttr( ds[test_dsrc], attr );

    }

    return result;

}

void MetMyGEOS::queryTimeSpacing( const std::string& quantity, double validAt, double& tinc, double& toff, int index )
{
    std::string result;
    bool ok;
    std::string toffstr;
    int nt;
    std::string validTime;
    
    result = "";
    
    validTime = time2Cal( validAt );
    ok =  dsInit(quantity, validTime );
    if ( ok ) {
    
       if ( index < 0 ) {
          index = test_dsrc;
       }
       
       
       catlog.get_timeSpacing( ds[index], &tinc, toffstr, &nt );
       toff = cal2Time( toffstr);

    }

}

std::string MetMyGEOS::queryVertical( int index )
{
    std::string result;
    bool ok;
    std::string vcode;
    
    result = "";
    
    ok =  dsInit();
    if ( ok ) {
    
       if ( index < 0 ) {
          index = test_dsrc;
       }
       
       vcode = catlog.getAttr( ds[index], "vertCoord" );
       
       //result.push_back( vcode[0] );   
       result = vcode;    

    }

    return result;

}

std::string MetMyGEOS::queryHorizontal( int index )
{
    std::string result;
    bool ok;
    
    result = "";
    
    ok =  dsInit();
    if ( ok ) {
    
       if ( index < 0 ) {
          index = test_dsrc;
       }
       
       result = catlog.getAttr( ds[test_dsrc], "horizCoord" );

    }

    return result;


}

//-------------------------------------------------------------------------------------------------------


bool MetMyGEOS::try_again( const int error, int &trial ) const
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
         std::cerr << " --- netcdf failure (" << error << "), trial " << trial 
                   << " of " << ntries << "; waiting " <<  waittry*trial << " seconds"
                   << std::endl;
         if ( waittry > 0 ) {
            (void) sleep( waittry*trial );
         }
         result = true;
      } else {
         // No more trials.
         std::cerr << " --- netcdf failure (" << error << "), trial " << trial 
                   << " of " << ntries << "; giving up now."
                   << std::endl;
         result = false;
      }
    }

    return result;
}

void MetMyGEOS::Source_open( bool pre, int index )
{
     std::string url;
     std::string name;
     int trial;
     int err;
     char attr_name[NC_MAX_NAME+1];
     char attr_cval[NC_MAX_NAME+1];
     int attr_type;
     size_t  attr_size;
     std::string s_value;
     float f_value;
     std::string attr_val_str;
     std::vector<float>  attr_val_f;
     std::vector<char>   attr_val_c;
     std::vector<int>    attr_val_i;
     std::vector<double> attr_val_d;
     bool do_open;

     int waittime;

     if ( dbug > 2 ) {
        std::cerr << "MetMyGEOS::Source_open: starting" << std::endl;
     }

     if ( index < 0 ) {
        index = test_dsrc;
     }
     
     // try this one first
     if ( index >= 0 ) {
        if ( pre ) {
           url = ds[index].pre;
        } else {
           url = ds[index].post;
        }
     }
     
     // No need to re-open the same URL unless:
     //  a) it's not open
     //  b) it's not the same URL
     if ( ( ! is_open) || ( url != opened_url ) ) { 

        // close the old one, if it is open
        if ( is_open ) {
           Source_close();
        }

        if ( dbug > 2 ) {
           std::cerr << "MetMyGEOS::Source_open: attempting initial nc_open of ds index " << index 
           << ": <<" << url  << ">>" << std::endl;
        }
             
        // but first, sleep some time between opens, to avoid being obnoxious to the server
        if ( openwait > 0 ) {
           if ( dbug > 2 ) {
              std::cerr << "************* About to wait " << openwait << " seconds on proc " 
                        << my_pgroup->id() << " group " << my_pgroup->group_id() << std::endl;
           }
           (void) sleep( openwait ); // sleep this time between opens, to avoid being obnoxious to the server
        }

        if ( ds[index].type != 2 ) {
        
           trial = 0;
           do {

              if ( dbug > 10 ) {
                 std::cerr << "MetMyGEOS::Source_open: attempting initial nc_open of: <<" << url  << ">>" << std::endl;
              }
                 
              std::cerr << " nc_opening url " << url << std::endl;
              err = nc_open( url.c_str(), NC_NOWRITE, &ncid);     
              //- std::cerr << " nc_opened url " << url << std::endl;
              
              // go through multiple open attempts only if this really is a URL.
           } while ( (err != NC_NOERR) && (ds[index].type == 1) && (try_again( err, trial ) ) );
           if ( err == NC_NOERR ) {
              is_open = true;
              opened_dsrc = index;
              opened_url = url;
              test_dsrc = index;
           } else {
              std::cerr << "MetMyGEOS::Source_open: Bad url: " << url << std::endl;
           }
        
        } else {
           if ( dbug > 5 ) {
              std::cerr << "Cannot open " << url << " as a netcdf file" << std::endl;
           }        
        }
        
        // either there was no first attempt, or there was one and it was unsuccessful
        if ( (index < 0) || ( err != NC_NOERR) ) {
        
           // ok, then try each DataSource in turn
           for ( index=0; index < ds.size(); index++ ) {
        
               if ( pre ) {
                  url = ds[index].pre;
               } else {
                  url = ds[index].post;
               }
           
               if ( dbug > 5 ) {
                  std::cerr << "MetMyGEOS::Source_open: attempting nc_open of ds index " << index 
                  << ": <<" << url  << ">>" << std::endl;
               }
           
              if ( ds[index].type != 2 ) {
           
                 trial = 0;
                 do {

                    if ( dbug > 5 ) {
                       std::cerr << "MetMyGEOS::Source_open: attempting nc_open of: <<" << url  << ">>" << std::endl;
                    }
                    
                    std::cerr << " nc_opening url " << url << std::endl;
                    err = nc_open( url.c_str(), NC_NOWRITE, &ncid);     
                    //- std::cerr << " nc_opened url " << url << std::endl;
                    
                    // go through multiple open attempts obly if this really is a URL.
                 } while ( (err != NC_NOERR) && (ds[index].type == 1) && (try_again( err, trial ) ) );
                 if ( err == NC_NOERR ) {
                 
                    if ( dbug > 5 ) {
                       std::cerr << "MetMyGEOS::Source_open: nc_open of ds index " << index 
                       << " succeeded" << std::endl;
                    }
                    
                    is_open = true;
                    
                    opened_dsrc = index;
                    opened_url = url;
                    test_dsrc = index;
                    
                    // set default target attribute priorities

                 
                    // don't try to open any more
                    break;
                 }
              } else {
                 if ( dbug > 5 ) {
                    std::cerr << "Cannot open " << url << " as a netcdf file" << std::endl;
                 }
              }
        
           }
        }
        
        // did at least one open attempt succees?
        if ( is_open ) {
        
           update_hgrid();
           update_vgrid();
           update_tgrid();
        
           trial = 0;
           do {
              err = nc_inq(ncid, &ndimens, &nvars, &ngatts, &unlimdim_idx);
           } while ( (ds[index].type == 1) && (try_again( err, trial ) ) );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        
           if ( dbug > 2 ) {
              std::cerr << "MetMyGEOS::Source_open: initial inq: " << ndimens << ", " << nvars 
                        << ", " << ngatts << ", " << unlimdim_idx << std::endl;
           }
            
           // go through the netcdf global attributes, one by one
           for (int i=0; i < ngatts; i++) {
               trial = 0;
               do {
                  err = nc_inq_attname(ncid, NC_GLOBAL, i, attr_name );
               } while ( (ds[index].type == 1) && (try_again( err, trial ) ) );
               if ( err != NC_NOERR ) {
                  throw(badNetcdfError(err));
               }
               
               // c-string to c++ string
               name.assign(attr_name);
               
               trial = 0;
               do {
                  err = nc_inq_att(ncid, NC_GLOBAL, attr_name, &attr_type, &attr_size );
               } while ( (ds[index].type == 1) && (try_again( err, trial ) ) );
               if ( err != NC_NOERR ) {
                  throw(badNetcdfError(err));
               }
               
               if ( dbug > 4 ) {
                  std::cerr << "MetMyGEOS::Source_open: reading global attr " << i << ": " << name 
                  << "(" << attr_type << " x " << attr_size << ") ";
               }
               
               switch (attr_type) {
               case NC_CHAR: 
                   // character-based metadata consists of human-readable information
                   // that is useful to know
                   //- std::cerr << " [NC_CHAR] ";
                   
                   // (but omit "ArchivedMetadata.0", since that does not work)
                   if ( name != "ArchivedMetadata.0" ) {
                   
                      Source_read_attr( attr_val_str, attr_name, NC_GLOBAL, attr_size );
                      if ( dbug > 4  ) {
                         std::cerr << "= " << gattr_strings[name];
                      }
                      gattr_strings[name] = attr_val_str;
                      
                   }
                   break;
               case NC_FLOAT:
                   //- std::cerr << " [NC_FLOAT] ";
                   // a float attribute may be an array of floats.
                   Source_read_attr( attr_val_f, attr_name, NC_GLOBAL, attr_size );
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
                   Source_read_attr( attr_val_d, attr_name, NC_GLOBAL, attr_size );
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

           // do the initial inquiries to get basic sizes and shapes
           Source_read_all_dims();

        } else {
           std::cerr << "MetMyGEOS::Source_open: Could not open any Data Sources for : " << ds[0].name << std::endl;
           throw(badNetcdfOpen(err));     
        }

     }

} 

void MetMyGEOS::Source_close()
{
    int err;
    int trial;
    
    if ( is_open ) {
       trial = 0;
       do {
          //- std::cerr << "nc_closing url" << std::endl;
          err = nc_close(ncid);
          //- std::cerr << "nc_closed url" << std::endl;
       } while ( try_again( err, trial ) );   
       if ( err != NC_NOERR ) {
          throw(badNetcdfError(err));
       } 
       is_open = 0; 
       opened_dsrc = -1;
       opened_url = "";
       // but of course we keep test_dsrc unchanged
       if ( dbug > 2 ) {
          std::cerr << "MetMyGEOS::Source_close: nc_close success!" << std::endl;        
       }
    }
    
    // reset the open-file dimensions 
    tgrid.clear();
    hgrid.clear();
    vgrid.clear();

} 


void MetMyGEOS::Source_checkdims(const int nvdims, const int*dimids )
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
            std::cerr << "MetMyGEOS::Source_checkdims:     dim " << i << " is " << name << " of size " << dim_size << std::endl;
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
               std::cerr << "MetMyGEOS::Source_checkdims: Unknown dimension named " << name << std::endl;
               throw(badUnknownDim());
         }
            
     }

}


double MetMyGEOS::Source_time_nativeTo1900( double nativeTime )
{
      //return nativeTime/24.0/3600.0  + 33969.000;
      return nativeTime/24.0/60.0  + time_zero;
};

double MetMyGEOS::Source_deltime_nativeTo1900( double nativeDelTime )
{
      return nativeDelTime/24.0/60.0;
};


int MetMyGEOS::Source_findtime(const std::string& quantity, const double desired_time )
{
    int result;
    std::string validTime;
    int idx;
    double xtime;
    double btime;
    double tincrement;
    bool ok;
    
    
    result = -1;
    
    //  time values are evenly-spaced, so we can calculate
    //  which one is closest, and if it is close enough

    if ( dbug > 4 ) {
       std::cerr << "MetMyGEOS::Source_findtime: Looking for time " << desired_time  << std::endl;
    }
    
    // put the time into string form, to feed to the Castalog
    validTime = time2Cal( desired_time );
    // Now have the Catalog look it up
    ok =  dsInit(quantity, validTime );
    if ( ok ) {
    
       // get the time of the first snapshot in the file, 
       // converted from Catalog time to model time
       btime = catTime2metTime( ds[test_dsrc].preStart_t );
       // get the delta time between snapshoits in the file
       tincrement = ds[test_dsrc].tDelta;
       if ( dbug > 4 ) {
          std::cerr << "MetMyGEOS::Source_findtime:  time inc is " << tincrement << std::endl;
          std::cerr << "MetMyGEOS::Source_findtime:  base time is " << btime << std::endl;
       }
    
    
       idx = 0;
       if ( tincrement > 0 ) {
          idx = round( (desired_time - btime)/tincrement );
       }
       
       xtime = btime + idx*tincrement;
       if ( dbug > 4 ) {
          std::cerr << "MetMyGEOS::Source_findtime:  determined index " << idx << " for " << xtime << std::endl;
       }    
   
       if ( idx >= 0  ) {
          // should be within 2 minutes
          if ( ABS(xtime - desired_time) < (2.0/60.0/24.0) ) {
             result = idx;
          } else {
            if ( dbug > 5 ) {
               double xt2 = desired_time;
               double xt1 = xtime;
               std::string st1 =  cal.date1900( xt1 );
               std::string st2 =  cal.date1900( xt2 );
               std::cerr << "MetMyGEOS::Source_findtime:  outside time tolerance  " 
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
       std::cerr << "MetMyGEOS::Source_findtime:  Returning result " << result << std::endl;
    }    
    return result;
    
}


void MetMyGEOS::Source_dim_attrs(const std::string &dname, const int var_id, const int nattrs )
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
       std::cerr << "MetMyGEOS::Source_get_attrs: " << nattrs << " Attributes:" << std::endl;
    }
    
    
    // for each attribute of this variable...
    for ( int ia=0; ia<nattrs; ia++ ) {
    
        if ( dbug > 5 ) {
           std::cerr << "MetMyGEOS::Source_get_attrs:      Attribute " << ia << " of " << nattrs << ":" << std::endl;
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
           std::cerr << "MetMyGEOS::Source_get_attrs:          attribute name is  " << aname <<  std::endl;
        }   
        
        // clear the output value
        outval.clear();
        
        // how we read in the attribute value depends on the value type
        switch (attr_type) {
        case NC_CHAR:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_get_attrs:          Reading " << attr_size << " characters for attribute " << aname << std::endl;
             }   

             Source_read_attr( outval, attrname, var_id, attr_size );

             if ( dname == legalDims[3] && attrname == "units" ) {
                size_t istart;
                istart = outval.find(" since ");
                if ( istart != std::string::npos ) {
                   std::string unts;
                   std::string strt;
                   unts = outval.substr( 0, istart );
                   double fctr;
                   if ( unts == "days" ) {
                      fctr = 1.0;
                   } else if ( unts == "hours" ) {
                      fctr = 1.0/24.0;
                   } else if ( unts == "minutes" ) {
                      fctr = 1.0/24.0/60.0;
                   } else if ( unts == "seconds" ) {
                      fctr = 1.0/24.0/3600.0;
                   } else {
                      fctr = 1.0;
                   }
                   strt = outval.substr( istart + 7 );
                   size_t iblank;
                   iblank = strt.find(" ");
                   if ( iblank != std::string::npos ) {
                      strt.replace( iblank, 1, "T" );
                      double timebeg;
                      timebeg = cal2Time( strt );
//+ do something with timebeg/strt and fctr here??

                   }
                }
             }
             
           break;
        case NC_BYTE:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_get_attrs:          Reading " << attr_size << " bytes for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_b, attrname, var_id, attr_size );

             attr_val_b.clear();

           break;
        case NC_SHORT:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_get_attrs:          Reading " << attr_size << " shorts for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_s, attrname, var_id, attr_size );
             
             attr_val_s.clear();
                
           break;
        case NC_INT:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_get_attrs:          Reading " << attr_size << " ints for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_i, attrname, var_id, attr_size );

             attr_val_i.clear();    
           break;
        case NC_FLOAT:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_get_attrs:          Reading " << attr_size << " floats for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_f, attrname, var_id, attr_size );

             
             attr_val_f.clear();    

           break;
        case NC_DOUBLE:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_get_attrs:          Reading " << attr_size << " doubles for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_d, attrname, var_id, attr_size );


             attr_val_d.clear();    
           break;
        default:
           if (dbug > 5 ) {
              std::cerr << "MetMyGEOS::Source_get_attrs:   unknown attr data type " << attr_type << " for " << attrname << std::endl;
           }
           throw(badNetcdfError(NC_EBADTYPE));   
        } 

        
    
    }
    

}


// 3D
void MetMyGEOS::Source_getattrs(const int var_id, const int nattrs, GridLatLonField3D *datagrid )
{
    int err;
    real badval_def = fillval;
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
       std::cerr << "MetMyGEOS::Source_getattrs: (3D) Attributes:" << std::endl;
    }
    
    // for each attribute of this variable...
    for ( int ia=0; ia<nattrs; ia++ ) {
    
        if ( dbug > 5 ) {
           std::cerr << "MetMyGEOS::Source_getattrs: (3D)      Attribute " << ia << " of " << nattrs << ":" << std::endl;
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
           std::cerr << "MetMyGEOS::Source_getattrs:  (3D)        attribute name is  " << aname <<  std::endl;
        }   
        
        // clear the output value
        outval.clear();
        
        sconv.str("");
        
        // how we read in the attribute value depends on the value type
        switch (attr_type) {
        case NC_CHAR:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_getattrs: (3D)         Reading " << attr_size << " characters for attribute " << aname << std::endl;
             }   

             Source_read_attr( outval, attrname, var_id, attr_size );

             if ( aname == "units" ) {
                if ( datagrid->units() != outval ) {
                   std::cerr << "Warning; " << datagrid->quantity() << " units are really "
                   << outval << " instead of " << datagrid->units() << std::endl;
                }
                datagrid->set_units( outval );
             } 
             
             sconv << outval; 
             
           break;
        case NC_BYTE:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_getattrs:  (3D)        Reading " << attr_size << " bytes for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_b, attrname, var_id, attr_size );

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
                std::cerr << "MetMyGEOS::Source_getattrs: (3D)         Reading " << attr_size << " shorts for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_s, attrname, var_id, attr_size );

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
                std::cerr << "MetMyGEOS::Source_getattrs:  (3D)        Reading " << attr_size << " ints for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_i, attrname, var_id, attr_size );

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
                std::cerr << "MetMyGEOS::Source_getattrs: (3D)         Reading " << attr_size << " floats for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_f, attrname, var_id, attr_size );

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
                std::cerr << "MetMyGEOS::Source_getattrs:  (3D)        Reading " << attr_size << " doubles for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_d, attrname, var_id, attr_size );

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
              std::cerr << "MetMyGEOS::Source_getattrs: (3D)  unknown attr data type " << attr_type << " for " << attrname << std::endl;
           }
           throw(badNetcdfError(NC_EBADTYPE));   
        } 
        
        
        // store the attribute
        if ( dbug > 5 ) {
           std::cerr << "MetMyGEOS::Source_getattrs:  (3D)        Setting attribute " << aname << " to:<<" << sconv.str() << ">>" << std::endl;
        }   
        datagrid->set_attribute( aname, sconv.str() ); 
        
    
    }
    
    // set the bad-or-missing-data flag
    if ( badval_0 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetMyGEOS::Source_getattrs: (3D)      Setting badval to 0: " << badval_0 << std::endl;
       }   
       datagrid->set_fillval( badval_0 );
    } else if ( badval_1 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetMyGEOS::Source_getattrs: (3D)      Setting badval to 1: " << badval_1 << std::endl;
       }   
       datagrid->set_fillval( badval_1 );
    } else if ( badval_2 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << " MetMyGEOS::Source_getattrs:  (3D)       Setting badval to 2: " << badval_2 << std::endl;
       }   
       datagrid->set_fillval( badval_1 );
    } else {
       if ( dbug > 5 ) {
          std::cerr << "MetMyGEOS::Source_getattrs:  (3D)     Setting badval to def: " << badval_def << std::endl;
       }   
       datagrid->set_fillval( badval_def );
    }         

}

/// Sfc
void MetMyGEOS::Source_getattrs(const int var_id, const int nattrs, GridLatLonFieldSfc *datagrid )
{
    int err;
    real badval_def = fillval;
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
       std::cerr << "MetMyGEOS::Source_getattrs: (Sfc)  Attributes:" << std::endl;
    }
    
    // for each attribute of this variable...
    for ( int ia=0; ia<nattrs; ia++ ) {
    
        if ( dbug > 5 ) {
           std::cerr << "MetMyGEOS::Source_getattrs: (Sfc)     Attribute " << ia << " of " << nattrs << ":" << std::endl;
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
           std::cerr << "MetMyGEOS::Source_getattrs:  (Sfc)       attribute name is  " << aname <<  std::endl;
        }   
        
        // clear the output value
        outval.clear();
        
        sconv.str("");
        
        // how we read in the attribute value depends on the value type
        switch (attr_type) {
        case NC_CHAR:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_getattrs: (Sfc)        Reading " << attr_size << " characters for attribute " << aname << std::endl;
             }   

             Source_read_attr( outval, attrname, var_id, attr_size );

             if ( aname == "units" ) {
                datagrid->set_units( outval );
             } 
             
             sconv << outval; 
             
           break;
        case NC_BYTE:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_getattrs: (Sfc)        Reading " << attr_size << " bytes for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_b, attrname, var_id, attr_size );

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
                std::cerr << "MetMyGEOS::Source_getattrs:  (Sfc)       Reading " << attr_size << " shorts for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_s, attrname, var_id, attr_size );

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
                std::cerr << "MetMyGEOS::Source_getattrs: (Sfc)        Reading " << attr_size << " ints for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_i, attrname, var_id, attr_size );

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
                std::cerr << "MetMyGEOS::Source_getattrs:  (Sfc)       Reading " << attr_size << " floats for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_f, attrname, var_id, attr_size );

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
                std::cerr << "MetMyGEOS::Source_getattrs:  (Sfc)       Reading " << attr_size << " doubles for attribute " << aname << std::endl;
             }   

             Source_read_attr( attr_val_d, attrname, var_id, attr_size );

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
              std::cerr << "MetMyGEOS::Source_getattrs:  (Sfc)unknown attr data type " << attr_type << " for " << attrname << std::endl;
           }
           throw(badNetcdfError(NC_EBADTYPE));   
        } 
        
        
        // store the attribute
        if ( dbug > 5 ) {
           std::cerr << "MetMyGEOS::Source_getattrs: (Sfc)        Setting attribute " << aname << " to:<<" << sconv.str() << ">>" << std::endl;
        }   
        datagrid->set_attribute( aname, sconv.str() ); 
        
    
    }
    
    // set the bad-or-missing-data flag
    if ( badval_0 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetMyGEOS::Source_getattrs: (Sfc)     Setting badval to 0: " << badval_0 << std::endl;
       }   
       datagrid->set_fillval( badval_0 );
    } else if ( badval_1 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetMyGEOS::Source_getattrs:  (Sfc)    Setting badval to 1: " << badval_1 << std::endl;
       }   
       datagrid->set_fillval( badval_1 );
    } else if ( badval_2 != 0.0 ) {
       if ( dbug > 5 ) {
          std::cerr << "MetMyGEOS::Source_getattrs:  (Sfc)       Setting badval to 2: " << badval_2 << std::endl;
       }   
       datagrid->set_fillval( badval_1 );
    } else {
       if ( dbug > 5 ) {
          std::cerr << "MetMyGEOS::Source_getattrs:  (Sfc)    Setting badval to def: " << badval_def << std::endl;
       }   
       datagrid->set_fillval( badval_def );
    }         

}



void MetMyGEOS::Source_read_attr( std::string &result, char *attr_name, int varid, size_t len )
{
    char *val;
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

    result.clear();
    result.assign(val, len);

    delete[] val;
   
}


void MetMyGEOS::Source_read_attr( std::vector<unsigned char> &result, char *attr_name, int varid, size_t len )
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
    
    delete[] val;
   
}

void MetMyGEOS::Source_read_attr( std::vector<short> &result, char *attr_name, int varid, size_t len )
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
    
    delete[] val;

}


void MetMyGEOS::Source_read_attr( std::vector<int> &result, char *attr_name, int varid, size_t len )
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
    
    delete[] val;

}


void MetMyGEOS::Source_read_attr( std::vector<float> &result, char *attr_name, int varid, size_t len )
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

void MetMyGEOS::Source_read_attr( std::vector<double> &result, char *attr_name, int varid, size_t len )
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



void MetMyGEOS::Source_read_dim( std::string &dim_name, nc_type &dim_type, SpanTriplet &span) 
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
     // name of the attribute
     char attrname[NC_MAX_NAME+1];
     // attribute type
     nc_type attr_type;
     // number of elements in the attribute
     size_t attr_size;
     // attribute name
     std::string aname;
     // attribute value string
     std::string aoutval;
     // attribute integer value
     std::vector<int> attr_val_i;
     // base time of the file (two different sources)
     std::string refTime1;
     std::string refTime2;
     double refTime;
     // used for int->string conversions
     std::ostringstream sconv;
     // factor for converting the times
     double tfactor;
     // time delta from file attributes
     double tdelta;
     
     
     // by default, the times in the file are in minutes
     // so to convert to days we do multiply by
     tfactor = 1.0/60.0/24.0;
     
     // set this to a flag value
     tdelta = -1.0;


     c_dim_name = dim_name.c_str();
     
     if ( dbug >= 3 ) {
        std::cerr << "MetMyGEOS::Source_read_dim: attempting to read dimension " << dim_name << std::endl;
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
        std::cerr << "MetMyGEOS::Source_read_dim: no netdcf varianble id for dimension " << dim_name << std::endl;
        throw(badNetcdfError(err));
     }
     if ( dbug >= 5 ) {
        std::cerr << "MetMyGEOS::Source_read_dim: var id =  " << dvar_id << std::endl;
     } 

     // get the characteristics
     trial = 0;
     do {
        err = nc_inq_var(ncid, dvar_id, dimvar_name, &dim_type, &dndim, ddims, &dnatt );
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        std::cerr << "MetMyGEOS::Source_read_dim: cannot read characteristics for ncdf var id " << dvar_id << " or dimension " << dim_name << std::endl;
        throw(badNetcdfError(err));
     }
     if ( dndim != 1 ) {
        std::cerr << "MetMyGEOS::Source_read_dim: Bad number-of-dimensions for " << dim_name << " : " << dndim << std::endl;        
     }
     if ( dbug >= 5 ) {
        std::cerr << "MetMyGEOS::Source_read_dim: dndim =  " << dndim << std::endl;
     }
     
     // If this is time, look at the attributes, since
     // the reference starting point of the time values is
     // usually given here
     if ( dim_name == "time" ) {

        sconv.str("");
        sconv.setf(std::ios::dec, std::ios::basefield);
        sconv.setf(std::ios::fixed, std::ios::floatfield);
        // all numbers should be printed with leading zeroes
        sconv.fill('0');
        // fill from the front
        sconv.setf(std::ios::internal, std::ios::adjustfield);
        
        attr_val_i.clear();    

        // for each attribute of this variable...
        for ( int ia=0; ia<dnatt; ia++ ) {
    
           if ( dbug > 5 ) {
              std::cerr << "MetMyGEOS::Source_read_dim: time Attribute " << ia << " of " << dnatt << ":" << std::endl;
           } 
        
           trial = 0;
           do {  
              err = nc_inq_attname( ncid, dvar_id, ia, attrname);
           } while ( try_again( err, trial ) );   
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        
           // get information about the attribute
           trial = 0;
           do {  
              err = nc_inq_att(ncid, dvar_id, attrname, &attr_type, &attr_size );
           } while ( try_again( err, trial ) );   
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        
           aname.assign(attrname);
           if ( dbug > 5 ) {
              std::cerr << "MetMyGEOS::Source_read_dim:  time attribute name is  " << aname <<  std::endl;
           }   
        
           if ( aname == "units" ) {
              if ( attr_type == NC_CHAR ) {
                 if ( dbug > 5 ) {
                    std::cerr << "MetMyGEOS::Source_read_dim: time Reading " << attr_size << " characters for attribute " << aname << std::endl;
                 }   

                 Source_read_attr( aoutval, attrname, dvar_id, attr_size );
                 
                 // now parse the units
                 int pos;
                 if ( (aoutval.size() > 11) && (aoutval.substr(0,1) == "d" || aoutval.substr(0,1) == "D" ) ) {
                    if ( aoutval.substr(1,10) == "ays since " ) {
                       tfactor = 1.0;
                       pos = 11;      
                    }
                 } else if ( (aoutval.size() > 12) && (aoutval.substr(0,1) == "h" || aoutval.substr(0,1) == "H" ) ) {
                    if ( aoutval.substr(1,11) == "ours since " ) {
                       tfactor = 1.0/24.0;
                       pos = 12;      
                    }
                 } else if ( (aoutval.size() > 14) && (aoutval.substr(0,1) == "m" || aoutval.substr(0,1) == "M" ) ) {
                    if ( aoutval.substr(1,13) == "inutes since " ) {
                       tfactor = 1.0/(24.0*60.0);
                       pos = 14;      
                    }
                 } else if ( (aoutval.size() > 14) && (aoutval.substr(0,1) == "s" || aoutval.substr(0,1) == "S" ) ) {
                    if ( aoutval.substr(1,13) == "econds since " ) {
                       tfactor = 1.0/(24.0*60.0*60.0);      
                       pos = 14;
                    }
                 }
                 
                 refTime2 = aoutval.substr(pos);
                 refTime2[10] = 'T';
                 
              }
           } else if ( aname == "begin_date" ) {
              if ( dbug > 5 ) {
                 std::cerr << "MetMyGEOS::Source_read_dim: time Reading " << attr_size << " ints for attribute " << aname << std::endl;
              }   

              Source_read_attr( attr_val_i, attrname, dvar_id, attr_size );

              int tval = attr_val_i[0];
              int year = tval / 10000;
              int mon  = (tval % 10000)/100;
              int day  = tval % 100;
             
              sconv.width( 4 );
              sconv << year;
              sconv << "-";
              sconv.width( 2 );
              sconv << mon;
              sconv << "-";
              sconv.width( 2 );
              sconv << day;
              if ( refTime1 == "" ) {
                  // no time yet
                  refTime1 = sconv.str();
              } else {
                  // time is already in place
                  refTime1 = sconv.str() + refTime1;
              }
             
              sconv.str("");
              attr_val_i.clear();    
           
           } else if ( aname == "begin_time" ) {

              if ( dbug > 5 ) {
                 std::cerr << "MetMyGEOS::Source_read_dim: time Reading " << attr_size << " ints for attribute " << aname << std::endl;
              }   

              Source_read_attr( attr_val_i, attrname, dvar_id, attr_size );

              int tval = attr_val_i[0];
              int hour = tval / 10000;
              int min  = (tval % 10000)/100;
              int sec  = tval % 100;
             
              sconv << "T";
              sconv.width( 2 );
              sconv << hour;
              sconv << ":";
              sconv.width( 2 );
              sconv << min;
              sconv << ":";
              sconv.width( 2 );
              sconv << sec;
              refTime1 = refTime1 + sconv.str();
             
              sconv.str("");
              attr_val_i.clear();    
           
           } else if ( aname == "time_increment" ) {

              if ( dbug > 5 ) {
                 std::cerr << "MetMyGEOS::Source_read_dim: time Reading " << attr_size << " ints for attribute " << aname << std::endl;
              }   

              Source_read_attr( attr_val_i, attrname, dvar_id, attr_size );

              int tval = attr_val_i[0];
              int hour = tval / 10000;
              int min  = (tval % 10000)/100;
              int sec  = tval % 100;

              tdelta = static_cast<double>(hour)
                       + ( static_cast<double>(min)
                         + static_cast<double>(sec)/60.0 
                         )/60.0;
                           
              attr_val_i.clear();    
           
           }
           
           if ( refTime1 != "" ) {
              refTime = cal2Time( refTime1 );
           } else if ( refTime2 != "" ) {
              refTime = cal2Time( refTime2 );
           } else {
              refTime = 0.0;
           }
        
        } // end of attr loop

     }
     
     // now read the dimension data values
     switch (dim_type) {
     case NC_FLOAT:
        if ( dbug > 5 ) {
            std::cerr << "MetMyGEOS::Source_read_dim:    trying to read 2 of " << dim_size << " NC_FLOATs for dim " << dim_name << std::endl;
        }
        Source_read_dim_floats( f_first, f_last, f_delta, dvar_id, dim_size );
        span.floatSpec.first = f_first;
        span.floatSpec.last = f_last;
        span.floatSpec.delta = f_delta;
        span.floatSpec.size = dim_size;
        if ( dim_name == "time" ) {
           span.floatSpec.first = f_first*tfactor + refTime;
           span.floatSpec.last = f_last*tfactor + refTime;
           span.floatSpec.delta = f_delta*tfactor*24.0;  // hours
           if ( tdelta >= 0 ) {
              span.floatSpec.delta = tdelta;
           }
        }
        break;
     case NC_DOUBLE:
        if ( dbug > 5 ) {
            std::cerr << "MetMyGEOS::Source_read_dim:    trying to read 2 of " << dim_size << " NC_DOUBLEs for dim " << dim_name << std::endl;
        }
        Source_read_dim_doubles( d_first, d_last, d_delta, dvar_id, dim_size );
        span.doubleSpec.first = d_first;
        span.doubleSpec.last = d_last;
        span.doubleSpec.delta = d_delta;
        span.doubleSpec.size = dim_size;
        if ( dim_name == "time" ) {
           span.doubleSpec.first = d_first*tfactor + refTime;
           span.doubleSpec.last = d_last*tfactor + refTime;
           span.doubleSpec.delta = d_delta*tfactor*24.0;  // hours
           if ( tdelta >= 0 ) {
              span.doubleSpec.delta = tdelta;
           }
        }
        break;
     case NC_INT:
        if ( dbug > 5 ) {
            std::cerr << "MetMyGEOS::Source_read_dim:    trying to read 2 of " << dim_size << " NC_INTs for dim " << dim_name << std::endl;
        }
        Source_read_dim_ints( i_first, i_last, i_delta, dvar_id, dim_size );
        span.intSpec.first = i_first;
        span.intSpec.last = i_last;
        span.intSpec.delta = i_delta;
        span.intSpec.size = dim_size;
        break;
     default:
        std::cerr << "MetMyGEOS::Source_read_dim: Unimplemented dimensional netcdf format: " << dim_type << std::endl;
        throw(badNetcdfError(err));
     }

}


void MetMyGEOS::Source_read_dim( std::string &dim_name, std::vector<real> &vals)
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
        std::cerr << "MetMyGEOS::Source_read_dim: no netdcf varianble id for dimension " << dim_name << std::endl;
        throw(badNetcdfError(err));
     }
     // get the characteristics
     trial = 0;
     do {
        err = nc_inq_var(ncid, dvar_id, dimvar_name, &dim_type, &dndim, ddims, &dnatt );
     } while ( try_again( err, trial ) );       
     if ( err != NC_NOERR ) {
        std::cerr << "MetMyGEOS::Source_read_dim: cannot read characteristics for ncdf var id " << dvar_id << " or dimension " << dim_name << std::endl;
        throw(badNetcdfError(err));
     }
     if ( dndim != 1 ) {
        std::cerr << "MetMyGEOS::Source_read_dim: Bad number-of-dimensions for " << dim_name << " : " << dndim << std::endl;
        
     }
     
     switch (dim_type) {
     case NC_FLOAT:
        if ( dbug > 5 ) {
            std::cerr << "MetMyGEOS::Source_read_dim:    trying to read " << dim_size << " NC_FLOATs for dim " << dim_name << std::endl;
        }
        Source_read_dim_floats( vals, dvar_id, dim_size );
        break;
     case NC_DOUBLE:
        if ( dbug > 5 ) {
            std::cerr << "MetMyGEOS::Source_read_dim:    trying to read " << dim_size << " NC_DOUBLEs for dim " << dim_name << std::endl;
        }
        Source_read_dim_doubles( vals, dvar_id, dim_size );
        break;
     case NC_INT:
        if ( dbug > 5 ) {
            std::cerr << "MetMyGEOS::Source_read_dim:    trying to read " << dim_size << " NC_INTs for dim " << dim_name << std::endl;
        }
        Source_read_dim_ints( vals, dvar_id, dim_size );
        break;
     default:
        std::cerr << "MetMyGEOS::Source_read_dim: Unimplemented dimensional netcdf format: " << dim_type << std::endl;
        throw(badNetcdfError(err));
     }


}


void MetMyGEOS::update_hgrid()
{
    bool ok;
    std::string hspec;
    int newLen;
    real lonstrt, lonend, londelta;
    int nlon;
    real latstrt, latend, latdelta;
    int nlat;
    

    ok =  dsInit();
    if ( ok ) {
       hspec = queryHorizontal();
    
    }
    
    if ( hspec == "MN" ) {
    
       hgrid.code = hspec;

       lonstrt = -180.0;
       lonend = 179.375;
       nlon = 576;
       latstrt = -90.0;
       latend = 90.0;
       nlat = 361;
       
       if ( ! hgrid.set( &lonstrt, &lonend, NULLPTR, &nlon, &latstrt, &latend, NULLPTR, &nlat ) ) {
          std::cerr << "Bad horizontal grid: " << hspec << std::endl;
          throw (badCoordinateGrid());
       }
    
    } else if ( hspec == "FN" ) {
    
       hgrid.code = hspec;

       lonstrt = -180.0;
       lonend = 179.6875;
       nlon = 1152;
       latstrt = -90.0;
       latend = 90.0;
       nlat = 721;
       
       if ( ! hgrid.set( &lonstrt, &lonend, NULLPTR, &nlon, &latstrt, &latend, NULLPTR, &nlat ) ) {
          std::cerr << "Bad horizontal grid: " << hspec << std::endl;
          throw (badCoordinateGrid());
       }

    } else if ( hspec == "1x1" ) {
    
       hgrid.code = hspec;

       lonstrt = -180.0;
       lonend = 179.0;
       nlon = 360;
       latstrt = -90.0;
       latend = 90.0;
       nlat = 181;
       
       if ( ! hgrid.set( &lonstrt, &lonend, NULLPTR, &nlon, &latstrt, &latend, NULLPTR, &nlat ) ) {
          std::cerr << "Bad horizontal grid: " << hspec << std::endl;
          throw (badCoordinateGrid());
       }

    } else if ( hspec == "0.5x0.5" ) {
    
       hgrid.code = hspec;

       lonstrt = -180.0;
       lonend = 179.5;
       nlon = 720;
       latstrt = -90.0;
       latend = 90.0;
       nlat = 361;
       
       if ( ! hgrid.set( &lonstrt, &lonend, NULLPTR, &nlon, &latstrt, &latend, NULLPTR, &nlat ) ) {
          std::cerr << "Bad horizontal grid: " << hspec << std::endl;
          throw (badCoordinateGrid());
       }

    } else {
        std::cerr << "Unknown coordinate grid: " << hspec << std::endl;
        throw (badCoordinateGrid());
    }
    
    // adjust the horiz grid if we are thinning things out
    hgrid.thin = 1;
    if ( skip > 0 && skip < (hgrid.nLons/2) ) {
       hgrid.thin = skip;
    }
    hgrid.thin_offset = 0;
    if ( skoff >= 0 && skoff < skip ) {
       hgrid.thin_offset = skoff;
    }
   
    newLen = hgrid.nLons/hgrid.thin;
    if ( newLen*hgrid.thin < hgrid.nLons ) {
       newLen++;
    }

     // reset the lons and lats
     lons.clear();
     lons.reserve( newLen );
     for ( int i=0; i<newLen; i++ ) {
         lons.push_back( hgrid.startLon + hgrid.deltaLon*(hgrid.thin_offset + hgrid.thin*i) );
     }
     nlons = lons.size();
         
     newLen = hgrid.nLats/hgrid.thin;
     if ( newLen*hgrid.thin < hgrid.nLats ) {
        newLen++;
     }
 
     lats.clear();
     lats.reserve( newLen );
     for ( int i=0; i<newLen; i++ ) {
         lats.push_back( hgrid.startLat + hgrid.deltaLat*hgrid.thin*i );
     }
     nlats = lats.size();

}
void MetMyGEOS::update_vgrid()
{
    int nz;
    bool ok;
    std::string vspec;
    int newLen;
    real scale, offset;
    
    ok =  dsInit();
    if ( ok ) {
       vspec = queryVertical();
    }

    vgrid.clear();
    
    if ( vspec[0] == 'P' ) {
    
       scale = 100.0;
       offset = 0.0;
          
       vgrid.set( vspec, pressure_name, "hPa", &scale, &offset, NULLPTR );
              
    } else if ( vspec == "H" ) {
    
       scale = 1.0;
       offset = 0.0;
          
       vgrid.set( vspec, pottemp_name, "K", &scale, &offset, NULLPTR );
       
    } else if ( vspec == "a" ) {
    
       scale = 1000.0;
       offset = 0.0;
          
       vgrid.set( vspec, palt_name, "km", &scale, &offset, NULLPTR );
       
    } else if ( vspec == "z" ) {
    
       scale = 1000.0;
       offset = 0.0;
          
       vgrid.set( vspec, altitude_name, "km", &scale, &offset, NULLPTR );
       
    } else if ( vspec[0] == 'L' ) {
    
       scale = 1.0;
       offset = 0.0;
          
       vgrid.set( vspec, "Model-Levels", "1", &scale, &offset, NULLPTR );
       
    } else if ( vspec == "E" ) {
    
       scale = 1.0;
       offset = 0.0;
          
       vgrid.set( vspec, "Model-Edges", "1", &scale, &offset, NULLPTR );

    } else if ( vspec == "2" ) {
        std::cerr << "Cannot set a vert coordinate for a 2D grid: " << vspec << std::endl;
        throw (badCoordinateGrid());
    } else {
        std::cerr << "Unknown vert coordinate grid: " << vspec << std::endl;
        throw (badCoordinateGrid());
    }


    // native is what we are using
    native_zs = vgrid.levs;
    native_nzs = vgrid.nLevs;

}

void MetMyGEOS::update_tgrid()
{
     double start, next, delta;
     int n;
     std::string tx;
          
     start = catTime2metTime( ds[test_dsrc].preStart_t );
     next  = catTime2metTime( ds[test_dsrc].postStart_t );
     delta = ds[test_dsrc].tDelta;
     
     tgrid.set( start, next, delta );

}

void MetMyGEOS::Source_read_all_dims()
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
    HGridSpec xhgrid;
    VGridSpec xvgrid;
    TGridSpec xtgrid;
    double tstart, tend, tdelta;
    real xstart, xend, xdelta;
    real ystart, yend, ydelta;
    int tn, xn, yn;
    HGridSpec url_hgrid;
    VGridSpec url_vgrid;
    TGridSpec url_tgrid;
    
    dname = legalDims[3]; // time
    Source_read_dim( dname, dim_type, span);
    switch (dim_type) {
    case NC_DOUBLE:
        tstart = span.doubleSpec.first;
        tend   = span.doubleSpec.last;
        tdelta = span.doubleSpec.delta;
        tn     = span.doubleSpec.size;      
       break;
    case NC_FLOAT:
        tstart = static_cast<double>(span.floatSpec.first);
        tend   = static_cast<double>(span.floatSpec.last);
        tdelta = static_cast<double>(span.floatSpec.delta);
        tn     = span.floatSpec.size;
       break;
    case NC_INT:
       tstart = static_cast<double>(span.intSpec.first);
       tend   = static_cast<double>(span.intSpec.last);
       tdelta = static_cast<double>(span.intSpec.delta);
       tn     = span.intSpec.size; 
       break;
    default:
       std::cerr << "MetMyGEOS::Source_read_all_dims: Unimplemented format for dim " << dname << ": " << dim_type << std::endl;
       throw(badDimsForm());
    }
    url_tgrid.set( tstart, tstart + tn*tdelta/24.0, tdelta/24.0 );
    if ( ! tgrid.test( url_tgrid ) ) {
       std::cerr << "MetMyGEOS::Source_read_all_dims: The file's time gridding is incompatible with its specifications. " << dname << ": " << dim_type << std::endl;
       throw(badDimsForm()); 
    }
       
    dname = legalDims[0]; // longitude
    Source_read_dim( dname, dim_type, span);
    switch (dim_type) {
    case NC_DOUBLE:
        xstart = span.doubleSpec.first;
        xend   = span.doubleSpec.last;
        xdelta = span.doubleSpec.delta;
        xn     = span.doubleSpec.size;
        // these are used below for url_hgrid
       break;
    default:
       std::cerr << "MetMyGEOS::Source_read_all_dims: Unimplemented format for dim " << dname << ": " << dim_type << std::endl;
       throw(badDimsForm());
    }
       
    dname = legalDims[1];  // latitude
    Source_read_dim( dname, dim_type, span);
    switch (dim_type) {
    case NC_DOUBLE:
        ystart = span.doubleSpec.first;
        yend   = span.doubleSpec.last;
        ydelta = span.doubleSpec.delta;
        yn     = span.doubleSpec.size;
        
       break;
    default:
       std::cerr << "MetMyGEOS::Source_read_all_dims: Unimplemented format for dim " << dname << ": " << dim_type << std::endl;
       throw(badDimsForm());
    }
    url_hgrid.set( &xstart, NULLPTR, &xdelta, &xn, &ystart, NULLPTR, &ydelta, &yn ); 
    if ( ! hgrid.test( url_hgrid ) ) {
       std::cerr << "MetMyGEOS::Source_read_all_dims: The file's horizontal gridding is incompatible with its specifications. " << dname << ": " << dim_type << std::endl;
       throw(badDimsForm()); 
    }
    
    
    try {
       dname = legalDims[2]; // vertical level
       Source_read_dim( dname, zvals);
       
       url_vgrid = vgrid;
       url_vgrid.levs = zvals;
       url_vgrid.nLevs = zvals.size();

       
    //- std::cerr << " init tim:" << tgrid.n << " vals from " << tgrid.start 
    //<< " to " << tgrid.end << " via " << tgrid.delta 
    //<< " w/ base " << basetime << std::endl;
       
    } catch (badMissingDim err) {
       // no vertical levels in this file.
       zvals.clear();
       
       url_vgrid.code = "2";
       url_vgrid.levs.clear();
       url_vgrid.nLevs = 0;
    } 
    
    if ( ! vgrid.test( url_vgrid ) ) {
       std::cerr << "MetMyGEOS::Source_read_all_dims: The file's vertical gridding is incompatible with its specifications. " << dname << ": " << dim_type << std::endl;
       throw(badDimsForm()); 
    }
//-    update_vgrid();   

}




void MetMyGEOS::Source_read_dim_doubles( std::vector<real>&vals, int varid, size_t len )
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
       std::cerr << "MetMyGEOS::Source_read_dim_doubles: Failed trying to read " << len << " NC_DOUBLEs " << std::endl;
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
        vals.push_back( buffr[j] );
    }
    
    delete[] buffr;
    
}

void MetMyGEOS::Source_read_dim_doubles( double &first, double &last, double &delta, int varid, size_t len )
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
       std::cerr << "MetMyGEOS::Source_read_dim_doubles: Failed trying to read " << len << " NC_DOUBLEs " << std::endl;
       throw(badNetcdfError(err));
    }
    
    first = buffr[0];
    if ( len > 1 ) {
       last = buffr[1];
       delta = ( last - first )/(len - 1);
    } else {
       last = buffr[0];
       delta = 0.0;
    }

}

void MetMyGEOS::Source_read_dim_floats( std::vector<real>&vals, int varid, size_t len )
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
       std::cerr << "MetMyGEOS::Source_read_dim_floats: Failed trying to read " << len << " NC_FLOATs " << std::endl;
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
        vals.push_back( buffr[j] );
    }
    
    delete[] buffr;

}

void MetMyGEOS::Source_read_dim_floats( float &first, float &last, float &delta, int varid, size_t len )
{
    float buffr[2];
    int err;
    size_t zero = 0;
    size_t count;
    ptrdiff_t stride;
    int trial;
    
    if ( len > 1 ) {
       count = 2;
       stride = len - 1;
    } else {
       count = 1;
       stride = 1;
    }

    //- std::cerr << "Reading " << len << " float dims " << std::endl;    
    trial = 0;
    do {
       err = nc_get_vars_float( ncid, varid, &zero, &count, &stride, buffr );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       std::cerr << "MetMyGEOS::Source_read_dim_floats: Failed trying to read " << len << " NC_FLOATs " << std::endl;
       throw(badNetcdfError(err));
    }
    
    first = buffr[0];
    if ( len > 1 ) {
       last = buffr[1];
       delta = ( last - first )/(len - 1);
    } else {
       last = buffr[0];
       delta = 0.0;
    }
}

void MetMyGEOS::Source_read_dim_ints( std::vector<real>&vals, int varid, size_t len )
{
    int* buffr;
    int err;
    size_t zero = 0;
    size_t count;
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
       std::cerr << "MetMyGEOS::Source_read_dim_ints: Failed trying to read " << len << " NC_INTs " << std::endl;
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
        vals.push_back( static_cast<real>(buffr[j]) );
    }
    
    delete[] buffr;
    
}

void MetMyGEOS::Source_read_dim_ints( int &first, int &last, int &delta, int varid, size_t len )
{
    int buffr[2];
    int err;
    size_t zero = 0;
    size_t count;
    ptrdiff_t stride;
    int trial;
    
    if ( len > 1 ) {
       count = 2;
       stride = len - 1;
    } else {
       count = 1;
       stride = 1;
    }

    // std::cerr << "Reading " << len << " int dims " << std::endl;    
    trial = 0;
    do {
       err = nc_get_vars_int( ncid, varid, &zero, &count, &stride, buffr );
    } while ( try_again( err, trial ) );       
    if ( err != NC_NOERR ) {
       std::cerr << "MetMyGEOS::Source_read_dim_ints: Failed trying to read " << len << " NC_INTs " << std::endl;
       throw(badNetcdfError(err));
    }
    
    first = buffr[0];
    if ( len > 1 ) {
       last = buffr[1];
       delta = ( last - first )/(len - 1);
    } else {
       last = buffr[0];
       delta = 0;
    }

}

void MetMyGEOS::Source_read_data_floats( std::vector<real>&vals, int var_id, int ndims, size_t *starts, size_t *counts, ptrdiff_t *strides )
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
        std::cerr << "MetMyGEOS::Source_read_data: (" << ndims << "D):  about to read data! " <<  std::endl;
     }

     vals.clear();
     vals.reserve(totsize);
     //vals.resize(totsize);


     err = NC_NOERR;
     bp = buffr;
     int isample = toread/10;
     for ( int ichunk = 0; (ichunk < nchunks) && (err == NC_NOERR) ; ichunk++ ) {

         trial = 0;
         do {

            if ( dbug > 5 ) {
               std::cerr << "MetMyGEOS::Source_read_data: (" << ndims 
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
                     std::cerr << "MetMyGEOS::Source_read_data: (" << ndims << "D):  Read all zeroes! " <<  std::endl;
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


/// 3D
void MetMyGEOS::Source_getvar(const std::string& quantity, const double time, GridLatLonField3D* grid3d )
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
           std::cerr << "MetMyGEOS::Source_getvar: (3D)  variable <<" << quantity << ">> is var_id " << var_id << std::endl;
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
           std::cerr << "MetMyGEOS::Source_getvar: (3D)   variable id " <<  var_id << " has type=" << var_type << ", " << nvdims << " dims, " 
           << var_nattrs << " attrs" << std::endl;
        }

        if ( dbug > 4 ) {
           std::cerr << "MetMyGEOS::Source_getvar: (3D)   variable id " <<  var_id << std::endl;
        }
        
        
        // copy global string attributes into the object (in case it gets cached)
        for ( gs_iter=gattr_strings.begin(); gs_iter !=gattr_strings.end(); gs_iter++ ) {
           std::string aname = gs_iter->first;
           std::string aval = gs_iter->second;
           grid3d->set_attribute( aname, aval ); 
        }
        
        // read the variable attributes here, esp. bad-or-missing-data flag
        Source_getattrs(var_id, var_nattrs, grid3d);
        
        // get the variable's dimensions
        Source_checkdims( nvdims, dimids );
        
        // find the index of the time snapshot that we want
        tindex = Source_findtime( quantity, time );
        if ( tindex == -1 ) {
           throw(badTimeNotFound());
        }

         nlons = hgrid.nLons;
         nlats = hgrid.nLats;
         nzs = vgrid.nLevs;
        //- std::cerr << "       counts = " << nlons << ", " << nlats << ", " << nzs  << std::endl;

        // set up the grid dimensions for the data object
        xlons = new std::vector<real>;
        xlats = new std::vector<real>;
        query_hgrid( hgrid, *xlons, *xlats );
        xnlons = xlons->size();
        xnlats = xlats->size();
        
        xzs = new std::vector<real>;
        query_vgrid( vgrid, *xzs, vq, vu );
        xnzs = xzs->size();
        grid3d->set_vertical(vq);
        if ( vu == "mb" || vu == "hPa" ) {
           vscale = 100.0;
           voffset = 0.0;
        } else {
           vscale = 1.0;
           voffset = 0.0;
        }      
        grid3d->set_vunits(vu, vscale, voffset);
        
        if ( nlons != xnlons || nlats != xnlats || nzs != xnzs ) {
           if ( dbug > 0 ) {
              std::cerr << "MetMyGEOS::Source_getvar: (3D)         Dims Mismatch:  " << nlons << "/" << xnlons 
              << ", " << nlats << "/" << xnlats 
              << ", " << nzs << "/" << xnzs  << std::endl;
              std::cerr << "MetMyGEOS::Source_getvar: (3D)  zs=";
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
               xlons->push_back( hgrid.startLon + hgrid.deltaLon*(vstarts[3] + i*vstride[3]) );
           }    
           xlats->clear();
           for (int i=0; i<vcounts[2]; i++ ) {
               xlats->push_back( hgrid.startLat + hgrid.deltaLat*(vstarts[2] + i*vstride[2]) );
           }    
           
           nlons = xlons->size();
           nlats = xlats->size();
        } 

        // read the data values here
        // how we read in the attribute value depends on the value type
        switch (var_type) {
        case NC_FLOAT:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_getvar: (3D)         Reading " << nlons*nlats*nzs  << " floats for variable " << quantity << std::endl;
             }
          
             xdata = new std::vector<real>;
             Source_read_data_floats( *xdata, var_id, 3, vstarts, vcounts, vstride );
           
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_getvar: (3D)   finished reading data! " <<  std::endl;
             }

           break;
        default:
           if (dbug > 0) {
              std::cerr << "MetMyGEOS::Source_getvar: (3D)  unexpected var data type " << var_type << " for " << quantity << std::endl;
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
void MetMyGEOS::Source_getvar(const std::string& quantity, const double time, GridLatLonFieldSfc* grid2d )
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
           std::cerr << "MetMyGEOS::Source_getvar: (Sfc) variable <<" << quantity << ">> is var_id " << var_id << std::endl;
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
           std::cerr << "MetMyGEOS::Source_getvar: (Sfc) variable id " <<  var_id << " has type=" << var_type << ", " << nvdims << " dims, " 
           << var_nattrs << " attrs" << std::endl;
        }

        if ( dbug  > 4) {
           std::cerr << "MetMyGEOS::Source_getvar: (Sfc) variable id " <<  var_id << std::endl;
        }
        
        
        // copy global string attributes into the object (in case it gets cached)
        for ( gs_iter=gattr_strings.begin(); gs_iter !=gattr_strings.end(); gs_iter++ ) {
           std::string aname = gs_iter->first;
           std::string aval = gs_iter->second;
           grid2d->set_attribute( aname, aval ); 
        }
        
        // read the variable attributes here, esp. bad-or-missing-data flag
        Source_getattrs(var_id, var_nattrs, grid2d);
        
        // get the variable's dimensions
        Source_checkdims( nvdims, dimids );
        
        // find the index of the time snapshot that we want
        tindex = Source_findtime( quantity, time );
        if ( tindex == -1 ) {
           throw(badTimeNotFound());
        }


        nlons = hgrid.nLons;
        nlats = hgrid.nLats;
        nzs   = vgrid.nLevs;
        //- std::cerr << "       counts = " << nlons << ", " << nlats << ", " << nzs  << std::endl;

        // set up the grid dimensions for the data object
        xlons = new std::vector<real>;
        xlats = new std::vector<real>;
        query_hgrid( hgrid, *xlons, *xlats );
        xnlons = xlons->size();
        xnlats = xlats->size();
        
        if ( nlons != xnlons || nlats != xnlats ) {
           if ( dbug > 0 ) {
              std::cerr << "MetMyGEOS::Source_getvar: (Sfc)       Dims Mismatch:  " << nlons << "/" << xnlons 
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
               xlons->push_back( hgrid.startLon + hgrid.deltaLon*(vstarts[2] + i*vstride[2]) );
           }    
           xlats->clear();
           for (int i=0; i<vcounts[1]; i++ ) {
               xlats->push_back( hgrid.startLat + hgrid.deltaLat*(vstarts[1] + i*vstride[1]) );
           }    
           
           nlons = xlons->size();
           nlats = xlats->size();
        } 

        // read the data values here
        // how we read in the attribute value depends on the value type
        switch (var_type) {
        case NC_FLOAT:
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_getvar: (Sfc)       Reading " << nlons*nlats*nzs  << " floats for variable " << quantity << std::endl;
             }   
             
             xdata = new std::vector<real>;
             Source_read_data_floats( *xdata, var_id, 2, vstarts, vcounts, vstride );
             
             if ( dbug > 5 ) {
                std::cerr << "MetMyGEOS::Source_getvar: (Sfc) finished reading data! " <<  std::endl;
             }

           break;
        default:
           if (dbug > 5) {
              std::cerr << "MetMyGEOS::Source_getvar: (Sfc) unexpected var data type " << var_type << " for " << quantity << std::endl;
           }
           throw(badNetcdfError(NC_EBADTYPE));   
        } 

        
        grid2d->load( *xlons, *xlats, *xdata );
    
        delete xlons;
        delete xlats;
        delete xdata;
        

}


// 3D
void MetMyGEOS::readSource( const std::string& quantity, const std::string time, GridLatLonField3D* grid3d )
{
     double mtime;
     double preTime, postTime;
     int ndims;
     double timex;
     GridLatLonField3D* preData;
     GridLatLonField3D* postData;
     std::string ctime1, ctime2;
     GridLatLonField3D::iterator src1, src2, dest;
     real bad;
     
     
     init();
         
     mtime = cal2Time(time);
     grid3d->set_time( mtime, time );
     grid3d->set_quantity(quantity);
    
    
     // subtle point here: bracket() needs to know the data spacing,
     // by looking at what time characteristics are
     // available for this quantity, but NOT by opening
     // a remote file. The setup() call below goes a step further.
     // This is so that we can tell here whether we will need one
     // file read or two (and thus potentially two different setup() calls,
     // if the two times belong to two different remote files/urls.
     if ( bracket( quantity, mtime, &preTime, &postTime ) ) { 
        
        // This sets, among other things, test_url, which is used to
        // open the remote file below. Preferably, no new url
        // will be opened at this pont. But for some data sources,
        // setup() might open the new file in the course of looking
        // for a file that contains the requested time (e.g., when 
        // ultiple forecast files are available on the remote end).
        // setup() throws an error if there is no such quantity
        ndims = setup( quantity, time );
     
        // get the units from the configuration.
        // this may be overridden later by reading from the file
        grid3d->set_units( ds[test_dsrc].units );
     
        if ( dbug > 3 ) {     
           std::cerr << "MetMyGEOS::readSource: (3D) quantity=" << quantity << std::endl;
           std::cerr << "MetMyGEOS::readSource: (3D) time=" << time << std::endl;
           std::cerr << "MetMyGEOS::readSource: (3D) test_dsrc =" << test_dsrc << std::endl;
           ds[test_dsrc].dump(11);
        }
           
//-        timex = cal2Time( time );
        timex = mtime;
     
        //  open the URL (actually, this opens the URL only if 
        //  we do not already have it open.
        Source_open();
             
        grid3d->set_fillval( fillval );
        
        // read the snapshot of data  
        Source_getvar( quantity, timex,  grid3d );
     
   } else {
       // the desired time is bracketed by two data set snapshot times
       // read in two data fields and interpolate. (Sigh)

       if ( dbug > 4 ) {
          std::cerr << "MetMyGEOS::readSource: (3D) bracketted time: getting " << preTime << " to " << postTime << ", or ";
       }
       preData = dynamic_cast<GridLatLonField3D*>(grid3d->duplicate());
       ctime1 =  time2Cal(preTime);
       if ( dbug > 4 ) {
          std::cerr << ctime1 << " to ";
       }
       preData->set_time( preTime, ctime1 );
       readSource( quantity, ctime1, preData );
       
       postData = dynamic_cast<GridLatLonField3D*>(grid3d->duplicate());
       ctime2 =  time2Cal(postTime);
       if ( dbug > 4 ) {
          std::cerr << ctime2 << std::endl;
       }      
       postData->set_time( postTime, ctime2 );
       readSource( quantity, ctime2, postData );
   
       // grid3D is empty of data, dimensions, and even some metadata
       // preData and postData have all of that
       // so let's give grid3d what it should have read
       *grid3d = *preData;
       // except set the time back to the target time
       grid3d->set_time( mtime, time );

       grid3d->set_units( preData->units() );

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
void MetMyGEOS::readSource( const std::string& quantity, const std::string time, GridLatLonFieldSfc* grid2d )
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
        
        // ok, the two bracketing times are the same, so use the Pre
        
        // this sets, among other things, test_url 
        // throws an error if there is no such quantity
        // std::cerr << "about to setup for sfc read" << std::endl;
        ndims = setup( quantity, time );
        //- std::cerr << "done with setup for sfc read" << std::endl;
     
        // get the units from the configuration.
        // this may be overridden later by reading from the file
        grid2d->set_units( ds[test_dsrc].units );
     
        if ( dbug > 3 ) {
           std::cerr << "MetMyGEOS::readSource: (Sfc) quantity=" << quantity << std::endl;
           std::cerr << "MetMyGEOS::readSource: (Sfc) ctime=" << time << std::endl;
           std::cerr << "MetMyGEOS::readSource: (Sfc) mtime=" << mtime << std::endl;
           std::cerr << "MetMyGEOS::readSource: (3D) test_dsrc =" << test_dsrc << std::endl;
           ds[test_dsrc].dump(11);
        }
     
//-        timex = cal2Time( time );
        timex = mtime;
     
        // open the URL, if it is not already open
        Source_open();
     
        // populate some attributes of the grid
        grid2d->set_fillval( fillval );
     
        //- std::cerr << "about to getvar " << quantity << std::endl;
        Source_getvar( quantity, timex,  grid2d );
     
     } else {
        // the desired time is bracketed by two data set snapshot times
        // read in two data fields and interpolate. (Sigh)

        if ( dbug > 4 ) {
           std::cerr << "MetMyGEOS::readSource: (Sfc) bracketted time: getting " << preTime << " to " << postTime << ", or ";
        }
        preData = dynamic_cast<GridLatLonFieldSfc*>(grid2d->duplicate());
        ctime1 =  time2Cal(preTime);
        if ( dbug > 4 ) {
           std::cerr << ctime1 << " to ";
        }
        preData->set_time( preTime, ctime1 );
        // (careful! note the recursion here)
        readSource( quantity, ctime1, preData );
       
        postData = dynamic_cast<GridLatLonFieldSfc*>(grid2d->duplicate());
        ctime2 =  time2Cal(postTime);
        if ( dbug > 4 ) {
           std::cerr << ctime2 << std::endl;
        }
        postData->set_time( postTime, ctime2 );
        // (careful! note the recursion here)
        readSource( quantity, ctime2, postData );
   
        // grid2d is empty of data, dimensions, and even some metadata
        // preData and postData have all of that
        // so let's give grid3d what it should have read
        *grid2d = *preData;
        // except set the time back to the target time
        grid2d->set_time( mtime, time );

        grid2d->set_units( preData->units() );

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

//---------------------------------------------------

// constructor
MetMyGEOS::HGridSpec::HGridSpec()
{
   clear();
}

// destructor
MetMyGEOS::HGridSpec::~HGridSpec()
{
}

// clears the spec
void MetMyGEOS::HGridSpec::clear()
{
    code = "";

    startLon = 0.0;
    endLon = 0.0;
    deltaLon = 1.0;
    nLons = 0;

    startLat = 0.0;
    endLat = 0.0;
    deltaLat = 1.0;
    nLats = 0;

    thin = 1; 
    thin_offset = 0;
    
    given = 0;
}

bool MetMyGEOS::HGridSpec::set( const real* lonstart, const real* lonend, const real* londelta, const int* nlon 
         , const real* latstart, const real* latend, const real* latdelta, const int* nlat)
{
    bool result;
    double xstart;
    double xend;
    double xdelta;
    double xn;
    double ystart;
    double yend;
    double ydelta;
    double yn;
    int nn;
    double qend;
    double rend;
    
    result = false;
    
    if ( lonstart != NULLPTR ) {
       xstart = *lonstart;
       if ( lonend != NULLPTR ) {
          xend = *lonend;
          if ( londelta != NULLPTR ) {
             xdelta = *londelta;
             if ( nlon != NULLPTR ) {
                xn = *nlon; // note: converting int to double here
             } else {
                // No nlon is given, but the others are present
                if ( xdelta > 0 ) {
                   xn = ( xend - xstart )/xdelta + 1.0; 
                } else {
                   xn = 1.0;
                }
             }
             result = true;
          } else {
             // no londelta is given
             if ( nlon != NULLPTR ) {
                xn = *nlon;
                if ( xn > 0 ) {
                   // compute xdelta from xstart, xdelta, and xn
                   xdelta = ( xend - xstart)/( xn - 1.0);
                   result = true;
                } else {
                   // nlon was given but has a bad value
                   
                }
             } else {
                // neither londelta nor nlon was given. 
                // this is ok only if lonstart == lonend. Otherwise
                // we have an under-constrained time spec
                if ( xstart == xend ) {
                   // (yes, ordinarily it is a really bad idea to compare doubles, but it's ok here)
                   xn = 1.0;
                   xdelta = 0.0;
                   result = true;
                }
             }
          }
       } else {
          // lonstart, but no lonend
          if ( londelta != NULLPTR ) {
             xdelta = *londelta;
             
             if ( nlon != NULLPTR ) {
                xn = *nlon;
                // lonstart, londelta, nlon all given.
                // compute xend
                xend = xstart + xdelta*(xn - 1.0);
                result = true;
             } else {
                // only lonstart and londelta given.
                // only if londelta is 0 is this OK. otherwise
                // we have an under-constrained time spec
                if ( xdelta == 0.0 ) {
                   xend = xstart;
                   xn = 1.0;
                   result = true;
                }
             }
          } else {
             // no lonend or londelta
             // nlon had better be 1 or else we have an under-constrained time spec
             if ( nlon != NULLPTR ) {
                xn = *nlon;
                if ( *nlon == 1 ) {
                   xend = xstart;
                   xdelta = 0.0;
                   result = true;
                }
             }
          }
       }
    
    } else {
       // no lonstart.

       if ( lonend != NULLPTR ) {
          xend = *lonend;
          
          if ( londelta != NULLPTR ) {
             xdelta = *londelta;
       
             if ( nlon != NULLPTR ) {
                xn = *nlon;
                xstart = xend - xdelta*(xn - 1.0);
             } else {
                if ( xdelta == 0.0 ) {
                   xn = 1.0;
                   xstart = xend;
                   result = true;
                }
             }
          } else {
             
             if ( nlon != NULLPTR ) {
                xn = *nlon;
                
                if ( *nlon == 1 ) {
                   xdelta = 0.0;
                   xstart = xend;
                   result = true;
                }
             } else {
                // no lonstart or londelta or nlon--give up
             }
          }   

       } else {
           // no lonstart or lonend--hopeless
       }
    
    }
    
    if ( latstart != NULLPTR ) {
       ystart = *latstart;
       if ( latend != NULLPTR ) {
          yend = *latend;
          if ( latdelta != NULLPTR ) {
             ydelta = *latdelta;
             if ( nlat != NULLPTR ) {
                yn = *nlat; // note: converting int to double here
             } else {
                // No nlat is given, but the others are present
                if ( ydelta > 0 ) {
                   yn = ( yend - ystart )/ydelta + 1.0; 
                } else {
                   yn = 1.0;
                }
                result = true;
             }
          } else {
             // no latdelta is given
             if ( nlat != NULLPTR ) {
                yn = *nlat;
                if ( yn > 0 ) {
                   // compute ydelta from ystart, ydelta, and yn
                   ydelta = ( yend - ystart)/(yn - 1.0);
                   result = true;
                } else {
                   // nlat was given but has a bad value
                   
                }
             } else {
                // neither latdelta nor nlat was given. 
                // thi sis ok only if latstart == latend. Otherwise
                // we have an under-constrained time spec
                if ( ystart == yend ) {
                   // (yes, ordinarily it is a really bad idea to compare doubles, but it's ok here)
                   yn = 1.0;
                   ydelta = 0.0;
                   result = true;
                }
             }
          }
       } else {
          // latstart, but no latend
          if ( latdelta != NULLPTR ) {
             ydelta = *latdelta;
             
             if ( nlat != NULLPTR ) {
                yn = *nlat;
                // latstart, latdelta, nlat all given.
                // compute yend
                yend = ystart + ydelta*(yn - 1.0);
                result = true;
             } else {
                // only latstart and latdelta given.
                // only if latdelta is 0 is this OK. otherwise
                // we have an under-constrained time spec
                if ( ydelta == 0.0 ) {
                   yend = ystart;
                   yn = 1.0;
                   result = true;
                }
             }
          } else {
             // no latend or latdelta
             // nlat had better be 1 or else we have an under-constrained time spec
             if ( nlat != NULLPTR ) {
                yn = *nlat;
                if ( *nlat == 1 ) {
                   yend = ystart;
                   ydelta = 0.0;
                   result = true;
                }
             }
          }
       }
    
    } else {
       // no latstart.

       if ( latend != NULLPTR ) {
          yend = *latend;
          
          if ( latdelta != NULLPTR ) {
             ydelta = *latdelta;
       
             if ( nlat != NULLPTR ) {
                yn = *nlat;
                ystart = yend - ydelta*(yn - 1.0);
             } else {
                if ( ydelta == 0.0 ) {
                   yn = 1.0;
                   ystart = yend;
                   result = true;
                }
             }
          } else {
             
             if ( nlat != NULLPTR ) {
                yn = *nlat;
                
                if ( *nlat == 1 ) {
                   ydelta = 0.0;
                   ystart = yend;
                   result = true;
                }
             } else {
                // no latstart or latdelta or nlat--give up
             }
          }   

       } else {
           // no latstart or latend--hopeless
       }
    
    }
    
    if ( result ) {
       // now check for internal consistency
       qend = xstart + (xn - 1.0)*xdelta;
       // should be within 5 seconds each direction
       rend = ystart + (yn - 1.0)*ydelta;
       if ( ( fabs( qend - xend ) < 5.0/3600.0 )
          &&( fabs( rend - yend ) < 5.0/3600.0 ) ) {
          
          startLon = xstart;
          endLon = xend;
          deltaLon = xdelta;
          nLons =  xn + 0.5;
          
          startLat = ystart;
          endLat = yend;
          deltaLat = ydelta;
          nLats =  yn + 0.5;
          
          given = 0;
          if ( lonstart != NULLPTR ) {
             given = given | 0x01;
          }   
          if ( lonend != NULLPTR ) {
             given = given | 0x02;
          }   
          if ( londelta != NULLPTR ) {
             given = given | 0x04;
          }   
          if ( nlon != NULLPTR ) {
             given = given | 0x08;
          }   
          if ( latstart != NULLPTR ) {
             given = given | 0x10;
          }   
          if ( latend != NULLPTR ) {
             given = given | 0x20;
          }   
          if ( latdelta != NULLPTR ) {
             given = given | 0x40;
          }   
          if ( nlat != NULLPTR ) {
             given = given | 0x80;
          }   
          
       } else {
          result = false;
       }

    }
    
    return result;


}

bool MetMyGEOS::HGridSpec::test( const HGridSpec& cmp ) const
{
    bool result;
    double threshold;
    
    result = false;
    
    threshold = 5.0/3600.0;
    
    if ( ( fabs( startLon - cmp.startLon ) < threshold )
      && ( fabs(  endLon  - cmp.endLon   ) < threshold )
      && ( fabs( deltaLon - cmp.deltaLon ) < threshold )
      && ( nLons == cmp.nLons )  
      && ( fabs( startLat - cmp.startLat ) < threshold )
      && ( fabs(  endLat  - cmp.endLat   ) < threshold )
      && ( fabs( deltaLat - cmp.deltaLat ) < threshold )
      && ( nLats == cmp.nLats ) ) {
     
      result = true; 
      
    }   
    
    
    return result;
}




// constructor
MetMyGEOS::VGridSpec::VGridSpec()
{
   clear();
}

// destructor
MetMyGEOS::VGridSpec::~VGridSpec()
{
}

// clears the spec
void MetMyGEOS::VGridSpec::clear()
{
   code = "";
   quant = "";
   units = "";
   mksScale = 1.0;
   mksOffset = 0.0;
   levs.clear();
   nLevs = 0;

}

bool MetMyGEOS::VGridSpec::set( const std::string& vcode, const std::string& quantity, const std::string& qunits, real* scale, real* offset, std::vector<real>* levels )
{
    bool result;
   
    clear();
    
    code = vcode;
    quant = quantity;
    units = qunits;

    if ( code == "P" ) {
       // pressure coordinates
       
       if ( scale == NULLPTR ) {
          
          if ( units == "" ) {
              units = "hPa";
              mksScale = 100.0;
              mksOffset = 0.0;                       
          } else if ( units == "hPa" || units == "mb" ) {
              mksScale = 100.0;
              mksOffset = 0.0;             
          } else if ( units == "Pa" ) {       
              mksScale = 1.0;
              mksOffset = 0.0;   
          } 
       
       } else {
       
          mksScale = *scale;
          
          if ( offset == NULLPTR ) {
             mksOffset = 0.0;
          } else {
             mksOffset = *offset;
          }
       }

       if ( levels == NULLPTR ) {
          
          levs.clear();
          
          // default pressure levels
          levs.push_back( 1000.  );    
          levs.push_back(  975.  );    
          levs.push_back(  950.  );    
          levs.push_back(  925.  );    
          levs.push_back(  900.  );    
          levs.push_back(  875.  );    
          levs.push_back(  850.  );    
          levs.push_back(  825.  );    
          levs.push_back(  800.  );    
          levs.push_back(  775.  );    
          levs.push_back(  750.  );    
          levs.push_back(  725.  );    
          levs.push_back(  700.  );    
          levs.push_back(  650.  );    
          levs.push_back(  600.  );    
          levs.push_back(  550.  );    
          levs.push_back(  500.  );    
          levs.push_back(  450.  );    
          levs.push_back(  400.  );    
          levs.push_back(  350.  );    
          levs.push_back(  300.  );    
          levs.push_back(  250.  );    
          levs.push_back(  200.  );    
          levs.push_back(  150.  );    
          levs.push_back(  100.  );    
          levs.push_back(   70.  );    
          levs.push_back(   50.  );    
          levs.push_back(   40.  );    
          levs.push_back(   30.  );    
          levs.push_back(   20.  );    
          levs.push_back(   10.  );    
          levs.push_back(    7.  );    
          levs.push_back(    5.  );    
          levs.push_back(    4.  );    
          levs.push_back(    3.  );    
          levs.push_back(    2.  );    
          levs.push_back(    1.  );    
          levs.push_back(    0.7 );    
          levs.push_back(    0.5 );    
          levs.push_back(    0.4 );    
          levs.push_back(    0.3 );    
          levs.push_back(    0.1 );    
       
       } else {
       
          levs = *levels;
       
       }
       
       nLevs = levs.size();

    } else if ( code == "P48" ) {
       // pressure coordinates
       
       if ( scale == NULLPTR ) {
          
          if ( units == "" ) {
              units = "hPa";
              mksScale = 100.0;
              mksOffset = 0.0;                       
          } else if ( units == "hPa" || units == "mb" ) {
              mksScale = 100.0;
              mksOffset = 0.0;             
          } else if ( units == "Pa" ) {       
              mksScale = 1.0;
              mksOffset = 0.0;   
          } 
       
       } else {
       
          mksScale = *scale;
          
          if ( offset == NULLPTR ) {
             mksOffset = 0.0;
          } else {
             mksOffset = *offset;
          }
       }

       if ( levels == NULLPTR ) {
          
          levs.clear();
          
          // default pressure levels
          levs.push_back( 1000.  );    
          levs.push_back(  975.  );    
          levs.push_back(  950.  );    
          levs.push_back(  925.  );    
          levs.push_back(  900.  );    
          levs.push_back(  875.  );    
          levs.push_back(  850.  );    
          levs.push_back(  825.  );    
          levs.push_back(  800.  );    
          levs.push_back(  775.  );    
          levs.push_back(  750.  );    
          levs.push_back(  725.  );    
          levs.push_back(  700.  );    
          levs.push_back(  650.  );    
          levs.push_back(  600.  );    
          levs.push_back(  550.  );    
          levs.push_back(  500.  );    
          levs.push_back(  450.  );    
          levs.push_back(  400.  );    
          levs.push_back(  350.  );    
          levs.push_back(  300.  );    
          levs.push_back(  250.  );    
          levs.push_back(  200.  );    
          levs.push_back(  150.  );    
          levs.push_back(  100.  );    
          levs.push_back(   70.  );    
          levs.push_back(   50.  );    
          levs.push_back(   40.  );    
          levs.push_back(   30.  );    
          levs.push_back(   20.  );    
          levs.push_back(   10.  );    
          levs.push_back(    7.  );    
          levs.push_back(    5.  );    
          levs.push_back(    4.  );    
          levs.push_back(    3.  );    
          levs.push_back(    2.  );    
          levs.push_back(    1.  );    
          levs.push_back(    0.7 );    
          levs.push_back(    0.5 );    
          levs.push_back(    0.4 );    
          levs.push_back(    0.3 );    
          levs.push_back(    0.2 );    
          levs.push_back(    0.1 );    
          levs.push_back(    0.07 );    
          levs.push_back(    0.05 );    
          levs.push_back(    0.04 );    
          levs.push_back(    0.03 );    
          levs.push_back(    0.02 );    
       
       } else {
       
          levs = *levels;
       
       }
       
       nLevs = levs.size();

    } else if ( code == "H" ) {
       // potential temperature coordinates
       
       if ( scale == NULLPTR ) {
          
          if ( units == "" ) {
              units = "K";
              mksScale = 1.0;
              mksOffset = 0.0;                       
          } 
       
       } else {
       
          mksScale = *scale;
          
          if ( offset == NULLPTR ) { 
             mksOffset = 0.0;
          } else {
             mksOffset = *offset;
          }
       }

       if ( levels == NULLPTR ) {
          
          levs.clear();
          
          // default theta levels
          levs.push_back(  287.0 );   
          levs.push_back(  288.0 );   
          levs.push_back(  289.0 );   
          levs.push_back(  290.0 );   
          levs.push_back(  291.0 );   
          levs.push_back(  293.0 );   
          levs.push_back(  294.0 );   
          levs.push_back(  296.0 );   
          levs.push_back(  297.0 );   
          levs.push_back(  298.0 );   
          levs.push_back(  299.0 );   
          levs.push_back(  300.0 );   
          levs.push_back(  302.0 );   
          levs.push_back(  304.0 );   
          levs.push_back(  307.0 );   
          levs.push_back(  310.0 );   
          levs.push_back(  313.0 );   
          levs.push_back(  316.0 );   
          levs.push_back(  319.0 );   
          levs.push_back(  322.0 );   
          levs.push_back(  328.0 );   
          levs.push_back(  341.0 );   
          levs.push_back(  349.0 );   
          levs.push_back(  360.0 );   
          levs.push_back(  373.0 );   
          levs.push_back(  393.0 );   
          levs.push_back(  414.0 );   
          levs.push_back(  437.0 );   
          levs.push_back(  459.0 );   
          levs.push_back(  485.0 );   
          levs.push_back(  501.0 );   
          levs.push_back(  519.0 );   
          levs.push_back(  540.0 );   
          levs.push_back(  567.0 );   
          levs.push_back(  601.0 );   
          levs.push_back(  644.0 );   
          levs.push_back(  691.0 );   
          levs.push_back(  745.0 );   
          levs.push_back(  805.0 );   
          levs.push_back(  854.0 );   
          levs.push_back(  909.0 );   
          levs.push_back(  963.0 );   
          levs.push_back( 1022.0 );   
          levs.push_back( 1063.0 );   
          levs.push_back( 1108.0 );   
          levs.push_back( 1167.0 );   
          levs.push_back( 1231.0 );   
          levs.push_back( 1327.0 );   
          levs.push_back( 1426.0 );   
          levs.push_back( 1510.0 );   
          levs.push_back( 1600.0 );   
          levs.push_back( 1708.0 );   
          levs.push_back( 1784.0 );   
          levs.push_back( 1877.0 );   
          levs.push_back( 1969.0 );   
          levs.push_back( 2048.0 );   
          levs.push_back( 2130.0 );   
          levs.push_back( 2178.0 );   
          levs.push_back( 2232.0 );   
          levs.push_back( 2293.0 );   
          levs.push_back( 2356.0 );   
          levs.push_back( 2441.0 );   
          levs.push_back( 2500.0 );   
          levs.push_back( 2600.0 );   
          levs.push_back( 2764.0 );   
          levs.push_back( 2967.0 );   
          levs.push_back( 3100.0 );   
         
       } else {
       
          levs = *levels;
       
       }
       
       nLevs = levs.size();
    
    } else if ( code == "z" || code == "a" ) {
       // pressure altitude or altitude coordinates
       
       if ( scale == NULLPTR ) {
          
          if ( units == "" ) {
              units = "km";
              mksScale = 1000.0;
              mksOffset = 0.0;                       
          } 
       
       } else {
       
          mksScale = *scale;
          
          if ( offset == NULLPTR ) { 
             mksOffset = 0.0;
          } else {
             mksOffset = *offset;
          }
       }

       if ( levels == NULLPTR ) {
          
          levs.clear();
          
          // default altitude set, chosen to match pressure altitudes 
          // computed from the standard pressure levels
          levs.push_back(  0.109767 );  
          levs.push_back(  0.320891 );  
          levs.push_back(  0.537503 );  
          levs.push_back(  0.759888 );  
          levs.push_back(  0.988369 );  
          levs.push_back(  1.21819  ); 
          levs.push_back(  1.45441  ); 
          levs.push_back(  1.69767  ); 
          levs.push_back(  1.94842  ); 
          levs.push_back(  2.20230  ); 
          levs.push_back(  2.46325  ); 
          levs.push_back(  2.73306  ); 
          levs.push_back(  3.01204  ); 
          levs.push_back(  3.58772  ); 
          levs.push_back(  4.20438  ); 
          levs.push_back(  4.86374  ); 
          levs.push_back(  5.57129  ); 
          levs.push_back(  6.34065  ); 
          levs.push_back(  7.18340  ); 
          levs.push_back(  8.11584  ); 
          levs.push_back(  9.16200  ); 
          levs.push_back( 10.3595   );
          levs.push_back( 11.7840   );
          levs.push_back( 13.6084   );
          levs.push_back( 16.1797   );
          levs.push_back( 18.4416   );
          levs.push_back( 20.5767   );
          levs.push_back( 21.9997   );
          levs.push_back( 23.8489   );
          levs.push_back( 26.4818   );
          levs.push_back( 31.0547   );
          levs.push_back( 33.4541   );
          levs.push_back( 35.7776   );
          levs.push_back( 37.3548   );
          levs.push_back( 39.4309   );
          levs.push_back( 42.4412   );
          levs.push_back( 47.8201   );
          levs.push_back( 50.6458   );
          levs.push_back( 53.2829   );
          levs.push_back( 54.9943   );
          levs.push_back( 57.1530   );
          levs.push_back( 64.9467   );
    
       } else {
       
          levs = *levels;
       
       }
       
       nLevs = levs.size();
       
    } else if ( code == "L" ) {
       // model level coordinates

       if ( scale == NULLPTR ) {
          
          if ( units == "" ) {
              units = "";
              mksScale = 1.0;
              mksOffset = 0.0;                       
          } 
       
       } else {
       
          mksScale = *scale;
          
          if ( offset == NULLPTR ) { 
             mksOffset = 0.0;
          } else {
             mksOffset = *offset;
          }
       }

       if ( levels == NULLPTR ) {
          
          levs.clear();
          
          // default level set
          for ( int i=0; i<72; i++ ) {
             levs.push_back(  i );  
          }
                   
       } else {
       
          levs = *levels;
       
       }
       
       nLevs = levs.size();
    
    } else if ( code == "L1" ) {
       // model level coordinates

       if ( scale == NULLPTR ) {
          
          if ( units == "" ) {
              units = "";
              mksScale = 1.0;
              mksOffset = 0.0;                       
          } 
       
       } else {
       
          mksScale = *scale;
          
          if ( offset == NULLPTR ) { 
             mksOffset = 0.0;
          } else {
             mksOffset = *offset;
          }
       }

       if ( levels == NULLPTR ) {
          
          levs.clear();
          
          // default level set
          for ( int i=1; i<=72; i++ ) {
             levs.push_back(  i );  
          }
                   
       } else {
       
          levs = *levels;
       
       }
       
       nLevs = levs.size();
    
    } else if ( code == "2" ) {
       // 2D--no vertical coordinate

       if ( scale == NULLPTR ) {
          
          if ( units == "" ) {
              units = "";
              mksScale = 1.0;
              mksOffset = 0.0;                       
          } 
       
       } else {
       
          mksScale = *scale;
          
          if ( offset == NULLPTR ) { 
             mksOffset = 0.0;
          } else {
             mksOffset = *offset;
          }
       }

       if ( levels == NULLPTR ) {
          
          levs.clear();
          
          levs.push_back( 0.0 );  
                   
       } else {
        
          if ( levels->size() != 1 ) {
             std::cerr << "2D quantity was given a vector of vertical coordinate values?!" << std::endl;
             throw(badVerticalCoord());          
          }
       
          levs = *levels;
       
       }
       
       nLevs = levs.size();
    
    } else {
   
       std::cerr << "unknown vertical coordniate code:" << code << std::endl;
       throw(badVerticalCoord());
    
    }
    
    if ( nLevs < 10 && code != "2" ) {
       std::cerr << "3D quantity has fewer than ten vertical coordinate values" << std::endl;
       throw(badVerticalCoord());          
    }
   
    result = true;
  
    return result;

}

bool MetMyGEOS::VGridSpec::test( const VGridSpec& cmp ) const
{
    bool result;
    
    result = false;
    
    if ( code == cmp.code ) {
       if ( quant == cmp.quant ) {
          // note: th eunits do NOT have to be he same
          
          if ( nLevs == cmp.nLevs ) {
          
             result = true;
             
             for ( int i = 0; i < nLevs; i++ ) {
                 if ( ABS( levs[i] - cmp.levs[i] ) > 0.000001 ) {
                    result = false;
                    break;
                 }
             }
          }
       }
    }
    
    
    return result;

}

// constructor
MetMyGEOS::TGridSpec::TGridSpec()
{
   clear();
}

// destructor
MetMyGEOS::TGridSpec::~TGridSpec()
{
}

// clears the spec
void MetMyGEOS::TGridSpec::clear()
{
   tave = "";
   start = 0.0;
   end = 0.0;
   delta = 1.0;
   n = 0;
   toff = 0.0;
   given = 0;
}

bool MetMyGEOS::TGridSpec::set( double tstart, double tnext, double tdelta )
{
    bool result;
    double xstart;
    double xend;
    double xdelta;
    double xn;
    int nn;
    double qend;
    
    result = true;
    
    start = tstart;
    next = tnext;
    delta = tdelta;
    if ( next > start ) {
       if ( delta == 0.0 ) {
          if ( start != next ) {
             delta = next - start;
          }
       }
    } else if ( next == start ) {
       if ( delta > 0.0 ) {
          next = start + delta;
       }
    }

    
    return result;

}

bool MetMyGEOS::TGridSpec::test( const TGridSpec& cmp ) const
{
    bool result;
    double threshold;
    int n1, n2;
    double s1, s2;
    double e1, e2;
    double d1, d2;
    
    result = false;
    
    // times must match within 10 seconds
    threshold = 10.0/3600.0/24.0;
    
    // the start times must match
    if ( fabs( start - cmp.start ) < threshold ) {
       if ( (delta != 0.0) && (cmp.delta != 0.0) ) {
          if ( fabs( delta - cmp.delta ) < threshold ) {
             if ( fabs( next - cmp.next ) < threshold ) {   
                result = true;;
             }
          }
       } else {
          // at least one of the deltas is zero, which matches anything
          result = true;
       }
    }
       
/*
    // the number of times must match, unless one of them is zero
    if ( (n == cmp.n) || (n == 0) || (cmp.n == 0) ) {
    
       // the start times must match
       if ( fabs( start - cmp.start ) < threshold ) {
       
          // if there is more than one time tick...
          if ( (n > 1 ) || (cmp.n > 1) ) {
       
             // the deltas should match
             result = ( fabs( delta - cmp.delta ) < threshold );
             
             // no need to match the end times if the others match
             // (note that the components have already been screened for internal consistency
             // in the TGridSpec::set() method.)
       
          } else {
             // only one time tick
             // and the start times already match
             result = true;
          }
       }
    }
*/
    
    return result;
}


double MetMyGEOS::Source_time_nativeTo1900( double nativeTime ) const
{
      //return nativeTime/24.0/3600.0  + 33969.000;
      return nativeTime/24.0/60.0  + time_zero;
};

double MetMyGEOS::Source_deltime_nativeTo1900( double nativeDelTime ) const
{
      return nativeDelTime/24.0/60.0;
};

void MetMyGEOS::query_hgrid( const HGridSpec& qhgrid, std::vector<real>& lons, std::vector<real>& lats ) const
{
     lons.clear();
     for (int i=0; i < qhgrid.nLons; i++ ) {
         lons.push_back( qhgrid.startLon + i*qhgrid.deltaLon );
     }
     lons[ qhgrid.nLons - 1 ] = qhgrid.endLon;

     lats.clear();
     for (int i=0; i < qhgrid.nLats; i++ ) {
         lats.push_back( qhgrid.startLat + i*qhgrid.deltaLat );
     }
     lats[ qhgrid.nLats - 1 ] = qhgrid.endLat;

}

void MetMyGEOS::query_vgrid( const VGridSpec& qvgrid, std::vector<real>& levels, std::string& q, std::string& u  ) const
{
     levels = qvgrid.levs;
     q = qvgrid.quant;
     u = qvgrid.units;

}

