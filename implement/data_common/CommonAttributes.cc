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

    itsAttributesProject   = other.itsAttributesProject;

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
  
  void CommonAttributes::setAttributesProject (CommonAttributesProject const &attributesProject)
  {
    itsAttributesProject = attributesProject;
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
    
    os << "-- GROUPTYPE              = " << itsGroupType            << std::endl;
    os << "-- FILENAME               = " << itsFilename            << std::endl;
    os << "-- FILETYPE               = " << itsFiletype             << std::endl;
    os << "-- FILEDATE               = " << itsFiledate            << std::endl;
    os << "-- TELESCOPE              = " << itsTelescope           << std::endl;
    os << "-- OBSERVER               = " << itsObserver            << std::endl;
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
    itsFilename           = undefined;
    itsFiletype           = undefined;
    itsFiledate           = undefined;
    itsTelescope          = "LOFAR";
    itsObserver           = undefined;
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
    
    HDF5Attribute::setAttribute( groupID, "GROUPTYPE",            itsGroupType );
    HDF5Attribute::setAttribute( groupID, "FILENAME",             itsFilename );
    HDF5Attribute::setAttribute( groupID, "FILETYPE",             itsFiletype );
    HDF5Attribute::setAttribute( groupID, "FILEDATE",             itsFiledate );
    HDF5Attribute::setAttribute( groupID, "TELESCOPE",            itsTelescope );
    HDF5Attribute::setAttribute( groupID, "OBSERVER",             itsObserver );

    itsAttributesProject.h5write(groupID);
    itsAttributesObservation.h5write(groupID);

    HDF5Attribute::setAttribute( groupID, "ANTENNA_SET",          itsAntennaSet );
    HDF5Attribute::setAttribute( groupID, "FILTER_SELECTION",     itsFilterSelection );
    HDF5Attribute::setAttribute( groupID, "CLOCK_FREQUENCY",      itsClockFrequency );
    HDF5Attribute::setAttribute( groupID, "CLOCK_FREQUENCY_UNIT", itsClockFrequencyUnit );
    HDF5Attribute::setAttribute( groupID, "TARGET",               itsTarget );
    HDF5Attribute::setAttribute( groupID, "SYSTEM_VERSION",       itsSystemVersion );
    HDF5Attribute::setAttribute( groupID, "PIPELINE_NAME",        itsPipelineName );
    HDF5Attribute::setAttribute( groupID, "PIPELINE_VERSION",     itsPipelineVersion );
    HDF5Attribute::setAttribute( groupID, "NOTES",                itsNotes );

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

    itsAttributesProject.h5read (groupID);
    itsAttributesObservation.h5read (groupID);

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
  //                                                          setObservationStart
  
  void CommonAttributes::setObservationStart (std::string const &startMJD,
					      std::string const &startTAI,
					      std::string const &startUTC)
  {
    itsAttributesObservation.setStartMJD (startMJD);
    itsAttributesObservation.setStartTAI (startTAI);
    itsAttributesObservation.setStartUTC (startUTC);
  }
  
  //_____________________________________________________________________________
  //                                                            setObservationEnd
  
  void CommonAttributes::setObservationEnd (std::string const &endMJD,
					    std::string const &endTAI,
					    std::string const &endUTC)
  {
    itsAttributesObservation.setEndMJD (endMJD);
    itsAttributesObservation.setEndTAI (endTAI);
    itsAttributesObservation.setEndUTC (endUTC);
  }
  
} // Namespace DAL -- end
