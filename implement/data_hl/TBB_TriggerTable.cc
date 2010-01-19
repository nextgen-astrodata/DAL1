/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#include <TBB_TriggerTable.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             TBB_TriggerTable

  TBB_TriggerTable::TBB_TriggerTable ()
  {
    location_p = 0;
  }

  //_____________________________________________________________________________
  //                                                             TBB_TriggerTable

  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name -- Name of the structure (file, group, dataset, etc.) to be
           opened.
  */
  TBB_TriggerTable::TBB_TriggerTable (hid_t const &location,
				      std::string const &name)
  {
    open (location, name, true);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  TBB_TriggerTable::~TBB_TriggerTable ()
  {
    destroy();
  }
  
  void TBB_TriggerTable::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  TBB_TriggerTable& TBB_TriggerTable::operator= (TBB_TriggerTable const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void TBB_TriggerTable::copy (TBB_TriggerTable const &other)
  {
    location_p  = other.location_p;
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void TBB_TriggerTable::summary (std::ostream &os)
  {
    os << "[TBB_TriggerTable] Summary of internal parameters." << std::endl;
    os << "-- Trigger table ID = " << location_p               << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void TBB_TriggerTable::setAttributes ()
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
  bool TBB_TriggerTable::open (hid_t const &location,
			       std::string const &name,
			       bool const &create)
  {
    bool status (true);

    /* Internal setup */
    setAttributes();

    if (H5Lexists (location, name.c_str(), H5P_DEFAULT)) {
      location_p = H5Gopen (location,
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
	  std::string grouptype ("TriggerTable");
	  // write the attributes
	  h5set_attribute (location_p, "GROUPTYPE", grouptype);
	} else {
	  std::cerr << "[TBB_TriggerTable::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[TBB_TriggerTable::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[TBB_TriggerTable::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool TBB_TriggerTable::openEmbedded (bool const &create)
  {
    bool status = create;
    status      = true;
    return status;
  }


} // Namespace DAL -- end
