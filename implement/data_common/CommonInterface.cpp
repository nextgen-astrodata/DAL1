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
  
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  void CommonInterface::summary (std::ostream &os)
  {
    os << "[CommonInterface] Summary of internal parameters." << std::endl;
    os << "-- Location ID = " << location_p                   << std::endl;
  }
  
} // Namespace DAL -- end
