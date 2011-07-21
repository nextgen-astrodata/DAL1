/***************************************************************************
 *   Copyright (C) 2008-2011                                               *
 *   Joseph Masters (J.S.Masters@uva.nl)                                   *
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

#ifndef DALCOMMON_H
#define DALCOMMON_H

#include <iostream>
#include <stdint.h>
#include <sstream>
#include <assert.h>
#include <map>
#include <set>
#include <stdexcept>
#include <typeinfo>
#include <valarray>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;

#include <core/dalConversions.h>
#include <core/HDF5Attribute.h>
#include <core/HDF5Dataspace.h>

#ifdef DAL_WITH_CASA
#include <casa/Arrays/IPosition.h>
#include <casa/Arrays/Vector.h>
#include <casa/Quanta/Quantum.h>
#include <casa/OS/File.h>
#include <casa/OS/Path.h>
#include <casa/OS/SymLink.h>
#include <images/Images/ImageInterface.h>
#include <measures/Measures/MDirection.h>
#include <measures/Measures/MPosition.h>
#endif

/*!
  \file dalCommon.h

  \ingroup DAL
  \ingroup core

  \brief A collection of commonly used routines (for the Data Access Library)

  \author Joseph Masters
  \author Lars B&auml;hren

  \test tdalCommon.cc
  \test tdalCommon_operators.cc

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
    <li>CFITSIO library
  </ul>

  <h3>Synopsis</h3>

  A number of different things are included into this collection of commonly
  used routines:
  - Conversion routines
    - DAL::julday
  - Service functions
    - DAL::it_exists
    - DAL::BigEndian
  - Routines for the access of HDF5 attributes
    - DAL::h5attribute_summary

*/

namespace DAL {

  // ============================================================================
  //
  //  Conversion/Computation routines
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                BadConversion
  
  class BadConversion : public std::runtime_error {
  public:
  BadConversion(const std::string& s)
    : std::runtime_error(s)
      { }
  };
  
  //_____________________________________________________________________________
  //                                                                    stringify
  
  //! Convert array of elements to string
  template<typename T>
    inline std::string stringify(const T& x)
    {
      std::ostringstream o;
      if (!(o << x))
	throw BadConversion(std::string("stringify(")
			    + typeid(x).name() + ")");
      return o.str();
    }
  
  //_____________________________________________________________________________
  //                                                                    swapbytes
  
  //! Byte swap routine
  void swapbytes (char *addr,
		  int8_t nbytes);
  
  //_____________________________________________________________________________
  //                                                                        crc16

  //! Calculate a 16-bit CRC
  uint16_t crc16 (uint16_t * buffer,
		  uint32_t length);
  
  // ============================================================================
  //
  //  System inspection
  //
  // ============================================================================
  
  //! Test if the system is big endian
  bool BigEndian ( void );
  
  // ============================================================================
  //
  //  Array operations
  //
  // ============================================================================

  //! Get product of elements within the array
  template <class T>
    T product (const std::vector<T> &vec)
    {
      if (vec.empty()) {
	return T(0);
      } else {
	T tmp (1);
	for (size_t n(0); n<vec.size(); ++n) {
	  tmp *= vec[n];
	}
	return tmp;
      }
    }
  
  /*!
    \brief Get sum of elements within the array
    \param vec  -- Input vector/array of which the sum of the elements is
           computed.
    \return sum -- Sum of the elements in the input \e vec
  */
  template <class T>
    T sum (const std::vector<T> &vec)
    {
      T nelem (0);
      
      if (vec.empty()) {
	return nelem;
      } else {
	for (size_t n(0); n<vec.size(); ++n) {
	  nelem += vec[n];
	}
      }
      
      return nelem;
    }
  
  //_____________________________________________________________________________
  //                                                               IdentityMatrix
  
  /*!
    \brief Set up vector representing an identity matrix of sidelength \f$ N \f$
    \retval data      -- \f$ N \times N \f$ matrix represented as a linear array,
            following C-type ordering of the matrix elements.
    \param sidelength -- Sidelength of the matrix, \f$ N \f$.
           \f[ I_{1} = \left[ 1 \right] , \quad
               I_{2} = \left[ \begin{array}{cc} 1 & 0 \\ 0 & 1 \end{array} \right] \f]
  */
  template <class T> void IdentityMatrix (std::vector<T> &data,
					  unsigned int const &sidelength)
    {
      unsigned int nelem = sidelength*sidelength;
      // Adjust the size of the vector storing the matrix
      data.resize(nelem);
      // Populate the vector according to the matrix layout
      for (unsigned int n(0); n<sidelength; ++n) {
	data[n*sidelength+n] = T(1);
      }
    }
  
  //_____________________________________________________________________________
  //                                                         AllocateDynamicArray
  
  /*!
    \brief Allocate the memory for a dynamic 2-dimensional array
    \param shape -- Shape of the array, i.e. the number of elements along the axes.
    \return arr  -- Pointer to the allocated memory space
  */
  template <typename T> 
    T * AllocateDynamicArray (std::vector<unsigned int> const &shape)
    {
      unsigned int rank  = shape.size();
      unsigned int nelem = 1;

      for (unsigned int n(0); n<rank; ++n) {
	nelem *= shape[n];
      }

      T * arr = new T [nelem];

      return arr;
    }
  
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

  //! Verify a given ID is a valid id so it can be passed into an H5I C function.
  inline bool h5is_validID (hid_t const &object_id) {
    H5I_type_t id_type = H5Iget_type(object_id);
    if (id_type <= H5I_BADID || id_type >= H5I_NTYPES)
      return false;
    else
      return true;
  }
  
  //! Retrieve the name of an object based on the object identifier
  bool h5get_name (std::string &name,
                   hid_t const &object_id,
		   bool const &absolute=true);

  //! Retrieve the name of an object based on the object identifier
  bool h5get_name (std::string &name,
                   hid_t const &object_id,
                   hsize_t const &index);

  //! Retrieve the name of an object based on the object identifier
  bool h5get_filename (std::string &filename,
                       hid_t const &object_id);
  
  //! Retrieve list of objects of a certain type attached to a group
  bool h5get_names (std::vector<std::string> &names,
		    hid_t const &location_id,
		    int const &type=H5G_GROUP);
  
  //! Retrieve list of objects of a certain type attached to a group
  bool h5get_names (std::set<std::string> &names,
		    hid_t const &location_id,
		    int const &type=H5G_GROUP);

  // ============================================================================
  //
  //  Access to HDF5 attributes
  //
  // ============================================================================
  
  //! Print an attribute value. (Used with H5Aiterate).
  herr_t attr_info (hid_t loc_id,
                    const char *name,
                    void *opdata);

  //_____________________________________________________________________________
  // Passing of attribute values using casacore array classes
  
#ifdef DAL_WITH_CASA
  
  //! Get the shape of a dataset
  bool h5get_dataset_shape (hid_t const &attribute_id,
                            casa::IPosition &shape,
                            bool const &maxdims=false);
  //! Get physical quantity attribute as casa::Quantity
  casa::Quantity h5get_quantity (hid_t const &location_id,
				 std::string const &value,
				 std::string const &unit);
  //! Get a physical quantity describing a direction within a frame
  casa::MDirection h5get_direction (hid_t const &location_id,
                                    std::string const &value,
                                    std::string const &unit,
                                    std::string const &frame);
  //! Get a physical quantity describing a position/location
  casa::MPosition h5get_position (hid_t const &location_id,
                                  std::string const &value,
                                  std::string const &unit,
                                  std::string const &frame);
  /*!
    \brief Provide a summary of the properties of a casa::ImageInterface object
    
    Specific types of images (PagedImage, HDF5Image) within casacore are derived
    from a base class (ImageInterface) defining a common interface.
    
    \param image -- Image object derived from the ImageInterface class.
  */
  template <class T>
    void summary (casa::ImageInterface<T> &image)
    {
      casa::CoordinateSystem cs = image.coordinates();
      casa::IPosition shape (image.shape());
      int nofAxes (shape.nelements());
      double nofPixels (1.0);
      
      for (int n(0); n<nofAxes; n++) {
	nofPixels *= shape(n);
      }
      
      cout << "-- Image type ............. : " << image.imageType()        << endl;
      cout << "-- Table name ............. : " << image.name()             << endl;
      cout << "-- Image shape ............ : " << shape                    << endl;
      cout << "-- Number of pixels ....... : " << nofPixels                << endl;
      cout << "-- World axis names ....... : " << cs.worldAxisNames()      << endl;
      cout << "-- World axis units ....... : " << cs.worldAxisUnits()      << endl;
      cout << "-- Referemce pixel ........ : " << cs.referencePixel()      << endl;
      cout << "-- Reference value ........ : " << cs.referenceValue()      << endl;
      cout << "-- Increment .............. : " << cs.increment()           << endl;
      cout << "-- Maximum cache size ..... : " << image.maximumCacheSize() << endl;
      cout << "-- Is the image paged? .... : " << image.isPaged()          << endl;
      cout << "-- Is the image persistent? : " << image.isPersistent()     << endl;
      cout << "-- Is the image writable?   : " << image.isWritable()       << endl;
      cout << "-- Has the image a mask?    : " << image.hasPixelMask()     << endl;
    }
  
#endif  // DAL_WITH_CASA
    
} // namespace DAL


// ==============================================================================
//
//  Service functions outside the DAL namespace
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                           show

/*!
  \brief Write elements of an array/vector/set to an output stream
  
  \param os    -- Stream to which the output is written.
  \param start -- Iterator pointing to the first element.
  \param end   -- Iterator pointing to the position after the last element.
*/
template <typename T>
std::ostream& show (std::ostream& os,
		    T start,
		    T end)
{
  T it;
  
  os << "[";
  
  for (it=start; it!=end; ++it) {
    os << " " << *it;
  }
  
  os << " ]";
  
  return os;
}

//_______________________________________________________________________________
//                                                                           show

/*!
  \brief Write elements of an array to an output stream
  
  \param os    -- Stream to which the output is written
  \param arr   -- Pointer to the array with the data to be displayed
  \param nelem -- The number of elements stored within the array
*/
template <typename T, typename S>
  void show (std::ostream& os,
	     T *arr,
	     S const &nelem)
{
  os << "[";
  
  for (S n(0); n<nelem; ++n) {
    os << " " << arr[n];
  }
  
  os << " ]";
}

//_______________________________________________________________________________
//                                                                           show

/*!
  \brief Display the elements stored in an array
  
  \param arr   -- Pointer to the array with the data to be displayed
  \param nelem -- The number of elements stored within the array
*/
template <typename T, typename S>
  std::ostream& show (T *arr,
		      S const &nelem)
{
  typedef typename std::vector<T>::iterator it;
  it dataStart = static_cast<it> (arr);
  
  return show (std::cout,
	       dataStart,
	       dataStart+nelem);
}

//_______________________________________________________________________________
//                                                                     operator<<

/*!
  \brief Overloading of output operator to display std::vector<T>
  
  \param os  -- Output stream to which the result will be written to
  \param vec -- The vector to be displayed
*/
template <typename T>
std::ostream& operator<< (std::ostream &os,
                          const std::vector<T> &vec)
{
  return show (os, vec.begin(), vec.end());
}

//_______________________________________________________________________________
//                                                                     operator<<

/*!
  \brief Overloading of output operator to display std::valarray<T>
  
  \param os  -- Output stream to which the result will be written to
  \param vec -- The valarray to be displayed
*/
template <typename T>
std::ostream& operator<< (std::ostream &os,
                          const std::valarray<T> &vec)
{
  size_t nelem = vec.size();
  
  os << "[";
  
  for (size_t n(0); n<nelem; ++n) {
    os << " " << vec[n];
  }
  
  os << " ]";

  return os;
}

//_______________________________________________________________________________
//                                                                     operator<<

/*!
  \brief Overloading of output operator to display std::set<T>
  
  \param os -- Output stream to which the result will be written to
  \param s  -- The set to be displayed
*/
template <typename T>
std::ostream& operator<< (std::ostream &os,
                          const std::set<T> &s)
{
  return show (os, s.begin(), s.end());
}

//_______________________________________________________________________________
//                                                                     operator<<

/*!
  \brief Overloading of output operator to display std::map<K,V>
  
  \param os -- Output stream to which the result will be written to
  \param m  -- The map to be displayed
*/
template <typename K, typename V>
  std::ostream& operator<< (std::ostream &os,
			    const std::map<K,V> &m)
{
  typename std::map<K,V>::iterator it;
  
  os << "[";
  
  for (it=m.begin(); it!=m.end(); ++it) {
    os << " (" << it->first << "," << it->second << ")";
  }
  
  os << " ]";
  
  return os;
}

// ------------------------------------------------------------------------------

#endif // DALCOMMON_H
