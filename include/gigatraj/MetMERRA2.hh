#ifndef GIGATRAJ_METMERRA2_H
#define GIGATRAJ_METMERRA2_H

#include <string>
#include <vector>
#include <map>

#include <netcdf.h>
#include <stdlib.h>
#include <sstream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetGEOSPortal.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/MERRA2_Directory.hh"
#include "gigatraj/ThetaOTF.hh"
#include "gigatraj/PressOTF.hh"
#include "gigatraj/ThetaDotOTF.hh"


namespace gigatraj {

/*!
\ingroup MetSrc
\brief class for reading the NASA GMAO MERRA2 data product.

This class implements access to the Modern-Era Retrospective analysis for Research and Applications II (MERRA2)
data products created by Global Modeling and Assimilation Office (GMAO) of
NASA's Goddard Space Flight Center. For more information about these products, see
https://gmao.gsfc.nasa.gov/reanalysis/MERRA2/


The names of meteorological fields are set by the GMAO, but each quantity has a 
cf-standard name as well. Fields which are not in the MERRA2 set include:
  - air_potential_temperature = "Theta" here
  - net_heating_rate = "ThetaDot" here

NOTE: These data are provided by an OPeNDAP server hosted by NASA's "GES DISC" system.
which requires registration and authentication.  You are free to register and access
the data, but without an account MERRA2 access will fail. For more information, see
https://disc.gsfc.nasa.gov/data-access
Once you have an account, create a netrc file somewhere with your credentials
in it (as described in the GES DISC documentation) and then create an empty file
for cookies. Finally, create a $HOME/.dodsrc file that defines HTTP.NETRC
and HTTP.COOKIEJAR, as described in the section "Grid Analysis and Display System (GrADS)".


*/

class MetMERRA2 : public MetGEOSPortal {

   public:

      /// An exception for a mis-specified model run
      class badModelRun {};

      
      /// Default constructor
      /*! This is the default constructor. 
      */
      MetMERRA2( );
      
      /// Constructor with base date specified
      /*! In this version of the constructor, the base date is specified that is equivalent to internal model time zero
       is explicitly given

          \param date a string containing a date in ISO8601 format (e.g., "2005-04-25")
                      that will serve as the base time from which all model times
                      will be referenced.
      */
      MetMERRA2( std::string& date );                

      

      /// destructor
      /*! This is the destructor. 
      */
      ~MetMERRA2();

      
      /// copy constructor
      /*! This is the copy constructor. 
      */
      MetMERRA2( const MetMERRA2& src );

      /// assignment operator
      MetMERRA2& operator=(const MetMERRA2& src);

      
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

      /*! Check that a quantity name is recognized by this data source
          Returns true if recognized, false otherwise
          
       \param quantity  a string that  holds the name of the quantity   
      */
      bool legalQuantity( const std::string quantity );     


      /// return the type of MetData object this is
      /*! This method retuns a string that identifies the specific class of MetData this object is,.
      
          \return the name of the class 
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
                      * HorizontalGridOffset - the longititude offset specified w/ thinning
                      * ForecastOnly - if 1, then read only forecast data
                      * AnalysisOnly - if 1, then read only analysis data
                      * AnalysisAndForecast - if 1 then read either analysis or forecast data
                      * Delay - a number of seconds to wait befoe opening a new URL
                      
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
                      * HorizontalGridOffset - the longititude offset specified w/ thinning
                      * ForecastOnly - 1 if reading only forecast data; 0 otherwise
                      * AnalysisOnly - 1 if reading  only analysis data; 0 otherwise
                      * AnalysisAndForecast - 1 if reading  either analysis or forecast data; 0 otherwise
                      
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


      /// waits a certain time
      /*! This method waits for a time, to avoid have a large number of processors
          running in parallel, all attempting to access the same network resource at the
          same moment. Instead, the processors of a single group may all
          delay together for a randomly-determined tie, to offset them from other
          processor groups.
          
          If parallel processing is not being used, the wait time is fixed at zero, and so there is 
          is no delay. 
          
          Subclasses which require such delays may override this virtual method
          with their own.
      */
      void delay();


   protected:

       
       /// the type of object this is
       static const string iam;

       /// copy a given object into this object
       /*! This method copies properties of a given MetaData object
           into this object.
      
           \param src the object to be copied
       */    
       void assign( const MetMERRA2& src );
 
      /// converts an integer to a string
      std::string int2asc( int val );

      /// generate a file path to a cache file for a gridded met field
      /*! This method returns a pointer to a full file path for a disk cache file, or NULL
          if the data are not to be cached on disk.

          \param item the GridFieldSfc data object which is to be cached

          \return the cache file path name
      */
      FilePath* cachefile( const GridFieldSfc* item ) const;

      /// generate a fle path to a cache file for a gridded met field
      /*! This method returns a pointer to a full file path for a disk cache file, or NULL
          if the data are not to be cached on disk.

          \param item the GridField3D data object which is to be cached
          
          \return the cache file path name
      */
      FilePath* cachefile( const GridField3D* item ) const;

      /// translate CF quantity names to GEOS fp Forecast names
      /*! This method translates quantity names according to the CF convention,
          into GEOS fp Forecast quantity names.

          \param quantity the desired physical quantity name using CF conventions
          
          \return the name of a physical quantity according
          to GEOS fp Forecast conventions. 
      */    
       std::string quantityName_cf2geos(const std::string &quantity ) const;

      /// translate GEOS fp Forecast quantity names to CF names
      /*! This method translates quantity names according to the GEOS fp Forecast,
          into CF quantity names.

          \param quantity the desired physical quantity name using GEOS fp Forecast conventions
          
          \return the name of a physical quantity according
          to CF conventions. 
      */    
       std::string quantityName_geos2cf(const std::string &quantity ) const;

  
      /// set up for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.

           \param quantity the name of the quantity desired
           \param time the valid-at time for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      int setup(  const std::string quantity, const double time );    
  
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

       /// creates a new MetMERRA2 object
       /*! This method  duplicates the current object,
           creating a new MetMERRA2 object
           with the same key characteristics and settings 
           as the old. However, the new object has no open files
           or met data held in memory cache.
           
           This is useful for calculating quantities on the fly
           from consitituent quantities that have to be read in 
           separately.
           
           \return a pointer to the new object
       */
       MetMERRA2* myNew();
       


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


      /// find the two calendar string timestamps that bracket the desired calendar string model time
      /*! This method finds the two calendar string timestamps that bracket the
          desired model time.

         \param quantity the desired quantity (Some data sources have different
                time spacings for different quantities)
         \param time the desired model time, expressed as a calendar string.
         \param t1 the last data-valid time before or at the desired time
         \param t2 the first data-valid time after or at the desired time
         \return true if the bracketing times are the same; false if they are different.
      */   
      bool bracket( const std::string &quantity, const std::string &time, std::string *t1, std::string *t2);





      /// Returns a vector of prerequisite quantities 
      /*! Returns a vector of quantities that must exist for the given quantity to exist
          
       \param quantity  a string that  holds the name of the desired quantity   
      */
      std::vector<string> *new_testQuantity( string quantity );

      /// sets the name of the vertical coordinate used in the data source
      /*! This method forces the meteorological data source object to use a specific vertical coordinate.
          If the original data for this source use a different vertical coordinate,
          data fields read in will be vertically interpolated to the new coordinate system
          automatically whenever they are read in.
       
          This enables a single data source to be used for kinematic trajectories
          (in which the vertical coordinate is usually pressure or pressure altitude)
          and isentropic trajectories (in which the vertical coordinate is
          potential temperature).
          
          \param quantity the desired quantity to be used for the vertical coordinate
          \param units the units of the desired vertical coordinate
      */
      void set_vertical( const std::string quantity, const std::string units );

      /// sets the name of the vertical coordinate used in the data source
      /*! This method forces the meteorological data source object to use a specific vertical coordinate.
          If the original data for this source use a different vertical coordinate,
          data fields read in will be vertically interpolated to the new coordinate system
          automatically whenever they are read in.
       
          This enables a single data source to be used for kinematic trajectories
          (in which the vertical coordinate is usually pressure or pressure altitude)
          and isentropic trajectories (in which the vertical coordinate is
          potential temperature).
          
          \param quantity the desired quantity to be used for the vertical coordinate
          \param units the units of the desired vertical coordinate
          \param levels a pointer to a vector of values of the new vertical coordinate
      */
      void set_vertical( const std::string quantity, const std::string units, const std::vector<real>* levels );


      /// get a 3D data field valid at a certain time, using basic access
      /*! This method reads meteorological data from some source and returns
          it in a GridField3D object. 
          
          It takes the data directly from the source, with no caching or
          met data client/server interactions.
          
           \param quantity the (internal or cf-convention) name of the quantity desired
           \param time the valid-at datestamp string for which data is desired

           \return a pointer to a GridField3D object that holds the data. The caller is
                   responsible for deleting this object, using the MetGridData remove() method.

      */
      GridLatLonField3D* new_directGrid3D( const std::string quantity, const std::string time );

      /// get a 2D data field valid at a certain time, using basic access
      /*! This method reads meteorological data from some source and returns
          it in a GridFieldSfc object.
          
          It takes the data directly from the source, with no caching or
          met data client/server interactions.
          
           \param quantity the (internal or cf-convention) name of the quantity desired
           \param time the valid-at datestamp string for which data is desired

           \return a pointer to a GridFieldSfc object that holds the data. The caller is
                   responsible for deleting this object, using the MetGridData remove() method.

      */
      GridLatLonFieldSfc* new_directGridSfc( const std::string quantity, const std::string time );


       
       /// Object for finding MERRA2 data
       static const gigatraj::MERRA2_Directory dir;
       
       

       /// reset the internals
       /*! This method resets the internal state of this object.
       */
       void reset();
 
       /// loads horizontal and vertical grid specification tables
       /*! This methods loads internal tables describing how the horizontal abnd vertical grids are specified
       */
       void setup_gridSpecs();


       /// converts time from the file internal format to day1900
       /*! This method converts a time value from the format used in the original remote data file,
           to day1900 (i.e., days elasped since 1900-01-01T00:00:00).
           
           \param nativeTime the time value, in file-native format (minutes elapsed since 00Z the date of the file)
       */    
       double Portal_time_nativeTo1900( double nativeTime );

       /// converts time interval from the file internal format to day1900
       /*! This method converts a time delta (interval) value from the format used in the original remote data file,
           to day1900 (i.e., days).
           
           \param nativeDelTime the time value, in file-native format (minutes elapsed since 00Z the date of the file)
       */    
       double Portal_deltime_nativeTo1900( double nativeDelTime );

       
   private:
       
     void setup_vars();
     
     
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
