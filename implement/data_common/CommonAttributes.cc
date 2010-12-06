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
    itsFilename             = other.itsFilename;
    filetype_p             = other.filetype_p;
    filedate_p             = other.filedate_p;
    telescope_p            = other.telescope_p;

    attributesProject_p    = other.attributesProject_p;

    observer_p             = other.observer_p;
    antennaSet_p           = other.antennaSet_p;
    filterSelection_p      = other.filterSelection_p;
    clockFrequency_p       = other.clockFrequency_p;
    clockFrequencyUnit_p   = other.clockFrequencyUnit_p;
    target_p               = other.target_p;
    systemVersion_p        = other.systemVersion_p;
    pipelineName_p         = other.pipelineName_p;
    pipelineVersion_p      = other.pipelineVersion_p;
    notes_p                = other.notes_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                         setAttributesProject
  
  void CommonAttributes::setAttributesProject (CommonAttributesProject const &attributesProject)
  {
    attributesProject_p = attributesProject;
  }

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
    CommonAttributesProject attributes (projectID,
					projectTitle,
					projectPI,
					projectCoI,
					projectContact);
    
    setAttributesProject (attributes);
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void CommonAttributes::summary (std::ostream &os)
  {
    os << "[CommonAttributes] Summary of internal parameters." << std::endl;
    
    os << "-- GROUPTYPE              = " << groupType_p            << std::endl;
    os << "-- FILENAME               = " << itsFilename             << std::endl;
    os << "-- FILETYPE               = " << filetype_p             << std::endl;
    os << "-- FILEDATE               = " << filedate_p             << std::endl;
    os << "-- TELESCOPE              = " << telescope_p            << std::endl;
    os << "-- OBSERVER               = " << observer_p             << std::endl;
    os << "-- ANTENNA_SET            = " << antennaSet_p           << std::endl;
    os << "-- FILTER_SELECTION       = " << filterSelection_p      << std::endl;
    os << "-- CLOCK_FREQUENCY        = " << clockFrequency_p       << std::endl;
    os << "-- CLOCK_FREQUENCY_UNIT   = " << clockFrequencyUnit_p   << std::endl;
    os << "-- TARGET                 = " << target_p               << std::endl;
    os << "-- SYSTEM_VERSION         = " << systemVersion_p        << std::endl;
    os << "-- PIPELINE_NAME          = " << pipelineName()         << std::endl;
    os << "-- PIPELINE_VERSION       = " << pipelineVersion()      << std::endl;
    os << "-- NOTES                  = " << notes_p                << std::endl;
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
    attributes_p.insert("OBSERVER");
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
    itsFilename            = undefined;
    filetype_p            = undefined;
    filedate_p            = undefined;
    telescope_p           = "LOFAR";
    observer_p            = undefined;
    antennaSet_p          = undefined;
    filterSelection_p     = undefined;
    clockFrequency_p      = 0.0;
    clockFrequencyUnit_p  = undefined;
    target_p              = undefined;
    systemVersion_p       = undefined;
    pipelineName_p        = undefined;
    pipelineVersion_p     = undefined;
    notes_p               = undefined;
  }
  
#ifdef HAVE_HDF5
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  void CommonAttributes::h5write (hid_t const &groupID)
  {
    DAL::h5set_attribute( groupID, "GROUPTYPE",              groupType_p );
    DAL::h5set_attribute( groupID, "FILENAME",               itsFilename );
    DAL::h5set_attribute( groupID, "FILETYPE",               filetype_p );
    DAL::h5set_attribute( groupID, "FILEDATE",               filedate_p );
    DAL::h5set_attribute( groupID, "TELESCOPE",              telescope_p );
    DAL::h5set_attribute( groupID, "OBSERVER",               observer_p );

    attributesProject_p.h5write(groupID);
    observation_p.h5write(groupID);

    DAL::h5set_attribute( groupID, "ANTENNA_SET",            antennaSet_p );
    DAL::h5set_attribute( groupID, "FILTER_SELECTION",       filterSelection_p );
    DAL::h5set_attribute( groupID, "CLOCK_FREQUENCY",        clockFrequency_p );
    DAL::h5set_attribute( groupID, "CLOCK_FREQUENCY_UNIT",   clockFrequencyUnit_p );
    DAL::h5set_attribute( groupID, "TARGET",                 target_p );
    DAL::h5set_attribute( groupID, "SYSTEM_VERSION",         systemVersion_p );
    DAL::h5set_attribute( groupID, "PIPELINE_NAME",          pipelineName_p );
    DAL::h5set_attribute( groupID, "PIPELINE_VERSION",       pipelineVersion_p );
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
    DAL::h5get_attribute( groupID, "FILENAME",               itsFilename );
    DAL::h5get_attribute( groupID, "FILETYPE",               filetype_p );
    DAL::h5get_attribute( groupID, "FILEDATE",               filedate_p );
    DAL::h5get_attribute( groupID, "TELESCOPE",              telescope_p );
    DAL::h5get_attribute( groupID, "OBSERVER",               observer_p );

    attributesProject_p.h5read (groupID);
    observation_p.h5read (groupID);

    DAL::h5get_attribute( groupID, "ANTENNA_SET",            antennaSet_p );
    DAL::h5get_attribute( groupID, "FILTER_SELECTION",       filterSelection_p );
    DAL::h5get_attribute( groupID, "CLOCK_FREQUENCY",        clockFrequency_p );
    DAL::h5get_attribute( groupID, "CLOCK_FREQUENCY_UNIT",   clockFrequencyUnit_p );
    DAL::h5get_attribute( groupID, "TARGET",                 target_p );
    DAL::h5get_attribute( groupID, "SYSTEM_VERSION",         systemVersion_p );
    DAL::h5get_attribute( groupID, "PIPELINE_NAME",          pipelineName_p );
    DAL::h5get_attribute( groupID, "PIPELINE_VERSION",       pipelineVersion_p );
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
  //                                                                getAttributes

#ifdef HAVE_CASA

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
  
} // Namespace DAL -- end
