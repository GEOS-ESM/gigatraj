
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

#include "gigatraj/MetGEOSfpFcast.hh"

using namespace gigatraj;

const std::string MetGEOSfpFcast::iam = "MetGEOSfpFcast";


// default constructor
MetGEOSfpFcast::MetGEOSfpFcast() : MetGEOSfp() 
{    

   whichSrc = 1;
   forceSrc = 1;

}

MetGEOSfpFcast::MetGEOSfpFcast( std::string& mrun ) : MetGEOSfp() 
{
    
   whichSrc = 1;
   forceSrc = 1;
   
   modelRun = mrun;


}


MetGEOSfpFcast::MetGEOSfpFcast( std::string& mrun, std::string& date ) : MetGEOSfp() 
{
    
   whichSrc = 1;
   forceSrc = 1;

   modelRun = mrun;
   basetime = cal.day1900( date );

}




std::string MetGEOSfpFcast::model_run()
{
    return modelRun;
}

std::string MetGEOSfpFcast::model_run_cal()
{ 
    std::string result;

    result = "";
    if ( modelRun.length() == 11 ) {
       result = modelRun.substr(0,4)
              + "-" + modelRun.substr(4,2)
              + "-" + modelRun.substr(6,2)
              + "T" + modelRun.substr(9,2)
              + ":00";
   } 
   
   return result;
}

void MetGEOSfpFcast::set_model_run(std::string &mrun)
{
    modelRun = mrun;
}



int MetGEOSfpFcast::which_src()
{
    return whichSrc;
}

int MetGEOSfpFcast::which_forced_src()
{
    return forceSrc;
}

void MetGEOSfpFcast::set_forced_src( int which )
{
    throw(badNoAssim());    
}


       

bool MetGEOSfpFcast::bracket( const std::string &quantity, double time, double *t1, double *t2)
{
    double prev, next;
    std::string caltime;
    int true_hgrid, true_vgrid, true_tspace, true_tave, true_tbase;
    int status;
    double xtime, ytime, ptime;
    double tbase=0.0;
    double tspace = 24;
    std::vector<std::string> *testquants;
    double mtime;
    bool sametime;
    
    if ( debug > 5 ) {
       std::cerr << "MetGEOSfpFcast::bracket: Bracketing time " << time << " against base " << basetime << std::endl;
    }
    
    // convert model time to calendar time
    caltime = time2Cal(time);
    
    // note: for on-the-fly calculations, we need
    // to get the bracket of the quantities that the OTF calculaiton
    // depends on. Here, we will just be using the 0th such quantity.
    // But if the component quantities are on different time ticks
    // this may not be sufficient.
    testquants = new_testQuantity( quantity );
    
    // find the variable we are looking for
    status = fdir.LookUp( (*testquants)[0], desired_hgrid_id, desired_vgrid_id, tspace, desired_tave
         , NULL, NULL, NULL 
         , &true_vgrid, &true_hgrid, &true_tspace, &true_tave, &true_tbase, NULL, modelRun );
    if ( status && ! strict ) {
       status = fdir.LookUp( (*testquants)[0], -1, -1, -1, -1 
         , NULL, NULL, NULL 
         , &true_vgrid, &true_hgrid, &true_tspace, &true_tave, &true_tbase, NULL, modelRun );
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

    if ( debug > 5 ) {
       std::cerr << "MetGEOSfpFcast::bracket:   Found times " << prev << " and " << next << " using interval " << tspace << std::endl;
    }
   
    *t1 = prev-basetime;
    *t2 = next-basetime;

    if ( debug > 5 ) {
       std::cerr << "MetGEOSfpFcast::bracket:   Translated bracket times to  " << *t1 << " and " << *t2  << std::endl;
    }

    return sametime;
}





void MetGEOSfpFcast::setOption( const std::string &name, const std::string &value )
{
}

void MetGEOSfpFcast::setOption( const std::string &name, int value )
{
    int tmp;
    
    if ( name == "HorizontalGridThinning" ) {
        set_thinning(value);
    } else if ( name == "HorizontalGridOffset" ) {
        tmp = thinning();
        set_thinning( tmp, value );
    }
    
}

void MetGEOSfpFcast::setOption( const std::string &name, float value )
{
}

void MetGEOSfpFcast::setOption( const std::string &name, double value )
{
}

bool MetGEOSfpFcast::getOption( const std::string &name, std::string &value )
{
   value = "";
   return false;
}


bool MetGEOSfpFcast::getOption( const std::string &name, int &value )
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


bool MetGEOSfpFcast::getOption( const std::string &name, float &value )
{
    bool result = false;
    value = 0.0;
    return result;
}


bool MetGEOSfpFcast::getOption( const std::string &name, double &value )
{
    bool result = false;
    value = 0.0;
    return result;
}




int MetGEOSfpFcast::setup(  const std::string quantity, const std::string &time )
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

       if ( debug >= 3 ) {
         std::cerr << "MetGEOSfpFcast::setup: testing for " << quantity << " @ " << caltime << std::endl;
         std::cerr << "MetGEOSfpFcast::setup: the model run is <<" << modelRun << ">>" << std::endl; 
       }
       test_quant = quantity;
       test_date = caltime;
       
       // find the variable we are looking for
       status = fdir.LookUp( test_quant, desired_hgrid_id, desired_vgrid_id, desired_tspace, desired_tave
            , NULL, NULL, &test_ndims
            , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl, modelRun );
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
          status = fdir.LookUp( test_quant, use_hgrid, use_vgrid, use_tspace, use_tavg
            , NULL, NULL, &test_ndims
            , &test_vgrid, &test_hgrid, &test_tspace, &test_tave, &test_tbase, &newUrl, modelRun );
       }
       
       if ( status == 0 ) {
          
          test_url = *newUrl;
          if ( debug >= 3 ) {
             std::cerr << "MetGEOSfpFcast::setup:  testing was successful.  ndims is " << test_ndims << ", url=<<" << test_url << ">>" << std::endl;
          }
          
          delete newUrl;
       
       } else {
          throw(badDataNotFound());
       }
    }
      
    // std::cerr << " setup returns " << test_ndims << std::endl;    
    
    return test_ndims;

}    


time_t MetGEOSfpFcast::expiration( const std::string &valid_at )
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
     } else if (  cur_tgrid.n > 0 ) {
        // the model run is "" (i.e., ".latest"),
        // but maybe we have some data available
        // for which we have times
        itime = cur_tgrid.start - basetime;
        std::cerr << "MetGEOSfpFcast::expiration: itime = " << itime << std::endl;
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
     
        // now convert to time_t format (second elapsed since 1970-01-01T00 GMT)
        result = (etime - 25568.00)*24.0*3600.0; 
     }
     
     
     return result;

}


MetGEOSfpFcast* MetGEOSfpFcast::myNew()
{
   MetGEOSfpFcast *dup;
   
   dup = new MetGEOSfpFcast;
   
   dup->debug = debug;
   dup->setPgroup(my_pgroup, my_metproc);
   dup->defineCal( time2Cal(0), basetime );
   dup->maxsnaps = this->maxsnaps;
   dup->setCacheDir( this->diskcachedir );
   dup->modelRun = modelRun;
   
   return dup;
}




