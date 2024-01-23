
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "math.h"

#include <sstream>

#include "gigatraj/CalGregorian.hh"

using namespace gigatraj;

// the length of each month (in a non-leap year)
// (Note that the month number (January = 1) can
// be used as an index into this array, hence the
// leading 0 element at the beginning.)
const int CalGregorian::monthlengths[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
// the day of the year at the end of each month (of a non-leap year)
const int CalGregorian::daynums[13] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };


CalGregorian::CalGregorian()
{
   ofmt = 2;
}

CalGregorian::~CalGregorian()
{
}


bool CalGregorian::isLeap( int year ) const 
{
     // K&R, p. 41
     return (   ((year % 4) == 0) 
             && ( ((year % 100) != 0) || (( year % 400) == 0 ) )
            );
}

bool CalGregorian::isLeap( const std::string& date ) const
{    
     // the year
     int year;  
     // a string input stream 
     std::istringstream itmp;

     try {
       // ingest the year part of the date string into the input stream 
       itmp.str(  date.substr(0,4) );
       // and take the integer out of the input stream
       itmp >> year;
     } catch (...) {
        throw (badGregorianDateFormat());
     }
     if ( itmp.fail() ) {
        throw (badGregorianDateFormat());
     }
      
     // and test the integer for being a leap year
     return isLeap(year);
     
}

void CalGregorian::parseDate( const std::string& date, int* year, int *month, int* day, int*hour, int*minute, float*second ) const
{
     // a string input stream, used for converting strings to integers
     std::istringstream itmp;
     // single characters to be read in from the input data string
     // These should hold the corresponding characters ("-", "T", ":", respectively)
     char dash, tee, colon;
     // flags to indicate missing parts of the date string
     bool gotTime, gotMins, gotSecs;
     // the component parts of the date
     int myyear, mymonth, myday, myhour, mymin;
     float mysecond;

     // ensures initialization
     dash = '!'; // will be overridden by a dash 

     try {
       // feed the input data string into the input stream
       itmp.str( date );
       // and now pull the pieces out of the input stream,
       // one by one.
       // First, the year
       itmp >> myyear;
       // then, a dash
       itmp >> dash;
       // and it really must be a dash
       if ( dash != '-' ) {
          itmp.setstate(std::ios::failbit);
       }   
       // next, the month
       itmp >> mymonth;
       // and another dash
       itmp >> dash;
       if ( dash != '-' ) {
          itmp.setstate(std::ios::failbit);
       }
       // next, the day of the month
       itmp >> myday;
     } catch (...) {
        std::cerr << "Bad date format: " << date << std::endl;
        throw (badGregorianDateFormat());
     }   
     if ( itmp.fail() ) {
        std::cerr << "Bad date format: " << date << std::endl;
        throw (badGregorianDateFormat());
     }
     // At this point, we should have a date.
     // But we might also have the time part "Thh:mm" appended
     // to the date string, so try to extract that.
     try {
        // grab a "T"
        itmp >> tee;
        gotTime = ( tee == 'T' );
     } catch (...) {
        gotTime = false;
     }
     if ( itmp.fail() ) {
        gotTime = false;
     }
     
     if ( gotTime ) {
        try {
            // get the hour of the day
            itmp >> myhour;
        } catch (...) {
           std::cerr << "Bad date format: " << date << std::endl;
           throw (badGregorianDateFormat());
        }   
        if ( itmp.fail() ) {
           std::cerr << "Bad date format: " << date << std::endl;
           throw (badGregorianDateFormat());
        }
        
        try {
            // next comes a ":" to separate the hour from the minute
            itmp >> colon;
            gotMins = ( colon == ':' );
        } catch (...) {
            gotMins = false;
        }
        if ( itmp.fail() ) {
            gotMins = false;
        }
        
        if ( gotMins ) {
           try{    
               // get the minute of the hour
               itmp >> mymin;  
           } catch (...) {
              std::cerr << "Bad date format: " << date << std::endl;
              throw (badGregorianDateFormat());
           }   
           if ( itmp.fail() ) {
              std::cerr << "Bad date format: " << date << std::endl;
              throw (badGregorianDateFormat());
           }
        } else {
           // no colon, so no minute, but that's an OK
           // abbreviation.  Just assume the minute is 0.
           mymin = 0;
           itmp.clear();
        }   

        try {
            // next comes a ":" to separate the minute from the seconds
            itmp >> colon;
            gotSecs = ( colon == ':' );
        } catch (...) {
            gotSecs = false;
        }
        if ( itmp.fail() ) {
            gotSecs = false;
        }
        if ( gotSecs ) {
           try{    
               // get the second of the minute 
               itmp >> mysecond;  
           } catch (...) {
              std::cerr << "Bad date format: " << date << std::endl;
              throw (badGregorianDateFormat());
           }   
           if ( itmp.fail() ) {
              std::cerr << "Bad date format: " << date << std::endl;
              throw (badGregorianDateFormat());
           }
        } else {
           // no colon, so no seconds, but that's an OK
           // abbreviation.  Just assume the second is 0.
           mysecond = 0.0;
           itmp.clear();
        }   
        
        
     } else {
        // no "T", so no hour or minute, but that's an OK
        // abbreviation.  Just assume the hour and the minute 
        // are both 0.
        myhour = 0;
        mymin = 0;
        mysecond = 0.0;
        itmp.clear();
     }
     
     // whichever arguments are not null pointers, 
     // stuff the results back up into them.
     if ( year != NULLPTR ) {
        *year = myyear;
     }   
     if ( month != NULLPTR ) {
        *month = mymonth;
     }   
     if ( day != NULLPTR ) {
        *day = myday;
     }   
     if ( hour != NULLPTR ) {
        *hour = myhour;
     }   
     if ( minute != NULLPTR ) {
        *minute = mymin;
     }   
     if ( second != NULLPTR ) {
        *second = mysecond;
     }   
     
}

std::string CalGregorian::buildDate( int year, int month, int day, int hour, int minute, float second, int format ) const
{
     // the output date string
     std::string date;
     // a string output stream for integer->string conversions
     std::ostringstream otmp;
     
     if ( format == -999 ) {
        format = ofmt;
     }

     // set the formatting: decimal (base-10) output
     otmp.setf(std::ios::dec, std::ios::basefield);
     otmp.setf(std::ios::fixed, std::ios::floatfield);
     // all numbers should be printed with leading zeroes
     otmp.fill('0');
     // fill from the front
     otmp.setf(std::ios::internal, std::ios::adjustfield);
     // year is four digits
     otmp.width(4);
     // do the year and the dash that follows
     otmp << year << "-";
     // months is two digits
     otmp.width(2);
     // do the month and the dash that follows
     otmp << month << "-";
     // day is two digits
     otmp.width(2);
     // do the day
     otmp << day;
     if ( format >= 1 ) {
        // do the "T" that separates date from time
        otmp << "T";
        // hour is two digits
        otmp.width(2);
        // do the hour
        otmp << hour;
        if ( format >= 2 ) {
           // do the ":" that separates hour from minute
           otmp << ":";
           // minute is two digits
           otmp.width(2);
           // do the minute
           otmp << minute;
           if ( format >= 3 ) {
              // do the ":" that separates minute from second
              otmp << ":";
              if ( format == 3 ) {
                 // set the output to two integer digits
                 otmp.width(2);
                 // do the second
                 otmp << static_cast<int>(second + 0.5);
              } else {
                 // set the output to two digits before the decimal
                 // followed by four decimal digits
                 otmp.width(7);
                 otmp.precision(4);
                 otmp << second;
              }
     
           }
       }
     }
     
     // now retrieve the formatted string from the output stream.
     date = otmp.str();
     
     return date;

}



double CalGregorian::day1900( const std::string& date ) const
{
     // the component parts of a date
     int year, month, day, hours, minutes;
     float seconds;
     // the number of years since 1900
     int yr;
     // the lengths of the current month
     int monthlen;
     // a string input stream, used for converting strings to integers
     std::istringstream itmp;
     // the maximum day of this month
     int dmax;
     // correction term for leap years
     int leapadj;
     // holds the output day1900 result
     double result;

     // split the date string up into its component parts
     parseDate( date, &year, &month, &day, &hours, &minutes, &seconds );

     // sanity-check the parts
     if ( year < 1583 || month < 1 || month > 12 ) {
        std::cerr << "Bad date format: " << date << std::endl;
        throw (badGregorianDateFormat());
     }   

     // start with the number of years since 1900     
     yr = year - 1900;
     leapadj = 0;
     // how long is this month?
     monthlen = monthlengths[month];
     // what is the day of the year at the end of this month?
     dmax = daynums[month];
     // adjust for leap years as necessary
     if ( isLeap(year) ) {
        if ( month == 2 ) {
           // Leap February is longer
           monthlen++;
           dmax++;
        } 
        if ( month > 2 ) {
           // Leap post-February will need adjustment
           leapadj ++;
        }      
     }  
     // more sanity checking
     if ( day < 1 || day > monthlen ) {
        std::cerr << "Bad date format: " << date << std::endl;
        throw (badGregorianDateFormat());
     }        

     if ( day > dmax ) {
        day = dmax;
     }

     // calculate the days elapsed since 1900-01-01.
     result = static_cast<double>( yr*365+ (yr-1)/4+leapadj + daynums[month-1] + day );
     
     // add in the time as a fractional part of the day
     result = result + ( static_cast<double>(hours) 
                       + ( static_cast<double>(minutes)
                         + static_cast<double>(seconds)/60.0
                         )/60.0 
                       )/24.0;
     
     return result;    

}

std::string CalGregorian::date1900( double day, int format ) const  
{
     // the component parts of the day
     int year, month, dayofmonth, hours, minutes;
     float seconds;
     // a calculation term
     int residual;
     // used for adjusting for leap years
     int leapadj = 0;
     // string output stream used for converting integers to strings
     std::ostringstream otmp;
     // the time of the day
     double time;
     double day2;
     int y,d,m;
     int lastmon;
     
 
     if ( format == -999 ) {
        format = ofmt;
     }

 
     day2 = day + 693900.0;
     y = floor( (day2 + 1.478)/365.2425 );
     if ( day2 < (365*y+y/4-y/100+y/400) ) {
        y--;
     }
     d = floor( day2 - (365*y+y/4-y/100+y/400) );
     m = (100*d+52)/3060;
     month = ((m+2) % 12)+1;
     year = y+(m+2)/12;
     dayofmonth = d - (m*306+5)/10+1;
     
     
     // now do the time
     time = (day - trunc(day)) * 24.0 ;
     hours = static_cast<int>( time );
     minutes =  static_cast<int>(( time - static_cast<double>(hours) )*60.0 );
     seconds = ( time - static_cast<double>(hours) )*3600.0 - minutes*60.0;
     
     // now round according to the output format
     if ( format < 4 ) {
        // no floating-point seconds, so round them to integer
        seconds = roundf( seconds );
     }
     while ( seconds >= 60.0 ) {
        minutes = minutes + 1;
        seconds = seconds - 60.0;
     }
     if ( format < 3 ) {
        // no seconds at all--round the minutes
        if ( seconds >= 30.0 ) {
           seconds = 0;
           minutes = minutes + 1;
        }
     }     
     while ( minutes >= 60.0 ) {
        hours = hours + 1.0;
        minutes = minutes - 60.0;
     }
     if ( format < 2 ) {
        // no minutes--round to hours
        if ( minutes > 30 ) {
           minutes = 0;
           hours = hours + 1;
        }
     }
     // At this point, we could round hours to days.
     // But it seems wrong to round 2012-04-15T13:45:35 to
     // 2012-04-16. But what about 2012-04-15T23:59:59.99999?
     // It seems right to round that to 2012-04-16.
     while ( hours >= 24 ) {
        hours = hours - 24;
        dayofmonth = dayofmonth + 1;
        lastmon = monthlengths[month];
        if ( isLeap( year ) && ( month == 2 ) ) {
           lastmon++;
        }
        if ( dayofmonth > lastmon ) {
           dayofmonth = 1;
           month++;
           if ( month > 12 ) {
              month = 1;
              year ++;
           }
        }
     }
     
     return buildDate( year, month, dayofmonth, hours, minutes, seconds, format );
     
}

std::string CalGregorian::epochDate( time_t t, int format ) const
{
     struct tm *tparts;
     std::string result;

     result = "invalid";
     
     tparts = gmtime( &t );
     if ( tparts != NULLPTR ) {
     
        result = buildDate( tparts->tm_year + 1900 
                          , tparts->tm_mon + 1
                          , tparts->tm_mday 
                          , tparts->tm_hour 
                          , tparts->tm_min
                          , tparts->tm_sec * 1.0
                          , format );
     
     }
     
     return result;
}

int CalGregorian::format() const
{
    return ofmt;
}
      
void CalGregorian::format( const int fmt )
{
   if ( fmt >=0 && fmt <= 4 ) {
      ofmt = fmt;
   }
}
