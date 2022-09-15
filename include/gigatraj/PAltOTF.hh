#ifndef GIGATRAJ_PALTOTF
#define GIGATRAJ_PALTOTF

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetOnTheFly.hh"
#include "gigatraj/GridField3D.hh"
#include "gigatraj/GridFieldSfc.hh"


namespace gigatraj {

/*!
\ingroup OnTheFly

\brief class for computing pressure altitude from other meteorological data fields

\details

The PAltOTF class provides GridField3D and GridFieldSfc objects full of
pressure altitude values. Pressure altitude is an estimated altitude
that is a function of pressure only. Based on the U. S. Standard
Atmosphere (1962) profile, it is commonly used for aircraft altitudes.

Sources of gridded meteorological fields seldom provide pressure altitude
as one of their products, 
since their products are usually given on isobaric surfaces.
Nevertheless, pressure altitude can be a very useful 
quantity in its own right, especially when products have
been interpolated to new vertical coordinate systems.  
Since pressure altitude is easily calculated
from pressure, the PAltOTF class
ensures that pressure altitude is always available from a given meteorological
source.

It is intended that this class be used by implementers of new
subclasses of MetGridData, to satisfy users' requests pressure altitude data
when the original data source does not provide such fields explicitly.
Note that the calc() methods of this class take fields of the
GridField3D or GridFieldSfc virtual classes as input parameters and produce a new
GridField3D or GridFieldSfc object as output.  Objects of any single subclass 
of GridField3D/GridFieldSfc
may be used as inputs, so long as they are grid-compatible with each other,
and the return value will be of the same
subclass of GridField3D/GridFieldSfc as the inputs.  However, the calling routine will
need to do a dynamic cast from GridField3D or GridfieldSfc to the particular
subclass being used.

Users should obtain their pressure altitude fields by requesting them
from a MetGridData subclass such as MetMERRA, rather than by
reading other fields and giving them to PAltOTF.


*/

class PAltOTF : public MetOnTheFly {
  
     public:

         /// basic constructor 
         /*!
            This is the basic constructor for a new PAltOTF object.
         */
         PAltOTF();

         /// constructor that initializes input quantity names
         /*! This version of the constructor that initializes the 
            names of the quantities that are used to derive pressure altitude.
            These names are used to identify the input gridded fields
            in the calc() methods.

            \param palt the name of the pressure altitude quantity
            \param press the name of the pressure quantity
         */
         PAltOTF(const std::string& palt, const std::string& press);

         /// destructor
         /*! This is the class destructor
         */
         ~PAltOTF();

         /// sets the name of the input quantity used for air pressure
         /*! This function sets the name used to identify an input field as 
             the air pressure quantity, according to local 
             naming conventions.  
             The default is to use the UCAR CF conventions.

            \param quantity the name to be used
         */
         inline void setPressureName(const std::string quantity ) {
            press_name = quantity;
         }            


         /// does the calculation to produce an air pressure field from a gridded 3D field
         /*! This function computes air pressure from a single input 3D field.
             This covers two cases: any quantity on pressure surfaces (i.e., pressure
             is the vertical coordinate), or pressure on any surface.

            \return a pointer to a new GridField3D object that holds the output pressure field.  
                    When the input is on pressure surfaces, the output values 
                    will have the same units as the vertical coordinate of the 
                    input field.  Then the input is on isentropic surfaces,
                    the output will have units of hectoPascals (hPa), which is
                    the same as millibars (mb). The calling routine is responsible for
                    deleting the new object when it is no longer needed.
            \param input the input data field.
            \param flags OTF_* flags to to affect the calculation results
         */
         GridField3D* calc( const GridField3D& input, int flags=0) const;


         /// does the calculation to produce an air pressure field from a gridded 2D field
         /*! This function computes air pressure from a single input 2D pressure field.

            \return a pointer to a new GridField3D object that holds the output pressure field.  
                    When the input is on pressure surfaces, the output values 
                    will have the same units as the vertical coordinate of the 
                    input field.  Then the input is on isentropic surfaces,
                    the output will have units of hectoPascals (hPa), which is
                    the same as millibars (mb). The calling routine is responsible for
                    deleting the new object when it is no longer needed.
            \param input the input data field.
            \param flags OTF_* flags to to affect the calculation results
         */
         GridFieldSfc* calc( const GridFieldSfc& input, int flags=0) const;

         
         /// calculates pressure altitude from a pressure value
         /*! This method does the work of converting a pressure
             value to a pressure altitude value. it works by interpolating
             a standard atmospheric profile in log-pressure.
             This is the inverse of the clac() method.
             
             \param p the input pressure value, in Pascals (not hPa, and not mb)
             \return the pressure altitude value, in km.
         */    
         real calc( real p ) const;

         /// calculates pressure from a pressure altitude value
         /*! This method does the work of converting a pressure altitude
             value to a pressure value. it works by interpolating
             a standard atmospheric profile in log-pressure.
             This is the inverse of the calc() method.
             
             \param z the input pressure altitude value, in km
             \return the pressure altitude value, in Pa.
         */    
         real clac( real z ) const;

     protected:
         /// name of the pressure quantity for the data source being used
         std::string press_name;
         
         /// length of standard profi;e
         static const int stdN;
         /// set of standard altitudes (in km)
         static const real *stdZ;
         /// set of natural log of standard pressures (in Pa, not hPa!)
         static const real *stdLogP;
};
}

#endif



/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/
            
