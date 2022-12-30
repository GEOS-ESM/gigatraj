#ifndef GIGATRAJ_METGEOSPORTAL_H
#define GIGATRAJ_METGEOSPORTAL_H

#include <netcdf.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>


#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetGridLatLonData.hh"
#include "gigatraj/CalGregorian.hh"
#include "gigatraj/GEOSfpFcast_Directory.hh"
#include "gigatraj/GEOSfpAssim_Directory.hh"
#include "gigatraj/PressOTF.hh"
#include "gigatraj/ThetaOTF.hh"
#include "gigatraj/ThetaDotOTF.hh"
#include "gigatraj/PAltOTF.hh"
#include "gigatraj/PAltDotOTF.hh"


namespace gigatraj {


/*! \brief type Strictitude holds flag values to indicate how strict or flexible we should be in requesting data
 
  A Strictitude type variable holds bitwise flags that signify whether we
  can accept multiple types of grid characteristics.
 
*/  
typedef int Strictitude;

/*! @name StrictitudeVals
    \defgroup strictitudevals Data request strictness/flexibility values
    
    A MetGEOSPortal data source can have look strictly or flexibly for data.
    If some aspect of the data gridding is to be strict, then only the specified grid
    characteristic is acceptable. If not strict (flexible), then any value of
    that grid characteristic can be accepted, with the understanding that the data read
    will need to be converted or regridded in some way.
    
*/
/*! \brief  Strictitude value:  StrictAboutHgrid = the request should be strict about the horizontsl grid specification
  \ingroup strictitudevals
*/
const Strictitude StrictAboutHgrid = 0x01;
      
/*! \brief  Strictitude value:  StrictAboutVgrid = the request should be strict about the vertical grid specification
  \ingroup strictitudevals
*/
const Strictitude StrictAboutVgrid = 0x02;
 
      
/*! \brief  Strictitude value:  StrictAboutTgrid = the request should be strict about the time spacing specification
  \ingroup strictitudevals
*/
const Strictitude StrictAboutTgrid = 0x04;
      
/*! \brief  Strictitude value:  StrictAboutTavg = the request should be strict about the time averaging specification
  \ingroup strictitudevals
*/
const Strictitude StrictAboutTavg = 0x08;

/*! \brief  Strictitude value:  StrictAboutNothing = the request should be lax about everything
*/
const Strictitude StrictAboutNothing = 0x00;

/*! \brief  Strictitude value:  StrictAboutGridding = the request should be strict about horizontal, vertical, and time gridding
*/
const Strictitude StrictAboutGridding = 0x07;



/*!
\ingroup MetSrc

\brief abstract class for reading the GEOS data products from the NASA GMAO data portal OPeNDAP server.

This class provides an interface for accessing 
data products provided by the Globl Modeling and Assimilation Office (GMAO) of
NASA's Goddard Space Flight Center. 
For more information about these products, see
https://gmao.gsfc.nasa.gov/GEOS_systems/. These data are obtained from
the GMAO Data Portal, an OPeNDAP server at https://opendap.nccs.nasa.gov/dods/GEOS-5/.

The names of meteorological fields are set by the GMAO. Unfortunately, most
of the products on the GMAO Data Portal do not have a cf-standard name or units
in the files' metadata. Fortunately, most of the variables are also defined
in the MERRA2 system and hence in the MetMERRA2 class. The cf-standard names
and the units have been copied from that class's variables and applied to 
this class's. Fields which are not in the GEOS fp Assimilation set include:
  - air_potential_temperature = "Theta" here
  - net_heating_rate = "ThetaDot" here


*/

class MetGEOSPortal : public MetGridLatLonData {

   public:

      /// An exception for failing to find GEOS Portal quantity
      class badDataNotFound {};

      /// An exception for given dimensions not matching the file name's specs
      class badDimsMismatch {};

      /// An exception for given dimensions not being in the proper form
      class badDimsForm {};

      /// An exception for an unknown dimension in a variable
      class badUnknownDim {};

      /// An exception for trying to read a nonexistent dimension
      class badMissingDim {};

      /// An exception for failing to allocate buffer memory
      class badNoMem {};

      /// An exception for failing to find the requested time snapshot
      class badTimeNotFound {};

      /// An exception for failing to open the GEOS Portal netcdf file
      class badNetcdfOpen {
         public:
            /// error code
            int error;
            
            /// constructor
            badNetcdfOpen(int err) { error = err; }
            
      };

      /// An exception for some miscellaneous error while trying to read the GEOS Portal netcdf file
      class badNetcdfError {
         public:
            /// error code
            int error;
            /// constructor
            badNetcdfError(int err) { error = err; }
            
      };
      



      /// Default constructor
      /*! This is the default constructor. 
      */
      MetGEOSPortal( );
      
      /// Constructor with the model run specified
      /*! In this version of the constructor, the model run is explicitly given

          \param date a string containing a date in ISO8601 format (e.g., "2005-04-25")
                      that will serve as the base time from which all model times
                      will be referenced.
      */
      MetGEOSPortal( std::string& date );                

      /// destructor
      /*! This is the destructor. 
      */
      ~MetGEOSPortal();

      
      /// copy constructor
      /*! This is the copy constructor. 
      */
      MetGEOSPortal( const MetGEOSPortal& src );

      /// assignment operator
      MetGEOSPortal& operator=(const MetGEOSPortal& src);

      /// assignment operator for next level up
      MetGridLatLonData& operator=(const MetGridLatLonData& src); 


      /// convert model time to a date+time calendar string
      /*! This method converts internal model time to an ISO8601 date string
          (e.g., "2003-07-15T12:35").

          \param time the model time that is to be converted
          \param format = the output format of the date:
                  * -999 = use the default format 
                  * 0 = date only, with no time (yyyy-mm-dd)
                  * 1 = date plus the hour (yyyy-mm-ddThh)
                  * 2 = date plus the hour and minute (yyyy-mm-ddThh:mm)
                  * 3 = date plus the hour, minute, and integer second (yyyy-mm-ddThh:mm:ss)
                  * 4 = date plus the hour, minute, and floating second (to four decimal places) (yyyy-mm-ddThh:mm:ss.ssss) 

          \return the time as expressed in ISO8601 format.
          
      */   
      std::string time2Cal( const double time, int format=-999 );

      /// convert a date+time calendar string to model time
      /*! This method converts an ISO8601 date string to internal model time.

          \param date a timestamp in ISO8601 format (e.g., "2005-04-20T18:45:03")
          
          \return the internal model time that corresponds to the input string
               
      */   
      double cal2Time( const std::string date );

      /// sets the equivalence between internal model time and a date+time calendar string
      /*! A data source will usually associate meteorological field values with
          some kind of time stamp, here expressed as a string. The trajectory mode;
          represents time as a double-precision floating-point number.
          This routine defines the equivalence between the two.
          
          \param caldate  a time as expressed in some data-specific calendar system 
          \param time the model time that corresponds to cal  
      */
      void defineCal( string caldate, double time=0.0 );

      /// retrieve the thinning factor for horizontal data
      /*! This method returns the thinning factor that is applied to horizontal data.
          This determines how many data points are to be skipped. A value
          of 1 or less means that every point is used. A value of 2 means that
          only every other longitude and latitude is retained. A value of three
          means that only every third longitude and latitude is retained.
          (All vertical levels are alays retained.)
          
          \param offset an in pointer to which, if non-NULL, is returned the starting index to be used in longitudes

          \return the thinning factor

      */
      int thinning( int* offset=NULL );
      
      /// sets the thinning factor for horizontal data    
      /*! This method sets the thinning factor that is applied to horizontal data.
          This determines how many data points are to be skipped. A value
          of 1 or less means that every point is used. A value of 2 means that
          only every other longitude and latitude is retained. A value of three
          means that only every third longitude and latitude is retained.
          (All vertical levels are alays retained.)
          
          \param thin the thinning factor to be applied to subsequent data reads
          \param offset a offset index used to determine the starting longitude value;
          
      */
      void set_thinning( int thin, int offset=0 );
 
      /// Returns a vector of prerequisite quantities 
      /*! Returns a vector of quantities that must exist for the given quantity to exist
          
       \param quantity  a string that  holds the name of the desired quantity   
      */
      virtual std::vector<string> *new_testQuantity( string quantity ) = 0;


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
      virtual void set_vertical( const std::string quantity, const std::string units, const std::vector<real>* levels=NULLPTR ) = 0;


      /// sets the name of the vertical coordinate used in the data source
      /*! This method forces the meteorological data source object to use a specific vertical coordinate.
          
          This function gets called by the various subclasses's set_vertical() functions.
          
          \param quantity the desired quantity to be used for the vertical coordinate
          \param units the units of the desired vertical coordinate
          \param levels a pointer to a vector of values of the new vertical coordinate
          \param scale a scale factor to be applied to this quantity that would convert it to MKS units
          \param offset an offset to be applied ot this quantity (after the scale factor) that would convert it to MKS units
      */
      virtual void set_verticalBase( const std::string quantity, const std::string units, const std::vector<real>* levels=NULLPTR, real scale=1.0, real offset=0.0 );

      /// sets the vertical coordinates from a GridField3D object
      /*! This method forces the meteorological data source object to use vertical coordinates taken form a GridField3D object.
      
          \param grid the grid whose vertical coordinates are to be used.
      */
      virtual void set_verticalBase( GridField3D *grid );    


      /// set the base time
      /*! This method sets the base time: the ISO8601 tdate string that corresponds to internal model time zero.
          \param date the date (in ISO8601 format) from which all model times
                 are to be referenced.
      */
      void set_BaseTime( std::string& date );           

      /// return the base time
      /*! This method returns the base time, as an ISO8601 string that corresponds to 
          the internal model time 0.000000.  Note that a new string is created for the return
          value, and it is up to the calling routine to delete it when it is
          no longer needed.
       */
      std::string BaseTime( ) const;  
      
      /// return a 3D data field object 
      /*! This method returns a 3D field object full of data valid at a given time.
        
           \param quantity the name of the desired meteorological data field
           \param time the ISO8601 data string specifying the valid-at time of the desired data
           
           \return a pointer to a new gridded-field object that holds the data. The caller is
                   responsible for deleting this object, using the MetGridData remove() method.
      
      */
      GridLatLonField3D* Get3D( const std::string quantity, const std::string time );         
      
      /// return a 2D data field object 
      /*! This method returns a 2D quasi-horizontal surface field object full of data valid at a given time.
        
           \param quantity the name of the desired meteorological data field
           \param time the ISO8601 data string specifying the valid-at time of the desired data
           
           \return a pointer to a new gridded-field object that holds the data. The caller is
                   responsible for deleting this object, using the MetGridData remove() method.
      
      */
      GridLatLonFieldSfc* GetSfc( const std::string quantity, const std::string time );

      /// returns the first time available froma RUL
      /*! This method returns the first time stamp that is present in a given file.
          The URL is opened, if not already opened, and its times are read. It is then
          left open for further access.
          
          /param url the URL of the file to be opened. If the null string, then
                        the currently opene file (if any) is used.
         
          /return the time stamp of the first data snahot in the URL, in day1900 format.
      */
       double get_first_time1900( std::string &url );

       /// returns the last time available from a URL
       /*! This method returns the last time stamp that is present in a given file.
           The URL is opened, if not already opened, and its times are read. It is then
           left open for further access.
           
           /param url the URL of the file to be opened. If the null string, then
                         the currently opene file (if any) is used.
          
           /return the time stamp of the first data snahot in the URL, in day1900 format.       
      */
       double get_last_time1900( std::string &url );


   protected:

       
       /// the type of object this is
       static const string iam;

       /// copy a given object into this object
       /*! This method copies properties of a given MetaData object
           into this object.
      
           \param src the object to be copied
       */    
       void assign( const MetGEOSPortal& src );

       /// reset the internals
       /*! This method resets the internal state of this object.
       */
       void reset();

      /// returns a default set of vertical coordinate values
      /*! This method returns a vector of vertical coordinate values, depending on the coordinate system desired.
      
          \param coordSys a pointer ot a string giving the name of the vertical coordinate. If NULLPTR, the 
                        meteorological source's current vertical coordinate is used.
           \return a pointer to a vector of coordinate values corresponding to the usual or default values
                        for this data souirce on the desired coordinates.              
      
      */
      std::vector<real>* vcoords( const std::string *coordSys ) const;


      /// Converts the vertical coordinate of a GridData3D object 
      /*! For some data sources, a vertical coordinate quantity may be an 
          analytical function of another quantity. For example, pressure
          altitude is an analytical function of pressure.
          This method checks if that is the case and, if it is,
          converts the vertical coordinate.
          
          \param input a pointer to a GridData3D object 
          \param quant the quantity desired as the vertical coordinate
          \param units the units desired for the new vertical cooridnate
          \param scale the scale factor to take the new vertical coordinate into MKS units
          \param offset the offset to take the new vertical coordinate into MKS units
          \return a boolean that is true if the conversion was done, and false otherwise
      
      */
      bool vConvert( GridField3D *input, std::string quant, std::string units, real scale=1.0, real offset=0.0 );


       
      /// prepare for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.
          
          This has the side effect that it constructs an internal URL, which
          setup() does not necessarily do.

           \param quantity the name of the quantity desired
           \param time the valid-at time for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      virtual int prep(  const std::string quantity, const double time );    
  
      /// prepare for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.

          This has the side effect that it constructs an internal URL, which
          setup() does not necessarily do.

           \param quantity the name of the quantity desired
           \param time the valid-at datestamp string for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      virtual int prep(  const std::string quantity, const std::string &time );    



 
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
      virtual GridLatLonField3D* new_directGrid3D( const std::string quantity, const std::string time ) = 0;

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
      virtual GridLatLonFieldSfc* new_directGridSfc( const std::string quantity, const std::string time ) = 0;

      /// translate CF quantity names to GMAO GEOS names
      /*! This method translates quantity names according to the CF convention,
          into GEOS quantity names.

          \param quantity the desired physical quantity name using CF conventions
          
          \return the name of a physical quantity according
          to GEOS conventions.  
      */    
      virtual std::string quantityName_cf2geos(const std::string &quantity ) const = 0;

      /// translate GEOS Portal quantity names to CF names
      /*! This method translates quantity names according to the GEOS Portal,
          into CF quantity names.

          \param quantity the desired physical quantity name using GEOS Portal conventions
          
          \return the name of a physical quantity according
          to CF conventions. 
      */    
       virtual std::string quantityName_geos2cf(const std::string &quantity ) const = 0;

       /// queries a horizontal GEOS Portal grid 
       /*! GEOS Portal data products are available on several horizontal grid resolutions.
           This method returns characteristics of the horizontal grid queried for:  the set of 
           longitudes and latitudes used by that grid.

           \param qhgrid chooses one of these supported grids: 
                  - -1 = whatever current grid is being used
                  - 0 = native GEOS Portal resolution ((denoted by "N" in GEOS file naming conventions)
                  - 1 = GEOS Portal reduced resolution (denoted by "C" in GEOS file naming conventions)
                  - 2 = GEOS Portal reduced FV resolution (denoted by "F" in GEOS file naming conventions)
           \param lons the vector of longitudes returned
           \param lats the vector of latitudes returned       
       */
       void query_hgrid( int qhgrid, std::vector<real>& lons, std::vector<real>& lats ); 

       /// sets the horizontal GEOS Portal grid to be used
       /*! GEOS Portal data products are available on several horizontal grid resolutions.
           This method sets the horizontal grid to be used, and retrieves the set of 
           longitudes and latitudes used by that grid.

           \param hgrid0 chooses one of three supported grids: 
                  - 0 = native GEOS Portal resolution (0.3125 longitude by 0.25 latitude)
                  - 1 = GEOS Portal reduced resolution (2/3 longitude by 1/2 latitude)
                  - 2 = GEOS Portal reduced FV resolution (denoted by "F" in GEOS file naming conventions)
       */
       void set_hgrid( const int hgrid0 ); 

       /// queries a vertical GEOS Portal grid
       /*! GEOS Portal data products are available on several vertical grids (mainly, pressure
           levels and model sigma levels). This method queries the characterictics
           of a given vertical grid.
           retrieving the set of vertical levels used by that grid, along with the
           physical quantity of the vertical coordinate and its units.

           \param qvgrid chooses one of these supported grids: 
                  - -1 = use the current vertical grid
                  - 1 = native model levels ("v" in the GEOS file naming scheme)
                  - 2 = pressure levels ("p" in the GEOS file naming scheme)
                  - 3 = native model layer edges ("e" in the GEOS file naming scheme)
           \param zs the vector of coordinate values returned
           \param vquant the name of the vertical coordinate
           \param vunits the units of the vertical coordinate       
       */
       void query_vgrid( const int qvgrid, std::vector<real>& zs, std::string& vquant, std::string& vunits ); 

       /// sets the vertical GEOS Portal grid to be used
       /*! GEOS Portal data products are available on several vertical grids (namely, pressure
           levels and model sigma levels). This method set the vertical grid to be used and
           retrieves the set of vertical levels used by that grid, along wiht the
           physical quantity of the vertical coordinate and its units.

           \param vgrid0 chooses one of three supported grids: 
                  - 1 = native model levels; 
                  - 2 = pressure levels
                  - 3 = native model layer edges
       */
       void set_vgrid( const int vgrid0 ); 

      /// sets how strictly requested data should conform to the specified grid
      /*! GEOS Portal data are available on several horizontal and vertical grids, but not
          every meteorological field is available on every grid. By default, this class
          will obtain the field on the desired grid, and if it is not available on that
          grid then it will be read on a different grid and interpolated to the desired grid.
          
          This method sets flags that determine whether this interpolation is to be
          done, for the horizontal and vertical grids, as well as in time. If a flags is
          set to strict (1), then no interpolation will be done for that aspect
          of the grid.
          
         \param horiz set to 1 if strictness is required for the horizontal grid, 0 otherwise 
         \param vert  set to 1 if strictness is required for the vertical grid/coordinates, 0 otherwise 
         \param tspace set to 1 if strictness is required for the time spacing , 0 otherwise
         \param tavg set to 1 if strictness is required for time-averaging , 0 otherwise 
      */
      void setStrictness( const bool horiz, const bool vert, const bool tspace, const bool tavg );

      /// gets how strictly requested data must conform to the desired horizontal grid
      /*! This method returns how strictly requested data must conform to the desired horizontal grid.
      
          \return true if strict horizontal grid is current required when looking for data,
                  false if some laxness and interpolation are allowed.
      */    
      bool horizStrictness();      
      
      /// gets how strictly requested data must conform to the desired vertical grid
      /*! This method returns how strictly requested data must conform to the desired vertical grid.
      
          \return true if strict vertical grid is required when looking for data,
                  false if some laxness and interpolation are allowed.
      */    
      bool vertStrictness();      

      /// gets how strictly requested data must conform to the desired time spacing
      /*! This method returns how strictly requested data must conform to the desired time spacing.
      
          \return true if strict time spacing is required when looking for data,
                  false if some laxness and interpolation are allowed.
      */    
      bool tspaceStrictness();      

      /// gets how strictly requested data must conform to the desired time averaging
      /*! This method returns how strictly requested data must conform to the desired time averaging.
      
          \return true if strict time averaging is required when looking for data,
                  false if some laxness and interpolation are allowed.
      */    
      bool tavgStrictness();      
      
      /// sets the maximum number of open attempts
      /*! Because this class obtains GEOS Portal data products from an OPeNDAP server,
          data reads can sometimes fail because of network problems, an overloaded
          server at NASA, etc.  Consequently, several attempts may
          be made to open the URI. This method changes that number of attempts.
          (The default is five.)

          \param n the number of times to try.  Must be > 0.
      */
      void setNTries( const int n);
      
      /// gets the maximum number of netcdf operation attempts
      /*! Because this class obtains GEOS Portal data products from an OPeNDAP server,
          data reads can sometimes fail because of network problems, an overloaded
          server at NASA, etc.  Consequently,m several attempts will
          be made to open the URI. This method retrieves that number of attempts.

           \return the number of times a GEOS Portal URL open will be attempted before giving up.    
      */
      int nTries();
      
      /// sets the time interval between netcdf operation attempts
      /*! Because this class obtains GEOS Portal data products from an OPeNDAP server,
          data reads can sometimes fail because of network problems, an overloaded
          server at NASA, etc.  Consequently, several attempts may
          be made to open the URI, separated by a time interval. 
          This method changes that time interval.
          (The default is forty seconds.)

          \param interval the waiting period, in seconds
      */
      void setWaitTries( const unsigned int interval);
       
      /// gets the time interval between netcdf operation attempts
      /*! Because this class obtains GEOS Portal data products from an OPeNDAP server,
          data reads can sometimes fail because of network problems, an overloaded
          server at NASA, etc.  Consequently, several attempts may
          be made to open the URI, separated by a time interval. 
          This method retrieves that time interval.

          \return interval the waiting period, in seconds
      */
      unsigned int waitTries();
       
      /// sets the time interval between open attempts
      /*! to be polite to our server, we pause before each netcdf open.
          This method changes that time interval.
          (The default is one second.)

          \param interval the waiting period, in seconds
      */
      void setWaitOpen( const unsigned int interval);
       
      /// gets the time interval between open attempts
      /*! Because this class obtains GEOS Portal data products from an OPeNDAP server,
          data reads can sometimes fail because of network problems, an overloaded
          server at NASA, etc.  Consequently, several attempts may
          be made to open the URI, separated by a time interval. 
          This method retrieves that time interval.

          \return interval the waiting period, in seconds
      */
      unsigned int waitOpen();
       
  
      /// set up for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.

           \param quantity the name of the quantity desired
           \param time the valid-at time for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      virtual int setup(  const std::string quantity, const double time ) = 0;    
  
      /// set up for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.

           \param quantity the name of the quantity desired
           \param time the valid-at datestamp string for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      virtual int setup(  const std::string quantity, const std::string &time ) = 0;    
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
       virtual time_t expiration( const std::string &valid_at ) = 0;



       /// Object for dealing with Dates
       static const gigatraj::CalGregorian cal;
       
       /// base time from which all model times are referenced, in day1900 format.
       // (day1900 format is the number of days elapsed since 1900-01-01T00:00:00)
       double basetime;
       
       /// holds horizontal grid specifications
       class HGridSpec {
         public:
           /// File name code letter for the horizontal grid (' ', 'N', 'C', 'F')
           char code;
           /// horizontal grid:
           //   -1 = undetermined (code ' ')
           //    0 = 'N' native grid
           //    1 = 'C' reduced-resolution grid
           //    2 = 'F' special-purpose grid 
           int  id;
           /// starting longitude
           real startLon;
           /// ending longitude
           real endLon;
           /// longitude spacing
           real deltaLon;
           /// number of longitudes
           int   nLons;
           /// starting latitude
           real startLat;
           /// ending latitude
           real endLat;
           /// latitude spacing
           real deltaLat;
           /// number of latitudes
           int   nLats;
           /// factor by which to thin out lons and lats
           int thin;
           /// offset for thinning longitudes
           int thin_offset;
           
           /// constructor
           HGridSpec();
           
           /// destructor
           ~HGridSpec();
           
           /// clears the spec
           void clear();
           
       };
       
       /// holds vertical grid specifications
       class VGridSpec {
         public:
           /// file name code for vertical grid (' ', 'x', 'n', 'p', 'v')
           char code;
           /// vertical grid: 
           //   -1 = undetermined
           //    0 = 'x' 2D single-level data
           //    1 = 'n' native model levels
           //    2 = 'p' pressure levels
           //    3 = 'e' model layer edges
           int  id;
           
           /// quantity name
           std::string quant;
           /// units of the quantity
           std::string units;
           
           /// the set of vertical levels
           std::vector<real> levs;
           /// number of vertical levels
           int nLevs;
           
           /// constructor
           VGridSpec();
           
           /// destructor
           ~VGridSpec();
           
           /// clears the spec
           void clear();
           
       };
       
       
       /// holds time grid specifications
       class TGridSpec {
         public:
           /// time-averaging: 
           //   -1 = very long-term averaged data
           //    0 = instantaneous
           //    1 = one-hour average
           //    3 = three-hour average
           //    6 = six-hour average
           int tave;
           /// time offset of first data of day:
           //   -1 = undetermined
           //    0 = 00:00
           //   90 = 01:30
           int tbase;
           /// time interval:
           //   -1 = other
           //    0 = not applicable
           //    1 = one hour
           //    3 = three hours
           //    6 = six hours
           int tspace;
           /// the time of the first snapshot in the file, in day1900 format
           double start;
           /// the time of the last snapshot in the file, in day1900 format
           double end;
           /// the delta between successive timestaps, in days
           double delta;
           /// the number of time snapshots in the file
           int n;
           
           /// constructor
           TGridSpec();
           
           /// destructor
           ~TGridSpec();
           
           /// clears the spec
           void clear();
           
       };
       
       /// holds first/last/delta/size specs as read from a dimension of some numeric type
       typedef union spantrip {
          /// holds first/last/delta/size specs for integer data
          struct inttrip {
             /// first index
             int first;
             /// final index
             int last;
             /// index spacing
             int delta;
             /// number of points
             int size;
          } 
          /// name of the integer specifier struct
          intSpec;
          /// holds first/last/delta/size specs for float data
          struct floattrip {
             /// first index
             float first;
             /// final index
             float last;
             /// index spacing
             float delta;
             /// number of points
             int size;
          } 
          /// name of the float specifier struct
          floatSpec;
          /// holds first/last/delta/size specs for double data
          struct doubletrip {
             /// first index
             double first;
             /// final index
             double last;
             /// index spacing
             double delta;
             /// number of points
             int size;
          } 
          /// name of the double specifier struct
          doubleSpec;
       } SpanTriplet;
       
       /// holds attribute value in any of several formats
       typedef struct spanattr {
              /// netcdf data type
              nc_type type;
              /// netcdf data name
              std::string name;
              /// vector of data values (integer format)
              std::vector<int> ivalue;
              /// vector of data values (float format)
              std::vector<float> fvalue;
              /// vector of data values (double format)
              std::vector<double> dvalue;
              /// string data value
              std::string svalue;
       } SpanAttr;
       

       /// holds a list of known possible horizontal grid specifications       
       std::vector<HGridSpec> hspecs;
       /// holds a list of known possible vertical grid specifications
       std::vector<VGridSpec> vspecs;
       
       //--- grid preference specifiers   
       /// How strictly to stick to our gridding preferences
       Strictitude strict;
       /// horizontal grid  (-1 = unknown/flexible, 0="N", 1="C", 2="F" )     
       int desired_hgrid_id;    
       /// vertical grid ( -1=unknown/felxible, 0="x", 1="v", 2="p", 3="e" )  
       int desired_vgrid_id;       
       /// time-average period (-1=unknown/flexible, 0=no averaging, >0 is hrs averaged)
       int desired_tave;
       /// base time
       int desired_tbase;   
       /// time snapshot spacing (-1-unknown/flexible, >0 is hours of spacing)   
       int desired_tspace;
       
       
       
       /// skip factor in the horizontal grid (0=use every point, 1=use every other point, etc.
       int skip;
       /// longiutude offset factor to be used when skipping
       int skoff;
       
       /// number of times to try a given netcdf/OPeNDAP operation 
       int ntries;
       
       /// time to wait between tries (seconds)
       unsigned int waittry;

       /// time to wait betweeen url opens (to lessen the load on the server). If negative, it's a random time between zero and abs(openwait)
       int openwait;

       /// holds the remote file's legal variable names for longitude, latitude, vertical level, and time
       std::vector<std::string> legalDims;
              
       /// the URL last opened
       std::string cur_url;
       
       /// horizontal grid spec for the currently-opened remote file
       HGridSpec cur_hgrid;
       /// vertical grid spec for the currently-opened remote file
       VGridSpec cur_vgrid;
       /// time grid spec for the currently-opened remote file
       TGridSpec cur_tgrid;
              
       /// whether a URL is currently open
       int isOpen;
       
       // test values, for a remote file not yet opened
       /// test-probe quantity name
       std::string test_quant;
       /// test-probe date
       std::string test_date;
       /// test-probe access URL
       std::string test_url;
       /// test-probe dimensionality
       int test_ndims;
       /// test-probe horizontal grid
       int test_hgrid;
       /// test-probe vertical grid
       int test_vgrid;
       /// test-probe time spacing
       int test_tspace;
       /// test-probe time-averaging
       int test_tave;
       /// test-probe base time
       int test_tbase;

       /// a native base time form which times in the remote file are referenced, in their native format.
       double time_zero;

       /// on-the-fly calculator for theta (potential temperature)
       ThetaOTF gettheta; 
       /// on-the-fly calculator for the local time derivative of theta
       ThetaDotOTF getthetadot; 
       /// on-the-fly calculator for pressure
       PressOTF getpress; 
       /// on-the-fly calculator for pressure altitude
       PAltOTF getpalt;
       /// on-the-fly calculator for time rate of change of pressure altitude
       PAltDotOTF getpaltdot;
       
       // //// NETCDF-related variables:
       /// the netcdf handle
       int ncid;
       /// the number of variable in the netcdf file which are dimensions 
       int ndimens;
       /// the number of variable in the netcdf file
       int nvars;
       /// the number of lgobal attributes in the netcdf file
       int ngatts;
       /// the index of the unbounded dimension in the netcdf file, if any
       int unlimdim_idx;
       /// global attributes--string form
       std::map<std::string,std::string> gattr_strings;
       /// global attributes--real form
       std::map<std::string,real> gattr_reals;
       /// iterator for string global attributes
       std::map<std::string,std::string>::const_iterator gs_iter;
       /// iterator for real global attributes
       std::map<std::string,real>::const_iterator gr_iter;

       /// waits a given time interval
       /*! This method is a utility function for use in
           netcdf read operations. When accessing the GMAO data portal,
           temporary failures sometimes occur. Most of the time, waiting
           for a bit of time and and then re-trying the operation
           will succeed. You need want to try multiple times
           before the operation succeeds.
           
           This function takes two arguments. The first, "error", is
           compared to the netcdf library's NC_NOERR value.
           If it is equal to NO_NOERR, then false is returned.
           Otherwise, we consider that an error has occurred and we must decide
           whether to re-try the operation.
           The second argument, the trial number "trial" is incremented and compared to
           this object's ntries element. If lesz than ntries,
           the function will wait for the number of seconds in 
           (waittry*trial) seconds and then return true.
           Otherwise, the function will return false immediately.
           
           Thus, you can use this function like this:
           
           \code
               int trial, err;
               
               trial = 0;
               do {
                  err = nc_do_something(...);
               } while ( try_again( err, trial ) );
               if (err != NC_ERR ) {
                  throw(netcdfErrorHere());
               }
           \endcode
           
           \param error an error status form the calling routine. This is compared
                        to NC_NOERR.
           \param trial the trial number. This is typically initialized to 0 before
                        first attempting the operation.         

           \return true if an opeation should try again, false if not.

       */
       bool try_again( const int error, int &trial ) const;
           
     
       /// open the GEOS Portal url  
       /*! This method opens a GEOS Portal URL.

            \param url the OPeNDAP web address to be opened
       */
       void Portal_open(const std::string& url );
       /// close a GEOS Portal url
       /*! This method closes an open GEOS Portal OPeNDAP connection.
       */
       void Portal_close();
       /// load dimensional informaton from the GEOS Portal OPeNDAP server
       /*! This method loads dimensional informaton from the GEOS Portal OPeNDAP server.
           Dimensional information is stored in this object.
       
           \param nvdims the number of dimensions to retrieve
           \param dimids an nvdims-element array containing the indices of each GEOS Portal grid dimension 
       */
       void Portal_checkdims(const int nvdims, const int*dimids );

       /// converts time from the file internal format to day1900
       /*! This method converts a time value from the format used in the original remote data file,
           to day1900 (i.e., days elasped since 1900-01-01T00:00:00).
           
           \param nativeTime the time value, in file-native format
       */    
       virtual double Portal_time_nativeTo1900( double nativeTime ) = 0;

       /// converts a time interval from the file internal format to day1900
       /*! This method converts a time delta (interval) value from the format used in the original remote data file,
           to day1900 (i.e., days).
           
           \param nativeDelTime the time value, in file-native format 
       */    
       virtual double Portal_deltime_nativeTo1900( double nativeDelTime ) = 0;

       /// find the GEOS Portal time closest to the requested time
       /*! This method finds the data valid-at time on the OPeNDAP server that is
           close the desired time.
           
           \param desired_time the internal model time desired
           
           \return the index into the GEOS Portal time dimension that corresponds to the desired time.
                   If the nearest time is more than two minutes away from a GEOS Portal time value, an error
                   message is printed.
       
       */
       int Portal_findtime( const double desired_time ) const;



       /// get netcdf attributes of a given dimension
       /*! This method reads the netcdf attributes associated with a given dimension.
       
           \param dname (referenced but not actually used)
           \param var_id the dimension ID whose attributes are to be obtained
           \param nattrs the numnber of attributes
       
       */
       void Portal_dim_attrs(const std::string &dname, const int var_id, const int nattrs );


       /// get netcdf attributes of a given 3D variable
       /*! This method reads the netcdf attributes associated with a given 3D variable.
           Such attributes include the units and bad-or-missing-data fill values.
       
           \param var_id the netcdf variable ID
           \param nattrs the numnber of attributes
           \param datagrid a pointer to a GridLatLonField3D object into which the attributes will be loaded.
       
       */
       void Portal_getattrs(const int var_id, const int nattrs, GridLatLonField3D *datagrid );

       /// get netcdf attributes of a given 2D surface variable
       /*! This method reads the netcdf attributes associated with a given 2D surface variable.
           Such attributes include the units and bad-or-missing-data fill values.
       
           \param var_id the netcdf variable ID
           \param nattrs the numnber of attributes
           \param datagrid a pointer to a GridLatLonFieldSfc object into which the attributes will be loaded.
       
       */
       void Portal_getattrs(const int var_id, const int nattrs, GridLatLonFieldSfc *datagrid );

       /// reads an attribute of type chars into a string
       /*! This method reads a character-based attribute from an open netcdf file
              \param result the value of the attribute, returned as a string 
              \param attr_name the name of the attribute (null-terminated C string)
              \param varid the ID of the attribute (NC_GLOBAL for global attributes)
              \param len the number of characters expected in the attribute value
       */
       void Portal_read_attr( std::string& result, char *attr_name, int varid, size_t len );

       /// reads an attribute of type byte into a vector of unsigned chars
       /*! This method reads a byte-based attribute from an open netcdf file.
              \param result the value of the attribute, returned as a vector of unsigned chars
              \param attr_name the name of the attribute (null-terminated C string)
              \param varid the ID of the attribute (NC_GLOBAL for global attributes)
              \param len the number of bytes expected in the attribute value
       */
       void Portal_read_attr( std::vector<unsigned char> &result, char *attr_name, int varid, size_t len );

       /// reads an attribute of type short into a vector of shorts
       /*! This method reads a short attribute from an open netcdf file.
              \param result the value of the attribute, returned as a vector of shorts
              \param attr_name the name of the attribute (null-terminated C string)
              \param varid the ID of the attribute (NC_GLOBAL for global attributes)
              \param len the number of short expected in the attribute value; only the first one is returned.
       */
       void Portal_read_attr( std::vector<short> &result, char *attr_name, int varid, size_t len );

       /// reads an attribute of type int into a vector of ints
       /*! This method reads an int attribute from an open netcdf file.
              \param result the value of the attribute, returned as a vector of ints
              \param attr_name the name of the attribute (null-terminated C string)
              \param varid the ID of the attribute (NC_GLOBAL for global attributes)
              \param len the number of short expected in the attribute value; only the first one is returned.
       */
      void Portal_read_attr( std::vector<int> &result, char *attr_name, int varid, size_t len );

       /// reads an attribute of type float into a vector of floats
       /*! This method reads a float attribute from an open netcdf file.
              \param result the value of the attribute, returned as a vector of floats
              \param attr_name the name of the attribute (null-terminated C string)
              \param varid the ID of the attribute (NC_GLOBAL for global attributes)
              \param len the number of floats expected in the attribute value; only the first one is returned.
       */
       void Portal_read_attr( std::vector<float> &result, char *attr_name, int varid, size_t len );

       /// reads an attribute of type double into a vector of doubles
       /*! This method reads a double attribute from an open netcdf file.
              \param result the value of the attribute, returned as a vector of doubles
              \param attr_name the name of the attribute (null-terminated C string)
              \param varid the ID of the attribute (NC_GLOBAL for global attributes)
              \param len the number of doubles expected in the attribute value; only the first one is returned.
       */
       void Portal_read_attr( std::vector<double> &result, char *attr_name, int varid, size_t len );


       /// reads a dimension from an open remote file
       /*! This method reads dimensional values from an open remote file.
           It actually reads just the first and last values and determines
           the delta between successive dimensional values.
           
           \param dim_name the name of the dimension to be read
           
           \param dim_type the numeric type of the dimensional values: 
                  NC_DOUBLE, NC_FLOAT, or NC_INT.
           
           \param span a union that holds the first dimensional value (.first), 
                       the last dimensional value (.last),
                       the delta between successive values (.delta),
                       and the number of values (.size).
                       There are three elements in the union:
                       intSpec for values of type int,
                       floatSpec for values of type float,
                       and doubleSpec for values of type double.
                       Each in turn has the elements lists above: 
                       first, last, delta, and size. Thus, if the
                       return type is NC_DOUBLE, then you would
                       access the delta value using span.doubleSpec.delta.
                       
       */
       void Portal_read_dim( std::string &dim_name, nc_type &dim_type, SpanTriplet &span);

       /// reads a dimension from an open remote file
       /*! This method reads dimensional values from an open remote file.
       
           \param dim_name the name of the dimension to be read
           \param vals (output) a vector of dimensional values. All numeric types
                       are converted to type real.
       */
       void Portal_read_dim( std::string &dim_name, std::vector<real> &vals);

       /// updates the horizontal grid
       /*! This method updates the met source's horizonal grid information,
           based on characteristics in cur_hgrid, which comes from the current open file.
           This affects the id of cur_hgrid, as well as the lons and lats elements
           of this object,
       */    
       void update_hgrid();
       
       /// updates the vertical grid
       /*! This method updates the met source's vertical grid information,
           based on characteristics in cur_vgrid, which comes from the current open file.
           This affects the id of cur_vgrid, as well as the native_zs and native_nzs elements
           of this object,
       */    
       void update_vgrid();
       /// updates the time grid
       /*! This method updates the met source's time grid information,
           based on characteristics in cur_vgrid, which comes from the current open file.
           This affects tbase and tspace of cur_tgrid.
       */    
       void update_tgrid();
       
       /// reads the standard set of dimension from an open remote file
       /*! This method reads the time, lon, lat, and (if present) lev dimensions
           from an open remote file.
           This results in the cur_hgrid, cur_vgrid, and cur_tgrid obejcts
           all being set to values consistent with the characteristics of
           the open file.
       */    
       void Portal_read_all_dims();


       /// reads a key set of double dimension values
       /*! This method reads the first and last value of a set of dimensional values;
              \param first (output) the first dimensional value
              \param last (output) the last dimensional value
              \param delta (output) the delta betweel two successive dimensional values
              \param varid the ID of the dimension variable
              \param len the number of doubles expected in the dimension
       */
       void Portal_read_dim_doubles( double &first, double &last, double &delta, int varid, size_t len );

       /// reads a set of double dimension values
       /*! This method reads a set of double dimension values (longitude, latitude, vertical level) from an open netcdf file
              \param vals returned vector of reals containing the dimensional values
              \param varid the ID of the dimension variable
              \param len the number of doubles expected in the dimension
       */
       void Portal_read_dim_doubles( std::vector<real>&vals, int varid, size_t len );

       /// reads a key set of float dimension values
       /*! This method reads the first and last value of a set of dimensional values;
              \param first (output) the first dimensional value
              \param last (output) the last dimensional value
              \param delta (output) the delta betweel two successive dimensional values
              \param varid the ID of the dimension variable
              \param len the number of floats expected in the dimension
       */
       void Portal_read_dim_floats( float &first, float &last, float &delta, int varid, size_t len );

       /// reads a set of float dimension values
       /*! This method reads a set of float dimension values (longitude, latitude, vertical level) from an open netcdf file
              \param vals returned vector of reals containing the dimensional values
              \param varid the ID of the dimension variable
              \param len the number of floats expected in the dimension
       */
       void Portal_read_dim_floats( std::vector<real>&vals, int varid, size_t len );

       /// reads a key set of int dimension values
       /*! This method reads the first and last value of a set of dimensional values;
              \param first (output) the first dimensional value
              \param last (output) the last dimensional value
              \param delta (output) the delta betweel two successive dimensional values
              \param varid the ID of the dimension variable
              \param len the number of ints expected in the dimension
       */
       void Portal_read_dim_ints( int &first, int &last, int &delta, int varid, size_t len );

       /// reads a set of int dimension values
       /*! This method reads a set of int dimension values (longitude, latitude, vertical level) from an open netcdf file
              \param vals returned vector of reals containing the dimensional values
              \param varid the ID of the dimension variable
              \param len the number of ints expected in the dimension
       */
       void Portal_read_dim_ints( std::vector<real>&vals, int varid, size_t len );

       /// reads a variable's float-format data
       /*! This method reads a data variable from an open netcdf file.
              \param vals the returned vector of data, as a vectors of floats in row-major order
              \param var_id the netcdf variable id of the data to be read
              \param ndims 3 for three-dimensional (lon,lat,level) data, 2 for two-dimensional (lon, lat) data.
              \param starts a 3- or 2-element array of starting-indices into the data variable. 
                     For 3D data, the indices are, in order: time (0), level (1), latitude (2), and longitude (3).
                     For 3D data, the indices are: time (0), latitude (1), and longitude (2).
                     (This reflects the column-major order of storage on the remote end.)
                     Typically, all elements are zero.
              \param counts a 3- or 2-element array of how many value are to be read from the data variable.
                     The index order is the same as for starts[].
                     Typically, the 0th elements is 0, as only one time snapshot is read,
                     and the other elements are the lengths of their respective dimensions.
              \param strides a 3- or 2-element array of how many values are to be skipped along each dimension.
                     The index order is the same as for starts[] and counts[].
                     Typically all elements are 1.        
       */
       void Portal_read_data_floats( std::vector<real>&vals, int var_id, int ndims, size_t *starts, size_t *counts, ptrdiff_t *strides );



       /// read just the desired 3D variable from the OPeNDAP server
       /*! This method reads just the desired 3D variable from the OPeNDAP server.
       
           \param quantity the name of the meteorological fields to be read
           \param time the internal model time for which the data are desired
           \param grid3d a pointer to a GridLatLonField3D object into which the data are to be loaded
       
       */
       void Portal_getvar(const std::string quantity, const double time, GridLatLonField3D* grid3d );

       /// read just the desired 2D surface ariable from the OPeNDAP server
       /*! This method reads just the desired 2D surface variable from the OPeNDAP server.
       
           \param quantity the name of the meteorological fields to be read
           \param time the internal model time for which the data are desired
           \param grid2d a pointer to a GridLatLonFieldSfc object into which the data are to be loaded
       
       */
       void Portal_getvar(const std::string quantity, const double time, GridLatLonFieldSfc* grid2d );


       /// read a 3D meteorological field from the OPeNDAP server
       /*! This method reads the desired 3D quantity from the OPeNDAP server.
           It sets up default attributes, handles opening and closing ULSs, and 
           calls the Portal_getvar() method, 
       
           \param quantity the name of the meteorological fields to be read
           \param time the internal model time for which the data are desired
           \param grid3d a pointer to a GridLatLonField3D object into which the data are to be loaded
       
       */
       void readPortal( const std::string quantity, const std::string time, GridLatLonField3D* grid3d );

       /// read a 2D surface meteorological field from the OPeNDAP server
       /*! This method reads the desired 2D surface quantity from the OPeNDAP server.
           It sets up default attributes, handles opening and closing ULSs, and 
           calls the Portal_getvar() method, 
       
           \param quantity the name of the meteorological fields to be read
           \param time the internal model time for which the data are desired
           \param grid2d a pointer to a GridLatLonFieldSfc object into which the data are to be loaded
       
       */
       void readPortal( const std::string quantity, const std::string time, GridLatLonFieldSfc* grid2d );

       
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
