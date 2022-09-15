
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/ThetaOTF.hh"


using namespace gigatraj;

ThetaOTF::ThetaOTF() 
{
   
   quant = "air_potential_temperature";
   uu = "Kelvin";
   pname = "air_pressure";
   tname = "air_temperature";

}

ThetaOTF::ThetaOTF( const std::string& theta, const std::string& temperature, const std::string& pressure)
{
   quant = theta;
   uu = "Kelvin";
   pname = pressure;
   tname = temperature;
}

ThetaOTF::~ThetaOTF()
{
}

real ThetaOTF::calc( real t, real p) const
{
   real val;

   // sanity-check the input values   
   if ( t < 100.0 || t > 1000.0 || p <= 0.0 || p > 150000.0 ) {
      throw (badinputquant());
   }

   val = dothet(t,p);
   
   return val;
}

GridField3D* ThetaOTF::calc( const GridField3D& t, const GridField3D& p, int flags) const
{
   // scale and offset to convert temperature (whatever units are being used) to (MKS) Kelvin
   real t_scale;
   real t_offset;
   // scale and offset to convert pressure to Pascals
   real p_scale;
   real p_offset;
   // iterator over input gridpoints
   GridField3D::const_iterator tPnt;
   // iterator over input gridpoints
   GridField3D::const_iterator pPnt;
    // iterator over output gridpoints
   GridField3D::iterator destPnt;
    // the output potential temperature field
   GridField3D *result;
   // temporary variables for intermediate results
   real tt, pp;
    // bad-or-missing-data fill values for holding inputs
   real tbad, pbad;
   
   // the input fields must be grid-compatible
   if ( ! t.compatible(p) ) {
      throw (badgrid());
   }

   // the input fields must be the right physical quantities
   if ( t.quantity() != tname || p.quantity() != pname ) {
      throw (badinputquant());
   }
   
   
   // get the conversion factors from the inputs
   t_scale = t.mksScale;
   t_offset = t.mksOffset;
   p_scale = p.mksScale;
   p_offset = p.mksOffset;
   
   // use the first input quantity as a base for the result
   result = t.duplicate();
   // change the quantity and units to potential temperature
   result->set_quantity(quant);
   result->set_units(uu);
   
   tbad = t.fillval();
   pbad = p.fillval();
   
   /* loop over every point in the two input objects, and set the value
      of the corresponding point in the output object.
      Note that all three grids have the same dimensions (as per the compatibility
      check above), so we need only the one end-check for the loop.
   */   
   for ( tPnt  = t.begin(), pPnt  = p.begin(), destPnt  = result->begin();
         tPnt != t.end();
         tPnt++, pPnt++, destPnt++ ) {
         
         
         tt = *tPnt;
         pp = *pPnt;
         if ( tt != tbad && pp != pbad && pp > 0.0 ) {
            tt = tt*t_scale + t_offset;
            pp = pp*p_scale + p_offset;
         
            *destPnt = dothet( tt, pp );
         } else {
            *destPnt = tbad;
         }
   }
   
   return result;

}


GridField3D* ThetaOTF::calc( const GridField3D& t, int flags ) const
{
   // scale and offset to convert temperature (whatever units are being used) to (MKS) Kelvin
   real t_scale;
   real t_offset;
   // scale and offset to convert pressure to Pascals
   real p_scale;
   real p_offset;
   // the output potential temperature field
   GridField3D *result;
   // the vertical coordinate values (pressure)
   std::vector<real> p;
   // temporary variables for intermediate results
   real tt, pp;
   // bad-or-missing-data fill values for holding inputs
   real tbad, pbad;
   // iterator over input gridpoints
   GridField3D::const_iterator tPnt;
    // iterator over output gridpoints
   GridField3D::iterator destPnt;
   
   // the input field must be temperature on pressure surfaces
   if ( t.quantity() != tname || t.vertical() != pname ) {
      throw (badinputquant());
   }
   

   // convert temperature (whatever units are being used) to (MKS) Kelvin
   t_scale = t.mksScale;
   t_offset = t.mksOffset;
   // convert pressure to millibars
   p_scale = t.mksVScale;
   p_offset = t.mksVOffset;


   // use the input quantity as a base for the result
   result = t.duplicate();
   // change the quantity and units to potential temperature
   result->set_quantity(quant);
   result->set_units(uu);
   
   tbad = t.fillval();
   
   p = t.levels();

   /* loop over every point in the two input objects, and set the value
      of the corresponding point in the output object.
      Note that both grids have the same dimensions, so we need 
      only the one end-check for the loop.
   */
   
   for ( int k=0; k < p.size(); k++ ) {

       pp = p[k]*p_scale + p_offset;

       for ( tPnt  = t.begin(k), destPnt  = result->begin(k);
           tPnt != t.end(k);
           tPnt++, destPnt++ ) {
         
         
           tt = *tPnt;
           if ( tt != tbad ) {
           
              tt = tt*t_scale + t_offset;
         
              *destPnt = dothet( tt, pp );
           } else {
              *destPnt = tbad;
           }

       }

   }

   
   return result;


}


GridFieldSfc* ThetaOTF::calc( const GridFieldSfc& t, const GridFieldSfc& p, int flags) const
{
   // scale and offset to convert temperature (whatever units are being used) to (MKS) Kelvin
   real t_scale;
   real t_offset;
   // scale and offset to convert pressure to Pascals
   real p_scale;
   real p_offset;
    // the output potential temperature field
   GridFieldSfc *result;
   // temporary variables for intermediate results
   real tt, pp;
    // bad-or-missing-data fill values for holding inputs
   real tbad, pbad;
    // iterator over input gridpoints
   GridFieldSfc::const_iterator tPnt;
    // iterator over input gridpoints
   GridFieldSfc::const_iterator pPnt;
    // iterator over output gridpoints
   GridFieldSfc::iterator destPnt;
   
    // the two input fields must be grid-compatible
   if ( ! t.compatible(p) ) {
      throw (badgrid());
   }

   // and they must be the right physical quantities
   if ( t.quantity() != tname || p.quantity() != pname ) {
      throw (badinputquant());
   }
   
   
   // convert temperature (whatever units are being used) to (MKS) Kelvin
   t_scale = t.mksScale;
   t_offset = t.mksOffset;
   // convert pressure to millibars
   p_scale = p.mksScale;
   p_offset = p.mksOffset;
   

   // use the first input quantity as a base for the result
   result = t.duplicate();
   // change the quantity and units to potential temperature
   result->set_quantity(quant);
   result->set_units(uu);
   
   tbad = t.fillval();
   pbad = p.fillval();

   // loop over every gridpoint
   for ( tPnt  = t.begin(), pPnt  = p.begin(), destPnt  = result->begin();
         tPnt != t.end();
         tPnt++, pPnt++, destPnt++ ) {
         
         tt = *tPnt;
         pp = *pPnt;
         if ( tt != tbad && pp != pbad && pp > 0.0 ) {

            tt = tt*t_scale + t_offset;
            pp = pp*p_scale + p_offset;
         
            *destPnt = dothet( tt, pp );
         } else {
            *destPnt = tbad;
         }

   }

   
   return result;

}

