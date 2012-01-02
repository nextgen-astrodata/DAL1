/***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#include <data_common/CommonAttributes.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                             CommonAttributes

  /*!
    Creates an object with just a minimal subset of its internal parameters
    assigned:
    - GROUPTYPE   = "Root"
    - TELESCOPE   = "LOFAR"
  */
  CommonAttributes::CommonAttributes ()
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                             CommonAttributes

  /*!
    \param filename -- The name of the file.
  */
  CommonAttributes::CommonAttributes (DAL::Filename const &filename)
  {
    init ();
    setFilename (filename);
  }

  //_____________________________________________________________________________
  //                                                             CommonAttributes
  
  /*!
    \param filename -- The name of the file.
    \param filedate -- The file creation date.
  */
  CommonAttributes::CommonAttributes (DAL::Filename const &filename,
				      std::string const &filedate)
  {
    init ();
    setFilename (filename);
    setFiledate (filedate);
  }

  //_____________________________________________________________________________
  //                                                             CommonAttributes
  
  CommonAttributes::CommonAttributes (std::string const &projectID,
				      std::string const &projectTitle,
				      std::string const &projectPI,
				      std::string const &projectCoI,
				      std::string const &projectContact)
  {
    init ();
    setProjectID (projectID);
    setProjectTitle (projectTitle);
    setProjectPI (projectPI);
    setProjectCoI (projectCoI);
    setProjectContact (projectContact);
  }
  
  //_____________________________________________________________________________
  //                                                             CommonAttributes
  
#ifdef DAL_WITH_HDF5    
  CommonAttributes::CommonAttributes (hid_t const &locationID)
  {
    // Initialize parameters to default values
    init ();
    // Read the attribute values from the file
    h5read (locationID);
  }
#endif
  
  //_____________________________________________________________________________
  //                                                             CommonAttributes

  CommonAttributes::CommonAttributes (CommonAttributes const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  CommonAttributes::~CommonAttributes ()
  {
    destroy();
  }
  
  void CommonAttributes::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  CommonAttributes& CommonAttributes::operator= (CommonAttributes const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }

  //_____________________________________________________________________________
  //                                                                         copy
  
  void CommonAttributes::copy (CommonAttributes const &other)
  {
    itsAttributes.clear();
    if (!other.itsAttributes.empty()) {
      itsAttributes = other.itsAttributes;
    }
    
    itsGroupType           = other.itsGroupType;
    itsFilename            = other.itsFilename;
    itsFiletype            = other.itsFiletype;
    itsFiledate            = other.itsFiledate;
    itsTelescope           = other.itsTelescope;
    /* Common LOFAR attributes for description of project */
    itsProjectID           = other.itsProjectID;
    itsProjectTitle        = other.itsProjectTitle;
    itsProjectPI           = other.itsProjectPI;
    itsProjectCoI          = other.itsProjectCoI;
    itsProjectContact      = other.itsProjectContact;
    /* Common LOFAR attributes for description of observation */
    itsObservationID   = other.itsObservationID;
    itsStartMJD        = other.itsStartMJD;
    itsStartTAI        = other.itsStartTAI;
    itsStartUTC        = other.itsStartUTC;
    itsEndMJD          = other.itsEndMJD;
    itsEndTAI          = other.itsEndTAI;
    itsEndUTC          = other.itsEndUTC;
    
    itsStationsList.clear();
    if (other.itsStationsList.empty()) {
      itsNofStations  = 0;
    } else {
      itsStationsList = other.itsStationsList;
      itsNofStations  = other.itsNofStations;
    } 
    
    itsFrequencyMin     = other.itsFrequencyMin;
    itsFrequencyMax     = other.itsFrequencyMax;
    itsFrequencyCenter  = other.itsFrequencyCenter;
    itsFrequencyUnit    = other.itsFrequencyUnit;
    itsNofBitsPerSample = other.itsNofBitsPerSample;
    /*________________________________________________________________
    */
    itsObserver            = other.itsObserver;
    itsAntennaSet          = other.itsAntennaSet;
    itsFilterSelection     = other.itsFilterSelection;
    itsClockFrequency      = other.itsClockFrequency;
    itsClockFrequencyUnit  = other.itsClockFrequencyUnit;
    itsTarget              = other.itsTarget;
    itsSystemVersion       = other.itsSystemVersion;
    itsPipelineName        = other.itsPipelineName;
    itsPipelineVersion     = other.itsPipelineVersion;
    itsNotes               = other.itsNotes;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                         setAttributesProject
  
  /*!
    \param projectID      -- Unique identifier for the project
    \param projectTitle   -- Name of the project
    \param projectPI      -- Name of the project's principal investigator
    \param projectCoI    -- Name(s) of the project's co-PI(s)
    \param projectContact -- Names/Email-addresses of the project's primary
           contact person(s)
  */
  void CommonAttributes::setAttributesProject (std::string const &projectID,
					       std::string const &projectTitle,
					       std::string const &projectPI,
					       std::string const &projectCoI,
					       std::string const &projectContact)
  {
    setProjectID (projectID);
    setProjectTitle (projectTitle);
    setProjectPI (projectPI);
    setProjectCoI (projectCoI);
    setProjectContact (projectContact);
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void CommonAttributes::summary (std::ostream &os)
  {
    os << "[CommonAttributes] Summary of internal parameters." << std::endl;
    
    os << "-- GROUPTYPE                       = " << groupType()        << std::endl;
    os << "-- FILENAME                        = " << filename()         << std::endl;
    os << "-- FILETYPE                        = " << filetype()         << std::endl;
    os << "-- FILEDATE                        = " << filedate()         << std::endl;
    os << "-- TELESCOPE                       = " << telescope()        << std::endl;
    os << "-- OBSERVER                        = " << observer()         << std::endl;
    /*________________________________________________________________
      Common LOFAR attributes for description of project 
    */
    os << "-- PROJECT_ID                      = " << itsProjectID       << std::endl;
    os << "-- PROJECT_TITLE                   = " << itsProjectTitle    << std::endl;
    os << "-- PROJECT_PI                      = " << itsProjectPI       << std::endl;
    os << "-- PROJECT_CO_I                    = " << itsProjectCoI      << std::endl;
    os << "-- PROJECT_CONTACT                 = " << itsProjectContact  << std::endl;
    /*________________________________________________________________
      Common LOFAR attributes for description of observation
    */
    os << "-- OBSERVATION_ID                  = " << itsObservationID    << std::endl;
    os << "-- OBSERVATION_START_MJD           = " << itsStartMJD         << std::endl;
    os << "-- OBSERVATION_END_MJD             = " << itsEndMJD           << std::endl;
    os << "-- OBSERVATION_START_TAI           = " << itsStartTAI         << std::endl;
    os << "-- OBSERVATION_END_TAI             = " << itsEndTAI           << std::endl;
    os << "-- OBSERVATION_START_UTC           = " << itsStartUTC         << std::endl;
    os << "-- OBSERVATION_END_UTC             = " << itsEndUTC           << std::endl;
    os << "-- OBSERVATION_NOF_STATIONS        = " << itsNofStations      << std::endl;
    os << "-- OBSERVATION_STATIONS_LIST       = " << itsStationsList     << std::endl;
    os << "-- OBSERVATION_FREQUENCY_MIN       = " << itsFrequencyMin     << std::endl;
    os << "-- OBSERVATION_FREQUENCY_CENTER    = " << itsFrequencyCenter  << std::endl;
    os << "-- OBSERVATION_FREQUENCY_MAX       = " << itsFrequencyMax     << std::endl;
    os << "-- OBSERVATION_FREQUENCY_UNIT      = " << itsFrequencyUnit    << std::endl;
    os << "-- OBSERVATION_NOF_BITS_PER_SAMPLE = " << itsNofBitsPerSample << std::endl;
    /*________________________________________________________________
     */
    os << "-- ANTENNA_SET                     = " << antennaSet()         << std::endl;
    os << "-- FILTER_SELECTION                = " << filterSelection()    << std::endl;
    os << "-- CLOCK_FREQUENCY                 = " << clockFrequency()     << std::endl;
    os << "-- CLOCK_FREQUENCY_UNIT            = " << clockFrequencyUnit() << std::endl;
    os << "-- TARGET                          = " << target()             << std::endl;
    os << "-- SYSTEM_VERSION                  = " << systemVersion()      << std::endl;
    os << "-- PIPELINE_NAME                   = " << pipelineName()       << std::endl;
    os << "-- PIPELINE_VERSION                = " << pipelineVersion()    << std::endl;
    os << "-- NOTES                           = " << notes()              << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  void CommonAttributes::init ()
  {
    /* Set up the list of attributes */
    itsAttributes.clear();
    itsAttributes.insert("GROUPTYPE");
    itsAttributes.insert("FILENAME");
    itsAttributes.insert("FILETYPE");
    itsAttributes.insert("FILEDATE");
    itsAttributes.insert("TELESCOPE");
    itsAttributes.insert("OBSERVER");
    itsAttributes.insert("PROJECT_ID");
    itsAttributes.insert("PROJECT_TITLE");
    itsAttributes.insert("PROJECT_PI");
    itsAttributes.insert("PROJECT_CO_I");
    itsAttributes.insert("PROJECT_CONTACT");
    itsAttributes.insert("OBSERVATION_ID");
    itsAttributes.insert("OBSERVATION_START_MJD");
    itsAttributes.insert("OBSERVATION_START_TAI");
    itsAttributes.insert("OBSERVATION_START_UTC");
    itsAttributes.insert("OBSERVATION_END_MJD");
    itsAttributes.insert("OBSERVATION_END_TAI");
    itsAttributes.insert("OBSERVATION_END_UTC");
    itsAttributes.insert("OBSERVATION_NOF_STATIONS");
    itsAttributes.insert("OBSERVATION_STATIONS_LIST");
    itsAttributes.insert("OBSERVATION_FREQUENCY_MIN");
    itsAttributes.insert("OBSERVATION_FREQUENCY_MAX");
    itsAttributes.insert("OBSERVATION_FREQUENCY_CENTER");
    itsAttributes.insert("OBSERVATION_FREQUENCY_UNIT");
    itsAttributes.insert("OBSERVATION_NOF_BITS_PER_SAMPLE");
    itsAttributes.insert("ANTENNA_SET");
    itsAttributes.insert("FILTER_SELECTION");
    itsAttributes.insert("CLOCK_FREQUENCY");
    itsAttributes.insert("CLOCK_FREQUENCY_UNIT");
    itsAttributes.insert("TARGET");
    itsAttributes.insert("SYSTEM_VERSION");
    itsAttributes.insert("PIPELINE_NAME");
    itsAttributes.insert("PIPELINE_VERSION");
    itsAttributes.insert("OBSERVATION_NOF_STATIONS");
    itsAttributes.insert("OBSERVATION_STATIONS_LIST");
    itsAttributes.insert("ICD_NUMBER");
    itsAttributes.insert("ICD_VERSION");
    itsAttributes.insert("NOTES");
    /* Default values of the attributes */
    std::string undefined ("UNDEFINED");
    //
    itsGroupType          = "Root";
    itsFilename           = undefined;
    itsFiletype           = undefined;
    itsFiledate           = undefined;
    itsTelescope          = "LOFAR";
    itsObserver           = undefined;
    itsProjectID          = undefined;
    itsProjectTitle       = undefined;
    itsProjectPI          = undefined;
    itsProjectCoI         = undefined;
    itsProjectContact     = undefined;
    itsObservationID      = undefined;
    itsStartMJD           = undefined;
    itsStartTAI           = undefined;
    itsStartUTC           = undefined;
    itsEndMJD             = undefined;
    itsEndTAI             = undefined;
    itsEndUTC             = undefined;
    itsNofStations        = 0;
    itsStationsList       = std::vector<std::string> (1,undefined);
    itsFrequencyMin       = 0;
    itsFrequencyMax       = 0;
    itsFrequencyCenter    = 0;
    itsFrequencyUnit      = "Hz";
    itsNofBitsPerSample   = 0;
    itsAntennaSet         = undefined;
    itsFilterSelection    = undefined;
    itsClockFrequency     = 0;
    itsClockFrequencyUnit = undefined;
    itsTarget             = undefined;
    itsSystemVersion      = undefined;
    itsPipelineName       = undefined;
    itsPipelineVersion    = undefined;
    itsIcdNumber          = undefined;
    itsIcdVersion         = undefined;
    itsNotes              = undefined;
  }
  
#ifdef DAL_WITH_HDF5
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  /*!
    \param id -- Object identifier for the HDF5 group or dataset to which the 
           attributes will be attached.
  */
  bool CommonAttributes::h5write (hid_t const &id)
  {
    bool status (true);
    
    HDF5Attribute::write (id, "GROUPTYPE", itsGroupType );
    HDF5Attribute::write (id, "FILENAME",  itsFilename );
    HDF5Attribute::write (id, "FILETYPE",  itsFiletype );
    HDF5Attribute::write (id, "FILEDATE",  itsFiledate );
    HDF5Attribute::write (id, "TELESCOPE", itsTelescope );
    HDF5Attribute::write (id, "OBSERVER",  itsObserver );
    /*________________________________________________________________
      Common LOFAR attributes for description of project 
    */
    HDF5Attribute::write (id, "PROJECT_ID",      itsProjectID );
    HDF5Attribute::write (id, "PROJECT_TITLE",   itsProjectTitle );
    HDF5Attribute::write (id, "PROJECT_PI",      itsProjectPI );
    HDF5Attribute::write (id, "PROJECT_CO_I",    itsProjectCoI );
    HDF5Attribute::write (id, "PROJECT_CONTACT", itsProjectContact );
    /*________________________________________________________________
    */
    HDF5Attribute::write (id, "OBSERVATION_ID",               itsObservationID);
    HDF5Attribute::write (id, "OBSERVATION_START_MJD",        itsStartMJD);
    HDF5Attribute::write (id, "OBSERVATION_START_TAI",        itsStartTAI);
    HDF5Attribute::write (id, "OBSERVATION_START_UTC",        itsStartUTC);
    HDF5Attribute::write (id, "OBSERVATION_END_MJD",          itsEndMJD);
    HDF5Attribute::write (id, "OBSERVATION_END_TAI",          itsEndTAI);
    HDF5Attribute::write (id, "OBSERVATION_END_UTC",          itsEndUTC);
    HDF5Attribute::write (id, "OBSERVATION_NOF_STATIONS",     itsNofStations);
    HDF5Attribute::write (id, "OBSERVATION_STATIONS_LIST",    itsStationsList);
    HDF5Attribute::write (id, "OBSERVATION_FREQUENCY_MIN",    itsFrequencyMin);
    HDF5Attribute::write (id, "OBSERVATION_FREQUENCY_MAX",    itsFrequencyMax);
    HDF5Attribute::write (id, "OBSERVATION_FREQUENCY_CENTER", itsFrequencyCenter);
    HDF5Attribute::write (id, "OBSERVATION_FREQUENCY_UNIT",   itsFrequencyUnit);
    HDF5Attribute::write (id, "OBSERVATION_NOF_BITS_PER_SAMPLE", itsNofBitsPerSample);
    /*________________________________________________________________
    */
    HDF5Attribute::write (id, "ANTENNA_SET",          itsAntennaSet );
    HDF5Attribute::write (id, "FILTER_SELECTION",     itsFilterSelection );
    HDF5Attribute::write (id, "CLOCK_FREQUENCY",      itsClockFrequency );
    HDF5Attribute::write (id, "CLOCK_FREQUENCY_UNIT", itsClockFrequencyUnit );
    HDF5Attribute::write (id, "TARGET",               itsTarget );
    HDF5Attribute::write (id, "SYSTEM_VERSION",       itsSystemVersion );
    HDF5Attribute::write (id, "PIPELINE_NAME",        itsPipelineName );
    HDF5Attribute::write (id, "PIPELINE_VERSION",     itsPipelineVersion );
    HDF5Attribute::write (id, "ICD_NUMBER",           itsIcdNumber       );
    HDF5Attribute::write (id, "ICD_VERSION",          itsIcdVersion      );
    HDF5Attribute::write (id, "NOTES",                itsNotes           );

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  /*!
    \param location -- Identifier to the HDF5 group from which to read the
           attributes
  */
  bool CommonAttributes::h5read (hid_t const &location)
  {
    bool status (true);

    HDF5Attribute::read (location, "GROUPTYPE",      itsGroupType );
    HDF5Attribute::read (location, "FILENAME",       itsFilename );
    HDF5Attribute::read (location, "FILETYPE",       itsFiletype );
    HDF5Attribute::read (location, "FILEDATE",       itsFiledate );
    HDF5Attribute::read (location, "TELESCOPE",      itsTelescope );
    HDF5Attribute::read (location, "OBSERVER",       itsObserver );
    /*________________________________________________________________
      Common LOFAR attributes for description of project 
    */
    HDF5Attribute::read (location, "PROJECT_ID",      itsProjectID );
    HDF5Attribute::read (location, "PROJECT_TITLE",   itsProjectTitle );
    HDF5Attribute::read (location, "PROJECT_PI",      itsProjectPI );
    HDF5Attribute::read (location, "PROJECT_CO_I",    itsProjectCoI );
    HDF5Attribute::read (location, "PROJECT_CONTACT", itsProjectContact );
    /*________________________________________________________________
      Common LOFAR attributes for description of observation
    */
    HDF5Attribute::read (location, "OBSERVATION_ID",               itsObservationID);
    HDF5Attribute::read (location, "OBSERVATION_START_MJD",        itsStartMJD);
    HDF5Attribute::read (location, "OBSERVATION_START_TAI",        itsStartTAI);
    HDF5Attribute::read (location, "OBSERVATION_START_UTC",        itsStartUTC);
    HDF5Attribute::read (location, "OBSERVATION_END_MJD",          itsEndMJD);
    HDF5Attribute::read (location, "OBSERVATION_END_TAI",          itsEndTAI);
    HDF5Attribute::read (location, "OBSERVATION_END_UTC",          itsEndUTC);
    HDF5Attribute::read (location, "OBSERVATION_NOF_STATIONS",     itsNofStations);
    HDF5Attribute::read (location, "OBSERVATION_STATIONS_LIST",    itsStationsList);
    HDF5Attribute::read (location, "OBSERVATION_FREQUENCY_MIN",    itsFrequencyMin);
    HDF5Attribute::read (location, "OBSERVATION_FREQUENCY_MAX",    itsFrequencyMax);
    HDF5Attribute::read (location, "OBSERVATION_FREQUENCY_CENTER", itsFrequencyCenter);
    HDF5Attribute::read (location, "OBSERVATION_FREQUENCY_UNIT",   itsFrequencyUnit);
    HDF5Attribute::read (location, "OBSERVATION_NOF_BITS_PER_SAMPLE", itsNofBitsPerSample);
    /*________________________________________________________________
     */
    HDF5Attribute::read (location, "ANTENNA_SET",           itsAntennaSet );
    HDF5Attribute::read (location, "FILTER_SELECTION",      itsFilterSelection );
    HDF5Attribute::read (location, "CLOCK_FREQUENCY",       itsClockFrequency );
    HDF5Attribute::read (location, "CLOCK_FREQUENCY_UNIT",  itsClockFrequencyUnit );
    HDF5Attribute::read (location, "TARGET",                itsTarget );
    HDF5Attribute::read (location, "SYSTEM_VERSION",        itsSystemVersion );
    HDF5Attribute::read (location, "PIPELINE_NAME",         itsPipelineName );
    HDF5Attribute::read (location, "PIPELINE_VERSION",      itsPipelineVersion );
    HDF5Attribute::read (location, "ICD_NUMBER",            itsIcdNumber       );
    HDF5Attribute::read (location, "ICD_VERSION",           itsIcdVersion      );
    HDF5Attribute::read (location, "NOTES",                 itsNotes           );

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  bool CommonAttributes::h5read (hid_t const &location,
				 std::string const &name)
  {
    bool status   = true;
    hid_t groupID = H5Gopen (location, name.c_str(), H5P_DEFAULT);
    
    if (groupID) {
      status = h5read (groupID);
    }
    else {
      std::cerr << "[CommonAttributes::h5read] Error opening group "
		<< name
		<< std::endl;
      status = false;
    }
    
    H5Gclose (groupID);
    
    return status;
  }
  
#endif
  
  //_____________________________________________________________________________
  //                                                                getAttributes

#ifdef DAL_WITH_CASA

  /*!
    \retval rec -- casa::Record to which the attribute values are written. 
    
    \return status -- Status of the operation; return \e false in case an error
            was encountered.
  */
  bool CommonAttributes::getAttributes (casa::Record &rec)
  {
    bool status (true);

    try {
      rec.define("FILENAME",   filename()   );
      rec.define("FILETYPE",   filetype()   );
      rec.define("FILEDATE",   filedate()   );
      rec.define("TELESCOPE",  telescope()  );
      rec.define("OBSERVER",   observer()   );
    } catch (casa::AipsError x) {
      cerr << "[CommonAttributes::getAttributes] "
	   << "Error filling the record with attribute values!\n"
	   << x.getMesg() << endl;
      status = false;
    }
    
    return status;
  }

#endif 
  
  //_____________________________________________________________________________
  //                                                                 setFrequency

  void CommonAttributes::setFrequency (double const &freqMin,
				       double const &freqMax,
				       std::string const &freqUnit)
  {
    itsFrequencyMin    = freqMin;
    itsFrequencyMax    = freqMax;
    itsFrequencyUnit   = freqUnit;
    itsFrequencyCenter = (freqMin+freqMax)/2;
  }
  
  //_____________________________________________________________________________
  //                                                          setObservationStart
  
  void CommonAttributes::setObservationStart (std::string const &startMJD,
					      std::string const &startTAI,
					      std::string const &startUTC)
  {
    setStartMJD (startMJD);
    setStartTAI (startTAI);
    setStartUTC (startUTC);
  }
  
  //_____________________________________________________________________________
  //                                                            setObservationEnd
  
  void CommonAttributes::setObservationEnd (std::string const &endMJD,
					    std::string const &endTAI,
					    std::string const &endUTC)
  {
    setEndMJD (endMJD);
    setEndTAI (endTAI);
    setEndUTC (endUTC);
  }
  
} // Namespace DAL -- end
