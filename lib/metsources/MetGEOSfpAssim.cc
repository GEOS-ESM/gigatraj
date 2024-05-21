
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
MetGEOSfpAssim::MetGEOSfpAssim() : MetGEOSfp() 
{    

   whichSrc = -1;
   forceSrc = -1;

}

MetGEOSfpAssim::MetGEOSfpAssim( std::string& date ) : MetGEOSfp() 
{
    
   whichSrc = -1;
   forceSrc = -1;
   
   basetime = cal.day1900( date );

}


MetGEOSfpAssim::MetGEOSfpAssim( std::string& date, std::string& mrun ) : MetGEOSfp() 
{
    throw(badNoForecasts());    

}



std::string MetGEOSfpAssim::model_run()
{
    throw(badNoForecasts());    
   
    return modelRun;
}

std::string MetGEOSfpAssim::model_run_cal()
{ 
    std::string result;

    throw(badNoForecasts());    

   return result;
}

void MetGEOSfpAssim::set_model_run(std::string &mrun)
{
    throw(badNoForecasts());    
}



int MetGEOSfpAssim::which_src()
{
    return whichSrc;
}

int MetGEOSfpAssim::which_forced_src()
{
    return forceSrc;
}

void MetGEOSfpAssim::set_forced_src( int which )
{
    throw(badNoForecasts());    
}

       



void MetGEOSfpAssim::setOption( const std::string &name, const std::string &value )
{
}

void MetGEOSfpAssim::setOption( const std::string &name, int value )
{
    int tmp;
    
    if ( name == "HorizontalGridThinning" ) {
        set_thinning(value);
    } else if ( name == "HorizontalGridOffset" ) {
        tmp = thinning();
        set_thinning( tmp, value );
    }
    
}

void MetGEOSfpAssim::setOption( const std::string &name, float value )
{
}

void MetGEOSfpAssim::setOption( const std::string &name, double value )
{
}


bool MetGEOSfpAssim::getOption( const std::string &name, std::string &value )
{
   value = "";
   return false;
}


bool MetGEOSfpAssim::getOption( const std::string &name, int &value )
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


bool MetGEOSfpAssim::getOption( const std::string &name, float &value )
{
    bool result = false;
    value = 0.0;
    return result;
}


bool MetGEOSfpAssim::getOption( const std::string &name, double &value )
{
    bool result = false;
    value = 0.0;
    return result;
}






int MetGEOSfpAssim::setup(  const std::string quantity, const std::string &time )
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

       if ( dbug >= 3 ) {
         std::cerr << "MetGEOSfpAssim::setup: testing for " << quantity << " @ " << caltime << std::endl;
       }
       test_quant = quantity;
       test_date = caltime;
       
       // find the variable we are looking for
       status = adir.LookUp( test_quant, desired_hgrid_id, desired_vgrid_id, desired_tspace, desired_tave
            , NULL, NULL, &test_ndims
            , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl );
       if ( status && (strict != 0x07) ) {
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
          status = adir.LookUp( test_quant, use_hgrid, use_vgrid, use_tspace, use_tavg
            , NULL, NULL, &test_ndims
            , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl );
       }
       
       if ( status == 0 ) {
          
          test_url = *newUrl;
          if ( dbug >= 3 ) {
             std::cerr << "MetGEOSfpAssim::setup:  testing was successful.  ndims is " << test_ndims << ", url=<<" << test_url << ">>" << std::endl;
          }
          
          delete newUrl;
       
       } else {
          throw(badDataNotFound());
       }
    }
      
    // std::cerr << " setup returns " << test_ndims << std::endl;
    
    
    return test_ndims;

}    


time_t MetGEOSfpAssim::expiration( const std::string &valid_at )
{
     // the expiration timestamp
     time_t result;

     result = 0.0; // no expirations for assimilation
          
     return result;
}


MetGEOSfpAssim* MetGEOSfpAssim::myNew()
{
   MetGEOSfpAssim *dup;
   
   dup = new MetGEOSfpAssim;
   
   dup->dbug = dbug;
   dup->setPgroup(my_pgroup, my_metproc);
   dup->defineCal( time2Cal(0), basetime );
   dup->maxsnaps = this->maxsnaps;
   dup->setCacheDir( this->diskcachedir );
   dup->modelRun = modelRun;
   
   return dup;
}


bool MetGEOSfpAssim::bracket( const std::string &quantity, double time, double *t1, double *t2)
{
    double prev, next;
    std::string caltime;
    int true_hgrid, true_vgrid, true_tspace, true_tave, true_tbase;
    int status;
    double xtime, ytime, ptime;
    double tbase=0.0;
    double tspace = 24;
    std::vector<std::string> *testquants;
    bool sametime;
    
    if ( dbug > 5 ) {
       std::cerr << "MetGEOSfpAssim::bracket: Bracketing time " << time << " against base " << basetime << std::endl;
    }
    
    // note: for on-the-fly calculations, we need
    // to get the bracket of the quantities that the OTF calculaiton
    // depends on. Here, we will just be using the 0th such quantity.
    // But if the component quantities are on different time ticks
    // this may not be sufficient.
    testquants = new_testQuantity( quantity );
    
    // find the variable we are looking for
    status = adir.LookUp( (*testquants)[0], desired_hgrid_id, desired_vgrid_id, tspace, desired_tave
         , NULL, NULL, NULL 
         , &true_vgrid, &true_hgrid, &true_tspace, &true_tave, &true_tbase, NULL );
    if ( status && ! strict ) {
       status = adir.LookUp( (*testquants)[0], -1, -1, -1, -1 
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
    
    // convert from model seconds to day1900
    ptime = time + basetime;
    // get the day1900 date
    xtime = floor( ptime );
    // convert the time part to intervals/indices
    ytime = (( ptime - xtime)*24.0 - tbase)/tspace; 
    // std::cerr << "  xtime= " << xtime << " , ytime=" << ytime  << std::endl;
    
    // get the datestamp of the previous integral time index
    prev = (floor(ytime)*tspace + tbase)/24.0 + xtime;
    // get the datestamp of the next integral time index
    if ( ABS( prev - ptime )/tspace*24.0 > 1e-2 ) {
       // get the datestamp of the next integral time index
       next = (ceil(ytime)*tspace + tbase)/24.0 + xtime;
       sametime = false;
    } else {
       // no interpolation needed.
       //std::cerr << "BANG: bracketed times are indentical!" << std::endl;
       next = ptime;
       sametime = true;
    }

    if ( dbug > 5 ) {
       std::cerr << "MetGEOSfpAssim::bracket:   Found times " << prev << " and " << next << " using interval " << true_tspace << std::endl;
    }
   
    *t1 = prev-basetime;
    *t2 = next-basetime;

    if ( dbug > 5 ) {
       std::cerr << "MetGEOSfpAssim::bracket:   Translated bracket times to  " << *t1 << " and " << *t2  << std::endl;
    }

    return sametime;

}




