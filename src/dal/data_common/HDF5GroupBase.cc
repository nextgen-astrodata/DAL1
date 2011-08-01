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

#include "HDF5GroupBase.h"

namespace DAL { // Namespace DAL -- begin

  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  void HDF5GroupBase::destroy ()
  {
    if (hasValidID()) {
      // Close the object
      HDF5Object::close(location_p);
      // Decrement reference count for the object
      if (H5Iis_valid(location_p)) {
	H5Idec_ref(location_p);
      }
    }
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                    operator=
  
  HDF5GroupBase& HDF5GroupBase::operator= (HDF5GroupBase const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5GroupBase::copy (HDF5GroupBase const &other)
  {
    // Initialize internal variables
    location_p = 0;
    attributes_p.clear();
    // Copy variable values from other object
    location_p   = other.location_p;
    attributes_p = other.attributes_p;
    itsGroupType = other.itsGroupType;
    // Book-keeping
    incrementRefCount ();
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    attribute

  /*!
    \param index -- The index number within the set, for which the attribute name
           is requested.

    \param name -- The name of the attribute at position <tt>index</tt> within
           the internal list
  */
  std::string HDF5GroupBase::attribute (unsigned int const &index)
  {
    unsigned int n (0);
    std::set<std::string>::iterator it;

    for (it=attributes_p.begin(); n<index; ++it) {
      ++n;
    }
    return *it;
  }

  //_____________________________________________________________________________
  //                                                                 addAttribute

  /*!
    \param name -- Name of the attribute to be added to the internally kept set.
    \return status -- Returns <tt>false</tt> if the provided attribute name
            already was in the internally kept list of attributes; if the 
	    attribute wasn't in the list previously and has been added as new
	    <tt>true</tt> is returned.
  */
  bool HDF5GroupBase::addAttribute (std::string const &name)
  {
    if (static_cast<bool>(attributes_p.count(name))) {
      return false;
    } else {
      attributes_p.insert(name);
      return true;
    }
  }

  //_____________________________________________________________________________
  //                                                                addAttributes

  /*!
    \param names -- Names of the attributes to be added.
  */
  bool HDF5GroupBase::addAttributes (std::set<std::string> const &names)
  {
    bool status (true);
    std::set<std::string>::iterator iterBegin = names.begin();
    std::set<std::string>::iterator iterEnd   = names.end();

    attributes_p.insert (iterBegin, iterEnd);

    return status;
  }

  //_____________________________________________________________________________
  //                                                              removeAttribute

  /*!
    \return status -- Returns \e true if the element of \c name was removed from
            the set, \e false is the set did not contain an element \c name.
  */
  bool HDF5GroupBase::removeAttribute (std::string const &name)
  {
    return attributes_p.erase(name);
  }
  
  //_____________________________________________________________________________
  //                                                              removeAttribute
  
  bool HDF5GroupBase::removeAttributes (std::set<std::string> const &names)
  {
    bool status (true);
    std::set<std::string>::iterator it;

    for (it=names.begin(); it!=names.end(); ++it) {
      status *= removeAttribute (*it);
    }
   
    return status;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                            incrementRefCount

  void HDF5GroupBase::incrementRefCount ()
  {
    if (hasValidID()) {
      if (H5Iinc_ref(location_p) < 0) {
	std::cerr << "[HDF5GroupBase::incrementRefCount]"
		  << " Failed to increment object reference count!"
		  << std::endl;
      }
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   hasValidID

  bool HDF5GroupBase::hasValidID ()
  {
    return hasValidID (location_p);
  }
  
  //_____________________________________________________________________________
  //                                                                   hasValidID

  bool HDF5GroupBase::hasValidID (hid_t const &location)
  {
    H5I_type_t id_type = H5Iget_type(location);
    if (id_type <= H5I_BADID || id_type >= H5I_NTYPES)
      return false;
    else
      return true;
  }
  
  //_____________________________________________________________________________
  //                                                                   objectType
  
  /*!
    \return type -- The type identifier of the object.
  */
  H5I_type_t HDF5GroupBase::objectType ()
  {
    return objectType(location_p);
  }

  //_____________________________________________________________________________
  //                                                                   objectType
  
  /*!
    \param location -- Identifier for the object for which to check the type.
    \return type    -- The type identifier of the object.
  */
  H5I_type_t HDF5GroupBase::objectType (hid_t const &location)
  {
    if (H5Iis_valid(location)) {
      return H5Iget_type (location);
    } else {
      std::cerr << "[HDF5GroupBase::objectType] Invalid object identifier!"
		<< std::endl;
      return H5I_BADID;
    }
  }

  //_____________________________________________________________________________
  //                                                                   objectName
  
  std::string HDF5GroupBase::objectName ()
  {
    return HDF5Object::objectName (location_p);
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  bool HDF5GroupBase::open (hid_t const &location)
  {
    bool status       = true;
    bool absolutePath = false;
    std::string filename;
    std::string dataset;
    
    // Get name of file and dataset ________________________
    
    status  = DAL::h5get_filename (filename, location);
    status *= DAL::h5get_name (dataset, location, absolutePath);
    
    if (status) {
      // open the file
      hid_t fileID = H5Fopen (filename.c_str(),
			      H5F_ACC_RDWR,
			      H5P_DEFAULT);
      if (fileID<0) {
	fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDONLY,
			  H5P_DEFAULT);
      }
      // open the dataset
      status = open (fileID, dataset, false);
      // release file handler
      HDF5Object::close (fileID);
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 locationName
  
  std::string HDF5GroupBase::locationName ()
  {
    std::string name;
    
    if (!h5get_name(name,location_p)) {
      std::cerr << "[HDF5GroupBase::locationName]"
		<< " Unable to retrieve name of location!"
		<< std::endl;
      name = "UNDEFINED";
    }
    
    return name;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void HDF5GroupBase::summary (std::ostream &os)
  {
    os << "[HDF5GroupBase] Summary of internal parameters." << std::endl;
    os << "-- Location ID    = " << location_p              << std::endl;
    os << "-- Group type ID  = " << itsGroupType            << std::endl;
    os << "-- I/O mode flags = " << itsFlags.names()        << std::endl;
  }

  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================

  /*!
    \retval groupID     -- 
    \param location     -- 
    \param name         -- Name of the group to be opened/created.
    \param flags        -- I/O mode flags.
    \return groupCreate -- Was \c H5Gcreate used in order to create the group
            from scratch?
    */
  bool HDF5GroupBase::open (hid_t &groupID,
			    hid_t const &location,
			    std::string const &name,
			    IO_Mode const &flags)
  {
    bool groupExists = false;
    bool groupCreate = false;
    
    /*______________________________________________________
      Check if the provided location identifier points to a
      valid object; if this is not the case abort. If the
      location ID is ok, check if the group already exists.
    */
    
    if ( H5Iis_valid(location) ) {

      if ( H5Lexists (location, name.c_str(), H5P_DEFAULT) ) {
	
	H5O_info_t info;
	H5Oget_info_by_name (location, name.c_str(), &info, H5P_DEFAULT);
	/* Check if 'name' points to a group object */
	if ( info.type == H5O_TYPE_GROUP ) {
	  groupExists = true;
	} else {
	  groupExists = false;
	}
	
      }
      
    } else {
      std::cerr << "[HDF5GroupBase::open] Invalid location ID!" << std::endl;
      groupID = 0;
      return false;
    }

    /*______________________________________________________
      Handle the different cases of I/O mode flags; use
      recursive call of this function to handle recursive
      relation of flags.
    */
    
    if ( flags.flags() & IO_Mode::OpenOrCreate ) {
      if (groupExists) {
	return open (groupID,location,name,IO_Mode(IO_Mode::Open));
      } else {
	return open (groupID,location,name,IO_Mode(IO_Mode::Create));
      }
    }
    else if ( flags.flags() & IO_Mode::Create ) {
      if (groupExists) {
	return open (groupID,location,name,IO_Mode(IO_Mode::Truncate));
      } else {
	return open (groupID,location,name,IO_Mode(IO_Mode::CreateNew));
      }
    }
    else if ( flags.flags() & IO_Mode::Open ) {
      if (groupExists) {
	groupID     = H5Gopen (location, name.c_str(), H5P_DEFAULT);
	groupCreate = false;
      } else {
	groupID     = 0;
	groupCreate = false;
      }
    }
    else if ( flags.flags() & IO_Mode::CreateNew ) {
      if (groupExists) {
	groupID     = 0;
	groupCreate = false;
      } else {
	groupCreate = true;
	groupID     = H5Gcreate (location,
				 name.c_str(),
				 H5P_DEFAULT,
				 H5P_DEFAULT,
				 H5P_DEFAULT);
      }
    }
    else if ( flags.flags() & IO_Mode::Truncate ) {
      // Delete the group ...
      H5Ldelete (location,
		 name.c_str(),
		 H5P_DEFAULT);
      // ... and create it once more from scratch
      groupCreate = true;
      groupID     = H5Gcreate (location,
			       name.c_str(),
			       H5P_DEFAULT,
			       H5P_DEFAULT,
			       H5P_DEFAULT);
    }
    
    return groupCreate;
  }
  
} // Namespace DAL -- end
