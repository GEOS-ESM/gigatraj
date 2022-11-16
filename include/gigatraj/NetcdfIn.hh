#ifndef GIGATRAJ_NETCDFIN_H
#define GIGATRAJ_NETCDFIN_H

#include <netcdf.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>


#include "gigatraj/gigatraj.hh"
#include "gigatraj/ParcelInitializer.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/Flock.hh"
#include "gigatraj/Swarm.hh"
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

/*!
\ingroup parcelinitializers

\brief loads Parcel state information from a netcdf file that follows certain conventions.

The NetcdfIn class reads information about Parcels from a netcdf4 file.
The file must have at least one dimensional variable: an integer parcel ID number, 
called "parcelID". (The information in this variable is not actually used.)
The file must also have data variables "lon" and "lat".
If must also have at least one variable with the "positive" attribute, which
denotes a vertical level. There should be one of these variables
with the attribute :"vertical _coordinate", and that attribute should
be set to "yes". That data varibale is taken as the vertical cooridnate
of the Parcels.

The file may have a second dimensional variable, "time". If so, then each of 
the file's data variables is a 2D variable. By default, Parcels from the first 
timestep (not necessarily the earliest in time, but the zeroeth element of the
time array) is used.


*/

class NetcdfIn : public ParcelInitializer {


   public:
   
      /// An exception for failing to open the netcdf file
      class badNetcdfOpen {
         public:
            /// error code
            int error;
            
            /// constructor
            badNetcdfOpen(int err) { error = err; }
            
      };

      /// An exception for some miscellaneous error while trying to read the netcdf file
      class badNetcdfError {
         public:
            /// error code
            int error;
            /// constructor
            badNetcdfError(int err) { error = err; }
            
      };
      
      /// An exception for when the input does not match the format
      class badFileConventions: public ParcelInitializer::badinit {};

   /// default constructor
   /*! this is the default constrictor for the NetcdfIn class
   
   */
   NetcdfIn();
   
   /// destructor
   /*!
       This is the destructor for the NetcdfIn class
   */
   ~NetcdfIn();



   /// sets the netcdf file name
   /*! This method sets the netcdf file name.
   
       \param file the name of the netcdf file to be opened for input
   */
   void filename( std::string file );
   
   /// returns the name of the netcdf file
   /*! This method returns the name of the netcdf file.
   
       \return a reference to a string object that contains the name of the netcdf file
   */
   std::string& filename();




   /// sets the vertical quantity to use
   /*! This method sets which variable in the file to use as the vertical coordinate
   
       \param vert the name of the vertical quantity in the netcdf file. If this is the empty 
                   string (the default), then the variable with the attribute "vertical_coordinate" set
                   to "yes" will be used.
   */
   void vertical( std::string vert );
   
   /// returns the name of the vertical coordinate to be used
   /*! This method returns the name of variable in the netcdf file that is to be used
       as the vaetical coordinate.
   
       \return a reference to a string object that contains the name of the vertical coordinate
   */
   std::string& vertical();



   /// open a netcdf file for input
   /*! This method opens a netcdf file for input
   
       \param file the name of the file to open.
   */
   void open( std::string file="" );    

   /// closes the netcdf file
   /*! this method clsoes the netcdf file
   */
   void close();
   
   
   /// returns the base calendar time read from the file
   /*! This method returns the base calendar time read from the netcdf file.
       This is the time that corresponds to the model time 0.
       It is not necessarily the time at which the parcels are initialized.
       
       \return the calendar base time string
   */    
   std::string& cal();
   
   /// returns the parcel model time read from the file
   /*! This method returns the parcel time read from the netcdf file.
       This is either the beginning or the endoing time from the netcdf file.
       
       \return the time
   */    
   double time();

    /// sets which time 
    /*! This method sets whether the parcel time should be read from the end of the netcdf file.
        The default is false, so the first time in the file is used.
    */    
    void at_end( bool select );
     
    /// resturns which time 
    /*! This method returns whether the parcel time will be read from the end of the netcdf file.
    */    
    bool at_end();
     
    /// returns the number of parcels
    /*! This method returns the number of parcels in the netcdf file
    
         \return the number of parcels
    */
    size_t n_parcels();
    
    /// returns the index of the next Parcel to be read from the file
    /*! This method returns the index of the next Parcel to be read
        from the netcdf file.
        
        \return the index of the next Parcel. If all the Parcels have already been read, -1 is returned.
    */
    int next_parcel();
    
    /// resets the parcel index
    /*! This method sets the next-parcel index to zero.
    
    */
    void reset();
    
    /// sets debug mode
    /*! This method sets the debugging mode
    
         \param mode the debugging mode. 0 = no diagnostic messages. 
                     The higher the mode number, the more diagnostic messages are printed to std::cerr
                      
    */
    void debug( int mode );
    
    /// returns the debug mode
    /*! This method returns the current debug mode setting
    
        \return the current debug mode setting
    */
    int debug();
    
        
      /// Initialize a single Parcel
      /*! This method initializes a single Parcel object.
      
          \param p the Parcel object to be initialized

      */
      void apply( Parcel& p ); 

      /// Initialize an array of Parcels
      /*! This method initializes an array of Parcels.
      
          \param p the array of Parcel objects to be initialized
           
          \param n the number of Parcel objects to be initialized

      */
      void apply( Parcel * p, const int n ); 

      /// Initialize a vector of parcels
      /*! This method initializes a vector of Parcels
      
          \param p the vector of Parcel objects to be initialized
           
      */
      void apply( std::vector<Parcel>& p ); 

      /// Initialize a list of parcels
      /*! This method initializes a list of Parcels
      
          \param p the list of Parcel objects to be initialized
           
      */
      void apply( std::list<Parcel>& p ); 

      /// Initialize a deque of parcels
      /*! This method initializes a deque of Parcels
      
          \param p the deque of Parcel objects to be initialized
           
      */
      void apply( std::deque<Parcel>& p ); 


      /// Initialize a Flock of parcels
      /*! This method initializes a Flock of Parcels
      
          \param p the Flock of Parcel objects to be initialized
           
      */
      void apply( Flock& p ); 

      /// Initialize a Swarm of parcels
      /*! This method initializes a Swarm of Parcels
      
          \param p the Swarm of Parcel objects to be initialized
           
      */
      void apply( Swarm& p ); 

   private:

      // file name
      std::string fname;
   
      // vertical coordinate
      std::string vcoord;
   
      // is the file open or not?
      bool is_open;
   
      // the reference time string (where 0 = model time)
      std::string time0;
   
      // the model time of the parcels being read
      double t0;
   
      // get time from beginning or end of input?
      bool end;
   
   
      // the number of parcels
      size_t np;
      
      // the index of the next parcel to be read
      size_t ip;
   
      // the number of times in the file
      size_t nt;
   
      /// index of the time to be read
      size_t it;

      // debug setting
      int dbug;
   
   
      // //// NETCDF-related variables:
      /// the netcdf handle
      int ncid;
      /// the number of variable in the netcdf file
      int nvars;
      /// the number of lgobal attributes in the netcdf file
      int ngatts;
      
      /// dimensional ID of the time
      int did_time;
      /// dimensional ID of the parcel ID
      int did_id;

      /// the var type of the parcel longitudes
      int vid_lon;
      /// the var type of the parcel latitudes
      int vid_lat;
      /// the var type of the parcel vertical coordinates
      int vid_z;
      /// the var type of the parcel statuses
      int vid_status;
      /// the var type of the parcel flags
      int vid_flags;
      /// the var type of the parcel tag
      int vid_tag; 

      /// the var type of the parcel longitudes
      int vtyp_lon;
      /// the var type of the parcel latitudes
      int vtyp_lat;
      /// the var type of the parcel vertical coordinates
      int vtyp_z;
      /// the var type of the parcel statuses
      int vtyp_status;
      /// the var type of the parcel flags
      int vtyp_flags;
      /// the var type of the parcel tag
      int vtyp_tag; 
      
      /// retrieves the variable ID for a netcdf variable
      /*! This method obtains the netcdf varibale ID, given 
          the desitred variable name and a few other pieces of information.
          
          \param varname this is a reference to a string that contains the nameof the desired variable
          \param required a boolean flag, set to true if the variable must be present in the file.
                 If it is required but not present, then a badFileConventions error is thrown. 
          \param flag A reference to a string that contains the name of an attribute. If the varname
                      parameter is the empty string, and if flag is non-empty, then the variables
                      are seatched for an attribute with this name, whose value is set to "yes".
                      That variable's ID is then returned.
          
          \return the netcdf variable ID of the desired variable. If the value is -1 , then the variable
                   was not found. 
      */                               
      int get_var_id( const std::string &varname, bool required, const std::string &flag, int *vtype = NULLPTR );


      /// reads real numbers from a netcdf variable
      /*! This method reads one or more real numbers from a variable
          in the netcdf file.
          
          \param var_id the netcdf varible ID to be read from
          \param var_type the netcdf type of the variable
          \param n the number of reals to be read
          \param destination a pointer to aa eal or array of reals to hold the data read in

      */
      void rd_real( int var_id, int var_type, size_t n, real* destination ); 
      

      /// reads integer numbers from a netcdf variable
      /*! This method reads one or more integer numbers from a variable
          in the netcdf file.
          
          \param var_id the netcdf varible ID to be read from
          \param var_type the netcdf type of the variable
          \param n the number of integers to be read
          \param destination a pointer to aa eal or array of integers to hold the data read in

      */
      void rd_int( int var_id, int var_type, size_t n, int* destination ); 
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
