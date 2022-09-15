#ifndef GIGATRAJ_MERRA2_DIRECTORY_H
#define GIGATRAJ_MERRA2_DIRECTORY_H

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include "gigatraj/gigatraj.hh"


namespace gigatraj {

/*!
\brief class that provides an inventory for GMAO MERRA2 meteorological data.


*/

class MERRA2_Directory {

    public:


      /// An exception for trying to look up a name that is not recognized
      class badName {};

      /// An exception for failing to look up specs for a desired quantity
      class badNotFound {};
    
      /*! Default Constructor 
      */
      MERRA2_Directory( );
      
      /*! Default Destructor
      */
      ~MERRA2_Directory( );
    
    
      /*! Returns a string containing a CF-standard quantity name, given its 
          MERRA2-standard equivalent
          \param merra2Name a string containing the MERRA2-standard name of the quantity
      */
      std::string CFName(const std::string &merra2Name ) const;

      /*! Returns a string containing a MERRA2-standard quantity name, given its 
      CF-standard equivalent
          \param cfName a string containing the CF-standard name of the quantity
      */
      std::string MERRA2Name(const std::string &cfName ) const;
    
    
      /*! Given a MERRA2-standard quantity name, LookUp returns
          the base URL, the category, and the file tag to be
          used in constructing the URL needd to access MERRA2 data.
          
          function returns value is 0 if the lookup was successful, 1 otherwise.
          
          \param merra2Name a MERRA2-standard quantity name
          \param date the day for which the data are desired.  This is in ISO8601 format (e.g., ``2005-07-21'')
          \param vertgrid  desired vertical grid: 1 for native model grid, 2 for pressure surfaces, -1 for don't-care
          \param horizgrid  desired horizontal grid: 0 for native model grid, 1 for reduced resolution, 2 for reduced FV resolution, -1 for don't-care
          \param timespace desired time spacing, in hours
          \param timeavg desired time averaging, in hours: 0=instantaneous, -1=don't-care )
          \param LongName returns the long-name descriptive string for the quantity. Note that this is a new string which the calling rroutine should destroy at some point.
          \param Units returns the units string for the quantity. Note that this is a new string which the calling rroutine should destroy at some point.
          \param ndims returns the number of spaitan dimensions for this quantity (2 or 3)
          \param actualVertGrid  returns the found vertical grid
          \param actualHorizGrid  returns the found horizontal grid
          \param actualTimeSpace returns the found time spacing, in hours
          \param actualTimeAvg returns the found time averaging, in hours
          \param actualBaseTime returns the base time, in hours
          \param url returns the the url to use to access the data
          
      */
      int LookUp(const std::string merra2Name, const std::string date
                , const int vertgrid, const int horizgrid, const int timespace, const int timeavg 
                , std::string **LongName, std::string **Units, int *ndims
                , int *actualVertGrid, int *actualHorizGrid, int *actualTimeSpace, int *actualTimeAvg, int *actualBaseTime
                , std::string **url 
                 ) const;    
    
    protected:

       /// holds information about data variables' location, gridding, and timing
       class MVarLoc {
       
          public:
             
             /// constructor
             MVarLoc(const std::string Baseurl, const std::string Categ, const std::string Tag, const int Hgrid, const int Vcoord, const int Tspace, const int Tavg, const int Tbase=0);
             
             /// destructor
             ~MVarLoc();
          
             /// the first part of the URL
             std::string baseurl;
             /// the category to which the quantity belongs
             std::string categ;
             /// the sub-category to which the field belongs
             std::string tag;
             /// the horizontal grid code
             int hgrid;
             /// the vertical grid code
             int vcoord;
             /// the time-averaging code
             int tavg;
             /// the time-spacing code
             int tspace;
             /// the base time for the data at the URL
             int tbase;
       };      

       /// holds descriptive information about data variables (names, units, etc.)
       class MVarDesc {
          
          
          public:
             /// constructor
             MVarDesc(const std::string Quantity, const int Ndims, const std::string Stdname, const std::string Longname, const std::string Units, const std::string Loclist );
             
             /// destructor
             ~MVarDesc();
             
             /// the name of the field quantity
             std::string name;
             /// the standard name for the quantity
             std::string stdname;
             /// a long descriptive name for the quantity
             std::string longname;
             /// the units of the quantity
             std::string units; 
             /// the number of dimensions of the quantity (2 or 3)
             int ndims;
             /// a vector of possible locations at which this quantity may be found
             std::vector<MVarLoc> locs;
       };
             
    private:
            
       std::map<std::string, std::string> cf_names;
       std::map<std::string, std::string> gmao_names;
       std::map<std::string, MVarDesc *> gmao_vars;
       

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
