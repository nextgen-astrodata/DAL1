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

#include "Sky_ImageDataset.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             Sky_ImageDataset
  
  Sky_ImageDataset::Sky_ImageDataset ()
    : HDF5DatasetBase()
  {
  }
  
  //_____________________________________________________________________________
  //                                                             Sky_ImageDataset
  
  /*!
    \param location -- Object identifier for the location below which the
           dataset \c name is to be found.
    \param name     -- Name of the dataset.
    \param flags    -- I/O mode flags.
  */
  Sky_ImageDataset::Sky_ImageDataset (hid_t const &location,
				      std::string const &name,
				      IO_Mode const &flags)
    : HDF5DatasetBase(location, name, flags)
  {
  }

  //_____________________________________________________________________________
  //                                                             Sky_ImageDataset
  
  /*!
    \param location -- Identifier for the location at which the dataset is about
           to be created.
    \param name     -- Name of the dataset.
    \param shape    -- Shape of the dataset.
    \param datatype -- Datatype for the elements within the Dataset
    \param flags    -- I/O mode flags.
  */
  Sky_ImageDataset::Sky_ImageDataset (hid_t const &location,
				      std::string const &name,
				      std::vector< hsize_t > const &shape,
				      hid_t const &datatype,
				      IO_Mode const &flags)
    : HDF5DatasetBase(location, name, shape, datatype, flags)
  {
  }

  //_____________________________________________________________________________
  //                                                             Sky_ImageDataset
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  Sky_ImageDataset::Sky_ImageDataset (Sky_ImageDataset const &other)
    : HDF5DatasetBase (other)
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
    os << "-- Grouptype        = " << itsGroupType     << std::endl;
    os << "-- I/O mode flags   = " << itsFlags.names() << std::endl;
    os << "-- Attributes       = " << itsAttributes    << std::endl;
    os << "-- Path to WCS info = " << itsWCSinfo       << std::endl;
    os << "-- Dataset rank     = " << nofAxes()        << std::endl;
    os << "-- Dataset shape    = " << shape()          << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  

} // Namespace DAL -- end
