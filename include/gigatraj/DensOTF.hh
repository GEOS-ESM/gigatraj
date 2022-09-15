#ifndef GIGATRAJ_DENSOTF
#define GIGATRAJ_DENSOTF

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetOnTheFly.hh"
#include "gigatraj/GridField3D.hh"
#include "gigatraj/GridFieldSfc.hh"


namespace gigatraj {

/*!
\ingroup OnTheFly

\brief class for computing air density from other meteorological data fields


\details

The DensOTF class provides GridField3D and GridFieldSfc objects full of
air density values.  

Sources of gridded meteorological fields seldom provide air density
as one of their products, but density can be a very useful 
quantity in many calculations.  Since density is easily calculated
from other quantities that are commonly provided, the DensOTF class
ensures that density is always available from a given meteorological
source.

It is intended that this class be used by implementers of new
subclasses of MetGridData, to satisfy users' requests air density data
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

Users should obtain their air density fields by requesting them
from a MetGridData subclass such as MetMERRA, rather than by
reading other fields and giving them to DensOTF.


*/

class DensOTF : public MetOnTheFly {
  
     public:

         /// basic constructor 
         /*!
            This is the basic constructor for a new DensOTF object.
         */
         DensOTF();

         /// constructor that initializes input quantity names
         /*! This version of the constructor that initializes the 
            names of the quantities that are used to derive air density.
            These names are used to identify the input gridded fields
            in the calc() methods.
            
            \param dens the name of the air density quantity
            \param press the name of the air pressure quantity
            \param temp the name of the air temperature quantity
            \param theta the name of the air potential temperature quantity
         */
         DensOTF(const std::string& dens, const std::string& press, const std::string& temp, const std::string& theta);

         /// destructor
         /*! This is the class destructor
         */
         ~DensOTF();

         /// sets the name of the input quantity used for air temperature
         /*! This sets the name used to identify an input field as 
             the air temperature quantity, according to local 
             naming conventions.  
             The default is to use the UCAR CF conventions.

            \param quantity the name to be used
         */
         inline void setTemperatureName(const std::string quantity ) {
            temp_name = quantity;
         }            

         /// sets the name of the input quantity used for air potential temperature
         /*! This function sets the name used to identify an input field as
             the air potential temperature quantity, according to local 
             naming conventions.  
             The default is to use the UCAR CF conventions.

            \param quantity the name to be used
         */
         inline void setPotentialTemperatureName(const std::string quantity ) {
            theta_name = quantity;
         }            

         /// sets the name of the input quantity used for air pressure
         /*! This function sets the name used to identify an input field as 
             the air pressure quantity, according to local 
             naming conventions.  The default is to use the UCAR CF conventions.

            \param quantity the name to be used
         */
         inline void setPressureName(const std::string quantity ) {
            press_name = quantity;
         }            

         /// does the calculation to produce an air density field from a single gridded 3D field
         /* This function computes density from a single input field.
            This covers four cases: temperature on pressure surfaces (i.e., pressure
            is the vertical coordinate), temperature on isentropic surfaces
            (i.e., potential temperature is the vertical coordinate), 
            potential temperature on pressure surfaces, or 
            pressure on isentropic surfaces.
            
            \return a pointer to a new GridField3D object that holds the output density field.  
                    Its values will have units of kg/m^3. The calling routine is
                    responsible for deleting the new object once it is no longer needed.
            \param input the input data field.
            \param flags OTF_* flags to to affect the calculation results
         */
         GridField3D* calc( const GridField3D& input, int flags=0) const;

         /// does the calculation to produce an air density field from two gridded 3D fields
         /*! This function computes potential temperature from two input fields.
            This covers three cases: temperature and pressure, 
            temperature and potential temperature, and pressure and potential temperature.
            These quantities may be on arbitrary
            surfaces, but both quantities must be on the same
            set of vertical coordinates (as well as the same horizontal grid, of course).  
            The order of the two input parameters is not important. 
            
            \return a pointer to a new GridField3D object that holds the output density field.  
                    Its values will have units of kg/m^3. The calling routine is
                    responsible for deleting the new object once it is no longer needed.
            \param input1 the first input data field.
            \param input2 the second input data field.
            \param flags OTF_* flags to to affect the calculation results
         */
         GridField3D* calc( const GridField3D& input1, const GridField3D& input2, int flags=0 ) const;

         /// does the calculation to produce an air density field from two gridded 2D (surface) fields
         /* This function computes potential temperature from two input fields
            on a quasi-horizontal two-dimensional surface.
            This covers three cases: temperature and pressure, 
            temperature and potential temperature, and pressure and potential temperature.
            The order of the two input parameters is not important, but they must
            both be on the ame surface.
             
            \return a pointer to a new GridFieldSfcmake object that holds the output density field.  
                    Its values will have units of kg/m^3. The calling routine is
                    responsible for deleting the new object once it is no longer needed.
            \param input1 the first input data field.
            \param input2 the second input data field.
            \param flags OTF_* flags to to affect the calculation results
         */
         GridFieldSfc* calc( const GridFieldSfc& input1, const GridFieldSfc& input2, int flags=0 ) const;

     protected:
         /// name of the density quantity for the data source being used
         std::string dens_name;
         /// name of the temperature quantity
         std::string temp_name;
         /// name of the potential temperature quantity
         std::string theta_name;
         ///` name of the pressure quantity
         std::string press_name;

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
            
