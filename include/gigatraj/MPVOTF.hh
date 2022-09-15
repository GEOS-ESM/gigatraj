#ifndef GIGATRAJ_MPVOTF_H
#define GIGATRAJ_MPVOTF_H

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetOnTheFly.hh"
#include "gigatraj/GridField3D.hh"
#include "gigatraj/GridFieldSfc.hh"


namespace gigatraj {

/*!
\ingroup OnTheFly

\brief class for calculating "modified" potential vorticity ("MPV") data fields from other meteorological data fields

\details

The MPVOTF class provides GridField3D and GridFieldSfc objects full of
"modified potential vorticity" values.  (See Lait, 1994: An alternative form
for potential vorticity.  \a J. \a Atmos. \a Sci., \b 51, 1754-1759.)

MPV is not a standard product from sources of 
gridded meteorological fields, but it can be a very useful 
quantity.  Since it is easily calculated from other quantities
that are commonly provided by meteorological data sources, the
MPVOTF class ensures that MPV is always available from a given meteorological
source.

It is intended that this class be used by implementers of new
subclasses of MetGridData, to satisfy users' requests MPV data
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

Users should obtain their MPV fields by requesting them
from a MetGridData subclass such as MetMERRA, rather than by
reading other fields and giving them to MPVOTF.



*/

class MPVOTF : public MetOnTheFly {
  
     public:

         /// basic constructor 
         /*!
            This is the basic constructor for a new MPVOTF object.
         */
         MPVOTF();
         
         /// constructor that initializes input quantity names
         /*! This version of the constructor that initializes the 
            names of the quantities that are used to derive MPV.
            These names are used to identify the input gridded fields
            in the calc() methods.
            
             \param epv the name of the Ertel's potential vorticity quantity
             \param theta the name of the potential temperature quantity
         */
         MPVOTF( const std::string& epv, const std::string& theta ); 
            
         /// destructor
         /*! This is the class destructor
         */
         ~MPVOTF();

         /// sets the name of the input quantity used for EPV
         /*! This function sets the name used to identify an input field as 
             Ertel's potential vorticity, according to local 
             naming conventions.  
             The default is to use the UCAR CF conventions.

            \param quantity the name to be used
         */
         inline void setEPVName(const std::string quantity ) {
            ename = quantity;
         }            

         /// sets the name of the input quantity used for air potential temperature
         /*! This function sets the name used to identify an input field as
             the air potential temperature quantity, according to local 
             naming conventions.  
             The default is to use the UCAR CF conventions.

            \param quantity the name to be used
         */
         inline void setPotentialTemperatureName(const std::string quantity ) {
            hname = quantity;
         }            


         /// Calculates MPV from EPV and Theta
         /*! This function calculates the MPV field
             from fields of Ertel's potential vorticity (EPV) and potential temperature (theta).
            
            \return a pointer to a new GridField3D object that holds the output MPV field.  
                    Its values will have the same units as the input EPV field.
                    The calling routine is responsible for deleting the new object 
                    once it is no longer needed. 
            \param epv a GridData3D field of Ertel's potential vorticity
            \param theta a GridField3D field of potential temperatures
            \param flags OTF_* flags to to affect the calculation results
         */   
         GridField3D* calc( const GridField3D& epv, const GridField3D& theta, int flags=0) const;


         /// Calculates MPV from EPV on isentropic surfaces
         /*! This function calculates the MPV field
             from a field of Ertel's potential vorticity on isentropic surfaces
             
            \return a pointer to a new GridField3D object that holds the output MPV field.  
                    Its values will have the same units as the input EPV field. 
                    The calling routine is responsible for deleting the new object 
                    once it is no longer needed. 
            \param epv a GridField3D field of isentropic Ertel's potential vorticity
            \param flags OTF_* flags to to affect the calculation results

         */   
         GridField3D* calc( const GridField3D& epv, int flags=0 ) const;


         /// Calculates MPV from EPV and Theta
         /*! This function calculates the MPV field
             from fields of Ertel's potential vorticity and potential temperature
             on quasi-horizontal 2D surfaces.  
             Note that the EPV and theta fields must be on the same surface.
             
            \return a pointer to a new GridFieldSfc object that holds the output MPV field.  
                    Its values will have the same units as the input EPV field. 
                    The calling routine is responsible for deleting the new object 
                    once it is no longer needed. 
            \param epv a GridFieldSfc field of Ertel's potential vorticity
            \param theta a GridFieldSfc field of potential temperatures 
            \param flags OTF_* flags to to affect the calculation results
         */           
         GridFieldSfc* calc( const GridFieldSfc& epv, const GridFieldSfc& theta, int flags=0) const;

     protected:
        /// the name of the EPV quantity
        std::string ename;
        /// the name of the potential temperature quantity
        std::string hname;
        

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
