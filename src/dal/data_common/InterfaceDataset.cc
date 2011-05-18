/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include "InterfaceDataset.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  InterfaceDataset::InterfaceDataset ()
    : HDF5Dataset()
  {
  }
  
  InterfaceDataset::InterfaceDataset (hid_t const &location,
				      std::string const &name)
    : HDF5Dataset(location,name)
  {
  }

  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  InterfaceDataset::InterfaceDataset (InterfaceDataset const &other)
    : HDF5Dataset (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  InterfaceDataset::~InterfaceDataset ()
  {
    destroy();
  }
  
  void InterfaceDataset::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another InterfaceDataset object from which to make a copy.
  */
  InterfaceDataset& InterfaceDataset::operator= (InterfaceDataset const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void InterfaceDataset::copy (InterfaceDataset const &other)
  {
    if (H5Iis_valid(other.itsLocation)) {
      itsLocation = -1;
    }
}

  // ============================================================================
  //
  //  Parameter access
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void InterfaceDataset::summary (std::ostream &os)
  {
    os << "[InterfaceDataset] Summary of internal parameters." << std::endl;
    os << "-- GROUPTYPE = " << itsGroupType << std::endl;
    os << "-- WCSINFO   = " << itsWCSinfo   << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  // ============================================================================
  //
  //  Private methods
  //
  // ============================================================================
  
  void InterfaceDataset::setAttributes ()
  {
    itsAttributes.clear();

    itsAttributes.insert("GROUPTYPE");
    itsAttributes.insert("WCSINFO");
    itsAttributes.insert("DATASET_NOF_AXES");
    itsAttributes.insert("DATASET_SHAPE");
  }

} // Namespace DAL -- end
