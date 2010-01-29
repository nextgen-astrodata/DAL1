/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#ifndef HDF5DATASET_H
#define HDF5DATASET_H

#include <iostream>
#include <string>
#include <vector>

#include <dalCommon.h>

namespace DAL {
  
  /*!
    \class HDF5Dataset

    \ingroup DAL
    \ingroup data_common

    \brief A class to encapsulate the operations required to work with a dataset

    \author Lars B&auml;hren

    \date 2009/12/04
    
    \todo Implement read function to access data array within the dataset.
    \todo Implement Hyperslab to access multidimensional datasets/arrays.
  */
  class HDF5Dataset {
    
    //! Name of the dataset
    std::string name_p;
    //! Shape of the dataset
    std::vector<hsize_t> shape_p;
    //! Dataspace identifier
    hid_t dataspaceID_p;
    //! Datatype identifier
    hid_t datatypeID_p;
    //! Dataset identifier
    hid_t datasetID_p;
    
  public:
    
    // === Construction =========================================================

    //! Default constuctor
    HDF5Dataset ();
    
    //! Argumented constructor
    HDF5Dataset (hid_t const &location,
		 std::string const &name);
    
    //! Argumented constructor
    HDF5Dataset (hid_t const &location,
		 std::string const &name,
		 std::vector<hsize_t> const &shape,
		 hid_t const &datatype=H5T_NATIVE_DOUBLE);
    
    // === Destruction ==========================================================
    
    // Destructor
    ~HDF5Dataset ();
    
    // === Parameter access =====================================================
    
    //! Get the name of the dataset
    inline std::string name () const {
      return name_p;
    }
    
    //! Get the shape of the dataset
    inline std::vector<hsize_t> shape () const {
      return shape_p;
    }
    
    //! Get the rank (i.e. the number of axes) of the dataset
    inline int rank () const {
      return shape_p.size();
    }
    
    //! Get the dataspace identifier
    inline hid_t dataspaceID () const {
      return dataspaceID_p;
    }
    
    //! Get the datatype identifier
    inline hid_t datatypeID () const {
      return datatypeID_p;
    }
    
    // === Methods ==============================================================
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    
    //! Provide a summary of the internal status
    void summary (std::ostream &os);
    
    //! Open the dataset
    bool open (hid_t const &location,
	       std::string const &name,
	       std::vector<hsize_t> const &shape,
	       hid_t const &datatype=H5T_NATIVE_DOUBLE);
    
    /*!
      \brief Get the value of an attribute
      
      \param name -- Name of the attribute for which the value is about to be
             retrieved.
      \retval val -- The value of the attribute.
      \return status -- The status of the operation; returns <tt>false</tt> in
              case an error was encountered.
    */
    template <class T>
      inline bool getAttribute (std::string const &name,
				T &val)
      {
	bool status (true);
	
	if (datasetID_p > 0) {
	  status = h5get_attribute (datasetID_p,
				    name,
				    val);
	} else {
	  std::cerr << "[HDF5Dataset::getAttribute] Not connected to Dataset!"
		    << std::endl;
	  status = false;
	}

	return status;
      }

    /*!
      \brief Set the value of an attribute
      
      \param name -- Name of the attribute for which the value is about to be
             retrieved.
      \retval val -- The value of the attribute.
      \return status -- The status of the operation; returns <tt>false</tt> in
              case an error was encountered.
    */
    template <class T>
      inline bool setAttribute (std::string const &name,
				T const &val)
      {
	bool status (true);

	if (datasetID_p > 0) {
	  status = h5set_attribute (datasetID_p,
				    name,
				    val);
	} else {
	  std::cerr << "[HDF5Dataset::setAttribute] Not connected to Dataset!"
		    << std::endl;
	  status = false;
	}

	return status;
      }

  private:
    
    //! Initialize the internal parameters
    void init ();
    
  }; // end class HDF5Dataset
  
} // end namespace DAL

#endif
