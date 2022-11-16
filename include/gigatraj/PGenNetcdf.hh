#ifndef PGENNETCDF_H
#define PGENNETCDF_H

#include <deque>
#include <list>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/Parcel.hh"
#include "gigatraj/ParcelGenerator.hh"
#include "gigatraj/PGenRep.hh"
#include "gigatraj/NetcdfIn.hh"


namespace gigatraj {

/*!
\ingroup parcelgenerators

\brief generates collections of parcels from a netcdf file of positions

The PGenNetcdf class generates collections of parcels whose positions
are initialized from a netcdf file. A NetcdfIn object is used to do the 
actual reading.  

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


It is perhaps worth noting that 
the main difference between this class and the NetcdfIn class is
that PGenNetcdf is a parcel generator, which creates collections
of Parcels (arrays, vectors, lists, dequeues, and Flocks)
and initializes them from a netcdf file 
The NetcdfIn class reads from a netcdf file and loads
the data into collections of Parcels that already exist.

*/

class PGenNetcdf: public ParcelGenerator {

   public:
   
      /// default constructor
      /*! This is the default constuctor for the PGenNetcdf class
      
      */
      PGenNetcdf();
      
      /// destructor
      /*! This is the destructor for the PGenNetcdf class.
      
      */
      ~PGenNetcdf();
      
      /// read locations from a file into an array of Parcels
      /*! This method creates an array of Parcels and initializes their locations from a file.
      
         \param p the input parcel whose settings we are to copy
         \param np the number of parcels is returned in this variable
         \param file the name of the file from which parcel locations are to be read
         
         \return a Parcel pointer that points to an array of Parcel objects.
      */
      Parcel * create_array(const Parcel& p, size_t *np
                           , const std::string &file
                           ); 


      /// read locations from a file into a vector container of Parcels
      /*! This method creates a vector container of Parcels and initializes their locations from a file.
      
         \param p the input parcel whose settings we are to copy
         \param file the name of the file from which parcel locations are to be read
         
         \return a pointer to a vector of Parcel objects; the calling routine must delete this vector once it is no longer needed.
      */
       std::vector<Parcel>* create_vector(const Parcel& p
                           , const std::string &file
                           );
   



      /// read locations from a file into a list container of Parcels
      /*! This method creates a list container of Parcels and initializes their locations from a file.
            
         \param p the input parcel whose settings we are to copy
         \param file the name of the file from which parcel locations are to be read
         
         \return a pointer to a list of Parcel obejcts; the calling routine must delete this list once it is no longer needed.
      */
       std::list<Parcel>* create_list(const Parcel& p
                           , const std::string &file 
                           );
      


      /// read locations from a file into a deque container of Parcels
      /*! This method creates a deque container of Parcels and initializes their locations from a file.
      
         \param p the input parcel whose settings we are to copy
         \param file the name of the file from which parcel locations are to be read
         
         \return a pointer to a deque of Parcel objects; the calling routine must delete this deque once it is no longer needed.
      */
       std::deque<Parcel>* create_deque(const Parcel& p
                           , const std::string &file
                           );
      


      /// read locations from a file into a Flock container of Parcels
      /*! This method creates a Flock container of Parcels and initializes their locations from a file.
          This version uses an open istream and is thus suitable for use with stdin.
      
         \param p the input parcel whose settings we are to copy
         \param file the name of the file from which parcel locations are to be read
         \param pgrp a process-group object that is used for parallel processing
         \param r    the ratio of meteorological-data processors to parcel-tracing processors.
                     (For example, if r=3 then there will be one met processor for
                     every 3 parcel-tracing processors) 
         
         \return a pointer to a Flock of Parcel objects; the calling routine must delete this Flock once it is no longer needed.
      */
       Flock* create_Flock(const Parcel& p
                           , const std::string &file, ProcessGrp* pgrp=NULLPTR, int r=0
                           );
      
            

      /// read locations from a file into a Swarm pseudo-container of Parcels
      /*! This method creates a Swarm pseudo-container of Parcels and initializes their locations from a file.
          This version uses an open istream and is thus suitable for use with stdin.
      
         \param p the input parcel whose settings we are to copy
         \param file the name of the file from which parcel locations are to be read
         \param pgrp a process-group object that is used for parallel processing
         \param r    the ratio of meteorological-data processors to parcel-tracing processors.
                     (For example, if r=3 then there will be one met processor for
                     every 3 parcel-tracing processors) 
         
         \return a pointer to a Swarm of Parcel objects; the calling routine must delete this Swarm once it is no longer needed.
      */
       Swarm* create_Swarm(const Parcel& p
                           , const std::string &file, ProcessGrp* pgrp=NULLPTR, int r=0
                           );
      


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
    
        
      
   
   private:
   
      /// the NetcdfIn object that actuslly does the work
      NetcdfIn ncIn;
      
      /// the PGenRep obejct that makes the parcels
      PGenRep pgen;
      
      int dbug;
      
      std::string fname;
      std::string vcoord;
      std::string caltime;
      double time0;
      bool atend;
      size_t n;
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
