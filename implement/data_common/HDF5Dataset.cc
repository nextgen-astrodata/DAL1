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

#include <HDF5Dataset.h>

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                  HDF5Dataset

  HDF5Dataset::HDF5Dataset ()
  {
    init ();
  }

  //_____________________________________________________________________________
  //                                                                  HDF5Dataset

  /*!
    \param location -- Identifier for the location to which the dataset is
           attached.
    \param name     -- Name of the dataset.
  */
  HDF5Dataset::HDF5Dataset (hid_t const &location,
			    std::string const &name)
  {
    init ();
    open (location,name,false);
  }
  
  //_____________________________________________________________________________
  //                                                                  HDF5Dataset

  /*!
    \param location -- Identifier for the location at which the dataset is about
           to be created.
    \param name     -- Name of the dataset.
    \param shape    -- Shape of the dataset.
    \param datatype -- Datatype for the elements within the Dataset
  */
  HDF5Dataset::HDF5Dataset (hid_t const &location,
			    std::string const &name,
			    std::vector<hsize_t> const &shape,
			    hid_t const &datatype)
  {
    // initialize internal parameters
    init ();
    // open the dataset
    open (location,
	  name,
	  shape,
	  datatype);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Dataset::~HDF5Dataset ()
  {
    H5Tclose (datatype_p);
    H5Sclose (dataspace_p);
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init

  void HDF5Dataset::init ()
  {
    name_p          = "Dataset";
    location_p      = 0;
    dataspace_p     = 0;
    datatype_p      = 0;
    nofHyperslabs_p = 0;
    shape_p.clear();
  }

  //_____________________________________________________________________________
  //                                                                setAttributes

  void HDF5Dataset::setAttributes ()
  {
    attributes_p.clear();
  }

  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool HDF5Dataset::open (hid_t const &location,
			  std::string const &name,
			  bool const &create)
  {
    bool status (true);
    
    /* Set up the list of attributes attached to the root group */
    setAttributes();
    
    if (H5Lexists (location, name.c_str(), H5P_DEFAULT)) {
      location_p = H5Dopen (location,
			    name.c_str(),
			    H5P_DEFAULT);
    } else {
      location_p = 0;
    }
    
    if (location_p > 0) {
      status = true;
    } else {
      /* If failed to open the group, check if we are supposed to create one */
      if (create && dataspace_p && datatype_p) {
	location_p = H5Dcreate (location,
				name_p.c_str(),
				datatype_p,
				dataspace_p,
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  /* write attribute attached to the dataset */
	} else {
	  std::cerr << "[HDF5Dataset::open] Failed to create dataset "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[HDF5Dataset::open] Failed to open dataset "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[HDF5Dataset::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier for the location at which the dataset is about
           to be created.
    \param name     -- Name of the dataset.
    \param shape    -- Shape of the dataset.
    \param datatype -- Datatype for the elements within the Dataset
  */
  bool HDF5Dataset::open (hid_t const &location,
			  std::string const &name,
			  std::vector<hsize_t> const &shape,
			  hid_t const &datatype)
  {
    bool status (true);
    
    // update internal parameters
    name_p = name;
    shape_p.resize(shape.size());
    shape_p = shape;
    
    // Create Dataspace
    int rank = shape_p.size();
    hsize_t dimensions [rank];
    for (int n(0); n<rank; ++n) {
      dimensions[n] = shape_p[n];
    }
    dataspace_p = H5Screate_simple (rank,dimensions,NULL);

    // Set the datatype for the elements within the Dataset
    datatype_p  = H5Tcopy (datatype);

    // Create the Dataset
    location_p = H5Dcreate (location,
			    name_p.c_str(),
			    datatype_p,
			    dataspace_p,
			    H5P_DEFAULT,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                 openEmbedded

  bool HDF5Dataset::openEmbedded (bool const &create)
  {
    bool status = create;
    return status;
  }

  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param start     -- Offset of the starting element of the specified hyperslab
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param selection -- Selection operator to determine how the new selection is
           to be combined with the already existing selection for the dataspace.

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Dataset::setHyperslab (std::vector<int> const &start,
				  std::vector<int> const &count,
				  H5S_seloper_t const &selection)
  {
    std::vector<int> stride;
    std::vector<int> block;
    
    return setHyperslab (start,stride,count,block,selection);
  }
  
  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param start     -- Offset of the starting element of the specified hyperslab
    \param stride    -- Number of elements to separate each element or block to
           be selected
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param block     -- The size of the block selected from the dataspace
    \param selection -- Selection operator to determine how the new selection is
           to be combined with the already existing selection for the dataspace.

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Dataset::setHyperslab (std::vector<int> const &start,
				  std::vector<int> const &stride,
				  std::vector<int> const &count,
				  std::vector<int> const &block,
				  H5S_seloper_t const &selection)
  {
    bool status (true);

    //____________________________________________
    // Check the input parameters

    /* Minimal requirement: start and count must be of the same size */
    if (start.size() != count.size()) {
      std::cerr << "[HDF5Dataset::setHyperslab] Parameter mismatch: start - count."
		<< std::endl;
      std::cerr << "-- start = " << start << std::endl;
      std::cerr << "-- count = " << count << std::endl;
      return false;
    }
    else {
      if (start.size() != shape_p.size()) {
      std::cerr << "[HDF5Dataset::setHyperslab] Parameter mismatch: start - shape."
		<< std::endl;
      std::cerr << "-- start = " << start   << std::endl;
      std::cerr << "-- shape = " << shape_p << std::endl;
      return false;
      }
    }

    //____________________________________________
    // Set up the hyperslab

    //____________________________________________
    // Book-keeping

    if (status) {
      ++nofHyperslabs_p;
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*
    \param os -- Output stream to which the summary is written.
  */
  void HDF5Dataset::summary (std::ostream &os)
  {
    os << "[HDF5Dataset] Summary of internal parameters" << std::endl;
    os << "-- Dataset name  = " << name_p      << std::endl;
    os << "-- Dataset ID    = " << location_p  << std::endl;
    os << "-- Dataspace ID  = " << dataspace_p << std::endl;
    os << "-- Datatype ID   = " << datatype_p  << std::endl;
    os << "-- Dataset shape = " << shape_p     << std::endl;
  }
  
} // end namespace DAL
