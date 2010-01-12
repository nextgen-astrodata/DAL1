/*-------------------------------------------------------------------------*
 | $Id:: dalCommon.cc 3919 2009-12-22 21:37:37Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include "HDF5Attribute.h"

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                HDF5Attribute

  HDF5Attribute::HDF5Attribute ()
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Attribute

  HDF5Attribute::HDF5Attribute (hid_t const &location,
				std::string const &name)
  {
    // Does 'location' point to a valid HDF5 object?
    if (H5Iis_valid(location)) {
      // Does attribute 'name' exist?
      if (H5Aexists(location,name.c_str())) {
	// Open the attribute and get its datatype
	location_p = H5Aopen (location,
			      name.c_str(),
			      H5P_DEFAULT);
	datatype_p = H5Aget_type (location_p);
	// Store the name of the attribute
	name_p = name;
      } else {
	init();
      }
    } else {
      init();
    }
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Attribute::~HDF5Attribute ()
  {
    if (location_p > 0 && H5Iis_valid(location_p)) {
      H5Aclose (location_p);
      location_p = 0;
    }
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  void HDF5Attribute::init ()
  {
    location_p = 0;
    name_p     = "";
    datatype_p = 0;
  }

  /*!
    \return rank -- Rank of the attribute's dataspace; returns <tt>-1</tt> in
            case the object is not connected to a valid attribute.
  */
  int HDF5Attribute::rank ()
  {
    if (location_p > 0) {
      return rank (location_p);
    } else {
      return -1;
    }
  }

  /*!
    \return rank -- Rank of the attribute's dataspace; returns <tt>-1</tt> in
            case the object is not connected to a valid attribute.
  */
  int HDF5Attribute::rank (hid_t const &location)
  {
    int rank (-1);
    hid_t dataspaceID (0);
    herr_t h5error;
    
    switch (H5Iget_type (location)) {
    case H5I_ATTR:
      dataspaceID = H5Aget_space (location);
      rank        = H5Sget_simple_extent_ndims (dataspaceID);
      h5error     = H5Sclose (dataspaceID);
      break;
    case H5I_DATASPACE:
      rank = H5Sget_simple_extent_ndims (location);
      break;
    default:
      std::cerr << "[HDF5Attribute::rank] Invalid input object!" << std::endl;
      break;
    };
    
    return rank;
  }

  //_____________________________________________________________________________
  //                                                                     datatype

  std::string HDF5Attribute::getDatatype (hid_t const &id)
  {
    std::string name;
    H5I_type_t objectType = H5Iget_type (id);

    if (objectType == H5I_DATATYPE) {
      
      if      (id == H5T_NATIVE_INT)    { name = "H5T_NATIVE_INT";    }
      else if (id == H5T_NATIVE_UINT)   { name = "H5T_NATIVE_UINT";   }
      else if (id == H5T_NATIVE_FLOAT)  { name = "H5T_NATIVE_FLOAT";  }
      else if (id == H5T_NATIVE_SHORT)  { name = "H5T_NATIVE_SHORT";  }
      else if (id == H5T_NATIVE_LONG)   { name = "H5T_NATIVE_LONG";   }
      else                              { name = "UNDEFINED"; }
    } 
    
    return name;
  }

  //_____________________________________________________________________________
  //                                                                      summary

  void HDF5Attribute::summary (std::ostream &os)
  {
    os << "[HDF5Attribute] Summary of internal parameters" << std::endl;
    os << " -- Location ID    = " << location_p << std::endl;
    os << " -- Attribute name = " << name_p     << std::endl;
    os << " -- Datatype ID    = " << datatype_p << std::endl;
  }
  
  // ============================================================================
  //
  //  Template specialization for getAttribute()
  //
  // ============================================================================
  
  //! Get attribute of type \e short 
  template <>
  bool HDF5Attribute::getAttribute (hid_t const &location,
				    std::string const &name,
				    short * value,
				    int size)
  {
    hid_t datatype = H5T_NATIVE_SHORT;
    return getAttribute (location,
			 name,
			 value,
			 size,
			 datatype);
  }

  //! Get attribute of type \e long 
  template <>
  bool HDF5Attribute::getAttribute (hid_t const &location,
				    std::string const &name,
				    long * value,
				    int size)
  {
    hid_t datatype = H5T_NATIVE_LONG;
    return getAttribute (location,
			 name,
			 value,
			 size,
			 datatype);
  }

  //! Get attribute of type \e int 
  template <>
  bool HDF5Attribute::getAttribute (hid_t const &location,
				    std::string const &name,
				    int * value,
				    int size)
  {
    hid_t datatype = H5T_NATIVE_INT;
    return getAttribute (location,
			 name,
			 value,
			 size,
			 datatype);
  }

  //! Get attribute of type \e uint 
  template <>
  bool HDF5Attribute::getAttribute (hid_t const &location,
				    std::string const &name,
				    uint * value,
				    int size)
  {
    hid_t datatype = H5T_NATIVE_UINT;
    return getAttribute (location,
			 name,
			 value,
			 size,
			 datatype);
  }

  //! Get attribute of type \e float 
  template <>
  bool HDF5Attribute::getAttribute (hid_t const &location,
				    std::string const &name,
				    float * value,
				    int size)
  {
    hid_t datatype = H5T_NATIVE_FLOAT;
    return getAttribute (location,
			 name,
			 value,
			 size,
			 datatype);
  }

  //! Get attribute of type \e double 
  template <>
  bool HDF5Attribute::getAttribute (hid_t const &location,
				    std::string const &name,
				    double * value,
				    int size)
  {
    hid_t datatype = H5T_NATIVE_DOUBLE;
    return getAttribute (location,
			 name,
			 value,
			 size,
			 datatype);
  }

};  //  end -- namespace DAL
