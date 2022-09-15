
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/MPVOTF.hh"

using namespace gigatraj;


MPVOTF::MPVOTF() 
{
   
   quant = "modified_potential_vorticity";
   uu = "K m^2/kg/s";
   ename = "ertel_potential_vorticity";
   hname = "air_potential_temperature";

}
MPVOTF::MPVOTF( const std::string& epv, const std::string& theta )
{
   
   quant = "modified potential vorticity";
   uu = "K m^2/kg/s";
   ename = epv;
   hname = theta;

}

MPVOTF::~MPVOTF()
{
}

GridField3D* MPVOTF::calc( const GridField3D& epv, const GridField3D& theta, int flags) const
{

   // the output MPV field
   GridField3D *result;
   // iterator over the first input's gridpoints
   GridField3D::const_iterator inPnt1;
   // iterator over the second input's gridpoints
   GridField3D::const_iterator inPnt2;
   // iterator over output's gridpoints
   GridField3D::iterator outPnt;
   // temporary variables for input quantities
   real ee, hh;
   // bad-or-missing-data fill values
   real epvbad, thetabad;
   
   // the two input fields must be grid-compatible
   if ( ! epv.compatible(theta) ) {
      throw (badgrid());
   }
   // and they must be the right quantities
   if ( epv.quantity() != ename || theta.quantity() != hname ) {
      throw (badinputquant());
   }

   // create the output by copying the input grid 
   result = epv.duplicate();                    
   // and change its metadata                     
   result->set_quantity(quant);               
   // note: keep same units as epv field
   
   epvbad = epv.fillval();
   thetabad = theta.fillval();
   
   // iterate over all gridpoints                                                 
   for ( inPnt1 = epv.begin(), inPnt2 = theta.begin(), outPnt = result->begin() ; 
         inPnt1 != epv.end();                                                    
         inPnt1++, inPnt2++, outPnt++ ) {                                         
                                                                                  
         // grab the two input values                                             
         ee = *inPnt1;                                                          
         hh = *inPnt2;                                                          
                                                                                  
         if ( ee != epvbad && hh != thetabad && hh > 0.0 ) { 
            // transform to MKS units                                                      
            ee = ee * epv.mksScale + epv.mksOffset;                      
            hh = hh * theta.mksScale + theta.mksOffset;                      
            // calculate MPV
            ee = ee * POW( (hh/420.0), (-9./2.) );
            // scale it and store it in the profile
            *outPnt = ( ee - result->mksOffset )/result->mksScale;  
         } else {
            *outPnt = epvbad;
         }   

   }                                                                              
   
   if ( flags & OTF_MKS ) {
      result->transform("K m^2/kg/s");
   }                                                                            
                                                                                  
   return result;                                                                

}

GridField3D* MPVOTF::calc( const GridField3D& epv, int flags ) const
{
   // the output MPV field
   GridField3D *result;
   // the set of vertical coordinates
   std::vector<real> theta;
   // a vector of vertical profile data extracted from the input grid
   std::vector<real> *profile;
   // iterator over input gridpoint profiles  
   GridField3D::const_profileIterator inProf; 
   // iterator over output gridpoint profiles 
   GridField3D::profileIterator outProf;      
   // temporary variables for input quantities
   real ee, hh;
   // bad-or-missing-data fill value
   real epvbad;
   
   // the input physical quantity and its vertical coordinate quantity
   // must be correct. 
   if ( epv.quantity() != ename || epv.vertical() != hname ) {
      throw (badinputquant());
   }
   

   // create the output by copying the input grid 
   result = epv.duplicate();                    
   // and change its metadata                     
   result->set_quantity(quant);               
   // note: keep same units as epv field

   // get the vertical coordinates and                       
   // transform them to MKS units.                           
   theta = epv.levels();                                     
   for ( int k=0; k<theta.size(); k++ ) {                    
       theta[k] = theta[k] * epv.mksVScale + epv.mksVOffset; 
   }                                                         
   
   
   epvbad = epv.fillval();
   
   // iterate over each profile in the horizontal grid                                       
   for ( inProf = epv.profileBegin(), outProf = result->profileBegin();                    
         inProf != epv.profileEnd();                                                       
         inProf++, outProf++ ) {                                                             
                                                                                             
         // get the data profile                                                             
         profile = *inProf;                                                                  
                                                                                             
         // calculate the density values for this profile                                    
         for ( int k=0; k<profile->size(); k++ ) {                                           

              hh = theta[k]; 
              ee = (*profile)[k];                                                               
                                                                                             
              if ( ee != epvbad && hh > 0.0 ) {                
                  // transform to MKS units                                                      
                  ee = (*profile)[k] * epv.mksScale + epv.mksOffset;                      
                  // calculate MPV
                  ee = ee * POW( (hh/420.0), (-9./2.) );
                  // scale it and store it in the profile
                  (*profile)[k] = ( ee - result->mksOffset )/result->mksScale;  
              } else {                                                      
                 (*profile)[k] = epvbad;
              }                                    
         }                                                                                   

         // store the density into the output grid                                           
         outProf.assign(*profile);                                                           
                                                                                             
         delete profile;                                                                     

   }                                                                                         
   
   if ( flags & OTF_MKS ) {
      result->transform("K m^2/kg/s");
   }                                                                            
                                                                                  
   
   return result;


}


GridFieldSfc* MPVOTF::calc( const GridFieldSfc& epv, const GridFieldSfc& theta, int flags) const
{

   // the output MPV field
   GridFieldSfc *result;
   // temporary variables for input quantities
   real hh, ee;
   // bad-or-missing-data fill values
   real epvbad, thetabad;
   // iterator over the first input's gridpoints
   GridFieldSfc::const_iterator inPnt1;
   // iterator over the second input's gridpoints
   GridFieldSfc::const_iterator inPnt2;
   // iterator over output's gridpoints
   GridFieldSfc::iterator outPnt;
   
   // the two input fields must be grid-compatible
   if ( ! epv.compatible(theta) ) {
      throw (badgrid());
   }
   // and they must be the right quantities
   if ( epv.quantity() != ename || theta.quantity() != hname ) {
      throw (badinputquant());
   }
   
   // create the output by copying the input grid 
   result = epv.duplicate();                    
   // and change its metadata                     
   result->set_quantity(quant);               
   // note: keep same units as epv field
   
   epvbad = epv.fillval();
   thetabad = theta.fillval();

   // iterate over all gridpoints
   for ( inPnt1 = epv.begin(), inPnt2 = theta.begin(), outPnt = result->begin() ;
         inPnt1 != epv.end();
         inPnt1++, inPnt2++, outPnt++ ) 
   {
         
         // grab the two input values
         ee = *inPnt1;
         hh = *inPnt2;
       
         if ( ee != epvbad && hh != thetabad && hh > 0.0  ) {
            // transform to MKS units                                                      
            ee = ee * epv.mksScale + epv.mksOffset;                      
            hh = hh * theta.mksScale + theta.mksOffset;                      
            // calculate MPV
            ee = ee * POW( (hh/420.0), (-9./2.) );
            // scale it and store it in the profile
            *outPnt = ( ee - result->mksOffset )/result->mksScale;  
         } else {
            *outPnt = epvbad;
         }   

   }      
                                                                                  
   if ( flags & OTF_MKS ) {
      result->transform("K m^2/kg/s");
   }                                                                            
   
   return result;   
   
}

