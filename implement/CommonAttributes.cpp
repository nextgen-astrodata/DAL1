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
  CommonAttributes::CommonAttributes (std::string const &filename,
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
    groupType_p          = other.groupType_p;
    filename_p           = other.filename_p;
    filetype_p           = other.filetype_p;
    filedate_p           = other.filedate_p;
    telescope_p          = other.telescope_p;
    projectID_p          = other.projectID_p;
    projectTitle_p       = other.projectTitle_p;
    projectDescription_p = other.projectDescription_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                               setProjectInfo
  
  /*!
    \param projectID          -- Unique identifier for the project
    \param projectTitle       -- Name of the project
    \param projectDescription -- Brief project description
    \param projectPI          -- Name of the project's principal investigator
  */
  void CommonAttributes::setProjectInfo (std::string const &projectID,
					 std::string const &projectTitle,
					 std::string const &projectDescription,
					 std::string const &projectPI)
  {
    setProjectID (projectID);
    setProjectTitle (projectTitle);
    setProjectDescription (projectDescription);
    setProjectPI (projectPI);
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
					     std::string const &obsDateEnd)
  {
    setObservationID (obsID);
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
    os << "-- Project description           = " << projectDescription_p
       << std::endl;
    os << "-- Observer name(s)              = " << observer_p
       << std::endl;
    os << "-- Observation ID                = " << observationID_p
       << std::endl;
    os << "-- Start date of the observation = " << observationDateStart_p
       << std::endl;
    os << "-- End date of the observtaion   = " << observationDateEnd_p
       << std::endl;
    os << "-- Filter selection              = " << filterSelection_p
       << std::endl;
    os << "-- Oberservation target(s)       = " << target_p
       << std::endl;
    os << "-- Processing pipeline name      = " << pipelineName_p
       << std::endl;
    os << "-- Processing pipeline version   = " << pipelineVersion_p
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
    projectDescription_p   = "UNDEFINED";
    projectPI_p            = "UNDEFINED";
    projectCO_I_p          = "UNDEFINED";
    projectContact_p       = "UNDEFINED";
    observer_p             = "UNDEFINED";
    observationID_p        = "UNDEFINED";
    observationDateStart_p = "UNDEFINED";
    observationDateEnd_p   = "UNDEFINED";
    filterSelection_p      = "UNDEFINED";
    target_p               = "UNDEFINED";
    systemTime_p           = "UNDEFINED";
    systemVersion_p        = "UNDEFINED";
    pipelineName_p         = "UNDEFINED";
    pipelineVersion_p      = "UNDEFINED";
    nofStations_p          = 0;
    notes_p                = "UNDEFINED";
  }
  
#ifdef HAVE_HDF5
  
  //_____________________________________________________________________________
  //                                                                       h5read

  /*!
    \param groupID -- Identifier to the HDF5 group from which to read the
           attributes
  */
  void CommonAttributes::h5read (hid_t const &groupID)
  {
    DAL::h5get_attribute( groupID, "GROUPTYPE",              groupType_p );
    DAL::h5get_attribute( groupID, "FILENAME",               filename_p  );
    DAL::h5get_attribute( groupID, "FILETYPE",               filetype_p  );
    DAL::h5get_attribute( groupID, "FILEDATE",               filedate_p  );
    DAL::h5get_attribute( groupID, "TELESCOPE",              telescope_p );
    DAL::h5get_attribute( groupID, "PROJECT_ID",             projectID_p );
    DAL::h5get_attribute( groupID, "PROJECT_TITLE",          projectTitle_p );
    DAL::h5get_attribute( groupID, "PROJECT_DESCRIPTION",    projectDescription_p );
    DAL::h5get_attribute( groupID, "OBSERVATION_DATE_START", observationDateStart_p );
    DAL::h5get_attribute( groupID, "OBSERVATION_DATE_END",   observationDateEnd_p );
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
