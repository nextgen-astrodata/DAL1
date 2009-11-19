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
  //  Construction
  //
  // ============================================================================
  
  CommonInterface::CommonInterface ()
  {;}
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  CommonInterface::~CommonInterface ()
  {
    destroy();
  }
  
  void CommonInterface::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  bool CommonInterface::setAttributes (std::set<std::string> const &attributes)
  {
    bool status (true);

    /* Clear the current set and copy the provided values */
    attributes_p.clear();
    attributes_p = attributes;
    /* Check consistency between internal and provided set */
    if (attributes_p.size() != attributes.size()) {
      std::cerr << "[CommonInterface::setAttributes]"
		<< " Mismatch in number of set elements!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
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
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
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
  
  void CommonInterface::summary (std::ostream &os)
  {
    os << "[CommonInterface] Summary of internal parameters." << std::endl;
    os << "-- Location ID = " << location_p                   << std::endl;
  }
  
} // Namespace DAL -- end
