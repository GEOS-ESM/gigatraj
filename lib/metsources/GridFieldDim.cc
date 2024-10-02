
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

#include "gigatraj/GridFieldDim.hh"

using namespace gigatraj;


const std::string GridFieldDim::iam = "GridFieldDim";


// constructor
GridFieldDim::GridFieldDim() : GridField()
{
   
   nzs = 0;
   zdir = 0;
   ctime = "N/A";
   mtime = 0.0;
   
}


// destructor
GridFieldDim::~GridFieldDim()
{
};

// copy constructor
GridFieldDim::GridFieldDim(const GridFieldDim& src) : GridField(src)
{

   zdir = src.zdir;
   nzs = src.nzs;

}



void GridFieldDim::assign( const GridFieldDim& src )
{
   
   GridField::assign(src);
   
   zdir = src.zdir;
   nzs = src.nzs;

}

void GridFieldDim::clear() {
   
   GridField::clear();

   nzs = 0;
   zdir = 0;
   ctime = "N/A";
   mtime = 0.0;
      
}

int GridFieldDim::status() const
{
    int result = 0;
    
    result = GridField::status();

    if ( nzs <= 0 ) {
       result = result | GFS_NODIMS;
    }
    if ( data.size() != nzs ) {
       result = result | GFS_GRIDERR;
    }
    
    return result;

}


void GridFieldDim::index( real z, int* i1, int* i2 ) const
{
     int i;
     
     if ( nzs <= 1 ) {
        throw (baddataindex());
     }

     if ( zdir > 0 ) {
        // levels increase

        // find the highest-indexed z that is below the test z
        i = 0;
        *i1 = -1;
        while ( i < nzs && data[i] < z ) {
        
           if ( data[i] < z ) {
              *i1 = i;
           } 
           i++;
        }
        if ( *i1 != -1 ) {
           // we have a lower bound for the test z in zs
        
           // do we have an upper bound?
           if ( *i1 < (nzs-1) ) {
              *i2 = *i1 + 1;
           } else {
              if ( ABS( data[nzs-1] - z ) > 0.0001 ) {
                 // test val lies above range of zs
                 throw (baddataindex());
              } else {
                 // close enough
                 *i2 = nzs-1;
                 *i1 = *i2 - 1;
              }           
           }   
        } else {
              if ( ABS( data[0] - z ) > 0.0001 ) {
                 // test z lies below the range of zs
                 throw (baddataindex());
              } else {
                 *i1 = 0;
                 *i2 = 1;
              }   
        }
     } else {
        // levels decrease

        // find the lowest-indexed z that is below the test z
        i = nzs-1;
        *i2 = -1;
        while ( i >= 0 && data[i] < z ) {
        
           if ( data[i] < z ) {
              *i2 = i;
           } 
        
           i--;
        }
        if ( *i2 != -1 ) {
           // we have an lower bound for the test z in zs
        
           // do we have an upper bound?
           if ( *i2 > 0 ) {
              *i1 = *i2 - 1;
           } else {
              if ( ABS( data[0] - z ) > 0.0001 ) {
                 // test val lies above range of zs
                 throw (baddataindex());
              } else {
                 // close enough
                 *i1 = 0;
                 *i2 = 1;
              }           
           }   
        } else {
              if ( ABS( data[nzs-1] - z ) > 0.0001 ) {
                 // test z lies below the range of zs
                 throw (baddataindex());
              } else {
                 *i1 = nzs-2;
                 *i2 = nzs-1;
              }   
        }
     
     }

};


real GridFieldDim::value( int i )  const
{
    
    if ( ! hasdata() ) {
        throw (baddatareq());
    }    
    
    // ensure that the coordinate index is legal
    if ( i < 0 || i >= nzs ) {
        throw (baddatareq());
    }    
    
    return data[i];
};

void GridFieldDim::set_value( int i, real val )
{
    if ( ! hasdata() ) {
        throw (baddatareq());
    }    
    
    // ensure that the coordinate index is legal
    if ( i < 0 || i >= nzs ) {
        throw (baddatareq());
    }    
    if ( i > 0 ) {
       if (  (zdir > 0) && (data[i - 1] >= val) ) {
          throw (badNonmonotonic());
       } else if ( (zdir < 0) && (data[i - 1] <= val) ) {
          throw (badNonmonotonic());
       }    
    }
    if ( i < (nzs - 1) ) {
       if (  (zdir > 0) && (data[i + 1] <= val) ) {
          throw (badNonmonotonic());
       } else if ( (zdir < 0) && (data[i + 1] >= val) ) {
          throw (badNonmonotonic());
       }    
    }

    data[i] = val;

}    


real* GridFieldDim::values( int n, real* vals, int *indices ) const
{
     int indx;
     int ref;
     int i,j;

     if ( ! hasdata() ) {
         std::cerr << "GridFieldDim: has no data in values() call" << std::endl;
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

int GridFieldDim::size()  const
{
   return nzs;
};

real GridFieldDim::operator()( int i ) const 
{
   return value(i);
}        

void GridFieldDim::transform( const std::string unyts, real scale, real offset ) 
{
    real ms, mo;
    real s, o;
    int i;
    
    uu = unyts;

    s = mksScale/scale;
    o = ( mksOffset - offset)/scale;
    
    // Alter our scale and offset so that they will
    // still take us to MKS units, regardless of
    // what the old units were and what the new units will be.
    
    // apply the scaling
    for ( i=0; i < nzs; i++ ) {
        data[i] = data[i]*s + o;
    }    

    mksOffset = offset;
    mksScale  = scale;

}

bool GridFieldDim::checkdim( const realvec& inx ) const
{
    bool result;
    int i;
    real val;
    int nv;
    
    result = false;
    
    nv = inx.size();
    if ( nv > 0 ) {
       result = true;
       
       if ( nv > 1 ) {
          val = inx[0];
          if ( inx[1] > inx[0] ) {
              for ( i = 1; i < nv; i++ ) {
                  if ( val >= inx[i] ) {
                     result = false;
                     break;
                  }
                  val = inx[i];
              } 
          } else {
              for ( i = 1; i < nv; i++ ) {
                  if ( val <= inx[i] ) {
                     result = false;
                     break;
                  }
                  val = inx[i];
              } 
          
          }
          
       }
    }   
    
    return result;

}

void GridFieldDim::setDir( const int loadFlags )
{
     zdir = 0;
     
     if ( nzs > 1 ) {
        zdir = ( data[1] > data[0] );
     }
     
}


void GridFieldDim::load( const realvec& inx, const int loadFlags  )
{
   int indx = 0;
   int i;
   
   checkdim(inx);
   
   nzs = inx.size();  
   data = inx;
      
   clear_nodata();   
   
   /// determine the direction in which the coordinates go
   setDir( loadFlags );
   
}


bool GridFieldDim::compatible( const GridFieldDim& obj, int flags ) const  
{
    int result = true;
    std::vector<real> dim;
    
    if ( obj.size() == nzs ) {

       for ( int i=0; i < nzs ; i++ ) {
           if ( data[i] != obj.value(i) ) {
              result = false;
           }
       }
       
    } else {
       result = false;
    }         
    
    return result;      

}

bool GridFieldDim::match( const GridFieldDim& obj ) const  
{
    int result = true;
    std::vector<real> dim;
    
    if ( ! this->compatible(obj) ) {
       result = false;
    }
    
    if ( obj.quantity() != quant 
    ||   obj.units() != uu ) { 
       result = false;
    }   

    return result;      
       
}

int GridFieldDim::dataSize() const
{
    if ( nzs <= 0  ) {
       throw (badnodims());
    }
    
    return nzs;   

}

void GridFieldDim::gridpoints( int n, int* is, real* vals, int flags ) const
{
     int cmd;
     int local;
     int done;
     
     local = flags & 0x01;
     done = flags * 0x02;

     // get the data from a central met processors    

     if ( pgroup == NULLPTR || metproc < 0 || local != 0 ) {
         // serial processing.  Access the data locally
         this->values( n, vals, is );
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
         pgroup->send_ints( metproc, n, is, PGR_TAG_GCOORDS );
         // receive the values
         pgroup->receive_reals( metproc, n, vals, PGR_TAG_GVALS );

         if ( done ) {
            svr_done();
         }
     
     }

}

void GridFieldDim::receive_meta()
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
         //- std::cerr << "   GridLatLonField3D::receive_meta: r-100 with " << metproc << std::endl;
         pgroup->receive_string( metproc, &quant, PGR_TAG_GMETA ); // quantity
         //- std::cerr << "   GridFieldDim::receive_meta: r-110 from " << metproc  << std::endl;
         pgroup->receive_string( metproc, &uu, PGR_TAG_GMETA );  // units
         //- std::cerr << "   GridFieldDim::receive_meta: r-170 from " << metproc  << std::endl;
         pgroup->receive_ints( metproc, 1, &nzs, PGR_TAG_GMETA );  // number of lats
         //- std::cerr << "   GridFieldDim::receive_meta: r-200 from " << metproc  << std::endl;

         set_nodata();  // there are no data
   
         // get the coordinates
         try {
            dimvals = new real[nzs];
         } catch(...) {
            throw (badmemreq());
         }
         pgroup->receive_reals( metproc, nzs, dimvals, PGR_TAG_GDIMS ); 
         data.clear();
         for ( i=0; i<nzs; i++ ) {
             data.push_back(dimvals[i]);
         }    
         delete[] dimvals;
         setDir();

         metaID = 0;
   
     }

}

void GridFieldDim::svr_send_meta(int id) const
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
         //- std::cerr << "   GridFieldDim::svr_send_meta: s-100 with " << id << std::endl;
         pgroup->send_string( id, quant, PGR_TAG_GMETA ); // quantity
         //- std::cerr << "   GridFieldDim::svr_send_meta: s-110 to " << id << std::endl;
         pgroup->send_string( id, uu, PGR_TAG_GMETA );  // units
         //- std::cerr << "   GridFieldDim::svr_send_meta: s-160 to " << id << std::endl;
         pgroup->send_ints( id, 1, &nzs, PGR_TAG_GMETA );  // number of lons
         //- std::cerr << "   GridFieldDim::svr_send_meta: s-200 to " << id << std::endl;

         // send the coordinates
         try  {
            dimvals = new real[nzs];
         } catch(...)  {
            throw (badmemreq());
         }
         for ( i=0; i<nzs; i++ ) {
             dimvals[i] = data[i];
         }    
         pgroup->send_reals( id, nzs, dimvals, PGR_TAG_GDIMS ); 
         delete[] dimvals;
   
     }

}


void GridFieldDim::serialize(std::ostream& os) const
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

      GridField::serialize(os);
        
      // output the version
      os.write( reinterpret_cast<char *>( &version), static_cast<std::streamsize>( sizeof(int)));
      
      // output the number of coordinates
      ival = nzs;
      os.write( reinterpret_cast<char *>( &ival), static_cast<std::streamsize>( sizeof(int)));
  
      // output the data
      for ( i=0; i<nzs; i++ ) {
          val = data[i];
          os.write( reinterpret_cast<char *>( &val), static_cast<std::streamsize>( sizeof(real)));
      }
    

  } catch (...) {
      throw;
  }    

}

void GridFieldDim::deserialize(std::istream& is)
{
   string str;
   int len;
   char cc;
   int i;
   double t;
   real val;
   int ival;
   int nxzs;
   std::vector<real> xzs, xdata;
   std::vector<real> dat;
   int version;
   
   clear();

   try {
        
      GridField::deserialize(is);
   
       // read the version
       is.read(reinterpret_cast<char *>( &version), static_cast<std::streamsize>( sizeof(int)));
          
       // read the number of coordinates
       is.read( reinterpret_cast<char *>(&nxzs), static_cast<std::streamsize>( sizeof(int)) );
       
       // read the coordinates
       xdata.reserve(nxzs);
       for ( i=0; i<nxzs; i++ ) {
           is.read( reinterpret_cast<char *>(&val), static_cast<std::streamsize>( sizeof(real) ));
           xdata.push_back(val);
       }    
              
       // call the load method for checking, setting nlats, latdir, etc.
       load( xdata );

   } catch (...) {
       throw;
   }    

}

void GridFieldDim::svr_send_vals( int id ) const
{
     int* coords;
     real* vals;
     int cmd;
     int n;
     
     if ( metproc < 0 ) {
         // serial processing.  Send nothing, but
         // check that the object has valid data
         if ( ! this->hasdata() ) {
            throw (baddataload());
         }
         
     } else {
     
        // get the number of points desired
        pgroup->receive_ints( id, 1, &n, PGR_TAG_GNUM );

        // get the integer coordinates of the points
        try {
           coords = new int[n];
        } catch (...) {
           throw (badmemreq());
        }
        pgroup->receive_ints( id, n, coords, PGR_TAG_GCOORDS );

        // send the data values requested
        vals = new real[n];
        for ( int i=0; i<n; i++ ) {
            vals[i] = this->value( coords[i] );
        }
        // todo: send error instead of numbers
        
        pgroup->send_reals( id, n, vals, PGR_TAG_GVALS );
        
        delete vals;
        delete coords;
     }

}




/****************** Iterators **************************/

GridFieldDim::iterator::iterator()
{
   my_index = 0;
   my_grid = NULLPTR;
}   


GridFieldDim::iterator::iterator( GridFieldDim* grid, int n )
{
   my_index = n;
   my_grid = grid;
}

/// override operator =
GridFieldDim::iterator& GridFieldDim::iterator::operator=(const GridFieldDim::iterator& src)
{
   my_index = src.my_index;
   my_grid  = src.my_grid;
   
   return *this;
}


/// override operator *, returns a reference to the current data element
real GridFieldDim::iterator::operator*() const 
{

   if ( my_index >= 0 && my_index < my_grid->data.size() ) {
      return (my_grid->data)[my_index];
   } else {
      throw (baddataindex());
   }
}

/// override operator == ; tests two iterators for equality
bool GridFieldDim::iterator::operator==(const iterator& x) const
{
   return ( my_index == x.my_index );
}

/// override operator != ; tests two iterators for inequality
bool GridFieldDim::iterator::operator!=(const iterator& x) const
{
   return ( my_index != x.my_index );
}

/// override operator ++ ; increments the iterator to point to the next data element
GridFieldDim::iterator& GridFieldDim::iterator::operator++(int n) 
{
   my_index++;

   return *this;
}

void GridFieldDim::iterator::assign( real val )
{  
  if ( my_index > 0 ) {
     if ( ( my_grid->zdir > 0 ) && ( (my_grid->data)[my_index - 1] >= val ) ) {
        throw (badNonmonotonic());
     } else if ( ( my_grid->zdir < 0 ) && ( (my_grid->data)[my_index - 1] <= val ) ) {
        throw (badNonmonotonic());
     }   
  }
  if ( my_index < ( (my_grid->data).size() - 1 ) ) {
     if ( ( my_grid->zdir > 0 ) && ( (my_grid->data)[my_index + 1] <= val ) ) {
        throw (badNonmonotonic());
     } else if ( ( my_grid->zdir < 0 ) && ( (my_grid->data)[my_index + 1] >= val ) ) {
        throw (badNonmonotonic());
     }   
  }
  (my_grid->data).at(my_index) = val;
}

void GridFieldDim::iterator::indices( int* i ) const 
{
    *i = my_index;
}

GridFieldDim::iterator GridFieldDim::begin()
{
   
   return GridFieldDim::iterator( this );

}

GridFieldDim::iterator GridFieldDim::end()
{
   
   return GridFieldDim::iterator( this, data.size() );

}

 
///////////////////////////////

GridFieldDim::const_iterator::const_iterator()
{
   my_index = 0;
   my_grid = NULLPTR;
}   


GridFieldDim::const_iterator::const_iterator( const GridFieldDim* grid, int n )
{
   my_index = n;
   my_grid = grid;
}

/// override operator =
GridFieldDim::const_iterator& GridFieldDim::const_iterator::operator=(const GridFieldDim::const_iterator& src)
{
   my_index = src.my_index;
   my_grid  = src.my_grid;

   return *this;
}


/// override operator *, returns the current data element
real GridFieldDim::const_iterator::operator*() const 
{

   if ( my_index >= 0 && my_index < my_grid->data.size() ) {
      return (my_grid->data)[my_index];
   } else {
      throw (baddataindex());
   }
}

/// override operator == ; tests two const_iterators for equality
bool GridFieldDim::const_iterator::operator==(const const_iterator& x) const
{
   return ( my_index == x.my_index );
}

/// override operator != ; tests two const_iterators for inequality
bool GridFieldDim::const_iterator::operator!=(const const_iterator& x) const
{
   return ( my_index != x.my_index );
}

/// override operator ++ ; increments the const_iterator to point to the next data element
GridFieldDim::const_iterator& GridFieldDim::const_iterator::operator++(int n) 
{
   my_index++;

   return *this;
}

void GridFieldDim::const_iterator::indices( int* i ) const 
{
    *i = my_index;
}

GridFieldDim::const_iterator GridFieldDim::begin() const
{
   
   return GridFieldDim::const_iterator( this );

}

GridFieldDim::const_iterator GridFieldDim::end() const
{
   
   return GridFieldDim::const_iterator( this, data.size() );

}



namespace gigatraj {

// outputs the Met Field 
std::ostream& operator<<( std::ostream& os, const GridFieldDim& p)
{
  
  try {
  
      p.serialize(os);
  

  } catch (...) {
      throw;
  }    
  
  return os;

};

// inputs the Met field
std::istream& operator>>( std::istream& is, GridFieldDim& p)
{

   try {
   
       p.deserialize(is);

   } catch (...) {
       throw;
   }    
   
   return is;

};

}

 
