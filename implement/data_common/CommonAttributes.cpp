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
    - SYSTEM_TIME = "UTC"
  */
  CommonAttributes::CommonAttributes ()
  {
    init ();
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
    init ();
    // Set provided parameter values
    setFilename (filename);
    setFiletype (filetype);
    setFiledate (filedate);
  }

  //_____________________________________________________________________________
  //                                                             CommonAttributes
  
  /*!
    \param observationID        -- Unique identifier for the observation
    \param observationTimeSys   -- Reference system for time
    \param observationDateStart -- Start date of the observation
    \param observationDateEnd   -- End date of the observation
  */
  CommonAttributes::CommonAttributes (std::string const &observationID,
				      std::string const &observationTimeSys,
				      std::string const &observationDateStart,
				      std::string const &observationDateEnd)
  {
    // Initialize parameters to default values
    init ();
    // Set provided parameter values
    setObservationID (observationID);
    setObservationTimeSys (observationTimeSys);
    setObservationDateStart (observationDateStart);
    setObservationDateEnd (observationDateEnd);
  }
  
  //_____________________________________________________________________________
  //                                                             CommonAttributes
  
#ifdef HAVE_HDF5    
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
    groupType_p            = other.groupType_p;
    filename_p             = other.filename_p;
    filetype_p             = other.filetype_p;
    filedate_p             = other.filedate_p;
    telescope_p            = other.telescope_p;
    projectID_p            = other.projectID_p;
    projectTitle_p         = other.projectTitle_p;
    projectPI_p            = other.projectPI_p;
    projectCoI_p          = other.projectCoI_p;
    projectContact_p       = other.projectContact_p;
    observer_p             = other.observer_p;
    observationID_p        = other.observationID_p;
    observationTimeSys_p   = other.observationTimeSys_p;
    observationDateStart_p = other.observationDateStart_p;
    observationDateEnd_p   = other.observationDateEnd_p;
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
  //                                                           setObservationInfo
  
  /*!
    \param obsID        -- Unique identifier for the observation.
    \param obsDateStart -- Start date of the observation.
    \param obsDateEnd   -- End date of the observation.
   */
  void CommonAttributes::setObservationInfo (std::string const &obsID,
					     std::string const &obsDateStart,
					     std::string const &obsDateEnd,
					     std::string const &obsTimeSys)
  {
    setObservationID (obsID);
    setObservationDateStart (obsDateStart);
    setObservationDateEnd (obsDateEnd);
    setObservationTimeSys (obsTimeSys);
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
  */
  void CommonAttributes::addStationsList (std::string const &station)
  {
    bool newStation (true);

    /* If the list just has been initialized and no actual data have been added
       we can simply overwrite the first element, which was a placeholder anyway.
     */
    if (nofStations() == 1 && stationsList_p[0] == "UNDEFINED") {
      stationsList_p[0] == station;
    }

    for (unsigned int n(0); n<stationsList_p.size(); ++n) {
      if (stationsList_p[n] == station) {
	newStation = false;
	n=stationsList_p.size();
      }
    }
    
    if (newStation) {
      stationsList_p.push_back(station);
    }
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  void CommonAttributes::summary (std::ostream &os)
  {
    os << "[CommonAttributes] Summary of internal parameters." << std::endl;

    os << "-- Group type                    = " << groupType_p
       << std::endl;
    os << "-- File name                     = " << filename_p
       << std::endl;
    os << "-- File type                     = " << filetype_p
       << std::endl;
    os << "-- File creation date            = " << filedate_p
       << std::endl;
    os << "-- Telescope name                = " << telescope_p
       << std::endl;
    os << "-- Project identifier            = " << projectID_p
       << std::endl;
    os << "-- Project title                 = " << projectTitle_p
       << std::endl;
    os << "-- Observer name(s)              = " << observer_p
       << std::endl;
    os << "-- Observation ID                = " << observationID_p
       << std::endl;
    os << "-- Reference system for time     = " << observationTimeSys_p
       << std::endl;
    os << "-- Observation date, start       = " << observationDateStart_p
       << std::endl;
    os << "-- Observation date, end         = " << observationDateEnd_p
       << std::endl;
    os << "-- Antenna set specification     = " << antennaSet_p
       << std::endl;
    os << "-- Filter selection              = " << filterSelection_p
       << std::endl;
    os << "-- Clock frequency               = " << clockFrequency_p
       << std::endl;
    os << "-- Clock frequency unit          = " << clockFrequencyUnit_p
       << std::endl;
    os << "-- Oberservation target(s)       = " << target_p
       << std::endl;
    os << "-- System version number         = " << systemVersion_p
       << std::endl;
    os << "-- Processing pipeline name      = " << pipelineName_p
       << std::endl;
    os << "-- Processing pipeline versionn  = " << pipelineVersion_p
       << std::endl;
    os << "-- Number of stations used       = " << nofStations_p
       << std::endl;
    os << "-- List of stations used         = " << stationsList_p
       << std::endl;
    os << "-- Notes and/or comments         = " << notes_p
       << std::endl;
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
    groupType_p            = "Root";
    filename_p             = "UNDEFINED";
    filetype_p             = "UNDEFINED";
    filedate_p             = "UNDEFINED";
    telescope_p            = "LOFAR";
    projectID_p            = "UNDEFINED";
    projectTitle_p         = "UNDEFINED";
    projectPI_p            = "UNDEFINED";
    projectCoI_p          = "UNDEFINED";
    projectContact_p       = "UNDEFINED";
    observer_p             = "UNDEFINED";
    observationID_p        = "UNDEFINED";
    observationTimeSys_p   = "UNDEFINED";
    observationDateStart_p = "UNDEFINED";
    observationDateEnd_p   = "UNDEFINED";
    antennaSet_p           = "UNDEFINED";
    filterSelection_p      = "UNDEFINED";
    clockFrequency_p       = 0;
    clockFrequencyUnit_p   = "UNDEFINED";
    target_p               = "UNDEFINED";
    systemVersion_p        = "UNDEFINED";
    pipelineName_p         = "UNDEFINED";
    pipelineVersion_p      = "UNDEFINED";
    notes_p                = "UNDEFINED";
    
    std::vector<std::string> names (1,"UNDEFINED");
    setStationsList (names);
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
    DAL::h5set_attribute( groupID, "OBSERVATION_TIMESYS",    observationTimeSys_p );
    DAL::h5set_attribute( groupID, "OBSERVATION_DATE_START", observationDateStart_p );
    DAL::h5set_attribute( groupID, "OBSERVATION_DATE_END",   observationDateEnd_p );
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
    DAL::h5get_attribute( groupID, "OBSERVATION_TIMESYS",    observationTimeSys_p );
    DAL::h5get_attribute( groupID, "OBSERVATION_DATE_START", observationDateStart_p );
    DAL::h5get_attribute( groupID, "OBSERVATION_DATE_END",   observationDateEnd_p );
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
    
} // Namespace DAL -- end
