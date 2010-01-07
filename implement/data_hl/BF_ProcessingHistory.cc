/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1964 2008-09-06 17:52:38Z baehren                   $ |
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

#include <BF_ProcessingHistory.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                         BF_ProcessingHistory
  
  BF_ProcessingHistory::BF_ProcessingHistory ()
  {
    location_p = 0;
  }
  
  //_____________________________________________________________________________
  //                                                         BF_ProcessingHistory
  
  BF_ProcessingHistory::BF_ProcessingHistory (hid_t const &location,
					      bool const &create)
  {
    std::string name ("ProcessingHistory");

    open (location,
	  name,
	  create);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                        ~BF_ProcessingHistory
  
  BF_ProcessingHistory::~BF_ProcessingHistory ()
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
  
  void BF_ProcessingHistory::summary (std::ostream &os)
  {
    os << "[BF_ProcessingHistory] Summary of internal parameters." << std::endl;
    os << "-- Location ID = " << location_p << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void BF_ProcessingHistory::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("OBSPARSET");
    attributes_p.insert("PRESTOLOG");
    attributes_p.insert("PARFILE");
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
  bool BF_ProcessingHistory::open (hid_t const &location,
			std::string const &name,
			bool const &create)
  {
    bool status (true);

    /* Set up the list of attributes attached to the root group */
    setAttributes();

    /* Try to open the group: get list of groups attached to 'location' and
       check if 'name' is part of it.
    */
    std::set<std::string> groups;
    h5get_names (groups,location,H5G_GROUP);
    if (static_cast<bool>(attributes_p.count(name))) {
      location_p = H5Gopen2 (location,
			     name.c_str(),
			     H5P_DEFAULT);
    } else {
      location_p = 0;
    }
      
    if (location_p > 0) {
      status = true;
    } else {
      /* If failed to open the group, check if we are supposed to create one */
      if (create) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  std::string string_group ("ProcessingHistory");
	  // write the attributes
	  h5set_attribute (location_p, "GROUPTYPE", string_group);
	  h5set_attribute (location_p, "OBSPARSET", false);
	  h5set_attribute (location_p, "PRESTOLOG", false);
	  h5set_attribute (location_p, "PARFILE",   false);
	} else {
	  std::cerr << "[BF_ProcessingHistory::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[BF_ProcessingHistory::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool BF_ProcessingHistory::openEmbedded (bool const &create)
  {
    bool status = create;
    
    return status;
  }
  
} // Namespace DAL -- end
