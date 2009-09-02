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

#include <dalAttribute.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  dalAttribute::dalAttribute ()
  {
  }

  dalAttribute::dalAttribute (Name const &id)
  {
    attributeID_p = id;
  }
  
  dalAttribute::dalAttribute (dalAttribute const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  dalAttribute::~dalAttribute ()
  {
    destroy();
  }
  
  void dalAttribute::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  dalAttribute& dalAttribute::operator= (dalAttribute const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  void dalAttribute::copy (dalAttribute const &other)
  {
    attributeID_p = other.attributeID_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void dalAttribute::summary (std::ostream &os)
  {
    os << "[dalAttribute] Summary of internal parameters." << std::endl;
  }
  
  
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  std::string dalAttribute::getName (Name const &id)
  {
    std::string name;
    
    switch (id) {
      /************************************************************************
       * Common attributes to be attached to the root group of every data-set
       ************************************************************************/
    case GROUPTYPE:
      name = "GROUPTYPE";
      break;
    case FILENAME:
      name = "FILENAME";
      break;
    case FILETYPE:
      name = "FILETYPE";
      break;
    case FILEDATE:
      name = "FILEDATE";
      break;
    case TELESCOPE:
      name = "TELESCOPE";
      break;
    case PROJECT_ID:
      name = "PROJECT_ID";
      break;
    case PROJECT_NAME:
      name = "PROJECT_NAME";
      break;
    case PROJECT_DESCRIPTION:
      name = "PROJECT_DESCRIPTION";
      break;
      /************************************************************************
       * Attributes required for TBB data
       ************************************************************************/
    case STATION_ID:
      name = "STATION_ID";
      break;
    case RSP_ID:
      name = "RSP_ID";
      break;
    case RCU_ID:
      name = "RCU_ID";
      break;
    case ANTENNA_POSITION_VALUE:
      name = "ANTENNA_POSITION_VALUE";
      break;
    case ANTENNA_POSITION_UNIT:
      name = "ANTENNA_POSITION_UNIT";
      break;
    case ANTENNA_POSITION_FRAME:
      name = "ANTENNA_POSITION_FRAME";
      break;
      /************************************************************************
       * Default value (in case attribute is unknown)
       ************************************************************************/
    default:
      name = "UNDEFINED";
      break;
    };
    
    return name;
  }
  
} // Namespace DAL -- end
