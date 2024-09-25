
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

#include "gigatraj/GridFieldProfile.hh"

using namespace gigatraj;



// constructor
GridFieldProfile::GridFieldProfile() : GridField()
{
   
   prof = "none";   
   
}


// destructor
GridFieldProfile::~GridFieldProfile()
{
};

// copy constructor
GridFieldProfile::GridFieldProfile(const GridFieldProfile& src) : GridField(src)
{

   prof = src.profile();

}

void GridFieldProfile::assign( const GridFieldProfile& src )
{
   
   GridField::assign(src);
   
   prof = src.profile();

}

void GridFieldProfile::clear() {

   prof = "none";
   
   GridField::clear();
      
}

int GridFieldProfile::status() const
{
    int result = 0;
    
    result = GridField::status();
    if ( prof == "none" ) {
       result = result | 0x02;
    } 
    
    return result;

}


real GridFieldProfile::operator()( int i ) const 
{
   return value(i);
}        

real& GridFieldProfile::operator()( int i )
{
   return valueref(i);
}        

std::string GridFieldProfile::profile() const
{
   return prof;
}


void GridFieldProfile::set_profile( const std::string nm )
{
   prof = nm;
}

void GridFieldProfile::transform( const std::string unyts, real scale, real offset ) 
{
    real ms, mo;
    GridFieldProfile::iterator data;
    real s, o;
    
    uu = unyts;

    s = mksScale/scale;
    o = ( mksOffset - offset)/scale;
    
    // Alter our scale and offset so that they will
    // still take us to MKS units, regardless of
    // what the old units were and what the new units will be.
    
    // apply the scaling
    for ( data = this->begin(); data != this->end(); data++ ) {
        if (*data != fill_value ) {
           *data = (*data)*s + o;
        }
    } 

    mksOffset = offset;
    mksScale  = scale;

}



bool GridFieldProfile::compatible( const GridFieldProfile& obj, int flags ) const  
{
    int result = true;
    std::vector<real> dim;
    
    if ( flags & METCOMPAT_VERT ) {
       // the surface types must match
       if ( obj.profile() != prof ) {
          result = false;
       }
    }         

    if ( flags & METCOMPAT_TIME ) {
       // the model times and the calendar times must both match
       if ( obj.time() != mtime || obj.met_time() != ctime ) {
          result = false;
       }   
    }
    
    return result;      

}





void GridFieldProfile::svr_send_vals( int id ) const
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


void GridFieldProfile::serialize(std::ostream& os) const
{
  string str;
  int len;
  int version = 1;
  
  try {

      GridField::serialize(os);
        
      // output the version
      os.write( reinterpret_cast<char *>( &version), static_cast<std::streamsize>( sizeof(int)));
      
      // output the surface name
      len = prof.length();
      os.write( reinterpret_cast<char *>( &len), static_cast<std::streamsize>( sizeof(int)));
      os.write( prof.c_str(), static_cast<std::streamsize>( len*sizeof(char)));


  } catch (...) {
      throw;
  }    
  
  //return os;


}


void GridFieldProfile::deserialize(std::istream& is)
{
   string str;
   int len;
   char cc;
   int i;
   double t;
   real val;
   std::vector<real> dat;
   int ival;
   int version;
   

   try {

       GridField::deserialize(is);
        
       // read the version
       is.read(reinterpret_cast<char *>( &version), static_cast<std::streamsize>( sizeof(int)));
          
       // read the quantity
       is.read(reinterpret_cast<char *>( &len), static_cast<std::streamsize>( sizeof(int)));
       str = "";
       if ( len >= 0 ) {
          prof.clear();
          for ( i=0; i < len; i++ ) {
             is.read( &cc, static_cast<std::streamsize>( sizeof(char)));
             str.push_back( cc );
          }   
       }
       prof = str;

   } catch (...) {
       throw;
   }    
   
   //return is;


}


/****************** Iterators **************************/

GridFieldProfile::iterator::iterator()
{
   my_index = 0;
   my_grid = NULLPTR;
}   


GridFieldProfile::iterator::iterator( GridFieldProfile* grid, int n )
{
   my_index = n;
   my_grid = grid;
}

/// override operator =
GridFieldProfile::iterator& GridFieldProfile::iterator::operator=(const GridFieldProfile::iterator& src)
{
   my_index = src.my_index;
   my_grid  = src.my_grid;
   
   return *this;
}


/// override operator *, returns a reference to the current data element
real& GridFieldProfile::iterator::operator*() const 
{

   if ( my_index >= 0 && my_index < my_grid->data.size() ) {
      return (my_grid->data).at(my_index);
   } else {
      throw (baddataindex());
   }
}

/// override operator == ; tests two iterators for equality
bool GridFieldProfile::iterator::operator==(const iterator& x) const
{
   return ( my_index == x.my_index );
}

/// override operator != ; tests two iterators for inequality
bool GridFieldProfile::iterator::operator!=(const iterator& x) const
{
   return ( my_index != x.my_index );
}

/// override operator ++ ; increments the iterator to point to the next data element
GridFieldProfile::iterator& GridFieldProfile::iterator::operator++(int n) 
{
   my_index++;

   return *this;
}

void GridFieldProfile::iterator::assign( real val )
{
  (my_grid->data).at(my_index) = val;
}

GridFieldProfile::iterator GridFieldProfile::begin()
{
   
   return GridFieldProfile::iterator( this );

}

GridFieldProfile::iterator GridFieldProfile::end()
{
   
   return GridFieldProfile::iterator( this, data.size() );

}

 
///////////////////////////////

GridFieldProfile::const_iterator::const_iterator()
{
   my_index = 0;
   my_grid = NULLPTR;
}   


GridFieldProfile::const_iterator::const_iterator( const GridFieldProfile* grid, int n )
{
   my_index = n;
   my_grid = grid;
}

/// override operator =
GridFieldProfile::const_iterator& GridFieldProfile::const_iterator::operator=(const GridFieldProfile::const_iterator& src)
{
   my_index = src.my_index;
   my_grid  = src.my_grid;

   return *this;
}


/// override operator *, returns the current data element
real GridFieldProfile::const_iterator::operator*() const 
{

   if ( my_index >= 0 && my_index < my_grid->data.size() ) {
      return (my_grid->data)[my_index];
   } else {
      throw (baddataindex());
   }
}

/// override operator == ; tests two const_iterators for equality
bool GridFieldProfile::const_iterator::operator==(const const_iterator& x) const
{
   return ( my_index == x.my_index );
}

/// override operator != ; tests two const_iterators for inequality
bool GridFieldProfile::const_iterator::operator!=(const const_iterator& x) const
{
   return ( my_index != x.my_index );
}

/// override operator ++ ; increments the const_iterator to point ot the next data element
GridFieldProfile::const_iterator& GridFieldProfile::const_iterator::operator++(int n) 
{
   my_index++;

   return *this;
}

GridFieldProfile::const_iterator GridFieldProfile::begin() const
{
   
   return GridFieldProfile::const_iterator( this );

}

GridFieldProfile::const_iterator GridFieldProfile::end() const
{
   
   return GridFieldProfile::const_iterator( this, data.size() );

}


 
