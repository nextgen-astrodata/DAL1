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

#include "Sky_ImageDataset.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  Sky_ImageDataset::Sky_ImageDataset ()
  {;}
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  Sky_ImageDataset::Sky_ImageDataset (Sky_ImageDataset const &other)
    : HDF5Dataset (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  Sky_ImageDataset::~Sky_ImageDataset ()
  {
    destroy();
  }
  
  void Sky_ImageDataset::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another Sky_ImageDataset object from which to make a copy.
  */
  Sky_ImageDataset& Sky_ImageDataset::operator= (Sky_ImageDataset const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void Sky_ImageDataset::copy (Sky_ImageDataset const &other)
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
  void Sky_ImageDataset::summary (std::ostream &os)
  {
    os << "[Sky_ImageDataset] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  

} // Namespace DAL -- end
