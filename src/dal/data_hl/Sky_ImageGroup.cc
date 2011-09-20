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

#include "Sky_ImageGroup.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  Sky_ImageGroup::Sky_ImageGroup ()
  {;}
  
  /*!
    \param other -- Another Sky_ImageGroup object from which to create this new
           one.
  */
  Sky_ImageGroup::Sky_ImageGroup (Sky_ImageGroup const &other)
    : HDF5GroupBase (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  Sky_ImageGroup::~Sky_ImageGroup ()
  {
    destroy();
  }
  
  void Sky_ImageGroup::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another Sky_ImageGroup object from which to make a copy.
  */
  Sky_ImageGroup& Sky_ImageGroup::operator= (Sky_ImageGroup const &other)
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
    \param other -- Another Sky_ImageGroup object from which to make a copy.
  */
  void Sky_ImageGroup::copy (Sky_ImageGroup const &other)
  {
    itsCoordinates = other.itsCoordinates;
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
  void Sky_ImageGroup::summary (std::ostream &os)
  {
    os << "[Sky_ImageGroup] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           image group is being attached.
    \param name   -- Name of the group to be opened.
    \param flags  -- I/O mode flags.
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool Sky_ImageGroup::open (hid_t const &location,
			     std::string const &name,
			     IO_Mode const &flags)
  {
    if (H5Iis_valid(location)) {
      
      // if ( H5Lexists (location, name.c_str(), H5P_DEFAULT) ) {
      // }
      
      setAttributes();
      return true;
    } else {
      return false;
    }
  }
  
  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  

  // ============================================================================
  //
  //  Private methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void Sky_ImageGroup::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
  }

  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  /*!
    \param flags   -- I/O mode flags.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool Sky_ImageGroup::openEmbedded (IO_Mode const &flags)
  {
    bool status = true;

    /* Check HDF5 identifier */
    if (H5Iis_valid(location_p)) {

      /* Check for coordinates group */
      if ( H5Lexists (location_p, "COORDINATES", H5P_DEFAULT) ) {
	  itsCoordinates = CoordinatesGroup (location_p, "COORDINATES");
      }

      /* Check for image dataset */
      if (H5Lexists (location_p, "DATA", H5P_DEFAULT)) {
	  itsDataset.open(location_p, "DATA", flags);
      }

      /* Check for source table */
      if (H5Lexists (location_p, "SOURCE_TABLE", H5P_DEFAULT)) {
	  status = true;
      }

      return status;
    } else {
      return false;
    }
  }

} // Namespace DAL -- end
