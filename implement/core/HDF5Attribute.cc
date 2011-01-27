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

#include "HDF5Attribute.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  HDF5Attribute::HDF5Attribute ()
    : HDF5Object ()
  {
    init ();
  }

  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  HDF5Attribute::HDF5Attribute (HDF5Attribute const &other)
    : HDF5Object (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Attribute::~HDF5Attribute ()
  {
    destroy();
  }
  
  void HDF5Attribute::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another HDF5Attribute object from which to make a copy.
  */
  HDF5Attribute& HDF5Attribute::operator= (HDF5Attribute const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5Attribute::copy (HDF5Attribute const &other)
  {
    itsDatatype  = other.itsDatatype;
    itsDataspace = other.itsDataspace;
    itsRank      = other.itsRank;
    itsStatus    = other.itsStatus;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void HDF5Attribute::init ()
  {
    itsDatatype  = -1;
    itsDataspace = -1;
    itsRank      = -1;
    itsStatus    = -1;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void HDF5Attribute::summary (std::ostream &os)
  {
    os << "[HDF5Attribute] Summary of internal parameters." << std::endl;
    os << "-- Attribute ID   = " << itsLocation     << std::endl;
    os << "-- Datatype ID    = " << itsDatatype     << std::endl;
    os << "-- Datatype class = " << datatypeClass() << std::endl;
    os << "-- Dataspace ID   = " << dataspace()     << std::endl;
    os << "-- Dataspace rank = " << rank()          << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================

  bool HDF5Attribute::open (hid_t const &location,
			    std::string const &name,
			    hid_t const &access)
  {
    bool status = true;
    itsLocation = HDF5Object::open (location, name, access);

    if (H5Iis_valid(itsLocation)) {
      /* Local variables */
      hsize_t sdim[64];
      /* Retrieve attribute information */
      itsDatatype  = H5Aget_type(itsLocation);
      itsDataspace = H5Aget_space(itsLocation);
      itsRank      = H5Sget_simple_extent_ndims(itsDataspace);
      itsStatus    = H5Sget_simple_extent_dims(itsDataspace, sdim, NULL);
    } else {
      return false;
    }

    return status;
  }
  
  
  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                            setAttribute(int)
  
  //! Set attribute of type (int)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    int const *data,
				    unsigned int const &size)
  {
    return setAttribute (location,
			 name,
			 data,
			 size,
			 H5T_NATIVE_INT);
  }

  //_____________________________________________________________________________
  //                                                          setAttribute(short)
  
  //! Set attribute of type (short)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    short const *data,
				    unsigned int const &size)
  {
    return setAttribute (location,
			 name,
			 data,
			 size,
			 H5T_NATIVE_SHORT);
  }

  //_____________________________________________________________________________
  //                                                           setAttribute(long)
  
  //! Set attribute of type (long)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    long const *data,
				    unsigned int const &size)
  {
    return setAttribute (location,
			 name,
			 data,
			 size,
			 H5T_NATIVE_LONG);
  }

  //_____________________________________________________________________________
  //                                                          setAttribute(float)
  
  //! Set attribute of type (float)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    float const *data,
				    unsigned int const &size)
  {
    return setAttribute (location,
			 name,
			 data,
			 size,
			 H5T_NATIVE_FLOAT);
  }

  //_____________________________________________________________________________
  //                                                         setAttribute(double)
  
  //! Set attribute of type (double)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    double const *data,
				    unsigned int const &size)
  {
    return setAttribute (location,
			 name,
			 data,
			 size,
			 H5T_NATIVE_DOUBLE);
  }

  //_____________________________________________________________________________
  //                                                    setAttribute(std::string)
  
  //! Set attribute of type (std::string)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    std::string const *data,
				    unsigned int const &size)
  {
    return setAttribute (location,
			 name,
			 data,
			 size,
			 H5T_NATIVE_CHAR);
  }
  
} // Namespace DAL -- end
