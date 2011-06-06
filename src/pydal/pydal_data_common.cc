/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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
  \file pydal_data_common.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings DAL C++ classes in the \e data_common directory

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"

using DAL::CommonAttributes;
using DAL::Filename;
using DAL::HDF5Measure;
using DAL::HDF5Quantity;
using DAL::SAS_Settings;
using DAL::Timestamp;

//_____________________________________________________________________________
//                                                             CommonAttributes

void export_CommonAttributes () 
{
  bpl::class_<CommonAttributes>("CommonAttributes")
    .def( bpl::init<>())
    .def( bpl::init<Filename>())
    .def( bpl::init<Filename,string>())
    .def( "groupType", &CommonAttributes::groupType,
	  "Get the LOFAR group type.")
    .def( "filename", &CommonAttributes::filename,
	  "Get the name of the file.")
    .def( "setFilename", &CommonAttributes::setFilename,
	  "Set the name of the file.")
    .def( "filetype", &CommonAttributes::filetype,
	  "Get the type of the file.")
    .def( "filedate", &CommonAttributes::filedate,
	  "Get the file creation date.")
    .def( "setFiledate", &CommonAttributes::setFiledate,
	  "Set the file creation date.")
    .def( "telescope", &CommonAttributes::telescope,
	  "Get the name of the telescope.")
    .def( "setTelescope", &CommonAttributes::setTelescope,
	  "Set the name of the telescope.")
    .def( "projectID", &CommonAttributes::projectID,
	  "Unique identifier for the project.")
    .def( "setProjectID", &CommonAttributes::setProjectID,
	  "Set unique identifier for the project.")
    .def( "projectTitle", &CommonAttributes::projectTitle,
	  "Get name/title of the project")
    .def( "setProjectTitle", &CommonAttributes::setProjectTitle,
	  "Set name/title of the project")
    .def( "projectPI", &CommonAttributes::projectPI,
	  "Get name of the project's principal investigator")
    .def( "setProjectPI", &CommonAttributes::setProjectPI,
	  "Set name of the project's principal investigator")
    .def( "projectCoI", &CommonAttributes::projectCoI,
	  "Get name(s) of the project's co-PI(s)")
    .def( "setProjectCoI", &CommonAttributes::setProjectCoI,
	  "Set name(s) of the project's co-PI(s)")
    .def( "projectContact", &CommonAttributes::projectContact,
	  "Get names/Email-addresses of the project's primary contact person(s)")
    .def( "setProjectContact", &CommonAttributes::setProjectContact,
	  "Set names/Email-addresses of the project's primary contact person(s)")
    .def( "observationID", &CommonAttributes::observationID,
	  "Get unique identifier for the observation")
    .def( "startMJD", &CommonAttributes::startMJD,
	  "Get start date of the observation (MJD)")
    .def( "setStartMJD", &CommonAttributes::setStartMJD,
	  "Set start date of the observation (MJD)")
    .def( "startTAI", &CommonAttributes::startTAI,
	  "Get start date of the observation (TAI)")
    .def( "setStartTAI", &CommonAttributes::setStartTAI,
	  "Set start date of the observation (TAI)")
    .def( "startUTC", &CommonAttributes::startUTC,
	  "Get start date of the observation (UTC)")
    .def( "setStartUTC", &CommonAttributes::setStartUTC,
	  "Set start date of the observation (UTC)")
    .def( "endMJD", &CommonAttributes::endMJD,
	  "Get end date of the observation (MJD)")
    .def( "setEndMJD", &CommonAttributes::setEndMJD,
	  "Set end date of the observation (MJD)")
    .def( "endTAI", &CommonAttributes::endTAI,
	  "Get end date of the observation (TAI)")
    .def( "setEndTAI", &CommonAttributes::setEndTAI,
	  "Set end date of the observation (TAI)")
    .def( "endUTC", &CommonAttributes::endUTC,
	  "Get end date of the observation (UTC)")
    .def( "setEndUTC", &CommonAttributes::setEndUTC,
	  "Set end date of the observation (UTC)")
    .def( "nofStations", &CommonAttributes::nofStations,
	  "nof. stations used during the observation")
    .def( "stationsList", &CommonAttributes::stationsList,
	  "Get list of stations used during the observation")
    .def( "setStationsList", &CommonAttributes::setStationsList,
	  "Set list of stations used during the observation")
    .def( "frequencyMin", &CommonAttributes::frequencyMin,
	  "Observation minimum frequency")
    .def( "setFrequencyMin", &CommonAttributes::setFrequencyMin,
	  "Set observation minimum frequency")
    .def( "frequencyMax", &CommonAttributes::frequencyMax,
	  "Observation maximum frequency")
    .def( "setFrequencyMax", &CommonAttributes::setFrequencyMax,
	  "Set observation maximum frequency")
    .def( "frequencyCenter", &CommonAttributes::frequencyCenter,
	  "Observation center frequency")
    .def( "setFrequencyCenter", &CommonAttributes::setFrequencyCenter,
	  "Set observation center frequency")
    .def( "frequencyUnit", &CommonAttributes::frequencyUnit,
	  "Observation frequency physical units")
    .def( "setFrequencyUnit", &CommonAttributes::setFrequencyUnit,
	  "Set observation frequency physical units")
    ;
}

//_______________________________________________________________________________
//                                                                       Filename

void export_Filename ()
{
  /* Enumeration: File type */
  bpl::enum_<Filename::Type>("Type")
    .value("uv",      Filename::uv)
    .value("sky",     Filename::sky)
    .value("rm",      Filename::rm)
    .value("nfi",     Filename::nfi)
    .value("dynspec", Filename::dynspec)
    .value("bf",      Filename::bf)
    .value("tbb",     Filename::tbb)
    ;
  
  /* Enumeration: File extension */
  bpl::enum_<Filename::Extension>("Extension")
    .value("MS",     Filename::MS)
    .value("h5",     Filename::h5)
    .value("fits",   Filename::fits)
    .value("log",    Filename::log)
    .value("parset", Filename::parset)
    .value("lsm",    Filename::lsm)
    .value("IM",     Filename::IM)
    .value("PD",     Filename::PD)
    .value("vds",    Filename::vds)
    .value("gds",    Filename::gds)
    .value("conf",   Filename::conf)
    ;

  void (Filename::*summary1)() 
    = &Filename::summary;
  void (Filename::*summary2)(std::ostream &) 
    = &Filename::summary;
  
  bpl::class_<Filename>("Filename")
    // Construction
    .def( bpl::init<>())
    .def( bpl::init<string,string,Filename::Type,Filename::Extension>())
    // Parameters
    .def( "observationID", &Filename::observationID,
	  "Get the unique observation ID.")
    .def( "setObservationID", &Filename::setObservationID,
	  "Set the unique observation ID.")
    .def( "optionalDescription", &Filename::optionalDescription,
	  "Get optional descriptors.")
    .def( "setOptionalDescription", &Filename::setOptionalDescription,
	  "Set optional descriptors.")
    .def( "filetype", &Filename::filetype,
	  "Get the file type.")
    .def( "filetypeName", &Filename::filetypeName,
	  "Get the file type name.")
    .def( "setFiletype", &Filename::setFiletype,
	  "Set the file type.")
    .def( "extension", &Filename::extension,
	  "Get the file extension type.")
    .def( "extensionName", &Filename::extensionName,
	  "Get the file extension name.")
    .def( "setExtension", &Filename::setExtension,
	  "Set the file extension type.")
    .def( "filename", &Filename::filename,
	  "Get the name of the file.")
    // Methods
    .def("summary", summary1)
    .def("summary", summary2)
    ;
}

//_____________________________________________________________________________
//                                                                    Timestamp

void export_Timestamp () 
{
  //__________________________________________________________________
  // Enumeration: Month of the year
  
  bpl::enum_<Timestamp::Month>("Month")
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
  
  void (Timestamp::*setMonth1)(int const &)   = &Timestamp::setMonth;
  void (Timestamp::*setMonth2)(Timestamp::Month const &) = &Timestamp::setMonth;
  
  bpl::class_<Timestamp>("Timestamp")
    .def( bpl::init<>())
    .def( bpl::init<int,int,int,int,int,double>())
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

//_____________________________________________________________________________
//                                                                 SAS_Settings

void export_SAS_Settings () 
{
  bpl::class_<SAS_Settings>("SAS_Settings")
    .def( bpl::init<>())
    .def( "className", &SAS_Settings::className,
	  "Get the name of the class.")
    .def( "antennaSet", &SAS_Settings::antennaSet,
	  "Get the values for the AntennaSet field in SAS.")
    .def( "antennaSet", &SAS_Settings::antennaSet,
	  "Get the values for the AntennaSet field in SAS.")
    .def( "haveAntennaSet", &SAS_Settings::haveAntennaSet,
	  "Is name a valid value for the AntennaSet field in SAS?")
    .def( "filterSelection", &SAS_Settings::filterSelection,
	  "Get the values for the FilterSelection field in SAS.")
    .def( "haveFilterSelection", &SAS_Settings::haveFilterSelection,
	  "Is name a valid value for the AntennaSet field in SAS?")
    ;  
}

//_____________________________________________________________________________
//                                                                 HDF5Quantity

/*!
  \todo Complete implementation.
*/
void export_HDF5Quantity () 
{
  void (HDF5Quantity::*summary1)() 
    = &HDF5Quantity::summary;
  void (HDF5Quantity::*summary2)(std::ostream &) 
    = &HDF5Quantity::summary;
  
  bpl::class_<HDF5Quantity>("HDF5Quantity")
    .def( bpl::init<>())
    .def( bpl::init<string>())
    .def( bpl::init<string,string,string,string>())
    .def( bpl::init<string,double,string>())
    .def( bpl::init<string,vector<double>,string>())
    .def( bpl::init<string,vector<double>,vector<string> >())
    .def( bpl::init<HDF5Quantity>())
    .def( "className", &HDF5Quantity::className,
	  "Get the name of the class.")
    .def( "name", &HDF5Quantity::name,
	  "Get the name of the quantity used a base for the attributes.")
    .def( "setName", &HDF5Quantity::setName,
	  "Set the name of the quantity used a base for the attributes.")
    .def( "valueSuffix", &HDF5Quantity::valueSuffix,
	  "Get the suffix appended to the attribute storing the value(s).")
    .def( "setValueSuffix", &HDF5Quantity::setValueSuffix,
	  "Get the suffix appended to the attribute storing the value(s).")
    .def( "unitsSuffix", &HDF5Quantity::unitsSuffix,
	  "Get the suffix appended to the attribute storing the units(s).")
    .def( "setUnitsSuffix", &HDF5Quantity::setUnitsSuffix,
	  "Get the suffix appended to the attribute storing the units(s).")
    .def( "separator", &HDF5Quantity::separator,
	  "Get the separator inserted betwen the base and the suffix of the name.")
    .def( "setSeparator", &HDF5Quantity::setSeparator,
	  "Set the separator inserted betwen the base and the suffix of the name.")
    .def( "value", &HDF5Quantity::value,
	  "Get the numerical value(s).")
    .def( "setValue", &HDF5Quantity::setValue,
	  "Set the name of the class.")
    .def( "units", &HDF5Quantity::units,
	  "Get the physical unit(s) associated with the value(s).")
    .def( "setUnits", &HDF5Quantity::setUnits,
	  "Set the physical unit(s) associated with the value(s).")
    /* Public methods */
    .def("summary", summary1)
    .def("summary", summary2)
    .def( "nameValue", &HDF5Quantity::nameValue,
	  "Get the name for the attribute storing the value of the quantity.")
    .def( "nameUnits", &HDF5Quantity::nameUnits,
	  "Get the name for the attribute storing the units of the quantity.")
    ;  
}

//_____________________________________________________________________________
//                                                                  HDF5Measure

/*!
  \todo Complete implementation.
*/
void export_HDF5Measure () 
{
  void (HDF5Measure::*summary1)() 
    = &HDF5Measure::summary;
  void (HDF5Measure::*summary2)(std::ostream &) 
    = &HDF5Measure::summary;

  bpl::class_<HDF5Measure>("HDF5Measure")
    .def( bpl::init<>())
    .def( bpl::init<string>())
    .def( bpl::init<string,string,string,string,string>())
    .def( bpl::init<string,double,string,string>())
    .def( bpl::init<string,vector<double>,string,string>())
    .def( bpl::init<string,vector<double>,vector<string>,string>())
    .def( bpl::init<HDF5Quantity,string>())
    .def( bpl::init<HDF5Measure>())
    .def( "className", &HDF5Measure::className,
	  "Get the name of the class.")
    /* Public methods */
    .def("summary", summary1)
    .def("summary", summary2)
    ;  
}
