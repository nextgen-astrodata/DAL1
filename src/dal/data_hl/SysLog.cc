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

#include <data_hl/SysLog.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                       SysLog
  
  SysLog::SysLog ()
  {
    location_p = 0;
  }
  
  //_____________________________________________________________________________
  //                                                                       SysLog
  
  /*
    \param location -- 
    \param flags    -- I/O mode flags.
   */
  SysLog::SysLog (hid_t const &location,
		  IO_Mode const &flags)
  {
    std::string name = getName();

    open (location,
	  name,
	  flags);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  SysLog::~SysLog ()
  {
    if (location_p > 0) {
      herr_t h5error;
      H5I_type_t object_type = H5Iget_type(location_p);
      // release HDF5 object
      if (object_type == H5I_GROUP) {
	h5error = H5Gclose(location_p);
	location_p = 0;
      }
    }
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                      summary
  
  void SysLog::summary (std::ostream &os)
  {
    os << "[SysLog] Summary of internal parameters." << std::endl;
    os << "-- Location ID = " << location_p << std::endl;
    os << "-- Group name  = " << getName()  << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void SysLog::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
  }

  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool SysLog::open (hid_t const &location,
		     std::string const &name,
		     IO_Mode const &flags)
  {
    bool status (true);

    /*______________________________________________________
      Check if the provided location ID points to a valid 
      object.
    */
    
    if (H5Iis_valid(location)) {
      /* Set up the list of attributes attached to the root group */
      setAttributes();
      
      /* Try to open the group: get list of groups attached to 'location' and
	 check if 'name' is part of it.
      */
      if (H5Lexists (location, name.c_str(), H5P_DEFAULT)) {
	location_p = H5Gopen (location,
			      name.c_str(),
			      H5P_DEFAULT);
      } else {
	location_p = 0;
      }
    } else {
      std::cerr << "[SysLog::open] Invalid location ID - aborting!" << std::endl;
      return false;
    }
    
    if (location_p > 0) {
      status = true;
    } else {
      /* If failed to open the group, check if we are supposed to create one */
      if ( (flags.flags() & IO_Mode::OpenOrCreate) ||
	   (flags.flags() & IO_Mode::Create) ||
	   (flags.flags() & IO_Mode::CreateNew) ) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  std::string groupName = "SysLog";
	  // write the attributes
	  HDF5Attribute::write (location_p, "GROUPTYPE", groupName);
	} else {
#ifdef DAL_DEBUGGING_MESSAGES
	  std::cerr << "[SysLog::open] Failed to create group "
		    << name
		    << std::endl;
#endif
	  status = false;
	}
      } else {
#ifdef DAL_DEBUGGING_MESSAGES
	std::cerr << "[SysLog::open] Failed to open group "
		  << name
		  << std::endl;
#endif
	status = false;
      }
    }
    
    return status;
  }
  
} // Namespace DAL -- end
