/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Joseph Masters (J.S.Masters@uva.nl)                                   *
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

#ifndef DALCOMMON_H
#define DALCOMMON_H

#include <iostream>
#include <string>
#include <sstream>

#ifdef HAVE_CASA
#include <casa/Arrays/IPosition.h>
#include <casa/Arrays/Vector.h>
#include <casa/Quanta/Quantum.h>
#include <measures/Measures/MDirection.h>
#include <measures/Measures/MPosition.h>
using casa::MDirection;
using casa::MPosition;
using casa::MVPosition;
using casa::Quantity;
#endif

#include "dalBaseTypes.h"
#include "Enumerations.h"

/*!
  \file dalCommon.h

  \ingroup DAL 

  \brief A collection of commonly used routines (for the Data Access Library)

  \author Joseph Masters, Lars B&auml;hren

  \test tdalCommon.cpp

  <h3>Prerequisite</h3>
  
  <ul type="square">
    <li>HDF5 Library
    <ul>
      <li><a href="http://hdfgroup.com/HDF5/doc/RM_H5Front.html">API
      Specification</a>
      <li><a href="http://hdf.ncsa.uiuc.edu/HDF5/doc/UG/index.html">User's
      Guide</a>
      <li><a href="http://hdfgroup.org/HDF5/hdf5_hl/doc/RM_hdf5lt.html">HDF5
      Lite API</a> - higher-level functions which do more operations per call
      than the basic HDF5 interface. The purpose is to wrap intuitive
      functions around certain sets of features in the existing APIs. This
      version of the API has two sets of functions: dataset and attribute
      related functions.
    </ul>
    <li>casacore Library
    <ul>
      <li><a href="http://www.astron.nl/casacore/doc/html">API
      Specification</a>
    </ul>
  </ul>

  <h3>Synopsis</h3>
  
  A number of different things are included into this collection of commonly
  used routines:
  - Conversion routines
    - DAL::julday
    - DAL::mjd2unix
  - Service functions
    - DAL::it_exists
  - Routines for the access of HDF5 attributes
  - Boost.Python wrappers 
    - DAL::mjd2unix_boost

  <h3>Coding conventions</h3>

  All get/set functions wrapping the underlying functionality should conform to
  a common ordering of the function arguments to avoid confusion:
  \code
  template <typename T>
    bool h5get_attribute (hid_t const &attribute_id,
			  T &value);

  template <typename T>
    bool h5get_attribute (hid_t const &locationID,
			  std::string const &name,
			  T &value);  
  template <typename T>
    bool h5get_attribute (hid_t const &attribute_id,
			  std::vector<T> &value);

  template <typename T>
    bool h5get_attribute (hid_t const &locationID,
			  std::string const &name,
			  std::vector<T> &value);  
  \endcode
  
*/

namespace DAL {
  
  // ============================================================================
  //
  //  Conversion routines
  //
  // ============================================================================

  //! Convert UNIX time in to Julian Day
  long double julday (time_t seconds,
		      long *intmjd,
		      long double *fracmjd);
  
  //! Convert Modified Julian Date (mjd) to unix time  
  double mjd2unix (double mjd_time);

  // ============================================================================
  //
  //  Service routines
  //
  // ============================================================================
  
  //! Check if an object exists in a vector
  template <typename T>
    bool it_exists( std::vector<T> vec, T item )
    {
      typename std::vector<T>::iterator it;
      for ( it=vec.begin() ; it < vec.end(); it++ ) {
	if ( item == *it ) {
	  return true;
	}
      }
      return false;
    }
  
  //! Retrieve the name of an object based on the object identifier
  bool h5get_name (std::string &name,
                   hid_t const &object_id);
  
  //! Retrieve the name of an object based on the object identifier
  bool h5get_name (std::string &name,
                   hid_t const &object_id,
                   hsize_t const &index);
  
  //! Retrieve the name of an object based on the object identifier
  bool h5get_filename (std::string &filename,
                       hid_t const &object_id);

  // ============================================================================
  //
  //  Access to HDF5 attributes
  //
  // ============================================================================
 
  //! Provide a summary of an attribute's internal structure
  void h5attribute_summary (std::ostream &os,
                            hid_t const &attribute_id);
  
  //! Print an attribute value. (Used with H5Aiterate).
  herr_t attr_info (hid_t loc_id,
		    const char *name,
		    void *opdata);
  
  //! Get the shape of a dataset
  bool h5get_dataset_shape (std::vector<uint> &shape,
                            hid_t const &dataset_id);
  //! Get the shape of the dataspace associated with the attribute
  bool h5get_dataspace_shape (std::vector<uint> &shape,
                              hid_t const &attribute_id);
  /*!
    \todo Function not yet implemented
    \brief Provide a summary of an attribute's internal structure

    \param os          -- Output stream to which the summary is written
    \param name        -- Name of the attribute, as which it is attached to
           another structure within the file
    \param location_id -- Identifier of the structure within the file, to which
           the attribut is attached to.
  */
  void h5attribute_summary (std::ostream &os,
                            std::string const &name,
                            hid_t const &location_id);

  //! Set attribute of type \e string
  bool h5setAttribute_string( hid_t const &obj_id,
			      std::string attrname,
                              std::string * data,
			      int size );
  //! Set attribute of type \e double
  bool h5setAttribute_double( hid_t const &obj_id,
			      std::string attrname,
                              double * data,
			      int32_t size );
  //! Set attribute of type \e int
  bool h5setAttribute_int( hid_t const &obj_id,
			   std::string attrname,
                           int * data,
			   int32_t size );
  //! Set attribute of type \e uint
  bool h5setAttribute_uint( hid_t const &obj_id,
			    std::string attrname,
                            uint * data,
			    int32_t size );
  //! Set attribute of type \e float
  bool h5setAttribute_float( hid_t const &obj_id, std::string attrname,
                             float * data, int32_t size );


  // ------------------------------------------------------------- h5setAttribute

  template <typename T>
  bool h5setAttribute( hid_t const &datatype,
		       hid_t obj_id,
		       std::string attrname,
                       T * data,
		       int size )
  {
    hid_t   attr1    = 0;  /* Attribute identifier */
    hid_t   aid1     = 0;  /* Attribute dataspace identifier */
    hsize_t dims[1]  = { size };

    aid1  = H5Screate_simple( 1, dims, NULL );
    if ( aid1 < 0 ) {
      std::cerr << "ERROR: Could not set attribute '" << attrname
		<< "' dataspace.\n";
      return DAL::FAIL;
    }
    
    attr1 = H5Acreate( obj_id, attrname.c_str(),
		       datatype, aid1, NULL, NULL );
    if ( attr1 < 0 ) {
      std::cerr << "ERROR: Could not create attribute '" << attrname
		<< "'.\n";
      return DAL::FAIL;
    }
    
    if ( H5Awrite(attr1, datatype, data) < 0 ) {
      std::cerr << "ERROR: Could not write attribute '" << attrname << "'.\n";
      return DAL::FAIL;
    }
    
    if ( H5Aclose( attr1 ) < 0 ) {
      std::cerr << "ERROR: Could not close attribute '" << attrname << "'.\n";
      return DAL::FAIL;
    }
    
    return DAL::SUCCESS;
    
  }
  
  // ------------------------------------------------------------ h5set_attribute

  //! Set the value of an attribute attached to a group or dataset
  template <typename T>
    bool h5set_attribute( hid_t const &datatype,
			  hid_t obj_id,
			  std::string name,
			  std::vector<T> const &value )
  {
    T *data;
    int nelem = value.size();

    data = new T [nelem];

    for (int n(0); n<nelem; n++) {
      data[n] = value[n];
    }

    return h5setAttribute (datatype,
			   obj_id,
			   name,
			   data,
			   nelem);

    delete [] data;
  }
    
  // ------------------------------------------------------------- h5getAttribute
  
  //! Get an HDF5 attribute - std::string type specialization
  inline bool h5getAttribute( hid_t const &obj_id,
			      std::string attrname,
                              std::string &value )
    {
      hsize_t dims;
      H5T_class_t type_class;
      size_t type_size;
      int rank = 0;
      
      // Check if attribute exists
      if ( H5Aexists( obj_id, attrname.c_str()) <= 0 )
	{
	  std::cerr << "ERROR: Attribute '" << attrname << "' does not exist.\n";
	  return DAL::FAIL;
	}
      
      std::string fullname = "/";
      
      if ( H5LTget_attribute_ndims( obj_id, fullname.c_str(), attrname.c_str(),
				    &rank ) < 0 )
	{
	  std::cerr << "ERROR: Attribute '" << attrname << "' does not exist.\n";
	  return DAL::FAIL;
	}
      
    if ( H5LTget_attribute_info( obj_id, fullname.c_str(), attrname.c_str(),
                                 &dims, &type_class, &type_size ) < 0 )
      {
        std::cerr << "ERROR: Could not get attribute '" << attrname
                  << "' info.\n";
        return DAL::FAIL;
      }

    if ( H5T_STRING == type_class )
      {
        char * data;
        data =  new char[256];
        if ( H5LTget_attribute_string( obj_id, fullname.c_str(),
                                       attrname.c_str(), data ) < 0 )
          {
            std::cerr << "ERROR: Could not get attribute '" << attrname << "'.\n";
            return DAL::FAIL;
          }

        value = stringify( data );

        return DAL::SUCCESS;
      }

    return DAL::FAIL;

  }

  //! Get the value of an attribute attached to a group or dataset
  template <typename T>
    bool h5get_attribute (hid_t const &attribute_id,
			  T &value);

  //! Get the value of an attribute attached to a group or dataset
  template <typename T>
    bool h5get_attribute (hid_t const &locationID,
			  std::string const &name,
			  T &value);

  //! Get the value of an attribute attached to a group or dataset
  template <typename T>
    bool h5get_attribute (hid_t const &attribute_id,
			  std::vector<T> &value)
    {
      bool status (true);
      std::vector<uint> shape;
      
      // get the shape of the dataspace
      status = h5get_dataspace_shape (shape,attribute_id);
      
      if (shape.size() == 1) {
	// adjust size of vector returning the result
	value.resize(shape[0]);
	// additional local variables
	herr_t h5error     = 0;
	hid_t datatype_id  = H5Aget_type (attribute_id);
	T *buffer;
	// allocate buffer memory
	buffer = new T [shape[0]];
	// read attribute value into buffer
	h5error = H5Aread (attribute_id,
			   datatype_id,
			   buffer);
	// copy retrieved data to returned vector
	if (h5error == 0) {
	  for (uint n(0); n<shape[0]; n++)
	    {
	      value[n] = buffer[n];
	    }
	  
	  delete [] buffer;
	  buffer = NULL;
	  
	}
	else {
	  cerr << "[h5get_attribute] Error reading value of attribute." << endl;
	  status = false;
	}
      }
      else {
	cerr << "[h5get_attribute] Wrong shape of attribute dataspace!"
	     << std::endl;
	status = false;
      }
      
      return status;
    }
  
  //! Get the value of an attribute attached to a group or dataset
  template <typename T>
    bool h5get_attribute (hid_t const &locationID,
			  std::string const &name,
			  std::vector<T> &value);  
  
  
#ifdef HAVE_CASA
  
  //! Get the shape of a dataset
  bool h5get_dataset_shape (hid_t const &attribute_id,
			    casa::IPosition &shape);
  //! Get the shape of the dataspace associated with the attribute
  bool h5get_dataspace_shape (hid_t const &attribute_id,
			      casa::IPosition &shape);
  //! Get the value of an attribute attached to a group or dataset
  template <typename T>
    bool h5get_attribute (hid_t const &attribute_id,
			  casa::Vector<T> &value);
  //! Get the value of an attribute attached to a group or dataset
  template <typename T>
    bool h5get_attribute (hid_t const &location_id,
			  std::string const &name,
			  casa::Vector<T> &value);
  //! Get physical quantity attribute as casa::Quantity
  Quantity h5get_quantity (hid_t const &location_id,
			   DAL::Attributes const &value,
			   DAL::Attributes const &unit);  
  //! Get a physical quantity describing a direction within a frame
  casa::MDirection h5get_direction (hid_t const &location_id,
				    DAL::Attributes const &value,
                                    DAL::Attributes const &unit,
                                    DAL::Attributes const &frame);  
  //! Get a physical quantity describing a position/location
  casa::MPosition h5get_position (hid_t const &location_id,
				  DAL::Attributes const &value,
				  DAL::Attributes const &unit,
				  DAL::Attributes const &frame);
  
#endif  // HAVE_CASA

  // ============================================================================
  //
  //  Access to Dataspaces and Datatypes
  //
  // ============================================================================



  // ============================================================================
  //
  //  Boost.Python wrappers
  //
  // ============================================================================

#ifdef PYTHON
  //! Convert Modified Julian Date (mjd) to unix time  
  bpl::numeric::array mjd2unix_boost( bpl::numeric::array mjd_time );
#endif

} // namespace DAL


// ==============================================================================
//
//  Service functions outside the DAL namespace
//
// ==============================================================================

/*!
  \brief Display the elements stored in an array
  
  \param arr   -- Pointer to the array with the data to be displayed
  \param nelem -- The number of elements stored within the array
*/
template <typename T>
void show (T const *arr,
	   uint const &nelem)
{
  show (std::cout,
	arr,
	nelem);
}

// ------------------------------------------------------------------------------

/*!
  \brief Write elements of an array to an output stream
  
  \param os    -- Stream to which the output is written
  \param arr   -- Pointer to the array with the data to be displayed
  \param nelem -- The number of elements stored within the array
*/
template <typename T> void show (std::ostream& os,
			      T const *arr,
			      uint const &nelem)
{
  os << "[";
  
  for (uint n(0); n<nelem; n++) {
    os << " " << arr[n];
  }
  
  os << " ]" << std::endl;
}

// ------------------------------------------------------------------------------

/*!
  \brief Overloading of output operator to display std::vector<T>
  
  \param os  -- Output stream to which the result will be written to
  \param vec -- The vector to be displayed
*/
template <typename T>
std::ostream& operator<< (std::ostream &os,
			  const std::vector<T> &vec)
{
  os << "[";
  
  for (uint n(0); n<vec.size(); n++) {
    os << " " << vec[n];
  }
  
  os << " ]";
  
  return os;
}

// ------------------------------------------------------------------------------

#endif // DALCOMMON_H
