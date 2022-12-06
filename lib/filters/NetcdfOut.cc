
#include "config.h"
#include <time.h>
#include <string.h>
#include <sstream>
#include "gigatraj/NetcdfOut.hh"

using namespace gigatraj;

#ifdef USE_DOUBLE
#define NC_REEL NC_DOUBLE
#else
#define NC_REEL NC_FLOAT
#endif

NetcdfOut::NetcdfOut()
{
    const char *nanstr = "";

    dbug = 0;
    
    maxchunk = 1000;
    
    fname = "";
    hdr_contents = "gigatraj output: air parcle trajectcory histories";
    hdr_contact = "";  // replace this with a gigatraj contact, but NOT a specific human person
    dir = 0;
    
    vcoord = "";
    vunits = "";
    vdesc = "";
    
    tagquant = "";
    tagunits = "";
    tagdesc = "";
    
    is_open = false;
    t0 = 0.0;
    tstamp = "";
    tyme = t0;
    pnum = 0;
    ip = 0;
    tnum = 0;
    do_status = false;
    do_flags = false;
    do_tag = false;
    do_tstamp = false;
    
    // time is not transformed
    to = 0.0;
    ts = 1.0;
    
    fmtspec = "";
    
    met = NULLPTR;
    
    NaN = RNAN(nanstr);
    dNaN = nan(nanstr);
    
    tyme = dNaN;

    vid_lon = -1;
    vtyp_lon = NC_REEL;

    vid_lat = -1;
    vtyp_lat = NC_REEL;

    vid_z = -1;
    vtyp_z = NC_REEL;

    vid_status = -1;
    vtyp_status = NC_INT;

    vid_flags = -1;
    vtyp_flags = NC_INT;

    vid_tag = -1;
    vtyp_tag = NC_DOUBLE;
    
    vid_tstamp = -1;
    vtyp_tstamp = NC_STRING;
    
}


NetcdfOut::~NetcdfOut()
{

    if ( ! is_open ) {
       close();
    }

}


void NetcdfOut::filename( const std::string file )
{

    if ( ! is_open ) {
       fname = file;
    } else {
       throw new badNetcdfTooLate();
    }
    

}

std::string& NetcdfOut::filename()
{
    return fname;
}

void NetcdfOut::vertical( const std::string vert )
{

    if ( ! is_open ) {
       vcoord = vert;
    } else {
       throw new badNetcdfTooLate();
    }

}

std::string& NetcdfOut::vertical()
{
    return vcoord;
}

void NetcdfOut::writeStatus( bool doit )
{
    if ( ! is_open ) {
       do_status = doit;
    } else {
       throw new badNetcdfTooLate();
    }

}

bool NetcdfOut::writeStatus()
{
     return do_status;
}


void NetcdfOut::writeFlags( bool doit )
{
    if ( ! is_open ) {
       do_flags = doit;
    } else {
       throw new badNetcdfTooLate();
    }

}

bool NetcdfOut::writeFlags()
{
     return do_flags;
}


void NetcdfOut::writeTag( bool doit, const std::string& quant, const std::string& units, const std::string& desc )
{

    if ( ! is_open ) {
       do_tag = doit;
       if ( quant != "" ) {
          tagquant = quant;
          if ( desc != "" ) {
             tagdesc = desc;
             if ( units != "" ) {
                tagunits = units;
             }
          }
       }
    } else {
       throw new badNetcdfTooLate();
    }
     
}

bool NetcdfOut::writeTag()
{
     return do_tag;
}

void NetcdfOut::addQuantity(  const std::string& quantity, const std::string& units, const std::string& desc  )
{

    if ( ! is_open ) {
    
       for ( std::vector<std::string>::iterator idx = other.begin(); idx != other.end(); idx++ ) {
           if ( *idx == quantity ) {
              // already here. do nothing.
              return;
           }
       }
       
       other.push_back( quantity );
       other_units.push_back( units );
       other_desc.push_back( desc );
       vid_other.push_back( -1 );
       vtyp_other.push_back( NC_REEL );

    } else {
       throw new badNetcdfTooLate();
    }
     
}

void NetcdfOut::delQuantity( std::string quantity )
{
    std::vector<std::string>::iterator qidx;
    std::vector<std::string>::iterator uidx;
    std::vector<std::string>::iterator didx;
     
    if ( ! is_open ) {
    
       for ( qidx = other.begin(), uidx = other_units.begin(), didx=other_desc.begin(); 
             qidx != other.end(); 
             qidx++, uidx++, didx++ ) {
           if ( *qidx == quantity ) {
              // found it
              
              // remove this element
              qidx = other.erase( qidx );
              uidx = other_units.erase( uidx );
              didx = other_desc.erase( didx );
              
              // ther other elements have not yet been used, so we
              // can just remove the one at the end--they are all the same
              vid_other.pop_back();
              vtyp_other.pop_back();
              
              break;
           }
       }
          
    } else {
       throw new badNetcdfTooLate();
    }
     
}

// should there be some kind of queryQuantity here?

void NetcdfOut::contents( std::string desc )
{
    if ( ! is_open ) {
       hdr_contents = desc;
    } else {
       throw new badNetcdfTooLate();
    }

}

std::string& NetcdfOut::contents()
{
    return hdr_contents;
}

void NetcdfOut::contact( std::string addr )
{
    if ( ! is_open ) {
       hdr_contact = addr;
    } else {
       throw new badNetcdfTooLate();
    }

}

std::string& NetcdfOut::contact()
{
   return hdr_contact;
}

void NetcdfOut::writeTimestamp( bool mode )
{
    if ( ! is_open ) {
       do_tstamp = mode;
    } else {
       throw new badNetcdfTooLate();
    }
}

bool NetcdfOut::writeTimestamp()
{
   return do_tstamp;
}


std::string& NetcdfOut::cal()
{
    return tstamp;
}

double NetcdfOut::time0()
{
    return t0;
}

void NetcdfOut::maxSequence( int n )
{
   if ( n > 0 ) {
      maxchunk = n;
   }

}

int NetcdfOut::maxSequence()
{
    return maxchunk;
}

void NetcdfOut::direction( int mode )
{
    if ( ! is_open ) {
       if ( ( mode == -1) || (mode == 1) ) {
          dir = mode;
       }
    } else {
       throw new badNetcdfTooLate();
    }
}

int NetcdfOut::direction()
{
    return dir;
}

bool NetcdfOut::is_root()
{
     ProcessGrp* pgrp;
     bool result;
     
     pgrp = met->getPgroup();
     if ( pgrp != NULLPTR ) { 
        result = pgrp->is_root();
     } else {
        result = true;
     }
     
     return result;
}

void NetcdfOut::clear()
{
    if ( ! is_open ) {
       do_flags = false;
       do_status = false;
       do_tag = false;
       do_tstamp = false;
       other.clear();
       other_units.clear();
       other_desc.clear();
       vid_other.clear();
       vtyp_other.clear();

    } else {
       throw new badNetcdfTooLate();
    }
}

void NetcdfOut::format( std::string fmt )
{
      int i;
      int state;
      std::string ch;
      bool first;
      std::string metfield;
      
      // since we are specifying a format, clear any previously-set flags
      clear();
      
      i = 0;
      state = 0;
      while ( i < fmt.size() ) {
         
         // grab the next character
         ch = fmt.substr(i,1);
      
         switch (state) {
         case 0: // in literal text, not a "%" sequence
             if ( ch == "%" ) {
                
                // and start a new format spec
                metfield = "";
                first = true;

                // start a % sequence
                state = 1;
             }
             break;
         case 1: // just begun a new "%" sequence
      
            if ( ch != "%" ) {
               
               // are we terminating the sequence already with 
               // a recognized format character?
               if ( ch == "t" || ch == "o" || ch == "a" || ch == "v" 
                 || ch == "i" || ch == "c" || ch == "x" ) {
                  
                  // these are all ignored
 
                  // reset the state to 0  
                  state = 0;             

               } else if ( ch == "T" ) {
               
                  do_tstamp = true;
                  
                  // reset the state to 0  
                  state = 0;             

               } else if ( ch == "f" ) {
               
                  do_flags = true;
                  
                  // reset the state to 0  
                  state = 0;             

               } else if ( ch == "s" ) {
               
                  do_status = true;
                  
                  // reset the state to 0  
                  state = 0;             

               } else if ( ch == "g" ) {
                  
                  do_tag = true;
                  
                  // reset the state to 0  
                  state = 0;             

               } else if ( ch == "{"  ) {
               
                  // this must be a met field
                                   
                  state = 10;               
               
               } else {
                  // no, the user is specifying something more
                  
                  // expect digits or a sewuence terminator next
                  state = 2;
                  
               }
               
            } else {
               // this is just a "%%" sequence.
               // ignore it.
               state = 0;
            }
      
            break;
         case 2: // in a digit sequence
            
            if ( ch == "0" || ch == "1" || ch == "2"  || ch == "3" || ch == "4" 
              || ch == "5" || ch == "6" || ch == "7" || ch == "8" || ch == "9" 
              || ch == "." ) {

              // ignore these numeric specifier characters              
            
            } else if ( ch == "t" || ch == "o" || ch == "a" || ch == "v" 
                     || ch == "i" || ch == "c" || ch == "x" ) {
                  // termination of a % sequence
                  
                  // ignore these sequences
                  
                  // on to the next sequence
                  state = 0;
                  
            } else if ( ch == "T" ) {
            
               do_tstamp = true;
               
               // reset the state to 0  
               state = 0;             

            } else if ( ch == "f" ) {
            
               do_flags = true;
               
               // reset the state to 0  
               state = 0;             

            } else if ( ch == "s" ) {
            
               do_status = true;
               
               // reset the state to 0  
               state = 0;             

            } else if ( ch == "g" ) {
               
               do_tag = true;
               
               // reset the state to 0  
               state = 0;             
            } else if ( ch == "{"  ) {      
                                            
                  // not quite at termination of a %i,j{field}m sequence
                                                              
                  state = 10;                  
                                            
            } else {
                std::cerr << "Bad format: " << fmt << std::endl;
                throw (NetcdfOut::badNetcdfFormatSpec());   
            }
            break;
         case 10: // in a met field sequence
            
            if ( ch != "}" ) {
               metfield = metfield + ch;         
            } else { 
               state = 11;
            }
            break;

         case 11: // ends a met field sequence

            if ( ch == "m" ) {
               
               addQuantity( metfield );
                           
            } else {          
                std::cerr << "Bad format: " << fmt << std::endl;
                throw (NetcdfOut::badNetcdfFormatSpec());   
            }
            
            // read for the next sequence
            state = 0;

            break;
         }
            
         i++;
      
      }
      
      fmtspec = fmt;
      
}

std::string NetcdfOut::format()
{
    std:string result;
    int nstuff;
    
    if ( fmtspec == "" ) {
       result = "%t %o %a %v";
       if ( do_flags ) {
          result = result + " %f";
       }
       if ( do_status ) {
          result = result + " %s";
       }
       if ( do_tag ) {
          result = result + " %g";
       }
       nstuff = other.size();
       if ( nstuff > 0 ) {
          for ( int i=0; i < nstuff; i++ ) {   
              result = result + " %{" + other[i] + "}m";
          }
       }
    } else {
       result = fmtspec;    
    }
    
    return result;
}

void NetcdfOut::debug( int mode )
{
     dbug = mode;
}

int NetcdfOut::debug()
{
     return dbug;
}

void NetcdfOut::setMet( MetData* metsrc )
{
     met = metsrc;
}

void NetcdfOut::setMet( Parcel* p )
{
    met = p->getMet();
}

MetData* NetcdfOut::getMet()
{
    return met;
}   

void NetcdfOut::init( Parcel *p, unsigned int n)
{
     init( p->getMet(), n );
}

void NetcdfOut::init( MetData *metsrc, unsigned int n)
{
    if ( ! is_open ) {
    
       tyme = dNaN;
       tnum = 0;
       
       dir = 0;
    
       met = metsrc;
     
       if ( vcoord == "" ) {
          vcoord = met->vertical();
          vunits = met->vunits();
          vdesc = vcoord;
       } else {
          vunits = met->units( vcoord );
       }
     
       if ( do_tag ) {
          if ( tagquant != "" ) {
             if ( tagquant == "" ) {
                tagquant = met->units( tagquant );
             }   
          }
       }
       
       if ( n > 0 ) {
          pnum = n;
       }
       
    } else {
       throw new badNetcdfTooLate();
    }
}

void NetcdfOut::setTimeTransform( double scale, double offset )
{
    if ( ! is_open ) {
       ts = scale;
       to = offset;
    } else {
       throw new badNetcdfTooLate();
    }
}

void NetcdfOut::getTimeTransform( double* scale, double* offset )
{
     *scale = ts;
     *offset = to;
}

std::string NetcdfOut::tunits()
{
   std::string result;
   double tm;
   std:string units;
   std::ostringstream oo;
   
   
   
   if ( abs( to - 693596.00 ) < 1e-3 ) {
      // It's the GEOS FP reference time 1-1-1 00:00:0.0
      result = "1-1-1 00:00:0.0";
   } else {
      // first, get the model time that corresponds to the
      // zero netcdf time
      tm = (0.0 - to)/ts;
      // now translate this to a calendar timestamp
      result = met->time2Cal( tm );
   }
   // now size up the units
   units = "";
   if ( abs( ts - 1.0 ) < 1e-3 ) {
      // it's in days
      units = "Days since ";
   } else if ( abs( ts - 24.0 ) < 1e-3 ) {
      // it's in hours
      units = "Hours since ";      
   } else if ( abs( ts - 24.0*60.0 ) < 1e-3 ) {
      // it's in minutes
      units = "Minutes since ";         
   } else if ( abs( ts - 24.0*60.0*60.0 ) < 1e-3 ) {
      // it's in seconds
      units = "Seconds since ";         
   } else {
      
      oo.setf( std::ios::fixed );
      oo.width( 14 );
      oo.precision( 10 );
      oo << 1.0/ts;
      
      units = oo.str() + " days since ";
   }
   
   result = units + result; 
   
   return result;
}

void NetcdfOut::open( std::string file, Parcel* p, unsigned int n )
{
     int err;
     std::string aname;
     std::string val;
     const char *aval;
     time_t t;
     int dims[2];
     float fval;
     double dval;
     int ival;
     const char* nanstr = "";
     int vid;
     size_t put_count;
     size_t put_start;
     ptrdiff_t put_stride;
     char *xstamp;
     struct tm *tm;
     std::string *tst1;
     ProcessGrp *pgrp;
     bool i_am_root;

     if ( is_open ) {
        close();
     }

     if ( file != "" ) {
        filename( file );
     }

     if ( dbug > 1 ) {
        std::cerr << "NetcdfOut::open: Trying to open " << fname <<  std::endl;
     }

     
     if ( p != NULLPTR ) {
        init(p);
     }

     if ( n > 0 ) {
        pnum = n;
     }

     i_am_root = is_root();
     
     if ( i_am_root ) {
        err = nc_create( fname.c_str(), NC_CLOBBER | NC_NETCDF4, &ncid);
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     is_open = true;

     // write Contents global attribute
     aname = "Contents";
     aval = hdr_contents.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, NC_GLOBAL, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     // write Creation_date global attribute
     aname = "Creation_date";
     // get the time
     t = time(NULL);
     // translate it to GMT in a time strcuture
     tm = gmtime( &t );
     // turn the time structure into a string
     tst1 = new std::string( asctime(tm)  );
     // replace the newline at the end with a space
     tst1->replace( tst1->size() - 1, 1, " " );
     // add the time zone at the end
     *tst1 = *tst1 + " GMT";
     // convert to a C string
     aval = tst1->c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, NC_GLOBAL, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     delete tst1;
     
     // write Contact global attribute
     if ( hdr_contact != "" ) {
        aname = "Contact";
        aval = hdr_contact.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, NC_GLOBAL, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
     }

     // write Trajectory_direction global attribute
     // (but only if dir != 0; otherwise it is determined dynamically and written by writeout()
     if ( dir == 1 ) {
        aname = "Trajectory_direction";
        val = "fwd";
        aval = val.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, NC_GLOBAL, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }        
        }        
     } else if ( dir == -1 ) {
        aname = "Trajectory_direction";
        val = "bck";
        aval = val.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, NC_GLOBAL, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }        
        }        
     
     }
     
     // write Trajectory_start global attribute
     aname = "Trajectory_start";
     if ( (tstamp == "") && (met != NULLPTR) ) {
        tstamp = met->time2Cal( t0 );
     }
     aval = tstamp.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, NC_GLOBAL, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     // define time dimension unbounded
     val = "time";
     if ( i_am_root ) {
        err = nc_def_dim( ncid, val.c_str(), NC_UNLIMITED, &did_time );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }

     // define id dimension unbounded
     val = "id";
     if ( i_am_root ) {
        err = nc_def_dim( ncid, val.c_str(), pnum, &did_id );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     //// define time variable
     val = "time";
     if ( i_am_root ) {
        err = nc_def_var( ncid, val.c_str(), NC_DOUBLE, 1, &did_time, &vid_time );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
        err = nc_def_var_fill( ncid, vid_time, NC_NOFILL, NULL );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // write long_name time attribute
     aname = "long_name";
     val = tunits(); 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_time, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // write standard_name time attribute
     aname = "standard_name";
     val = "time"; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_time, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // write units time attrbiute
     aname = "units";
     val = "day"; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_time, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     /// define the timestamp variable
     if ( do_tstamp ) {
        val = "timestamp";
        if ( i_am_root ) {
           err = nc_def_var( ncid, val.c_str(), NC_STRING, 1, &did_time, &vid_tstamp );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        // write long_name time attribute
        aname = "long_name";
        val = "Date + time"; 
        aval = val.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, vid_tstamp, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        // write units time attrbiute
        aname = "units";
        val = ""; 
        aval = val.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, vid_tstamp, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
     }
     
     
     //// define id variable
     val = "id";
     if ( i_am_root ) {
        err = nc_def_var( ncid, val.c_str(), NC_DOUBLE, 1, &did_id, &vid_id );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // write long_name id attribute
     aname = "long_name";
     val = "parcel id"; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_id, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // write units id attrbiute
     aname = "units";
     val = "1"; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_id, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     // set up dinensions for the regular variables
     dims[0] = did_time;
     dims[1] = did_id;
     
     //// define lon variable
     val = "lon";
     if ( i_am_root ) {
        err = nc_def_var( ncid, val.c_str(), NC_REEL, 2, dims, &vid_lon );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define missing_value attribute
     if ( i_am_root ) {
        err = nc_def_var_fill( ncid, vid_lon, NC_FILL, &NaN );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     aname = "missing_value";
     if ( i_am_root ) {
#ifdef USE_DOUBLE    
        err = nc_put_att_double( ncid, vid_lon, aname.c_str(), NC_DOUBLE, 1, &NaN );
#else
        err = nc_put_att_float( ncid, vid_lon, aname.c_str(), NC_FLOAT, 1, &NaN );
#endif
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define _FillValue attribute
     aname = "_FillValue";
     if ( i_am_root ) {
#ifdef USE_DOUBLE    
        err = nc_put_att_double( ncid, vid_lon, aname.c_str(), NC_DOUBLE, 1, &NaN );
#else
        err = nc_put_att_float( ncid, vid_lon, aname.c_str(), NC_FLOAT, 1, &NaN );
#endif
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define long_name attribute
     aname = "long_name";
     val = "longitude"; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_lon, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define units attribute
     aname = "units";
     val = "degrees_east"; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_lon, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     //// define lat variable
     val = "lat";
     if ( i_am_root ) {
        err = nc_def_var( ncid, val.c_str(), NC_REEL, 2, dims, &vid_lat );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define missing_value attribute
     if ( i_am_root ) {
        err = nc_def_var_fill( ncid, vid_lat, NC_FILL, &NaN );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     aname = "missing_value";
     if ( i_am_root ) {
#ifdef USE_DOUBLE    
        err = nc_put_att_double( ncid, vid_lat, aname.c_str(), NC_DOUBLE, 1, &NaN );
#else
        err = nc_put_att_float( ncid, vid_lat, aname.c_str(), NC_FLOAT, 1, &NaN );
#endif
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define _FillValue attribute
     aname = "_FillValue";
     if ( i_am_root ) {
#ifdef USE_DOUBLE    
        err = nc_put_att_double( ncid, vid_lat, aname.c_str(), NC_DOUBLE, 1, &NaN );
#else
        err = nc_put_att_float( ncid, vid_lat, aname.c_str(), NC_FLOAT, 1, &NaN );
#endif
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define long_name attribute
     aname = "long_name";
     val = "latitude"; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_lat, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define units attribute
     aname = "units";
     val = "degrees_north"; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_lat, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     //// define vertical coordinate variable
     val = vcoord;
     if ( i_am_root ) {
        err = nc_def_var( ncid, val.c_str(), NC_REEL, 2, dims, &vid_z );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define missing_value attribute
     if ( i_am_root ) {
        err = nc_def_var_fill( ncid, vid_z, NC_FILL, &NaN );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     aname = "missing_value";
     if ( i_am_root ) {
#ifdef USE_DOUBLE    
        err = nc_put_att_double( ncid, vid_z, aname.c_str(), NC_DOUBLE, 1, &NaN );
#else
        err = nc_put_att_float( ncid, vid_z, aname.c_str(), NC_FLOAT, 1, &NaN );
#endif
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define _FillValue attribute
     aname = "_FillValue";
     if ( i_am_root ) {
#ifdef USE_DOUBLE    
        err = nc_put_att_double( ncid, vid_z, aname.c_str(), NC_DOUBLE, 1, &NaN );
#else
        err = nc_put_att_float( ncid, vid_z, aname.c_str(), NC_FLOAT, 1, &NaN );
#endif
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define long_name attribute
     if ( vdesc != "" ) {
        aname = "long_name";
        val = vdesc; 
        aval = val.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, vid_z, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
     }
     // define units attribute
     aname = "units";
     val = vunits; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_z, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     // define the positive attribute
     aname = "positive";
     val = "up"; 
     // note: this should be determiend by the MetData object 
     if ( vcoord == "pressure" ) {
        val = "down";
     }
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_z, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     //define the vertical_coordinate attribute
     aname = "vertical_coordinate";
     val = "yes"; 
     aval = val.c_str();
     if ( i_am_root ) {
        err = nc_put_att_string( ncid, vid_z, aname.c_str(), 1, &aval );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     
     
     if ( do_status ) {
        //// define status variable
        val = "status";
        if ( i_am_root ) {
           err = nc_def_var( ncid, val.c_str(), NC_INT, 2, dims, &vid_status );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        // define long_name attribute
        aname = "long_name";
        val = "parcel bitwise status "; 
        aval = val.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, vid_status, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        // define units attribute
        aname = "units";
        val = "1"; 
        aval = val.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, vid_status, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
     }
     
     if ( do_flags ) {
        //// define flags variable
        val = "flags";
        if ( i_am_root ) {
           err = nc_def_var( ncid, val.c_str(), NC_INT, 2, dims, &vid_flags);
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        // define long_name attribute
        aname = "long_name";
        val = "parcel bitwise flags"; 
        aval = val.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, vid_flags, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        // define units attribute
        aname = "units";
        val = "1"; 
        aval = val.c_str();
        if ( i_am_root ) {
           err = nc_put_att_string( ncid, vid_flags, aname.c_str(), 1, &aval );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
     }
     
     if ( do_tag ) {
        // define tag variable
        val = "tag";
        if ( i_am_root ) {
           err = nc_def_var( ncid, val.c_str(), NC_DOUBLE, 2, dims, &vid_tag );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        // define missing_value attribute       
        if ( i_am_root ) {
           err = nc_def_var_fill( ncid, vid_tag, NC_FILL, &dNaN );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        aname = "missing_value";
        if ( i_am_root ) {
           err = nc_put_att_double( ncid, vid_tag, aname.c_str(), NC_DOUBLE, 1, &dNaN );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        // define _FillValue attribute
        aname = "_FillValue";
        if ( i_am_root ) {
           err = nc_put_att_double( ncid, vid_tag, aname.c_str(), NC_DOUBLE, 1, &dNaN );
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }
        if ( tagdesc != "" ) {
           // define long_name attribute
           aname = "long_name";
           val = tagquant; 
           if ( tagdesc != "" ) {
              val = val + "; " + tagdesc;
           }
           aval = val.c_str();
           if ( i_am_root ) {
              err = nc_put_att_string( ncid, vid_tag, aname.c_str(), 1, &aval );
              if ( err != NC_NOERR ) {
                 throw(badNetcdfError(err));
              }
           }
        }
        if ( tagunits != "" ) {
           // define units attribute
           aname = "units";
           val = tagunits; 
           aval = val.c_str();
           if ( i_am_root ) {
              err = nc_put_att_string( ncid, vid_tag, aname.c_str(), 1, &aval );
              if ( err != NC_NOERR ) {
                 throw(badNetcdfError(err));
              }
           }
        }
     }

     
     for ( int i=0; i < other.size(); i++ ) {
         //// define other variable
         val = other[i];
         if ( i_am_root ) {
            err = nc_def_var( ncid, val.c_str(), NC_REEL, 2, dims, &vid );
            if ( err != NC_NOERR ) {
               throw(badNetcdfError(err));
            }
         }
         vid_other[i] = vid;
         vtyp_other[i] = NC_REEL;
         
         // define missing_value attribute
         if ( i_am_root ) {
            err = nc_def_var_fill( ncid, vid, NC_FILL, &NaN );
            if ( err != NC_NOERR ) {
               throw(badNetcdfError(err));
            }
         }
         aname = "missing_value";
         if ( i_am_root ) {
#ifdef USE_DOUBLE    
            err = nc_put_att_double( ncid, vid, aname.c_str(), NC_DOUBLE, 1, &NaN );
#else
            err = nc_put_att_float( ncid, vid, aname.c_str(), NC_FLOAT, 1, &NaN );
#endif
            if ( err != NC_NOERR ) {
               throw(badNetcdfError(err));
            }
         }
         // define _FillValue attribute
         aname = "_FillValue";
         if ( i_am_root ) {
#ifdef USE_DOUBLE    
            err = nc_put_att_double( ncid, vid, aname.c_str(), NC_DOUBLE, 1, &NaN );
#else
            err = nc_put_att_float( ncid, vid, aname.c_str(), NC_FLOAT, 1, &NaN );
#endif
            if ( err != NC_NOERR ) {
               throw(badNetcdfError(err));
            }
         }
         val = other_desc[i];
         if ( val != "" ) {
            // define long_name attribute
            aname = "long_name";
            aval = val.c_str();
            if ( i_am_root ) {
               err = nc_put_att_string( ncid, vid, aname.c_str(), 1, &aval );
               if ( err != NC_NOERR ) {
                  throw(badNetcdfError(err));
               }
            }
         }
         val = other_units[i];
         if ( val != "" ) {
            // define units attribute
            aname = "units";
            aval = val.c_str();
            if ( i_am_root ) {
               err = nc_put_att_string( ncid, vid, aname.c_str(), 1, &aval );
               if ( err != NC_NOERR ) {
                  throw(badNetcdfError(err));
               }
            }
         }   
     }
     
     
     // end definitoin mode
     if ( i_am_root ) {
        err = nc_enddef( ncid );
        if ( err != NC_NOERR ) {
           throw(badNetcdfError(err));
        }
     }
     
     // write id variable values
     put_count = 1;
     put_stride = 1;
     for ( int i=0; i < pnum; i++ ) {
         dval = i;
         put_start = i;
         if ( i_am_root ) {
            err = nc_put_vars_double( ncid, vid_id, &put_start, &put_count, &put_stride, &dval );
            if ( err != NC_NOERR ) {
               throw(badNetcdfError(err));
            }
         }
     }

     if ( dbug > 1 ) {
        std::cerr << "NetcdfOut::open: " << fname << " is opened." << std::endl;
     }


}    


void NetcdfOut::close()
{
     int err;
     bool i_am_root;
     
     if ( is_open ) {

        i_am_root = is_root();

        if ( i_am_root) {
           err = nc_close(ncid);
           if ( err != NC_NOERR ) {
              throw(badNetcdfError(err));
           }
        }

        is_open = false;

        if ( dbug > 1 ) {
           std::cerr << "NetcdfOut::close: " << fname << " is closed." << std::endl;
        }

     }
     
     vid_lon = -1;
     vid_lat = -1;
     vid_z = -1;
     vid_status = -1;
     vid_flags = -1;
     vid_tag = -1;
     for ( int i=0; i < other.size(); i++ ) {
         vid_other[i] = -1;
     }
     
}

void NetcdfOut::writeout( double t, unsigned int n, real *lons, real *lats, real *zs, int *flags, int *statuses, double *tags, real **stuff )
{
   int err;
   bool notrace;
   size_t t_start;
   size_t t_count;
   ptrdiff_t t_stride;
   size_t put_start[2];
   size_t put_count[2];
   ptrdiff_t put_stride[2];
   int nstuff;
   int vid;
   std::string aname;
   std::string val;
   const char *aval;
   bool i_am_root;
   double netcdf_time;

   i_am_root = is_root();

   if ( dbug > 5 ) {
      std::cerr << "NetcdfOut::writeout: seeking to write " << n << " Parcels at time " << t << std::endl;
   }
   if ( ( ! finite( tyme ) ) || ( ( t != tyme ) && (! notrace) ) ) {
      // new time to output to file as part of the time dimension
      if ( dbug > 10 ) {
         std::cerr << "NetcdfOut::writeout: advancing time from " << tyme << " to " << t << std::endl;
      }
      
      // between the two times, determine which direction we are moving
      if ( (dir == 0) && (FINITE(tyme)) ) {
         if ( tyme < t ) {
            dir = 1;
         } else {
            dir = -1;
         }
         if ( dbug > 10 ) {
            std::cerr << "NetcdfOut::writeout: setting traj direction to " << dir << std::endl;
            std::cerr << "                     tyme = " << tyme << ", t = " << t << std::endl;
         }
         if ( dir > 0 ) {
            aname = "Trajectory_direction";
            val = "fwd";
            aval = val.c_str();
            if ( i_am_root) {
               err = nc_put_att_string( ncid, NC_GLOBAL, aname.c_str(), 1, &aval );
               if ( err != NC_NOERR ) {
                  throw(badNetcdfError(err));
               }        
            }        
         } else {
            aname = "Trajectory_direction";
            val = "bck";
            aval = val.c_str();
            if ( i_am_root) {
               err = nc_put_att_string( ncid, NC_GLOBAL, aname.c_str(), 1, &aval );
               if ( err != NC_NOERR ) {
                  throw(badNetcdfError(err));
               }        
            }        
     
         }
      }
      
      t_start = tnum;
      t_count = 1;
      t_stride = 1;
      if ( i_am_root) {
         netcdf_time = tconv( t );
         err = nc_put_vars_double( ncid, vid_time, &t_start, &t_count, &t_stride, &netcdf_time );
         if ( err != NC_NOERR ) {
            throw(badNetcdfError(err));
         }
      }
      if ( do_tstamp ) {
         std::string ts = met->time2Cal( t );
         const char *tsc = ts.c_str();
         if ( i_am_root ) {
            err = nc_put_vars_string( ncid, vid_tstamp, &t_start, &t_count, &t_stride, &tsc );
            if ( err != NC_NOERR ) {
               throw(badNetcdfError(err));
            }
         }
      }
      
      tnum++;
      tyme = t;
      ip = 0;
   
   }
   
   // Can we write this many Parcels?
   if ( ( n > 0 ) && ( (ip+n) <= pnum ) ) {
   
       put_start[0] = tnum - 1;
       put_start[1] = ip;
       put_count[0] = 1;
       put_count[1] = n;
       put_stride[0] = 1;
       put_stride[1] = 1;
       
       if ( i_am_root) {
#ifdef USE_DOUBLE   
          err = nc_put_vars_double( ncid, vid_lon, put_start, put_count, put_stride, lons );
#else
          err = nc_put_vars_float( ncid, vid_lon, put_start, put_count, put_stride, lons );
#endif
          if ( err != NC_NOERR ) {
             throw(badNetcdfError(err));
          }
       }
         
       if ( i_am_root) {
#ifdef USE_DOUBLE   
          err = nc_put_vars_double( ncid, vid_lat, put_start, put_count, put_stride, lats );
#else
          err = nc_put_vars_float( ncid, vid_lat, put_start, put_count, put_stride, lats );
#endif
          if ( err != NC_NOERR ) {
             throw(badNetcdfError(err));
          }
       }

       if ( i_am_root) {
#ifdef USE_DOUBLE   
          err = nc_put_vars_double( ncid, vid_z, put_start, put_count, put_stride, zs );
#else
          err = nc_put_vars_float( ncid, vid_z, put_start, put_count, put_stride, zs );
#endif
          if ( err != NC_NOERR ) {
             throw(badNetcdfError(err));
          }
       }
   
       if ( flags != NULL ) {
          if ( i_am_root) {
             err = nc_put_vars_int( ncid, vid_flags, put_start, put_count, put_stride, flags );
             if ( err != NC_NOERR ) {
                throw(badNetcdfError(err));
             }
          }
       }
       if ( statuses != NULL ) {
          if ( i_am_root) {
             err = nc_put_vars_int( ncid, vid_status, put_start, put_count, put_stride, statuses );
             if ( err != NC_NOERR ) {
                throw(badNetcdfError(err));
             }
          }
       }
       if ( tags != NULL ) {
          if ( i_am_root) {
             err = nc_put_vars_double( ncid, vid_tag, put_start, put_count, put_stride, tags );
             if ( err != NC_NOERR ) {
                throw(badNetcdfError(err));
             }
          }
       }
       if ( stuff != NULL ) {
          nstuff = other.size();
          for ( int i=0; i< nstuff; i++ ) {
              vid = vid_other[i];
              if ( i_am_root) {
#ifdef USE_DOUBLE   
                 err = nc_put_vars_double( ncid, vid, put_start, put_count, put_stride, stuff[i] );
#else
                 err = nc_put_vars_float( ncid, vid, put_start, put_count, put_stride, stuff[i] );
#endif
                 if ( err != NC_NOERR ) {
                    throw(badNetcdfError(err));
                 }
              }
          
          }
       }
   
   
       ip += n;
   } else {
      throw(badNetcdfBadNumberParcels());
   } 
   
   
}

void NetcdfOut::apply( Parcel& p )
{
   double t;
   real val;
   real lon,lat,z;
   double tagval;
   double* tag;
   bool notrace;
   int iflags;
   int* flags;
   int istatus;
   int* statuses;
   real **stuff;
   int nstuff;
   
   notrace = p.queryNoTrace();
   
   
   t = p.getTime();
 
   lon = NaN;
   lat = NaN;
   z = NaN;
   tag = NULL;
   flags = NULL;
   statuses = NULL;
   stuff = NULL;
   
   
   if ( ! notrace ) {
      lon = p.getLon();
      lat = p.getLat();
      z = p.getZ();
   }
   if ( do_flags ) {
      iflags = p.flags();
      flags = &iflags;
   }
   if ( do_status ) {
      istatus = p.status();
      statuses = &istatus;
   }
   if ( do_tag ) {
      tagval = p.tag();
      tag = &tagval;
   }
   nstuff = other.size();
   if ( nstuff > 0 ) {
      stuff = new real*[nstuff];
      for ( int i=0; i < nstuff; i++ ) {
          stuff[i] = new real[1];
          if ( ! notrace ) {
             val = met->getData( other[i], t, lon, lat, z );
          } else {
             val = NaN;
          }
          *(stuff[i]) = val;
      }
   }
 
   writeout( t, 1, &lon, &lat, &z, flags, statuses, tag, stuff );
 
   if ( nstuff > 0 ) {
      for ( int i=0; i < nstuff; i++ ) {
          delete stuff[i];
      }
      delete stuff;
   }

}

void NetcdfOut::apply( Parcel * const p, const int n )
{
   int chunksize;
   double t;
   double tt;
   real val;
   real* lons;
   real* lats;
   real* zs;
   double* tags;
   bool notrace;
   int* flags;
   int* statuses;
   real **stuff;
   int nstuff;
   int ii;
   int j;
   
   if ( n > 0 )  { 
   
      // first, allocate the space for this chunk
      lons = new real[maxchunk];
      lats = new real[maxchunk];
      zs   = new real[maxchunk];
      flags = NULL;
      if ( do_flags ) {
          flags = new int[maxchunk];
      }
      statuses = NULL;
      if ( do_status ) {
          statuses = new int[maxchunk];
      }
      tags = NULL;
      if ( do_tag ) {
          tags = new double[maxchunk];
      }    
      nstuff = other.size();
      if ( nstuff > 0 ) {
         stuff = new real*[nstuff];
         for ( int i=0; i < nstuff; i++ ) {
             stuff[i] = new real[maxchunk];
         }
      }
      
      // find the time for this batch of Parcels
      tt = 0.0;
      for ( j=0; j < pnum; j++ ) {
          notrace = p[j].queryNoTrace();
          if ( ! notrace ) {
             // probably need to be more sophisticated about this
             tt = p[j].getTime();
          }
      }
      if ( dbug > 50 ) {
         std::cerr << " output time tt=" << tt << std::endl;      
      }
      ii = 0;
      while ( ii < n ) {
   
         chunksize = n - ii;
         if ( chunksize > maxchunk ) {
            chunksize = maxchunk;
         }
         
        
         // now load the content
         for ( int i=0; i < chunksize; i++ ) {
         
             j = ii + i;
         
             notrace = p[j].queryNoTrace();
             if ( ! notrace ) {
                // we only output parcels which are out the standard time for this batch of parcels
                if (  abs( p[j].getTime() - tt ) > 1e-5 ) {
                   notrace = true;
                }
             }
             
             lons[i] = NaN;
             lats[i] = NaN;
             zs[i]   = NaN;
             if ( do_tag ) {
                tags[i] = dNaN;
             }   
             if ( ! notrace ) {
                p[j].getPos( &(lons[i]), &(lats[i]) );
                zs[i] = p[j].getZ();
                
                if ( do_tag ) {
                   tags[i] = p[j].tag();
                }
                         
             }
             if ( do_flags ) {
                flags[i] = p[j].flags();
             }
             if ( do_status ) {
                statuses[i] = p[j].status();
             }
         
         }
         if ( nstuff > 0 ) {
            for ( int i=0; i < nstuff; i++ ) {
               met->getData( other[i], tt, chunksize, lons, lats, zs, stuff[i], METDATA_NANBAD );
               for ( int k=0; k < chunksize; k++ ) {
                   j = ii + k;
                   notrace = p[j].queryNoTrace();
                   if ( notrace ) {
                      (stuff[i])[j] = NaN;
                   }
               }
            }
         }
        
         writeout( tt, chunksize, lons, lats, zs, flags, statuses, tags, stuff );
        
       
       
         // on to the next chunk
         ii = ii + chunksize;
         
      }
      // now free the space for this chunk
      if ( nstuff > 0 ) {
         for ( int i=0; i < nstuff; i++ ) {
             delete stuff[i];
         }
         delete stuff;
      }
      if ( tags != NULL ) {
         delete tags;
      }
      if ( statuses != NULL ) {
         delete statuses;
      }
      if ( flags != NULL ) {
         delete flags;
      } 
      delete zs;
      delete lats;
      delete lons;
   
   } else {
      // zero or fewer Parcels
      throw(badNetcdfBadNumberParcels());
   } 
   
}

void NetcdfOut::apply( std::vector<Parcel>& p )
{
   int chunksize;
   double t;
   double tt;
   real val;
   real* lons;
   real* lats;
   real* zs;
   double* tags;
   bool notrace;
   int* flags;
   int* statuses;
   real **stuff;
   int nstuff;
   int ii;
   int j;
   int n;
   std::vector<Parcel>::iterator vi;
   
   
   n = p.size();
   
   if ( n > 0 )  { 
   
      // first, allocate the space for this chunk
      lons = new real[maxchunk];
      lats = new real[maxchunk];
      zs   = new real[maxchunk];
      flags = NULL;
      if ( do_flags ) {
          flags = new int[maxchunk];
      }
      statuses = NULL;
      if ( do_status ) {
          statuses = new int[maxchunk];
      }
      tags = NULL;
      if ( do_tag ) {
          tags = new double[maxchunk];
      }    
      nstuff = other.size();
      if ( nstuff > 0 ) {
         stuff = new real*[nstuff];
         for ( int i=0; i < nstuff; i++ ) {
             stuff[i] = new real[maxchunk];
         }
      }
      
      // find the time for this batch of Parcels
      tt = 0.0;
      for ( vi = p.begin(); vi != p.end(); vi++ ) {
          notrace = vi->queryNoTrace();
          if ( ! notrace ) {
             // probably need to be more sophisticated about this
             tt = vi->getTime();
          }
      }
      if ( dbug > 50 ) {
         std::cerr << " output time tt=" << tt << std::endl;      
      }
      ii = 0;
      vi = p.begin();
      while ( ii < n ) {
   
         chunksize = n - ii;
         if ( chunksize > maxchunk ) {
            chunksize = maxchunk;
         }
         
        
         // now load the content
         for ( int i=0; i < chunksize; i++ ) {
         
             notrace = vi->queryNoTrace();
             if ( ! notrace ) {
                // we only output parcels which are out the standard time for this batch of parcels
                if (  abs( vi->getTime() - tt ) > 1e-5 ) {
                   notrace = true;
                }
             }
             
             lons[i] = NaN;
             lats[i] = NaN;
             zs[i]   = NaN;
             if ( do_tag ) {
                tags[i] = dNaN;
             }   
             if ( ! notrace ) {
                vi->getPos( &(lons[i]), &(lats[i]) );
                zs[i] = vi->getZ();
                
                if ( do_tag ) {
                   tags[i] = vi->tag();
                }
                         
             }
             if ( do_flags ) {
                flags[i] = vi->flags();
             }
             if ( do_status ) {
                statuses[i] = vi->status();
             }
         
             vi++;
         }
         if ( nstuff > 0 ) {
            for ( int i=0; i < nstuff; i++ ) {
               met->getData( other[i], tt, chunksize, lons, lats, zs, stuff[i], METDATA_NANBAD );
               for ( int k=0; k < chunksize; k++ ) {
                   j = ii + k;
                   notrace = vi->queryNoTrace();
                   if ( notrace ) {
                      (stuff[i])[j] = NaN;
                   }
               }
            }
         }
        
         writeout( tt, chunksize, lons, lats, zs, flags, statuses, tags, stuff );
        
         // on to the next chunk
         ii = ii + chunksize;
         
      }
      // now free the space for this chunk
      if ( nstuff > 0 ) {
         for ( int i=0; i < nstuff; i++ ) {
             delete stuff[i];
         }
         delete stuff;
      }
      if ( tags != NULL ) {
         delete tags;
      }
      if ( statuses != NULL ) {
         delete statuses;
      }
      if ( flags != NULL ) {
         delete flags;
      } 
      delete zs;
      delete lats;
      delete lons;
   
   } else {
      // zero or fewer Parcels
      throw(badNetcdfBadNumberParcels());
   } 
   
}

void NetcdfOut::apply( std::list<Parcel>& p )
{
   int chunksize;
   double t;
   double tt;
   real val;
   real* lons;
   real* lats;
   real* zs;
   double* tags;
   bool notrace;
   int* flags;
   int* statuses;
   real **stuff;
   int nstuff;
   int ii;
   int j;
   int n;
   std::list<Parcel>::iterator vi;
   
   
   n = p.size();
   
   if ( n > 0 )  { 
   
      // first, allocate the space for this chunk
      lons = new real[maxchunk];
      lats = new real[maxchunk];
      zs   = new real[maxchunk];
      flags = NULL;
      if ( do_flags ) {
          flags = new int[maxchunk];
      }
      statuses = NULL;
      if ( do_status ) {
          statuses = new int[maxchunk];
      }
      tags = NULL;
      if ( do_tag ) {
          tags = new double[maxchunk];
      }    
      nstuff = other.size();
      if ( nstuff > 0 ) {
         stuff = new real*[nstuff];
         for ( int i=0; i < nstuff; i++ ) {
             stuff[i] = new real[maxchunk];
         }
      }
      
      // find the time for this batch of Parcels
      tt = 0.0;
      for ( vi = p.begin(); vi != p.end(); vi++ ) {
          notrace = vi->queryNoTrace();
          if ( ! notrace ) {
             // probably need to be more sophisticated about this
             tt = vi->getTime();
          }
      }
      if ( dbug > 50 ) {
         std::cerr << " output time tt=" << tt << std::endl;      
      }
      ii = 0;
      vi = p.begin();
      while ( ii < n ) {
   
         chunksize = n - ii;
         if ( chunksize > maxchunk ) {
            chunksize = maxchunk;
         }
         
        
         // now load the content
         for ( int i=0; i < chunksize; i++ ) {
         
             notrace = vi->queryNoTrace();
             if ( ! notrace ) {
                // we only output parcels which are out the standard time for this batch of parcels
                if (  abs( vi->getTime() - tt ) > 1e-5 ) {
                   notrace = true;
                }
             }
             
             lons[i] = NaN;
             lats[i] = NaN;
             zs[i]   = NaN;
             if ( do_tag ) {
                tags[i] = dNaN;
             }   
             if ( ! notrace ) {
                vi->getPos( &(lons[i]), &(lats[i]) );
                zs[i] = vi->getZ();
                
                if ( do_tag ) {
                   tags[i] = vi->tag();
                }
                         
             }
             if ( do_flags ) {
                flags[i] = vi->flags();
             }
             if ( do_status ) {
                statuses[i] = vi->status();
             }
         
             vi++;
         }
         if ( nstuff > 0 ) {
            for ( int i=0; i < nstuff; i++ ) {
               met->getData( other[i], tt, chunksize, lons, lats, zs, stuff[i], METDATA_NANBAD );
               for ( int k=0; k < chunksize; k++ ) {
                   j = ii + k;
                   notrace = vi->queryNoTrace();
                   if ( notrace ) {
                      (stuff[i])[j] = NaN;
                   }
               }
            }
         }
        
         writeout( tt, chunksize, lons, lats, zs, flags, statuses, tags, stuff );
        
         // on to the next chunk
         ii = ii + chunksize;
         
      }
      // now free the space for this chunk
      if ( nstuff > 0 ) {
         for ( int i=0; i < nstuff; i++ ) {
             delete stuff[i];
         }
         delete stuff;
      }
      if ( tags != NULL ) {
         delete tags;
      }
      if ( statuses != NULL ) {
         delete statuses;
      }
      if ( flags != NULL ) {
         delete flags;
      } 
      delete zs;
      delete lats;
      delete lons;
   
   } else {
      // zero or fewer Parcels
      throw(badNetcdfBadNumberParcels());
   } 
   
}

void NetcdfOut::apply( std::deque<Parcel>& p )
{
   int chunksize;
   double t;
   double tt;
   real val;
   real* lons;
   real* lats;
   real* zs;
   double* tags;
   bool notrace;
   int* flags;
   int* statuses;
   real **stuff;
   int nstuff;
   int ii;
   int j;
   int n;
   std::deque<Parcel>::iterator vi;
   
   
   n = p.size();
   
   if ( n > 0 )  { 
   
      // first, allocate the space for this chunk
      lons = new real[maxchunk];
      lats = new real[maxchunk];
      zs   = new real[maxchunk];
      flags = NULL;
      if ( do_flags ) {
          flags = new int[maxchunk];
      }
      statuses = NULL;
      if ( do_status ) {
          statuses = new int[maxchunk];
      }
      tags = NULL;
      if ( do_tag ) {
          tags = new double[maxchunk];
      }    
      nstuff = other.size();
      if ( nstuff > 0 ) {
         stuff = new real*[nstuff];
         for ( int i=0; i < nstuff; i++ ) {
             stuff[i] = new real[maxchunk];
         }
      }
      
      // find the time for this batch of Parcels
      tt = 0.0;
      for ( vi = p.begin(); vi != p.end(); vi++ ) {
          notrace = vi->queryNoTrace();
          if ( ! notrace ) {
             // probably need to be more sophisticated about this
             tt = vi->getTime();
          }
      }
      if ( dbug > 50 ) {
         std::cerr << " output time tt=" << tt << std::endl;      
      }
      ii = 0;
      vi = p.begin();
      while ( ii < n ) {
   
         chunksize = n - ii;
         if ( chunksize > maxchunk ) {
            chunksize = maxchunk;
         }
         
        
         // now load the content
         for ( int i=0; i < chunksize; i++ ) {
         
             notrace = vi->queryNoTrace();
             if ( ! notrace ) {
                // we only output parcels which are out the standard time for this batch of parcels
                if (  abs( vi->getTime() - tt ) > 1e-5 ) {
                   notrace = true;
                }
             }
             
             lons[i] = NaN;
             lats[i] = NaN;
             zs[i]   = NaN;
             if ( do_tag ) {
                tags[i] = dNaN;
             }   
             if ( ! notrace ) {
                vi->getPos( &(lons[i]), &(lats[i]) );
                zs[i] = vi->getZ();
                
                if ( do_tag ) {
                   tags[i] = vi->tag();
                }
                         
             }
             if ( do_flags ) {
                flags[i] = vi->flags();
             }
             if ( do_status ) {
                statuses[i] = vi->status();
             }
         
             vi++;
         }
         if ( nstuff > 0 ) {
            for ( int i=0; i < nstuff; i++ ) {
               met->getData( other[i], tt, chunksize, lons, lats, zs, stuff[i], METDATA_NANBAD );
               for ( int k=0; k < chunksize; k++ ) {
                   j = ii + k;
                   notrace = vi->queryNoTrace();
                   if ( notrace ) {
                      (stuff[i])[j] = NaN;
                   }
               }
            }
         }
        
         writeout( tt, chunksize, lons, lats, zs, flags, statuses, tags, stuff );
        
         // on to the next chunk
         ii = ii + chunksize;
         
      }
      // now free the space for this chunk
      if ( nstuff > 0 ) {
         for ( int i=0; i < nstuff; i++ ) {
             delete stuff[i];
         }
         delete stuff;
      }
      if ( tags != NULL ) {
         delete tags;
      }
      if ( statuses != NULL ) {
         delete statuses;
      }
      if ( flags != NULL ) {
         delete flags;
      } 
      delete zs;
      delete lats;
      delete lons;
   
   } else {
      // zero or fewer Parcels
      throw(badNetcdfBadNumberParcels());
   } 
   
}

void NetcdfOut::apply( Flock& p )
{
   int chunksize;
   double t;
   double tt;
   double ttbck;
   double tttmp;
   real val;
   real* lons;
   real* lats;
   real* zs;
   double* tags;
   bool *notrace;
   int* flags;
   int* statuses;
   real **stuff;
   int nstuff;
   int ii;
   int j;
   int n;
   Flock::iterator vi;
   bool i_am_root;
   Parcel* px;
   bool gotit;
   
   i_am_root = p.is_root();

   
   n = p.size();
   
   if ( n > 0 )  { 
   
      // first, allocate the space for this chunk
      lons = new real[maxchunk];
      lats = new real[maxchunk];
      zs   = new real[maxchunk];
      notrace = new bool[maxchunk];
      flags = NULL;
      if ( do_flags ) {
          flags = new int[maxchunk];
      }
      statuses = NULL;
      if ( do_status ) {
          statuses = new int[maxchunk];
      }
      tags = NULL;
      if ( do_tag ) {
          tags = new double[maxchunk];
      } 
      stuff = NULL;   
      nstuff = other.size();
      if ( nstuff > 0 ) {
         stuff = new real*[nstuff];
         for ( int i=0; i < nstuff; i++ ) {
             stuff[i] = new real[maxchunk];
         }
      }
       
      p.sync();
      
      // find the time for this batch of Parcels
      // (A better way to do this would be to have each
      // processor find the time for all of its share of the parcels.
      // then have the root processor select the best time from the other processors.)
      tt = dNaN;
      for ( int j=0; j < pnum; j++ ) {
          
          notrace[j] = true;
          
          try {
             // get the parcel.
             // If this is the root processor, we get a valid pointer.
             // If this is the non-root owner processor, we also get a valid pointer 
             // If this is a non-root non-owner processor for the ith parcel, we get NULLPTR
             // or a badparcelindex error.
             px =  p.parcel( j, 1 );
             // But we only want the case of the root processor
             gotit = ( ( px != NULLPTR ) && i_am_root );

          } catch (Swarm::badparcelindex()) {
            // no problem. ignore this. 
            gotit = false; 
          }; 
          if ( gotit ) {
             
             notrace[j] = px->queryNoTrace();
             
             tttmp = px->getTime();
             if ( j == 0 ) {
                ttbck = tttmp;
             } else {
                if ( dir == -1 ) {
                   // note: this works even if tyme is still NaN
                   if ( tttmp < tyme ) {
                      ttbck = tttmp;
                   }   
                } else if ( dir == 1 ) {
                   if ( tttmp > tyme ) {
                       ttbck = tttmp;
                   }
                }
             }
             if ( ! notrace[j] ) {
                // probably need to be more sophisticated about this
               tt = tttmp;
             }
          }   
      }
      if ( ! FINITE(tt) ) {
         // all Parcels in this batch are no-trace
         tt = ttbck;
         // this is probably a previously-used time
         // so shift this item by about 10 s.
         if ( dir == -1 ) {
            tt = ttbck - 1e-4;
         } else if ( dir == 1 ) {
            tt = ttbck + 1e-4;
         }
      } 
      if ( dbug > 50 ) {
         std::cerr << " output time tt=" << tt << std::endl;      
      }
      ii = 0;
      while ( ii < n ) {
   
         chunksize = n - ii;
         if ( chunksize > maxchunk ) {
            chunksize = maxchunk;
         }
         
        
         // now load the content
         for ( int i=0; i < chunksize; i++ ) {
         
             j = ii + i;
         
             try {
                // get the parcel.
                // If this is the root processor, we get a valid pointer.
                // If this is the non-root owner processor, we also get a valid pointer 
                // If this is a non-root non-owner processor for the ith parcel, we get NULLPTR
                // or a badparcelindex error.
                px =  p.parcel( j, 1 );
                // But we only want the case of the root processor
                gotit = ( ( px != NULLPTR ) && i_am_root );

             } catch (Swarm::badparcelindex()) {
               // no problem. ignore this. 
               gotit = false; 
             }; 
             if ( gotit ) {
                if ( ! notrace[j] ) {
                   // we only output parcels which are around the standard time for this batch of parcels
                   if (  abs( px->getTime() - tt ) > 1e-5 ) {
                      notrace[j] = true;
                   }
                }
             
                lons[i] = NaN;
                lats[i] = NaN;
                zs[i]   = NaN;
                if ( do_tag ) {
                   tags[i] = dNaN;
                }   
                if ( ! notrace[j] ) {
                   px->getPos( &(lons[i]), &(lats[i]) );
                   zs[i] = px->getZ();
                   
                   if ( do_tag ) {
                      tags[i] = px->tag();
                   }
                            
                }
                if ( do_flags ) {
                   flags[i] = px->flags();
                }
                if ( do_status ) {
                   statuses[i] = px->status();
                }
         
             }
         }
         if ( nstuff > 0 ) {
            for ( int i=0; i < nstuff; i++ ) {
               met->getData( other[i], tt, chunksize, lons, lats, zs, stuff[i], METDATA_NANBAD );
               for ( int k=0; k < chunksize; k++ ) {
                   j = ii + k;
                   if ( notrace[j] ) {
                      (stuff[i])[j] = NaN;
                   }
               }
            }
         }
        
         writeout( tt, chunksize, lons, lats, zs, flags, statuses, tags, stuff );
        
         // on to the next chunk
         ii = ii + chunksize;
         
      }
      // now free the space for this chunk
      if ( nstuff > 0 ) {
         for ( int i=0; i < nstuff; i++ ) {
             delete stuff[i];
         }
         delete stuff;
      }
      if ( tags != NULL ) {
         delete tags;
      }
      if ( statuses != NULL ) {
         delete statuses;
      }
      if ( flags != NULL ) {
         delete flags;
      } 
      delete zs;
      delete lats;
      delete lons;
   
   } else {
      // zero or fewer Parcels
      throw(badNetcdfBadNumberParcels());
   } 
   
}

void NetcdfOut::apply( Swarm& p )
{
   int chunksize;
   double t;
   double tt;
   real val;
   real* lons;
   real* lats;
   real* zs;
   double* tags;
   bool notrace;
   int* flags;
   int* statuses;
   real **stuff;
   int nstuff;
   int ii;
   int j;
   int n;
   Swarm::iterator vi;
   bool i_am_root;
   Parcel* px;
   bool gotit;
   
   i_am_root = p.is_root();

   
   n = p.size();
   
   if ( n > 0 )  { 
   
      // first, allocate the space for this chunk
      lons = new real[maxchunk];
      lats = new real[maxchunk];
      zs   = new real[maxchunk];
      flags = NULL;
      if ( do_flags ) {
          flags = new int[maxchunk];
      }
      statuses = NULL;
      if ( do_status ) {
          statuses = new int[maxchunk];
      }
      tags = NULL;
      if ( do_tag ) {
          tags = new double[maxchunk];
      }    
      nstuff = other.size();
      if ( nstuff > 0 ) {
         stuff = new real*[nstuff];
         for ( int i=0; i < nstuff; i++ ) {
             stuff[i] = new real[maxchunk];
         }
      }
       
      p.sync();
      
      // find the time for this batch of Parcels
      // (A better way to do this would be to have each
      // processor find the time for all of its share of the parcels.
      // then have the root processor select the best time from the other processors.)
      tt = 0.0;
      for ( int j=0; j < pnum; j++ ) {
          try {
             // get the parcel.
             // If this is the root processor, we get a valid pointer.
             // If this is the non-root owner processor, we also get a valid pointer 
             // If this is a non-root non-owner processor for the ith parcel, we get NULLPTR
             // or a badparcelindex error.
             px =  p.parcel( j, 1 );
             // But we only want the case of the root processor
             gotit = ( ( px != NULLPTR ) && i_am_root );

          } catch (Swarm::badparcelindex()) {
            // no problem. ignore this. 
            gotit = false; 
          }; 
          if ( gotit ) {
             notrace = px->queryNoTrace();
             if ( ! notrace ) {
                // probably need to be more sophisticated about this
               tt = px->getTime();
             }
          }   
      }
      if ( dbug > 50 ) {
         std::cerr << " output time tt=" << tt << std::endl;      
      }
      ii = 0;
      while ( ii < n ) {
   
         chunksize = n - ii;
         if ( chunksize > maxchunk ) {
            chunksize = maxchunk;
         }
         
        
         // now load the content
         for ( int i=0; i < chunksize; i++ ) {
         
             j = ii + i;
         
             try {
                // get the parcel.
                // If this is the root processor, we get a valid pointer.
                // If this is the non-root owner processor, we also get a valid pointer 
                // If this is a non-root non-owner processor for the ith parcel, we get NULLPTR
                // or a badparcelindex error.
                px =  p.parcel( j, 1 );
                // But we only want the case of the root processor
                gotit = ( ( px != NULLPTR ) && i_am_root );

             } catch (Swarm::badparcelindex()) {
               // no problem. ignore this. 
               gotit = false; 
             }; 
             if ( gotit ) {
                notrace = px->queryNoTrace();
                if ( ! notrace ) {
                   // we only output parcels which are out the standard time for this batch of parcels
                   if (  abs( px->getTime() - tt ) > 1e-5 ) {
                      notrace = true;
                   }
                }
             
                lons[i] = NaN;
                lats[i] = NaN;
                zs[i]   = NaN;
                if ( do_tag ) {
                   tags[i] = dNaN;
                }   
                if ( ! notrace ) {
                   px->getPos( &(lons[i]), &(lats[i]) );
                   zs[i] = px->getZ();
                   
                   if ( do_tag ) {
                      tags[i] = px->tag();
                   }
                            
                }
                if ( do_flags ) {
                   flags[i] = px->flags();
                }
                if ( do_status ) {
                   statuses[i] = px->status();
                }
         
             }
         }
         if ( nstuff > 0 ) {
            for ( int i=0; i < nstuff; i++ ) {
               met->getData( other[i], tt, chunksize, lons, lats, zs, stuff[i], METDATA_NANBAD );
               for ( int k=0; k < chunksize; k++ ) {
                   j = ii + k;
                   notrace = vi->queryNoTrace();
                   if ( notrace ) {
                      (stuff[i])[j] = NaN;
                   }
               }
            }
         }
        
         writeout( tt, chunksize, lons, lats, zs, flags, statuses, tags, stuff );
        
         // on to the next chunk
         ii = ii + chunksize;
         
      }
      // now free the space for this chunk
      if ( nstuff > 0 ) {
         for ( int i=0; i < nstuff; i++ ) {
             delete stuff[i];
         }
         delete stuff;
      }
      if ( tags != NULL ) {
         delete tags;
      }
      if ( statuses != NULL ) {
         delete statuses;
      }
      if ( flags != NULL ) {
         delete flags;
      } 
      delete zs;
      delete lats;
      delete lons;
   
   } else {
      // zero or fewer Parcels
      throw(badNetcdfBadNumberParcels());
   } 
   
}
