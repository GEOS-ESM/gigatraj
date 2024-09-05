#ifndef GIGATRAJ_METGEOSFP_H
#define GIGATRAJ_METGEOSFP_H

#include <string>
#include <vector>
#include <map>

#include <netcdf.h>
#include <stdlib.h>
#include <sstream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetMyGEOS.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/ThetaOTF.hh"
#include "gigatraj/PressOTF.hh"
#include "gigatraj/ThetaDotOTF.hh"


namespace gigatraj {

/*!
\ingroup MetSrc

\brief class for reading the NASA GMAO GEOS fp assimilation and forecast data products.


This class provides an interface for accessing both
assimilation products and forecast model products
from the  Goddard Earth Observing System (GEOS) "fp" processing stream
created by Globl Modeling and Assimilation Office (GMAO) of
NASA's Goddard Space Flight Center. 
For more information about these products, see
https://gmao.gsfc.nasa.gov/GEOS_systems/. These data are obtained from
the GMAO Data Portal, an OPeNDAP server at https://opendap.nccs.nasa.gov/dods/GEOS-5/;
this class uses the "fp" product stream.

On the GMAO data portal, there are two streams of "fp" products: the
assimilation, which  is based on measurement data and is therefore
necessarily confined to  the past; and the forecast model, which  
projects into the future. There are two other classes, MetGEOSfpAssim
and MetGEOSfpFcast, reespectively, which access these data.
This class is intended to provide a seamless interface to access
both as a single data source.

(The GMAO OPeNDAP server also offers its own seamless data stream, which
appends the latest forecast to the assimilation. However, the latest
forecast can extend anywhere from 30 hours to 240 hours into the 
future, depending on which model run is the latest. The MetGEOSfp class
will select and use the forecast model run that fits best with the data
you are trying to access.)

The names of meteorological fields are set by the GMAO. Unfortunately, most
of the products on the GMAO Data Portal do not have a cf-standard name or units
in the files' metadata. Fortunately, most of the variables are also defined
in the MERRA2 system and hence in the MetMERRA2 class. The cf-standard names
and the units have been copied from that class's variables and applied to 
this class's. Fields which are not in the GEOS fp Assimilation set include:
  - air_potential_temperature = "Theta" here
  - net_heating_rate = "ThetaDot" here




*/

class MetGEOSfp : public MetMyGEOS {

   public:

      /// Default constructor
      /*! This is the default constructor. 
      */
      MetGEOSfp( );
      
      /// Constructor with base date specified
      /*! In this version of the constructor, the base date is specified that is equivalent to internal model time zero
       is explicitly given

          \param date a string containing a date in ISO8601 format (e.g., "2005-04-25")
                      that will serve as the base time from which all model times
                      will be referenced.
      */
      MetGEOSfp( std::string& date );                

      

      /// destructor
      /*! This is the destructor. 
      */
      ~MetGEOSfp();

      
      /// copy constructor
      /*! This is the copy constructor. 
      */
      MetGEOSfp( const MetGEOSfp& src );

      /// assignment operator
      MetGEOSfp& operator=(const MetGEOSfp& src);

     /// create a copy of a MetData subclass object, as a MetData superclass object
     /*! This method creates a new copy of a specific MetData object, which 
         ordinarily would mean that the copy would be of the same specific subclass
         as the original. Which in turn would mean that it could not be used in
         the generic, subclass-independent calling routines which know only about
         the MetData superclass. To remedy this, the copy() method creates the copy
         and retuns it as a MetData object.
     
           \return returns a MetData pointer to the copy. Note that the calling routine is 
                   responsible for deleting the new object.
     */
     MetData *genericCopy();

      /// returns a copy of the object, cast to the MetGridData class
      /*!
           Sometimes a routine that deals with objects of a MetGridData subclass
           needs to make a copy of the object, but needs to do so in a way
           that allows the copy to be made with no information about
           the exact subclass being used.
           
           This method, implemented by hte various subclasses, will create
           a copy of the object, cast the new object to the MetData class,
           and then return the result.
           
           \return a pointer to a new MetData object. The calling routine is repsonsible for delteing the
                   object when it is no longer needed 
      */
      virtual MetGridData* MetGridCopy();

      /// return the type of MetData object this is
      /*! This method retuns a string that identifies the specific class of MetData this object is,.
      
          \return the name of the class 
      */
      std::string id() const { return iam; };

   protected:
       
       
       /// the type of object this is
       static const string iam;

       /// copy a given object into this object
       /*! This method copies properties of a given MetaData object
           into this object.
      
           \param src the object to be copied
       */    
       void assign( const MetGEOSfp& src );
 
     
};
}

#endif



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
