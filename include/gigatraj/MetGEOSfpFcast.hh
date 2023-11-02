#ifndef GIGATRAJ_METGEOSFPFCAST_H
#define GIGATRAJ_METGEOSFPFCAST_H

#include <string>
#include <vector>
#include <map>

#include <netcdf.h>
#include <stdlib.h>
#include <sstream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetGEOSfp.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/GEOSfpFcast_Directory.hh"
#include "gigatraj/ThetaOTF.hh"
#include "gigatraj/PressOTF.hh"
#include "gigatraj/ThetaDotOTF.hh"


namespace gigatraj {

/*!
\ingroup MetSrc
\brief class for reading the NASA GMAO GEOS fp Forecast data products.

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
this class's. Fields which are not in the GEOS fp Forecast set include:
  - air_potential_temperature = "Theta" here
  - net_heating_rate = "ThetaDot" here


This class accesses only forecast data from a specific model run.
For assimilation data, see the MetGEOSfpAssim class.
For a seamless mixture of the two (assimilation from the past
and forecasts for the future), see the GEOSfp class.


*/

class MetGEOSfpFcast : public MetGEOSfp {

   public:

      /// An exception for an attmepted assimilation-related opteration
      class badNoAssim {};

      /// Default constructor
      /*! This is the default constructor. 
      */
      MetGEOSfpFcast( );

      /// Constructor with the model run specified
      /*! In this version of the constructor, the model run is explicitly given

          \param mrun a string given the date and time of model initialization, in 
                 YYYYMMDD_HH format.
      */
      MetGEOSfpFcast( std::string& mrun );                

      
      /// Constructor with model run and base date specified
      /*! In this version of the constructor, the base date is specified that is equivalent to internal model time zero.
          (Note that the arguments are in the reverse order from the MetGEOSfp() two-argument constructor.)

          \param mrun a string given the date and time of model initialization, in 
                 YYYYMMDD_HH format.

          \param date a string containing a date in ISO8601 format (e.g., "2005-04-25")
                      that will serve as the base time from which all model times
                      will be referenced.
      */
      MetGEOSfpFcast( std::string& mrun, std::string& date );                



      /// return the model run
      /*! This method returns the model initialization timestamp string.
      
          \return the model run, in YYYYMMSS_HH format.
      */    
      std::string model_run();

      /// return the model run
      /*! This method returns the model initialization timestamp string, in calendar format.
      
          \return the model run, in ISO YYYY-MM-DDTHH:00 format.
      */    
      std::string model_run_cal();

      /// set the model run
      /*! This method sets the model initialization timestamp string.
      
          \param mrun the mode run, in YYYYMMSS_HH format.
      */    
      void set_model_run(std::string &mrun);

      


      /// return whether assimilation or forecast data are being used
      /*! This method returns 1 to indicate that only forecast data are being used.
          
          \return 1 
      
      */
      int which_src();
      
      /// return whether assimilation or forecast data are required to be used
      /*! This method returns 1 to indicte that only forecast data are used.
          
          \return 1 
      
      */
      int which_forced_src();

      /// sets whether assimilation or forecast data are required to be used
      /*! This method overrides that of the parent class and throws an error,
          since we are using only forecasts.
          
          \param which unused
      
      */
      void set_forced_src( int which );



      /// find the two met data source times that bracket the desired model time
      /*! This method finds the two times from the meteorological data source that bracket the
          desired model time. This assumes that the met data are available at discrete
          snapshots in time. 

         \param quantity the desired quantity (Some data sources have different
                time spacings for different quantities)
         \param time the desired model time
         \param t1 the last data-valid time before or at the desired time
         \param t2 the first data-valid time after or at the desired time.
                   If t1 == t2, then the model time is exctly at a snapshot time.
         \return true if the bracketing times are the same; false if they are different.
      */   
      bool bracket( const std::string &quantity, double time, double *t1, double *t2);



      /// return the type of MetData object this is
      /*! This method retuns a string that identifies the specific class of MetData this object is,.
      
          \return the namne of the class 
      */
      std::string id() const { return iam; };


      /// configures the met source
      /*! This method provides a means of setting options specific
          to a subclass. In this way, objects of a subclass
          that has special capabilities may be upcast to MetData
          in a program that can use any of several met data sources,
          and yet still be able to configure those special options
          that apply to the subclass.

          \param name the name of the option that is to be configured.
                      Names that are not recognized by a specific subclass are
                      silently ignored.
                      
          \param value the value to be applied to the named configuration option
      
      */
      void setOption( const std::string &name, const std::string &value );

      /// configures the met source
      /*! This method provides a means of setting options specific
          to a subclass. In this way, objects of a subclass
          that has special capabilities may be upcast to MetData
          in a program that can use any of several met data sources,
          and yet still be able to configure those special options
          that apply to the subclass.

          \param name the name of the option that is to be configured.
                      Names that are not recognized by a specific subclass are
                      silently ignored.
                      
                      Allowed names are:
                      * HorizontalGridThinning - the thining factor
                      * HorizontalGrifOffset - the longititude offset specified w/ thinning
                      
          \param value the value to be applied to the named configuration option
      
      */
      void setOption( const std::string &name, int value );

      /// configures the met source
      /*! This method provides a means of setting options specific
          to a subclass. In this way, objects of a subclass
          that has special capabilities may be upcast to MetData
          in a program that can use any of several met data sources,
          and yet still be able to configure those special options
          that apply to the subclass.

          \param name the name of the option that is to be configured.
                      Names that are not recognized by a specific subclass are
                      silently ignored.
                      
          \param value the value to be applied to the named configuration option
      
      */
      void setOption( const std::string &name, float value );

      /// configures the met source
      /*! This method provides a means of setting options specific
          to a subclass. In this way, objects of a subclass
          that has special capabilities may be upcast to MetData
          in a program that can use any of several met data sources,
          and yet still be able to configure those special options
          that apply to the subclass.

          \param name the name of the option that is to be configured.
                      Names that are not recognized by a specific subclass are
                      silently ignored.
                      
          \param value the value to be applied to the named configuration option
      
      */
      void setOption( const std::string &name, double value );


      /// queries configuration of the met source
      /*! This method provides a means of reading options specific
          to a subclass. In this way, objects of a subclass
          that has special capabilities may be upcast to MetData
          in a program that can use any of several met data sources,
          and yet still be able to configure those special options
          that apply to the subclass.

          \param name the name of the option that is to be obtained.
                      Names that are not recognized by a specific subclass 
                      will return an empty string.
                      
          \param value (output) the value to be obtained from the named configuration option
      
      */
      bool getOption( const std::string &name, std::string &value );


      /// queries configuration of the met source
      /*! This method provides a means of reading options specific
          to a subclass. In this way, objects of a subclass
          that has special capabilities may be upcast to MetData
          in a program that can use any of several met data sources,
          and yet still be able to configure those special options
          that apply to the subclass.

          \param name the name of the option that is to be obtained.
                      Names that are not recognized by a specific subclass 
                      will return 0.
                      
                      Allowed names are:
                      * HorizontalGridThinning - the thining factor
                      * HorizontalGrifOffset - the longititude offset specified w/ thinning
                      
          \param value (output) the value to be obtained from the named configuration option
      
      */
      bool getOption( const std::string &name, int &value );


      /// queries configuration of the met source
      /*! This method provides a means of reading options specific
          to a subclass. In this way, objects of a subclass
          that has special capabilities may be upcast to MetData
          in a program that can use any of several met data sources,
          and yet still be able to configure those special options
          that apply to the subclass.

          \param name the name of the option that is to be obtained.
                      Names that are not recognized by a specific subclass 
                      will return 0.
                      
          \param value (output) the value to be obtained from the named configuration option
      
      */
      bool getOption( const std::string &name, float &value );


      /// queries configuration of the met source
      /*! This method provides a means of reading options specific
          to a subclass. In this way, objects of a subclass
          that has special capabilities may be upcast to MetData
          in a program that can use any of several met data sources,
          and yet still be able to configure those special options
          that apply to the subclass.

          \param name the name of the option that is to be obtained.
                      Names that are not recognized by a specific subclass 
                      will return 0.
                      
          \param value (output) the value to be obtained from the named configuration option
      
          \return true if the option was valid; false if the value returned is meaningless
      */
      bool getOption( const std::string &name, double &value );


   protected:

       
       /// the type of object this is
       static const string iam;

   
      /// set up for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.

           \param quantity the name of the quantity desired
           \param time the valid-at datestamp string for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      int setup(  const std::string quantity, const std::string &time );    

       /// calculates an expiration time for the data returned by this object.
       /*! This method calculates an expiration time from the model run,
           for use with GridField objects created by this class.
           The time at which the data should no longer be used depends on 
           both the model run and the valid-at time of the data,
           since the different GMAO model runs extend out to different
           forecast periods.
           
           \param valid_at a string contaiing the valid-at time of the data
                           whose expiration is to be calculated.
          
           \return the expiration timestamp beyond which the data should not be used,
                   in Unix epoch format.
       */
       time_t expiration( const std::string &valid_at );


      

       /// creates a new MetGEOSfpFcast object
       /*! This method  duplicates the current object,
           creating a new MetGEOSfpFcast object
           with the same key characteristics and settings 
           as the old. However, the new object has no open files
           or met data held in memory cache.
           
           \return a pointer to the new object
       */
       MetGEOSfpFcast* myNew();
       
       
   private:
       
       
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
