#ifndef GIGATRAJ_METSELECTOR_H
#define GIGATRAJ_METSELECTOR_H

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"

#include "gigatraj/MetSBRot.hh"
#include "gigatraj/MetGridSBRot.hh"
#ifdef USE_MERRA
#include "gigatraj/MetMERRA.hh"
#endif
#ifdef USE_MERRA2
#include "gigatraj/MetMERRA2.hh"
#endif
#ifdef USE_GEOSFP
#include "gigatraj/MetGEOSfp.hh"
#include "gigatraj/MetGEOSfpFcast.hh"
#include "gigatraj/MetGEOSfpAssim.hh"
#endif
#ifdef USE_MYGEOS
#include "gigatraj/MetMyGEOS.hh"
#endif


namespace gigatraj {

/*! @name Meteorological Source Characteristocs flags
    These METCAP_* flags indicate charactericstics and capabilities of variious meteorological data sources.

*/

/// Analytical functional data
const int METCAP_AFCN = 0x01;
/// Gridded data
const int METCAP_GRID = 0x02;
/// Gridded in latitude/longitude
const int METCAP_LATLON = 0x04;
/// Uses a remote network service to obtain data
const int METCAP_REMOTE = 0x10;    
/// Forecast data are available
const int METCAP_FCST = 0x20;

/*!
\ingroup MetSrc

\brief class for selecting and generating a met source object

The MetSelector class provides functions that serve as factory methods to
produce objects of various specific kinds of meteorological data source objects.

Note that these objects will be of some generic variety, so their unique
capabilities will be unavailable. Some of those capabilities mey be accessed
through their setOptions() and getOption() methods; see their individual
documentation pages for more information.


*/

class MetSelector {

   public:

      /// An exception for a mis-specified model run
      class badUnknownSource {};


      /// Constructor
      /*! This is the constructor for the MetSelector object.
      */
      MetSelector();
      
      /// Destructor
      /*! This is the destructor
      */
      ~MetSelector();
      
      
      /// Lists meteorological data sources
      /*! This method produces a list of meteorological data sources that are available.
      
          \param flags Any of the METCAP_* Meteorological Source Characteristics flags, OR'ed together.
                 The returned vector will include only those met sources whose characteristics
                 match those specified here. By default, all flags are matched.
          \return a vector of strings, each of which is a name of a met data source
      */
      std::vector<std::string> list( int flags=-1 );
      
      /// Returns the characteristics of a met data source
      /*! This method returns the characteristics of a met data source.
      
          \param source the name of the met source for which characteristics are desired.
          
          \return Any of the METCAP_* Meteorological Source Characteristics flags associated
                 with the given data source, OR'ed together.
      */
      int characterization(const  std::string &source );
      
      /// Returns a brief descriptionof a met data source
      /*! This method returns a description of a met data source.
      
          \param source the name of the met source for which characteristics are desired.
          
          \return a string containing a brief description of a data source
      */
      std::string description(const std::string &source );
      
      /// Returns a meteorological data source
      /*! This method returns a meteorological data source of the requested type.
      
          \param source the name of the met data source being requested 
          
          \return a pointer to a new met data source object. Note that this object should
                  be deleted once it is no longer needed.
      */            
      MetData *source(const  std::string &source );
       
   private:

      // holds into about a single met source
      typedef struct metdesc {
         // name of the source
         std::string name;
         // brief description of the source
         std::string desc;
         // characteristics/capabilities of the source
         int caps;
      } MetDesc;
      
      
      // Holds all the info about the met sources
      // This class is defined that wthat we can use staic const for the variable metinfo below
      class MetInfo {
          public:
             MetInfo();
             
             std::vector<MetDesc> mdesc;            
      };

      // the info about the met data sources
      static const MetInfo metinfo;
      
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
