#ifndef GIGATRAJ_METGRIDLATLONDATA_H
#define GIGATRAJ_METGRIDLATLONDATA_H

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/MetGridData.hh"

namespace gigatraj {

/*!
\ingroup MetSrc
\brief abstract class for a longitude-latitude-vertical gridded meteorological field data source

The MetGridLatLonData class is an abstract subclass of MetData. If is a little less
abstract than that class, because it deals specifically with rectangular latitude-longitude
grids in the horizontal, holding its data in objects of the GridLatLonField3D and
GridLatLonFieldSfc classes.  The MetGridLatLonData class abstracts out the 
common functionality that applies to lon-lat gridded data---which is most
data sources.

Subclasses which inherit from this class are expected to provide their
own methods new_directGrid3D() and new_directGridSfc() for reading met data, as well
as certain other methods for dealing with naming conventions for
meteorological fields and data cache files.


*/

class MetGridLatLonData : public MetGridData {

   public:

      /// Error: a surface was requested that is undefined
      class badsurface {};


      /// Default constructor
      /*! This is the default constructor. 
      */
      MetGridLatLonData();
      
      
      /// Full Constructor 
      /*! This is the full constructor. 

           \param quantity the desired quantity to be used for the vertical coordinate
           \param units the units of the desired vertical coordinate
           \param levels a pointer to a vector of values of the new vertical coordinate
           \param lons vector of gridpoint longitudes
           \param lats vector of gridpoint latitudes
      */
      MetGridLatLonData(
        const std::string quantity, const std::string units
      , const std::vector<real>& levels
      , const std::vector<real>& lons
      , const std::vector<real>& lats
      );  


      /// destructor
      /*! This is the destructor. 
      */
      ~MetGridLatLonData();

      
      /// copy constructor
      /*! This is the copy constructor. 
      */
      MetGridLatLonData( const MetGridLatLonData& src );

      /// assignment operator
      virtual MetGridLatLonData& operator=(const MetGridLatLonData& src) = 0;

      /// assignment operator
      MetGridData& operator=(const MetGridData& src);

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
      virtual void setOption( const std::string &name, const std::string &value );

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
      virtual void setOption( const std::string &name, int value );

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
      virtual void setOption( const std::string &name, float value );

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
      virtual void setOption( const std::string &name, double value );

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
      virtual bool getOption( const std::string &name, std::string &value );


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
      virtual bool getOption( const std::string &name, int &value );


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
      virtual bool getOption( const std::string &name, float &value );

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
      virtual bool getOption( const std::string &name, double &value );




     /// obtain the value of the zonal wind component at a given point, in m/s
     /*! This method obtains the value of the zonal wind component at a given point.
         The return value is guaranteed to be in meters per second, regardless of 
         the units used in the original data.

         \param time the model time for which data is to be returned  
         \param lon the longitude at which data is to be returned
         \param lat the latitude at which data is to be returned
         \param z the vertical coordinate value at which the data is to be returned.
      
         \return the wind value, in m/s
     
      */
      real get_u( double time, real lon, real lat, real z);


     /// obtain the value of the meridional wind component at a given point, in m/s
     /*! This method obtains the value of the meridional wind component at a given point.
         The return value is guaranteed to be in meters per second, regardless of 
         the units used in the original data.

         \param time the model time for which data is to be returned  
         \param lon the longitude at which data is to be returned
         \param lat the latitude at which data is to be returned
         \param z the vertical coordinate value at which the data is to be returned.
     
         \return the wind value, in m/s
      
      */
      real get_v( double time, real lon, real lat, real z);


     /// obtain the value of the vertical wind component at a given point, in m/s
     /*! This method obtains the value of the vertical wind component at a given point.
         The return value is guaranteed to be in meters per second, regardless of 
         the units used in the original data.

         \param time the model time for which data is to be returned  
         \param lon the longitude at which data is to be returned
         \param lat the latitude at which data is to be returned
         \param z the vertical coordinate value at which the data is to be returned.
     
         \return the wind value, in m/s
     
      */
      real get_w( double time, real lon, real lat, real z);


     /// obtain the values of all three wind components at a given point, in m/s
     /*! This method obtains the value of the zonal, meridional, and vertical wind component at a given point.
         The return value is guaranteed to be in meters per second, regardless of 
         the units used in the original data.

         \param time the model time for which data is to be returned  
         \param lon the longitude at which data is to be returned
         \param lat the latitude at which data is to be returned
         \param z the vertical coordinate value at which the data is to be returned.
         \param u the returned zonal wind value
         \param v the returned meridional wind value
         \param w the returned vertical wind value
     
      */
      void get_uvw( double time, real lon, real lat, real z, real *u, real *v, real *w);

     /// obtain the values of all three wind components for arrays of given points, in m/s
     /*! This method obtains the values of the zonal, meridional, and vertical 
         wind component at set of given points.
         The return values are guaranteed to be in meters per second, regardless of 
         the units used in the original data. Invalid (bad or missing data) values are set to Not-a-Number (NaN).

         \param time the model time for which data is to be returned  
         \param n the number of elements in the lons, lats, and zs arrays.
         \param lons a pointer to an array of longitudes at which data are to be returned
         \param lats a pointer to an array of latitudes at which data is are be returned
         \param zs a pointer to an array of vertical coordinate values at which the data are to be returned
         \param u a pointer to an array for the returned zonal wind values.
         \param v a pointer to an array for the returned meridional wind values
         \param w a pointer to an array for the returned vertical wind values
     
      */
      void get_uvw( double time, int n, real* lons, real* lats, real* zs, real *u, real *v, real *w);

      /// obtain the value of a meteorological field at a given point 
      /*! This method obtains the value of a meteorological field at a given point. 
          Unlike the get_u(), get_v(), get_w(), and get_uvw()
          methods, quantities returned by getData() are not guaranteed to be in MKS units.
      
          \param quantity a string that  holds the name of the desired quantity.      
          \param time the model time for which data is to be returned  
          \param lon the longitude at which data is to be returned
          \param lat the latitude at which data is to be returned
          \param z the vertical coordinate value at which the data is to be returned.
                    If the quantity is an inherently two-dimensional quasihorizontal surface,
                    then this parameter is ignored.
          \param flags flag values:
                  METDATA_MKS = convert the return value to MKS units
                  METDATA_FAILBAD = throw an error if the value to be returned is invalid.
                  METDATA_NANBAD = return a NaN value if the value to be returned is invalid.
                  METDATA_INFBAD = return a value of infinity if the value to be returned is invalid.
                  METDATA_KEEPBAD = return an invalid value as-is.

          \return the value of the quantity at the given location and time

      */
      real getData( string quantity, double time, real lon, real lat, real z, int flags=0);

      /// obtain the value of a meteorological field at a set of given points 
      /*! This method obtains values of a meteorological field, given arrays of given points. 
          Unlike the get_u(), get_v(), get_w(), and get_uvw()
          methods, quantities returned by getData() are not guaranteed to be in MKS units.
      
          \param quantity a string that  holds the name of the desired quantity.      
          \param time the model time for which data is to be returned  
          \param n  the number of points at which values are desired
          \param lons a pointer to an array of longitudes at which data are to be returned
          \param lats a pointer to an array of latitudes at which data are to be returned
          \param zs a pointer to an array of vertical coordinate values at which the data are to be returned.
                    If the quantity is an inherently two-dimensional quasihorizontal surface,
                    then this parameter is ignored.
          \param values a pointer to an array of values of the quantity at the given location and time
          \param flags flag values:
                  METDATA_MKS = convert the return value to MKS units
                  METDATA_FAILBAD = throw an error if the value to be returned is invalid.
                  METDATA_NANBAD = return a NaN value if the value to be returned is invalid.
                  METDATA_INFBAD = return a value of infinity if the value to be returned is invalid.
                  METDATA_KEEPBAD = return an invalid value as-is.

      */
      void getData( string quantity, double time, int n, real* lons, real* lats, real* zs, real* values, int flags=0 );
      
      /// obtain the value of a meteorological vector field at a given point 
      /*! This method obtains the value of a meteorological vector field at a given point.
          Because of the sphericity of the earth, special procedures should be
          applied when interpolating near the poles. (Components of a vector
          quantity are not scalar quantities!) Depending on compiler definition settings,
          this routine may apply such procedures. 
          Unlike the get_u(), get_v(), get_w(), and get_uvw()
          methods, quantities returned by getData() are not guaranteed to be in MKS units.
      
          \param lonquantity a string that  holds the name of vector quantity's zonal component.      
          \param latquantity a string that  holds the name of vector quantity.'s meridional component      
          \param lonval reference to the returned zonal vector quantity, interpolated to the given position
          \param latval reference to the returned meridional vector quantity, interpolated to the given position
          \param time the model time for which data is to be returned  
          \param lon the longitude at which data is to be returned
          \param lat the latitude at which data is to be returned
          \param z the vertical coordinate value at which the data is to be returned.
                    If the quantity is an inherently two-dimensional quasihorizontal surface,
                    then this parameter is ignored.
          \param flags flag values:
                  METDATA_MKS = convert the return value to MKS units
                  METDATA_FAILBAD = throw an error if the value to be returned is invalid.
                  METDATA_NANBAD = return a NaN value if the value to be returned is invalid.
                  METDATA_INFBAD = return a value of infinity if the value to be returned is invalid.
                  METDATA_KEEPBAD = return an invalid value as-is.


      */
      void getVectorData( string lonquantity, string latquantity, real &lonval, real &latval, double time, real lon, real lat, real z, int flags=0);

      /// obtain the value of a meteorological vector field at an array of given points 
      /*! This method obtains the value of a meteorological vector field at an array of given points.
          Because of the sphericity of the earth, special procedures should be
          applied when interpolating near the poles. (Components of a vector
          quantity are not scalar quantities!) Depending on compiler definition settings,
          this routine may apply such procedures. 
          Unlike the get_u(), get_v(), get_w(), and get_uvw()
          methods, quantities returned by getData() are not guaranteed to be in MKS units.
      
          \param n the number of given points (i.e, the length of the arrays)
          \param lonquantity a string that  holds the name of vector quantity's zonal component.      
          \param latquantity a string that  holds the name of vector quantity.'s meridional component      
          \param lonvals the array of returned zonal vector quantity values, interpolated to each given position
          \param latvals the array of returned meridional vector quantity values, interpolated to each given position
          \param time the model time for which data are to be returned  
          \param lons the longitudes at which data are to be returned
          \param lats the latitudes at which data are to be returned
          \param zs the vertical coordinate values at which the data are to be returned.
                    If the quantity is an inherently two-dimensional quasihorizontal surface,
                    then this parameter is ignored.
          \param flags flag values:
                  METDATA_MKS = convert the return value to MKS units
                  METDATA_FAILBAD = throw an error if the value to be returned is invalid.
                  METDATA_NANBAD = return a NaN value if the value to be returned is invalid.
                  METDATA_INFBAD = return a value of infinity if the value to be returned is invalid.
                  METDATA_KEEPBAD = return an invalid value as-is.


      */
      void getVectorData( int n, string lonquantity, string latquantity, real* lonvals, real *latvals, double time, real* lons, real* lats, real* zs, int flags=0);

      /// get the default tne delta between field snapshots
      /*! This method returns the default time spacing. Because this may depend on the field
          and on the time for which the fueld is desired, those are required parameters.
          
          \param quantity the name of the field whose time spacing is to ve determined
          \param caltime the (internal model) time near which the time spacing is desired.
          \return the time spacing, in hours
      */
      double get_default_timedelta( const std::string &quantity, const std::string &caltime );


     /// reconciles one 3D gridded data object to another
     /*! This method interpolates the data from one gridded data object to match the grid of another.
           \param dat1 the GridLatLonField3D object whose data is to be interpolated
           \param dat2 a GridLatLonField3D on whose gridpoints the data are to be interpolated
           \return a pointer to a new GridLatLonField3D field with the interpolated data. The calling routine
                   is responsible to removing this object when it is no longer needed.
     */
     GridLatLonField3D* reconcile( const GridLatLonField3D  &dat1, const GridLatLonField3D  &dat2 );            

     /// reconciles one 2D gridded data object to another
     /*! This method interpolates the data from one gridded data object to match the grid of another.
           \param dat1 the GridLatLonFieldSfc object whose data is to be interpolated
           \param dat2 a GridLatLonFieldSfc on whose gridpoints the data are to be interpolated
           \return a pointer to a new GridLatLonFieldSfc field with the interpolated data. The calling routine
                   is responsible to removing this object when it is no longer needed.
     */
     GridLatLonFieldSfc* reconcile( const GridLatLonFieldSfc  &dat1, const GridLatLonFieldSfc  &dat2 );            

   protected:

       /// number of grid longitudes 
       int nlons;
       /// vector of grid longitudes
       std::vector<real> lons;
       
       /// number of grid latitudes
       int nlats;
       /// vector of grid latitudes
       std::vector<real> lats;
       
       /// the name of native grid levels (not necessarily the chosen vertical grid's levels)
       std::string native_vquant;
       /// the units if the native vquant
       std::string native_vuu;
       /// the number of native grid levels 
       int native_nzs;
       /// the vector of native grid levels
       std::vector<real> native_zs;
       
      /// copy a given object into this object
      /*! This method copies properties of a given MetaData object
          into this object.
      
          \param src the object to be copied
      */    
       void assign( const MetGridLatLonData& src );
 

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
      virtual bool bracket( const std::string &quantity, double time, double *t1, double *t2) = 0;


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
      virtual bool bracket( const std::string &quantity, const std::string &time, std::string *t1, std::string *t2) = 0;

      /// generate a fle path to a cache file for a gridded met field
      /*! This method returns a pointer to a full file path for a disk cache file, or NULL
          if the data are not to be cached on disk.

          \param item the GridFieldSfc data object which is to be cached

          \return the cache file path name
      */
      virtual FilePath* cachefile( const GridFieldSfc* item ) const = 0;

      /// generate a fle path to a cache file for a gridded met field
      /*! This method returns a pointer to a full file path for a disk cache file, or NULL
          if the data are not to be cached on disk.

          \param item the GridField3D data object which is to be cached
          
          \return the cache file path name
      */
      virtual FilePath* cachefile( const GridField3D* item ) const = 0;

      /// write an MetGridField3D object to disk cache
      /*! This method writes a MetGridField3D object to disk cache

          \param item the GridField3D data object which is to be cached
      */
      void writeCache( const GridField3D* item ) const;
      
      /// write an MetGridFieldSfc object to disk cache
      /*! This method writes a MetGridFieldSfc object to disk cache

          \param item the GridFieldSfc data object which is to be cached
      */
      void writeCache( const GridFieldSfc* item ) const;

      ///  read a MetGridField3D object from disk cache
      /*!  This method reads a MetGridField3D object from disk cache.

           \param quantity the name of the quantity desired
           \param time the valid-at datestamp string for which data is desired

           \return a pointer to a GridField3D object that holds the data. the calling routine
           is thus responsible to dleteing the new object, usually with the MetGridData remove()
           method.
      */
      GridField3D* readCache3D( const std::string quantity, const std::string time );
      
      /// read an MetGridFieldSfc object from disk cache
      /*! This method reads an MetGridFieldSfc object from disk cache.

           \param quantity the cf-convention name of the quantity desired.  If the
                  desired quanity is on a specified surface, then a "@" is appended,
                  followed by the surface.  For example, "total column ozone"
                  might be simply "total column ozone", but "tropopause temperature"
                  might be "temperature@tropopause".
           \param time the valid-at datestamp string for which data is desired

           \return a pointer to a GridFieldSfc object that holds the data. the calling routine
           is thus responsible to dleteing the new object, usually with the MetGridData remove()
           method.
      */
      GridFieldSfc* readCacheSfc( const std::string quantity, const std::string time );

      /// set up for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.
          Because this function is used independently of the caching mechanism, 
          individual subclass implementations should not actually open access to 
          original data sources (files, OPeNDAP servers, etc.), since that
          would partially defeat the purpose of any caching that might be done.

           \param quantity the name of the quantity desired
           \param time the valid-at time for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      virtual int setup(  const std::string quantity, const double time );    
  
      /// set up for data access
      /*! Given a quantity and time, this method sets up any internal parameters that 
          may be used repeatedly during the course of data access.
          Because this function is used independently of the caching mechanism, 
          individual subclass implementations should not actually open access to 
          original data sources (files, OPeNDAP servers, etc.), since that
          would partially defeat the purpose of any caching that might be done.

           \param quantity the name of the quantity desired
           \param time the valid-at datestamp string for which data is desired
           \return  the number of dimensions of a data object (i.e, 2 or 3)
      */
      virtual int setup(  const std::string quantity, const std::string &time );    


      /// (parallel processing) gets a 3D field valid at a certain time, as a client of a met data sertver
      /*! This method contacts a meteorological data server to obtain a new gridded data object,
          instead of reading the data itself. 
          
          To conserve memory, the GridField object produced by this method
          will contain only the metadtaa, including dimensional values, but no actual
          data values. Thus the value() method and related methods will not work with the
          resulting object. Instead, the met source's request_data3D() method should be called
          to alert the met server that data will soon be required, and then the 
          object's gridpoints() method should be called to obtain data values on
          a sets of gridpoints, by asking the met server for them.

           \param quantity the (internal or cf-convention) name of the quantity desired
           \param time the valid-at datestamp string for which data is desired
           \return a pointer to a GridField3D object that holds the metadata. 
           This object
           may be cached, or it may be a met client in a multiprocessing environment.
           Consequently, when the calling routine has no further use for the object
           it should use the MetGridData remove() method to delete it, instead of deleting it
           itself.

      */
      GridField3D* new_clientGrid3D( const std::string& quantity, const std::string& time );
          
      /// (parallel processing) gets a 2D field valid at a certain time, as a client of a met data sertver
      /*! This method contacts a meteorological data server to obtain a new gridded data object,
          instead of reading the data itself. 
          
          To conserve memory, the GridField object produced by this method
          will contain only the metadtaa, including dimensional values, but no actual
          data values. Thus the value() method and related methods will not work with the
          resulting object. Instead, the met source's request_dataSfc() method should be called
          to alert the met server that data will soon be required, and then the 
          object's gridpoints() method should be called to obtain data values on
          a sets of gridpoints, by asking the met server for them.

           \param quantity the (internal or cf-convention) name of the quantity desired
           \param time the valid-at datestamp string for which data is desired
           \return a pointer to a GridFieldSfc object that holds the metadata. 
           This object
           may be cached, or it may be a met client in a multiprocessing environment.
           Consequently, when the calling routine has no further use for the object
           it should use the MetGridData remove() method to delete it, instead of deleting it
           itself.

      */
      GridFieldSfc* new_clientGridSfc( const std::string& quantity, const std::string& time );
          


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
