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
    \param location  -- Identifier for the location at which the dataset is about
           to be created.
    \param name      -- Name of the dataset.
    \param shape     -- Shape of the dataset.
    \param datatype  -- Datatype for the elements within the Dataset
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
  
  //_____________________________________________________________________________
  //                                                                  HDF5Dataset

  /*!
    \param location  -- Identifier for the location at which the dataset is about
           to be created.
    \param name      -- Name of the dataset.
    \param shape     -- Shape of the dataset.
    \param chunksize -- Chunk size for extendible array
    \param datatype  -- Datatype for the elements within the Dataset
  */
  HDF5Dataset::HDF5Dataset (hid_t const &location,
			    std::string const &name,
			    std::vector<hsize_t> const &shape,
			    std::vector<hsize_t> const &chunksize,
			    hid_t const &datatype)
  {
    // initialize internal parameters
    init ();
    // open the dataset
    open (location,
	  name,
	  shape,
	  chunksize,
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
    shape_p.clear();
    chunksize_p.clear();
    hyperslab_p.clear();
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
      // Open the dataset
      location_p = H5Dopen (location,
			    name.c_str(),
			    H5P_DEFAULT);
      // Assign internal parameters
      name_p      = name;
      dataspace_p = H5Dget_space (location_p);
      datatype_p  = H5Dget_type (location_p);
      // Get the shape of the dataset array
      DAL::h5get_dataspace_shape(location_p, shape_p);
      // Retrieve the size of chunks for the raw data
      status = getChunksize ();
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
    std::vector<hsize_t> chunksize;
    
    return open (location,
		 name,
		 shape,
		 chunksize,
		 datatype);
  }
    
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location  -- Identifier for the location at which the dataset is about
           to be created.
    \param name      -- Name of the dataset.
    \param shape     -- Shape of the dataset.
    \param chunksize -- Chunk size for extendible array.
    \param datatype  -- Datatype for the elements within the Dataset
  */
  bool HDF5Dataset::open (hid_t const &location,
			  std::string const &name,
			  std::vector<hsize_t> const &shape,
			  std::vector<hsize_t> const &chunksize,
			  hid_t const &datatype)
  {
    bool status = true;
    int rank    = shape.size();

    //____________________________________________
    // Update internal parameters

    shape_p.resize(rank);
    chunksize_p.resize(rank);

    name_p  = name;
    shape_p = shape;

    if (chunksize.empty() || chunksize.size() != shape.size()) {
      chunksize_p = shape_p;
    } else {
      chunksize_p = chunksize;
    }

    //____________________________________________
    // Create Dataspace

    hsize_t dims [rank];
    hsize_t maxdims [rank];
    hsize_t chunkdims [rank];
    hid_t chunkParam;
    herr_t h5error;

    for (int n(0); n<rank; ++n) {
      dims[n]      = shape_p[n];
      maxdims[n]   = H5S_UNLIMITED;
      chunkdims[n] = chunksize_p[n];
    }

    dataspace_p = H5Screate_simple (rank, dims, maxdims);
    // Create the dataset creation property list
    chunkParam  = H5Pcreate (H5P_DATASET_CREATE);
    // Set the chunk size
    h5error     = H5Pset_chunk (chunkParam, rank, chunkdims);
    // Set the datatype for the elements within the Dataset
    datatype_p  = H5Tcopy (datatype);
    // Create the Dataset
    location_p  = H5Dcreate (location,
			     name_p.c_str(),
			     datatype_p,
			     dataspace_p,
			     H5P_DEFAULT,
			     chunkParam,
			     H5P_DEFAULT);
    
    // Release HDF5 object identifiers
    H5Pclose (chunkParam);
    
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
  //                                                                 getChunksize

  bool HDF5Dataset::getChunksize ()
  {
    bool status (true);
    hid_t propertyID (0);
    int rank (shape_p.size());
    int nofAxes (rank);
    hsize_t chunksize[rank];

    // Get the creation property list for the dataset
    propertyID = H5Dget_create_plist (location_p);

    // Retrieve the size of chunks for the raw data of a chunked layout dataset
    if (H5Iis_valid(propertyID)) {
      rank  = H5Pget_chunk (propertyID, nofAxes, chunksize);
    } else {
      std::cerr << "[HDF5Dataset::getChunksize]"
		<< " Failed to get creation property list for the dataset!"
		<< std::endl;
      status = false;
    }

    if (rank>0 && nofAxes>0) {
      chunksize_p.resize(nofAxes);
      for (int n(0); n<nofAxes; ++n) {
	chunksize_p[n] = chunksize[n];
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                nofDatapoints
  
  unsigned int HDF5Dataset::nofDatapoints ()
  {
    unsigned int nofPoints (1);

    for (unsigned int n(0); n<shape_p.size(); ++n) {
      nofPoints *= shape_p[n];
    }

    return nofPoints;
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
    return HDF5Hyperslab::setHyperslab (location_p,
					start,
					count,
					selection);
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
    return HDF5Hyperslab::setHyperslab (location_p,
					start,
					stride,
					count,
					block,
					selection);
  }

  //_____________________________________________________________________________
  //                                                                     readData
  
  /// @cond TEMPLATE_SPECIALIZATIONS
  
  template <> bool HDF5Dataset::readData (int data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_INT);
  }
  
  template <> bool HDF5Dataset::readData (uint data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_UINT);
  }
  
  template <> bool HDF5Dataset::readData (short data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_SHORT);
  }
  
  template <> bool HDF5Dataset::readData (long data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_LONG);
  }
  
  template <> bool HDF5Dataset::readData (float data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_FLOAT);
  }
  
  template <> bool HDF5Dataset::readData (double data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_DOUBLE);
  }
  
  /// @endcond
  
  //_____________________________________________________________________________
  //                                                                    writeData
  
  /// @cond TEMPLATE_SPECIALIZATIONS
  
  template <> bool HDF5Dataset::writeData (int const data[],
					  HDF5Hyperslab &slab)
  {
    return writeData (data, slab, H5T_NATIVE_INT);
  }
  
  template <> bool HDF5Dataset::writeData (uint const data[],
					  HDF5Hyperslab &slab)
  {
    return writeData (data, slab, H5T_NATIVE_UINT);
  }
  
  template <> bool HDF5Dataset::writeData (short const data[],
					  HDF5Hyperslab &slab)
  {
    return writeData (data, slab, H5T_NATIVE_SHORT);
  }
  
  template <> bool HDF5Dataset::writeData (long const data[],
					  HDF5Hyperslab &slab)
  {
    return writeData (data, slab, H5T_NATIVE_LONG);
  }
  
  template <> bool HDF5Dataset::writeData (float const data[],
					  HDF5Hyperslab &slab)
  {
    return writeData (data, slab, H5T_NATIVE_FLOAT);
  }
  
  template <> bool HDF5Dataset::writeData (double const data[],
					  HDF5Hyperslab &slab)
  {
    return writeData (data, slab, H5T_NATIVE_DOUBLE);
  }
  
  /// @endcond
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*
    \param os -- Output stream to which the summary is written.
  */
  void HDF5Dataset::summary (std::ostream &os)
  {
    os << "[HDF5Dataset] Summary of internal parameters" << std::endl;
    os << "-- Dataset name    = " << name_p          << std::endl;
    os << "-- Dataset ID      = " << location_p      << std::endl;
    os << "-- Dataspace ID    = " << dataspace_p     << std::endl;
    os << "-- Datatype ID     = " << datatype_p      << std::endl;
    os << "-- Dataset rank    = " << rank()          << std::endl;
    os << "-- Dataset shape   = " << shape_p         << std::endl;
    os << "-- Chunk size      = " << chunksize_p     << std::endl;
    os << "-- nof. datapoints = " << nofDatapoints() << std::endl;
  }
  
} // end namespace DAL
