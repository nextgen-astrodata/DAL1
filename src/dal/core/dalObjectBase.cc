/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include <core/dalObjectBase.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                dalObjectBase
  
  /*!
    \param filetype -- File type: CASA_MS, HDF5, FITS, etc.
  */
  dalObjectBase::dalObjectBase (dalFileType const &filetype)
  {
    itsObjectHandler = NULL;
    itsFiletype      = filetype;
    itsFlags         = IO_Mode();
    itsName          = "";
  }

  //_____________________________________________________________________________
  //                                                                dalObjectBase
 
  /*!
    \param name     -- Name of the object (File, Dataset, Table, etc.)
    \param filetype -- File type: CASA_MS, HDF5, FITS, etc.
    \param flags    --  I/O mode flags
  */
  dalObjectBase::dalObjectBase (std::string const &name,
				dalFileType const &filetype,
				IO_Mode const &flags)
  {
    itsObjectHandler = NULL;
    itsFiletype      = filetype;
    itsFlags         = IO_Mode(flags);
    itsName          = name;
  }
  
  //_____________________________________________________________________________
  //                                                                dalObjectBase
  
  /*!
    \param other -- Another dalObjectBase object from which to create this new
           one.
  */
  dalObjectBase::dalObjectBase (dalObjectBase const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  dalObjectBase::~dalObjectBase ()
  {
    destroy();
  }
  
  void dalObjectBase::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another dalObjectBase object from which to make a copy.
  */
  dalObjectBase& dalObjectBase::operator= (dalObjectBase const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  /*!
    \param other -- Another dalObjectBase object from which to make a copy.
  */
  void dalObjectBase::copy (dalObjectBase const &other)
  {
    itsObjectHandler = other.itsObjectHandler;
    itsFiletype      = other.itsFiletype;
    itsFlags         = other.itsFlags;
    itsName          = other.itsName;
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
  void dalObjectBase::summary (std::ostream &os)
  {
    os << "[dalObjectBase] Summary of internal parameters." << std::endl;
    os << "-- File type      = " << itsFiletype.name() << std::endl;
    os << "-- I/O mode flags = " << itsFlags.names()   << std::endl;
    os << "-- Object name    = " << itsName            << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  

  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  

} // Namespace DAL -- end
