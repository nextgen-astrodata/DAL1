/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#include "Sky_RootGroup.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  Sky_RootGroup::Sky_RootGroup ()
  {;}
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  Sky_RootGroup::Sky_RootGroup (Sky_RootGroup const &other)
    : HDF5GroupBase(other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  Sky_RootGroup::~Sky_RootGroup ()
  {
    destroy();
  }
  
  void Sky_RootGroup::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  Sky_RootGroup& Sky_RootGroup::operator= (Sky_RootGroup const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void Sky_RootGroup::copy (Sky_RootGroup const &other)
  {
    if (H5Iis_valid(other.location_p)) {
      location_p = -1;
    }
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
  void Sky_RootGroup::summary (std::ostream &os)
  {
    os << "[Sky_RootGroup] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  bool Sky_RootGroup::open (hid_t const &location,
			    std::string const &name,
			    IO_Mode const &flags)
  {
    bool status = flags.flags();
    
    std::cout << "[Sky_RootGroup::open]" << std::endl;
    std::cout << "-- Location ID    = " << location      << std::endl;
    std::cout << "-- Group name     = " << name          << std::endl;
    
    return status;
  }
  
  
} // Namespace DAL -- end
