/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <CommonAttributes.h>

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
    \param filetype -- The type of the file.
    \param filedate -- The file creation date.
   */
  CommonAttributes::CommonAttributes (Filename const &filename,
				      std::string const &filetype,
				      std::string const &filedate)
  {
    // Initialize parameters to default values
    setAttributes ();
    // Set provided parameter values
    setFilename (filename);
    setFiletype (filetype);
    setFiledate (filedate);
  }

  //_____________________________________________________________________________
  //                                                             CommonAttributes
  
#ifdef HAVE_HDF5    
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
    groupType_p            = other.groupType_p;
    filename_p             = other.filename_p;
    filetype_p             = other.filetype_p;
    filedate_p             = other.filedate_p;
    telescope_p            = other.telescope_p;
    projectID_p            = other.projectID_p;
    projectTitle_p         = other.projectTitle_p;
    projectPI_p            = other.projectPI_p;
    projectCoI_p           = other.projectCoI_p;
    projectContact_p       = other.projectContact_p;
    observer_p             = other.observer_p;
    observationID_p        = other.observationID_p;
    observationStartMJD_p  = other.observationStartMJD_p;
    observationStartTAI_p  = other.observationStartTAI_p;
    observationStartUTC_p  = other.observationStartUTC_p;
    observationEndMJD_p    = other.observationEndMJD_p;
    observationEndTAI_p    = other.observationEndTAI_p;
    observationEndUTC_p    = other.observationEndUTC_p;
    antennaSet_p           = other.antennaSet_p;
    filterSelection_p      = other.filterSelection_p;
    clockFrequency_p       = other.clockFrequency_p;
    clockFrequencyUnit_p   = other.clockFrequencyUnit_p;
    target_p               = other.target_p;
    systemVersion_p        = other.systemVersion_p;
    pipelineName_p         = other.pipelineName_p;
    pipelineVersion_p      = other.pipelineVersion_p;
    nofStations_p          = other.stationsList_p.size();
    stationsList_p.resize(nofStations_p);
    stationsList_p         = other.stationsList_p;
    notes_p                = other.notes_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                               setProjectInfo
  
  /*!
    \param projectID      -- Unique identifier for the project
    \param projectTitle   -- Name of the project
    \param projectPI      -- Name of the project's principal investigator
    \param projectCoI    -- Name(s) of the project's co-PI(s)
    \param projectContact -- Names/Email-addresses of the project's primary
           contact person(s)
  */
  void CommonAttributes::setProjectInfo (std::string const &projectID,
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
  //                                                              setPipelineInfo
  
  /*!
    \param pipelineName    -- Processing pipeline name
    \param pipelineVersion -- Processing pipeline version number
   */
  void CommonAttributes::setPipelineInfo (std::string const &name,
					  std::string const &version)
  {
    setPipelineName (name);
    setPipelineVersion (version);
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param station -- Name of the station to be added to the list of stations

    \param status -- Status of the operation; returns <tt>false</tt> in case
           the station has been found to be in the list already.
  */
  bool CommonAttributes::addStationsList (std::string const &station)
  {
    bool status (true);
    bool newStation (true);

    /* If the list just has been initialized and no actual data have been added
       we can simply overwrite the first element, which was a placeholder anyway.
     */
    if (nofStations() == 1 && stationsList_p[0] == "UNDEFINED") {
      stationsList_p[0] == station;
      return true;
    }

    for (unsigned int n(0); n<stationsList_p.size(); ++n) {
      if (stationsList_p[n] == station) {
	newStation = false;
	n=stationsList_p.size();
      }
    }
    
    if (newStation) {
      stationsList_p.push_back(station);
      ++nofStations_p;
    } else {
      status = false;
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void CommonAttributes::summary (std::ostream &os)
  {
    os << "[CommonAttributes] Summary of internal parameters." << std::endl;
    
    os << "-- GROUPTYPE              = " << groupType()            << std::endl;
    os << "-- FILENAME               = " << filename()             << std::endl;
    os << "-- FILETYPE               = " << filetype()             << std::endl;
    os << "-- FILEDATE               = " << filedate()             << std::endl;
    os << "-- TELESCOPE              = " << telescope()            << std::endl;
    os << "-- PROJECT_ID             = " << projectID()            << std::endl;
    os << "-- PROJECT_TITLE          = " << projectTitle()         << std::endl;
    os << "-- PROJECT_PI             = " << projectPI()            << std::endl;
    os << "-- PROJECT_CO_I           = " << projectCoI()           << std::endl;
    os << "-- PROJECT_CONTACT        = " << projectContact()       << std::endl;
    os << "-- OBSERVER               = " << observer()             << std::endl;
    os << "-- OBSERVATION_ID         = " << observationID()        << std::endl;
    os << "-- OBSERVATION_START_MJD  = " << observationStartMJD()  << std::endl;
    os << "-- OBSERVATION_START_TAI  = " << observationStartTAI()  << std::endl;
    os << "-- OBSERVATION_START_UTC  = " << observationStartUTC()  << std::endl;
    os << "-- OBSERVATION_END_MJD    = " << observationEndMJD()    << std::endl;
    os << "-- OBSERVATION_END_TAI    = " << observationEndTAI()    << std::endl;
    os << "-- OBSERVATION_END_UTC    = " << observationEndUTC()    << std::endl;
    os << "-- ANTENNA_SET            = " << antennaSet()           << std::endl;
    os << "-- FILTER_SELECTION       = " << filterSelection()      << std::endl;
    os << "-- CLOCK_FREQUENCY        = " << clockFrequency()       << std::endl;
    os << "-- CLOCK_FREQUENCY_UNIT   = " << clockFrequencyUnit()   << std::endl;
    os << "-- TARGET                 = " << target()               << std::endl;
    os << "-- SYSTEM_VERSION         = " << systemVersion()        << std::endl;
    os << "-- PIPELINE_NAME          = " << pipelineName()         << std::endl;
    os << "-- PIPELINE_VERSION       = " << pipelineVersion()      << std::endl;
    os << "-- NOF_STATIONS           = " << nofStations()          << std::endl;
    os << "-- STATIONS               = " << stationsList()         << std::endl;
    os << "-- NOTES                  = " << notes()                << std::endl;
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
    attributes_p.clear();
    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("FILENAME");
    attributes_p.insert("FILETYPE");
    attributes_p.insert("FILEDATE");
    attributes_p.insert("TELESCOPE");
    attributes_p.insert("PROJECT_ID");
    attributes_p.insert("PROJECT_TITLE");
    attributes_p.insert("PROJECT_PI");
    attributes_p.insert("PROJECT_CO_I");
    attributes_p.insert("PROJECT_CONTACT");
    attributes_p.insert("OBSERVER");
    attributes_p.insert("OBSERVATION_ID");
    attributes_p.insert("OBSERVATION_START_MJD");
    attributes_p.insert("OBSERVATION_START_TAI");
    attributes_p.insert("OBSERVATION_START_UTC");
    attributes_p.insert("OBSERVATION_END_MJD");
    attributes_p.insert("OBSERVATION_END_TAI");
    attributes_p.insert("OBSERVATION_END_UTC");
    attributes_p.insert("OBSERVATION_FREQUENCY_MAX");
    attributes_p.insert("OBSERVATION_FREQUENCY_MIN");
    attributes_p.insert("OBSERVATION_FREQUENCY_CENTER");
    attributes_p.insert("OBSERVATION_FREQUENCY_UNIT");
    attributes_p.insert("OBSERVATION_FREQUENCY_MAX");
    attributes_p.insert("OBSERVATION_FREQUENCY_MIN");
    attributes_p.insert("OBSERVATION_FREQUENCY_CENTER");
    attributes_p.insert("ANTENNA_SET");
    attributes_p.insert("FILTER_SELECTION");
    attributes_p.insert("CLOCK_FREQUENCY");
    attributes_p.insert("CLOCK_FREQUENCY_UNIT");
    attributes_p.insert("TARGET");
    attributes_p.insert("SYSTEM_VERSION");
    attributes_p.insert("PIPELINE_NAME");
    attributes_p.insert("PIPELINE_VERSION");
    attributes_p.insert("OBSERVATION_NOF_STATIONS");
    attributes_p.insert("OBSERVATION_STATIONS_LIST");
    attributes_p.insert("NOTES");
    /* Default values of the attributes */
    std::string undefined ("UNDEFINED");
    std::vector<std::string> stations (1,undefined);
    //
    groupType_p           = "Root";
    filename_p            = undefined;
    filetype_p            = undefined;
    filedate_p            = undefined;
    telescope_p           = "LOFAR";
    projectID_p           = undefined;
    projectTitle_p        = undefined;
    projectPI_p           = undefined;
    projectCoI_p          = undefined;
    projectContact_p      = undefined;
    observer_p            = undefined;
    observationID_p       = undefined;
    observationStartMJD_p = undefined;
    observationStartTAI_p = undefined;
    observationStartUTC_p = undefined;
    observationEndMJD_p   = undefined;
    observationEndTAI_p   = undefined;
    observationEndUTC_p   = undefined;
    antennaSet_p          = undefined;
    filterSelection_p     = undefined;
    clockFrequency_p      = 0.0;
    clockFrequencyUnit_p  = undefined;
    target_p              = undefined;
    systemVersion_p       = undefined;
    pipelineName_p        = undefined;
    pipelineVersion_p     = undefined;
    nofStations_p         = stations.size();
    stationsList_p        = stations;
    notes_p               = undefined;
  }
  
#ifdef HAVE_HDF5
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  void CommonAttributes::h5write (hid_t const &groupID)
  {
    DAL::h5set_attribute( groupID, "GROUPTYPE",              groupType_p );
    DAL::h5set_attribute( groupID, "FILENAME",               filename_p );
    DAL::h5set_attribute( groupID, "FILETYPE",               filetype_p );
    DAL::h5set_attribute( groupID, "FILEDATE",               filedate_p );
    DAL::h5set_attribute( groupID, "TELESCOPE",              telescope_p );
    DAL::h5set_attribute( groupID, "PROJECT_ID",             projectID_p );
    DAL::h5set_attribute( groupID, "PROJECT_TITLE",          projectTitle_p );
    DAL::h5set_attribute( groupID, "PROJECT_PI",             projectPI_p );
    DAL::h5set_attribute( groupID, "PROJECT_CO_I",           projectCoI_p );
    DAL::h5set_attribute( groupID, "PROJECT_CONTACT",        projectContact_p );
    DAL::h5set_attribute( groupID, "OBSERVER",               observer_p );
    DAL::h5set_attribute( groupID, "OBSERVATION_ID",         observationID_p );
    DAL::h5set_attribute( groupID, "OBSERVATION_DATE_START", observationStartMJD_p );
    DAL::h5set_attribute( groupID, "OBSERVATION_DATE_END",   observationEndMJD_p );
    DAL::h5set_attribute( groupID, "ANTENNA_SET",            antennaSet_p );
    DAL::h5set_attribute( groupID, "FILTER_SELECTION",       filterSelection_p );
    DAL::h5set_attribute( groupID, "CLOCK_FREQUENCY",        clockFrequency_p );
    DAL::h5set_attribute( groupID, "CLOCK_FREQUENCY_UNIT",   clockFrequencyUnit_p );
    DAL::h5set_attribute( groupID, "TARGET",                 target_p );
    DAL::h5set_attribute( groupID, "SYSTEM_VERSION",         systemVersion_p );
    DAL::h5set_attribute( groupID, "PIPELINE_NAME",          pipelineName_p );
    DAL::h5set_attribute( groupID, "PIPELINE_VERSION",       pipelineVersion_p );
    DAL::h5set_attribute( groupID, "NOF_STATIONS",           nofStations_p );
    DAL::h5set_attribute( groupID, "STATIONS_LIST",          stationsList_p );
    DAL::h5set_attribute( groupID, "NOTES",                  notes_p );
  }
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  /*!
    \param groupID -- Identifier to the HDF5 group from which to read the
           attributes
  */
  void CommonAttributes::h5read (hid_t const &groupID)
  {
    DAL::h5get_attribute( groupID, "GROUPTYPE",              groupType_p );
    DAL::h5get_attribute( groupID, "FILENAME",               filename_p );
    DAL::h5get_attribute( groupID, "FILETYPE",               filetype_p );
    DAL::h5get_attribute( groupID, "FILEDATE",               filedate_p );
    DAL::h5get_attribute( groupID, "TELESCOPE",              telescope_p );
    DAL::h5get_attribute( groupID, "PROJECT_ID",             projectID_p );
    DAL::h5get_attribute( groupID, "PROJECT_TITLE",          projectTitle_p );
    DAL::h5get_attribute( groupID, "PROJECT_PI",             projectPI_p );
    DAL::h5get_attribute( groupID, "PROJECT_CO_I",           projectCoI_p );
    DAL::h5get_attribute( groupID, "PROJECT_CONTACT",        projectContact_p );
    DAL::h5get_attribute( groupID, "OBSERVER",               observer_p );
    DAL::h5get_attribute( groupID, "OBSERVATION_ID",         observationID_p );
    DAL::h5get_attribute( groupID, "OBSERVATION_DATE_START", observationStartMJD_p );
    DAL::h5get_attribute( groupID, "OBSERVATION_DATE_END",   observationEndMJD_p );
    DAL::h5get_attribute( groupID, "ANTENNA_SET",            antennaSet_p );
    DAL::h5get_attribute( groupID, "FILTER_SELECTION",       filterSelection_p );
    DAL::h5get_attribute( groupID, "CLOCK_FREQUENCY",        clockFrequency_p );
    DAL::h5get_attribute( groupID, "CLOCK_FREQUENCY_UNIT",   clockFrequencyUnit_p );
    DAL::h5get_attribute( groupID, "TARGET",                 target_p );
    DAL::h5get_attribute( groupID, "SYSTEM_VERSION",         systemVersion_p );
    DAL::h5get_attribute( groupID, "PIPELINE_NAME",          pipelineName_p );
    DAL::h5get_attribute( groupID, "PIPELINE_VERSION",       pipelineVersion_p );
    DAL::h5get_attribute( groupID, "NOF_STATIONS",           nofStations_p );
    DAL::h5get_attribute( groupID, "STATIONS_LIST",          stationsList_p );
    DAL::h5get_attribute( groupID, "NOTES",                  notes_p );
  }
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  void CommonAttributes::h5read (hid_t const &locationID,
				 std::string const &name)
  {
    hid_t groupID (0);
    
    groupID = H5Gopen1 (locationID,
                        name.c_str());
    
    if (groupID) {
      h5read (groupID);
    }
    else {
      std::cerr << "[CommonAttributes::h5read] Error opening group "
		<< name
		<< std::endl;
    }
    
    H5Gclose (groupID);
  }

#endif

  //_____________________________________________________________________________
  //                                                                     toRecord
  
  /*!
    \return rec -- casa::Record to which the list of common attributes have been 
            written.
  */
  casa::Record CommonAttributes::toRecord ()
  {
    casa::Record rec;

    toRecord (rec, true);

    return rec;
  }

  //_____________________________________________________________________________
  //                                                                     toRecord

  /*!
    \retval rec      -- casa::Record to which the attribute values are written. 
    \param overwrite -- Overwrite entries in the record?
    
    \return status -- Status of the operation; return \e false in case an error
            was encountered.
  */
  bool CommonAttributes::toRecord (casa::Record &rec,
				   bool const &overwrite)
  {
    bool status (true);

    rec.define("FILENAME",               filename()       );
    rec.define("FILETYPE",               filetype()       );
    rec.define("FILEDATE",               filedate()       );
    rec.define("TELESCOPE",              telescope()      );
    rec.define("PROJECT_ID",             projectID()      );
    rec.define("PROJECT_TITLE",          projectTitle()   );
    rec.define("PROJECT_PI",             projectPI()      );
    rec.define("PROJECT_CO_I",           projectCoI()     );
    rec.define("PROJECT_CONTACT",        projectContact() );
    rec.define("OBSERVER",               observer()       );
    rec.define("OBSERVATION_ID",         observationID()  );
    rec.define("OBSERVATION_DATE_START", observationStartMJD() );
    rec.define("OBSERVATION_DATE_END",   observationEndMJD()   );

    return status;
  }
  
} // Namespace DAL -- end
