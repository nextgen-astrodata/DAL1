/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#ifndef HDF5COMMON_H
#define HDF5COMMON_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

#ifdef WITH_CASA
#include <casa/Arrays/IPosition.h>
#include <casa/Arrays/Vector.h>
#include <casa/Quanta/Quantum.h>
#include <measures/Measures/MDirection.h>
#include <measures/Measures/MPosition.h>
#endif

#ifndef ENUMERATIONS_H
#include "Enumerations.h"
#endif

#include <hdf5.h>

namespace DAL   // Namespace DAL -- begin
  {

  /*!
    \file HDF5Common.h

    \ingroup DAL

    \brief A collection of routines to work with HDF5 datasets

    \author Lars B&auml;hren

    \date 2008/01/17

    \test tHDF5Common.cpp

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
    </ul>

    <h3>Synopsis</h3>

    <h3>Example(s)</h3>

    <h3>Missing features</h3>

    <ol>
      <li>We need to account for the fact, that certain data - most likely
      attributes though - can show up at different levels within the
      file-structure; it might be desirable to directly support vertical search
      for such attributes (at least upwards).
      <li>When using the CASA array classes (Array, Vector, Matrix, Cube), we
      need to be able to properly read and write their contents. Most likely
      this can be done in a straight-forward manner wrapping some of the
      underlying functions of the HDF5 library.
    </ol>

  */

  // ============================================================================
  //
  //  Inspection and Feedback
  //
  // ============================================================================

  /*!
    \brief Write vector to an output stream

    \param os  -- Stream to which the output is written
    \param vec -- Vector to be written to the designated output stream

    \return os
  */
  template<class T>
  void show_vector (std::ostream& os,
                    std::vector<T> &vec);

  /*!
    \brief Provide a summary of an attribute's internal structure

    \param os           -- Output stream to which the summary is written
    \param attribute_id --
  */
  void h5attribute_summary (std::ostream &os,
                            hid_t const &attribute_id);

  /*!
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

  //@{
  /*!
    \brief Get the shape of a dataset

    \reval shape -- The shape of the dataset i.e.the length of the array axes in
                    case of multidimensional data
    \param dataset_id -- Identifier of the dataset within the HDF5 file

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_dataset_shape (std::vector<uint> &shape,
                            hid_t const &dataset_id);
#ifdef WITH_CASA
  bool h5get_dataset_shape (casa::IPosition &shape,
                            hid_t const &attribute_id);
#endif
  //@}

  //@{
  /*!
    \brief Get the shape of the dataspace associated with the attribute

    \reval shape -- The shape of the dataspace attached to the attribute, i.e.
           the length of the array axes in case of multidimensional data
    \param attribute_id -- Identifier of the attribute within the HDF5 file

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_dataspace_shape (std::vector<uint> &shape,
                              hid_t const &attribute_id);
#ifdef WITH_CASA
  bool h5get_dataspace_shape (casa::IPosition &shape,
                              hid_t const &attribute_id);
#endif
  //@}

  /*!
    \brief Retrieve the name of an object based on the object identifier

    \retval name     -- Name of the object (if set or defined).
    \param object_id -- Identifier for the objects of which to retrieve the
           name.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_name (std::string &name,
                   hid_t const &object_id);

  /*!
    \brief Retrieve the name of an object based on the object identifier

    \retval name     -- Name of the object (if set or defined).
    \param object_id -- Identifier for the objects of which to retrieve the
           name.
    \param index     -- Transient index identifying the object whose name to
           retrieve.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_name (std::string &name,
                   hid_t const &object_id,
                   hsize_t const &index);

  /*!
    \brief Retrieve the name of an object based on the object identifier

    \retval filename -- Name of the HDF5 file within which the object is
            contained
    \param object_id -- Identifier for the objects of which to retrieve the
           name.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_filename (std::string &filename,
                       hid_t const &object_id);

  // ============================================================================
  //
  //  Access to attributes
  //
  // ============================================================================

  //@{
  /*!
    \brief Get the value of an attribute attached to a group or dataset

    \retval value       -- Value of the attribute
    \param attribute_id -- Identifier of the attribute within the HDF5 file

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  template <class T>
  bool h5get_attribute (T &value,
                        hid_t const &attribute_id);
  template <class T>
  bool h5get_attribute (std::vector<T> &value,
                        hid_t const &attribute_id);
#ifdef WITH_CASA
  /*   template <class T> */
  /*     bool h5get_attribute (casa::Array<T> &value, */
  /* 			  hid_t const &attribute_id); */
  /*   template <class T> */
  /*     bool h5get_attribute (casa::Matrix<T> &value, */
  /* 			  hid_t const &attribute_id); */
  template <class T>
  bool h5get_attribute (casa::Vector<T> &value,
                        hid_t const &attribute_id);
#endif
  //@}

  /*!
    \brief Set the value of an attribute attached to a group or dataset

    \param location_id -- Identifier for the HDF5 object - file, group, dataset,
           array - the attribute is attached to.
    \param name        -- Name of the attribute
    \param val         -- Value stored as attribute

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  template <class T>
  bool h5set_attribute (hid_t const &location_id,
                        std::string const &name,
                        T const &val);
  bool h5set_attribute (hid_t const &location_id,
                        std::string const &name,
                        uint const &val);


  //@{
  /*!
    \brief Get the value of an attribute attached to a group or dataset

    \retval value      -- Value of the attribute
    \param name        -- Name of the attribute
    \param location_id -- Identifier for the HDF5 object - file, group, dataset,
           array - the attribute is attached to.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  template <class T>
  bool h5get_attribute (T &value,
                        std::string const &name,
                        hid_t const &location_id);
  template <class T>
  bool h5get_attribute (std::vector<T> &value,
                        std::string const &name,
                        hid_t const &location_id);
#ifdef WITH_CASA
  /*   template <class T> */
  /*     bool h5get_attribute (casa::Array<T> &value, */
  /* 			  std::string const &name, */
  /* 			  hid_t const &location_id); */
  /*   template <class T> */
  /*     bool h5get_attribute (casa::Matrix<T> &value, */
  /* 			  std::string const &name, */
  /* 			  hid_t const &location_id); */
  template <class T>
  bool h5get_attribute (casa::Vector<T> &value,
                        std::string const &name,
                        hid_t const &location_id);
#endif

  /*!
    \brief Print an attribute value. (Used with H5Aiterate).

    This is a function parameter meant to be used with H5Aiterate.

    \param loc_id -- Identifier for the HDF5 object - file, group, dataset,
           array - the attribute is attached to.
    \param name   -- Name of the attribute
    \param opdata -- not used, but necessary as a placeholder
    \return error status
  */
  herr_t attr_info(hid_t loc_id, const char *name, void *opdata);

  //@}

#ifdef WITH_CASA
  /*!
    \brief Get physical quantity attribute as casa::Quantity

    \param value -- Identifier for the attribute storing the numerical value of
           the quantity.
    \param unit  -- Identifier for the attribute storing the physical unit of
           the quantity
    \param location_id -- Identifier of the structure within the file, to which
           the attribut is attached to.

    \return quantity -- The physical quantity.
  */
  casa::Quantity h5get_quantity (DAL::Attributes const &value,
                                 DAL::Attributes const &unit,
                                 hid_t const &location_id);

  /*!
    \brief Get a physical quantity describing a direction within a frame

    \param value -- Identifier for the attribute storing the numerical value of
           the quantity.
    \param unit  -- Identifier for the attribute storing the physical unit of
           the quantity
    \param frame -- Identifier for the attribute storing the identifier for the
           reference frame within which the physical quantity is defined.
    \param location_id -- Identifier of the structure within the file, to which
           the attribut is attached to.

    \return direction -- The physical quantity.
  */
  casa::MDirection h5get_direction (DAL::Attributes const &value,
                                    DAL::Attributes const &unit,
                                    DAL::Attributes const &frame,
                                    hid_t const &location_id);

  /*!
    \brief Get a physical quantity describing a position/location

    \param value -- Identifier for the attribute storing the numerical value of
           the quantity.
    \param unit  -- Identifier for the attribute storing the physical unit of
           the quantity
    \param frame -- Identifier for the attribute storing the identifier for the
           reference frame within which the physical quantity is defined.
    \param location_id -- Identifier of the structure within the file, to which
           the attribut is attached to.

    \return position -- The physical quantity.
  */
  casa::MPosition h5get_position (DAL::Attributes const &value,
                                  DAL::Attributes const &unit,
                                  DAL::Attributes const &frame,
                                  hid_t const &location_id);
#endif

  // ============================================================================
  //
  //  Dataspaces and Datatypes
  //
  // ============================================================================

} // Namespace DAL -- end

#endif /* HDF5COMMON_H */

