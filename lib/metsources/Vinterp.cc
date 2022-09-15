
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include <string>
#include <stdlib.h>
#include <sstream>

#include "gigatraj/Vinterp.hh"

using namespace gigatraj;

GridFieldSfc* Vinterp::surface( const real z, const GridField3D& grid, const GridField3D& vgrid, int flags ) const
{
     // profile-iterator to let us sweep through the horizontal 
     // grid, grabbing a profile at each horizontal gridpoint
     GridField3D::const_profileIterator gridprf;
     // same as gridprf, but for the vertical-coordinate grid vgrid
     GridField3D::const_profileIterator vgridprf;
     // vector of vertical profile data extracted from the vgrid parameter
     const std::vector<real>* vgriddata;
     // vector of vertical profile data extracted from the grid parameter
     const std::vector<real>* griddata;
     // gridpoint interator for the output surface
     GridFieldSfc::iterator dest;
     // the surface to be output with the interpolated dvalues
     GridFieldSfc *newgrid;
     // number of vertical levels of grid and vgrid
     int nzs;
     // bad-or-missing-data fill values for grid and vgrid, respectively
     real bad, vbad;
     // direction in which the vertical coordinate increases
     int dir;
     // the number of horizontal gridpoints
     int n;
     // loop index for the zs levels being interpolated to
     int idx;
     // temporary interpolated variable
     real value;
     // used to assemble the parts of the surface value label
     std::ostringstream sfcval;
     
     // Note:
     //   this does NOT use the gridpoints() method, so it is NOT suitable
     //   for use in a tracing-only processor that does not hold the
     //   met data locally.  In those circumstance, this method will
     //   run, but it will be slow.
     
     
     // the two input grids must be compatible in every respect
     if ( ! grid.compatible(vgrid) ) {
         throw (badincompatible());
     } 

     // get both bad-flag values
     bad = grid.fillval();
     vbad = vgrid.fillval();
     
     // grab the lowest surface of the data grid, to get its metadata
     newgrid = grid.extractSurface(0);

     // set the surface value string
     sfcval << vgrid.quantity() << ": " << z << " " << vgrid.units();
     newgrid->set_surface(sfcval.str());

     // how many grid points in the surface
     n = newgrid->dataSize();
     
     // the number of vertical grid points
     nzs = grid.dataSize()/n;

     // we do not know whether the vertical quantity grows or shrinks with altitude
     dir = 0;

     
     // for each surface gridpoint...
     for ( gridprf = grid.profileBegin(), vgridprf = vgrid.profileBegin(), dest = newgrid->begin(); 
           gridprf !=grid.profileEnd() ; 
           gridprf++, vgridprf++, dest++ ) {
        
         // note:
         //    griddata is the old dependent data, to be interpolated (length zns) 
         //    oldlevels is not used here (length nzs)
         //    vgriddata is the old independent coord data  (length nzs)  
         //    zs is the new independent coord data (length n)
         griddata = *gridprf;
         vgriddata = *vgridprf;

         if ( dir == 0 )  {     // if we don't know the direction yet...
            // ...find it out now
            dir = getDirection(  vgriddata, vbad );      
         } 

         // assume that we cannot interpolate until we know we can
         value = bad;
            
         if ( dir != 0 ) {
            // do the interpolation
            value =  interp( griddata, vgriddata, z, bad, vbad, dir ); 
         }

         // store the result in the output surface
         *dest = value;
         
         // drop the temporary variables
         delete griddata;
         delete vgriddata;

     }     

     return newgrid;
}

GridFieldSfc* Vinterp::surface( const GridField3D& grid, const GridFieldSfc& vsfc, int flags ) const
{
     // profile-iterator to let us sweep through the horizontal 
     // grid, grabbing a profile at each horizontal gridpoint
     GridField3D::const_profileIterator gridprf;
     // iterator to go over the gridpoints of the vsfc surface
     GridFieldSfc::const_iterator sfcpnt;
     // vector of vertical profile data extracted from the vgrid parameter
     std::vector<real> vgriddata;
     // vector of vertical profile data extracted from the grid parameter
     const std::vector<real>* griddata;
     // iterator to go over the gridpoints of the output surface
     GridFieldSfc::iterator dest;
     // the surface to be output with the interpolated dvalues
     GridFieldSfc *newgrid;
     // number of vertical levels of grid and vgrid
     int nzs;
     // bad-or-missing-data fill values for grid and vgrid, respectively
     real bad, vbad;
     // direction in which the vertical coordinate increases
     int dir;
     // the number of horizontal gridpoints
     int n;
     // loop index for the zs levels being interpolated to
     int idx;
     // temporary interpolated variable
     real value;
     // vertical level value
     real z;
     
     // Note:
     //   this does NOT use the gridpoints() method, so it is NOT suitable
     //   for use in a tracing-only processor that does not hold the
     //   met data locally.
     
     
     // the two input grids must be compatible horizontally and in time
     if ( ! grid.compatible(vsfc, METCOMPAT_HORIZ | METCOMPAT_TIME ) ) {
         throw (badincompatible());
     } 

     // get both bad-flag values
     bad = grid.fillval();
     vbad = vsfc.fillval();
     
     // grab the lowest surface of the data grid, to get its metadata
     newgrid = grid.extractSurface(0);

     // copy the surface value string
     newgrid->set_surface(vsfc.surface());

     // how many grid points in the surface
     n = newgrid->dataSize();
     
     // the number of vertical grid points
     nzs = grid.dataSize()/n;

     // we do not know whether the vertical quantity grows or shrinks with altitude
     dir = 0;

     
     //    vgriddata is the old independent coord data  (length nzs)  
     vgriddata = grid.levels();
          
     // for each surface gridpoint...
     for ( gridprf = grid.profileBegin(), sfcpnt = vsfc.begin(), dest = newgrid->begin(); 
           gridprf !=grid.profileEnd() ; 
           gridprf++, sfcpnt++, dest++ ) {
        
         // note:
         //    griddata is the old dependent data, to be interpolated (length zns) 
         //    oldlevels is not used here (length nzs)
         //    z is the old independent coord value  
         //    zs is the new independent coord data (length n)
         griddata = *gridprf;
         z = *sfcpnt;

         if ( dir == 0 )  {     // if we don't know the direction yet...
            // ...find it out now
            dir = getDirection(  &vgriddata, vbad );      
         } 

         // assume that we cannot interpolate until we know we can
         value = bad;
            
         if ( dir != 0 ) {
            // do the interpolation
            value =  interp( griddata, &vgriddata, z, bad, vbad, dir ); 
         }

         // store the result in the output surface
         *dest = value;
            
         // drop the temporary variable
         delete griddata;


     }     

     return newgrid;
}


GridField3D* Vinterp::reProfile( const std::vector<real>& zs, const GridField3D& grid, const GridField3D& vgrid, int flags ) const
{
     // profile-iterator to let us sweep through the horizontal 
     // grid, grabbing a profile at each horizontal gridpoint
     GridField3D::const_profileIterator gridprf;
     // same as gridprf, but for the vertical-coordinate grid vgrid
     GridField3D::const_profileIterator vgridprf;
     // vector of vertical profile data extracted from the vgrid parameter
     const std::vector<real>* vgriddata;
     // vector of vertical profile data extracted from the grid parameter
     const std::vector<real>* griddata;
     // the output 3D grid of interpolated data
     GridField3D *newgrid;
     // bad-or-missing-data fill values for grid and vgrid, respectively
     real bad, vbad;
     // loop index for vertical levels 
     int k;
     // array to hold all the interpolated values, so they can be loaded
     // into the result all at once
     std::vector<real> data;
     // the original set of vertical coordinate values
     std::vector<real> oldlevels;
     // the length of oldlevels
     int nzs;
     // direction in which the vertical coordinate increases
     int dir;
     // temporary interpolated variable
     real value;
     // vertical coordinate value
     real z;
     // the number of new vertical levels
     int n;
     // the number of data points in the output 3D grid
     int nn;
     // the number of horizontal grid points
     int nh;
     // loop index for horizontal grid points
     int idx;
     
     // Note:
     //   this does NOT use the gridpoints() method, so it is NOT suitable
     //   for use in a trcing-only processor that does not hold the
     //   met data locally.
     
     // the two grids must be compatible in every way
     if ( ! grid.compatible(vgrid) ) {
         throw (badincompatible());
     } 

     // get both bad-flag values
     bad = grid.fillval();
     vbad = vgrid.fillval();
     
     // how many values do we interpolate to?
     n = zs.size();
     
     // get the old vertical levels
     oldlevels = grid.levels();
     nzs = oldlevels.size();


     // duplicate the grid, to copy its metadata
     newgrid = grid.duplicate();
     // set the new vertical coordinates and its strings
     newgrid->set_vertical( vgrid.quantity() );
     newgrid->set_vunits( vgrid.units() );
     newgrid->newVertical( zs );
     
     // we do not know whether the vertical quantity grows or shrinks with altitude index
     dir = 0;
     

     // the number of grid points in the output grid
     nn = newgrid->dataSize();
     
     // the number of horizontal gridpoints
     nh = nn / n;

     // we do not know whether the vertical quantity grows or shrinks with altitude
     dir = 0;
     
     // set up space to received the interpolated data
     data.resize(nn);
     
     // for each surface gridpoint...
     for ( idx = 0, gridprf = grid.profileBegin(), vgridprf = vgrid.profileBegin() ;
           gridprf !=grid.profileEnd() ; 
           idx++, gridprf++, vgridprf++ ) {
        
         // note:
         //    griddata is the old dependent data, to be interpolated (length zns) 
         //    oldlevels is not used here (length nzs)
         //    vgriddata is the old independent coord data  (length nzs)  
         //    zs is the new independent coord data (length n)
         griddata = *gridprf;
         vgriddata = *vgridprf;

         if ( dir == 0 )  {     // if we don't know the direction yet...
            // ...find it out now
            dir = getDirection(  vgriddata, vbad );      
         } 

         // for each new vertical gridpoint...
         for (k=0; k<n; k++) {
     
            // the new vertical coordinate value
            z = zs[k];
            
            // assume that we cannot interpolate until we know we can
            value = bad;
            
            if ( dir != 0 ) {
               // do the interpolation
               value =  interp( griddata, vgriddata, z, bad, vbad, dir ); 
            }

            // store the interpolated value
            data[ idx + k*nh ] = value;
            
         }
         
         // drop the temporaries
         delete griddata;
         delete vgriddata;


     }
 
     // load the interpolated values into the new grid
     newgrid->load( data );

     return newgrid;


}

GridField3D* Vinterp::reProfile( int n, const real* zs, const GridField3D& grid, const GridField3D& vgrid, int flags ) const
{
     // profile-iterator to let us sweep through the horizontal 
     // grid, grabbing a profile at each horizontal gridpoint
     GridField3D::const_profileIterator gridprf;
     // same as gridprf, but for the vertical-coordinate grid vgrid
     GridField3D::const_profileIterator vgridprf;
     // vector of vertical profile data extracted from the vgrid parameter
     const std::vector<real>* vgriddata;
     // vector of vertical profile data extracted from the grid parameter
     const std::vector<real>* griddata;
     // the output 3D grid of interpolated data
     GridField3D *newgrid;
     // loop index for vertical levels 
     int k;
     // array to hold all the interpolated values, so they can be loaded
     // into the result all at once
     std::vector<real> data;
     // the new vertical coordinate values
     std::vector<real> levels;
     // the old vertical coordinate values
     std::vector<real> oldlevels;
     // the length of "oldlevels"
     int nzs;
     // bad-or-missing-data fill values for grid and vgrid, respectively
     real bad, vbad;
     // direction in which the vertical coordinate increases
     int dir;
     // temporary variable to hold an interpolated value
     real value;
     // temporary variable to hold a new vertical coordinate value
     real z;
     // the number of data points in the output 3D grid
     int nn;
     // the number of horizontal grid points
     int nh;
     // loop index for horizontal grid points
     int idx;
     
     // Note:
     //   this does NOT use the gridpoints() method, so it is NOT suitable
     //   for use in a trcing-only processor that does not hold the
     //   met data locally.
     
     if ( ! grid.compatible(vgrid) ) {
         throw (badincompatible());
     } 

     // get both bad-flag values
     bad = grid.fillval();
     vbad = vgrid.fillval();
     
     // cast the new vertical levels into a vector
     levels.reserve(n);
     for (k=0; k<n; k++ ) {
         levels.push_back( zs[k] );
     }    

     oldlevels = grid.levels();
     nzs = oldlevels.size();

     // duplicate the grid, to copy its metadata
     newgrid = grid.duplicate();
     // set the new vertical coordinates and its strings
     newgrid->set_vertical( vgrid.quantity() );
     newgrid->set_vunits( vgrid.units() );
     newgrid->newVertical( levels );
     
     // we do not know whether the vertical quantity grows or shrinks with altitude
     dir = 0;
     
     // the number of grid points in the new grid
     nn = newgrid->dataSize();

     // get the number of horizontal gridpoints
     nh = grid.dataSize() / nzs;

     // we do not know whether the vertical quantity grows or shrinks with altitude
     dir = 0;
     
     // set up space to received the interpolated data
     data.resize(nn);
     
     // for each surface gridpoint...
     for ( idx = 0, gridprf = grid.profileBegin(), vgridprf = vgrid.profileBegin() ;
           gridprf !=grid.profileEnd() ; 
           idx++, gridprf++, vgridprf++ ) {
        
         // note:
         //    griddata is the old dependent data, to be interpolated (length zns) 
         //    oldlevels is not used here (length nzs)
         //    vgriddata is the old independent coord data  (length nzs)  
         //    zs is the new independent coord data (length n)
         griddata = *gridprf;
         vgriddata = *vgridprf;

         if ( dir == 0 )  {     // if we don't know the direction yet...
            // ...find it out now
            dir = getDirection(  vgriddata, vbad );      
         } 

         // for each vertical gridpoint...
         for (k=0; k<n; k++) {
     
            // the new vertical coordinate value
            z = zs[k];
            
            // assume that we cannot interpolate until we know we can
            value = bad;
            
            if ( dir != 0 ) {
               // do the interpolation
               value =  interp( griddata, vgriddata, z, bad, vbad, dir ); 
            }

            // store the interpolated value
            data[ idx + k*nh ] = value;
            
         }
         
         // drop the temporaries
         delete griddata;
         delete vgriddata;

     }
     

     // load the interpolated values into the new grid
     newgrid->load( data );

     return newgrid;

}

GridField3D* Vinterp::reProfile( const GridField3D& grid, const GridField3D& vgrid, int flags ) const
{
     // profile-iterator to let us sweep through the horizontal 
     // grid, grabbing a profile at each horizontal gridpoint
     GridField3D::const_profileIterator gridprf;
     // same as gridprf, but for the vertical-coordinate grid vgrid
     GridField3D::const_profileIterator vgridprf;
     // vector of vertical profile data extracted from the vgrid parameter
     const std::vector<real>* vgriddata;
     // vector of vertical profile data extracted from the grid parameter
     const std::vector<real>* griddata;
     // the output 3D grid of interpolated data
     GridField3D *newgrid;
     // loop index for vertical levels 
     int k;
     // array to hold all the interpolated values, so they can be loaded
     // into the result all at once
     std::vector<real> data;
     // vector of new vertical levels to interpolate to
     std::vector<real> levels;
     // the original set of vertical coordinate values
     std::vector<real> oldlevels;
     // the length of oldlevels
     int nzs;
     // bad-or-missing-data fill values for grid and vgrid, respectively
     real bad, vbad;
     // direction in which the vertical coordinate increases
     int dir;
     // temporary interpolated variable
     real value;
     // vertical coordinate value
     real z;
     // the number of new vertical levels
     int n;
     // the number of data points in the output 3D grid
     int nn;
     // the number of horizontal grid points
     int nh;
     // loop index for horizontal grid points
     int idx;
     // fake bad-or-missing-data fill vlaue used in vertical coordinates
     real xbad;
     
     // Note:
     //   this does NOT use the gridpoints() method, so it is NOT suitable
     //   for use in a trcing-only processor that does not hold the
     //   met data locally.
     
     
     // the two input grids must be compatible horizontally and temporally
     if ( ! grid.compatible(vgrid, METCOMPAT_HORIZ | METCOMPAT_TIME ) ) {
         throw (badincompatible());
     } 

     // get both bad-flag values
     bad = grid.fillval();
     vbad = vgrid.fillval();
     
     // get the new vertical levels
     levels = vgrid.levels();
     
     // how many values do we interpolate to?
     n = levels.size();
     
     // get the old vertical levels
     oldlevels = grid.levels();
     nzs = oldlevels.size();


     // duplicate the vgrid, to copy its coordinate structure
     newgrid = vgrid.duplicate();
     // but set the physical quantity from grid
     newgrid->set_quantity( grid.quantity() );
     newgrid->set_units( grid.units() );
     newgrid->mksScale = grid.mksScale;
     newgrid->mksOffset = grid.mksOffset;

     
     // the number of grid points in the surface
     nn = newgrid->dataSize();
     
     // the number of horizontal gridpoints
     nh = nn / n;

     // determine whether the vertical quantity grows or shrinks with altitude
     (oldlevels[1] > oldlevels[0]) ? dir=1 : dir = -1;

     // fake a bad-value for the vertical coord
     if ( dir > 0 ) {
         xbad = 2.0*oldlevels[nzs-1] - oldlevels[nzs-2];
     } else {
         xbad = 2.0*oldlevels[0] - oldlevels[1];
     }
     
     // grab the memory that we need for the results
     data.resize(nn);
     
     // for each surface gridpoint...
     for ( idx = 0, gridprf = grid.profileBegin(), vgridprf = vgrid.profileBegin() ;
           gridprf !=grid.profileEnd() ; 
           idx++, gridprf++, vgridprf++ ) {
        
         // note:
         //    griddata is the old dependent data, to be interpolated (length  nzs)
         //    oldlevels is the old independent data (length nzs)
         //    vgriddata is the new independent coord data (length n)
         //    levels is the analog for the new independent coord data (length n)         
         griddata = *gridprf;
         vgriddata = *vgridprf;
        
         // for each vertical gridpoint...
         for (k=0; k<n; k++) {
     
            // the new vertical coordinate value
            z = (*vgriddata)[k];
            
            // assume that we cannot interpolate until we know we can
            value = bad;
            
            if ( dir != 0 && z != vbad ) {
               // do the interpolation
               value =  interp( griddata, &oldlevels, z, bad, xbad, dir ); 
            }

            // store the interpolated value
            data[ idx + k*nh ] = value;
            
         }
         
         // drop the temporaries
         delete griddata;
         delete vgriddata;

     }
     

     // load the interpolated values into the new grid
     newgrid->load( data );

     return newgrid;


}


GridField3D* Vinterp::invert( const std::vector<real>& newlevels, const GridField3D& grid ) const
{
     // profile-iterator to let us sweep through the horizontal 
     // grid, grabbing a profile at each horizontal gridpoint
     GridField3D::const_profileIterator gridprf;
     // vector of vertical profile data extracted from the grid parameter
     const std::vector<real>* griddata;
     // the output 3D grid of interpolated data
     GridField3D *newgrid;
     // loop index for vertical levels 
     int k;
     // array to hold all the interpolated values, so they can be loaded
     // into the result all at once
     std::vector<real> data;
     // the new vertical coordinate values
     std::vector<real> levels;
     // the old vertical coordinate values
     std::vector<real> oldlevels;
     // the length of "oldlevels"
     int nzs;
     // bad-or-missing-data fill values for grid and vgrid, respectively
     real bad, vbad;
     // direction in which the vertical coordinate increases
     int dir;
     // temporary interpolated variable
     real value;
     // vertical coordinate value
     real z;
     // the number of new vertical levels
     int n;
     // the number of data points in the output 3D grid
     int nn;
     // the number of horizontal grid points
     int nh;
     // loop index for horizontal grid points
     int idx;
     // fake bad-or-missing-data fill vlaue used in vertical coordinates
     real xbad;
     // min and max values in a vertical column to be interpolated
     real minv, maxv;
     
     // Note:
     //   this does NOT use the gridpoints() method, so it is NOT suitable
     //   for use in a trcing-only processor that does not hold the
     //   met data locally.
     
     // get the bad-flag value
     bad = grid.fillval();
     
     // get the old vertical levels
     oldlevels = grid.levels();
     nzs = oldlevels.size();
     
     n = newlevels.size();

     xbad = grid.fillval();
     if ( xbad == 0 ) {
        // fake a bad-value for the new vertical coord
        if ( oldlevels[0] < oldlevels[1]  ) {
            xbad = 2.0*oldlevels[nzs-1] - oldlevels[nzs-2];
        } else {
            xbad = 2.0*oldlevels[0] - oldlevels[1];
        }
     }

     // duplicate this grid, to copy its coordinate structure
     newgrid = grid.duplicate();
     // set the new physical quantity from the old vertical coordinate
     newgrid->set_quantity( grid.vertical() );
     newgrid->set_units( grid.vunits() );
     newgrid->mksScale = grid.mksVScale;
     newgrid->mksOffset = grid.mksVOffset;
     // get the new vertical coordinate from the old physical quantity
     newgrid->set_vertical( grid.quantity() );
     newgrid->set_vunits( grid.units() );
     newgrid->mksVScale = grid.mksScale;
     newgrid->mksVOffset = grid.mksOffset;
     newgrid->newVertical(newlevels);
     newgrid->set_fillval( xbad );


     // the number of grid points in the surface
     nn = newgrid->dataSize();
     
     // the number of horizontal gridpoints
     nh = nn / n;

     // we do not know whether the new vertical quantity on the old grid grows or shrinks with altitude
     dir = 0;

     // grab the memory that we need
     data.resize(nn);
     
     // for each surface gridpoint...
     // for each surface gridpoint...
     for ( idx = 0, gridprf = grid.profileBegin() ;
           gridprf != grid.profileEnd() ; 
           idx++, gridprf++ ) {
        
         // note:
         //    oldlevels is the old dependent data (length nzs)
         //    griddata is the old independent data, to be interpolated (length  nzs)
         //    newlevels is the new independent coord data (length n)         
         griddata = *gridprf;
        
         if ( dir == 0 )  {     // if we don't know the direction yet...
            // ...find it out now
            dir = getDirection(  griddata, bad ); 
            
            // and while we are at it, find the
            // min and max vert coord value for this profile,
            // for which there are valid data. We will use
            // this to limit ourselves to interplation and
            // not extrapolation.
         } 
            
         // for each vertical gridpoint...
         for (k=0; k<n; k++) {
     
            // the new vertical coordinate value
            z = newlevels[k];

            // assume that we cannot interpolate until we know we can
            value = bad;
            
            if ( dir != 0 ) {
               // do the interpolation
               value =  interp( &oldlevels, griddata, z, xbad, bad, dir ); 
            }

            // store the interpolated value
            data[ idx + k*nh ] = value;
            
         }
         
         // drop the temporary
         delete griddata;

     }
     

     // load the interpolated values into the new grid
     newgrid->load( data );

     return newgrid;


}


real Vinterp::profile( const std::vector<real>& zs, const std::vector<real>& vals, real z, real bad, int flags ) const
{
     // number of vertical levels from which to interpolate
     int n;
     // loop index over vertical levels
     int k;
     // interpolated value
     real value;
     // bad-or-missing-data fill value
     real vbad;
     // indicates whether the vertical coordinate increases or decreases with index
     int dir;
     
     
     n = zs.size();

     // vertical level values must be the same length as the data values
     if ( n != vals.size() ) {
        throw (badincompatible());
     }
     // there must be enough values to do an interpolation
     if ( n < 2 ) {
        throw (badnodata());
     }

     // rig up a fake bad-value for the vertical coordinate
     if ( zs[0] < zs[1] ) {
        dir = 1;
        vbad = 2.0*zs[n-1] - zs[n-2];
     } else {
        dir = -1;
        vbad = 2.0*zs[0] - zs[1];
     }       

     // do the interpolation
     value = interp( &vals, &zs,  z, bad, vbad,  dir ); 
        

     return value;
}

