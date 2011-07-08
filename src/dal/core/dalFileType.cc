/***************************************************************************
 *   Copyright (C) 2010-2011                                               *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#include <core/dalFileType.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                  dalFileType

  dalFileType::dalFileType ()
  {
    itsType = DAL::dalFileType::UNDEFINED;
  }
  
  //_____________________________________________________________________________
  //                                                                  dalFileType

  dalFileType::dalFileType (dalFileType::Type const &filetype)
  {
    setType (filetype);
  }
  
  //_____________________________________________________________________________
  //                                                                  dalFileType

  dalFileType::dalFileType (std::string const &filetypeName)
  {
    itsType = getType (filetypeName);
  }
  
  //_____________________________________________________________________________
  //                                                                  dalFileType

  /*!
    \param other -- Another dalFileType object from which to create this new one.
  */
  dalFileType::dalFileType (dalFileType const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  dalFileType::~dalFileType ()
  {
    destroy();
  }
  
  void dalFileType::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another dalFileType object from which to make a copy.
  */
  dalFileType& dalFileType::operator= (dalFileType const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void dalFileType::copy (dalFileType const &other)
  {
    itsType = other.itsType;
  }

  // ============================================================================
  //
  //  Parameter access
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                      setType
  
  /*!
    \param filetype -- 
    \return status      -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool dalFileType::setType (dalFileType::Type const &filetype)
  {
    bool status (true);

    try {
      itsType = filetype;
    } catch (std::string message) {
      std::cerr << "[dalFileType::setType] Failed to set type! Invalid value."
		<< std::endl;
      status = false;
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      setType
  
  /*!
    \param filetypeName -- 
    \return status      -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool dalFileType::setType (std::string const &filetypeName)
  {
    dalFileType::Type filetype = getType (filetypeName);
    return setType (filetype);
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void dalFileType::summary (std::ostream &os)
  {
    os << "[dalFileType] Summary of internal parameters." << std::endl;
    os << "-- File type      = " << type() << std::endl;
    os << "-- File type name = " << name() << std::endl;
  }

  // ============================================================================
  //
  //  Public methods 
  //
  // ============================================================================

  bool dalFileType::isFITS ()
  {
    switch (itsType) {
    case dalFileType::FITS:
      return true;
      break;
    default:
      return false;
      break;
    };
  }

  bool dalFileType::isHDF5 ()
  {
    switch (itsType) {
    case dalFileType::HDF5:
      return true;
      break;
    default:
      return false;
      break;
    };
  }
  
  bool dalFileType::isCASA ()
  {
    switch (itsType) {
    case dalFileType::CASA_MS:
    case dalFileType::CASA_IMAGE:
      return true;
      break;
    default:
      return false;
      break;
    };
  }
  
  //_____________________________________________________________________________
  //                                                                         name
  
  std::string dalFileType::name ()
  {
    return getName (itsType);
  }

  // ============================================================================
  //
  //  Static methods 
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                       getMap
  
  std::map<dalFileType::Type,std::string> dalFileType::getMap ()
  {
    std::map<dalFileType::Type,std::string> result;

    result[HDF5]       = "HDF5";
    result[FITS]       = "FITS";
    result[CASA_MS]    = "CASA_MS";
    result[CASA_IMAGE] = "CASA_IMAGE";
    result[UNDEFINED]  = "UNDEFINED";

    return result;
  }

  //_____________________________________________________________________________
  //                                                                         type
  
  dalFileType::Type dalFileType::getType (std::string const &name)
  {
    std::map<dalFileType::Type,std::string> typesMap     = getMap ();
    std::map<dalFileType::Type,std::string>::iterator it = typesMap.begin();
    dalFileType::Type result                             = it->first;

    for (it=typesMap.begin(); it!=typesMap.end(); ++it) {
      if (it->second == name) {
	result = it->first;
      }
    }

    return result;
  }

  //_____________________________________________________________________________
  //                                                                         name
  
  /*!
    \param fileType -- File type to convert to corresponding string
    \return name    -- File type name.
  */
  std::string dalFileType::getName (dalFileType::Type const &fileType)
  {
    std::map<dalFileType::Type,std::string> typesMap     = getMap ();
    std::map<dalFileType::Type,std::string>::iterator it = typesMap.find(fileType);
    std::string result;

    if (it==typesMap.end()) {
      result = typesMap.begin()->second;
    } else {
      result = it->second;
    }

    return result;
  }

} // Namespace DAL -- end
