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

#include <CommonInterface.h>

namespace DAL { // Namespace DAL -- begin

  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  void CommonInterface::destroy ()
  {
    if (hasValidID()) {
      // Close the object
      H5Oclose (location_p);
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
  
  CommonInterface& CommonInterface::operator= (CommonInterface const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void CommonInterface::copy (CommonInterface const &other)
  {
    // Initialize internal variables
    location_p = 0;
    attributes_p.clear();
    // Copy variable values from other object
    location_p   = other.location_p;
    attributes_p = other.attributes_p;
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
  std::string CommonInterface::attribute (unsigned int const &index)
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
  bool CommonInterface::addAttribute (std::string const &name)
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
  bool CommonInterface::addAttributes (std::set<std::string> const &names)
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
    \return status -- Returns <tt>True</tt> if the element of <i>name</i>
            was removed from the set, <tt>False</tt> is the set did not contain
	    an element <i>name</i>.
  */
  bool CommonInterface::removeAttribute (std::string const &name)
  {
    return attributes_p.erase(name);
  }
  
  //_____________________________________________________________________________
  //                                                              removeAttribute
  
  bool CommonInterface::removeAttributes (std::set<std::string> const &names)
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
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                            incrementRefCount

  void CommonInterface::incrementRefCount ()
  {
    if (hasValidID()) {
      if (H5Iinc_ref(location_p) < 0) {
	std::cerr << "[CommonInterface::incrementRefCount]"
		  << " Failed to increment object reference count!"
		  << std::endl;
      }
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   hasValidID

  bool CommonInterface::hasValidID ()
  {
    return hasValidID (location_p);
  }
  
  bool CommonInterface::hasValidID (hid_t const &object_id)
  {
    H5I_type_t id_type = H5Iget_type(object_id);
    if (id_type <= H5I_BADID || id_type >= H5I_NTYPES)
      return false;
    else
      return true;
  }
  
  //_____________________________________________________________________________
  //                                                                   objectType
  
  H5I_type_t CommonInterface::objectType ()
  {
    return objectType(location_p);
  }

  /*!
    \param object_id -- Identifier for the object for which to check the type.
    \return type     -- The type identifier of the object.
  */
  H5I_type_t CommonInterface::objectType (hid_t const &object_id)
  {
    if (H5Iis_valid(object_id)) {
      return H5Iget_type (object_id);
    } else {
      return H5I_BADID;
    }
  }

  //_____________________________________________________________________________
  //                                                                   objectName
  
  std::string CommonInterface::objectName ()
  {
    std::string name;

    if (H5Iis_valid(location_p)) {
      h5get_name (name,location_p);
    } else {
      name = "";
    }

    return name;
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  bool CommonInterface::open (hid_t const &location)
  {
    bool status (true);
    bool absolutePath (false);
    herr_t h5error;
    std::string filename;
    std::string dataset;
    
    // Get name of file and dataset ________________________
    
    status  = DAL::h5get_filename (filename, location);
    status *= DAL::h5get_name (dataset, location,absolutePath);
    
    if (status) {
      // open the file
      hid_t fileID = H5Fopen (filename.c_str(),
			      H5F_ACC_RDWR,
			      H5P_DEFAULT);
      // open the dataset
      status = open (fileID,dataset,false);
      // release file handler
      h5error = H5Fclose (fileID);
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 locationName
  
  std::string CommonInterface::locationName ()
  {
    std::string name;
    
    if (!h5get_name(name,location_p)) {
      std::cerr << "[CommonInterface::locationName]"
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
  void CommonInterface::summary (std::ostream &os)
  {
    os << "[CommonInterface] Summary of internal parameters." << std::endl;
    os << "-- Location ID = " << location_p                   << std::endl;
  }
  
} // Namespace DAL -- end
