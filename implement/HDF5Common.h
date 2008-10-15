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

#ifdef HAVE_CASA
#include <casa/Arrays/IPosition.h>
#include <casa/Arrays/Vector.h>
#include <casa/Quanta/Quantum.h>
#include <measures/Measures/MDirection.h>
#include <measures/Measures/MPosition.h>
#endif

#ifndef ENUMERATIONS_H
#include "Enumerations.h"
#endif

#ifndef DALCOMMON_H
#include <dalCommon.h>
#endif

#include <hdf5.h>

namespace DAL {  //  Namespace DAL -- begin

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
      <li>casacore Library
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
  //  Access to attributes
  //
  // ============================================================================
  
#ifdef HAVE_CASA
#endif
  
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


} // Namespace DAL -- end

#endif /* HDF5COMMON_H */

