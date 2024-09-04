
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

#include <math.h>
#include <stdlib.h>

#include "gigatraj/GridLatLonField3D.hh"

using namespace gigatraj;


const std::string GridLatLonField3D::iam = "GridLatLonField3D";


// Default constructor
GridLatLonField3D::GridLatLonField3D(): GridField3D() 
{
   
   nlons = 0;
   londir = 0;
   nlats = 0;
   latdir = 0;  
   wraps = 0;

}

// Default destructor
GridLatLonField3D::~GridLatLonField3D()
{
};

// copy constructor
GridLatLonField3D::GridLatLonField3D(const GridLatLonField3D& src): GridField3D(src)
{

       src.dims( &nlons, &nlats, &nzs );
         lons = src.longitudes();
         lats = src.latitudes();
        wraps = src.wraps;

}

void GridLatLonField3D::clear() {
   
   lons.clear();
   lats.clear();

   nlons = 0;
   londir = 0;
   nlats = 0;
   latdir = 0;
   wraps = 0;

   GridField3D::clear();
   
}

int GridLatLonField3D::status() const
{
    int result = 0;

    result = GridField3D::status();
    
    if ( nlons <= 0 || nlats <= 0 ) {
       result = result | GFS_NODIMS;
    }
    if ( data.size() != nlons*nlats*nzs ) {
       result = result | GFS_GRIDERR;
    }
    
    return result;

}



GridLatLonField3D& GridLatLonField3D::operator=(const GridLatLonField3D& src)
{
    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }
    
    // copy the parent-class elements
    GridLatLonField3D::assign(src);
    
    // copy the elements unique to this class
    src.dims( &nlons, &nlats, &nzs );
         lons = src.longitudes();
       londir = src.londir;
         lats = src.latitudes();
       latdir = src.latdir;
        wraps = src.wraps; 

    return *this;

}      

void GridLatLonField3D::dims( int* nlon, int* nlat, int* nv)  const
{
   *nlon = nlons;
   *nlat = nlats;
   *nv = nzs;
};

real GridLatLonField3D::value( int i, int j, int k )  const
{
     int indx;
     real result;
   
     if ( ! hasdata() ) {
         std::cerr << "GridLatLonField3D: has no data in value() call" << std::endl;
         throw (baddatareq());
     }    
     
     // ensure that the longitude index is legal
     if ( wraps ) {
        i = iwrap( i );
     } else {
        if ( i < 0 || i >= nlons  ) {
            std::cerr << "GridLatLonField3D: out-of-range longitude index " << i << " in value() call" << std::endl;
            throw (baddatareq());
        }    
     }   
    // ensure that the latitude and vertical indices are legal
     if ( j < 0 || j >= nlats 
       || k < 0 || k >= nzs ) {
         std::cerr << "GridLatLonField3D: Quant=" << quant << " out-of-range latitude or vertical index "  << j << ", " << k << " of " << nlats << ", " << nzs << " in value() call" << std::endl;
         throw (baddatareq());
     }    
     
     // turn the three indices into a single index into the data array
     indx = (k*nlats + j)*nlons + i;
     result = data[indx];

     return result;
};

real GridLatLonField3D::value( int indx )  const
{
     real result;
   
     if ( ! hasdata() ) {
         std::cerr << "GridLatLonField3D: has no data in value() call" << std::endl;
         throw (baddatareq());
     }    
     
     result = data[indx];

     return result;
};

real* GridLatLonField3D::values( int n, real* vals, int *indices ) const
{
     int indx;
     int ref;

     if ( ! hasdata() ) {
         std::cerr << "GridLatLonField3D: has no data in values() call" << std::endl;
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

real& GridLatLonField3D::valueref( int i, int j, int k )  
{
     real *result;
     int indx;

     if ( ! hasdata() ) {
         std::cerr << "GridLatLonField3D: has no data in valueref() call" << std::endl;
         throw (baddatareq());
     }    
     
     // ensure that the longitude index is legal
     if ( wraps ) {
        i = iwrap( i );
     } else {
        if ( i < 0 || i >= nlons  ) {
            std::cerr << "GridLatLonField3D: out-of-range longitude index " << i << " in valueref() call" << std::endl;
            throw (baddatareq());
        }    
     }   
    // ensure that the latitude nad vertical indices are legal
     if ( j < 0 || j >= nlats 
       || k < 0 || k >= nzs ) {
         std::cerr << "GridLatLonField3D: out-of-range latitude or vertical index " << j << ", " << k << " in valueref() call" << std::endl;
         throw (baddatareq());
     }    
     
     // turn the three indices into a single index into the data array
     indx = (k*nlats + j)*nlons + i;
     
     result = &(data[indx]);
     return *result;
}        



std::vector<real> GridLatLonField3D::longitudes()  const
{ 
   return lons;
};

std::vector<real> GridLatLonField3D::latitudes()  const
{ 
   return lats;
};


real GridLatLonField3D::longitude( int i )  const
{
   if ( wraps ) {
      i = iwrap( i );
   } else {
      if ( i < 0 || i >= nlons ) {
          std::cerr << "GridLatLonField3D: out-of-range longitude index " << i << " in longitude() call" << std::endl;
          throw (baddatareq());
      }
   }
       
   return lons[i];
};
   
real GridLatLonField3D::latitude( const int j ) const
{
   if ( j < 0 || j >= nlats ) {
       std::cerr << "GridLatLonField3D: out-of-range latitude index " << j << " in latitude() call" << std::endl;
       throw (baddatareq());
   }    
   return lats[j];
};   

real GridLatLonField3D::wrap( real lon ) const
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

int GridLatLonField3D::iwrap( int i ) const
{
     
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
     
     return i;

};

void GridLatLonField3D::lonindex( real lon, int* i1, int* i2 ) const
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

void GridLatLonField3D::latindex( real lat, int* i1, int* i2 ) const
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


void GridLatLonField3D::setLonDir( const int loadFlags )
{
   londir = 0;
   if ( nlons > 1 ) {
      londir = ( lons[1] > lons[0] ) ? 1 : -1;
   }
}

void GridLatLonField3D::setLatDir( const int loadFlags )
{
   latdir = 0;
   if ( nlats > 1 ) {
      latdir = ( lats[1] > lats[0] ) ? 1 : -1;
   }
}


void GridLatLonField3D::setWraps( const int loadFlags )
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



void GridLatLonField3D::load( const realvec& inlons, const realvec& inlats
     , const realvec& inlevels
     , const realvec& indata, const int loadFlags )
{
   int indx = 0;
   int i;
   
   checkLons(inlons);
   checkLats(inlats);
   checkLevs(inlevels);
   
   nlons = inlons.size();
   nlats = inlats.size();
   nzs = inlevels.size();
   
   lons = inlons;
   for ( i=1; i<inlons.size(); i++ ) {
       lons[i] = wrap(inlons[i]);
   }
   lats = inlats;
   zs = inlevels;
   
   
   data.resize( nlons*nlats*nzs );

   // copy the data   
   for (int indx=0; indx<nlons*nlats*nzs; indx++ ) {
       data[indx] = indata[indx];
   }
   clear_nodata();
   
   
   // determine the longitudinal wrapping
   setWraps(loadFlags);
      
   /// determine the direction in which latitude and longitude go
   setLonDir( loadFlags );
   setLatDir( loadFlags );
   setZDir( loadFlags );


}


void GridLatLonField3D::load( const realvec& inlons, const realvec& inlats
     , const realvec& inlevels, const int loadFlags )
{
   int indx = 0;
   int i;
   
   checkLons(inlons);
   checkLats(inlats);
   checkLevs(inlevels);
   
   nlons = inlons.size();
   nlats = inlats.size();
   nzs = inlevels.size();
   
   lons = inlons;
   for ( i=1; i<inlons.size(); i++ ) {
       lons[i] = wrap(inlons[i]);
   }
   lats = inlats;
   zs = inlevels;
   
   
   // do not copy any data   
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
   setZDir( loadFlags );

}


void GridLatLonField3D::load( const realvec& indata, const int loadFlags )
{
   int indx = 0;
   int i;
   
   if ( nlons*nlats*nzs != indata.size() ) {
      throw(badincompatcoords());
   }      
   
   data = indata;
   clear_nodata();

   /// note: since the longitudes and latitudes are unchanged,
   /// we do not call set_Wraps(), setLonDir(), or setLatDir() here.   

}


bool GridLatLonField3D::compatible( const GridField3D& obj, int compatFlags ) const
{
    int result = true;
    std::vector<real> dim;
    const GridLatLonField3D *trueobj;
    
    if ( obj.id() == this->id() ) {
    
       result = GridField3D::compatible( obj, compatFlags );
    
       trueobj = dynamic_cast<const GridLatLonField3D*>( &obj );
       
       if ( compatFlags & METCOMPAT_HORIZ ) {
          // check that the longitudes match in number and values
          dim = trueobj->longitudes();
          if ( dim.size() == nlons ) {
             for ( int i=0; i<nlons; i++ ) {
                // longitudes must be within 0.001 degrees
                if ( ABS( dim[i] - lons[i] ) > 0.001 ) {
                   result = false;
                }   
             }
          }  else {
             result = false;
          }          
          // check that the latitudes match in number and values
          dim = trueobj->latitudes();
          if ( dim.size() == nlats ) {
             for ( int i=0; i<nlats; i++ ) {
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
       // objects of different grid types are by definition
       // incompatible
       result = false;
    }   


    return result;      
       
}


bool GridLatLonField3D::compatible( const GridFieldSfc& obj, int compatFlags ) const
{
    int result = true;
    std::vector<real> dim;
    
    const GridLatLonFieldSfc *trueobj;
    
    // we can only compare surface grids of this type
    if ( obj.id() == "GridLatLonFieldSfc" ) {
    
       result = GridField3D::compatible( obj, compatFlags );
       //if ( ! result ) {
       //    std::cerr << " GridField3D returns incompat!" << std::endl;
       //}
    
       trueobj = dynamic_cast<const GridLatLonFieldSfc*>( &obj );
       
       if ( compatFlags & METCOMPAT_HORIZ ) {
          // check that the longitudes match in number and values
          dim = trueobj->longitudes();
          if ( dim.size() == nlons ) {
             for ( int i=0; i<nlons; i++ ) {
                // longitudes must be within 0.001 degrees
                if ( ABS( dim[i] - lons[i] ) > 0.001 ) {
                   //if ( result ) {
                   //    std::cerr << " incompatible grid: lon value " << dim[i] << "vs. " << lons[i] << std::endl;
                   //}
                   result = false;
                }   
             }
          } else { 
             //std::cerr << " incompatible grid: nlons " << nlons << " vs. " << dim.size() << std::endl;
             result = false;
          }   
          // check that the latitudes match in number and values
          dim = trueobj->latitudes();
          if ( dim.size() == nlats ) {
             for ( int i=0; i<nlats; i++ ) {
                // latitudes must be within 0.001 degrees
                if ( ABS( dim[i] - lats[i] ) > 0.001 ) {
                   //if ( result ) {
                   //    std::cerr << " incompatible grid: lat values " << dim[i] << "vs. " << lats[i]  << std::endl;
                   //}
                   result = false;
                }   
             }
          } else {
             //std::cerr << " incompatible grid: nlats " << nlats << " vs. " << dim.size() << std::endl;
             result = false;
          }             
    
       }
    } else {
       // objects of different grid types are by definition
       // incompatible
       //std::cerr << " incompatible grid: type " << obj.id() << std::endl;
       result = false;
    }   

    return result;      
       
}

bool GridLatLonField3D::match( const GridLatLonField3D& obj ) const  
{
    int result = true;
    std::vector<real> dim;
    
    if ( ! this->compatible(obj) ) {
       result = false;
    }
    
    if ( obj.quantity() != quant 
    ||   obj.units() != uu  
    ||   obj.vertical() != vquant
    ||   obj.vunits() != vuu
    ||   obj.fillval() != fill_value ) {
       result = false;
    }   
    
    return result;      
       
}

int GridLatLonField3D::doeswrap() const
{
   return wraps;
}   


GridLatLonFieldSfc* GridLatLonField3D::extractSfc( int k ) const
{
    GridLatLonFieldSfc* result;
    std::vector<real> newdata;
    std::string name;
    int indx;
    
    if ( k < 0 || k >= nzs ) {
       std::cerr << "bad vertical index " << k << " in extractSfc() call" << std::endl;
       throw (baddatareq());
    }
    
    result = new GridLatLonFieldSfc;
    result->set_quantity( quant );
    result->set_units( uu );
    result->mksScale = mksScale;
    result->mksOffset = mksOffset;
    result->set_fillval( fill_value );
    result->set_time( mtime, ctime );
    
    name = zs[k]; 
    name = name + " " + vuu + " " + vquant + "surface";
    result->set_surface( name );
    
    newdata.reserve( nlons*nlats );
    for ( int j=0; j<nlats; j++ ) {
       for ( int i=0; i<nlons; i++ ) {
           indx = (k*nlats + j)*nlons + i;
           newdata.push_back( data[indx] );
       }
    }
    result->load( lons, lats, newdata );    

    return result;

}

GridFieldSfc* GridLatLonField3D::extractSurface( int k ) const
{
   return extractSfc(k);
}

void GridLatLonField3D::replaceLevel( GridFieldSfc& sfc, int k)
{
    real val;
    
    
    if ( this->compatible(sfc) ) {
    
      for ( int j=0; j<nlats; j++ ) {
         for ( int i=0; i<nlons; i++ ) {
             // scale the source data to MKS
             val = sfc(i,j) * sfc.mksScale + sfc.mksOffset;
             // scale the MKS data to our units
             this->valueref(i,j,k) = (val - this->mksOffset )/this->mksScale;
         }
      }

    } else {
       throw (badgrid());
    }

}


real GridLatLonField3D::area(int i, int j) const
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

real GridLatLonField3D::area(int i) const
{
    int ii,jj;
    
    ii = i % nlons;
    jj = i / nlons;
    
    return area(ii,jj);
    
}




GridFieldSfc* GridLatLonField3D::areas() const
{
    GridLatLonFieldSfc* result;
    GridLatLonFieldSfc* slice;
    std::vector<real> newdata;
    real ar;
    real dlon, dlat;
    real lat1, lat2, lon1, lon2;
    
    slice = this->extractSfc(0);
    result = dynamic_cast<GridLatLonFieldSfc*>(slice->areas());

    delete slice;
    
    return result;


}

GridField3D* GridLatLonField3D::duplicate() const
{
    GridLatLonField3D* newgrid;
   
    newgrid = new GridLatLonField3D();
    
    *newgrid = *this;

    return newgrid;

}

int GridLatLonField3D::dataSize() const
{
    if ( nzs == 0 || nlons == 0 || nlats == 0 ) {
       throw (badnodims());
    }
    
    return nlons*nlats*nzs;   

}

void GridLatLonField3D::gridpoints( int n, int* is, int* js, int* ks, real* vals, int flags) const
{
     int* coords;
     int cmd;
     int local;
     int done;
     
     local = flags & 0x01;
     done = flags * 0x02;

     //- std::cerr << " parallel gridpoints" << std::endl;
     // get the data from a central met processors    
     try {
        coords = new int[n];
     } catch(...) {
        throw (badmemreq());
     }
     
     joinIndex( n, coords, is, js, ks );
     
     if ( pgroup == NULLPTR || metproc < 0 || local != 0 ) {
         //- std::cerr << " serial gridpoints" << std::endl;
         // serial processing.  Access the data locally
         this->values( n, vals, coords );
         //for ( int i=0; i<n; i++ ) {
         //    vals[i] = this->value(is[i],js[i],ks[i]);
         //}
     } else {

         // send "get data" command to central met reader process
         //cmd = PGR_CMD_GDATA;
         // pgroup->send_ints( metproc, 1, &cmd, PGR_TAG_GREQ );
         //- std::cerr << "  about to send N to " << metproc << std::endl;
         // send request for n points 
         pgroup->send_ints( metproc, 1, &n, PGR_TAG_GNUM );
         //- std::cerr << "  sent N=" << n << " to " << metproc << std::endl;
         // send the coordinates
         pgroup->send_ints( metproc, n, coords, PGR_TAG_GCOORDS );
         //- std::cerr << "  sent pnt indices to " << metproc << std::endl;
         // receive the values
         pgroup->receive_reals( metproc, n, vals, PGR_TAG_GVALS );
         //- std::cerr << " yyyyyyyyyyyy: got " << n << " values " << std::endl;
     
         if ( done ) {
            //- std::cerr << " sending GDONE " << std::endl;
            svr_done();
         }
         
     }
     
     delete[] coords;

}

void GridLatLonField3D::gridpoints( int n, int* indices, real* vals, int flags) const
{
     int cmd;
     int local;
     int done;
     
     local = flags & 0x01;
     done = flags * 0x02;

     if ( pgroup == NULLPTR || metproc < 0 || local != 0 ) {
         //- std::cerr << " serial gridpoints" << std::endl;
         // serial processing.  Access the data locally
         this->values( n, vals, indices );

     } else {


         // send "get data" command to central met reader process
         //cmd = PGR_CMD_GDATA;
         // pgroup->send_ints( metproc, 1, &cmd, PGR_TAG_GREQ );
         //- std::cerr << "  about to send N to " << metproc << std::endl;
         // send request for n points 
         pgroup->send_ints( metproc, 1, &n, PGR_TAG_GNUM );
         //- std::cerr << "  sent N=" << n << " to " << metproc << std::endl;
         // send the coordinates
         pgroup->send_ints( metproc, n, indices, PGR_TAG_GCOORDS );
         //- std::cerr << "  sent pnt indices to " << metproc << std::endl;
         // receive the values
         pgroup->receive_reals( metproc, n, vals, PGR_TAG_GVALS );
         //- std::cerr << " yyyyyyyyyyyy: got " << n << " values " << std::endl;
     
         if ( done ) {
            //- std::cerr << " sending GDONE " << std::endl;
            svr_done();
         }
         
     }
     
}

void GridLatLonField3D::receive_meta()
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
     
         // receive the metadata
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-100 with " << metproc << std::endl;
         pgroup->receive_string( metproc, &quant, PGR_TAG_GMETA ); // quantity
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-110 from " << metproc  << std::endl;
         pgroup->receive_string( metproc, &uu, PGR_TAG_GMETA );  // units
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-120 from " << metproc  << std::endl;
         pgroup->receive_string( metproc, &ctime, PGR_TAG_GMETA );  // met time stamp
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-130 from " << metproc  << std::endl;
         pgroup->receive_doubles( metproc, 1, &mtime, PGR_TAG_GMETA );  // met time 
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-140 from " << metproc  << std::endl;
         pgroup->receive_reals( metproc, 1, &fill_value, PGR_TAG_GMETA );  // fill value
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-150 from " << metproc  << std::endl;
         pgroup->receive_ints( metproc, 1, &wraps, PGR_TAG_GMETA );  // longitude wrapping flag
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-160 from " << metproc  << std::endl;
         pgroup->receive_ints( metproc, 1, &nlons, PGR_TAG_GMETA );  // number of lons
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-170 from " << metproc  << std::endl;
         pgroup->receive_ints( metproc, 1, &nlats, PGR_TAG_GMETA );  // number of lats
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-180 from " << metproc  << std::endl;
         pgroup->receive_ints( metproc, 1, &nzs, PGR_TAG_GMETA );  // number of vertical coordinate values
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-190 from " << metproc  << std::endl;
         pgroup->receive_string( metproc, &vquant, PGR_TAG_GMETA );  // vertical coordinate quantity
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-200 from " << metproc  << std::endl;
         pgroup->receive_string( metproc, &vuu, PGR_TAG_GMETA );  // vertical coordinate units
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-210 from " << metproc << std::endl;
         pgroup->receive_ints( metproc, 1, &wraps, PGR_TAG_GMETA );  // do lons wrap?
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-220 from " << metproc << std::endl;

         set_nodata();  // there are no data
   
         // get the longitudes
         try {
            dimvals = new real[nlons];
         } catch(...) {
            throw (badmemreq());
         }
         pgroup->receive_reals( metproc, nlons, dimvals, PGR_TAG_GDIMS ); 
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-230: " << nlons << " from " << metproc << std::endl;
         lons.clear();
         for ( i=0; i<nlons; i++ ) {
             lons.push_back(dimvals[i]);
         }    
         delete[] dimvals;
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
         } catch(...) {
            throw (badmemreq());
         }
         pgroup->receive_reals( metproc, nlats, dimvals, PGR_TAG_GDIMS ); 
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-240: " << nlats << " from " << metproc << std::endl;
         lats.clear();
         for ( i=0; i<nlats; i++ ) {
             lats.push_back(dimvals[i]);
         }    
         delete[] dimvals;
         if ( nlats > 1 ) {
             if ( lats[1] > lats[0] ) {
                latdir = 1;
             }    
             if ( lats[1] < lats[0] ) {
                latdir = -1;
             }
         }

         // get the vertical coordinates
         try {
            dimvals = new real[nzs];
         } catch(...) {
            throw (badmemreq());
         }
         pgroup->receive_reals( metproc, nzs, dimvals, PGR_TAG_GDIMS ); 
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-250: " << nzs << " from " << metproc << std::endl;
         zs.clear();
         for ( i=0; i<nzs; i++ ) {
             zs.push_back(dimvals[i]);
         }    
         delete[] dimvals;
         if ( nzs > 1 ) {
             if ( zs[1] > zs[0] ) {
                zdir = 1;
             }    
             if ( zs[1] < zs[0] ) {
                zdir = -1;
             }
         }
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-260" << std::endl;

         metaID = 0;
   
     }

}



void GridLatLonField3D::svr_send_meta(int id) const
{
     real* dimvals;
     int cmd;
     int i;

     //- std::cerr << "   GridLatLonField3D::svr_send_meta: entry" << std::endl;
     if (  pgroup == NULLPTR || metproc < 0 ) {
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: 490 metproc = " << metproc << std::endl;
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
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-100 with " << id << std::endl;
         pgroup->send_string( id, quant, PGR_TAG_GMETA ); // quantity
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-110 to " << id << std::endl;
         pgroup->send_string( id, uu, PGR_TAG_GMETA );  // units
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-120 to " << id << std::endl;
         pgroup->send_string( id, ctime, PGR_TAG_GMETA );  // met time stamp
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-130 to " << id << std::endl;
         pgroup->send_doubles( id, 1, &mtime, PGR_TAG_GMETA );  // met time 
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-140 to " << id << std::endl;
         pgroup->send_reals( id, 1, &fill_value, PGR_TAG_GMETA );  // fill value
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-150 to " << id << std::endl;
         pgroup->send_ints( id, 1, &wraps, PGR_TAG_GMETA );  // longitude wrapping flag
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-160 to " << id << std::endl;
         pgroup->send_ints( id, 1, &nlons, PGR_TAG_GMETA );  // number of lons
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-170 to " << id << std::endl;
         pgroup->send_ints( id, 1, &nlats, PGR_TAG_GMETA );  // number of lats
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-180 to " << id << std::endl;
         pgroup->send_ints( id, 1, &nzs, PGR_TAG_GMETA );  // number of vertical coordinate values
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-190 to " << id << std::endl;
         pgroup->send_string( id, vquant, PGR_TAG_GMETA ); // vertical coordinate quantity
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-200 to " << id << std::endl;
         pgroup->send_string( id, vuu, PGR_TAG_GMETA ); // vertical coordinate units
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-210 to " << id << std::endl;
         pgroup->send_ints( id, 1, &wraps, PGR_TAG_GMETA );  // do lons wrap?
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-220 to " << id << std::endl;

         // send the longitudes
         try {
            dimvals = new real[nlons];
         } catch(...) {
            throw (badmemreq());
         }
         for ( i=0; i<nlons; i++ ) {
             dimvals[i] = lons[i];
         }    
         pgroup->send_reals( id, nlons, dimvals, PGR_TAG_GDIMS ); 
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-230: " << nlons << " to " << id << std::endl;
         delete[] dimvals;

         // send the latitudes
         try {
            dimvals = new real[nlats];
         } catch(...) {
            throw (badmemreq());
         }
         for ( i=0; i<nlats; i++ ) {
             dimvals[i] = lats[i];
         }    
         pgroup->send_reals( id, nlats, dimvals, PGR_TAG_GDIMS ); 
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-240:" << nlats << " to " << id << std::endl;
         delete[] dimvals;
   
         // send the vertical coordinate values
         try {
            dimvals = new real[nzs];
         } catch(...) {
            throw (badmemreq());
         }
         for ( i=0; i<nzs; i++ ) {
             dimvals[i] = zs[i];
         }    
         pgroup->send_reals( id, nzs, dimvals, PGR_TAG_GDIMS ); 
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-250: " << nzs << " to" << id << std::endl;
         delete[] dimvals;
   
         //- std::cerr << "   GridLatLonField3D::svr_send_meta: s-260" << std::endl;
     }

}



void GridLatLonField3D::serialize(std::ostream& os) const
{
  int prec;
  string str;
  int len;
  int i, j, k;
  double t;
  real val;
  int ival;
  int version = 1;
  
  try {    

      GridField3D::serialize(os);
      
      // output the version
      os.write( reinterpret_cast<char *>( &version), static_cast<std::streamsize>( sizeof(int)));
      
      // output the number of longitudes
      ival = nlons;
      os.write( reinterpret_cast<char *>(&ival), static_cast<std::streamsize>( sizeof(int)));
      // output the number of latitudes
      ival = nlats;
      os.write( reinterpret_cast<char *>(&ival), static_cast<std::streamsize>( sizeof(int)));
      // output the number of levels
      ival = nzs;
      os.write( reinterpret_cast<char *>(&ival), static_cast<std::streamsize>( sizeof(int)));
    
      // output the longitudes
      for ( i=0; i<nlons; i++ ) {
          val = lons[i];
          os.write( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real)));
      }    
      // output the latitudes
      for ( i=0; i<nlats; i++ ) {
          val = lats[i];
          os.write( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real)));
      }    
      // output the levels
      for ( i=0; i<nzs; i++ ) {
          val = zs[i];
          os.write( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real)));
      }    
      // output the data
      for ( k=0; k<nzs; k++ ) {
      for ( j=0; j<nlats; j++ ) {
      for ( i=0; i<nlons; i++ ) {
          val = value(i,j,k);
          os.write( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real)));
      }
      }
      }
    

  } catch (...) {
      throw;
  }    

}

void GridLatLonField3D::deserialize(std::istream& is)
{
   string str;
   int len;
   char cc;
   int i;
   double t;
   real val;
   int ival;
   int nxlons, nxlats, nxzs;
   std::vector<real> xlons, xlats, xzs, xdata;
   int version;

   clear();   

   try {
   
       GridField3D::deserialize(is);
      
       // read the version
       is.read(reinterpret_cast<char *>( &version), static_cast<std::streamsize>( sizeof(int)));
              
       // read the number of longitudes
       is.read( reinterpret_cast<char *>(&nxlons), static_cast<std::streamsize>( sizeof(int) ));
       // read the number of latitudes
       is.read( reinterpret_cast<char *>(&nxlats), static_cast<std::streamsize>( sizeof(int) ));
       // read the number of levels
       is.read( reinterpret_cast<char *>(&nxzs), static_cast<std::streamsize>( sizeof(int) ));
       
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
       // read the levels
       xzs.reserve(nxzs);
       for ( i=0; i<nxzs; i++ ) {
           is.read( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real)) );
           xzs.push_back(val);
       }    
       // read the data
       xdata.reserve(nxlats*nxlons*nxzs);
       for ( i=0; i<nxlats*nxlons*nxzs; i++ ) {
           is.read( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real)) );
           xdata.push_back(val);
       }    

       load(xlons,xlats,xzs,xdata);

   } catch (...) {
       throw;
   }    
   


}

/****************** Iterators **************************/


GridField3D::iterator GridLatLonField3D::begin()
{
   
   return GridField3D::iterator( this, 0, 0, data.size()-1 );

}

GridField3D::iterator GridLatLonField3D::end()
{
   
   return GridField3D::iterator( this, data.size(), 0, data.size()-1  );

}

GridField3D::iterator GridLatLonField3D::begin(int k)
{
   int start, end;
   
   start = nlons*nlats*k;
   end = nlons*nlats*(k+1) - 1;   
   
   return GridField3D::iterator( this, start, start, end );
   
}

GridField3D::iterator GridLatLonField3D::end(int k)
{
   int start, end;
   
   start = nlons*nlats*k;
   end = nlons*nlats*(k+1) - 1;
   
   return GridField3D::iterator( this, end+1, start, end );
   
}


///////////////////////////////


GridField3D::const_iterator GridLatLonField3D::begin() const
{
   
   return GridField3D::const_iterator( this, 0, 0, data.size()-1 );

}

GridField3D::const_iterator GridLatLonField3D::end() const
{
   
   return GridField3D::const_iterator( this, data.size(), 0, data.size()-1 );
   
}

GridField3D::const_iterator GridLatLonField3D::begin(int k) const
{
   int start, end;
   
   start = nlons*nlats*k;
   end = nlons*nlats*(k+1) - 1;   

   return GridField3D::const_iterator( this, start, start, end );

}


GridField3D::const_iterator GridLatLonField3D::end(int k) const
{
   int start, end;
   
   start = nlons*nlats*k;
   end = nlons*nlats*(k+1) - 1;   
   
   return GridField3D::const_iterator( this, end+1, start, end );

}

///////////////////////////////



GridField3D::profileIterator GridLatLonField3D::profileBegin()
{

   return GridField3D::profileIterator( this, 0, nzs, 0, nlons*nlats-1, nlons*nlats );
   
}

GridField3D::profileIterator GridLatLonField3D::profileEnd()
{
   
   return GridField3D::profileIterator( this, nlons*nlats, nzs, 0, nlons*nlats-1, nlons*nlats  );

}


///////////////////////////////


GridField3D::const_profileIterator GridLatLonField3D::profileBegin() const
{
   
   return GridField3D::const_profileIterator( this, 0, nzs, 0, nlons*nlats-1, nlons*nlats );

}

GridField3D::const_profileIterator GridLatLonField3D::profileEnd() const
{
   
   return GridField3D::const_profileIterator( this, nlons*nlats, nzs, 0, nlons*nlats-1, nlons*nlats  );

}

GridField3D::profileIterator GridLatLonField3D::profileBegin( int i, int j )
{
   
   return GridField3D::profileIterator( this, i+j*nlons, nzs, 0, nlons*nlats-1, nlons*nlats );

}

GridField3D::const_profileIterator GridLatLonField3D::profileBegin( int i, int j ) const
{
   
   return GridField3D::const_profileIterator( this, i+j*nlons, nzs, 0, nlons*nlats-1, nlons*nlats );
   
}

void GridLatLonField3D::splitIndex( int index, int* i, int*j, int*k ) const
{
    int subindex;
    
    // reduce the index to lie in the first vertical level
    subindex = index % (nlats*nlons);
    // now get the indices in that level
    *i = subindex % nlons;
    *j = subindex / nlons;
    *k = index / (nlats*nlons);

}

void GridLatLonField3D::splitIndex( int n, int *index, int* i, int* j, int* k ) const
{
    int subindex;
    int m;
    
    if ( n > 0 ) {
    
       for ( m=0; m < n; m++ ) {
          // reduce the index to lie in the first vertical level
          subindex = index[m] % (nlats*nlons);
      
          // now get the indices in that level
          i[m] = subindex % nlons;
          j[m] = subindex / nlons;
          k[m] = index[m] / (nlats*nlons);
       }
    }
}

int GridLatLonField3D::joinIndex( int i, int j, int k ) const
{
     int result;
     
     // ensure that the longitude index is legal
     if ( wraps ) {
        i = iwrap( i );
     } else {
        if ( i < 0 || i >= nlons  ) {
            std::cerr << "GridLatLonField3D: out-of-range longitude index " << i << " in joinIndex() call" << std::endl;
            throw (baddatareq());
        }    
     }   
     // ensure that the latitude and vertical indices are legal
     if ( j < 0 || j >= nlats 
       || k < 0 || k >= nzs ) {
         std::cerr << "GridLatLonField3D: out-of-range latitude or vertical index "  << j << ", " << k << " of " << nlats << ", " << nzs << " in joinIndex() call" << std::endl;
         throw (baddatareq());
     }    

     result = (k*nlats + j)*nlons + i;

     return result;
}

int* GridLatLonField3D::joinIndex( int n, int *index, int* i, int* j, int* k ) const{
     
     if ( n > 0 ) {
     
        if ( index == NULLPTR ) {
           index = new int[n];
        }
     
        for ( int m=0; m<n; m++ ) {           // ensure that the longitude index is legal
            if ( wraps ) {
               i[m] = iwrap( i[m] );
            } else {
               if ( i[m] < 0 || i[m] >= nlons  ) {
                   std::cerr << "GridLatLonField3D: out-of-range longitude index " << i[m] << " in joinIndex() call" << std::endl;
                   throw (baddatareq());
               }    
            }   
            // ensure that the latitude and vertical indices are legal                                                   
            if ( j[m] < 0 || j[m] >= nlats                                                                                     
              || k[m] < 0 || k[m] >= nzs ) {                                                                                   
                std::cerr << "GridLatLonField3D:  out-of-range latitude or vertical index "  << j[m]   
                << ", " << k[m] << " of " << nlats << ", " << nzs << " in joinIndex() call" << std::endl;                       
                throw (baddatareq());                                                                                    
            }
                                                                                                                 
            index[m] = (k[m]*nlats + j[m])*nlons + i[m];
        }
     }
     
     return index;
}

//.......................................................................................

namespace gigatraj {

// outputs the Met Field 
std::ostream& operator<<( std::ostream& os, const GridLatLonField3D& p)
{
  
  try {

      p.serialize(os);
  
  } catch (...) {
      throw;
  }    
  
  return os;

};

// inputs the Met field
std::istream& operator>>( std::istream& is, GridLatLonField3D& p)
{

   try {
   
      p.deserialize(is);

   } catch (...) {
       throw;
   }    
   
   return is;

};

}
