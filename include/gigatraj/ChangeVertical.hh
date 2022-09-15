#ifndef CHANGEVERTICAL_H
#define CHANGEVERTICAL_H


#include "gigatraj/gigatraj.hh"
#include "gigatraj/ParcelFilter.hh"
#include "gigatraj/MetData.hh"
#include "gigatraj/Flock.hh"

namespace gigatraj {

/*!
\ingroup parcelfilters

\brief chnages the vertical cooridnate of a parcel from one physical quantity to another

The ChangeVertical class switches a parcel's vertical coordinate from one vertical quantity to another.
Using and object of this class, a parcel may be initialized in, say, pressure altitude and
converted to potential temperature for tracing in quasi-isentropic mode and subsequent conversion 
and output back to pressure altitudes.

*/

class ChangeVertical : public ParcelFilter {

    public:
    
       /// An exception for a conflict in vertical coordinates
       class badvcoordconflict {};
    
       /// default contructor
       /*! With the default constructor, both the "from" vertical coordinate and the "to" vertical coordinate
           may be specified. If either one is omitted, the empty string is used. 
           A meteorological data source may be specified. If no meteorological data source is specified,
           then when the filter is applied then the Parcel's meteorological data source will be used.
           If the "from" or the "to" vertical coordinate quantity is the empty string,
           then the vertical coordinate of the meteorological source will be used. 
       
           \param to the name of the vertical coordinate to be converted to
           \param from the name of the vertical cooridnate to be converted to
           \param met a pointer to a meteorological data source whose vertical coordinate matches either 
                      the "from" or the "to" coordinate.  If NULL, th 
      */
      ChangeVertical( const std::string to="", const std::string from="", MetData* met=NULL );
      
      
      /// destructor
      /*! This is the destructor method for the ChangeVertical object
      */
      ~ChangeVertical();
      
      /// returns the "from" vertical coordinate quantity
      /*! This method returns the name of the "from" coordinate   
      
          \return the name of the "from" coordinate quantity    
      */
      inline std::string from()
      {
          return fromQuantity;
      }

      /// sets the "from" vertical coordinate quantity
      /*! This method sets the name of the "from" coordinate.
      
          \param q the name of the "from" vertical coordinate quantity
                
      */
      inline void from( const std::string q)
      {
          fromQuantity = q;
      }

      /// returns the "to" vertical coordinate quantity
      /*! This method returns the name of the "to" coordinate   
      
          \return the name of the "to" coordinate quantity    
      */
      inline std::string to()
      {
          return toQuantity;
      }

      /// sets the "to" vertical coordinate quantity
      /*! This method sets the name of the "to" coordinate.
      
          \param q the name of the "to" vertical coordinate quantity
                
      */
      inline void to( const std::string q)
      {
          toQuantity = q;
      }

      /// sets the meteoroligical data source
      /*! This method sets the meteorological data source to be used in the coordinate
          conversions. Ordinarily, the source is taken from the Parcel objects
          when the filter is applied. But this method allows for setting the source
          explicitly, for special circumstances.
     */     
     void setMet( MetData* met );
      
      
      /// convert a single Parcel
      /*! This method converts the vertical coordinate of the given Parcel
      
         \param p the Parcel object whose vertical coordinate is to be converted.
                  Note that if the "to" vertical coordinate is different from 
                  that of the Parcel's meteorological data source's vertical coordinate,
                  then the Parcel's NonVert status is set, and its trajectory will no longer
                  be traced. Conversely, if the vetical coordinate does match, then
                  the Parcel's NonVert status is cleared, and its trajectory will
                  be traced.
                  
      */
      void apply( Parcel& p );

      /// apply the ChangeVertical filter to an array of Parcels
      /*! Apply the filter to an array of parcels
      
      \param p the array of Parcel objects to which the filter is to be applied
    
      \param n the number of Parcel objects in the array

      */
      void apply( Parcel * const p, const int n ); 

      /// apply the ChangeVertical filter to  a vector of parcels
      /*! This method applies the ChangeVertical filter to a vector of Parcels.
      
          \param p the vector of Parcel objects to to which the ChangeVertical filter 
           
      */
      void apply( std::vector<Parcel>& p ); 

      /// apply the ChangeVertical filter to  a list of parcels
      /*! This method applies the ChangeVertical filter to a list of Parcels.
      
          \param p the list of Parcel objects to to which the ChangeVertical filter is to be applied
           
      */
      void apply( std::list<Parcel>& p ); 

      /// apply the ChangeVertical filter to  a deque of parcels
      /*! This method applies the ChangeVertical filter to a deque of Parcels.
      
          \param p the deque of Parcel objects to to which the ChangeVertical filter  is to be applied
           
      */
      void apply( std::deque<Parcel>& p ); 

      /// apply the ChangeVertical filter to  a Flock of parcels
      /*! This method applies the ChangeVertical filter to a Flock of Parcels.
      
          \param p the Flock of Parcel objects to to which the ChangeVertical filter is to be applied
           
      */
      void apply( Flock& p ); 


    private:
    
       // the name of the vertical coordinate to be converted from
       std::string fromQuantity;
       // the name of the vertical coorindate to be converted to
       std::string toQuantity;
       // meteorological data source
       // its vertical coordinate must be fromQuantity,
       // and it must have toQuantity as a valid quantity
       MetData *metsrc;
       // if true, then we had to create our own met data source
       bool myMet;

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
    
