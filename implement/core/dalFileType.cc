/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 3829 2009-12-09 13:04:56Z baehren                   $ |
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

#include <dalFileType.h>

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
    type_p = DAL::dalFileType::UNDEFINED;
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
    type_p = type (filetypeName);
  }
  
  //_____________________________________________________________________________
  //                                                                  dalFileType

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
    type_p = other.type_p;
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
      type_p = filetype;
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
    dalFileType::Type filetype = type (filetypeName);
    return setType (filetype);
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  void dalFileType::summary (std::ostream &os)
  {
    os << "[dalFileType] Summary of internal parameters." << std::endl;
    os << "-- File type      = " << type() << std::endl;
    os << "-- File type name = " << name() << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         type
  
  dalFileType::Type dalFileType::type (std::string const &fileTypeName)
  {
    dalFileType::Type filetype = dalFileType::UNDEFINED;

    if (fileTypeName == "HDF5") {
      filetype = dalFileType::HDF5;
    } else if (fileTypeName == "FITS") {
      filetype = dalFileType::FITS;
    } else if (fileTypeName == "MSCASA") {
      filetype = dalFileType::MSCASA;
    } else {
      filetype = dalFileType::UNDEFINED;
    }

    return filetype;
  }
  
  //_____________________________________________________________________________
  //                                                                         name
  
  std::string dalFileType::name ()
  {
    return name (type_p);
  }

  //_____________________________________________________________________________
  //                                                                         name
  
  /*!
    \param fileType -- File type to convert to corresponding string
    \return name    -- File type name.
  */
  std::string dalFileType::name (dalFileType::Type const &fileType)
  {
    std::string name;
    
    switch (fileType) {
    case dalFileType::HDF5:
      name = "HDF5";
      break;
    case dalFileType::FITS:
      name = "FITS";
      break;
    case dalFileType::MSCASA:
      name = "MSCASA";
      break;
    default:
      name = "UNDEFINED";
      break;
    };
    
    return name;
  }



} // Namespace DAL -- end
