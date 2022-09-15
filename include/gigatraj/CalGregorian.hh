 #ifndef GIGATRAJ_CALGREGORIAN_H
#define GIGATRAJ_CALGREGORIAN_H

#include "gigatraj/gigatraj.hh"

namespace gigatraj {

/*!
\ingroup misc
\brief provides Gregorian calendar functions for date manipulation in gigatraj

The CalGregorian class provides the ability to manipulate dates in the
Gregorian calendar.  Because this is the calendar system used with
production meteorological analysis systems, this class has functions
that can be useful with dates referencing those systems' data . 

Note that times may be specified here to the nearest minute, but not
to the nearest second.

Two formats are used to represent dates.  For a numeric format, the
double-precision value of the number of days (including fractional days)
since 1900-01-01T00:00 is used, with 1900-01-01T00:00 itself defined to correspond
to the value 1.0; this is called the "day1900" value.  For a string format,
the format "yyyy-mm-ddThh:mm" is used; this is called the "iso8601" format,
as it is compliant with that standard.

*/

class CalGregorian {

   public:

      /// Error: a string does not contain a valid Gregorian calendar date/time
      class badGregorianDateFormat {};

      /// constructor
      CalGregorian();
      
      /// destructor
      ~CalGregorian();

      /// Indicates whether a given year is a leap year
      /*! isLeap() examines a given year and finds if it is a leap year
                   in the Gregorian calendar system.
           \return true if the year is a leap year, false otherwise
           \param year the year whose status is to be determined
      */
      bool isLeap( int year ) const;

      /// Indicates whether a given year is a leap year
      /*! isLeap() examines a given year and finds if it is a leap year
                   in the Gregorian calendar system.
           \return true if the year is a leap year, false otherwise
           \param date an ISO8601-formatted date string whose year's status
                  is to be determined
      */
      bool isLeap( const std::string& date ) const;
           
      /// parses an ISO8601-formatted date.
      /*! parseDate examines an ISO8601-formatted date and breaks it into
           component parts.
          \param date the input date string
          \param year a pointer to an integer to hold the year.  Note that if
            this is the NULL pointer, no output is attempted;  The same applies 
            to the other parameters \p month, \p day, \p hour, and \p minute.
          \param month a pointer to an integer to hold the month of the year
          \param day a pointer to an integer to hold the day of the month
          \param hour a pointer to an integer to hold the hour of the day
          \param minute a pointer to an integer to hold the minute of the hour
          \param second a pointer to an integer to hold the second of the minute
      */    
      void parseDate( const std::string& date, int* year, int *month, int* day, int* hour=NULLPTR, int* minute=NULLPTR, float* second=NULLPTR ) const;

      /// builds an ISO8601-formatted date from components.
      /*! buildDate takes the components of a date, in integer form, and assembles
          them into an ISO-8601 format date string.
          Note that no rounding is done. It is up to the calling routine to
          set the arguments to avoid such outputs as
          "2012-10-22T24", "2012-10-22T23:60", or "2014-11-26T23:59:60".
          
          \return a string containing the date in ISO-8601 format
          ("yyyy-mm-ddThh:mm")
          
          \param year the year (e.g., 2014)
          \param month the month of the year (1-12)
          \param day the day of the month (1-31)
          \param hour the hour of the day (0-23)
          \param minute the minute of the hour (0-59)
          \param second the second of the minute (0-59.999999)
          \param format if -999, then this is ginored. Otherwise, it overrides the object's format code
      */    
      std::string buildDate( int year, int month, int day, int hour=0, int minute=0, float second=0.0, int format=-999 ) const;
   
      /// converts an ISO8601 date string to a day1900 floating point number
      /*! day1900 converts an ISO8601-formatted date string to 
          day1900 format (i.e., the number of days since 1900-01-01T00:00)

          \return the day1900 format double-precision number
          \param date the input IDO date
      */
      double day1900( const std::string& date ) const;

      /// converts a day1900 floating point number to an ISO8601 date string
      /*! date1900 takes a double-precision "day1900" number (i.e., 
          the number of days since 1900-01-01T00:00) and converts it
          to an ISO8601 format date string.

          \return the ISO8601 format date string
          \param day the number of days elapsed since 1900-01-01T00:00
          \param format if -999, then this is ginored. Otherwise, it overrides the object's format code
                  * -999 = use the default format set for this object
                  * 0 = date only, with no time (yyyy-mm-dd)
                  * 1 = date plus the hour (yyyy-mm-ddThh)
                  * 2 = date plus the hour and minute (yyyy-mm-ddThh:mm)
                  * 3 = date plus the hour, minute, and integer second (yyyy-mm-ddThh:mm:ss)
                  * 4 = date plus the hour, minute, and floating second (to four decimal places) (yyyy-mm-ddThh:mm:ss.ssss) 
      */
      std::string date1900( double day, int format=-999 ) const;    

      /// returns the output format style
      /*! Tbhis method returns a format code that determines the
          precisions of the output from date1900().
          
          \return the format code.
                  * -999 = use the default format set for thisw object
                  * 0 = date only, with no time (yyyy-mm-dd)
                  * 1 = date plus the hour (yyyy-mm-ddThh)
                  * 2 = date plus the hour and minute (yyyy-mm-ddThh:mm)
                  * 3 = date plus the hour, minute, and integer second (yyyy-mm-ddThh:mm:ss)
                  * 4 = date plus the hour, minute, and floating second (to four decimal places) (yyyy-mm-ddThh:mm:ss.ssss) 
         
      */
      int format() const;
      
      /// sets the output format style
      /*! This methos sets the precision of the string output of the date1900() method.
      
          \param fmt the format code.
                  * 0 = date only, with no time (yyyy-mm-dd)
                  * 1 = date plus the hour (yyyy-mm-ddThh)
                  * 2 = date plus the hour and minute (yyyy-mm-ddThh:mm) This is the default.
                  * 3 = date plus the hour, minute, and integer second (yyyy-mm-ddThh:mm:ss)
                  * 4 = date plus the hour, minute, and floating second (to four decimal places) (yyyy-mm-ddThh:mm:ss.ssss) 
          
      */
      void format( const int fmt=2 );
      
   private:
      static const int monthlengths[13];
      static const int daynums[13];
      
      // precision of calendar output
      // 0 = yyyy-mm-dd
      // 1 = yyyy-mm-ddThh
      // 2 = yyyy-mm-ddThh:mm (default)
      // 3 = yyyy-mm-ddThh:mm:ss
      // 4 = yyyy-mm-ddThh:mm:ss.ssss
      int ofmt;
   
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
