
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include <math.h>
#include <stdlib.h>

#include "gigatraj/GridCubedSphereFieldSfc.hh"

using namespace gigatraj;


const std::string GridCubedSphereFieldSfc::iam = "GridCubedSphereFieldSfc";


// Default constructor
GridCubedSphereFieldSfc::GridCubedSphereFieldSfc() : GridFieldSfc()
{   
   nlons = 0;
   londir = 0;
   nlats = 0; 
   latdir = 0;  
   wraps = 0;
   
}

GridCubedSphereFieldSfc::GridCubedSphereFieldSfc(int IM, int i1, int i2, int j1, int j2, int face): GridFieldSfc()
{
   iStart = i1;
   iEnd   = i2;
   jStart = j1;
   jEnd   = j2;
   nthFace = face;
   nlons = i2-i1 +1 + 2; // include halo 2
   nlats = j2-j1 +1 + 2; // include halo 2
   IM_WORLD = IM;
   londir = 0;
   latdir = 0;
   wraps = 0;

}


// Default destructor
GridCubedSphereFieldSfc::~GridCubedSphereFieldSfc()
{
};

// copy constructor
GridCubedSphereFieldSfc::GridCubedSphereFieldSfc(const GridCubedSphereFieldSfc& src) : GridFieldSfc(src)
{

         src.dims( &nlons, &nlats );
      
         lons = src.longitudes();
         lats = src.latitudes();
        wraps = src.wraps; 
       londir = src.londir;
       latdir = src.latdir;
       iStart = src.iStart;
        jStart = src.jStart;
        iEnd   = src.iEnd;
        jEnd   = src.jEnd;
        nlons  = src.nlons;
        nlats  = src.nlats;
        IM_WORLD = src.IM_WORLD;
         data = src.data;
}


void GridCubedSphereFieldSfc::clear() {
   
   lons.clear();
   lats.clear();
   
   nlons = 0;
   londir = 0;
   nlats = 0;
   latdir = 0;
   wraps = 0;
   
   GridFieldSfc::clear();
   
}

int GridCubedSphereFieldSfc::status() const
{
    int result = 0;
    
    result = GridFieldSfc::status();
    if ( nlons <= 0 || nlats <= 0 ) {
       result = result | GFS_NODIMS;
    }
    if ( data.size() != nlons*nlats ) {
       result = result | GFS_GRIDERR;
    }
    
    return result;

}


GridCubedSphereFieldSfc& GridCubedSphereFieldSfc::operator=(const GridCubedSphereFieldSfc& src)
{
    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }

    // copy the parent-class elements
    GridCubedSphereFieldSfc::assign(src);
    
    // copy the elements unique to this class
    src.dims( &nlons, &nlats );
      lons = src.longitudes();
    londir = src.londir;
      lats = src.latitudes();
    latdir = src.latdir;
     wraps = src.wraps; 
         iStart = src.iStart;
         jStart = src.jStart;
         iEnd   = src.iEnd;
         jEnd   = src.jEnd;
         nlons  = src.nlons;
         nlats  = src.nlats;
         IM_WORLD = src.IM_WORLD;
    return *this;
}      

void GridCubedSphereFieldSfc::dims( int* nlon, int* nlat)  const
{
   *nlon = nlons;
   *nlat = nlats;
};


real GridCubedSphereFieldSfc::value( int i, int j )  const
{
    int indx;

    if ( ! hasdata() ) {
        throw (baddatareq());
    }    
    
    // ensure that the longitude index is legal
    if ( wraps ) {
       // either wrap it
       i = iwrap( i );
    } else {
       // or test it
       if ( i < 0 || i >= nlons  ) {
           throw (baddatareq());
       }    
    }   
    // ensure that the latitude index is legal
    if ( j < 0 || j >= nlats ) {
        throw (baddatareq());
    }    
    
    // turn the two indices into a single index into the data array
    indx = j*nlons + i;
    
    return data[indx];
};


real& GridCubedSphereFieldSfc::valueref( int i, int j ) 
{
   real* result;
   int indx;

   if ( ! hasdata() ) {
       throw (baddatareq());
   }    
     
   // ensure that the longitude index is legal
   if ( wraps ) {
      i = iwrap( i );
   } else {
      if ( i < 0 || i >= nlons  ) {
          throw (baddatareq());
      }    
   }   
   // ensure that the latitude index is legal
   if ( j < 0 || j >= nlats ) {
       throw (baddatareq());
   }    

   // turn the two indices into a single index into the data array
   indx = j*nlons + i;
     
   result = &(data[indx]);
   return *result;
}        

real GridCubedSphereFieldSfc::value( int indx ) const
{
     real result;
   
     if ( ! hasdata() ) {
         std::cerr << "GridCubedSphereFieldSfc: has no data in value() call" << std::endl;
         throw (baddatareq());
     }    
     
     result = data[indx];

     return result;

}

real* GridCubedSphereFieldSfc::values( int n, real* vals, int *indices ) const
{
     int indx;
     int ref;
     int i,j;

     if ( ! hasdata() ) {
         std::cerr << "GridCubedSphereFieldSfc: has no data in values() call" << std::endl;
         throw (baddatareq());
     }    
     
     if ( n > 0 ) {
     
        if ( vals == NULLPTR ) {
           vals = new real[n];
        }
   
        for ( indx=0; indx < n; indx++ ) {
            vals[indx] = data[indices[indx]];
	    }
     }
        
     return vals;
};


std::vector<real> GridCubedSphereFieldSfc::longitudes()  const
{ 
   return lons;
};

std::vector<real> GridCubedSphereFieldSfc::latitudes()  const
{ 
   return lats;
};


real GridCubedSphereFieldSfc::longitude( int i )  const
{
/*   if ( wraps ) {
      i = iwrap( i );
   } else {
      if ( i < 0 || i >= nlons ) {
          throw (baddatareq());
      }
   }
*/
   return lons[i];
};
   
real GridCubedSphereFieldSfc::latitude( const int j ) const
{
/*   if ( j < 0 || j >= nlats ) {
       throw (baddatareq());
   }    
*/
   return lats[j];
};   
 

real GridCubedSphereFieldSfc::wrap( real lon ) const
{
     real extlon;
     
     if ( nlons <= 1 ) {
        throw (baddataindex());
     }
     
     if ( lons[0] < lons[1] ) {
     
        // calculate the max longitude
        extlon = lons[0] + 360.0;
     
        // longitudes increase with index
        while ( lon < lons[0] ) {
           lon = lon + 360.0;
        }
        while ( lon > extlon ) {
           lon = lon - 360.0;
        } 
        // in case we went one notch too far       
        if ( lon < lons[0] ) {
           lon = lon + 360.0;
        }
     } else {
        // calculate the min longitude
        extlon = lons[0] - 360.0;
     
        // longitudes decrease with index
        while ( lon > lons[0] ) {
           lon = lon - 360.0;
        }
        while ( lon < extlon ) {
           lon = lon + 360.0;
        }        
        if ( lon > lons[0] ) {
           lon = lon - 360.0;
        }
     }
     
     return lon;

};

int GridCubedSphereFieldSfc::iwrap( int i ) const
{
  /*   
     if ( nlons <= 1 ) {
        throw (baddataindex());
     }
     
     if ( wraps ) {
        while ( i < 0 ) {
           i = i + nlons;
        }
        while ( i >= nlons ) {
           i = i - nlons;
        } 
     } else {
        if ( i < 0 || i >= nlons ) {
           throw (baddataindex());
        }
     }
   */  
     return i;

};

/*void GridCubedSphereFieldSfc::lonindex( real lon, int* i1, int* i2 ) const
{
     int i;
     
     lon = wrap(lon);
     
     if ( nlons <= 1 ) {
        throw (baddataindex());
     }

     if ( londir > 0 ) {
        // longitudes increase

        // find the highest-indexed lon that is below the test lon
        i = 0;
        *i1 = -1;
        while ( i < nlons && lons[i] < lon ) {
        
           if ( lons[i] < lon ) {
              *i1 = i;
           } 
           i++;
        }
        if ( *i1 != -1 ) {
           // we have a lower bound for the test lon in lons
        
           // do we have an upper bound?
           if ( *i1 < (nlons-1) ) {
              *i2 = *i1 + 1;
           } else {
              if ( wraps ) {
                 *i2 = nlons;
              } else {
                 // test val lies above range of lons
                 throw (baddataindex());
              }           
           }   
        } else {
              if ( wraps ) {
                 *i1 = -1;
                 *i2 =  0;
              } else {
                 // test lon lies below the range of lons
                 throw (baddataindex());
              }   
        }
     } else {
        // longitudes decrease

        // find the lowest-indexed lon that is below the test lon
        i = nlons-1;
        *i2 = -1;
        while ( i >= 0 && lons[i] < lon ) {
        
           if ( lons[i] < lon ) {
              *i2 = i;
           } 
        
           i--;
        }
        if ( *i2 != -1 ) {
           // we have an lower bound for the test lon in lons
        
           // do we have an upper bound?
           if ( *i2 > 0 ) {
              *i1 = *i2 - 1;
           } else {
              if ( wraps ) {
                 *i1 = nlons-1;
                 *i2 = nlons;
              } else {
                 // test val lies above range of lons
                 throw (baddataindex());
              }           
           }   
        } else {
              if ( wraps ) {
                 *i1 = 0;
                 *i2 = -1;
              } else {
                 // test lon lies below the range of lons
                 throw (baddataindex());
              }   
        }
     
     }


};

void GridCubedSphereFieldSfc::latindex( real lat, int* i1, int* i2 ) const
{
     int i;
     
     if ( nlats <= 1 ) {
        throw (baddataindex());
     }


     if ( latdir > 0 ) {
        // latitudes increase

        // find the highest-indexed lat that is below the test lat
        i = 0;
        *i1 = -1;
        while ( i < nlats && lats[i] < lat ) {
        
           if ( lats[i] < lat ) {
              *i1 = i;
           } 
           i++;
        }
        if ( *i1 != -1 ) {
           // we have a lower bound for the test lat in lats
        
           // do we have an upper bound?
           if ( *i1 < (nlats-1) ) {
              *i2 = *i1 + 1;
           } else {
              if ( ABS( lats[nlats-1] - lat ) > 0.0001 ) {
                 // test val lies above range of lats
                 throw (baddataindex());
              } else {
                 // close enough
                 *i2 = nlats-1;
                 *i1 = *i2 - 1;
              }           
           }   
        } else {
              if ( ABS( lats[0] - lat ) > 0.0001 ) {
                 // test lat lies below the range of lats
                 throw (baddataindex());
              } else {
                 *i1 = 0;
                 *i2 = 1;
              }   
        }
     } else {
        // latitudes decrease

        // find the lowest-indexed lat that is below the test lat
        i = nlats-1;
        *i2 = -1;
        while ( i >= 0 && lats[i] < lat ) {
        
           if ( lats[i] < lat ) {
              *i2 = i;
           } 
        
           i--;
        }
        if ( *i2 != -1 ) {
           // we have an lower bound for the test lat in lats
        
           // do we have an upper bound?
           if ( *i2 > 0 ) {
              *i1 = *i2 - 1;
           } else {
              if ( ABS( lats[0] - lat ) > 0.0001 ) {
                 // test val lies above range of lats
                 throw (baddataindex());
              } else {
                 // close enough
                 *i1 = 0;
                 *i2 = 1;
              }           
           }   
        } else {
              if ( ABS( lats[nlats-1] - lat ) > 0.0001 ) {
                 // test lat lies below the range of lats
                 throw (baddataindex());
              } else {
                 *i1 = nlats-2;
                 *i2 = nlats-1;
              }   
        }
     
     }

};

*/

void GridCubedSphereFieldSfc::latlonindex(real lat_deg, real lon_deg, int& i, int& j ) const
{
   double shift  = 0.174532925199433;
   double tolerance    = 10.e-7;

   double pi = 3.1415926535;
   double c2r = pi/180.0;
   double lon = lon_deg*c2r;
   double lat = lat_deg*c2r;

   lon = lon + shift;

   double x = cos(lat)*cos(lon);
   double y = cos(lat)*sin(lon);
   double z = sin(lat);

   double max_abs = std::max(std::abs(z),std::max(std::abs(x), std::abs(y)));

   x = x/max_abs;
   y = y/max_abs;
   z = z/max_abs;

   // face = 1
   if ( std::abs(x-1.0) <= tolerance) {
       angle_to_index( y, z, i, j);
   }
  // face = 2
   else if (std::abs(y-1.0) <= tolerance) {
         angle_to_index(-x,  z, i, j);
   }

   // face = 3
   else if (abs(z-1.0) <= tolerance) {
         angle_to_index(-x, -y, i, j);
    }
   // face = 4
    else if (abs(x+1.0) <= tolerance) {
         angle_to_index(-z, -y, i, j);
    }
    // face = 5
    else if (abs(y+1.0) <= tolerance) {
         angle_to_index(-z,  x, i, j);
    }
    // face = 6
    else if (abs(z+1.0) <= tolerance) {
         angle_to_index(y,  x, i, j);
    }
}

void GridCubedSphereFieldSfc::angle_to_index(double xval, double yval, int& i, int& j) const
{
   double sqr2   = 1.41421356237310;
   double alpha  = 0.615479708670387;
   double dalpha = alpha/IM_WORLD;

   i = int(std::floor((std::atan(xval/sqr2) + alpha)/dalpha));
   j = int(std::floor((std::atan(yval/sqr2) + alpha)/dalpha));
   if (j == 2 * IM_WORLD) {j = 2*IM_WORLD-1;}
   if (i == 2 * IM_WORLD) {i = 2*IM_WORLD-1;}
}


void GridCubedSphereFieldSfc::setLonDir( const int loadFlags )
{
   londir = 0;
   if ( nlons > 1 ) {
      londir = ( lons[1] > lons[0] ) ? 1 : -1;
   }
}

void GridCubedSphereFieldSfc::setLatDir( const int loadFlags )
{
   latdir = 0;
   if ( nlats > 1 ) {
      latdir = ( lats[1] > lats[0] ) ? 1 : -1;
   }
}

void GridCubedSphereFieldSfc::setWraps( const int loadFlags )
{
   real dlon;
   real past_end;
   
   // set the wrap flag
   if ( loadFlags & ( GFL_WRAP | GFL_NOWRAP ) ) {
      // the wrap flag was specificed explicitly
      if ( loadFlags & GFL_NOWRAP ) {
         wraps = 0;
      }   
      if ( loadFlags & GFL_WRAP ) {
         wraps = 1;
      }
   } else {
      // wrap flag was not specified explicitly.
      // guess it from the longitude values
      
      // find the delta between longitudes
      dlon = lons[1] - lons[0];
      // use the delta to go oine past the last longuitd,e and then wrap around
      if ( dlon > 0 ) {
         past_end =  lons[nlons-1] + dlon - 360.0;
      } else {
         past_end =  lons[nlons-1] + dlon + 360.0;      
      }
      // now see if the past_end longitude is much different fomr the first longitude
      // Note: it does not have to be exact, and the longitudes do not have to
      // be regularly-spaced. All we are testing for is
      // whether they are close enough that we can consider the longitudes
      // to cover the globe
      
      wraps =  ( ABS( past_end - lons[0] ) <  ABS(dlon/4.0) );
   
   }
}





void GridCubedSphereFieldSfc::load( const realvec& inlons, const realvec& inlats, const realvec& indata, const int loadFlags )
{
   int indx = 0;
   int i;
   
   //checkLons(inlons);
   //checkLats(inlats);
      
   //nlons = inlons.size();
   //nlats = inlats.size();
   
   lons = inlons;
   for ( i=1; i<nlons*nlats; i++ ) {
       lons[i] = wrap(inlons[i]);
   }    
   lats = inlats;
   
   data.resize( nlons*nlats );

   // copy the data   
   for (int indx=0; indx<nlons*nlats; indx++ ) {
       data[indx] = indata[indx];
   }
   clear_nodata();
   
   // determine the longitudinal wrapping
   setWraps(loadFlags);
      
   /// determine the direction in which latitude and longitude go
   setLonDir( loadFlags );
   setLatDir( loadFlags );


}

void GridCubedSphereFieldSfc::load( const realvec& inlons, const realvec& inlats, const int loadFlags )
{
   int indx = 0;
   real dlon;
   int i;
   
  // checkLons(inlons);
  // checkLats(inlats);
   
   //nlons = inlons.size();
   //nlats = inlats.size();
   
   lons = inlons;
   //for ( i=1; i<nlons; i++ ) {
   //    lons[i] = wrap(inlons[i]);
   //}    
   lats = inlats;
   
   
   // Don't try to copy any data 
   data.clear();  
   set_nodata();
   if ( loadFlags & GFL_PREFILL ) {
      for ( i=0; i<nlons*nlats; i++ ) {
         data.push_back( fill_value );
      }
      clear_nodata();   
   }
   
   // determine the longitudinal wrapping
   setWraps(loadFlags);
      
   /// determine the direction in which latitude and longitude go
   setLonDir( loadFlags );
   setLatDir( loadFlags );
   
}

void GridCubedSphereFieldSfc::load( const realvec& indata, const int loadFlags )
{
   int indx = 0;
   real dlon;
   int i;
   if ( nlons*nlats != indata.size() ) {
      throw(badincompatcoords());
   }      
   
   data = indata;
   clear_nodata();


   /// note: since the longitudes and latitudes are unchanged,
   /// we do not call set_Wraps(), setLonDir(), or setLatDir() here.   

}


bool GridCubedSphereFieldSfc::compatible( const GridFieldSfc& obj, int compatFlags ) const  
{
    int result = true;
    std::vector<real> dim;
    const GridCubedSphereFieldSfc *trueobj;
    
    if ( obj.id() == id() ) {
    
       result = GridFieldSfc::compatible( obj, compatFlags );
       
       trueobj = dynamic_cast<const GridCubedSphereFieldSfc*>(&obj);
    
       if ( compatFlags & METCOMPAT_HORIZ ) {
          // check that the longitudes match in number and values
          dim = trueobj->longitudes();
          if ( dim.size() == nlons*nlats ) {
             for ( int i=0; i<nlons*nlats; i++ ) {
                // longitudes must be within 0.001 degrees
                if ( ABS( dim[i] - lons[i] ) > 0.001 ) {
                   result = false;
                }   
             }
          }   
          // check that the latitudes match in number and values
          dim = trueobj->latitudes();
          if ( dim.size() == nlats*nlons ) {
             for ( int i=0; i<nlats*nlons; i++ ) {
                // latitudes must be within 0.001 degrees
                if ( ABS( dim[i] - lats[i] ) > 0.001 ) {
                   result = false;
                }   
             }
          } else {
             result = false;
          }             
       }

    } else {
       // different grid types are by definition incompatible
       result = false;
    }   
    
    return result;      
       
}

bool GridCubedSphereFieldSfc::match( const GridCubedSphereFieldSfc& obj ) const  
{
    int result = true;
    std::vector<real> dim;
    
    if ( ! this->compatible(obj) ) {
       result = false;
    }
    
    if ( obj.quantity() != quant 
    ||   obj.units() != uu  
    ||   obj.fillval() != fill_value ) {
       result = false;
    }   

    return result;      
       
}

int GridCubedSphereFieldSfc::doeswrap() const
{
   return wraps;
}   

real GridCubedSphereFieldSfc::area(int i, int j) const
{
    real dlon, dlat;
    real lat1, lat2, lon1, lon2;
    real ar;

    if ( j > 0 ) {
       lat1 = (lats[j-1] + lats[j])/2.0;
    } else {
       lat1 = lats[j];
    }      
    if ( j < (nlats-1) ) {
       lat2 = (lats[j] + lats[j+1])/2.0;
    } else {
       lat2 = lats[j];
    }
    dlat = ABS( SIN(lat2/180.0*PI) - SIN(lat1/180.0*PI) );

    if ( i > 0 ) {
       lon1 = ( lons[i-1] + lons[i] )/2.0;
    } else {
       // i == 0 
       if ( wraps ) {
          if ( lons[nlons-1] > lons[0] ) {
             lon1 = (lons[0] +  (lons[nlons-1] - 360.0) )/2.0;
          } else {
             lon1 = (lons[0] +  (lons[nlons-1] + 360.0) )/2.0;
          }
       } else {
          lon1 = lons[0];
       }
    }   
    if ( i < (nlons-1) ) {
       lon2 = ( lons[i] + lons[i+1] )/2.0;
    } else {
       if ( wraps ) {
          if ( lons[0] < lons[nlons-2] ) {
             lon2 = ((lons[0] + 360.0) + lons[nlons-1])/2.0;
          } else {
             lon2 = ((lons[0] - 360.0) + lons[nlons-1])/2.0;
          }       
       } else {
          lon2 = lons[i];
       }
    }
    
    dlon = ABS( lon2 - lon1 );
    while ( dlon > 360.0 ) {
       dlon = dlon - 360.0;
    }   

    ar = dlon/180.0*PI * dlat;


    return ar;
}

real GridCubedSphereFieldSfc::area(int i) const
{
    int ii,jj;
    
    ii = i % nlons;
    jj = i / nlons;
    
    return area(ii,jj);
    
}

GridFieldSfc* GridCubedSphereFieldSfc::areas() const
{
    GridCubedSphereFieldSfc* result;
    std::vector<real> newdata;
    real ar;
    real dlon, dlat;
    real lat1, lat2, lon1, lon2;
    int maxlon;
    
    result = dynamic_cast<GridCubedSphereFieldSfc*>(this->duplicate());

    result->set_quantity( "normalized_area" );
    result->set_units( "" );
    result->set_fillval( -999.0 );
    result->set_surface( "" );
    
    newdata.reserve( nlons*nlats );
    for ( int j=0; j<nlats; j++ ) {
        if ( j > 0 ) {
           lat1 = (lats[j-1] + lats[j])/2.0;
        } else {
           lat1 = lats[j];
        }      
        if ( j < (nlats-1) ) {
           lat2 = (lats[j] + lats[j+1])/2.0;
        } else {
           lat2 = lats[j];
        }
        dlat = ABS( SIN(lat2/180.0*PI) - SIN(lat1/180.0*PI) );
        for ( int i=0; i<nlons; i++ ) {
           
           if ( i > 0 ) {
              lon1 = ( lons[i-1] + lons[i] )/2.0;
           } else {
              // i == 0 
              if ( wraps ) {
                 if ( lons[nlons-1] > lons[0] ) {
                    lon1 = (lons[0] +  (lons[nlons-1] - 360.0) )/2.0;
                 } else {
                    lon1 = (lons[0] +  (lons[nlons-1] + 360.0) )/2.0;
                 }
              } else {
                 lon1 = lons[0];
              }
           }   
           if ( i < (nlons-1) ) {
              lon2 = ( lons[i] + lons[i+1] )/2.0;
           } else {
              if ( wraps ) {
                 if ( lons[0] < lons[nlons-2] ) {
                    lon2 = ((lons[0] + 360.0) + lons[nlons-1])/2.0;
                 } else {
                    lon2 = ((lons[0] - 360.0) + lons[nlons-1])/2.0;
                 }       
              } else {
                 lon2 = lons[i];
              }
           }
           
           dlon = ABS( lon2 - lon1 );
           while ( dlon > 360.0 ) {
              dlon = dlon - 360.0;
           }   
           
           ar = dlon/180.0*PI * dlat;
           
           newdata.push_back( ar );
       }
    }
    result->load( lons, lats, newdata );    

    return result;


}



GridFieldSfc* GridCubedSphereFieldSfc::duplicate() const
{
    GridCubedSphereFieldSfc* newgrid;
   
    newgrid = new GridCubedSphereFieldSfc();
    
    *newgrid = *this;

    return newgrid;

}

int GridCubedSphereFieldSfc::dataSize() const
{
    if ( nlons == 0 || nlats == 0 ) {
       throw (badnodims());
    }
    
    return nlons*nlats;   

}



void GridCubedSphereFieldSfc::gridpoints( int n, int* is, int* js, real* vals, int flags) const
{
     int* coords;
     int cmd;
     int local;
     int done;
     
     local = flags & 0x01;
     done = flags * 0x02;

     // get the data from a central met processors    
     try {
        coords = new int[n];
     } catch(...) {
        throw (badmemreq());
     }
     
     joinIndex( n, coords, is, js );

     if ( pgroup == NULLPTR || metproc < 0 || local != 0 ) {
         // serial processing.  Access the data locally
         this->values( n, vals, coords );
         //for ( int i=0; i<n; i++ ) {
         //    vals[i] = this->value(is[i],js[i]);
         //}
     } else {
     
         // Never ask a dedicated met processor to fetch gridpoint data for itself
         if ( metproc == pgroup->id() ) {
            throw (badProcReq());
         }
     
         // send data request to central met reader process
         //cmd = PGR_CMD_GDATA;
         //pgroup->send_ints( metproc, 1, &cmd, PGR_TAG_GREQ );
         // send request for n points 
         pgroup->send_ints( metproc, 1, &n, PGR_TAG_GNUM );
         // send the coordinates
         pgroup->send_ints( metproc, n, coords, PGR_TAG_GCOORDS );
         // receive the values
         pgroup->receive_reals( metproc, n, vals, PGR_TAG_GVALS );

         if ( done ) {
            svr_done();
         }
     
     }

     delete coords;
}


void GridCubedSphereFieldSfc::gridpoints( int n, int* indices, real* vals, int flags) const
{
     int cmd;
     int local;
     int done;
     
     local = flags & 0x01;
     done = flags * 0x02;


     if ( metproc < 0 || local != 0 ) {
         // serial processing.  Access the data locally
         //for ( int i=0; i<n; i++ ) {
         //    vals[i] = this->value(is[i],js[i]);
         //}
         this->values( n, vals, indices );
     } else {
     
         // Never ask a dedicated met processor to fetch gridpoint data for itself
         if ( metproc == pgroup->id() ) {
            throw (badProcReq());
         }
     
         // send data request to central met reader process
         //cmd = PGR_CMD_GDATA;
         //pgroup->send_ints( metproc, 1, &cmd, PGR_TAG_GREQ );
         // send request for n points 
         pgroup->send_ints( metproc, 1, &n, PGR_TAG_GNUM );
         // send the coordinates
         pgroup->send_ints( metproc, n, indices, PGR_TAG_GCOORDS );
         // receive the values
         pgroup->receive_reals( metproc, n, vals, PGR_TAG_GVALS );

         if ( done ) {
            svr_done();
         }
     
     }

}

void GridCubedSphereFieldSfc::splitIndex( int index, int* i, int* j ) const
{
    *i = index % nlons;
    *j = index / nlons;

}

void GridCubedSphereFieldSfc::splitIndex( int n, int *index, int* i, int* j ) const
{
    int m;
    
    if ( n > 0 ) {
    
       for ( m=0; m < n; m++ ) {
          // now get the indices in that level
          i[m] = index[m] % nlons;
          j[m] = index[m] / nlons;
       }
    }

}

int GridCubedSphereFieldSfc::joinIndex( int i, int j ) const
{
     int result;
     
     // ensure that the longitude index is legal
     if ( wraps ) {
        i = iwrap( i );
     } else {
        if ( i < 0 || i >= nlons  ) {
            std::cerr << "GridCubedSphereFieldSfc: out-of-range longitude index " << i 
            << " in joinIndex() call" << std::endl;
            throw (baddatareq());
        }    
     }   
     // ensure that the latitude index is legal
     if ( j < 0 || j >= nlats ) {
         std::cerr << "GridCubedSphereFieldSfc: out-of-range latitude or vertical index "  
         << j << " of " << nlats << " in joinIndex() call" << std::endl;
         throw (baddatareq());
     }    

     result = j*nlons + i;

     return result;


}

int* GridCubedSphereFieldSfc::joinIndex( int n, int *index, int* i, int* j ) const
{
     
     if ( n > 0 ) {
     
        if ( index == NULLPTR ) {
           index = new int[n];
        }
     
        for ( int m=0; m<n; m++ ) {           // ensure that the longitude index is legal
            if ( wraps ) {
               i[m] = iwrap( i[m] );
            } else {
               if ( i[m] < 0 || i[m] >= nlons  ) {
                   std::cerr << "GridCubedSphereFieldSfc: out-of-range longitude index " 
                   << i[m] << " in joinIndex() call" << std::endl;
                   throw (baddatareq());
               }    
            }   
            // ensure that the latitude index is legal                                                   
            if ( j[m] < 0 || j[m] >= nlats ) {                                                                                   
                std::cerr << "GridCubedSphereField3D:  out-of-range latitude or vertical index "  << j[m]   
                << " of " << nlats << " in joinIndex() call" << std::endl;                       
                throw (baddatareq());                                                                                    
            }
                                                                                                                 
            index[m] = j[m]*nlons + i[m];
        }
     }
     
     return index;
}


void GridCubedSphereFieldSfc::receive_meta()
{
     real* dimvals;
     int cmd;
     int i;

     if (  pgroup == NULLPTR || metproc < 0 ) {
         // serial processing.  Load nothing, but
         // check that the object has valid metadata
         if ( this->status() & (~0x10) ) {
            throw (baddataload());
         }
         
     } else {
     
         // Never ask a dedicated met processor to fetch metadata for itself
         if ( metproc == pgroup->id() ) {
            throw (badProcReq());
         }
     
         // receive theta metadata
         //- std::cerr << "   GridCubedSphereField3D::receive_meta: r-100 with " << metproc << std::endl;
         pgroup->receive_string( metproc, &quant, PGR_TAG_GMETA ); // quantity
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-110 from " << metproc  << std::endl;
         pgroup->receive_string( metproc, &uu, PGR_TAG_GMETA );  // units
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-120 from " << metproc  << std::endl;
         pgroup->receive_string( metproc, &ctime, PGR_TAG_GMETA );  // met time stamp
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-130 from " << metproc  << std::endl;
         pgroup->receive_doubles( metproc, 1, &mtime, PGR_TAG_GMETA );  // met time 
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-140 from " << metproc  << std::endl;
         pgroup->receive_reals( metproc, 1, &fill_value, PGR_TAG_GMETA );  // fill value
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-150 from " << metproc  << std::endl;
         pgroup->receive_ints( metproc, 1, &wraps, PGR_TAG_GMETA );  // longitude wrapping flag
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-160 from " << metproc  << std::endl;
         pgroup->receive_ints( metproc, 1, &nlons, PGR_TAG_GMETA );  // number of lons
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-170 from " << metproc  << std::endl;
         pgroup->receive_ints( metproc, 1, &nlats, PGR_TAG_GMETA );  // number of lats
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-180 from " << metproc  << std::endl;
         pgroup->receive_string( metproc, &sfc, PGR_TAG_GMETA ); // quantity
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-190 from " << metproc  << std::endl;
         pgroup->receive_ints( metproc, 1, &wraps, PGR_TAG_GMETA );  // do lons wrap?
         //- std::cerr << "   GridCubedSphereFieldSfc::receive_meta: r-200 from " << metproc  << std::endl;

         set_nodata();  // there are no data
   
         // get the longitudes
         try {
            dimvals = new real[nlons];
         } catch(...) {
            throw (badmemreq());
         }
         pgroup->receive_reals( metproc, nlons, dimvals, PGR_TAG_GDIMS ); 
         lons.clear();
         for ( i=0; i<nlons; i++ ) {
             lons.push_back(dimvals[i]);
         }    
         delete dimvals;
         if ( nlons > 1 ) {
             if ( lons[1] > lons[0] ) {
                londir = 1;
             }    
             if ( lons[1] < lons[0] ) {
                londir = -1;
             }
         }

         // get the latitudes
         try {
            dimvals = new real[nlats];
         } catch(...)  {
            throw (badmemreq());
         }
         pgroup->receive_reals( metproc, nlats, dimvals, PGR_TAG_GDIMS ); 
         lats.clear();
         for ( i=0; i<nlats; i++ ) {
             lats.push_back(dimvals[i]);
         }    
         delete dimvals;
         if ( nlats > 1 ) {
             if ( lats[1] > lats[0] ) {
                latdir = 1;
             }    
             if ( lats[1] < lats[0] ) {
                latdir = -1;
             }
         }

         metaID = 0;
   
     }

}

void GridCubedSphereFieldSfc::svr_send_meta(int id) const
{
     real* dimvals;
     int cmd;
     int i;

     if ( metproc < 0 ) {
         // serial processing.  Send nothing, but
         // check that the object has valid metadata
         if ( this->status() & (~0x10) ) {
            throw (baddataload());
         }
         
     } else {
     
         // Ask only a dedicated met processor to send metadata
         if ( metproc != pgroup->id() ) {
            throw (badProcReq());
         }

         // by the time we get here, the receiving processor
         // has issued its request for metadata, and
         // the calling routine has already received that request
         // and called this method.

         // receive the metadata
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-100 with " << id << std::endl;
         pgroup->send_string( id, quant, PGR_TAG_GMETA ); // quantity
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-110 to " << id << std::endl;
         pgroup->send_string( id, uu, PGR_TAG_GMETA );  // units
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-120 to " << id << std::endl;
         pgroup->send_string( id, ctime, PGR_TAG_GMETA );  // met time stamp
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-130 to " << id << std::endl;
         pgroup->send_doubles( id, 1, &mtime, PGR_TAG_GMETA );  // met time 
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-140 to " << id << std::endl;
         pgroup->send_reals( id, 1, &fill_value, PGR_TAG_GMETA );  // fill value
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-150 to " << id << std::endl;
         pgroup->send_ints( id, 1, &wraps, PGR_TAG_GMETA );  // longitude wrapping flag
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-160 to " << id << std::endl;
         pgroup->send_ints( id, 1, &nlons, PGR_TAG_GMETA );  // number of lons
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-170 to " << id << std::endl;
         pgroup->send_ints( id, 1, &nlats, PGR_TAG_GMETA );  // number of lats
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-180 to " << id << std::endl;
         pgroup->send_string( id, sfc, PGR_TAG_GMETA ); // surface quantity
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-190 to " << id << std::endl;
         pgroup->send_ints( id, 1, &wraps, PGR_TAG_GMETA );  // do lons wrap?
         //- std::cerr << "   GridCubedSphereFieldSfc::svr_send_meta: s-200 to " << id << std::endl;

         // send the longitudes
         try  {
            dimvals = new real[nlons];
         } catch(...)  {
            throw (badmemreq());
         }
         for ( i=0; i<nlons; i++ ) {
             dimvals[i] = lons[i];
         }    
         pgroup->send_reals( id, nlons, dimvals, PGR_TAG_GDIMS ); 
         delete dimvals;

         // send the latitudes
         try {
            dimvals = new real[nlats];
         } catch(...)  {
            throw (badmemreq());
         }
         for ( i=0; i<nlats; i++ ) {
             dimvals[i] = lats[i];
         }    
         pgroup->send_reals( id, nlats, dimvals, PGR_TAG_GDIMS ); 
         delete dimvals;
   
     }

}

void GridCubedSphereFieldSfc::serialize(std::ostream& os) const
{
  int prec;
  string str;
  int len;
  int i, j;
  double t;
  real val;
  int ival;
  const char *cstr;
  int version = 1;

  
  try {
  
      GridFieldSfc::serialize(os);
      
      // output the version
      os.write( reinterpret_cast<char *>( &version), static_cast<std::streamsize>( sizeof(int)));

      // output the number of longitudes
      ival = nlons;
      os.write( reinterpret_cast<char *>( &ival), static_cast<std::streamsize>( sizeof(int)));
      // output the number of latitudes
      ival = nlats;
      os.write( reinterpret_cast<char *>( &ival), static_cast<std::streamsize>( sizeof(int)));
  
      // output the longitudes
      for ( i=0; i<nlons; i++ ) {
          val = lons[i];
          os.write( reinterpret_cast<char *>( &val), static_cast<std::streamsize>( sizeof(real)));
      }    
      // output the latitudes
      for ( i=0; i<nlats; i++ ) {
          val = lats[i];
          os.write( reinterpret_cast<char *>( &val), static_cast<std::streamsize>( sizeof(real)));
      }    
      // output the data
      for ( j=0; j<nlats; j++ ) {
      for ( i=0; i<nlons; i++ ) {
          val = value(i,j);
          os.write( reinterpret_cast<char *>( &val), static_cast<std::streamsize>( sizeof(real)));
      }
      }
    

  } catch (...) {
      throw;
  }    

}

void GridCubedSphereFieldSfc::deserialize(std::istream& is)
{
   string str;
   int len;
   char cc;
   int i;
   double t;
   real val;
   int ival;
   int nxlons, nxlats;
   std::vector<real> xlons, xlats, xdata;
   int version;
   
   clear();

   try {
   
       GridFieldSfc::deserialize(is);
      
       // read the version
       is.read(reinterpret_cast<char *>( &version), static_cast<std::streamsize>( sizeof(int)));
       
       
       // read the number of longitudes
       is.read( reinterpret_cast<char *>(&nxlons), static_cast<std::streamsize>( sizeof(int)) );
       // read the number of latitudes
       is.read( reinterpret_cast<char *>(&nxlats), static_cast<std::streamsize>( sizeof(int)) );
       
       // read the longitudes
       xlons.reserve(nxlons);
       for ( i=0; i<nxlons; i++ ) {
           is.read( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real)) );
           xlons.push_back(val);
       }    
       // read the latitudes
       xlats.reserve(nxlats);
       for ( i=0; i<nxlats; i++ ) {
           is.read( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real)) );
           xlats.push_back(val);
       }    
       // read the data
       xdata.reserve(nxlats*nxlons);
       for ( i=0; i<nxlats*nxlons; i++ ) {
           is.read( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real) ));
           xdata.push_back(val);
       }    
              
       // call the load method for checking, setting nlats, latdir, etc.
       load( xlons, xlats, xdata );
       

   } catch (...) {
       throw;
   }    

}

void GridCubedSphereFieldSfc::getindices( int index, int* i, int*j ) const
{
    *i = index % nlons;
    *j = index / nlons;

}


//.......................................................................................



namespace gigatraj {

// outputs the Met Field 
std::ostream& operator<<( std::ostream& os, const GridCubedSphereFieldSfc& p)
{
  
  try {
  
      p.serialize(os);
  

  } catch (...) {
      throw;
  }    
  
  return os;

};

// inputs the Met field
std::istream& operator>>( std::istream& is, GridCubedSphereFieldSfc& p)
{

   try {
   
       p.deserialize(is);

   } catch (...) {
       throw;
   }    
   
   return is;

};

}
