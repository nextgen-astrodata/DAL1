/***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#include "CommonAttributes.h"

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
    setAttributes ();
  }
  
  //_____________________________________________________________________________
  //                                                             CommonAttributes

  /*!
    \param filename -- The name of the file.
  */
  CommonAttributes::CommonAttributes (DAL::Filename const &filename)
  {
    // Initialize parameters to default values
    setAttributes ();
    // Set provided parameter values
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
    // Initialize parameters to default values
    setAttributes ();
    // Set provided parameter values
    setFilename (filename);
    setFiledate (filedate);
  }

  //_____________________________________________________________________________
  //                                                             CommonAttributes
  
#ifdef DAL_WITH_HDF5    
  CommonAttributes::CommonAttributes (hid_t const &locationID)
  {
    // Initialize parameters to default values
    setAttributes ();
    // Read the attribute values from the file
    h5read (locationID);
  }
#endif
  
  //_____________________________________________________________________________
  //                                                             CommonAttributes

  CommonAttributes::CommonAttributes (CommonAttributes const &other)
    : AttributesInterface (other)
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
    /*________________________________________________________________
    */
    itsObservationID   = other.itsObservationID;
    itsStartMJD        = other.itsStartMJD;
    itsStartTAI        = other.itsStartTAI;
    itsStartUTC        = other.itsStartUTC;
    itsEndMJD          = other.itsEndMJD;
    itsEndTAI          = other.itsEndTAI;
    itsEndUTC          = other.itsEndUTC;
    itsNofStations     = other.itsNofStations;
    itsStationsList    = other.itsStationsList;
    itsFrequencyMin    = other.itsFrequencyMin;
    itsFrequencyMax    = other.itsFrequencyMax;
    itsFrequencyCenter = other.itsFrequencyCenter;
    itsFrequencyUnit   = other.itsFrequencyUnit;
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
  
  void CommonAttributes::summary (std::ostream &os)
  {
    os << "[CommonAttributes] Summary of internal parameters." << std::endl;
    
    os << "-- GROUPTYPE              = " << itsGroupType       << std::endl;
    os << "-- FILENAME               = " << itsFilename        << std::endl;
    os << "-- FILETYPE               = " << itsFiletype        << std::endl;
    os << "-- FILEDATE               = " << itsFiledate        << std::endl;
    os << "-- TELESCOPE              = " << itsTelescope       << std::endl;
    os << "-- OBSERVER               = " << itsObserver        << std::endl;
    /*________________________________________________________________
      Common LOFAR attributes for description of project 
    */
    os << "-- PROJECT_ID             = " << itsProjectID       << std::endl;
    os << "-- PROJECT_TITLE          = " << itsProjectTitle    << std::endl;
    os << "-- PROJECT_PI             = " << itsProjectPI       << std::endl;
    os << "-- PROJECT_CO_I           = " << itsProjectCoI      << std::endl;
    os << "-- PROJECT_CONTACT        = " << itsProjectContact  << std::endl;
    /*________________________________________________________________
      Common LOFAR attributes for description of observation
    */
    os << "-- Observation ID                = " << itsObservationID   << std::endl;
    os << "-- Observation date (MJD) Start  = " << itsStartMJD        << std::endl;
    os << "-- Observation date (MJD) End    = " << itsEndMJD          << std::endl;
    os << "-- Observation date (TAI) Start  = " << itsStartTAI        << std::endl;
    os << "-- Observation date (TAI) End    = " << itsEndTAI          << std::endl;
    os << "-- Observation date (UTC) Start  = " << itsStartUTC        << std::endl;
    os << "-- Observation date (UTC) End    = " << itsEndUTC          << std::endl;
    os << "-- nof. stations participating   = " << itsNofStations     << std::endl;
    os << "-- List of stations              = " << itsStationsList    << std::endl;
    os << "-- Minimum observation frequency = " << itsFrequencyMin    << std::endl;
    os << "-- Center observation frequency  = " << itsFrequencyCenter << std::endl;
    os << "-- Maximum observation frequency = " << itsFrequencyMax    << std::endl;
    os << "-- Observation frequency unit    = " << itsFrequencyUnit   << std::endl;
    /*________________________________________________________________
     */
    os << "-- ANTENNA_SET            = " << itsAntennaSet           << std::endl;
    os << "-- FILTER_SELECTION       = " << itsFilterSelection      << std::endl;
    os << "-- CLOCK_FREQUENCY        = " << itsClockFrequency       << std::endl;
    os << "-- CLOCK_FREQUENCY_UNIT   = " << itsClockFrequencyUnit   << std::endl;
    os << "-- TARGET                 = " << itsTarget               << std::endl;
    os << "-- SYSTEM_VERSION         = " << itsSystemVersion        << std::endl;
    os << "-- PIPELINE_NAME          = " << pipelineName()         << std::endl;
    os << "-- PIPELINE_VERSION       = " << pipelineVersion()      << std::endl;
    os << "-- NOTES                  = " << itsNotes                << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void CommonAttributes::setAttributes ()
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
    itsAttributes.insert("NOTES");
    /* Default values of the attributes */
    std::string undefined ("UNDEFINED");
    std::vector<std::string> stations (1,undefined);
    //
    itsGroupType           = "Root";
    itsFilename            = undefined;
    itsFiletype            = undefined;
    itsFiledate            = undefined;
    itsTelescope           = "LOFAR";
    itsObserver            = undefined;
    itsProjectID           = undefined;
    itsProjectTitle        = undefined;
    itsProjectPI           = undefined;
    itsProjectCoI          = undefined;
    itsProjectContact      = undefined;
    itsObservationID       = undefined;
    itsStartMJD            = undefined;
    itsStartTAI            = undefined;
    itsStartUTC            = undefined;
    itsEndMJD              = undefined;
    itsEndTAI              = undefined;
    itsEndUTC              = undefined;
    itsNofStations         = 0;
    itsStationsList.clear();
    itsFrequencyMin        = 0.0;
    itsFrequencyMax        = 0.0;
    itsFrequencyCenter     = 0.0;
    itsFrequencyUnit       = "UNDEFINED";
    itsAntennaSet          = undefined;
    itsFilterSelection     = undefined;
    itsClockFrequency      = 0.0;
    itsClockFrequencyUnit  = undefined;
    itsTarget              = undefined;
    itsSystemVersion       = undefined;
    itsPipelineName        = undefined;
    itsPipelineVersion     = undefined;
    itsNotes               = undefined;
  }
  
#ifdef DAL_WITH_HDF5
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  bool CommonAttributes::h5write (hid_t const &groupID)
  {
    bool status (true);
    
    HDF5Attribute::write( groupID, "GROUPTYPE",            itsGroupType );
    HDF5Attribute::write( groupID, "FILENAME",             itsFilename );
    HDF5Attribute::write( groupID, "FILETYPE",             itsFiletype );
    HDF5Attribute::write( groupID, "FILEDATE",             itsFiledate );
    HDF5Attribute::write( groupID, "TELESCOPE",            itsTelescope );
    HDF5Attribute::write( groupID, "OBSERVER",             itsObserver );
    /*________________________________________________________________
      Common LOFAR attributes for description of project 
    */
    HDF5Attribute::write( groupID, "PROJECT_ID",           itsProjectID );
    HDF5Attribute::write( groupID, "PROJECT_TITLE",        itsProjectTitle );
    HDF5Attribute::write( groupID, "PROJECT_PI",           itsProjectPI );
    HDF5Attribute::write( groupID, "PROJECT_CO_I",         itsProjectCoI );
    HDF5Attribute::write( groupID, "PROJECT_CONTACT",      itsProjectContact );
    /*________________________________________________________________
    */
    HDF5Attribute::write(groupID, "OBSERVATION_ID",               itsObservationID);
    HDF5Attribute::write(groupID, "OBSERVATION_START_MJD",        itsStartMJD);
    HDF5Attribute::write(groupID, "OBSERVATION_START_TAI",        itsStartTAI);
    HDF5Attribute::write(groupID, "OBSERVATION_START_UTC",        itsStartUTC);
    HDF5Attribute::write(groupID, "OBSERVATION_END_MJD",          itsEndMJD);
    HDF5Attribute::write(groupID, "OBSERVATION_END_TAI",          itsEndMJD);
    HDF5Attribute::write(groupID, "OBSERVATION_END_UTC",          itsEndMJD);
    HDF5Attribute::write(groupID, "OBSERVATION_NOF_STATIONS",     itsNofStations);
    HDF5Attribute::write(groupID, "OBSERVATION_STATIONS_LIST",    itsStationsList);
    HDF5Attribute::write(groupID, "OBSERVATION_FREQUENCY_MIN",    itsFrequencyMin);
    HDF5Attribute::write(groupID, "OBSERVATION_FREQUENCY_MAX",    itsFrequencyMax);
    HDF5Attribute::write(groupID, "OBSERVATION_FREQUENCY_CENTER", itsFrequencyCenter);
    HDF5Attribute::write(groupID, "OBSERVATION_FREQUENCY_UNIT",   itsFrequencyUnit);
    /*________________________________________________________________
    */
    HDF5Attribute::write( groupID, "ANTENNA_SET",          itsAntennaSet );
    HDF5Attribute::write( groupID, "FILTER_SELECTION",     itsFilterSelection );
    HDF5Attribute::write( groupID, "CLOCK_FREQUENCY",      itsClockFrequency );
    HDF5Attribute::write( groupID, "CLOCK_FREQUENCY_UNIT", itsClockFrequencyUnit );
    HDF5Attribute::write( groupID, "TARGET",               itsTarget );
    HDF5Attribute::write( groupID, "SYSTEM_VERSION",       itsSystemVersion );
    HDF5Attribute::write( groupID, "PIPELINE_NAME",        itsPipelineName );
    HDF5Attribute::write( groupID, "PIPELINE_VERSION",     itsPipelineVersion );
    HDF5Attribute::write( groupID, "NOTES",                itsNotes );

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  /*!
    \param groupID -- Identifier to the HDF5 group from which to read the
           attributes
  */
  bool CommonAttributes::h5read (hid_t const &groupID)
  {
    bool status (true);

    DAL::h5get_attribute( groupID, "GROUPTYPE",             itsGroupType );
    DAL::h5get_attribute( groupID, "FILENAME",              itsFilename );
    DAL::h5get_attribute( groupID, "FILETYPE",              itsFiletype );
    DAL::h5get_attribute( groupID, "FILEDATE",              itsFiledate );
    DAL::h5get_attribute( groupID, "TELESCOPE",             itsTelescope );
    DAL::h5get_attribute( groupID, "OBSERVER",              itsObserver );
    /*________________________________________________________________
      Common LOFAR attributes for description of project 
    */
    DAL::h5get_attribute( groupID, "PROJECT_ID",      itsProjectID );
    DAL::h5get_attribute( groupID, "PROJECT_TITLE",   itsProjectTitle );
    DAL::h5get_attribute( groupID, "PROJECT_PI",      itsProjectPI );
    DAL::h5get_attribute( groupID, "PROJECT_CO_I",    itsProjectCoI );
    DAL::h5get_attribute( groupID, "PROJECT_CONTACT", itsProjectContact );
    /*________________________________________________________________
      Common LOFAR attributes for description of observation
    */
    DAL::h5get_attribute(groupID, "OBSERVATION_ID",               itsObservationID);
    DAL::h5get_attribute(groupID, "OBSERVATION_START_MJD",        itsStartMJD);
    DAL::h5get_attribute(groupID, "OBSERVATION_START_TAI",        itsStartTAI);
    DAL::h5get_attribute(groupID, "OBSERVATION_START_UTC",        itsStartUTC);
    DAL::h5get_attribute(groupID, "OBSERVATION_END_MJD",          itsEndMJD);
    DAL::h5get_attribute(groupID, "OBSERVATION_END_TAI",          itsEndMJD);
    DAL::h5get_attribute(groupID, "OBSERVATION_END_UTC",          itsEndMJD);
    DAL::h5get_attribute(groupID, "OBSERVATION_NOF_STATIONS",     itsNofStations);
    DAL::h5get_attribute(groupID, "OBSERVATION_STATIONS_LIST",    itsStationsList);
    DAL::h5get_attribute(groupID, "OBSERVATION_FREQUENCY_MIN",    itsFrequencyMin);
    DAL::h5get_attribute(groupID, "OBSERVATION_FREQUENCY_MAX",    itsFrequencyMax);
    DAL::h5get_attribute(groupID, "OBSERVATION_FREQUENCY_CENTER", itsFrequencyCenter);
    DAL::h5get_attribute(groupID, "OBSERVATION_FREQUENCY_UNIT",   itsFrequencyUnit);
    /*________________________________________________________________
     */
    DAL::h5get_attribute( groupID, "ANTENNA_SET",           itsAntennaSet );
    DAL::h5get_attribute( groupID, "FILTER_SELECTION",      itsFilterSelection );
    DAL::h5get_attribute( groupID, "CLOCK_FREQUENCY",       itsClockFrequency );
    DAL::h5get_attribute( groupID, "CLOCK_FREQUENCY_UNIT",  itsClockFrequencyUnit );
    DAL::h5get_attribute( groupID, "TARGET",                itsTarget );
    DAL::h5get_attribute( groupID, "SYSTEM_VERSION",        itsSystemVersion );
    DAL::h5get_attribute( groupID, "PIPELINE_NAME",         itsPipelineName );
    DAL::h5get_attribute( groupID, "PIPELINE_VERSION",      itsPipelineVersion );
    DAL::h5get_attribute( groupID, "NOTES",                 itsNotes );

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  bool CommonAttributes::h5read (hid_t const &locationID,
				 std::string const &name)
  {
    bool status   = true;
    hid_t groupID = H5Gopen (locationID, name.c_str(), H5P_DEFAULT);
    
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
      rec.define("FILENAME",      filename()   );
      rec.define("FILETYPE",      filetype()   );
      rec.define("FILEDATE",      filedate()   );
      rec.define("TELESCOPE",     telescope()  );
      rec.define("OBSERVER",      observer()   );
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
