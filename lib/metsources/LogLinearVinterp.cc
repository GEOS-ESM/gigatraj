
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

#include <string>
#include <stdlib.h>
#include <sstream>

#include "gigatraj/LogLinearVinterp.hh"

using namespace gigatraj;

LogLinearVinterp::LogLinearVinterp()
{
};

LogLinearVinterp::~LogLinearVinterp()
{
};

LogLinearVinterp* LogLinearVinterp::copy() const
{
   LogLinearVinterp* result;
   
   result = new LogLinearVinterp();
   // (any local element settings would be done here)
   
   return result;

};

std::string LogLinearVinterp::id() 
{ 
  return "LogLinearVinterp";
}
  

real  LogLinearVinterp::interp( const std::vector<real>* griddata, const std::vector<real>* vgriddata, real z, real bad, real vbad, int dir, int debug ) const
{
     // index of vertical gridpoint just under the desired vertical level
     int lowerk;
     // the value of the vertical coordinate at index lowerk
     real lowerz;
     // the value of the data at vertical index lowerk
     real lowerdat;
     // index of vertical gridpoint just above the desired vertical level
     int upperk;
     // the value of the vertical coordinate at index upperk
     real upperz;
     // the value of the data at vertical index upperk
     real upperdat;
     // the number of vertical coordinates
     int nzs;
     // looping index over vertical levels
     int k;
     // interpolated result
     real value;
     
     // assume the result is bad until we succeed later
     value = bad;
     
     // how many vertical coordinate values are there?
     nzs = vgriddata->size();
     
     if ( dir > 0 ) {  // new vertical coord increases with k index
      
         // starting from the top, find the topmost index into the profile whose 
         // new vertical coordinate data point lies just under the desired surface

         lowerz = vbad;
         lowerdat = bad;
         upperz = vbad;
         upperdat = bad;
         for ( k=nzs-2; k >= 0 && (lowerz == vbad || lowerdat == bad || lowerz > z ); k-- ) {
             lowerz   = (*vgriddata)[k];
             lowerdat = (*griddata)[k];
             lowerk   = k;
         }
         // was there such a point?
         if ( lowerz != vbad && lowerdat != bad && lowerz <= z  ) {

            // find the lowest non-bad point up from the lowerk point
            for ( k=lowerk + 1; k < nzs && ( upperz == vbad || upperdat == bad || upperz == lowerz || upperz < z ); k++ ) {
                upperz   = (*vgriddata)[k];
                upperdat = (*griddata)[k];
                upperk   = k;
            }
            // was there such a point?
            if ( upperz != vbad && upperdat != bad && upperz >= z ) {
                  
                value = this->minicalc( LOG(z)
                       , LOG(lowerz), LOG(upperz)
                       , lowerdat, upperdat );                  

            }
         }

     } else {   // new vertical coord decreases with k index

         // starting from the top, find the topmost index into the profile whose 
         // new vertical coordinate data point lies just over the desired surface

         lowerz = vbad;
         lowerdat = bad;
         upperz = vbad;
         upperdat = bad;
         for ( k=nzs-2; k >= 0 && (lowerz == vbad || lowerdat == bad || lowerz < z ); k-- ) {
             lowerz   = (*vgriddata)[k];
             lowerdat = (*griddata)[k];
             lowerk   = k;
         }
         // was there such a point?
         if ( lowerz != vbad && lowerdat != bad && lowerz >= z ) {
   
            // find the lowest non-bad point down from the lowerk point
            for ( k=lowerk + 1; k < nzs && ( upperz == vbad || upperdat == bad || lowerz == upperz || upperz > z ); k++ ) {
                upperz   = (*vgriddata)[k];
                upperdat = (*griddata)[k];
                upperk   = k;
            }
            // was there such a point?
            if ( upperz != vbad && upperdat != bad && upperz <= z ) {
                  
                value = this->minicalc( LOG(z)
                        , LOG(lowerz), LOG(upperz)
                        , lowerdat, upperdat );
                  
            }

         }
           
     } 
          
     if ( debug ) {
        std::cerr << "lowerk,z=" << lowerk << ", " << lowerz << ", dat=" << lowerdat << std::endl;
        std::cerr << "upperk,z=" << upperk << ", " << upperz << ", dat=" << upperdat << std::endl;
        std::cerr << "bad=" << bad << ", vbad=" << vbad << std::endl;
        std::cerr << "dir=" << dir << ", z=" << z << ", value=" << value << std::endl; 
     }           

     return value;
}




real LogLinearVinterp::calc( real z, const GridField3D& grid, int i, int j, int flags ) const
{

     // indices of vertical gridpoints that bracket the desired z value
     int k1, k2;
     // the interpolated value
     real result;
     // bad-or-missing-data fill value
     real bad;
     // data values that correspond to indices k1,k2
     real vals[2];
     // 3D data grid indices [ (i,j,k) for (x,y,z)] of the two bracketing points 
     int is[2];
     int js[2];
     int ks[2];
   
     /// \todo   take bad-data flags into account, according to flags setting

     // get the bad-data fill value
     bad = grid.fillval();

     // get the two indices in the vertical direction that that bracket
     // the desired value z
     grid.zindex(z, &k1, &k2);
   
     // assemble the grid indices of the two bracketing points
     is[0] = i;
     js[0] = j;
     ks[0] = k1;
     is[1] = i;
     js[1] = j;
     ks[1] = k2;
     // fetch the data values at these indices.
     // (Note that this may invbolve parallel processing, but
     // that is handled entriely internally to the grid object .)
     grid.gridpoints( 2, is, js, ks, vals, do_local(flags) );

     // both data values have to be good
     if ( vals[0] != bad && vals[1] != bad  ) {

        // do the linear interpolation
        result = minicalc( LOG(z)
                         , LOG(grid.level(k1)), LOG(grid.level(k2))
                         , vals[0], vals[1] );     

     } else {
        result = bad;
     }
     
     return result;         
     
}

std::vector<real>* LogLinearVinterp::calc( const std::vector<real>&  zs, const GridField3D& grid, int i, int j, int flags ) const
{
     // indices of vertical gridpoints that bracket the desired z value
     int k1, k2;
     // the interpolated values
     std::vector<real> *results;
     // bad-or-missing-data fill value
     real bad;
     // data values that correspond to gridpoints that we need
     real *vals;
     // 3D data grid indices [ (i,j,k) for (x,y,z)] of the gridpoints that we need
     int *is;
     int *js;
     int *ks;
     // loop index for the zs levels being interpolated to
     int idx;
     // the number of levels to interpolate to
     int n;

     // how many points?
     n = zs.size();

     // create a vector to hold the results
     results = new std::vector<real>;
     results->reserve(n);
     

     // get the bad-data fill value
     bad = grid.fillval();
     
     // create an array to hold the gridpoint values
     vals = new real[2*n];
     // create arrays to hold gridpoint indices
     is = new int[2*n];
     js = new int[2*n];
     ks = new int[2*n];
     
     
     // for each desired z value...
     for ( idx=0; idx<n; idx++ ) {
     
         // get the vertical gridpooints that bracket the current z value
         grid.zindex(zs[idx], &k1, &k2);
     
         // load the gridppoint indices for these two points into their arrays
         is[idx*2+0] = i;
         js[idx*2+0] = j;
         ks[idx*2+0] = k1;
         is[idx*2+1] = i;
         js[idx*2+1] = j;
         ks[idx*2+1] = k2;
     }
     
     // now that we have all the gridpoint indices, request their data values
     // from the GridField3D object.  Note that there may involve some
     // parallel processing, but such details are hidden inside
     // the grid object.
     grid.gridpoints( 2*n, is, js, ks, vals, do_local(flags) );
     
     // now that we have the gridpoint data values,
     // loop over each desired z value
     for ( idx=0; idx<n; idx++ ) {

        // both data values for the gridpoints that bracket
        // the desired value, must be good
        if ( vals[idx*2+0] != bad && vals[idx*2+1] != bad  ) {

             // do the linear interpolation and stroe the result
             results->push_back( this->minicalc( LOG(zs[idx])
                         , LOG(grid.level(ks[idx*2+0])), LOG(grid.level(ks[idx*2+1]))
                         , vals[idx*2+0], vals[idx*2+1] ) );

        } else {
             results->push_back( bad );
        }
        
     }   
     
     // drop the temporary variables
     delete[] ks;
     delete[] js;
     delete[] is;
     delete[] vals;
     
     return results;         
     
}

real* LogLinearVinterp::calc( int n, const real* zs, const GridField3D& grid, int i, int j, int flags ) const
{
     // indices of vertical gridpoints that bracket the desired z value
     int k1, k2;
     // the interpolated values
     real *results;
     // bad-or-missing-data fill value
     real bad;
     // data values that correspond to gridpoints that we need
     real *vals;
     // 3D data grid indices [ (i,j,k) for (x,y,z)] of the gridpoints that we need
     int *is;
     int *js;
     int *ks;
     // loop index for the zs levels being interpolated to
     int idx;

     // get the bad-data fill value
     bad = grid.fillval();
     
     // create an array to hold the results
     results = new real[2*n];
     // create an array to hold the gridpoint values
     vals = new real[2*n];
     // create arrays to hold gridpoint indices
     is = new int[2*n];
     js = new int[2*j];
     ks = new int[2*n];
     
     // for each desired z value...
     for ( idx=0; idx<n; idx++ ) {
     
         // get the vertical gridpooints that bracket the current z value
         grid.zindex(zs[idx], &k1, &k2);
     
         // load the gridppoint indices for these two points into their arrays
         is[idx*2+0] = i;
         js[idx*2+0] = j;
         ks[idx*2+0] = k1;
         is[idx*2+1] = i;
         js[idx*2+1] = j;
         ks[idx*2+1] = k2;
     }

     // now that we have all the gridpoint indices, request their data values
     // from the GridField3D object.  Note that there may involve some
     // parallel processing, but such details are hidden inside
     // the grid object.
     grid.gridpoints( 2*n, is, js, ks, vals, do_local(flags) );
     
     // now that we have the gridpoint data values,
     // loop over each desired z value
     for ( idx=0; idx<n; idx++ ) {

        // both data values for the gridpoints that bracket
        // the desired value, must be good
        if ( vals[idx*2+0] != bad && vals[idx*2+1] != bad  ) {

             // do the linear interpolation and stroe the result
             results[idx] = this->minicalc( LOG(zs[idx])
                         , LOG(grid.level(ks[idx*2+0])), LOG(grid.level(ks[idx*2+1]))
                         , vals[idx*2+0], vals[idx*2+1] );
        } else {
             results[idx] = bad;
        }
        
     }   
     
     
     // drop the temporary variables
     delete[] ks;
     delete[] js;
     delete[] is;
     delete[] vals;
     
     return results;         
     
}


GridFieldSfc* LogLinearVinterp::surface( const real z, const GridField3D& grid, int flags ) const
{
     // profile-iterator to let us sweep through the horizontal 
     // grid, grabbing a profile at each horizontal gridpoint
     GridField3D::const_profileIterator gridprf;
     // gridpoint interator for the output surface
     GridFieldSfc::iterator dest;
     // the surface to be output with the interpolated values
     GridFieldSfc *sfcgrid;
     // indices of vertical gridpoints that bracket the desired z value
     int k1, k2;
     // horizontal gridpoint indices
     int i,j;
     // bad-or-missing-data fill value
     real bad;
     // data values that correspond to gridpoints that we need
     real *vals;
     // 3D data grid indices [ (i,j,k) for (x,y,z)] of the gridpoints that we need
     int *is;
     int *js;
     int *ks;
     // loop index for the zs levels being interpolated to
     int idx;
     // the number of horizontal gridpoints
     int n;
     // temporary interpolated variable
     real val;
     // used to assemble the parts of the surface value label
     std::ostringstream sfcval;

     // quick way of making a surface from the 3D grid
     // This copies over the horizontal grid specification (dimensions)
     sfcgrid = grid.extractSurface(0);

     // set the surface value
     sfcval << grid.vertical() << ": " << z << " " << grid.vunits();
     sfcgrid->set_surface(sfcval.str());


     // get the bad-data fill value
     bad = grid.fillval();
     
     // the number of grid points in the surface
     n = sfcgrid->dataSize();

     // allocate space for the gridpoint values we are about to receive
     vals = new real[2*n];
     // create arrays to hold gridpoint indices
     is = new int[2*n];
     js = new int[2*n];
     ks = new int[2*n];

     // find the vertical level that brackets the desired level     
     grid.zindex( z, &k1, &k2);
     
     // loop over each surface gridpoint, filling the index arrays
     ;
     for ( idx=0,gridprf = grid.profileBegin() ; 
           idx<n; 
           idx++, gridprf++) {
           
         // get the vertical gridpooints that bracket the current z value
         // at this profile
         gridprf.indices( &i, &j );
     
         // load the gridppoint indices for these two points into their arrays
         is[idx*2+0] = i;
         js[idx*2+0] = j;
         ks[idx*2+0] = k1;
         is[idx*2+1] = i;
         js[idx*2+1] = j;
         ks[idx*2+1] = k2;
         
     }

     // now use these grid indices to grab data values
     // (which might be on a different processor, but we
     // will let gridpoints() deal with that)
     grid.gridpoints( 2*n, is, js, ks, vals, do_local(flags) );
     
     // loop over each surface gridpoint again,
     // using the retrieved data values (2 per gridpoint)
     // to do the interpolation
     dest=sfcgrid->begin();
     for ( idx=0; idx<n; idx++ ) {

         // both data values for this interpolated level must be good   
         if ( vals[idx*2+0] != bad && vals[idx*2+1] != bad  ) {
             // do the linear interpolation between them
             val = this->minicalc( LOG(z)
                         , LOG(grid.level(ks[idx*2+0])), LOG(grid.level(ks[idx*2+1]))
                         , vals[idx*2+0], vals[idx*2+1] );
        } else {
             val = bad;
        }
        
        // store the interpolated value in the surface grid
        *dest = val;

        // advance to the next gridpoint
        dest++;
     }   
     
     // drop the temporary variables
     delete[] ks;
     delete[] js;
     delete[] is;
     delete[] vals;
     
     
     return sfcgrid;
     
}

