 #ifndef GIGATRAJ_CALGREGORIAN_H
#define GIGATRAJ_CALGREGORIAN_H

#include "gigatraj/gigatraj.hh"
#include <time.h>

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
      /*! This method returns a format code that determines the
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
      
      /// converts a Unix epoch time to an ISO8601 string
      /*! this method converts a Unoix epoch time (as returned by the C time() function) 
          to an ISO8601 string.
          
          \param t the Unix epoch time (seconds elapsed wince 1970-01-01T00:00:00)
          \param format if -999, then this is ginored. Otherwise, it overrides the object's format code
                  * -999 = use the default format set for this object
                  * 0 = date only, with no time (yyyy-mm-dd)
                  * 1 = date plus the hour (yyyy-mm-ddThh)
                  * 2 = date plus the hour and minute (yyyy-mm-ddThh:mm)
                  * 3 = date plus the hour, minute, and integer second (yyyy-mm-ddThh:mm:ss)
                  * 4 = date plus the hour, minute, and floating second (to four decimal places) (yyyy-mm-ddThh:mm:ss.ssss) 
          \return the ISO8601 format date string
      
      */
      std::string epochDate( time_t t, int format=-999  ) const;
      
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
