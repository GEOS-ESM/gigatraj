/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/




#include <iostream>
#include <sstream>
#include "gigatraj/gigatraj.hh"
#include "gigatraj/CalGregorian.hh"

#include "test_utils.hh"

using namespace gigatraj;
using std::cerr;
using std::endl;
using std::ostringstream;
using std::istringstream;
using std::operator<<;
using std::operator>>;

int main() 
{
    CalGregorian cal;
    std::string date;
    std::string valdate;
    double day;
    int status;
    bool query;
    double val;
    std::string valstr;
    int year,month,dom,hour,minute;
    
    //================================= method isLeap(int)
    year = 1999;
    if ( cal.isLeap(year) ) {
       cerr << year << " was NOT a leap year!" << endl;
       exit(1);
    }
    year = 2004;
    if ( ! cal.isLeap(year) ) {
       cerr << year << " WAS a leap year!" << endl;
       exit(1);
    }
    year = 1900;
    if ( cal.isLeap(year) ) {
       cerr << year << " was NOT a leap year!" << endl;
       exit(1);
    }
    year = 2000;
    if ( ! cal.isLeap(year) ) {
       cerr << year << " WAS a leap year!" << endl;
       exit(1);
    }
    
    //================================= method parseDate()
    date = "1965-11-27";
    cal.parseDate(date, &year, &month, &dom );
    if ( year!=1965 || month != 11 || dom != 27  ) {
       cerr << "parseDate failed a: " << date << " => " << year << "/" << month << "/" << dom << ", " << hour << ":" << minute << endl;
       exit(1);
    }
    date = "1965-11-27";
    cal.parseDate(date, &year, &month, &dom, &hour, &minute );
    if ( year!=1965 || month != 11 || dom != 27 || hour != 0 || minute != 0 ) {
       cerr << "parseDate failed b: " << date << " => " << year << "/" << month << "/" << dom << ", " << hour << ":" << minute << endl;
       exit(1);
    }
    cal.parseDate(date, &year, &month, &dom );
    if ( year!=1965 || month != 11 || dom != 27 || hour != 0 || minute != 0 ) {
       cerr << "parseDate failed c: " << date << " => " << year << "/" << month << "/" << dom << ", " << hour << ":" << minute << endl;
       exit(1);
    }
    date = "1965-11-27T18";
    cal.parseDate(date, &year, &month, &dom, &hour, &minute );
    if ( year!=1965 || month != 11 || dom != 27 || hour != 18 || minute != 0 ) {
       cerr << "parseDate failed d: " << date << " => " << year << "/" << month << "/" << dom << ", " << hour << ":" << minute << endl;
       exit(1);
    }     
    date = "1965-11-27T18:45";
    cal.parseDate(date, &year, &month, &dom, &hour, &minute );
    if ( year!=1965 || month != 11 || dom != 27 || hour != 18 || minute != 45 ) {
       cerr << "parseDate failed e: " << date << " => " << year << "/" << month << "/" << dom << ", " << hour << ":" << minute << endl;
       exit(1);
    }
    date = "1900-01-01T00:00";
    cal.parseDate(date, &year, &month, &dom, &hour, &minute );
    if ( year!=1900 || month != 1 || dom != 1 || hour != 0 || minute != 0 ) {
       cerr << "parseDate failed e: " << date << " => " << year << "/" << month << "/" << dom << ", " << hour << ":" << minute << endl;
       exit(1);
    }
    
    //================================= method buildDate()
    valstr = date;
    date = "foobar";
    date = cal.buildDate( year, month, dom, hour, minute );
    if ( date != valstr ) {
        cerr << "buildDate failed a: " << date << " vs. " << valstr << endl;
        exit(1);
    }
    valstr = "1972-06-02T00:00";
    date = "foobar";
    date = cal.buildDate( 1972, 6, 2 );
    if ( date != valstr ) {
        cerr << "buildDate failed b: " << date << " vs. " << valstr << endl;
        exit(1);
    }

    //================================= method isLeap(std::string)
    date = "1999-03-23";
    if ( cal.isLeap(date) ) {
       cerr << date << " was NOT in a leap date!" << endl;
       exit(1);
    }
    date = "2004-02-28";
    if ( ! cal.isLeap(date) ) {
       cerr << date << " WAS in a leap date!" << endl;
       exit(1);
    }
    date = "1900-07-05";
    if ( cal.isLeap(date) ) {
       cerr << date << " was NOT in a leap date!" << endl;
       exit(1);
    }
    date = "2000-09-15";
    if ( ! cal.isLeap(date) ) {
       cerr << date << " WAS in a leap date!" << endl;
       exit(1);
    }
    
    status = 0;
    try {
        date = "blahblah";
        query = cal.isLeap(date);
    } catch (CalGregorian::badGregorianDateFormat err) {
        status = 1;
    }    
    if ( status != 1 ) {
       cerr << "isLeap failed to catch a bad date" << endl;
       exit(1);
    } 
    
    
    //============================= method day1900
    date = "1900-01-01";
    day = cal.day1900(date);
    val = 1;
    if ( mismatch( day, val ) ) {
       cerr << "day1900 failed: " << date << ": " << day << " vs. " << val << endl;
       exit(1);
    }    
    date = "1900-12-31";
    day = cal.day1900(date);
    val = 365;
    if ( mismatch( day, val ) ) {
       cerr << "day1900 failed: " << date << ": " << day << " vs. " << val << endl;
       exit(1);
    }    
    date = "1901-01-01";
    day = cal.day1900(date);
    val = 366;
    if ( mismatch( day, val ) ) {
       cerr << "day1900 failed: " << date << ": " << day << " vs. " << val << endl;
       exit(1);
    }    
    date = "2011-03-29";
    day = cal.day1900(date);
    val = 40630;
    if ( mismatch( day, val ) ) {
       cerr << "day1900 failed: " << date << ": " << day << " vs. " << val << endl;
       exit(1);
    }    
    date = "2011-03-29T06";
    day = cal.day1900(date);
    val = 40630.25;
    if ( mismatch( day, val ) ) {
       cerr << "day1900 failed: " << date << ": " << day << " vs. " << val << endl;
       exit(1);
    }    
    date = "2011-03-29T06:30";
    day = cal.day1900(date);
    val = 40630.270833333;
    if ( mismatch( day, val ) ) {
       cerr << "day1900 failed: " << date << ": " << day << " vs. " << val << endl;
       exit(1);
    } 
    
    
    // try some badly formatted dates
    //  obvious
    status = 0;
    date = "blahblah";
    try {
        day = cal.day1900(date);
    } catch (CalGregorian::badGregorianDateFormat err) {
        status = 1;
    }    
    if ( status != 1 ) {
       cerr << "isLeap failed to catch a bad date: " << date << endl;
       exit(1);
    } 
    //  less obvious
    status = 0;
    date = "2000-13-20";
    try {
        day = cal.day1900(date);
    } catch (CalGregorian::badGregorianDateFormat err) {
        status = 1;
    }    
    if ( status != 1 ) {
       cerr << "day1900 failed to catch a bad date: " << date << endl;
       exit(1);
    } 
    status = 0;
    date = "2000-05-42";
    try {
        day = cal.day1900(date);
    } catch (CalGregorian::badGregorianDateFormat err) {
        status = 1;
    }    
    if ( status != 1 ) {
       cerr << "day1900 failed to catch a bad date: " << date << endl;
       exit(1);
    } 
    //    subtle
    status = 0;
    date = "2003-02-29";
    try {
        day = cal.day1900(date);
    } catch (CalGregorian::badGregorianDateFormat err) {
        status = 1;
    }    
    if ( status != 1 ) {
       cerr << "day1900 failed to catch a bad date: " << date << endl;
       exit(1);
    } 
      

    //============================= method date1900
    day = 1;
    date = cal.date1900(day);
    valstr = "1900-01-01T00:00";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    day = 365;
    date = cal.date1900(day);
    valstr = "1900-12-31T00:00";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    day = 366;
    date = cal.date1900(day);
    valstr = "1901-01-01T00:00";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    day = 40630;
    date = cal.date1900(day);
    valstr = "2011-03-29T00:00";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    day = 40630;
    date = cal.date1900(day);
    valstr = "2011-03-29T00:00";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    day = 40630.25;
    date = cal.date1900(day);
    valstr = "2011-03-29T06:00";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    day = 40630.270833333;
    date = cal.date1900(day);
    valstr = "2011-03-29T06:30";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    


    // process time 0
    date = "1899-12-31T00:00";
    cal.parseDate(date, &year, &month, &dom, &hour, &minute );
    if ( year!=1899 || month != 12 || dom != 31 || hour != 0 || minute != 0 ) {
       cerr << "parseDate failed e: " << date << " => " << year << "/" << month << "/" << dom << ", " << hour << ":" << minute << endl;
       exit(1);
    }

    date = "1899-12-31T00:00";
    day = cal.day1900(date);
    val = 0.0;
    if ( mismatch( day, val ) ) {
       cerr << "day1900 failed: " << date << ": " << day << " vs. " << val << endl;
       exit(1);
    } 
    day = 0.0;
    date = cal.date1900(day);
    valstr = "1899-12-31T00:00";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    



    // test output formats
         
    day = 41186.56285;
    date = cal.date1900(day, 4);
    valstr = "2012-10-05T13:30:30.2400";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 3);
    valstr = "2012-10-05T13:30:30";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 2);
    valstr = "2012-10-05T13:31";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 1);
    valstr = "2012-10-05T14";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 0);
    valstr = "2012-10-05";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    


    day = 41186.10004;
    date = cal.date1900(day, 4);
    valstr = "2012-10-05T02:24:03.4560";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 3);
    valstr = "2012-10-05T02:24:03";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 2);
    valstr = "2012-10-05T02:24";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 1);
    valstr = "2012-10-05T02";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 0);
    valstr = "2012-10-05";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    


    day = 41186.99999;
    date = cal.date1900(day, 4);
    valstr = "2012-10-05T23:59:59.1360";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 3);
    valstr = "2012-10-05T23:59:59";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 2);
    valstr = "2012-10-06T00:00";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 1);
    valstr = "2012-10-06T00";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    
    date = cal.date1900(day, 0);
    valstr = "2012-10-06";
    if ( date != valstr ) {
       cerr << "date1900 failed: " << day << ": " << date << " vs. " << valstr << endl;
       exit(1);
    }    

    exit(0);

}
