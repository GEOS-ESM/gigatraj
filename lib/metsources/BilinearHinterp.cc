
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include <stdlib.h>

#include "gigatraj/BilinearHinterp.hh"

using namespace gigatraj;

const real gigatraj::BilinearHinterp::NEARPOLE = 88.0;

BilinearHinterp::BilinearHinterp()
{
    confml = 0;
};


BilinearHinterp::~BilinearHinterp()
{
};

BilinearHinterp* BilinearHinterp::copy() const
{
   BilinearHinterp *result;
   result = new BilinearHinterp();
   result->conformal( confml );
   
   return result;
}

std::string BilinearHinterp::id() 
{ 
  return "BilinearHinterp";
}

real BilinearHinterp::calc( real lon, real lat, const GridLatLonField3D& grid, const int k, int flags ) const      
{
     // array indices that bound the desired longitude
     int i1, i2;
     // array indices that bound the desired latitude
     int j1, j2;
     // the output interpolated value
     real result;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround the desired lat and lon
     real vals[4];
     // longitude indices of the four grid points 
     int is[4];
     // latitude indices of the four grid points 
     int js[4];
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int ks[4];
     
     
     // take care of any out-of-range longitude values
     lon = grid.wrap(lon);
     
     /*! \todo: 
         take bad-data flags into account, according to flags setting
     */    
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // get the i and j array coordinates that
     // correspond to this longitude and latitude
     grid.lonindex(lon, &i1, &i2);
     grid.latindex(lat, &j1, &j2);
     
     // set the four corders of the grid box that encompasses
     // our desired lat and lon location
     is[0] = i1;
     js[0] = j1;
     ks[0] = k;
     is[1] = i1;
     js[1] = j2;
     ks[1] = k;
     is[2] = i2;
     js[2] = j1;
     ks[2] = k;
     is[3] = i2;
     js[3] = j2;
     ks[3] = k;
     
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4, is, js, ks, vals, do_local(flags) );

     // Ok, now we have our four corner values.
     // All four of these need to be good
     if ( vals[0] != bad && vals[1] != bad && vals[2] != bad && vals[3] != bad ) {

        // do the weighted average (see the header file for the inline
        //   function minicalc() )
        result = minicalc( lon, lat
                         , grid.longitude(i1), grid.latitude(j1)
                         , grid.longitude(i2), grid.latitude(j2)
                         , vals[0], vals[1], vals[2], vals[3] );     

     } else {
        result = bad;
     }
     
     return result;         
     
};


real BilinearHinterp::calc( real lon, real lat, const GridLatLonFieldSfc& grid, int flags ) const     
{
     // array indices that bound the desired longitude
     int i1, i2;
     // array indices that bound the desired latitude
     int j1, j2;
     // the output interpolated value
     real result;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround the desired lat and lon
     real vals[4];
     // longitude indices of the four grid points 
     int is[4];
     // latitude indices of the four grid points 
     int js[4];
     
     // take care of any out-of-range longitude values
     lon = grid.wrap(lon);
     

     // the bad-or-missing-data fill value
     bad = grid.fillval();
     

     // get the i and j array coordinates that
     // correspond to this longitude and latitude
     grid.lonindex(lon, &i1, &i2);
     grid.latindex(lat, &j1, &j2);
     
     // set the four corders of the grid box that encompasses
     // our desired lat and lon location
     is[0] = i1;
     js[0] = j1;
     is[1] = i1;
     js[1] = j2;
     is[2] = i2;
     js[2] = j1;
     is[3] = i2;
     js[3] = j2;
     
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4, is, js, vals, do_local(flags) );
     
     // Ok, now we have our four corner values.
     // All four of these need to be good
     if ( vals[0] != bad && vals[1] != bad && vals[2] != bad && vals[3] != bad ) {

        // do the weighted average (see the header file for the inline
        //   function minicalc() )
        result = minicalc( lon, lat
                         , grid.longitude(i1), grid.latitude(j1)
                         , grid.longitude(i2), grid.latitude(j2)
                         , vals[0], vals[1], vals[2], vals[3] );     

     } else {
        result = bad;
     }
     
     return result;         
     

};


std::vector<real>*  BilinearHinterp::calc( const std::vector<real>& lons, const std::vector<real>& lats, const GridLatLonField3D& grid, const int k, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // vector of output interpolated values
     std::vector<real> *results;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // loop index for the locations we are interpolating to
     int i;
     // temporary longitude value
     real lon;
     // number of input locations
     int n;
     
     // how many shall we do?
     n = lons.size();
     
     // create the output variable
     results = new std::vector<real>;
     results->reserve(n);
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create an array to hold the grid point values
     vals = new real[4*n];
     // create arrays to hold grid point index values
     is = new int[4*n];
     js = new int[4*n];
     ks = new int[4*n];
     
     // for each point we are interpolating to...
     for ( i=0; i<n; i++ ) {
     
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);

         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         grid.lonindex(lon, &i1, &i2);
         grid.latindex(lats[i], &j1, &j2);
     
         // set the four corders of the grid box that encompasses
         // our desired lat and lon location
         is[i*4+0] = i1;
         js[i*4+0] = j1;
         ks[i*4+0] = k;
         is[i*4+1] = i1;
         js[i*4+1] = j2;
         ks[i*4+1] = k;
         is[i*4+2] = i2;
         js[i*4+2] = j1;
         ks[i*4+2] = k;
         is[i*4+3] = i2;
         js[i*4+3] = j2;
         ks[i*4+3] = k;
     }
          
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n, is, js, ks, vals, do_local(flags) );

     // for each location...
     for ( i=0; i<n; i++ ) {
         // wrap the longitude again
         lon = grid.wrap(lons[i]);
     
         // all four values surrounding this location must be good
         if ( vals[i*4+0] != bad && vals[i*4+1] != bad 
           && vals[i*4+2] != bad && vals[i*4+3] != bad ) {

             // do the weighted average (see the header file for the inline
             //   function minicalc() )
              results->push_back( this->minicalc( lon, lats[i]
                          , grid.longitude(is[i*4+0]), grid.latitude(js[i*4+0])
                          , grid.longitude(is[i*4+3]), grid.latitude(js[i*4+3])
                          , vals[i*4+0], vals[i*4+1], vals[i*4+2], vals[i*4+3] ) );

         } else {
              results->push_back( bad );
         }
        
     }   
     
     
     // drop all the temporary variables
     // before we leave
     delete ks;
     delete js;
     delete is;
     delete vals;
     
     return results;         
     

};

std::vector<real>*  BilinearHinterp::calc( const std::vector<real>& lons, const std::vector<real>& lats, const GridLatLonFieldSfc& grid, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // vector of the output interpolated values
     std::vector<real> *results;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // loop index for the locations we are interpolating to
     int i;
     // temporary longitude value
     real lon;
     // the number of input locations
     int n;
     
     
     
     n = lons.size();

     // create the vector to hold the output results
     results = new std::vector<real>;
     results->reserve(n);
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create an array to hold the grid point values
     vals = new real[4*n];
     // create arrays to hold grid point index values
     is = new int[4*n];
     js = new int[4*n];
     
     // for each point we are interpolating to...
     for ( i=0; i<n; i++ ) {
     
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);

         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         grid.lonindex(lon, &i1, &i2);
         grid.latindex(lats[i], &j1, &j2);
     
         // set the four corders of the grid box that encompasses
         // our desired lat and lon location
         is[i*4+0] = i1;
         js[i*4+0] = j1;
         is[i*4+1] = i1;
         js[i*4+1] = j2;
         is[i*4+2] = i2;
         js[i*4+2] = j1;
         is[i*4+3] = i2;
         js[i*4+3] = j2;
     }
          
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n, is, js, vals, do_local(flags) );

     // for each location...
     for ( i=0; i<n; i++ ) {
         // wrap the longitude again
         lon = grid.wrap(lons[i]);
     
         // all four values surrounding this location must be good
         if ( vals[i*4+0] != bad && vals[i*4+1] != bad 
           && vals[i*4+2] != bad && vals[i*4+3] != bad ) {

             // do the weighted average (see the header file for the inline
             //   function minicalc() )
              results->push_back( this->minicalc( lon, lats[i]
                          , grid.longitude(is[i*4+0]), grid.latitude(js[i*4+0])
                          , grid.longitude(is[i*4+3]), grid.latitude(js[i*4+3])
                          , vals[i*4+0], vals[i*4+1], vals[i*4+2], vals[i*4+3] ) );

         } else {
              results->push_back( bad );
         }
        
     }   
     
     
     // drop all the temporary variables
     // before we leave
     delete js;
     delete is;
     delete vals;
     
     return results;         
     


};

std::vector<real>*  BilinearHinterp::calc( const std::vector<real>& lons, const std::vector<real>& lats, const GridCubedSphereFieldSfc& grid, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // vector of the output interpolated values
     std::vector<real> *results;
     // the bad-or-missing-data fill value
     real bad, val;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // loop index for the locations we are interpolating to
     int i;
     // temporary longitude value
     real lon;
     // the number of input locations
     int n, pos0, pos1, pos2,pos3, idx, nlons, nlats;
     
     n = lons.size();
     grid.dims( &nlons, &nlats);
     // create the vector to hold the output results
     results = new std::vector<real>;
     results->reserve(n);
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create an array to hold the grid point values
     vals = new real[4*n];
     // create arrays to hold grid point index values
     is = new int[4*n];
     js = new int[4*n];
     
     // for each point we are interpolating to...
     for ( i=0; i<n; i++ ) {
     
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);

         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         //grid.lonindex(lon, &i1, &i2);
         //grid.latindex(lats[i], &j1, &j2);
         grid.latlonindex(lats[i], lon, i1, j1);
         i1 = (i1+1)/2;
         j1 = (j1+1)/2;
         // convert to relative to halo
         i1 = i1 - grid.iStart;
         j1 = j1 - grid.jStart;

         i2 = i1+1;
         j2 = j1+1;
         // set the four corders of the grid box that encompasses
         // our desired lat and lon location
         is[i*4+0] = i1;
         js[i*4+0] = j1;
         is[i*4+1] = i1;
         js[i*4+1] = j2;
         is[i*4+2] = i2;
         js[i*4+2] = j1;
         is[i*4+3] = i2;
         js[i*4+3] = j2;
     }
          
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n, is, js, vals, do_local(flags) );

     // for each location...
     for ( i=0; i<n; i++ ) {
         // wrap the longitude again
         lon = grid.wrap(lons[i]);
         idx = 4*i;     
         // all four values surrounding this location must be good
         if ( vals[i*4+0] != bad && vals[i*4+1] != bad 
           && vals[i*4+2] != bad && vals[i*4+3] != bad ) {

             pos0 = js[idx+0]*nlons + is[idx+0];
             pos1 = js[idx+1]*nlons + is[idx+1];
             pos2 = js[idx+2]*nlons + is[idx+2];
             pos3 = js[idx+3]*nlons + is[idx+3];

             val = minicalc( lon, lats[i]
                          , grid.longitude(pos0), grid.latitude(pos0)
                          , grid.longitude(pos1), grid.latitude(pos1)
                          , grid.longitude(pos2), grid.latitude(pos2)
                          , grid.longitude(pos3), grid.latitude(pos3)
                          , vals[idx+0], vals[idx+1], vals[idx+2], vals[idx+3] );
             results->push_back(val);
             // do the weighted average (see the header file for the inline
             //   function minicalc() )
             // results->push_back( this->minicalc( lon, lats[i]
             //             , grid.longitude(is[i*4+0]), grid.latitude(js[i*4+0])
             //             , grid.longitude(is[i*4+3]), grid.latitude(js[i*4+3])
             //             , vals[i*4+0], vals[i*4+1], vals[i*4+2], vals[i*4+3] ) );



         } else {
              results->push_back( bad );
         }
        
     }   
     
     
     // drop all the temporary variables
     // before we leave
     delete js;
     delete is;
     delete vals;
     
     return results;         
};

real* BilinearHinterp::calc( const int n, const real *lons, const real *lats, const GridLatLonField3D& grid, const int k, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // pointer to the output interpolated values
     real *results;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // loop index for the locations we are interpolating to
     int i;
     // temporary longitude value
     real lon;
     
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create the array to hold the output results
     results = new real[n];
     // create an array to hold the grid point values
     vals = new real[4*n];
     // create arrays to hold grid point index values
     is = new int[4*n];
     js = new int[4*n];
     ks = new int[4*n];
     
     
     // for each point we are interpolating to...
     for ( i=0; i<n; i++ ) {
     
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);
         
         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         grid.lonindex(lon, &i1, &i2);
         grid.latindex(lats[i], &j1, &j2);
     
         // set the four corders of the grid box that encompasses
         // our desired lat and lon location
         is[i*4+0] = i1;
         js[i*4+0] = j1;
         ks[i*4+0] = k;
         is[i*4+1] = i1;
         js[i*4+1] = j2;
         ks[i*4+1] = k;
         is[i*4+2] = i2;
         js[i*4+2] = j1;
         ks[i*4+2] = k;
         is[i*4+3] = i2;
         js[i*4+3] = j2;
         ks[i*4+3] = k;
     }
          
     // Get the values at those 4xn grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n, is, js, ks, vals, do_local(flags) );

     // for each location...
     for ( i=0; i<n; i++ ) {
         // wrap the longitude again
         lon = grid.wrap(lons[i]);
     
         // all four values surrounding this location must be good
         if ( vals[i*4+0] != bad && vals[i*4+1] != bad 
           && vals[i*4+2] != bad && vals[i*4+3] != bad ) {


             // do the weighted average (see the header file for the inline
             //   function minicalc() )
             results[i] = this->minicalc( lon, lats[i]
                         , grid.longitude(is[i*4+0]), grid.latitude(js[i*4+0])
                         , grid.longitude(is[i*4+3]), grid.latitude(js[i*4+3])
                         , vals[i*4+0], vals[i*4+1], vals[i*4+2], vals[i*4+3] );

        } else {
             results[i] = bad;
        }
        
     }   
     
     // drop all the temporary variables
     // before we leave
     delete ks;
     delete js;
     delete is;
     delete vals;
     
     return results;         
     

};

real* BilinearHinterp::calc( const int n, const real *lons, const real *lats, const GridLatLonFieldSfc& grid, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // pointer to the output interpolated values
     real *results;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // loop index for the locations we are interpolating to
     int i;
     // temporary longitude value
     real lon;
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create the array to hold the output results
     results = new real[n];
     // create an array to hold the grid point values
     vals = new real[4*n];
     // create arrays to hold grid point index values
     is = new int[4*n];
     js = new int[4*n];

     // for each point we are interpolating to...
     for ( i=0; i<n; i++ ) {
     
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);

         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         grid.lonindex(lon, &i1, &i2);
         grid.latindex(lats[i], &j1, &j2);
     
         // set the four corders of the grid box that encompasses
         // our desired lat and lon location
         is[i*4+0] = i1;
         js[i*4+0] = j1;
         is[i*4+1] = i1;
         js[i*4+1] = j2;
         is[i*4+2] = i2;
         js[i*4+2] = j1;
         is[i*4+3] = i2;
         js[i*4+3] = j2;
     }
          
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization which is handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n, is, js, vals, do_local(flags) );

     // for each location...
     for ( i=0; i<n; i++ ) {
         // wrap the longitude again
         lon = grid.wrap(lons[i]);
     
         // all four values surrounding this location must be good
         if ( vals[i*4+0] != bad && vals[i*4+1] != bad 
           && vals[i*4+2] != bad && vals[i*4+3] != bad ) {

             // do the weighted average (see the header file for the inline
             //   function minicalc() )
              results[i] = this->minicalc( lon, lats[i]
                          , grid.longitude(is[i*4+0]), grid.latitude(js[i*4+0])
                          , grid.longitude(is[i*4+3]), grid.latitude(js[i*4+3])
                          , vals[i*4+0], vals[i*4+1], vals[i*4+2], vals[i*4+3] );

         } else {
              results[i] = bad;
         }
        
     }   
     
     
     // drop all the temporary variables
     // before we leave
     delete js;
     delete is;
     delete vals;
     
     return results;         
     

};


void BilinearHinterp::calc( const int n, const real *lons, const real *lats, real* results, const GridLatLonFieldSfc& grid, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // loop index for the locations we are interpolating to
     int i;
     // temporary longitude value
     real lon;
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create an array to hold the grid point values
     vals = new real[ 4*n ];
     // create arrays to hold grid point index values
     is = new int[ 4*n ];
     js = new int[ 4*n ];
     
     // for each point we are interpolating to...
     for ( i=0; i<n; i++ ) {
     
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);

         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         grid.lonindex(lon, &i1, &i2);
         grid.latindex(lats[i], &j1, &j2);
     
         // set the four corders of the grid box that encompasses
         // our desired lat and lon location
         is[i*4+0] = i1;
         js[i*4+0] = j1;
         is[i*4+1] = i1;
         js[i*4+1] = j2;
         is[i*4+2] = i2;
         js[i*4+2] = j1;
         is[i*4+3] = i2;
         js[i*4+3] = j2;
     }
          
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization which is handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n, is, js, vals, do_local(flags) );

     // for each location...
     for ( i=0; i<n; i++ ) {
         // wrap the longitude again
         lon = grid.wrap(lons[i]);
     
         // all four values surrounding this location must be good
         if ( vals[i*4+0] != bad && vals[i*4+1] != bad 
           && vals[i*4+2] != bad && vals[i*4+3] != bad ) {

             // do the weighted average (see the header file for the inline
             //   function minicalc() )
              results[i] = this->minicalc( lon, lats[i]
                          , grid.longitude(is[i*4+0]), grid.latitude(js[i*4+0])
                          , grid.longitude(is[i*4+3]), grid.latitude(js[i*4+3])
                          , vals[i*4+0], vals[i*4+1], vals[i*4+2], vals[i*4+3] );

         } else {
              results[i] = bad;
         }
        
     }   
     
     
     // drop all the temporary variables
     // before we leave
     delete js;
     delete is;
     delete vals;
     

};

void BilinearHinterp::calc( const int n, const real* lons, const real* lats, real* results, const GridFieldSfc& grid, int flags ) const
{
    // downcast and interpolate
    calc( n, lons, lats, results, dynamic_cast<const GridLatLonFieldSfc&>(grid), flags);

}

void BilinearHinterp::calc( real lon, real lat, real &xval, real &yval, const GridLatLonField3D& xgrid, const GridLatLonField3D& ygrid, const int k, int flags ) const
{
     // array indices that bound the desired longitude
     int i1, i2;
     // array indices that bound the desired latitude
     int j1, j2;
     // the output interpolated value
     real result;
     // the bad-or-missing-data fill value for x and y
     real xbad, ybad;
     // data values at the four grid points that surround the desired lat and lon, for x and y components
     real xvals[4], yvals[4];
     // longitude indices of the four grid points 
     int is[4];
     // latitude indices of the four grid points 
     int js[4];
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int ks[4];
     // tmp values for conformal adjustment
     real xtmp, ytmp;
     
     if ( ! xgrid.compatible( ygrid ) )  {
        throw (badincompatible());
     }
     
     // take care of any out-of-range longitude values
     lon = xgrid.wrap(lon);
     
     /*! \todo: 
         take bad-data flags into account, according to flags setting
     */    
     // the bad-or-missing-data fill value
     xbad = xgrid.fillval();
     ybad = ygrid.fillval();
     
     // get the i and j array coordinates that
     // correspond to this longitude and latitude
     xgrid.lonindex(lon, &i1, &i2);
     xgrid.latindex(lat, &j1, &j2);
     
     // set the four corders of the grid box that encompasses
     // our desired lat and lon location
     is[0] = i1;
     js[0] = j1;
     ks[0] = k;
     is[1] = i1;
     js[1] = j2;
     ks[1] = k;
     is[2] = i2;
     js[2] = j1;
     ks[2] = k;
     is[3] = i2;
     js[3] = j2;
     ks[3] = k;
     
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     // The 0x02 flag ensures that each gridpoints call results in a svr_done() call.
     xgrid.ask_for_data();
     xgrid.gridpoints( 4, is, js, ks, xvals, do_local(flags) | 0x02 );
     ygrid.ask_for_data();
     ygrid.gridpoints( 4, is, js, ks, yvals, do_local(flags) | 0x02 );

     // Ok, now we have our four corner values.
     // All four of these need to be good
     if ( xvals[0] != xbad && xvals[1] != xbad && xvals[2] != xbad && xvals[3] != xbad 
       && yvals[0] != ybad && yvals[1] != ybad && yvals[2] != ybad && yvals[3] != ybad ) {

        // do the weighted average (see the header file for the inline
        //   function minicalc() )
        xval = minicalc( lon, lat
                         , xgrid.longitude(i1), xgrid.latitude(j1)
                         , xgrid.longitude(i2), xgrid.latitude(j2)
                         , xvals[0], xvals[1], xvals[2], xvals[3] );     
        yval = minicalc( lon, lat
                         , ygrid.longitude(i1), ygrid.latitude(j1)
                         , ygrid.longitude(i2), ygrid.latitude(j2)
                         , yvals[0], yvals[1], yvals[2], yvals[3] );     

     } else {
        xval = xbad;
        yval = ybad;
     }
     
};

void BilinearHinterp::calc( real lon, real lat, real &xval, real &yval, const GridField3D& xgrid, const GridField3D& ygrid, const int k, int flags ) const
{
    // downcast and interpolate
    return calc( lon, lat, xval, yval, dynamic_cast<const GridLatLonField3D&>(xgrid), dynamic_cast<const GridLatLonField3D&>(ygrid), k, flags);

}

void BilinearHinterp::calc( real lon, real lat, real &xval, real &yval, const GridLatLonFieldSfc& xgrid, const GridLatLonFieldSfc& ygrid, int flags ) const
{
     // array indices that bound the desired longitude
     int i1, i2;
     // array indices that bound the desired latitude
     int j1, j2;
     // the output interpolated value
     real result;
     // the bad-or-missing-data fill value for x and y
     real xbad, ybad;
     // data values at the four grid points that surround the desired lat and lon, for x and y components
     real xvals[4], yvals[4];
     // longitude indices of the four grid points 
     int is[4];
     // latitude indices of the four grid points 
     int js[4];
     // tmp values for conformal adjustment
     real xtmp, ytmp;
     
     if ( ! xgrid.compatible( ygrid ) )  {
        throw (badincompatible());
     }
     
     // take care of any out-of-range longitude values
     lon = xgrid.wrap(lon);
     

     // the bad-or-missing-data fill value
     xbad = xgrid.fillval();
     ybad = ygrid.fillval();
     

     // get the i and j array coordinates that
     // correspond to this longitude and latitude
     xgrid.lonindex(lon, &i1, &i2);
     xgrid.latindex(lat, &j1, &j2);
     
     // set the four corders of the grid box that encompasses
     // our desired lat and lon location
     is[0] = i1;
     js[0] = j1;
     is[1] = i1;
     js[1] = j2;
     is[2] = i2;
     js[2] = j1;
     is[3] = i2;
     js[3] = j2;
     
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     // The 0x02 flag ensures that each gridpoints call results in a svr_done() call.
     xgrid.ask_for_data();
     xgrid.gridpoints( 4, is, js, xvals, do_local(flags) | 0x02 );
     ygrid.ask_for_data();
     ygrid.gridpoints( 4, is, js, yvals, do_local(flags) | 0x02 );
     
     // Ok, now we have our four corner values.
     // All four of these need to be good
     if ( xvals[0] != xbad && xvals[1] != xbad && xvals[2] != xbad && xvals[3] != xbad 
       && yvals[0] != ybad && yvals[1] != ybad && yvals[2] != ybad && yvals[3] != ybad ) {

        // do the weighted average (see the header file for the inline
        //   function minicalc() )
        xval = minicalc( lon, lat
                         , xgrid.longitude(i1), xgrid.latitude(j1)
                         , xgrid.longitude(i2), xgrid.latitude(j2)
                         , xvals[0], xvals[1], xvals[2], xvals[3] );     
        yval = minicalc( lon, lat
                         , ygrid.longitude(i1), ygrid.latitude(j1)
                         , ygrid.longitude(i2), ygrid.latitude(j2)
                         , yvals[0], yvals[1], yvals[2], yvals[3] );     

     } else {
        xval = xbad;
        yval = ybad;
     }
     
};

void BilinearHinterp::calc( real lon, real lat, real &xval, real &yval, const GridFieldSfc& xgrid, const GridFieldSfc& ygrid, int flags ) const
{
    // downcast and interpolate
    return calc( lon, lat, xval, yval, dynamic_cast<const GridLatLonFieldSfc&>(xgrid), dynamic_cast<const GridLatLonFieldSfc&>(ygrid), flags);
}


void BilinearHinterp::calc( int n, real* lons, real* lats, real* xvals, real* yvals, const GridLatLonFieldSfc& xgrid, const GridLatLonFieldSfc& ygrid, int flags ) const
{
     // array indices that bound the desired longitude
     int i1, i2;
     // array indices that bound the desired latitude
     int j1, j2;
     // the output interpolated value
     real result;
     // the bad-or-missing-data fill value for x and y
     real xbad, ybad;
     // data values at the four grid points that surround the desired lat and lon, for x and y components
     real xvalstmp[4], yvalstmp[4];
     // longitude indices of the four grid points 
     int* is;
     // latitude indices of the four grid points 
     int* js;
     // tmp values for conformal adjustment
     real xtmp, ytmp;
     real lat, lon;
     real xval, yval;
     
     if ( ! xgrid.compatible( ygrid ) )  {
        throw (badincompatible());
     }
     
     // the bad-or-missing-data fill value
     xbad = xgrid.fillval();
     ybad = ygrid.fillval();
     
     
     is = new int[4*n];
     js = new int[4*n];
     real* xvtmp = new real[4*n];
     real* yvtmp = new real[4*n];
     
     for ( int i=0; i<n; i++ ) {
     
         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         xgrid.lonindex(xgrid.wrap(lons[i]), &i1, &i2);
         xgrid.latindex(lats[i], &j1, &j2);
     
         // set the four corders of the grid box that encompasses
         // our desired lat and lon location
         is[4*i + 0] = i1;
         js[4*i + 0] = j1;
         is[4*i + 1] = i1;
         js[4*i + 1] = j2;
         is[4*i + 2] = i2;
         js[4*i + 2] = j1;
         is[4*i + 3] = i2;
         js[4*i + 3] = j2;
     } 
    
     // Get the values at those n*4 grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     // The 0x02 flag ensures that each gridpoints call results in a svr_done() call.
     xgrid.ask_for_data();
     xgrid.gridpoints( 4*n, is, js, xvtmp, do_local(flags) | 0x02 );
     ygrid.ask_for_data();
     ygrid.gridpoints( 4*n, is, js, yvtmp, do_local(flags) | 0x02 );
     
     for ( int i=0; i<n; i++ ) {
     
         int ioff = 4*i;
         lat = lats[i];
         lon = xgrid.wrap(lons[i]);
             
         // Ok, now we have our four corner values.
         // All four of these need to be good
         if ( xvtmp[ioff + 0] != xbad && xvtmp[ioff + 1] != xbad && xvtmp[ioff + 2] != xbad && xvtmp[ioff + 3] != xbad 
           && yvtmp[ioff + 0] != ybad && yvtmp[ioff + 1] != ybad && yvtmp[ioff + 2] != ybad && yvtmp[ioff + 3] != ybad ) {
     
            i1 = is[ioff + 0];
            j1 = js[ioff + 0];
            i2 = is[ioff + 3];
            j2 = js[ioff + 3];
            
     
            // do the weighted average (see the header file for the inline
            //   function minicalc() )
            xvals[i] = minicalc( lon, lat
                             , xgrid.longitude(i1), xgrid.latitude(j1)
                             , xgrid.longitude(i2), xgrid.latitude(j2)
                             , xvtmp[ioff + 0], xvtmp[ioff + 1], xvtmp[ioff + 2], xvtmp[ioff + 3] );     
            yvals[i] = minicalc( lon, lat
                             , ygrid.longitude(i1), ygrid.latitude(j1)
                             , ygrid.longitude(i2), ygrid.latitude(j2)
                             , yvtmp[ioff + 0], yvtmp[ioff + 1], yvtmp[ioff + 2], yvtmp[ioff + 3] );     
     
         } else {
            xvals[i] = xbad;
            yvals[i] = ybad;
         }
         
     }     

     delete yvtmp;
     delete xvtmp;
     delete js;
     delete is;

};

void BilinearHinterp::calc( int n,  real* lons, real* lats, real* xvals, real* yvals, const GridFieldSfc& xgrid, const GridFieldSfc& ygrid, int flags ) const
{
    // downcast and interpolate
    return calc( n, lons, lats, xvals, yvals, dynamic_cast<const GridLatLonFieldSfc&>(xgrid), dynamic_cast<const GridLatLonFieldSfc&>(ygrid), flags);
}

GridLatLonFieldSfc* BilinearHinterp::regrid( const GridLatLonFieldSfc& input, const std::vector<real>& lons, const std::vector<real>& lats, int flags ) const
{
   // the output grid object
   GridLatLonFieldSfc *newgrid;
   // loop indices for longitude and latitude, respectively
   int i,j;
   // temporary variables for holding 2D longitude, latitude, 
   // and data values as a single long vector each
   std::vector<real> *xlons, *xlats, *xdata;
   // dimensions of the new grid
   int nlons, nlats;
   
   // create the new output grid object 
   newgrid = new GridLatLonFieldSfc();
   // copy the old metadata into the new grid
   *newgrid = input;

   // The dimensions of the new grid
   nlons = lons.size();
   nlats = lats.size();
   
   // make vectors to hold the new grid's lons and lats
   xlons = new std::vector<real>;
   xlats = new std::vector<real>;
   xlons->reserve(nlons*nlats);
   xlats->reserve(nlons*nlats);
   // populate the new grid vectors with the new lat and lon values
   // (Note that these vectors hold 2D data)
   for (j=0; j<nlats; j++ ) {
   for (i=0; i<nlons; i++ ) {
       xlons->push_back( lons[i] );
       xlats->push_back( lats[j] );
   }
   }
   // interpolate the data to the new gridpoint locations
   xdata = calc( *xlons, *xlats, input, flags );

   // load the new grid and data into the output grid object
   newgrid->load( lons, lats, *xdata );
   
   // drop the temporary variables
   delete xdata;
   delete xlons;
   delete xlats;
   
   return newgrid;

}


GridLatLonField3D* BilinearHinterp::regrid( const GridLatLonField3D& input, const std::vector<real>& lons, const std::vector<real>& lats, int flags ) const
{
   // the output grid object
   GridLatLonField3D *newgrid;
   // loop indices for longitude, latitude, and vertical coordinate, respectively
   int i,j,k;
   // temporary variables for holding 2D longitude, latitude, 
   // and data values as a single long vector each
   std::vector<real> *xlons, *xlats, *xdata;
   // vector of vertical coordinate values
   std::vector<real> zs;
   // vector to hold a verticla profile of data
   std::vector<real> *tmpdata;
   // dimensions of the new grid
   int nlons, nlats, nzs; 
   
   
   // create the new output grid object
   newgrid = new GridLatLonField3D();
   // copy the old metadata into the new grid
   *newgrid = input;

   // get the old grid's vertical coordinates,
   // which will remain unchanged for the new
   zs = input.levels();

   // The dimensions of the new grid
   nlons = lons.size();
   nlats = lats.size();
   nzs = zs.size();
     
   
   // make vectors to hold the new grid's lons and lats
   xlons = new std::vector<real>;
   xlats = new std::vector<real>;
   xlons->reserve(nlons*nlats*nzs);
   xlats->reserve(nlons*nlats*nzs);
   // populate the new grid vectors with the new lat and lon values
   // (Note that these vectors hold 2D data)
   for (j=0; j<nlats; j++ ) {
   for (i=0; i<nlons; i++ ) {
       xlons->push_back( lons[i] );
       xlats->push_back( lats[j] );
   }
   }
   // make a vector to hold the new grid's data values
   xdata = new std::vector<real>;
   xdata->reserve(nlons*nlats*nzs);
   // for each vertical level...
   for (k=0; k<nzs; k++ ) {
      // interpolate the old data to the new grid at this vertical level
      tmpdata = calc( *xlons, *xlats, input, k, flags );
      // store the inteproated points into the new grid values vector
      for (j=0; j<nlats; j++ ) {
      for (i=0; i<nlons; i++ ) {
          xdata->push_back( (*tmpdata)[ j*nlons+i ] );
      }
      }
      // no need for this any more
      delete tmpdata;
   }
   
   // load the new grid and data into the output grid object
   newgrid->load( lons, lats, zs, *xdata );
   
   // drop the temporary variables
   delete xdata;
   delete xlons;
   delete xlats;
   
   return newgrid;


}

GridLatLonField3D* BilinearHinterp::regrid( const GridLatLonField3D& input, const std::vector<real>& lons, const std::vector<real>& lats, const std::vector<real>& zs, const Vinterp& vin, int flags ) const
{
   // the output grid object
   GridLatLonField3D *newgrid;
   // loop indices for longitude, latitude, and vertical coordinate, respectively
   int i,j,k;
   // temporary variables for holding 2D longitude, latitude, level, 
   // and data values as a single long vector each
   std::vector<real> *xlons, *xlats, *xzs, *xdata;
   // vector to hold a verticla profile of data
   std::vector<real> *tmpdata;
   // dimensions of the new grid
   int nlons, nlats, nzs; 
   
   
   // copy the old metadata into the new grid
   newgrid = new GridLatLonField3D;
   *newgrid = input;

   // The dimensions of the new grid
   nlons = lons.size();
   nlats = lats.size();
   nzs = zs.size();
     
   
   // make vectors to hold the new grid's lons and lats
   xlons = new std::vector<real>;
   xlats = new std::vector<real>;
   xlons->reserve(nlons*nlats*nzs);
   xlats->reserve(nlons*nlats*nzs);
   // populate the new grid vectors with the new lat and lon values
   // (Note that these vectors hold 2D data)
   for (j=0; j<nlats; j++ ) {
   for (i=0; i<nlons; i++ ) {
       xlons->push_back( lons[i] );
       xlats->push_back( lats[j] );
   }
   }
   // make a vector to hold the new grid's data values
   xdata = new std::vector<real>;
   xdata->reserve(nlons*nlats*nzs);
   // for each vertical level...
   for (k=0; k<nzs; k++ ) {
      // fill in a 2D "array" with the desired z level
      xzs->clear();
      xzs->reserve(nlons*nlats*nzs);
      for (j=0; j<nlats; j++ ) {
      for (i=0; i<nlons; i++ ) {
          xzs->push_back( zs[i] );
      }
      }
      // interpolate the old data to the new grid at this vertical level
      tmpdata = vinterp( *xlons, *xlats, *xzs, input, vin, flags );
      
      // store the inteproated points into the new grid values vector
      for (j=0; j<nlats; j++ ) {
      for (i=0; i<nlons; i++ ) {
          xdata->push_back( (*tmpdata)[ j*nlons+i ] );
      }
      }
      // no need for this any more
      delete tmpdata;
   }
   
   // load the new grid and data into the output grid object
   newgrid->load( lons, lats, zs, *xdata );
   
   // drop the temporary variables
   delete xdata;
   delete xlons;
   delete xlats;
   delete xzs;
   
   return newgrid;




}


real BilinearHinterp::vinterp( real lon, real lat, real z, const GridLatLonField3D& grid, const Vinterp& vin, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // the output interpolated value
     real result;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // direct indices into the data array 
     int *indices;
     // loop index for the locations we are interpolating to
     int i;
     // loop index for the grid vertical level
     int k;
     // dimensions of the input data grid
     int nlons, nlats, nzs;
     // a vector to hold horizontally-interpolated data,
     // which will subsequently be vertically interpolated
     std::vector<real> profile;
     // index offset used in assembling indices from different grid levels
     int idx;
     // temporary variable to hold an interpolated result
     real val;
     // the vertical levels from the input data grid
     std::vector<real> zlevs;

     // get the dimensions of the input data grid
     grid.dims( &nlons, &nlats, &nzs );
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create an array to hold the grid point values
     vals = new real[4*nzs];
     // create arrays to hold grid point index values
     is = new int[4*nzs];
     js = new int[4*nzs];
     ks = new int[4*nzs];
     indices = new int[4*nzs];

     // take care of any out-of-range longitude values
     lon = grid.wrap(lon);
     
     
     // get the i and j array coordinates that
     // correspond to this longitude and latitude
     grid.lonindex(lon, &i1, &i2);
     grid.latindex(lat, &j1, &j2);
     
     // for each vertical level...
     for ( k=0; k<nzs; k++ ) {
     
        // make the base index into the grid-index arrays
        idx = k*4;

        // set the four corders of the grid box that encompasses
        // our desired lat and lon location
        is[idx+0] = i1;
        js[idx+0] = j1;
        ks[idx+0] =  k;
        is[idx+1] = i1;
        js[idx+1] = j2;
        ks[idx+1] =  k;
        is[idx+2] = i2;
        js[idx+2] = j1;
        ks[idx+2] =  k;
        is[idx+3] = i2;
        js[idx+3] = j2;
        ks[idx+3] =  k;

        indices[idx + 0] = grid.joinIndex( i1, j1, k );
        indices[idx + 1] = grid.joinIndex( i1, j2, k );
        indices[idx + 2] = grid.joinIndex( i2, j1, k );
        indices[idx + 3] = grid.joinIndex( i2, j2, k );
     }
          
     
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*nzs, indices, vals, do_local(flags) );

     // empty out the vertical profile
     profile.clear();
     
     // for each vertical level in the profile
     for ( k=0; k<nzs; k++ ) {
         
         // make the base index again
         idx = k*4;

         // all four values surrounding this location must be good
         if ( vals[idx+0] != bad && vals[idx+1] != bad 
           && vals[idx+2] != bad && vals[idx+3] != bad ) {

              // do the weighted average (see the header file for the inline
              //   function minicalc() )
              val = this->minicalc( lon, lat
                      , grid.longitude(is[idx+0]), grid.latitude(js[idx+0])
                      , grid.longitude(is[idx+3]), grid.latitude(js[idx+3])
                      , vals[idx+0], vals[idx+1], vals[idx+2], vals[idx+3] );

         } else {
              val = bad;
         }

         // store the horizontally-interpolated result in the profile
         profile.push_back(val);
     
     }

     // interpolate the horizontally-interplated profile vertically
     result = vin.profile( grid.levels(), profile, z, bad, flags );

     // drop all the temporary variables
     // before we leave
     delete indices;
     delete ks;
     delete js;
     delete is;
     delete vals;
     
     return result;         
     
}


//////////////////

void BilinearHinterp::vinterpVector( real lon, real lat, real z, real &xval, real &yval, const GridLatLonField3D& xgrid, const GridLatLonField3D& ygrid, const Vinterp& vin, int flags ) const 
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // the bad-or-missing-data fill value
     real xbad, ybad;
     // data values at the four grid points that surround each desired lat and lon
     real *xvals, *yvals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // direct indices into the data array 
     int *indices;
     // loop index for the locations we are interpolating to
     int i;
     // loop index for the grid vertical level
     int k;
     // dimensions of the input data grid
     int nlons, nlats, nzs;
     // a vector to hold horizontally-interpolated data,
     // which will subsequently be vertically interpolated
     std::vector<real> xprofile, yprofile;
     // index offset used in assembling indices from different grid levels
     int idx;
     // temporary variable to hold an interpolated result
     real val;
     // the vertical levels from the input data grid
     std::vector<real> zlevs;
     // temp values for conformal adjustment
     real xtmp, ytmp;
     // other temp values
     real x_val, y_val;
     // 
     real cdlon, sdlon;
     //
     real tmplon;


     if ( ! xgrid.compatible( ygrid ) )  {
        throw (badincompatible());
     }
     
     // get the dimensions of the input data grid
     xgrid.dims( &nlons, &nlats, &nzs );
     
     // the bad-or-missing-data fill value
     xbad = xgrid.fillval();
     ybad = ygrid.fillval();
     
     // create an array to hold the grid point values
     xvals = new real[4*nzs];
     yvals = new real[4*nzs];
     // create arrays to hold grid point index values
     is = new int[4*nzs];
     js = new int[4*nzs];
     ks = new int[4*nzs];
     indices = new int[4*nzs];
     
     // take care of any out-of-range longitude values
     lon = xgrid.wrap(lon);
     
     
     // get the i and j array coordinates that
     // correspond to this longitude and latitude
     xgrid.lonindex(lon, &i1, &i2);
     xgrid.latindex(lat, &j1, &j2);
     
     // for each vertical level...
     for ( k=0; k<nzs; k++ ) {
     
        // make the base index into the grid-index arrays
        idx = k*4;

        // set the four corders of the grid box that encompasses
        // our desired lat and lon location
        is[idx+0] = i1;
        js[idx+0] = j1;
        ks[idx+0] =  k;
        is[idx+1] = i1;
        js[idx+1] = j2;
        ks[idx+1] =  k;
        is[idx+2] = i2;
        js[idx+2] = j1;
        ks[idx+2] =  k;
        is[idx+3] = i2;
        js[idx+3] = j2;
        ks[idx+3] =  k;

        indices[idx + 0] = xgrid.joinIndex( i1, j1, k );
        indices[idx + 1] = xgrid.joinIndex( i1, j2, k );
        indices[idx + 2] = xgrid.joinIndex( i2, j1, k );
        indices[idx + 3] = xgrid.joinIndex( i2, j2, k );
     }

     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     // The 0x02 flag ensures that each gridpoints call results in a svr_done() call.
     xgrid.ask_for_data();
     xgrid.gridpoints( 4*nzs, indices, xvals, do_local(flags) | 0x02 );
     ygrid.ask_for_data();
     ygrid.gridpoints( 4*nzs, indices, yvals, do_local(flags) | 0x02 );

     if ( confml == 1 ) {
        if ( lat >= NEARPOLE || lat <= -NEARPOLE ) { 
           for ( k=0; k<nzs; k++ ) {
               idx = k*4;
               for ( i=0; i<4; i++ ) {
                   tmplon = xgrid.longitude(is[idx+i]);
                   cdlon = COS( (tmplon - lon)*RCONV  );
                   sdlon = SIN( (tmplon - lon)*RCONV  );
                   xtmp =  xvals[idx + i]*cdlon + yvals[idx + i]*sdlon; 
                   ytmp = -xvals[idx + i]*sdlon + yvals[idx + i]*cdlon;
                   xvals[idx + i] = xtmp;
                   yvals[idx + i] = ytmp;
               }
           }
        }
     }
     
     // empty out the vertical profile
     xprofile.clear();
     yprofile.clear();
     
     // for each vertical level in the profile
     for ( k=0; k<nzs; k++ ) {
         
         // make the base index again
         idx = k*4;

         // all four values surrounding this location must be good
         if ( xvals[idx+0] != xbad && xvals[idx+1] != xbad && xvals[idx+2] != xbad && xvals[idx+3] != xbad 
           && yvals[idx+0] != ybad && yvals[idx+1] != ybad && yvals[idx+2] != ybad && yvals[idx+3] != ybad ) {

              // do the weighted average (see the header file for the inline
              //   function minicalc() )
              x_val = this->minicalc( lon, lat
                      , xgrid.longitude(is[idx+0]), xgrid.latitude(js[idx+0])
                      , xgrid.longitude(is[idx+3]), xgrid.latitude(js[idx+3])
                      , xvals[idx+0], xvals[idx+1], xvals[idx+2], xvals[idx+3] );

              y_val = this->minicalc( lon, lat
                      , ygrid.longitude(is[idx+0]), ygrid.latitude(js[idx+0])
                      , ygrid.longitude(is[idx+3]), ygrid.latitude(js[idx+3])
                      , yvals[idx+0], yvals[idx+1], yvals[idx+2], yvals[idx+3] );

         } else {
              x_val = xbad;
              y_val = ybad;
         }


         // store the horizontally-interpolated result in the profile
         xprofile.push_back(x_val);
         yprofile.push_back(y_val);
     
     }

     // interpolate the horizontally-interplated profile vertically
     xval = vin.profile( xgrid.levels(), xprofile, z, xbad, flags );
     yval = vin.profile( ygrid.levels(), yprofile, z, ybad, flags );

     // drop all the temporary variables
     // before we leave
     delete indices;
     delete ks;
     delete js;
     delete is;
     delete xvals;
     delete yvals;
     
}

void BilinearHinterp::vinterpVector( real lon, real lat, real z, real &xval, real &yval, const GridField3D& xgrid, const GridField3D& ygrid, const Vinterp& vin, int flags ) const 
{
     vinterpVector( lon, lat, z, xval, yval, dynamic_cast<const GridLatLonField3D&>(xgrid), dynamic_cast<const GridLatLonField3D&>(ygrid), vin, flags );
}


void BilinearHinterp::vinterpVector( int n, const real* lons, const real* lats, const real* zs, real* xvals, real* yvals, const GridLatLonField3D& xgrid, const GridLatLonField3D& ygrid, const Vinterp& vin, int flags ) const 
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // the bad-or-missing-data fill value
     real xbad, ybad;
     // data values at the four grid points that surround each desired lat and lon
     real *xvalsprf, *yvalsprf;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // direct indices into the data array 
     int *indices;
     // loop index for the locations we are interpolating to
     int i;
     // loop index for the grid vertical level
     int k;
     // dimensions of the input data grid
     int nlons, nlats, nzs;
     // a vector to hold horizontally-interpolated data,
     // which will subsequently be vertically interpolated
     std::vector<real> xprofile, yprofile;
     // index offset used in assembling indices from different grid levels
     int idx;
     // temporary variable to hold an interpolated result
     real val;
     // the vertical levels from the input data grid
     std::vector<real> zlevs;
     // temp values for conformal adjustment
     real xtmp, ytmp;
     // other temp values
     real x_val, y_val;
     // lat, lon, vert
     real lat, lon, z;
     real lat1,lat2,lon1,lon2;
     
     // 
     real cdlon, sdlon;
     //
     real tmplon;

     if ( n <= 0 ) {
        return;
     }

     if ( ! xgrid.compatible( ygrid ) )  {
        throw (badincompatible());
     }
     
     // get the dimensions of the input data grid
     xgrid.dims( &nlons, &nlats, &nzs );
     
     // the bad-or-missing-data fill value
     xbad = xgrid.fillval();
     ybad = ygrid.fillval();
     
     // create an array to hold the grid point values
     xvalsprf = new real[4*nzs*n];
     yvalsprf = new real[4*nzs*n];

     // create arrays to hold grid point index values
     is = new int[4*nzs*n];
     js = new int[4*nzs*n];
     ks = new int[4*nzs*n];
     indices = new int[4*nzs*n];
     
     
     for ( int i=0; i<n; i++ ) {
      
         lon = xgrid.wrap( lons[i] );
         lat = lats[i];
         z = zs[i];
         
     
         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         xgrid.lonindex(lon, &i1, &i2);
         xgrid.latindex(lat, &j1, &j2);
     
         // for each vertical level...
         for ( k=0; k<nzs; k++ ) {
     
            // make the base index into the grid-index arrays
            idx = (i*nzs + k)*4;

            // set the four corders of the grid box that encompasses
            // our desired lat and lon location
            is[idx+0] = i1;
            js[idx+0] = j1;
            ks[idx+0] =  k;
            is[idx+1] = i1;
            js[idx+1] = j2;
            ks[idx+1] =  k;
            is[idx+2] = i2;
            js[idx+2] = j1;
            ks[idx+2] =  k;
            is[idx+3] = i2;
            js[idx+3] = j2;
            ks[idx+3] =  k;

            indices[idx + 0] = xgrid.joinIndex( i1, j1, k );
            indices[idx + 1] = xgrid.joinIndex( i1, j2, k );
            indices[idx + 2] = xgrid.joinIndex( i2, j1, k );
            indices[idx + 3] = xgrid.joinIndex( i2, j2, k );
         }

     }
     
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     // The 0x02 flag ensures that each gridpoints() call results in a svr_done() call.
     xgrid.ask_for_data();
     try {
     //xgrid.gridpoints( 4*nzs*n, indices, xvalsprf, do_local(flags) | 0x02 );
     xgrid.gridpoints( 4*nzs*n, is, js, ks, xvalsprf, do_local(flags) | 0x02 );
     } catch (...) {
         std::cerr << "************* HERE!" << std::endl;
         throw;
     }
     ygrid.ask_for_data();
     try {
     //ygrid.gridpoints( 4*nzs*n, indices, yvalsprf, do_local(flags) | 0x02 );
     ygrid.gridpoints( 4*nzs*n, is, js, ks, yvalsprf, do_local(flags) | 0x02 );
     } catch (...) {
         std::cerr << "************* THERE!" << std::endl;
         throw;
     }

     for ( i=0; i<n; i++ ) {

        lon = xgrid.wrap( lons[i] );
        lat = lats[i];
        z = zs[i];

        if ( confml == 1 ) {
           if ( lat >= NEARPOLE || lat <= -NEARPOLE ) { 
              for ( k=0; k<nzs; k++ ) {
                  idx = (i*nzs + k)*4;
                  for ( int ii=0; ii<4; ii++ ) {
                      tmplon = xgrid.longitude(is[idx+ii]);
                      cdlon = COS( (tmplon - lon)*RCONV  );
                      sdlon = SIN( (tmplon - lon)*RCONV  );
                      xtmp =  xvalsprf[idx + ii]*cdlon + yvalsprf[idx + ii]*sdlon; 
                      ytmp = -xvalsprf[idx + ii]*sdlon + yvalsprf[idx + ii]*cdlon;
                      xvalsprf[idx + ii] = xtmp;
                      yvalsprf[idx + ii] = ytmp;
                  }
              }
           }
        }
        
        // empty out the vertical profile
        xprofile.clear();
        yprofile.clear();
     
        // for each vertical level in the profile
        for ( k=0; k<nzs; k++ ) {
            
            // make the base index again
            idx = ( i*nzs + k)*4;
     
            // all four values surrounding this location must be good
            if ( xvalsprf[idx+0] != xbad && xvalsprf[idx+1] != xbad && xvalsprf[idx+2] != xbad && xvalsprf[idx+3] != xbad 
              && yvalsprf[idx+0] != ybad && yvalsprf[idx+1] != ybad && yvalsprf[idx+2] != ybad && yvalsprf[idx+3] != ybad ) {

                 // do the weighted average (see the header file for the inline
                 //   function minicalc() )
                 x_val = this->minicalc( lon, lat
                         , xgrid.longitude(is[idx+0]), xgrid.latitude(js[idx+0])
                         , xgrid.longitude(is[idx+3]), xgrid.latitude(js[idx+3])
                         , xvalsprf[idx+0], xvalsprf[idx+1], xvalsprf[idx+2], xvalsprf[idx+3] );

                 y_val = this->minicalc( lon, lat
                         , ygrid.longitude(is[idx+0]), ygrid.latitude(js[idx+0])
                         , ygrid.longitude(is[idx+3]), ygrid.latitude(js[idx+3])
                         , yvalsprf[idx+0], yvalsprf[idx+1], yvalsprf[idx+2], yvalsprf[idx+3] );

            } else {
                 x_val = xbad;
                 y_val = ybad;
            }


            // store the horizontally-interpolated result in the profile
            xprofile.push_back(x_val);
            yprofile.push_back(y_val);
     
        }

        // interpolate the horizontally-interplated profile vertically
        xvals[i] = vin.profile( xgrid.levels(), xprofile, z, xbad, flags );
        yvals[i] = vin.profile( ygrid.levels(), yprofile, z, ybad, flags );

     }
     
     // drop all the temporary variables
     // before we leave
     delete indices;
     delete ks;
     delete js;
     delete is;
     delete xvalsprf;
     delete yvalsprf;
     

}

void BilinearHinterp::vinterpVector( int n, const real* lons, const real* lats, const real* zs, real* xvals, real* yvals, const GridField3D& xgrid, const GridField3D& ygrid, const Vinterp& vin, int flags ) const 
{
     vinterpVector( n, lons, lats, zs, xvals, yvals, dynamic_cast<const GridLatLonField3D&>(xgrid), dynamic_cast<const GridLatLonField3D&>(ygrid), vin, flags );
}

void BilinearHinterp::vinterpVector( int n, const real* lons, const real* lats, const real* zs, real* xvals, real* yvals, const GridCubedSphereField3D& xgrid, const GridCubedSphereField3D& ygrid, const Vinterp& vin, int flags ) const 
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // the bad-or-missing-data fill value
     real xbad, ybad;
     // data values at the four grid points that surround each desired lat and lon
     real *xvalsprf, *yvalsprf;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // direct indices into the data array 
     int *indices;
     // loop index for the locations we are interpolating to
     int i;
     // loop index for the grid vertical level
     int k;
     // dimensions of the input data grid
     int nlons, nlats, nzs;
     // a vector to hold horizontally-interpolated data,
     // which will subsequently be vertically interpolated
     std::vector<real> xprofile, yprofile;
     // index offset used in assembling indices from different grid levels
     int idx;
     // temporary variable to hold an interpolated result
     real val;
     // the vertical levels from the input data grid
     std::vector<real> zlevs;
     // temp values for conformal adjustment
     real xtmp, ytmp;
     // other temp values
     real x_val, y_val;
     // lat, lon, vert
     real lat, lon, z;
     real lat1,lat2,lon1,lon2;
     int pos0, pos1, pos2, pos3;
     
     // 
     real cdlon, sdlon;
     //
     real tmplon;

     if ( n <= 0 ) {
        return;
     }

     if ( ! xgrid.compatible( ygrid ) )  {
        throw (badincompatible());
     }
     
     // get the dimensions of the input data grid
     xgrid.dims( &nlons, &nlats, &nzs );
     
     // the bad-or-missing-data fill value
     xbad = xgrid.fillval();
     ybad = ygrid.fillval();
     
     // create an array to hold the grid point values
     xvalsprf = new real[4*nzs*n];
     yvalsprf = new real[4*nzs*n];

     // create arrays to hold grid point index values
     is = new int[4*nzs*n];
     js = new int[4*nzs*n];
     ks = new int[4*nzs*n];
     indices = new int[4*nzs*n];
     
     
     for ( int i=0; i<n; i++ ) {
      
         lon = xgrid.wrap( lons[i] );
         lat = lats[i];
         z = zs[i];
         
     
         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         //xgrid.lonindex(lon, &i1, &i2);
         //xgrid.latindex(lat, &j1, &j2);
         xgrid.latlonindex(lat,lon, i1, j1);
         //convert to lower-left
         i1 = (i1+1)/2;
         j1 = (j1+1)/2;
         // convert to relative to halo
         i1 = i1 - xgrid.iStart;
         j1 = j1 - xgrid.jStart;

         i2 = i1+1;    
         j2 = j1+1;    

         // for each vertical level...
         for ( k=0; k<nzs; k++ ) {
     
            // make the base index into the grid-index arrays
            idx = (i*nzs + k)*4;

            // set the four corders of the grid box that encompasses
            // our desired lat and lon location
            is[idx+0] = i1;
            js[idx+0] = j1;
            ks[idx+0] =  k;
            is[idx+1] = i1;
            js[idx+1] = j2;
            ks[idx+1] =  k;
            is[idx+2] = i2;
            js[idx+2] = j1;
            ks[idx+2] =  k;
            is[idx+3] = i2;
            js[idx+3] = j2;
            ks[idx+3] =  k;

            indices[idx + 0] = xgrid.joinIndex( i1, j1, k );
            indices[idx + 1] = xgrid.joinIndex( i1, j2, k );
            indices[idx + 2] = xgrid.joinIndex( i2, j1, k );
            indices[idx + 3] = xgrid.joinIndex( i2, j2, k );
         }

     }
     
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     // The 0x02 flag ensures that each gridpoints() call results in a svr_done() call.
     xgrid.ask_for_data();
     try {
     //xgrid.gridpoints( 4*nzs*n, indices, xvalsprf, do_local(flags) | 0x02 );
     xgrid.gridpoints( 4*nzs*n, is, js, ks, xvalsprf, do_local(flags) | 0x02 );
     } catch (...) {
         std::cerr << "************* HERE!" << std::endl;
         throw;
     }
     ygrid.ask_for_data();
     try {
     //ygrid.gridpoints( 4*nzs*n, indices, yvalsprf, do_local(flags) | 0x02 );
     ygrid.gridpoints( 4*nzs*n, is, js, ks, yvalsprf, do_local(flags) | 0x02 );
     } catch (...) {
         std::cerr << "************* THERE!" << std::endl;
         throw;
     }

     for ( i=0; i<n; i++ ) {

        lon = xgrid.wrap( lons[i] );
        lat = lats[i];
        z = zs[i];

        if ( confml == 1 ) {
           if ( lat >= NEARPOLE || lat <= -NEARPOLE ) { 
              for ( k=0; k<nzs; k++ ) {
                  idx = (i*nzs + k)*4;
                  for ( int ii=0; ii<4; ii++ ) {
                      pos0 = js[idx+ii]*nlons + is[idx+ii];
                      tmplon = xgrid.longitude(pos0);
                      cdlon = COS( (tmplon - lon)*RCONV  );
                      sdlon = SIN( (tmplon - lon)*RCONV  );
                      xtmp =  xvalsprf[idx + ii]*cdlon + yvalsprf[idx + ii]*sdlon; 
                      ytmp = -xvalsprf[idx + ii]*sdlon + yvalsprf[idx + ii]*cdlon;
                      xvalsprf[idx + ii] = xtmp;
                      yvalsprf[idx + ii] = ytmp;
                  }
              }
           }
        }
        
        // empty out the vertical profile
        xprofile.clear();
        yprofile.clear();
     
        // for each vertical level in the profile
        for ( k=0; k<nzs; k++ ) {
            
            // make the base index again
            idx = ( i*nzs + k)*4;
     
            // all four values surrounding this location must be good
            if ( xvalsprf[idx+0] != xbad && xvalsprf[idx+1] != xbad && xvalsprf[idx+2] != xbad && xvalsprf[idx+3] != xbad 
              && yvalsprf[idx+0] != ybad && yvalsprf[idx+1] != ybad && yvalsprf[idx+2] != ybad && yvalsprf[idx+3] != ybad ) {

                 // do the weighted average (see the header file for the inline
                 //   function minicalc() )
                 pos0 = js[idx+0]*nlons + is[idx+0];
                 pos1 = js[idx+1]*nlons + is[idx+1];
                 pos2 = js[idx+2]*nlons + is[idx+2];
                 pos3 = js[idx+3]*nlons + is[idx+3];

                 x_val = minicalc( lon, lats[i]
                          , xgrid.longitude(pos0), xgrid.latitude(pos0)
                          , xgrid.longitude(pos1), xgrid.latitude(pos1)
                          , xgrid.longitude(pos2), xgrid.latitude(pos2)
                          , xgrid.longitude(pos3), xgrid.latitude(pos3)
                          , xvalsprf[idx+0], xvalsprf[idx+1], xvalsprf[idx+2], xvalsprf[idx+3] );
                 y_val = minicalc( lon, lats[i]
                          , ygrid.longitude(pos0), ygrid.latitude(pos0)
                          , ygrid.longitude(pos1), ygrid.latitude(pos1)
                          , ygrid.longitude(pos2), ygrid.latitude(pos2)
                          , ygrid.longitude(pos3), ygrid.latitude(pos3)
                         , yvalsprf[idx+0], yvalsprf[idx+1], yvalsprf[idx+2], yvalsprf[idx+3] );

                /* x_val = xgrid->minicalc( lon, lat
                         , xgrid.longitude(is[idx+0]), xgrid.latitude(js[idx+0])
                         , xgrid.longitude(is[idx+3]), xgrid.latitude(js[idx+3])
                         , xvalsprf[idx+0], xvalsprf[idx+1], xvalsprf[idx+2], xvalsprf[idx+3] );

                 y_val = this->minicalc( lon, lat
                         , ygrid.longitude(is[idx+0]), ygrid.latitude(js[idx+0])
                         , ygrid.longitude(is[idx+3]), ygrid.latitude(js[idx+3])
                         , yvalsprf[idx+0], yvalsprf[idx+1], yvalsprf[idx+2], yvalsprf[idx+3] );
               */
            } else {
                 x_val = xbad;
                 y_val = ybad;
            }


            // store the horizontally-interpolated result in the profile
            xprofile.push_back(x_val);
            yprofile.push_back(y_val);
     
        }

        // interpolate the horizontally-interplated profile vertically
        xvals[i] = vin.profile( xgrid.levels(), xprofile, z, xbad, flags );
        yvals[i] = vin.profile( ygrid.levels(), yprofile, z, ybad, flags );

     }
     
     // drop all the temporary variables
     // before we leave
     delete indices;
     delete ks;
     delete js;
     delete is;
     delete xvalsprf;
     delete yvalsprf;
     
}
//////////////////

std::vector<real>* BilinearHinterp::vinterp( const std::vector<real>& lons, const std::vector<real>& lats, const std::vector<real>& zs, const GridLatLonField3D& grid, const Vinterp& vin, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // vector to hold the output interpolated values
     std::vector<real> *results;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // direct indices into the data array 
     int *indices;
     // loop index for the locations we are interpolating to
     int i;
     // loop index for the grid vertical level
     int k;
     // temporary longitude variable
     real lon;
     // dimensions of the input data grid
     int nlons, nlats, nzs;
     // a vector to hold horizontally-interpolated data,
     // which will subsequently be vertically interpolated
     std::vector<real> profile;
     // index offset used in assembling indices from different grid levels
     int idx;
     // temporary variable to hold an interpolated result
     real val;   
     // the number of input points to be interpolated
     int n;
     
     // how many input points?
     n = lons.size();

     // make something to hold the output results
     results = new std::vector<real>;
     results->reserve(n);
     
     
     // get the dimensions of the input data grid
     grid.dims( &nlons, &nlats, &nzs );
     
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create an array to hold the grid point values
     vals = new real[4*n*nzs];
     // create arrays to hold grid point index values
     is = new int[4*n*nzs];
     js = new int[4*n*nzs];
     ks = new int[4*n*nzs];
     indices = new int[4*n*nzs];
     
     // for each input lat/lon location...     
     for ( i=0; i<n; i++ ) {
         
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);
         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         grid.lonindex(lon, &i1, &i2);
         grid.latindex(lats[i], &j1, &j2);
         if ( j1 > 1000 || j2 > 1000 ) {
            std::cerr << "***OUCH! ***" << i << " of " << n << ": " << lats[i] << " -> " << j1 << ", " << j2 << std::endl;
         }
         
         // for each vertical level...
         for ( k=0; k<nzs; k++ ) {
         
            // make the base index into the grid-index arrays
            idx = ( k + i*nzs )*4;

            // set the four corders of the grid box that encompasses
            // our desired lat and lon location
            is[idx+0] = i1;
            js[idx+0] = j1;
            ks[idx+0] =  k;
            is[idx+1] = i1;
            js[idx+1] = j2;
            ks[idx+1] =  k;
            is[idx+2] = i2;
            js[idx+2] = j1;
            ks[idx+2] =  k;
            is[idx+3] = i2;
            js[idx+3] = j2;
            ks[idx+3] =  k;

            indices[idx + 0] = grid.joinIndex( i1, j1, k );
            indices[idx + 1] = grid.joinIndex( i1, j2, k );
            indices[idx + 2] = grid.joinIndex( i2, j1, k );
            indices[idx + 3] = grid.joinIndex( i2, j2, k );
         }
     }
          
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n*nzs, indices, vals, do_local(flags) );

     // for each input point
     for ( i=0; i<n; i++ ) {
         
         // wrap the longitude again
         lon = grid.wrap(lons[i]);
         
         // empty out the vertical profile
         profile.clear();
         
         // for each vertical level in the profile
         for ( k=0; k<nzs; k++ ) {
             
             // make the base index again
             idx = ( k + i*nzs )*4;

             // all four values surrounding this location must be good
             if ( vals[idx+0] != bad && vals[idx+1] != bad 
               && vals[idx+2] != bad && vals[idx+3] != bad ) {

                  // do the weighted average (see the header file for the inline
                  //   function minicalc() )
                  val = this->minicalc( lon, lats[i]
                          , grid.longitude(is[idx+0]), grid.latitude(js[idx+0])
                          , grid.longitude(is[idx+3]), grid.latitude(js[idx+3])
                          , vals[idx+0], vals[idx+1], vals[idx+2], vals[idx+3] );

             } else {
                  val = bad;
             }

             // store the horizontally-interpolated result in the profile
             profile.push_back(val);
        
         }

         // interpolate the horizontally-interplated profile vertically,
         results->push_back( vin.profile( grid.levels(), profile, zs[i], bad, flags ) );

     }
     

     // drop all the temporary variables
     // before we leave
     delete indices;
     delete ks;
     delete js;
     delete is;
     delete vals;
     
     return results;         
     

}


real* BilinearHinterp::vinterp( const int n, const real* lons, const real* lats, const real* zs, const GridLatLonField3D& grid, const Vinterp& vin, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // pointer to an array that will hold the output interpolated values
     real *results;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // direct indices into the data array 
     int *indices;
     // loop index for the locations we are interpolating to
     int i;
     // loop index for the grid vertical level
     int k;
     // temporary longitude variable
     real lon;
     // dimensions of the input data grid
     int nlons, nlats, nzs;
     // a vector to hold horizontally-interpolated data,
     // which will subsequently be vertically interpolated
     std::vector<real> profile;
     // index offset used in assembling indices from different grid levels
     int idx;
     // temporary variable to hold an interpolated result
     real val;
     
     
     // get the dimensions of the input data grid
     grid.dims( &nlons, &nlats, &nzs );
     
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // make something to hold the output results
     results = new real[n];
     // create an array to hold the grid point values
     vals = new real[4*n*nzs];
     // create arrays to hold grid point index values
     is = new int[4*n*nzs];
     js = new int[4*n*nzs];
     ks = new int[4*n*nzs];
     indices = new int[4*n*nzs];
     
     // for each input lat/lon location...     
     for ( i=0; i<n; i++ ) {
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);
         
         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         grid.lonindex(lon, &i1, &i2);
         grid.latindex(lats[i], &j1, &j2);
         
         // for each vertical level...
         for ( k=0; k<nzs; k++ ) {
          
            // make the base index into the grid-index arrays
            idx = ( k + i*nzs )*4;

            // set the four corders of the grid box that encompasses
            // our desired lat and lon location
            is[idx+0] = i1;
            js[idx+0] = j1;
            ks[idx+0] =  k;
            is[idx+1] = i1;
            js[idx+1] = j2;
            ks[idx+1] =  k;
            is[idx+2] = i2;
            js[idx+2] = j1;
            ks[idx+2] =  k;
            is[idx+3] = i2;
            js[idx+3] = j2;
            ks[idx+3] =  k;

            indices[idx + 0] = grid.joinIndex( i1, j1, k );
            indices[idx + 1] = grid.joinIndex( i1, j2, k );
            indices[idx + 2] = grid.joinIndex( i2, j1, k );
            indices[idx + 3] = grid.joinIndex( i2, j2, k );
            
         }
     }
          
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n*nzs, indices, vals, do_local(flags) );

     // for each input point
     for ( i=0; i<n; i++ ) {
         
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);
         
         // empty out the vertical profile
         profile.clear();
         
         // for each vertical level in the profile
         for ( k=0; k<nzs; k++ ) {
             
             // make the base index again
             idx = ( k + i*nzs )*4;

             // all four values surrounding this location must be good
             if ( vals[idx+0] != bad && vals[idx+1] != bad 
               && vals[idx+2] != bad && vals[idx+3] != bad ) {

                  // do the weighted average (see the header file for the inline
                  //   function minicalc() )
                  val = this->minicalc( lon, lats[i]
                          , grid.longitude(is[idx+0]), grid.latitude(js[idx+0])
                          , grid.longitude(is[idx+3]), grid.latitude(js[idx+3])
                          , vals[idx+0], vals[idx+1], vals[idx+2], vals[idx+3] );

             } else {
                  val = bad;
             }

             // store the horizontally-interpolated result in the profile
             profile.push_back(val);
        
         }

         // interpolate the horizontally-interplated profile vertically,
         results[i] = vin.profile( grid.levels(), profile, zs[i], bad, flags );

     }
     

     // drop all the temporary variables
     // before we leave
     delete indices;
     delete ks;
     delete js;
     delete is;
     delete vals;
     
     return results;         
     
}

void BilinearHinterp::vinterp( const int n, const real* lons, const real* lats, const real* zs, real* results, const GridLatLonField3D& grid, const Vinterp& vin, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // direct indices into the data array 
     int *indices;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // loop index for the locations we are interpolating to
     int i;
     // loop index for the grid vertical level
     int k;
     // temporary longitude variable
     real lon;
     // dimensions of the input data grid
     int nlons, nlats, nzs;
     // a vector to hold horizontally-interpolated data,
     // which will subsequently be vertically interpolated
     std::vector<real> profile;
     // index offset used in assembling indices from different grid levels
     int idx;
     // temporary variable to hold an interpolated result
     real val;
     
     
     // get the dimensions of the input data grid
     grid.dims( &nlons, &nlats, &nzs );
     
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create an array to hold the grid point values
     vals = new real[4*n*nzs];
     // create arrays to hold grid point index values
     is = new int[4*n*nzs];
     js = new int[4*n*nzs];
     ks = new int[4*n*nzs];
     indices = new int[4*n*nzs];
     
     // for each input lat/lon location...     
     for ( i=0; i<n; i++ ) {
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);
         
         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         grid.lonindex(lon, &i1, &i2);
         grid.latindex(lats[i], &j1, &j2);
         
         // for each vertical level...
         for ( k=0; k<nzs; k++ ) {
          
            // make the base index into the grid-index arrays
            idx = ( k + i*nzs )*4;

            // set the four corders of the grid box that encompasses
            // our desired lat and lon location
            is[idx+0] = i1;
            js[idx+0] = j1;
            ks[idx+0] =  k;
            is[idx+1] = i1;
            js[idx+1] = j2;
            ks[idx+1] =  k;
            is[idx+2] = i2;
            js[idx+2] = j1;
            ks[idx+2] =  k;
            is[idx+3] = i2;
            js[idx+3] = j2;
            ks[idx+3] =  k;

            indices[idx + 0] = grid.joinIndex( i1, j1, k );
            indices[idx + 1] = grid.joinIndex( i1, j2, k );
            indices[idx + 2] = grid.joinIndex( i2, j1, k );
            indices[idx + 3] = grid.joinIndex( i2, j2, k );
         }
     }
          
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n*nzs, indices, vals, do_local(flags) );

     // for each input point
     for ( i=0; i<n; i++ ) {
         
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);
         
         // empty out the vertical profile
         profile.clear();
         
         // for each vertical level in the profile
         for ( k=0; k<nzs; k++ ) {
             
             // make the base index again
             idx = ( k + i*nzs )*4;

             // all four values surrounding this location must be good
             if ( vals[idx+0] != bad && vals[idx+1] != bad 
               && vals[idx+2] != bad && vals[idx+3] != bad ) {

                  // do the weighted average (see the header file for the inline
                  //   function minicalc() )
                  val = this->minicalc( lon, lats[i]
                          , grid.longitude(is[idx+0]), grid.latitude(js[idx+0])
                          , grid.longitude(is[idx+3]), grid.latitude(js[idx+3])
                          , vals[idx+0], vals[idx+1], vals[idx+2], vals[idx+3] );

             } else {
                  val = bad;
             }

             // store the horizontally-interpolated result in the profile
             profile.push_back(val);
        
         }

         // interpolate the horizontally-interplated profile vertically,
         results[i] = vin.profile( grid.levels(), profile, zs[i], bad, flags );

     }
     

     // drop all the temporary variables
     // before we leave
     delete indices;
     delete ks;
     delete js;
     delete is;
     delete vals;
     
}

void BilinearHinterp::vinterp( const int n, const real* lons, const real* lats, const real* zs, real* results, const GridField3D& grid, const Vinterp& vin, int flags ) const
{
    // downcast and interpolate
    vinterp( n, lons, lats, zs, results, dynamic_cast<const GridLatLonField3D&>(grid), vin, flags);

}
  
void BilinearHinterp::vinterp( const int n, const real* lons, const real* lats, const real* zs, real* results, const GridCubedSphereField3D& grid, const Vinterp& vin, int flags ) const
{
     // array indices that bound a desired longitude
     int i1, i2;
     // array indices that bound a desired latitude
     int j1, j2;
     // the bad-or-missing-data fill value
     real bad;
     // data values at the four grid points that surround each desired lat and lon
     real *vals;
     // longitude indices of the four grid points that surround each desired lat and lon
     int *is;
     // latitude indices of the four grid points that surround each desired lat and lon
     int *js;
     // direct indices into the data array 
     int *indices;
     // vertical grid point of the surface/level within the 3D object
     // on which we are doing the interpolation
     int *ks;
     // loop index for the locations we are interpolating to
     int i;
     // loop index for the grid vertical level
     int k;
     // temporary longitude variable
     real lon;
     // dimensions of the input data grid
     int nlons, nlats, nzs;
     // a vector to hold horizontally-interpolated data,
     // which will subsequently be vertically interpolated
     std::vector<real> profile;
     // index offset used in assembling indices from different grid levels
     int idx;
     // temporary variable to hold an interpolated result
     real val;
     int pos0, pos1, pos2, pos3;
     
     
     // get the dimensions of the input data grid
     grid.dims( &nlons, &nlats, &nzs );
     
     
     // the bad-or-missing-data fill value
     bad = grid.fillval();
     
     // create an array to hold the grid point values
     vals = new real[4*n*nzs];
     // create arrays to hold grid point index values
     is = new int[4*n*nzs];
     js = new int[4*n*nzs];
     ks = new int[4*n*nzs];
     indices = new int[4*n*nzs];
     
     // for each input lat/lon location...     
     for ( i=0; i<n; i++ ) {
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);
         
         // get the i and j array coordinates that
         // correspond to this longitude and latitude
         //grid.lonindex(lon, &i1, &i2);
         //grid.latindex(lats[i], &j1, &j2);
         // i1, i2 is left-lower conner
         grid.latlonindex(lats[i], lon, i1, j1);
         //convert to lower-left
         i1 = (i1+1)/2;
         j1 = (j1+1)/2;
         // convert to relative to halo
         i1 = i1 - grid.iStart;
         j1 = j1 - grid.jStart;
         
         i2 = i1+1;    
         j2 = j1+1; 

         // for each vertical level...
         for ( k=0; k<nzs; k++ ) {
          
            // make the base index into the grid-index arrays
            idx = ( k + i*nzs )*4;

            // set the four corders of the grid box that encompasses
            // our desired lat and lon location
            is[idx+0] = i1;
            js[idx+0] = j1;
            ks[idx+0] =  k;
            is[idx+1] = i1;
            js[idx+1] = j2;
            ks[idx+1] =  k;
            is[idx+2] = i2;
            js[idx+2] = j1;
            ks[idx+2] =  k;
            is[idx+3] = i2;
            js[idx+3] = j2;
            ks[idx+3] =  k;

            indices[idx + 0] = grid.joinIndex( i1, j1, k );
            indices[idx + 1] = grid.joinIndex( i1, j2, k );
            indices[idx + 2] = grid.joinIndex( i2, j1, k );
            indices[idx + 3] = grid.joinIndex( i2, j2, k );
         }
     }
          
     // Get the values at those four grid points.
     // (We do it this way in order to take advantage
     // of any parallization or caching, which are handled
     // by the grid object. 
     grid.ask_for_data();
     grid.gridpoints( 4*n*nzs, indices, vals, do_local(flags) );

     // for each input point
     for ( i=0; i<n; i++ ) {
         
         // take care of any out-of-range longitude values
         lon = grid.wrap(lons[i]);
         
         // empty out the vertical profile
         profile.clear();
         
         // for each vertical level in the profile
         for ( k=0; k<nzs; k++ ) {
             
             // make the base index again
             idx = ( k + i*nzs )*4;

             // all four values surrounding this location must be good
             if ( vals[idx+0] != bad && vals[idx+1] != bad 
               && vals[idx+2] != bad && vals[idx+3] != bad ) {

                  // do the weighted average (see the header file for the inline
                  //   function minicalc() )
                 
                  pos0 = js[idx+0]*nlons + is[idx+0];
                  pos1 = js[idx+1]*nlons + is[idx+1];
                  pos2 = js[idx+2]*nlons + is[idx+2];
                  pos3 = js[idx+3]*nlons + is[idx+3];
 
                  val = minicalc( lon, lats[i]
                          , grid.longitude(pos0), grid.latitude(pos0)
                          , grid.longitude(pos1), grid.latitude(pos1)
                          , grid.longitude(pos2), grid.latitude(pos2)
                          , grid.longitude(pos3), grid.latitude(pos3)
                          , vals[idx+0], vals[idx+1], vals[idx+2], vals[idx+3] );

             } else {
                  val = bad;
             }

             // store the horizontally-interpolated result in the profile
             profile.push_back(val);
        
         }

         // interpolate the horizontally-interplated profile vertically,
         results[i] = vin.profile( grid.levels(), profile, zs[i], bad, flags );

     }
     

     // drop all the temporary variables
     // before we leave
     delete indices;
     delete ks;
     delete js;
     delete is;
     delete vals;
     
}

//----------------------  standard methods follow ------------------------------------------

real BilinearHinterp::calc( real lon, real lat, const GridField3D& grid, const int k, int flags ) const
{
    // downcast and interpolate
    return calc( lon, lat, dynamic_cast<const GridLatLonField3D&>(grid), k, flags);

}

real BilinearHinterp::calc( real lon, real lat, const GridFieldSfc& grid, int flags ) const
{
    // downcast and interpolate
    return calc( lon, lat, dynamic_cast<const GridLatLonFieldSfc&>(grid), flags);

}

std::vector<real>* BilinearHinterp::calc( const std::vector<real>& lon, const std::vector<real>& lat, const GridField3D& grid, const int k, int flags ) const
{
    // downcast and interpolate
    return calc( lon, lat, dynamic_cast<const GridLatLonField3D&>(grid), k, flags);

}

std::vector<real>* BilinearHinterp::calc( const std::vector<real>& lon, const std::vector<real>& lat, const GridFieldSfc& grid, int flags ) const
{
    // downcast and interpolate
    return calc( lon, lat, dynamic_cast<const GridLatLonFieldSfc&>(grid), flags);

}

real* BilinearHinterp::calc( const int n, const real* lons, const real* lats, const GridField3D& grid, const int k, int flags ) const
{
    // downcast and interpolate
    return calc( n, lons, lats, dynamic_cast<const GridLatLonField3D&>(grid), k, flags);

}

real* BilinearHinterp::calc( const int n, const real* lons, const real* lats, const GridFieldSfc& grid, int flags ) const
{
    // downcast and interpolate
    return calc( n, lons, lats, dynamic_cast<const GridLatLonFieldSfc&>(grid), flags);

}

GridFieldSfc* BilinearHinterp::regrid( const GridFieldSfc& input, const std::vector<real>& x, const std::vector<real>& y, int flags ) const
{
    // downcast and regrid
    return regrid( dynamic_cast<const GridLatLonFieldSfc&>(input), x, y,  flags);

}

GridField3D* BilinearHinterp::regrid( const GridField3D& input, const std::vector<real>& x, const std::vector<real>& y, const std::vector<real>& z, const Vinterp& vin, int flags ) const
{
    // downcast and regrid
    return regrid( dynamic_cast<const GridLatLonField3D&>(input), x, y,  z, vin, flags);

}

GridField3D* BilinearHinterp::regrid( const GridField3D& input, const std::vector<real>& x, const std::vector<real>& y, int flags ) const
{
    // and regrid
    return regrid( dynamic_cast<const GridLatLonField3D&>(input),  x, y, flags);

}

real BilinearHinterp::vinterp( real lon, real lat, real z, const GridField3D& grid, const Vinterp& vin, int flags ) const
{
    // downcast and interpolate
    return vinterp( lon, lat, z, dynamic_cast<const GridLatLonField3D&>(grid), vin, flags);

}

std::vector<real>* BilinearHinterp::vinterp( const std::vector<real>& lons, const std::vector<real>& lats, const std::vector<real>& zs, const GridField3D& grid, const Vinterp& vin, int flags ) const
{
    // downcast and interpolate
    return vinterp( lons, lats, zs, dynamic_cast<const GridLatLonField3D&>(grid), vin, flags);

}

real* BilinearHinterp::vinterp( const int n, const real* lons, const real* lats, const real* zs, const GridField3D& grid, const Vinterp& vin, int flags ) const
{
    // downcast and interpolate
    return vinterp( n, lons, lats, zs,dynamic_cast<const GridLatLonField3D&>(grid), vin, flags);

}

real BilinearHinterp::gcdist( real lat1,real long1,real lat2,real long2) const
{
  real d;
  //real long1; //!! longitude of first site [rad]
  //real lat1;  // !! latitude of the first site [rad]
  //real long2; // !! longitude of the second site [rad]
  //real lat2;  //  !! latitude of the second site [rad]

  real clat1,clat2,slat1,slat2,dlon,cdlon;
  double pi = 3.1415926535;
  double c2r = pi/180.0;
  clat1 = std::cos(lat1*c2r);
  clat2 = std::cos(lat2*c2r);
  slat1 = std::sin(lat1*c2r);
  slat2 = std::sin(lat2*c2r);
  dlon = long1-long2;
  cdlon = std::cos(dlon*c2r);

  d = acos(slat1*slat2+clat1*clat2*cdlon);
  return d;
}

real BilinearHinterp::minicalc( real lon, real lat
                                 , real lon11, real lat11
                                 , real lon21, real lat21
                                 , real lon12, real lat12
                                 , real lon22, real lat22
                                 , real d11, real d21, real d12, real d22 ) const
{
   // assuming all lon are from 0:360
   real result;
   real dis, dis11, dis12, dis21, dis22;

   dis11 = gcdist(lat,lon,lat11,lon11);
   dis12 = gcdist(lat,lon,lat12,lon12);
   dis21 = gcdist(lat,lon,lat21,lon21);
   dis22 = gcdist(lat,lon,lat22,lon22);
   dis = dis11+dis12+dis21+dis22;

   result = dis11/dis * d11 + dis12/dis * d12 + dis21/dis * d21 + dis22/dis * d22;

   return result;
}

