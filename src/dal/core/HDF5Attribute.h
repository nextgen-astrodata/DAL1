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

#ifndef HDF5ATTRIBUTE_H
#define HDF5ATTRIBUTE_H

#include <cstdlib>

#include <core/HDF5Dataspace.h>

#ifdef DAL_WITH_CASA
#include <casa/Arrays/Vector.h>
#endif

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class HDF5Attribute
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class HDF5Attribute
    
    \author Lars B&auml;hren

    \date 2011/01/27

    \test tHDF5Attribute.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>Attribute Names: While any ASCII or UTF-8 character may be used in
      the name given to an attribute, it is usually wise to avoid the following
      kinds of characters:
      <ul>
        <li>Commonly used separators or delimiters such as slash, backslash,
	colon, and semi-colon (\, /, :, ;)
	<li>Escape characters
	<li>Wild cards such as asterisk and question mark (*, ?)
      </ul>
      \c NULL can be used within a name, but HDF5 names are terminated with a \c NULL:
      whatever comes after the \c NULL will be ignored by HDF5.
    </ul>

    <h3>Synopsis</h3>

    In order for derived classes to provide access to attributes attached to e.g.
    a group or a dataset, the following methods should be implemented:
    <ol>
      <li>Create/set attribute.
      \code
      // Create attribute / Get attribute value
      template <class T>
      inline bool write (std::string const &name,
			 T const *data,
			 unsigned int const &size)
      {
        return HDF5Attribute::write (location, name, data, size);
      }

      // Create attribute / Get attribute value
      template <class T>
      inline bool write (std::string const &name,
			 std::vector<T> &data)
      {
        return HDF5Attribute::write (location, name, &data[0], data.size());
      }

      // Create attribute / Get attribute value
      template <class T>
      inline bool write (std::string const &name,
				T const &data)
      {
	return HDF5Attribute::write (location, name, &data, 1);
      }
      \endcode
      <li>Get attribute value:
      \code
      // Get attribute value
      template <class T>
      inline bool read (std::string const &name,
                        std::vector<T> &data)
      {
        return HDF5Attribute::read (location, name, &data[0], data.size());
      }
      \endcode
    </ol>

    \b Remarks: 

    <ul>
      <li>Since at the time of creating this class there is no proper \e bool
      datatype, attributes of such type are being mapped onto \e int.
    </ul>
    
    <h3>Example(s)</h3>
    
  */  
  class HDF5Attribute : public HDF5Object {

    //! Datatype identifier
    hid_t itsDatatype;
    //! Dataspace identifier
    hid_t itsDataspace;
    //! Rank of the dataspace
    int   itsRank;
    //! Error status
    herr_t itsStatus;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    HDF5Attribute ();
    
    //! Argumented constructor
    HDF5Attribute (hid_t const &location,
		   std::string const &name,
		   hid_t const &access=H5P_DEFAULT);
    
    //! Copy constructor
    HDF5Attribute (HDF5Attribute const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~HDF5Attribute ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    HDF5Attribute& operator= (HDF5Attribute const &other); 
    
    // === Parameter access =====================================================
    
    //! Get the datatype identifier
    inline hid_t datatype () const {
      return itsDatatype;
    }

    //! Get the dataspace identifier
    inline hid_t dataspace () const {
      return itsDataspace;
    }

    //! Get the rank of the dataspace
    inline int rank () const {
      return itsRank;
    }

    //! Get error status of the object
    inline herr_t status () const {
      return itsStatus;
    }

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, HDF5Attribute.
    */
    inline std::string className () const {
      return "HDF5Attribute";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================

    //! Open attribute \c name attached to \c location.
    bool open (hid_t const &location,
	       std::string const &name,
	       hid_t const &access=H5P_DEFAULT);

    //! Returns the datatype class identifier.
    inline H5T_class_t datatypeClass () {
      return HDF5Object::datatypeClass(itsLocation);
    }
    
    // === Static methods =======================================================

    //! Name of attributes attached to the object
    static std::set<std::string> attributes (hid_t const &location);
    
    //! Rename the attribute
    static bool rename (hid_t const &location,
			std::string const &newName);

    //! Rename the attribute
    static bool rename (hid_t const &location,
			std::string const &oldName,
			std::string const &newName);
    
    /*!
      \brief Read attribute value
      \param location -- HDF5 identifier for the object to which the attribute
             is attached.
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      static bool read (hid_t const &location,
			std::string const &name,
			std::vector<T> &data)
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

	      hid_t nativeDatatype      = H5Tget_native_type(datatype, H5T_DIR_ASCEND);
	      /* hsize_t datatypeSize      = H5Tget_size (datatype); */
	      H5T_class_t datatypeClass = H5Tget_class (datatype);
	      int size                  = 1;

	      for (size_t n=0; n<dims.size(); ++n) {
		size *= dims[n];
	      }
	      
	      /* Adjust size of array returning the attribute data */
	      data.resize(size);
	      
	      /* Retrieve the attribute data */
	      if (datatypeClass == H5T_STRING) {
		std::cout << "[HDF5Attribute::read]"
			  << " Attribute of type string - not yet supported!"
			  << std::endl;
	      } else {
		h5err = H5Aread (attribute,
				 nativeDatatype,
				 &data[0]);
	      }
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

    /*!
      \brief Read attribute value
      \param location -- HDF5 identifier for the object to which the attribute
             is attached.
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      static bool read (hid_t const &location,
			std::string const &name,
			T &data)
      {
	std::vector<T> buffer;
	
	read (location, name, buffer);

	if (buffer.empty()) {
	  return false;
	} else if (buffer.size() > 1) {
	  data = buffer[0];
	  return false;
	} else {
	  data = buffer[0];
	  return true;
	}
      }
    
    /*!
      \brief Write attribute value
      \param location -- HDF5 identifier for the object to which the attribute
             is attached.
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \param size    -- nof. element in the data array.
      \return status -- Status of the operation
    */
    template <class T>
      static bool write (hid_t const &location,
			 std::string const &name,
			 T const *data,
			 unsigned int const &size,
			 hid_t const &datatype)
      {
	bool status       = true;
	hid_t   attribute = 0;
	hid_t   dataspace = 0;
	hsize_t dims[1]   = { size };
	hsize_t *maxdims  = 0;
	herr_t h5err      = 0;
	
	/*____________________________________________________________
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
	  Datatype conversion takes place at the time of a read or
	  write and is automatic. See the Data Conversion section of
	  "The Data Type Interface (H5T)" in the "HDF5 User's Guide"
	  for a discussion of data conversion, including the range of
	  conversions currently supported by the HDF5 libraries. 
	*/
	
	if (status) {
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
	HDF5Object::close (dataspace);
	HDF5Object::close (attribute);
	
	return status;
      }
    
    /*!
      \brief Write attribute value
      \param location -- HDF5 identifier for the object to which the attribute
             is attached.
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \param size    -- nof. element in the data array.
      \return status -- Status of the operation
    */
    template <class T>
      static bool write (hid_t const &location,
			 std::string const &name,
			 T const *data,
			 unsigned int const &size);
    
    /*!
      \brief Write attribute value
      \param location -- HDF5 identifier for the object to which the attribute
             is attached.
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation
    */
    template <class T>
      static bool write (hid_t const &location,
			 std::string const &name,
			 T const &data)
      {
	T buffer[] = {T(data)};
	return write (location, name, buffer, 1);
      }
    
    /*!
      \brief Write attribute value
      \param location -- HDF5 identifier for the object to which the attribute
             is attached.
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation
    */
    template <class T>
      static bool write (hid_t const &location,
			 std::string const &name,
			 std::vector<T> const &data)
      {
	return write (location, name, &data[0], data.size());
      }
    //! Write attribute value of type \e bool
    static bool write (hid_t const &location,
		       std::string const &name,
		       std::vector<bool> const &data);


    /*!
      \brief Write attribute value
      \param location -- HDF5 identifier for the object to which the attribute
             is attached.
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation
    */
    template <class T>
      bool write (hid_t const &location,
		  std::string const &name,
		  std::set<T> const &data)
      {
	std::vector<T> tmp (data.size());
	typename std::set<T>::iterator itSet=data.begin();
	typename std::vector<T>::iterator itVect=tmp.begin();
	
	for (; itSet!=data.end(); ++itSet) {
	  *itVect = *itSet;
	  ++itVect;
	}
	
	return write (location,
		      name,
		      tmp);
      }
    
#ifdef DAL_WITH_CASA
    /*!
      \brief Write attribute value
      \param location -- HDF5 identifier for the object to which the attribute
             is attached.
      \param name    -- Name of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation
    */
    template <class T>
      static bool write (hid_t const &location,
			 std::string const &name,
			 casa::Vector<T> const &data)
      {
	return write (location, name, &data[0], data.nelements());
      }
    
#endif
    
    /*!
      \brief Write multiple attributes to the same value.
      \param location -- HDF5 identifier for the object to which the attribute
             is attached.
      \param name    -- Names of the attribute.
      \param data    -- Data value(s) to be assigned to the attribute
      \return status -- Status of the operation
    */
    template <class T>
      static bool write (hid_t const &location,
			 std::vector<std::string> const &name,
			 T const &data)
      {
	bool status = true;
	T buffer[]  = {data};
	for (unsigned int n=0; n<name.size(); ++n) {
	  status *= write (location, name[n], buffer, 1);
	}
	return status;
      }
    
  private:
    
    //! Initialize internal parameters to default values
    void init ();

    //! Unconditional copying
    void copy (HDF5Attribute const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class HDF5Attribute -- end

} // Namespace DAL -- end

#endif /* HDF5ATTRIBUTE_H */
  
