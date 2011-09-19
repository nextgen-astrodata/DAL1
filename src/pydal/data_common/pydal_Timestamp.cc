/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

/*!
  \file pydal_Timestamp.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::Timestamp class

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <data_common/Timestamp.h>

// namespace usage
using DAL::Timestamp;


void export_Timestamp ()
{

  //__________________________________________________________________
  // Enumerations
  
  boost::python::enum_<Timestamp::Month>("Month")
    .value("Jan",Timestamp::Jan)
    .value("Feb",Timestamp::Feb)
    .value("Mar",Timestamp::Mar)
    .value("Apr",Timestamp::Apr)
    .value("May",Timestamp::May)
    .value("Jun",Timestamp::Jun)
    .value("Jul",Timestamp::Jul)
    .value("Aug",Timestamp::Aug)
    .value("Sep",Timestamp::Sep)
    .value("Oct",Timestamp::Oct)
    .value("Nov",Timestamp::Nov)
    .value("Dec",Timestamp::Dec)
    ;
  
  //________________________________________________________
  // Specialisation of overloaded methods

  void (Timestamp::*setMonth1)(int const &)   = &Timestamp::setMonth;
  void (Timestamp::*setMonth2)(Timestamp::Month const &) = &Timestamp::setMonth;
  
  //________________________________________________________
  // Bindings for class and its methods

  boost::python::class_<Timestamp>("Timestamp")
    .def( boost::python::init<>())
    .def( boost::python::init<int,int,int,int,int,double>())
    .def( "year", &Timestamp::year,
	  "Get the numerical value of the year.")
    .def( "setYear", &Timestamp::setYear,
	  "Set the numerical value of the year.")
    .def( "month", &Timestamp::month,
	  "Get the numerical value of the month.")
    .def( "setMonth", setMonth1,
	  "Set the numerical value of the month.")
    .def( "setMonth", setMonth2,
	  "Set the numerical value of the month.")
    .def( "day", &Timestamp::day,
	  "Get the numerical value of the day.")
    .def( "setDay", &Timestamp::setDay,
	  "Set the numerical value of the day.")
    .def( "hour", &Timestamp::hour,
	  "Get the numerical value of the hour.")
    .def( "setHour", &Timestamp::setHour,
	  "Set the numerical value of the hour.")
    .def( "minute", &Timestamp::minute,
	  "Get the numerical value of the minute.")
    .def( "setMinute", &Timestamp::setMinute,
	  "Set the numerical value of the minute.")
    .def( "second", &Timestamp::second,
	  "Get the numerical value of the second.")
    .def( "setSecond", &Timestamp::setSecond,
	  "Set the numerical value of the second.")
    .def( "fractionOfSecond", &Timestamp::fractionOfSecond,
	  "Get the fraction of the second.")
    .def( "dayOfWeek", &Timestamp::dayOfWeek,
	  "Get the name of the day of the week.")
    .def( "ymd", &Timestamp::ymd,
	  "Get string combining year, month and day.")
    .def( "hms", &Timestamp::hms,
	  "Get the time of day.")
    .def( "iso8601", &Timestamp::iso8601,
	  "Retrieve ISO 8601 conform version of the timestamp string.")
    .def( "rfc2822", &Timestamp::rfc2822,
	  "Retrieve RCF 2822 conform version of the timestamp string.")
    ;

}
