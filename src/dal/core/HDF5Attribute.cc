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
#include "HDF5Datatype.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                HDF5Attribute
  
  HDF5Attribute::HDF5Attribute ()
    : HDF5Object ()
  {
    init ();
  }

  //_____________________________________________________________________________
  //                                                                HDF5Attribute
  
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

  //_____________________________________________________________________________
  //                                                                HDF5Attribute
  
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

  //_____________________________________________________________________________
  //                                                                       rename
  
  /*!
    \param location -- Object identifier for the attribute to be renamed.
    \param newName  -- New name of the attribute.
  */
  bool HDF5Attribute::rename (hid_t const &location,
			      std::string const &newName)
  {
    bool status = true;

    if (H5Iis_valid(location)) {

      /* Retrieve the name of the object */
      std::string currenName = name (location);
      
      /* Returns a non-negative value if successful; otherwise returns a negative
	 value. */
      status = H5Arename (location,
			  currenName.c_str(),
			  newName.c_str());
      
      if (status > 0) {
	std::cerr << "[HDF5Attribute::rename] Error renaming attribute "
		  << currenName 
		  << " -> "
		  << newName
		  << std::endl;
	return false;
      }
      
    } else {
      std::cerr << "[HDF5Object::attributes] Invalid object identifier!"
		<< std::endl;
      status = false;
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                       rename
  
  /*!
    \param location -- Object identifier for the attribute to be renamed.
    \param name     -- Current name of the attribute.
    \param newName  -- New name of the attribute.
  */
  bool HDF5Attribute::rename (hid_t const &location,
			      std::string const &name,
			      std::string const &newName)
  {
    bool status = true;

    if (H5Iis_valid(location)) {
      
      /* Returns a non-negative value if successful; otherwise returns a negative
	 value. */
      status = H5Arename (location,
			  name.c_str(),
			  newName.c_str());
      
      if (status > 0) {
	std::cerr << "[HDF5Attribute::rename] Error renaming attribute "
		  << name 
		  << " -> "
		  << newName
		  << std::endl;
	return false;
      }
      
    } else {
      std::cerr << "[HDF5Object::attributes] Invalid object identifier!"
		<< std::endl;
      status = false;
    }

    return status;
  }
  
  
  //_____________________________________________________________________________
  //                                                                 read<string>
  
  /// @cond TEMPLATE_SPECIALIZATIONS
  template <>
  bool HDF5Attribute::read (hid_t const &location,
			    std::string const &name,
			    std::vector<std::string> &data)
  {
    bool status  = true;
    herr_t h5err = 0;
    
    /*____________________________________________________________
      Basic checks for reference location and attribute name.
    */
    
    if (H5Iis_valid(location)) {
      h5err = H5Aexists (location,
			 name.c_str());
    } else {
      std::cerr << "[HDF5Attribute::read]"
		<< " No valid HDF5 object found at reference location!"
		<< std::endl;
      return false;
    }
    
    /*____________________________________________________________
      If attribute has been found at reference location, open it.
      If opening has been successful, extract datatype and
      dataspace.
    */
    
    if (h5err>0) {
      hid_t attribute = H5Aopen (location,
				 name.c_str(),
				 H5P_DEFAULT);
      
      if (H5Iis_valid(attribute)) {
	
	std::vector<hsize_t> dims;
	std::vector<hsize_t> dimsMax;
	
	h5err           = HDF5Dataspace::shape (attribute,dims,dimsMax);
	hid_t datatype  = H5Aget_type (attribute);
	hid_t dataspace = H5Aget_space(attribute);
	
	if (dims.size()>0) {
	  
	  hid_t memtype  = H5Tcopy (H5T_C_S1);
	  /* Adjust the size of the buffer array */
	  char **buffer = (char **) std::malloc (dims[0] * sizeof (char *));
	  h5err = H5Tset_size (memtype, H5T_VARIABLE);
	  /* Read the attribute into the buffer */
	  h5err = H5Aread (attribute, memtype, buffer);
	  
	  /* Copy attribute data from buffer to returned array */
	  data.resize(dims[0]);
	  
	  for (hsize_t n=0; n<dims[0]; ++n) {
	    data[n] = buffer[n];
	  }
	  
	  /* Release allocated memory */
	  h5err = H5Dvlen_reclaim (memtype, dataspace, H5P_DEFAULT, buffer);
	  std::free (buffer);
	  
	} else {
	  return false;
	}
	
	/* Release HDF5 object identifiers */
	if (H5Iis_valid(datatype))  { H5Tclose (datatype);  };
	if (H5Iis_valid(dataspace)) { H5Sclose (dataspace); };
	if (H5Iis_valid(attribute)) { H5Aclose (attribute); };
      } else {
	std::cerr << "[HDF5Attribute::read]"
		  << " Failed to open attribute " << name << std::endl;
	status = false;
      }
    }   //  END -- (h5err>0)
    else {
      status = false;
    }
    
    return status;
  }
  /// @endcond

  //_____________________________________________________________________________
  //                                                                  write<char>
  
  /// @cond TEMPLATE_SPECIALIZATIONS
  //! Set attribute of type (char)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     char const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_CHAR);
  }
  /// @endcond
  
  //_____________________________________________________________________________
  //                                                                   write<int>
  
  /// \cond TEMPLATE_SPECIALIZATIONS
  /*!
    Set attribute of type (int)
  */
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     int const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_INT);
  }
  /// \endcond
  
  //_____________________________________________________________________________
  //                                                                  write(bool)
  
  /// \cond TEMPLATE_SPECIALIZATIONS
  //! Set attribute of type (bool)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     bool const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_INT);
  }
  /// \endcond
  
  //_____________________________________________________________________________
  //                                                          write(unsigned int)
  
  /// @cond TEMPLATE_SPECIALIZATIONS
  //! Set attribute of type (unsigned int)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     unsigned int const *data,
			     unsigned int const &size)
  {
    return write (location, name, data, size, H5T_NATIVE_UINT);
  }

  //_____________________________________________________________________________
  //                                                                 write(short)
  
  //! Set attribute of type (short)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     short const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_SHORT);
  }

  //_____________________________________________________________________________
  //                                                        write(unsigned short)
  
  //! Set attribute of type (unsigned short)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     unsigned short const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_USHORT);
  }

  //_____________________________________________________________________________
  //                                                                  write(long)
  
  //! Set attribute of type (long)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     long const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_LONG);
  }

  //_____________________________________________________________________________
  //                                                         write(unsigned long)
  
  //! Set attribute of type (long)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     unsigned long const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_ULONG);
  }
  
  //_____________________________________________________________________________
  //                                                             write(long long)
  
  //! Set attribute of type (long long)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     long long const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_LLONG);
  }
  
  //_____________________________________________________________________________
  //                                                    write(unsigned long long)
  
  //! Set attribute of type (unsigned long long)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     unsigned long long const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_ULLONG);
  }

  //_____________________________________________________________________________
  //                                                                 write(float)
  
  //! Set attribute of type (float)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     float const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_FLOAT);
  }
  
  //_____________________________________________________________________________
  //                                                                write(double)
  
  //! Set attribute of type (double)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     double const *data,
			     unsigned int const &size)
  {
    return write (location,
		  name,
		  data,
		  size,
		  H5T_NATIVE_DOUBLE);
  }
  
  //_____________________________________________________________________________
  //                                                           write(std::string)
  
  //! Set attribute of type (std::string)
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     std::string const *data,
			     unsigned int const &size)
  {
    bool status       = true;
    hid_t   attribute = 0;
    hid_t   dataspace = 0;
    hid_t   datatype  = H5Tcopy (H5T_C_S1);
    hsize_t dims[1]   = { size };
    hsize_t *maxdims  = NULL;
    herr_t  h5err     = 0;
 
    /*________________________________________________________________
      Basic checks for reference location and attribute name.
    */
    
    if (H5Iis_valid(location)) {
      h5err = H5Aexists (location,
			 name.c_str());
    } else {
      std::cerr << "[HDF5Attribute::write]"
		<< " No valid HDF5 object found at reference location!"
		<< std::endl;
      return false;
    }

    if ( (size==0) || (data==NULL)) {
      std::cerr << "[HDF5Attribute::write]"
		<< " Attribute value needs to at least contain one element!"
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
			       H5P_DEFAULT,
			       H5P_DEFAULT);
	/* ... and check if creation was successful */
	if (H5Iis_valid(attribute)) {
	  status = true;
	} else {
	  std::cerr << "[HDF5Attribute::write]"
		    << " H5Acreate() failed to create attribute "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[HDF5Attribute::write]"
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
	std::cerr << "[HDF5Attribute::write]"
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
  
  //_____________________________________________________________________________
  //                                                                  write(bool)
  
  /*!
    \brief Template specialization for type \e bool
  */
  template <>
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     bool const &data)
  {
    int buffer[] = {data};
    return write (location, name, buffer, 1);
  }

  //_____________________________________________________________________________
  //                                                          write(vector<bool>)
  
  bool HDF5Attribute::write (hid_t const &location,
			     std::string const &name,
			     std::vector<bool> const &data)
  {
    std::vector<int> buffer (data.size());
    for (size_t n=0; n<data.size(); ++n) {
      buffer[n] = data[n];
    }
    return write (location, name, &buffer[0], data.size());
  }
  
  /// @endcond

} // Namespace DAL -- end
