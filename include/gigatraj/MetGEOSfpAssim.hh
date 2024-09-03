#ifndef GIGATRAJ_METGEOSFPASSIM_H
#define GIGATRAJ_METGEOSFPASSIM_H

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
\brief class for reading the NASA GMAO GEOS fp Assimilation data products.

This class implements access to the Goddard Earth Observing System (GEOS) Model
data products created by Globl Modeling and Assimilation Office (GMAO) of
NASA's Goddard Space Flight Center. For more information about these products, see
https://gmao.gsfc.nasa.gov/GEOS_systems/. These data are obtained from
the GMAO Data Portal, an OPeNDAP server at https://opendap.nccs.nasa.gov/dods/GEOS-5/;
this class uses the "fp" product stream.

The names of meteorological fields are set by the GMAO. Unfortunately, most
of the products on the GMAO Data Portal do not have a cf-standard name or units
in the files' metadata. Fortunately, most of the variables are also defined
in the MERRA2 system and hence in the MetMERRA2 class. The cf-standard names
and the units have been copied from that class's variables and applied to 
this class's. Fields which are not in the GEOS fp Assimilation set include:
  - air_potential_temperature = "Theta" here
  - net_heating_rate = "ThetaDot" here


This class accesses only assimilation data, not forecast model runs.
(See the MetGEOSfpFcast class for those.)
For a seamless mixture of the two (assimilation from the past
and forecasts for the future), see the GEOSfp class.

*/

class MetGEOSfpAssim : public MetMyGEOS {

   public:

      /// Default constructor
      /*! This is the default constructor. 
      */
      MetGEOSfpAssim( );
      
      /// Constructor with base date specified
      /*! In this version of the constructor, the base date is specified that is equivalent to internal model time zero
       is explicitly given

          \param date a string containing a date in ISO8601 format (e.g., "2005-04-25")
                      that will serve as the base time from which all model times
                      will be referenced.
      */
      MetGEOSfpAssim( std::string& date );                

      

      /// destructor
      /*! This is the destructor. 
      */
      ~MetGEOSfpAssim();

      
      /// copy constructor
      /*! This is the copy constructor. 
      */
      MetGEOSfpAssim( const MetGEOSfpAssim& src );

      /// assignment operator
      MetGEOSfpAssim& operator=(const MetGEOSfpAssim& src);

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
       void assign( const MetGEOSfpAssim& src );
 
     
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
