
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/MetGridData.hh"

using namespace gigatraj;

#include "gigatraj/TropOTF.hh"

TropOTF::TropOTF() 
{
   sfc = "tropopause";
   quant = "tropopause_air_pressure";
   uu = "mb";
   pname = "air_pressure";
   tname = "air_temperature";
   aname = "altitude";
   hname = "air_potential_temperature";
   dname = "air_density";

}
TropOTF::TropOTF(std::string trop, std::string temperature, std::string pressure, std::string altitude, std::string theta, std::string density )
{
   // the quantity and units are ignored in the calculation methods.
   quant = "tropopause_air_pressure";
   uu = "mb";
   
   sfc = trop;
   pname = pressure;
   tname = temperature;
   aname = altitude;
   hname = theta;
   dname = density;
}

TropOTF::~TropOTF()
{
}


real TropOTF::wmo( const std::vector<real>&t, const std::vector<real>&alt, int flags ) const
{
     // the length of the input vectors
     unsigned len;
     // the profile of calculated lapse rates
     std::vector<real> lapse_rate;
     // the profile of altitudes between the given altitudes 
     // (i.e, centers of the lapse rates)
     std::vector<real> layer_alt;
     // the altitude of the tropopause
     real trop_alt;
     // the index in the profile that marks the tropopause
     unsigned pick;
     // whether the element was found
     bool picked;
     // debugging
     int debug = 0;

     len = t.size();
     if ( len != alt.size() ) {
        std::cerr << "TropOTF::wmo: size mismatch: " << len << " vs " << alt.size() << std::endl; 
        throw (badprofile());
     }   

     // calculate the lapse rates along the profile     
     for ( unsigned i=0; i<(len-1); i++ ) {
         if ( alt[i] == alt[i+1] ) {
             std::cerr << "TropOTF::wmo: duplicate vertical level at " << i << " and " << i + 1 << " : " << alt[i] << std::endl; 
             throw (badprofile());
         }    
         // (note the minus sign)
         lapse_rate.push_back( - (t[i+1]-t[i])/(alt[i+1]-alt[i]) );
         layer_alt.push_back( (alt[i]+alt[i+1])/2.0 );
     }
     
     // now find the lowest-in-altitude lapse rate that lies below 0.002 K/m
     // with no level within 2 km above it having a lapse rate greater 
     // than 0.002 K/m.
     picked = false;
     for ( unsigned i=0; 
           (i < lapse_rate.size()) && ( ! picked ) ; 
           i++ ) {
         
         if ( lapse_rate[i] < 0.002 ) {
            // tentatively assume that this is a right level
            pick = i;
            picked = true;
            for ( unsigned j=i+1; 
                 (j < lapse_rate.size()) && ((layer_alt[j]-layer_alt[i]) < 2000.0) ; 
                 j++ ) {
                if ( lapse_rate[j] > 0.002 ) {
                   // within 2 km but lapse rate > 0.002
                   // reset
                   picked = false;
                }   
            }
         }
     }
     if ( debug ) {
        std::cerr << "debug message here" << std::endl;
     }
     
     if ( picked && (pick > 0 || layer_alt[pick] > 17210.4 ) ) {
        // found a point.
        // find the altitude of the trop
        trop_alt = ( 0.002 - lapse_rate[pick-1] )
                  / ( lapse_rate[pick] - lapse_rate[pick-1]  )
                  * ( layer_alt[pick]-layer_alt[pick-1] ) + layer_alt[pick-1];
        if ( debug ) {
           std::cerr << "debug message here" << std::endl;
        }
     } else {
        trop_alt = 17210.4;
     }
     
     // note: we ignore any OTF_MKS flag at this point.
       
     return trop_alt;
}


GridFieldSfc* TropOTF::wmo( const GridField3D& t, int flags ) const
{
    // the output tropopause field
    GridFieldSfc* result;
    // iterator over the output grid points
    GridFieldSfc::iterator pnt;
    // iterator over the input grid vertical profiles
    GridField3D::const_profileIterator prof;
    // vector of vertical coordinates from the input grid
    std::vector<real> rawalts;
    // length of the input vertical coordinates
    int n;
    // vertical profile of temperatures, as extracted from the input grid, 
    // potentially with bad points.
    std::vector<real>* tp;
    // vertical profile of temperature, w/o bad points
    std::vector<real> dat;
    // vertical profile of altitude values, corresponding to dat
    std::vector<real> alts;
    // temporary variable
    real value;
    // temperature at a given vertical level
    real tval;
    // bad-or-missing fill value
    real tbad;
    // indicates what kind of vertical coordinates we are using
    int vcoord;
    // debugging messages flag
    int debug = 0;
    // MKS vertical units
    std::string vu;


    // the input quantity must be temperature
    if ( t.quantity() != tname ) {
       throw (badprofile());
    }   

    // Extract a surface from the input field.
    // This ensures that the output has the same horizontal grid as the input.
    result = t.extractSurface(0);
    // Set the metadata.
    // We return the tropopause in terms of the vertical coordinate of t, in the same units
    result->set_quantity(t.vertical());
    result->set_units(t.vunits());
    result->mksScale=t.mksVScale;
    result->mksOffset=t.mksVOffset;
    result->set_surface( sfc );

    // we can calculate the trop only for these vertical coords
    if ( t.vertical() == aname ) {
       // altitude
       vcoord = 0;
    } else if ( t.vertical() == pname ) {
       // pressure
       vcoord = 1;
    } else if ( t.vertical() == hname ) {
       // potential temperature
       vcoord = 2;
    } else if ( t.vertical() == dname ) {
       // density
       vcoord = 3;
    } else {
        throw (badprofile());
    } 
    
    
    tbad = t.fillval(); 

    // get the vertical coordinate, and scale it to SI units
    rawalts = t.levels();
    // note its length as well
    n = rawalts.size();
    // convert to SI units
    for ( int i=0; i<n; i++ ) {
        rawalts[i] = rawalts[i]*t.mksVScale + t.mksVOffset;
    }    

    // Do this here, to save a little time
    // Otherwise, we will be repeating this same calculation 
    // in every iteration of the profile loop below.
    if ( vcoord == 1 ) {   
       // convert pressures to log-P altitudes  
       for ( int i=0; i<n; i++ ) {
           rawalts[i] = 7000.0*LOG(100000.0/rawalts[i]);
       }    
    }

    // for each horizontal gridpoint...
    for ( prof  = t.profileBegin(), pnt = result->begin(); 
          prof != t.profileEnd(); 
          prof++, pnt++ ) {

        // reset our altitude and temperature variables
        alts.clear();
        alts.reserve(n);
        dat.clear();
        dat.reserve(n);
         
        // extract the temperature profile
        tp = *prof;
        // assemble vectors of T and alt with no bad points
        for ( int i=0; i<n; i++ ) {
            if ( (*tp)[i] != tbad ) {
              
               tval = ((*tp)[i]*t.mksScale+t.mksOffset); 
               dat.push_back( tval );
               
               /* note: in some of these cases, we generate a fake
                  log-pressure altitude.  This is OK, since
                  we will be using it to convert back to the
                  original vertical coordinate once we have the
                  tropopause.
               */   
               switch (vcoord) {
               case 0: // altitude
                  value = rawalts[i]; 
                  break;
               case 1: // pressure   
                  value = rawalts[i];  // (see note above)
                  break;
               case 2: // theta   
                  // convert theta to fake altitude, given temperature
                  //  given z=H*ln(p0/p), and theta = t*(p0/p)^kappa,
                  //  we want: z = -1/kappa * H * ln( theta/t )
                  value = (7./2.)*7000.0 * LOG( rawalts[i]/tval );
                  break;
               case 3: // density   
                  // convert density to fake altitude, given temperature
                  //  given z=H*ln(p0/p), and p = rho*R*t,
                  //  we want: z = H * ln( p0/(rho*r*t) )
                  value = 7000.0*LOG(100000.0/(rawalts[i]*287.04*tval) );
                  break;
               } 
               
               alts.push_back( value ); 
               
            }    
        }

        // find the altitude of the tropopuase
        value = wmo( dat, alts, debug ); 

        // convert this trop alt back to the original vertical coordinates
        // but note that altitude is in m right now, so we may
        // want to convert it
        switch (vcoord) {
        case 0: // altitude
           // no physical quantity conversion
           break;
        case 1: // pressure   
           // convert back to pressure
           value = 100000.0*EXP( - value/7000.0 );
           break;
        case 2: // theta   
           // "value" is in altitude.  find the temperature at this altitude
           tval = intrp( value, alts, dat );
           // use the temp to convert altitude back to theta
           value = tval * EXP( (2./7.)*value/7000.0 );
           break;
        case 3: // density   
           // "value" is in altitude.  find the temperature at this altitude
           tval = intrp( value, alts, dat );
           // convert altitude to density
           value = 100000.0*EXP( - value/7000.0 )/287.04/tval;
           break;        
        }

        
        // convert back to caller's units, before storing it
        *pnt =  ( value - result->mksOffset )/result->mksScale;

        
        delete tp;

    }
    

    if ( flags & OTF_MKS ) {
       switch (vcoord) {
       case 0: // altitude
          vu = "m";
          break;
       case 1: // pressure   
          vu = "Pa";
          break;
       case 2: // theta   
          vu = "K";
          break;
       case 3: // density   
          // "value" is in altitude.  find the temperature at this altitude
          vu = "kg/m^3";
          break;        
       }
    
       result->transform( vu );
    }    

    return result;
}




GridFieldSfc* TropOTF::wmo( const GridField3D& t, const GridField3D& alt, int flags) const
{
    // the output tropopause field
    GridFieldSfc* result;
    // iterator over the output grid points
    GridFieldSfc::iterator pnt;
    // iterators over the input grid vertical profiles
    GridField3D::const_profileIterator prof, aprof;
    // vector of vertical coordinates from the input grid
    std::vector<real> rawalts;
    // length of the input vertical coordinates
    int n;
    // vertical profile of temperatures, as extracted from the first input grid, 
    // potentially with bad points.
    std::vector<real>* tp;
    // vertical profile of altitudes, as extracted from the second input grid, 
    // potentially with bad points.
    std::vector<real>* ap;
    // vertical profile of temperature, w/o bad points
    std::vector<real> dat;
    // vertical profile of altitude-like quantity, w/o bad points
    std::vector<real> alts;
    // temporary variable
    real value;
    // temperature at a given vertical level
    real tval;
    // altitude-like value at a given vertical level
    real aval;
    // bad-or-missing fill value
    real tbad;
    // bad-or-missing fill value
    real abad;
    // indicates what kind of vertical coordinates we are using
    int vcoord;
    // debugging messages flag
    int debug = 0;
    // MKS vertical units
    std::string vu;

    if ( flags & OTF_DEBUG ) {
       debug = 1;
    }

    // the input quantities must be correct
    if ( t.quantity() != tname || alt.quantity() != aname ) {
       throw (badprofile());
    }  
    // and the two inout grids must be compatible
    if ( ! t.compatible(alt) ) {
       throw (badprofile());
    }  
    
    // we can calculate the trop only for these vertical coords
    if ( alt.quantity() == aname ) {
       // altitude
       vcoord = 0;
    } else if ( alt.quantity() == pname ) {
       // pressure
       vcoord = 1;
    } else if ( alt.quantity() == hname ) {
       // potential temperature
       vcoord = 2;
    } else if ( alt.quantity() == dname ) {
       // density
       vcoord = 3;
    } else {
        throw (badprofile());
    } 

    // Extract a surface from the input field.
    // This ensures that the output has the same horizontal grid as the input.
    result = t.extractSurface(0);
    // We return the tropopause in terms of the alt variable, in the same units
    result->set_quantity(alt.quantity());
    result->set_units(alt.units());
    result->mksScale=alt.mksScale;
    result->mksOffset=alt.mksOffset;
    result->set_surface( sfc );

    tbad = t.fillval(); 
    abad = alt.fillval();

    // for each horizontal gridpoint...
    for (prof=t.profileBegin(), aprof=alt.profileBegin(), pnt=result->begin(); 
         prof!=t.profileEnd(); 
         prof++, aprof++, pnt++ ) {

        // extract the temperature and altitude profiles
        tp = *prof;
        ap = *aprof;
        n = (*ap).size();

        // reset our altitude and temperature variables
        alts.clear();
        alts.reserve(n);
        dat.clear();
        dat.reserve(n);
         
        // convert t to SI units, assembling vectors of t and alt with no bad points
        for ( int i=0; i<n; i++ ) {
            tval = (*tp)[i];
            aval = (*ap)[i];
            if ( tval != tbad  && aval != abad ) {
               tval = (tval*t.mksScale+t.mksOffset); 
               aval = (aval*alt.mksScale+alt.mksOffset); 
               switch (vcoord) {
               case 0: // altitude
                  // no conversion
                  break;
               case 1: // pressure   
                  aval = 7000.0*LOG(100000.0/aval);
                  break;
               case 2: // theta   
                  // convert theta to fake altitude, given temperature
                  //  given z=H*ln(p0/p), and theta = t*(p0/p)^kappa,
                  //  we want: z = -1/kappa * H * ln( theta/t )
                  aval = (7./2.)*7000.0 * LOG( aval/tval );
                  break;
               case 3: // density   
                  // convert density to fake altitude, given temperature
                  //  given z=H*ln(p0/p), and p = rho*R*t,
                  //  we want: z = H * ln( p0/(rho*r*t) )
                  aval = 7000.0*LOG(100000.0/(aval*287.04*tval) );
                  break;
               } 
               
               dat.push_back( tval );
               alts.push_back( aval );
            }    
        }

        // find the trop
        value = wmo( dat, alts, debug ); 

        // convert this trop alt back to the original altitude-like coordinates
        switch (vcoord) {
        case 0: // altitude
           // no conversion
           break;
        case 1: // pressure   
           // convert back to pressure
           value = 100000.0*EXP( - value/7000.0 );
           break;
        case 2: // theta   
           // "value" is in altitude.  find the temperature at this altitude
           tval = intrp( value, alts, dat );
           // use the temp to convert altitude back to theta
           value = tval * EXP( (2./7.)*value/7000.0 );
           break;
        case 3: // density   
           // "value" is in altitude.  find the temperature at this altitude
           tval = intrp( value, alts, dat );
           // convert altitude to density
           value = 100000.0*EXP( - value/7000.0 )/287.04/tval;
           break;        
        }
        
        // convert back to caller's units, before storing it
        *pnt =  ( value - result->mksOffset )/result->mksScale;

        
        delete tp;
        delete ap;

    }

    if ( flags & OTF_MKS ) {
       switch (vcoord) {
       case 0: // altitude
          vu = "m";
          break;
       case 1: // pressure   
          vu = "Pa";
          break;
       case 2: // theta   
          vu = "K";
          break;
       case 3: // density   
          // "value" is in altitude.  find the temperature at this altitude
          vu = "kg/m^3";
          break;        
       }
    
       result->transform(vu);
    }

    return result;
}

 
 
real TropOTF::intrp( real xi, std::vector<real>&x, std::vector<real>&y ) const
{
      // length of input vectors
      int len;
      // output interpolated result
      real value = 0.0;
      
      // make sure the two input vectors are the same size
      len = x.size();
      if ( len != y.size() || len < 2 ) {
         throw (badprofile());
      }

      // work out way through the profile
      for ( int i=0; i<(len-1); i++ ) {      
      
           if ( (x[i+1]-xi)*(x[i]-xi) <= 0.0 ) {
              // found the two gridpoints that are straddled by our desired point
              
              // interpolate linearly
              value = ( y[i+1]-y[i] )/( x[i+1] - x[i] )*( xi - x[i] ) + y[i];
              
              // leave the loop; we are done
              break;
           }
      
      }
            
      return value;
 
 }
 
