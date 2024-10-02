#ifndef GIGATRAJ_GRIDFIELDDIM_H
#define GIGATRAJ_GRIDFIELDDIM_H

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/GridField.hh"

namespace gigatraj {

/*!
\brief abstract class to hold information about a single dimension.

\ingroup gridfield

The GridFieldDim class provides a mechanism for dealing with dimensional information.

*/
class GridFieldDim : public GridField {

   typedef std::vector<real> realvec;


   /// \brief overlays the << operator for writing a GridLatLonFieldSSfc object
   /*! This method serializes the GridFieldDim object and sends it to an ostream.
   
        \param os the output stream (ostream) object to which the serialized object is to be sent
        \param p the GridFieldDim object to be serialized and output
   */
   friend std::ostream& operator<<( std::ostream& os, const GridFieldDim& p);

   /// \brief overlays the >> operator for reading a GridFieldDim object
   /*! This method reads a GridFieldDim object's serialzied data from 
        an istream and deserializes it.
   
        \param is the input stream (istream) object from which the serialized object is to be read
        \param p the GridFieldDim object to be input and deserialzied
   */
   friend std::istream& operator>>( std::istream& is, GridFieldDim& p);



   public:
   
      /// Default constructor
      /*!
         This is the default contructor for the GridFieldDim class.
      */
      GridFieldDim();
      
      /// Default destructor
      /*!
         This is the default destructor for the GridFieldDim class.
      */
      virtual ~GridFieldDim();

      /// copy constructor
      /*!
         This is the copy contructor method for the GridFieldDim class.
      */
      GridFieldDim(const GridFieldDim& src);

      
      /// clears contents 
      /*! This method clears the contents of the object, except for information
          related to the process group
      */    
      void clear();

      /// returns the readiness status
      /*! This object returns the readiness status: whether the object
          contains valid data and metadata.
      
          \return returns 0 if the object is ready to be used, non-zero otherwise
      */
      int status() const;


      /// returns the size of the dimension
      /*! This method returns the number of gridpoints in this dimension
          \return the number of values
      */
      int size() const;


      /// returns a single data value
      /*! This method returns a single data value.
           \param i the index of the gridpoint
           \return the value of the ith element of the data grid
      */     
      real value( int i ) const;
      
      /// assigns a single data value
      /*~! This method sets e value of a single gridpoint in the dimension.
      
           It checks that dimensional values remain monotonic.
           
           It does not add values or increase tghe size of the dimension.
           
           \param i the index whose value is ot be set
           \param val the new value of the ith gridpoint
      */
      void set_value( int i, real val );     

      /// returns an array of dimensional values
      /*! This method fills an array of dimensional values.
         
           \param n the number of dimensional values to return
           \param vals a pointer to an n-element array of real values to hold the data.
                       If NULLPTR, then a new array is created, and the calling
                       routine has the responsibility of deleting it.
           \param indices a pointer to an n-element array of indices into the dimensional grid, that locate each data gridpoint
           \return a pointer to the vals array. If the input vals is not NULLPTR, then this is the same
                   as the input vals. otherwise, it points to the newly-created array of values.                          
      */     
      real* values( int n, real* vals, int* indices ) const;
      

      /// operator overlay allowing for a = obj(i) syntax      
      /*! This operator overlay for () allows 
          GridFieldDim objects' dimensional values to be referenced
          using an array-like syntax. 
          
          \param i the coordinate index
          \return the ith dimensional value
      */    
      real operator()( int i ) const;


      /// applies a linear transformation to the dimensional values
      /*!  This method applies a linear transformation to the dimensional values in this object.
           This is not the same as the units() method, which sets the units
           and optionally the scale factor and offset to take the dimensional values into MKS units,
           but leaves any dimensional in the object unchanged.  In contrast,
           the transform() method actually applies the scaling factor and offset to
           the dimensional and to the MKS scale and offset as well.
      
           \param unyts the new units that the dimensional values will have after the transformation
      
           \param scale a scaling factor to be applied to the dimensional values
      
           \param offset an offset to be applied to the dimensional. values
      */
      void transform( const std::string unyts, real scale=1.0, real offset=0.0 ); 
      

      ///  loads dimensions only into this object
      /*!  This method loads dimensions only into this object.
           \param inx vector of coordinates
           \param loadFlags bitwise flags:
                   - GFL_PREFILL if the data grid is to be pre-filled with bad-data flags
                                     (the default is to leave the data undefined)
      */
      void load( const realvec& inx, const int loadFlags=0  );


      /// checks whether the values of a given GridFieldDim object are compatible with this object.
      /*! This method checks another GridFieldDim object for compatibility with this one.
          
          \param obj the GridFieldDim object with which this object is to be compared
          \param flags ignored

          \return true if the dimensions and the times match, false otherwise
      */
      bool compatible( const GridFieldDim& obj, int flags = METCOMPAT_STRICT ) const;


      /// checks for metadata compatibility with a given GridFieldProfile object
      /*! This method checks whether another GridFieldProfile object has the same metadata as this one.
          Metadata include the quantity name and units, and the vertical coordinate
          name and units.
          
          \param obj the GridFieldProfile object with which this object is to be compared

          \return true if the metadata are the same, false otherwise
      */
      bool match( const GridFieldDim& obj ) const;

      /// returns the number of possible dimensional points in the grid.
      /*! This method returns the number of possible dimensional points in the grid.
          
          \return the total number of gridpoints, or zero
          if the grid has been specified but no dimensional have been loaded.
          
      */
      int dataSize() const;   

      /// returns the two indices which straddle a given data value
      /*! This method returns the two indices which straddle a given vdata value.
      
           \param z the input data value
           \param i1 returns the index that lies lower than the vertical level
           \param i2 returns the index that lies higher than the vertical level
      */     
      void index( real z, int* i1, int* i2 ) const;

      ///  returns a set of gridpoint values
      /*!  This method returns a set of gridpoint values.
      
           \param n the number of gridpoint values desired
           \param is n-element array of indices into the dimensional array
           \param vals n-element array of reals to hold the results
           \param flags a set of bitwise flags that control the behavior of the method:
                   0x01 = ignore any multiprocessing and fetch the gridpoints locally
                   0x02 = call svr_done once the gridpoints have been obtained.
      */
      void gridpoints( int n, int* is, real* vals, int flags=0) const;

      /// (parallel processing) receives metadata from a central met processor
      /*! This method receives metadata from a central met processor, if
         we are dedicating processors to handling met data.
         Otherwise, does nothing.  On success, all information
         about the data 
         except for the actual data values will be present in the current
         object.
      */   
      void receive_meta();
      
      /// (parallel processing) handles the server side of the gridpoints transaction
      /*! This method handles the server side of the gridpoints transaction, for multi-processing
         with a dedicated meteorological data processor.
         For serial processing or when there is no dedicated met processor,
         this routine does nothing.
         
         \param id the ID of the processor whose request is to be satisfied
      */   
      void svr_send_vals( int id ) const;

      /// (parallel processing) handles the server side of the receive_metadata transaction
      /*! This method handles the server side of the  receive_metadata transaction, for multi-processing
         with a dedicated meteorological data processor.
         For serial processing or when there is no dedicated met processor,
         this routine does nothing. 
      
         \param id the ID of the processor to which we are sending the metadata
         
      */      
      void svr_send_meta(int id) const;

      /// the type of object this is
      static const string iam;

      /// returns a string with the specific type of object this is
      /*! This method returns the kind of object this is.
      
          \return the class name
      
      */
      std::string id() const { return iam; };

      /// writes the current object to an ostream
      /*! This method serializes the object and writes the serialization to an ostream.

          \param os the output ostream
      */
       void serialize(std::ostream& os) const;

      /// reads the current object from an istream.
      /*!  This method reads a GridField serialization from an istream, and
            deserialized it into the current object.
            
          \param is the input ostream
      */
      void deserialize(std::istream& is);


      /// throws an error
      /*! This method throws an error, since dimensions are not associated with specific times.
         
         \param tyme the internal model time
         \param caltag the real-world calendar-oriented date+time stamp
      
      */     
      void set_time( const double tyme, const std::string caltag )
      {
         throw (badInvalidOp());
      } 
      
      
      /// throws an error
      /*! This method throws an error, since dimensions do not use fill values
          
          \param val the fill value
      */
      void set_fillval(const real val) {
          throw (badInvalidOp());
      }


     class iterator;
     friend class iterator;
     /*!
      \brief iterator for the GridFieldDim class.

      This iterator permits looping over each element of the grid.
      If looping is done over identical grid objects (the same child class, 
      populated with the same dimension) simultaneously, then
      each of the objects' iterators is guaranteed to be pointing
      to the same gridpoint.
      
     */
     class iterator  {
        public:
           
           /// default constructor for the GridFieldDim::iterator (do not use)
           /*! This is the default constructor for the GridFieldDim::iterator.
               Note that since it is not initialized by GridFieldDim::begin() or GridFieldDim::end(), 
               this default constructor is useless.
           */
           iterator();
           
           /// the usual constructor for the GridFieldDim::iterator
           /*! This is the usual constructor for the GridFieldDim::iterator.
           
               \param grid a pointer to the GridFieldDim object being iterated over
               \param n the index into the data vector, to which
                        this iterator will be initialized
           */
           iterator( GridFieldDim* grid, int n=0 );
           
           /// override operator *, returns a reference to the current data element
           real operator*() const;
           
           /// override operator == ; tests two iterators for equallity
           bool operator==(const iterator& x) const;
           
           /// override operator != ; tests two iterators for inequality
           bool operator!=(const iterator& x) const;
           
           /// override operator ++ ; increments the iterator to point to the next data element
           iterator& operator++(int n);
           
           /// override operator =
           iterator& operator=(const iterator& src); 
     
           /// returns the index of the gridpoint the interator points to
           /*! This method returns the index into the GridFieldDim object's data
               of the grid point at which the iterator is pointing.
               
               \param i a pointer to the index
           */
           void indices( int* i ) const;

           /// assigns a value to the data element pointed to by the iterator
           /*! This method uses the iterator to assign a value to the GridFieldDim object's data
               at the current gridpoint.
               
               \param val the value to be assigned
           */    
           void assign( real val ); 


        protected:

           /// index into the data element
           int my_index;
           /// which data vector is this an iterator for?
           GridFieldDim* my_grid;
     
     };


     class const_iterator;
     friend class const_iterator;
     /*!
     \brief iterator for a const object of the GridFieldDim class.

      Like the regular iterators, this iterator permits looping over each
      element of the grid.
      Unlike the regular iterator, const_iterator cannot be used to assign
      values to the data vector.
      
     */
     class const_iterator  {
        public:
           
           /// default constructor for the GridFieldDim::const_iterator (do not use)
           /*! default constructor for the GridFieldDim::const_iterator
               Note that since it is not initialized by GridFieldDim::begin() or GridFieldDim::end(), 
               this default constructor is useless.
           */
           const_iterator();
           
           /// the usual constructor for the GridFieldDim::const_iterator
           /*! This is the usual constructor for the GridFieldDim::const_iterator.

               \param grid a pointer to the GridFieldDim object being iterated over
               \param n the index into the data vector, to which
                        this iterator will be initialized
           */
           const_iterator( const GridFieldDim* grid, int n=0 );
           
           /// override operator *, returns the current data element
           real operator*() const;
           
           /// override operator == ; tests two const_iterators for equallity
           bool operator==(const const_iterator& x) const;
           
           /// override operator != ; tests two const_iterators for inequality
           bool operator!=(const const_iterator& x) const;
           
           /// override operator ++ ; increments the const_iterator to point to the next data element
           const_iterator& operator++(int n);
           
           /// override operator =
           const_iterator& operator=(const const_iterator& src); 
     
           /// returns the index of the gridpoint the interator points to
           /*! This method returns the index into the GridFieldDim object's data
               of the grid point at which the iterator is pointing.
               
               \param i a pointer to the index
           */
           void indices( int* i ) const;

        protected:

           /// index into the data element
           int my_index;
           /// which data vector is this an iterator for?
           const GridFieldDim* my_grid;
     
     };


     /// returns the beginning of the Grid
     /*! This method initializes an iterator to the beginning of a gridded field
         
         \return an iterator that points to the beginning of a grid
     */
     iterator begin();

     /// returns (just past) the end of the Grid
     /*! This method initializes an iterator to the end of a gridded field, or more
         precisely one past the last element of the grid.
         
         \return an iterator that points to the end of a grid
     */
     iterator end();

     /// returns the beginning of the Grid
     /*! This method initializes an immutable iterator to the beginning of a gridded field
         
         \return n immutable iterator that points to the beginning of a grid
     */
     const_iterator begin() const;

     /// returns (just past) the end of the Grid
     /*! This method initializes an immutable iterator to the end of a gridded field, or more
         precisely one past the last element of the grid.
         
         \return an immutable iterator that points to the end of a grid
     */
     const_iterator end() const;

     
   protected:

      
      /// how many coord values
      int nzs;    
      /// +1 if coordinate values increase with index, -1 if they decrease, 0 if undefined
      int zdir;
      ///  sets the cooridnate direction
      /*! This method sets the coordinate direction, based on whether the coordinates increase
          or decrease with array index number.
            \param loadFlags (reserved for future expension)  
      */
      void setDir( const int loadFlags=0 ); 
            
      /// used by child classes for operator= overriding methods
      void assign( const GridFieldDim& src);

      /// checms dimensional values to ensure they are monotonic
      bool checkdim( const realvec& inx ) const;
      
      //void init_md5();
      
      //void add_md5( real val );
      
      //void end_md5();
      

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
