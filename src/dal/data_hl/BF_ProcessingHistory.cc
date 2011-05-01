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

#include <data_hl/BF_ProcessingHistory.h>

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
    init ();
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
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void BF_ProcessingHistory::summary (std::ostream &os)
  {
    os << "[BF_ProcessingHistory] Summary of internal parameters." << std::endl;
    os << "-- Location ID = " << location_p   << std::endl;
    os << "-- Attributes  = " << attributes() << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  void BF_ProcessingHistory::init ()
  {
    location_p = -1;
    setAttributes ();
  }

  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void BF_ProcessingHistory::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("PARSET_OBS");
    attributes_p.insert("LOG_PRESTO");
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

    /* Basic initialization */
    init ();

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
    
    if (location_p > 0) {
      status = openEmbedded(create);
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
	  std::string string_group ("ProcessHist");
	  // write the attributes
	  HDF5Attribute::setAttribute (location_p, "GROUPTYPE",  string_group);
	  HDF5Attribute::setAttribute (location_p, "PARSET_OBS", false);
	  HDF5Attribute::setAttribute (location_p, "LOG_PRESTO", false);
	  HDF5Attribute::setAttribute (location_p, "PARFILE",    false);
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
  
  /*!
    \return status -- Status of the operation; return \e false in case an error 
            was encountered.
   */
  bool BF_ProcessingHistory::openEmbedded (bool const &create)
  {
    bool status = create;
    
    return status;
  }
  
} // Namespace DAL -- end
