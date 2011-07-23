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

#include <core/HDF5Attribute.h>
#include <core/HDF5Datatype.h>

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
    \param location -- File or group identifier.
    \param name     -- Path to the object, relative to \c location.
    \param access   -- Access property list identifier for the link pointing to
           the object.
  */
  HDF5Attribute::HDF5Attribute (hid_t const &location,
				std::string const &name,
				hid_t const &access)
    : HDF5Object ()
  {
    init ();
    open (location, name, access);
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

  /*!
    \param location -- File or group identifier.
    \param name     -- Path to the object, relative to \c location.
    \param access   -- Access property list identifier for the link pointing to
           the object.
    \return status  -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool HDF5Attribute::open (hid_t const &location,
			    std::string const &name,
			    hid_t const &access)
  {
    bool status = true;
    itsLocation = HDF5Object::open (location, name, access);
    
    if (H5Iis_valid(itsLocation)) {
      /* Local variables */
      hsize_t sdim[128];
      /* Retrieve attribute information */
      itsDatatype  = H5Aget_type(itsLocation);
      itsDataspace = H5Aget_space(itsLocation);
      itsRank      = H5Sget_simple_extent_ndims(itsDataspace);
      itsStatus    = H5Sget_simple_extent_dims(itsDataspace, sdim, NULL);
    } else {
      std::cerr << "[HDF5Attribute::open] Failed to open object "
		<< name << "!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                   attributes
  
  /*!
    \param location -- Identifier of the HDF5 object, for which the information
           is being extracted.
  */
  std::set<std::string> HDF5Attribute::attributes (hid_t const &location)
  {
    std::set<std::string> names;
    
    if (H5Iis_valid(location)) {

      herr_t status   = 0;
      hid_t attribute = 0;
      hid_t datatype  = 0;
      hsize_t nofAttr = nofAttributes (location);
      
      if (nofAttr>0) {

	for (hsize_t n=0; n<nofAttr; ++n) {
	  attribute = H5Aopen_by_idx (location,
				      ".",
				      H5_INDEX_CRT_ORDER,
				      H5_ITER_INC,
				      n,
				      H5P_DEFAULT,
				      H5P_DEFAULT);
	  // Get the type of the attribute and its class
	  datatype = H5Aget_type(attribute);
	  /* Feedback */
	  std::cout << "index=" << n 
 		    << ", objectType=" << HDF5Object::objectType(attribute)
		    << ", objectName=" << HDF5Object::objectName(attribute)
		    << ", attribute="  << attribute
		    << ", datatype="   << HDF5Datatype::datatypeName(attribute)
		    << ", dataclass="  << HDF5Object::datatypeClass(attribute)
		    << std::endl;
	  /* Release object handlers */
	  status = H5Aclose(attribute);
	  status = H5Tclose(datatype);
	}
      }
    } else {
      std::cerr << "[HDF5Object::attributes] Invalid object identifier!"
		<< std::endl;
    }
    
    return names;
  }
  
  /// @cond TEMPLATE_SPECIALIZATIONS
  
  //_____________________________________________________________________________
  //                                                           setAttribute(char)
  
  //! Set attribute of type (char)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    char const *data,
				    unsigned int const &size)
  {
    return setAttribute (location, name, data, size, H5T_NATIVE_CHAR);
  }

  //_____________________________________________________________________________
  //                                                            setAttribute(int)
  
  //! Set attribute of type (int)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    int const *data,
				    unsigned int const &size)
  {
    return setAttribute (location, name, data, size, H5T_NATIVE_INT);
  }

  //_____________________________________________________________________________
  //                                                           setAttribute(bool)
  
  //! Set attribute of type (bool)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    bool const *data,
				    unsigned int const &size)
  {
    return setAttribute (location, name, data, size, H5T_NATIVE_HBOOL);
  }

  //_____________________________________________________________________________
  //                                                   setAttribute(unsigned int)
  
  //! Set attribute of type (unsigned int)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    unsigned int const *data,
				    unsigned int const &size)
  {
    return setAttribute (location, name, data, size, H5T_NATIVE_UINT);
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
    return setAttribute (location, name, data, size, H5T_NATIVE_SHORT);
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
  //                                                  setAttribute(unsigned long)
  
  //! Set attribute of type (long)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    unsigned long const *data,
				    unsigned int const &size)
  {
    return setAttribute (location,
			 name,
			 data,
			 size,
			 H5T_NATIVE_ULONG);
  }

  //_____________________________________________________________________________
  //                                                      setAttribute(long long)
  
  //! Set attribute of type (long long)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    long long const *data,
				    unsigned int const &size)
  {
    return setAttribute (location,
			 name,
			 data,
			 size,
			 H5T_NATIVE_LLONG);
  }

  //_____________________________________________________________________________
  //                                             setAttribute(unsigned long long)
  
  //! Set attribute of type (unsigned long long)
  template <>
  bool HDF5Attribute::setAttribute (hid_t const &location,
				    std::string const &name,
				    unsigned long long const *data,
				    unsigned int const &size)
  {
    return setAttribute (location,
			 name,
			 data,
			 size,
			 H5T_NATIVE_ULLONG);
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
    bool status       = true;
    hid_t   attribute = 0;
    hid_t   dataspace = 0;
    hid_t   datatype  = H5Tcopy (H5T_C_S1);
    hsize_t dims[1]   = { size };
    hsize_t *maxdims  = 0;
    herr_t h5err      = 0;

    /* Check if asked to write an empty string, if so abort to avoid
       zero size write error. */
    if (size == 0) return false;
    
    /*________________________________________________________________
      Basic checks for reference location and attribute name.
    */
    
    if (H5Iis_valid(location)) {
      h5err = H5Aexists (location,
			 name.c_str());
    } else {
      std::cerr << "[HDF5Attribute::setAttribute]"
		<< " No valid HDF5 object found at reference location!"
		<< std::endl;
      return false;
    }
    
    /*____________________________________________________________
      Check if attribute 'name' already exits at the given
      'location'; if this is not the case, we need to create the 
      attribute.
    */
    
    if (h5err>0) {
      attribute = H5Aopen (location,
			   name.c_str(),
			   H5P_DEFAULT);
    } else {
      /* Create dataspace for the attribute */
      h5err     = H5Tset_size (datatype, H5T_VARIABLE);
      dataspace = H5Screate_simple (1, dims, maxdims );
      if (H5Iis_valid(dataspace)) {
	/* Create the attribute itself ... */
	attribute = H5Acreate (location,
			       name.c_str(),
			       datatype,
			       dataspace,
			       0,
			       0);
	/* ... and check if creation was successful */
	if (H5Iis_valid(attribute)) {
	  status = true;
	} else {
	  std::cerr << "[HDF5Attribute::setAttribute]"
		    << " H5Acreate() failed to create attribute "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[HDF5Attribute::setAttribute]"
		  << " H5Screate_simple() failed to create dataspace!"
		  << std::endl;
	status = false;
      }
    }

    /*____________________________________________________________
      H5Awrite() returns a non-negative value if successful;
      otherwise returns a negative value. 
    */
    
    if (status) {
      datatype = H5Aget_type(attribute);
      /* Write the data to the attribute ... */
      h5err = H5Awrite (attribute, datatype, data);
      /* ... and check the return value of the operation */
      if (h5err<0) {
	std::cerr << "[HDF5Attribute::setAttribute]"
		  << " H5Awrite() failed to write attribute!"
		  << std::endl;
	status = false;
      }
    }
    
    /*____________________________________________________________
      Release HDF5 object handles
    */
    HDF5Object::close (datatype);
    HDF5Object::close (dataspace);
    HDF5Object::close (attribute);
    
    return status;
  }
  
  /// @endcond

  // ============================================================================
  //
  //
  //
  // ============================================================================
  
  // template bool HDF5Attribute::setAttribute (hid_t const &location,
  // 					     std::string const &name,
  // 					     int const &data);
  // template bool HDF5Attribute::setAttribute (hid_t const &location,
  // 					     std::string const &name,
  // 					     short const &data);
  // template bool HDF5Attribute::setAttribute (hid_t const &location,
  // 					     std::string const &name,
  // 					     long const &data);
  // template bool HDF5Attribute::setAttribute (hid_t const &location,
  // 					     std::string const &name,
  // 					     float const &data);
  // template bool HDF5Attribute::setAttribute (hid_t const &location,
  // 					     std::string const &name,
  // 					     double const &data);
  
} // Namespace DAL -- end
