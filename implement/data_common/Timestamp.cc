/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2006                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <Timestamp.h>

namespace DAL {
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                    Timestamp
  
  Timestamp::Timestamp ()
  {
    setTime ();
  }

  //_____________________________________________________________________________
  //                                                                    Timestamp
  
  Timestamp::Timestamp (int const &year,
			int const &month,
			int const &day,
			int const &hour,
			int const &minute,
			double const &second)
  {
    setTime ();
    //
    valYear_p        = year;
    valMonth_p       = month;
    valDay_p         = day;
    valMinute_p      = hour;
    valMinute_p      = minute;
    setSecond(second);
  }
  
  //_____________________________________________________________________________
  //                                                                    Timestamp
  
  Timestamp::Timestamp (Timestamp const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  Timestamp::~Timestamp ()
  {
    destroy();
  }
  
  void Timestamp::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  Timestamp& Timestamp::operator= (Timestamp const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }

  //_____________________________________________________________________________
  //                                                                         copy
  
  void Timestamp::copy (Timestamp const &other)
  {
    rawtime_p        = other.rawtime_p;

    valYear_p        = other.valYear_p;
    valMonth_p       = other.valMonth_p;
    valDay_p         = other.valDay_p;
    valHour_p        = other.valHour_p;
    valMinute_p      = other.valMinute_p;
    valSecond_p      = other.valSecond_p;
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                      setYear

  void Timestamp::setYear (int const &year)
  {
    valYear_p = year;
    setRawtime();
  }

  //_____________________________________________________________________________
  //                                                                     setMonth

  void Timestamp::setMonth (Month const &month)
  {
    int m = 0;

    switch (month) {
    case Timestamp::Jan:
      m = 1;
      break;
    case Timestamp::Feb:
      m = 2;
      break;
    case Timestamp::Mar:
      m = 3;
      break;
    case Timestamp::Apr:
      m = 4;
      break;
    case Timestamp::May:
      m = 5;
      break;
    case Timestamp::Jun:
      m = 6;
      break;
    case Timestamp::Jul:
      m = 7;
      break;
    case Timestamp::Aug:
      m = 8;
      break;
    case Timestamp::Sep:
      m = 9;
      break;
    case Timestamp::Oct:
      m = 10;
      break;
    case Timestamp::Nov:
      m = 11;
      break;
    case Timestamp::Dec:
      m = 12;
      break;
    };
    
    setMonth (m);
  }
  
  //_____________________________________________________________________________
  //                                                                     setMonth

  /*!
    \param month -- The numerical value for the month, [1-12]
  */
  void Timestamp::setMonth (int const &month)
  {
    /* Check if the input value is within the acceptable range. */
    if (month >= 1 && month <= 12) {
      valMonth_p = month;
      setRawtime();
    } else {
      std::cerr << "[Timestamp::setMonth] Input value rejected - out of range!"
		<< std::endl;
    }
  }

  //_____________________________________________________________________________
  //                                                                       setDay
  
  void Timestamp::setDay (int const &day)
  {
    /* Check if the input value is within the acceptable range. */
    if (day > 0 && day < 32) {
      valDay_p = day;
      setRawtime();
    } else {
      std::cerr << "[Timestamp::setDay] Input value rejected - out of range!"
		<< std::endl;
    }
  }

  //_____________________________________________________________________________
  //                                                                      setHour
  
  void Timestamp::setHour (int const &hour)
  {
    /* Check if the input value is within the acceptable range. */
    if (hour >= 0 && hour <= 23) {
      valHour_p = hour;
      setRawtime();
    } else {
      std::cerr << "[Timestamp::setHour] Input value rejected - out of range!"
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                    setMinute
  
  void Timestamp::setMinute (int const &minute)
  {
    /* Check if the input value is within the acceptable range. */
    if (minute >= 0 && minute <= 59) {
      valMinute_p = minute;
      setRawtime();
    } else {
      std::cerr << "[Timestamp::setMinute] Input value rejected - out of range!"
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                       second
  
  int Timestamp::second ()
  {
    int fullSecond = floor(valSecond_p);

    return fullSecond;
  }
  
  //_____________________________________________________________________________
  //                                                                    setSecond
  
  void Timestamp::setSecond (double const &second)
  {
    int fullSecond = floor(second);
    /* Check if the input value is within the acceptable range. */
    if (fullSecond >= 0 && fullSecond <= 59) {
      valSecond_p = second;
      setRawtime();
    } else {
      std::cerr << "[Timestamp::setSecond] Input value rejected - out of range!"
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void Timestamp::summary (std::ostream &os)
  {
    os << "[Timestamp] Summary of internal parameters." << std::endl;
    os << "-- Raw time    = " << rawtime_p     << std::endl;
    os << "-- ctime       = " << rfc2822();
    os << "-- iso8601     = " << iso8601()     << std::endl;
    os << "-- ymd         = " << ymd()         << std::endl;
    os << "-- hms         = " << hms()         << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                     asString

  std::string Timestamp::asString (int const &val)
  {
    std::stringstream out;
    
    if (val < 10) {
      out << "0" << val;
    } else {
      out << val;
    }

    return out.str();
  }

  //_____________________________________________________________________________
  //                                                                      setTime

  void Timestamp::setTime ()
  {
    /* Initialize timestamp to current time */
    time (&rawtime_p);
    /* Store the time information */
    setTime (rawtime_p);
  }
  
  //_____________________________________________________________________________
  //                                                                      setTime
  
  /*!
    \param rawtime - The raw time in UNIX seconds
  */
  void Timestamp::setTime (time_t const &rawtime)
  {
    /* Store the raw time information */
    rawtime_p = rawtime;
    /* Convert time information to time structure */
    struct tm * timeinfo;
    timeinfo = localtime(&rawtime);
    /* Store the time information with the internal variables */
    valYear_p          = timeinfo->tm_year+1900;
    valMonth_p         = timeinfo->tm_mon+1;
    valDay_p           = timeinfo->tm_mday;
    valHour_p          = timeinfo->tm_hour;
    valMinute_p        = timeinfo->tm_min;
    valSecond_p        = timeinfo->tm_sec;
  }
  
  //_____________________________________________________________________________
  //                                                                   setRawtime

  void Timestamp::setRawtime ()
  {
    /* Convert time information to time structure */
    struct tm * timeinfo;
    timeinfo = localtime(&rawtime_p);
    /* Update the value inside the time structure */
    timeinfo->tm_year = valYear_p-1900;
    timeinfo->tm_mon  = valMonth_p-1;
    timeinfo->tm_mday = valDay_p;
    timeinfo->tm_hour = hour();
    timeinfo->tm_min  = minute();
    timeinfo->tm_sec  = second();
    /* Update the value of the raw time */
    rawtime_p = mktime (timeinfo);
  }

  //_____________________________________________________________________________
  //                                                                    dayOfWeek

  /*!
    \param longName -- Return the name of the day in its long format? If set 
           <tt>true</tt> then e.g. in instead of <i>Mon</i> the full name of the
	   day will be return -- in this case <i>Monday</i>.
  */
  std::string Timestamp::dayOfWeek (bool const &longName)
  {
    std::string wday;
    const char* tmp = rfc2822().c_str();

    wday += tmp[0];
    wday += tmp[1];
    wday += tmp[2];

    if (longName) {
      if (wday == "Mon") {
	wday = "Monday";
      }
      else if (wday == "Tue") {
	wday = "Tuesday";
      }
      else if (wday == "Wed") {
	wday = "Wednesday";
      }
      else if (wday == "Thu") {
	wday = "Thursday";
      }
      else if (wday == "Friday") {
	wday = "Friday";
      }
      else if (wday == "Sat") {
	wday = "Saturday";
      }
      else {
	wday = "Sunday";
      }
    }

    return wday;
  }

  //_____________________________________________________________________________
  //                                                                          hms

  /*!
    \param sep -- Separator inserted between the individual elements of the
           string. By default <tt>sep=":"</tt>, which will yield
	   <tt>hh:mm:ss</tt>.
  */
  std::string Timestamp::hms (std::string const &sep)
  {
    std::stringstream out;
    
    if (sep == "") {
      out << asString(valHour_p)
	  << asString(valMinute_p)
	  << asString(valSecond_p);
    }
    else {
      out << asString(valHour_p) << sep
	  << asString(valMinute_p) << sep
	  << asString(valSecond_p);
    }

    return out.str();
  }
  
  //_____________________________________________________________________________
  //                                                                          ymd
  
  /*!
    \param sep -- Separator inserted between the individual elements of the
           string. By default <tt>sep="-"</tt>, which will yield
	   <tt>yyyy-mm-dd</tt>.
  */
  std::string Timestamp::ymd (std::string const &sep)
  {
    std::stringstream out;
    
    if (sep == "") {
      out << asString(valYear_p)
	  << asString(valMonth_p)
	  << asString(valDay_p);
    } else {
      out << asString(valYear_p) << sep
	  << asString(valMonth_p) << sep
	  << asString(valDay_p);
    }
    
    return out.str();
  }

  //_____________________________________________________________________________
  //                                                                      iso8601
  
  /*!
    \return iso -- ISO 8601 conform version of the timestamp string
  */
  std::string Timestamp::iso8601 ()
  {
    std::stringstream out;
    
    out << ymd() << "T" << hms() << ".00Z";
    
    return out.str();
  }
  
}
