/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1964 2008-09-06 17:52:38Z baehren                   $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Lars Baehren (bahren@astron.nl)                                       *
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
  
  CommonAttributes::CommonAttributes ()
  {
    init ();
  }
  
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
  
  CommonAttributes& CommonAttributes::operator= (CommonAttributes const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  void CommonAttributes::copy (CommonAttributes const &other)
  {;}

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void CommonAttributes::summary (std::ostream &os)
  {
    os << "[CommonAttributes] Summary of internal parameters." << std::endl;

    os << "-- Group type                  = " << groupType_p          << std::endl;
    os << "-- File name                   = " << filename_p           << std::endl;
    os << "-- File type                   = " << filetype_p           << std::endl;
    os << "-- File creation date          = " << filedate_p           << std::endl;
    os << "-- Telescope name              = " << telescope_p          << std::endl;
    os << "-- Project identifier          = " << projectID_p          << std::endl;
    os << "-- Name of the project         = " << projectName_p        << std::endl;
    os << "-- Project description         = " << projectDescription_p << std::endl;
    os << "-- Observer name(s)            = " << observer_p           << std::endl;
    os << "-- Observation ID              = " << observationID_p      << std::endl;
    os << "-- Observation date            = " << observationDate_p    << std::endl;
    os << "-- Observation mode            = " << observationMode_p    << std::endl;
    os << "-- Oberservation target(s)     = " << target_p             << std::endl;
    os << "-- Processing pipeline name    = " << pipelineName_p       << std::endl;
    os << "-- Processing pipeline version = " << pipelineVersion_p    << std::endl;

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
    groupType_p          = "Root";
    filename_p           = "UNDEFINED";
    filetype_p           = "UNDEFINED";
    filedate_p           = "UNDEFINED";
    telescope_p          = "LOFAR";
    projectID_p          = "UNDEFINED";
    projectName_p        = "UNDEFINED";
    projectDescription_p = "UNDEFINED";
    observer_p           = "UNDEFINED";
    observationID_p      = "UNDEFINED";
    observationDate_p    = "UNDEFINED";
    observationMode_p    = "UNDEFINED";
    target_p             = "UNDEFINED";
    systemTime_p         = "UNDEFINED";
    systemVersion_p      = "UNDEFINED";
  }
  
#ifdef HAVE_HDF5
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  void CommonAttributes::h5read (hid_t const &groupID)
  {
    DAL::h5get_attribute( groupID, "GROUPTYPE", groupType_p );
    DAL::h5get_attribute( groupID, "FILENAME",  filename_p  );
    DAL::h5get_attribute( groupID, "FILETYPE",  filetype_p  );
    DAL::h5get_attribute( groupID, "FILEDATE",  filedate_p  );
    DAL::h5get_attribute( groupID, "TELESCOPE", telescope_p );
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
