#ifndef GIGATRAJ_METGEOSFP_H
#define GIGATRAJ_METGEOSFP_H

#include <string>
#include <vector>
#include <map>

#include <netcdf.h>
#include <stdlib.h>
#include <sstream>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetGEOSPortal.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/GEOSfpFcast_Directory.hh"
#include "gigatraj/GEOSfpAssim_Directory.hh"
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

class MetGEOSfp : public MetGEOSPortal {

   public:

      /// An exception for a mis-specified model run
      class badModelRun {};

      
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

      
      /// Constructor with base date and model run specified
      /*! In this version of the constructor, the base date is specified that is equivalent to internal model time zero.

          \param date a string containing a date in ISO8601 format (e.g., "2005-04-25")
                      that will serve as the base time from which all model times
                      will be referenced.

          \param mrun a string given the date and time of model initialization, in 
                 YYYYMMDD_HH format.
      */
      MetGEOSfp( std::string& date, std::string& mrun );                

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
      /*! This method returns a value that indicates whether assimilation or forecast
          data are currently being read. This may change from read operation
          to read operation.
          
          \return -1 if assimilation data are being read, +1 if forecast data are being read, 0 if indeterminate
      
      */
      int which_src();
      
      /// return whether assimilation or forecast data are required to be used
      /*! This method returns a value that indicates whether assimilation or forecast
          data are forced to be read. The setting default to "either one that works best"
          but may be changed by the user.
          
          \return -1 if only assimilation data are to read, +1 if only forecast data are to be read, 0 if either is OK
      
      */
      int which_forced_src();

      /// sets whether assimilation or forecast data are required to be used
      /*! This method changes the internal setting that forces either assimilation or forecast
          data to be read. The setting default to "either one that works best".
          
          \param which -1 if only assimilation data are to read, +1 if only forecast data are to be read, 0 if either is OK
      
      */
      void set_forced_src( int which );


      /*! Check that a quantity name is recognized by this data source
          Returns true if recognized, false otherwise
          
       \param quantity  a string that  holds the name of the quantity   
      */
      bool legalQuantity( const std::string quantity );     


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
       void assign( const MetGEOSfp& src );
 
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

       /// creates a new MetGEOSfp object
       /*! This method  duplicates the current object,
           creating a new MetGEOSfp object
           with the same key characteristics and settings 
           as the old. However, the new object has no open files
           or met data held in memory cache.
           
           This is useful for calculating quantities on th efly
           from consitituent quantities that have to be read in 
           separately.
           
           \return a pointer to the new object
       */
       MetGEOSfp* myNew();
       
      /// queries for characteristics of a particular quantity within a data source
      /*! This method obtains information about the structure of a data field in the data source.
          Not all quantities share the same spatial grid or time spacing. This function
          returns information about these things.
      
           \param quantity the name of the quantity to be looked up
           \param longName (output) if non-NULL, the long descriptive name of the quantity is returned
           \param units (output) if non-NULL, the units of the quantity are returned
           \param ndims (output) if non-NULL, the number of spatial dimensions (2 or 3) is returned
           \param actualVertGrid (output) if non-NULL, the vertical grid specification is returned
           \param actualHorizGrid (output) if non-NULL, the horizontal grid specification is returned
           \param actualTimeSpace (output) if non-NULL, the time spacing is returned
           \param actualTimeAvg (output) if non-NULL, the time averagin is returned
           \param actualBaseTime (output) if non-NULL, the base time is returned
           \param url  (output) if non-NULL, the URL by which this quantity is access is returned
           
           \return a status indicating whether the quantity's characteristics were identified. Zero indicates success.
                   nonzero indicates failure.
        
      */    
      int queryQuantity( const std::string quantity
                , std::string **longName, std::string **units, int *ndims
                , int *actualVertGrid, int *actualHorizGrid, int *actualTimeSpace, int *actualTimeAvg, int *actualBaseTime
                , std::string **url );



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

      /// prepare for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.
          
          This has the side effect that it constructs an internal URL, and that 
          requires opening a connection ot the OPeNDAP server, to determine whether
          the assimilation or the forecasts should be used.

           \param quantity the name of the quantity desired
           \param time the valid-at time for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      int prep(  const std::string quantity, const double time );    
  
      /// prepare for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.

          This has the side effect that it constructs an internal URL, and that 
          requires opening a connection ot the OPeNDAP server, to determine whether
          the assimilation or the forecasts should be used.

           \param quantity the name of the quantity desired
           \param time the valid-at datestamp string for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      int prep(  const std::string quantity, const std::string &time );    





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


       /// Object for finding GEOS fp Assimilation data
       static const gigatraj::GEOSfpAssim_Directory adir;
       /// Object for finding GEOS fp Forecast data
       static const gigatraj::GEOSfpFcast_Directory fdir;
       
       /// model run
       std::string modelRun;
       
       /// which data stream are we using:
       //   -1 = assimilation
       //    1 = forecast
       //    0 = undetermined
       int whichSrc;
       
       /// force the data stream to use:
       //   -1 = assimilation
       //    1 = forecast
       //    0 = whichever is best
       int forceSrc;
       
       

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
           
           \param nativeTime the time value, in file-native format (days elapsed since 0001-01-01T00:00:00)
       */    
       double Portal_time_nativeTo1900( double nativeTime );

       /// converts time interval from the file internal format to day1900
       /*! This method converts a time delta (interval) value from the format used in the original remote data file,
           to day1900 (i.e., days).
           
           \param nativeDelTime the time value, in file-native format (days elapsed since 0001-01-01T00:00:00)
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
