/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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
  //  Parameter access
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                 setChunksize

  /*!
    \param shape     -- Shape of the dataset.
    \param chunksize -- Chunk size for extendible array.
  */
  bool HDF5Dataset::setShape (std::vector<hsize_t> const &shape,
			      std::vector<hsize_t> const &chunksize)
  {
    bool status    = true;
    size_t rank    = shape.size();

    // Store the shape of the dataset ______________________

    if (shape_p.empty() || shape_p.size() != rank) {
      shape_p.resize(rank);
    }

    shape_p = shape;

    // Store the chunksize of the dataset __________________

    /* Check dimensions of the internal array */
    if (chunking_p.empty() || chunking_p.size() != rank) {
      chunking_p.resize(rank);
    }

    /* Check dimensions of the input array */
    if (chunksize.empty() || chunksize.size() != rank) {
      chunking_p = shape_p;
    } else {
      chunking_p = chunksize;
    }
    
    adjustChunksize();
    
    return status;
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
    name_p         = "Dataset";
    location_p     = 0;
    dataspace_p    = 0;
    datatype_p     = 0;
    layout_p       = H5D_COMPACT;
    shape_p.clear();
    chunking_p.clear();
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
    
    /* Set up the list of attributes attached to the group */
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
	location_p = H5Dcreate2 (location,
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
    bool status (true);

    //____________________________________________
    // Update internal parameters

    name_p     = name;
    datatype_p = H5Tcopy (datatype);

    setShape (shape, chunksize);

    //____________________________________________
    // Create Dataspace

    int rank = shape.size();
    hsize_t dims [rank];
    hsize_t maxdims [rank];
    hsize_t chunkdims [rank];
    hid_t dcpl;
    herr_t h5error;

    for (int n(0); n<rank; ++n) {
      dims[n]      = shape_p[n];
      maxdims[n]   = H5S_UNLIMITED;
      chunkdims[n] = chunking_p[n];
    }

    dataspace_p = H5Screate_simple (rank, dims, maxdims);
    // Create the dataset creation property list
    dcpl  = H5Pcreate (H5P_DATASET_CREATE);
    // Set the chunk size
    h5error     = H5Pset_chunk (dcpl, rank, chunkdims);
    // Create the Dataset
    location_p  = H5Dcreate2 (location,
			      name_p.c_str(),
			      datatype_p,
			      dataspace_p,
			      H5P_DEFAULT,
			      dcpl,
			      H5P_DEFAULT);
    
    // Release HDF5 object identifiers
    H5Pclose (dcpl);
    
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

    /* Get the creation property list for the dataset */
    propertyID = H5Dget_create_plist (location_p);

    /* Return the layout of the raw data for a dataset. */
    if (H5Iis_valid(propertyID)) {
      layout_p = H5Pget_layout (propertyID);
    } else {
      std::cerr << "[HDF5Dataset::getChunksize]"
		<< " Failed to get creation property list for the dataset!"
		<< std::endl;
      status = false;
    }
    
    /* Retrieve the size of chunks for the raw data of a chunked layout dataset */
    if (layout_p == H5D_CHUNKED) {
      rank  = H5Pget_chunk (propertyID, nofAxes, chunksize);
    } else {
      rank = -1;
    }

    /* Process the result from the inspection of the dataset */
    if (rank<0) {
      chunking_p.clear();
    } else {
      if (nofAxes>0) {
	chunking_p.resize(nofAxes);
	for (int n(0); n<nofAxes; ++n) {
	  chunking_p[n] = chunksize[n];
	}
      }
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                              adjustChunksize
  
  /*!
    As explained in the source code documentation of the HDF5 library (see
    H5Dchunk.c), chunks cannot be larger than can be represented in 32-bits; in
    order to avoid running in error when assigning the chunking, we check the
    chunking before applying it to the extendable dataset.
   */
  bool HDF5Dataset::adjustChunksize ()
  {
    bool status         = true;
    size_t datatypeSize = H5Tget_size (datatype_p);
    size_t rank         = chunking_p.size();
    uint64_t chunkSize  = datatypeSize;

    /* Check the number of datapoints per chunk */
    for (size_t n(0); n<rank; ++n) {
      chunkSize *= (uint64_t)chunking_p[n];
    }

    /* Check for chunk larger than can be represented in 32-bits */
    if (chunkSize > (uint64_t)0xffffffff) {
      std::cerr << "[HDF5Dataset::adjustChunksize] Adjusting chunking from "
		<< chunking_p;
      for (size_t n(0); n<rank; ++n) {
	chunking_p[n] /= 2;
      }
      std::cerr << " -> " << chunking_p << std::endl;
      adjustChunksize ();
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
    \param slab          -- The HDF5Hyperslab, which will be applied to the
           dataset; internal book-keeping is done to track if multiple hyperslabs
	   have been applied to the dataset.
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool HDF5Dataset::setHyperslab (HDF5Hyperslab &slab,
				  bool const &resizeDataset)
  {
    bool status (true);

    if (H5Iis_valid(location_p) && H5Iis_valid(dataspace_p)) {
      
      /* Assign the Hyperslab selection */
      status = slab.setHyperslab (location_p,
				  dataspace_p,
				  resizeDataset);
#ifdef DEBUGGING_MESSAGES
      {
	std::vector<hsize_t> posStart;
	std::vector<hsize_t> posEnd;
	HDF5Hyperslab::getBoundingBox (dataspace_p, posStart, posEnd);
	std::cout << "-- Bounding box = " << posStart << " .. " << posEnd << std::endl;
      }
#endif
      
      /* Book-keeping: store the assigned hyperslab for later inspection. */
      
      switch (slab.selection()) {
      case H5S_SELECT_SET:
	hyperslab_p.empty();
	hyperslab_p.push_back(slab);
	break;
      default:
	hyperslab_p.push_back(slab);
	break;
      };
      
      /* As the dataset might have been resized, the "shape" parameter needs to be
	 updated to the current value.
      */
      DAL::h5get_dataspace_shape(location_p, shape_p);
      
    } else {
      std::cerr << "[HDF5Dataset::setHyperslab]"
		<< " Unable to select hyperslab - invalid HDF5 object!"
		<< std::endl;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param start     -- Offset of the starting element of the specified hyperslab
    \param block     -- The size of the block selected from the dataspace
    \param selection -- Selection operator to determine how the new selection is
           to be combined with the already existing selection for the dataspace.

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Dataset::setHyperslab (std::vector<int> const &start,
				  std::vector<int> const &block,
				  H5S_seloper_t const &selection)
  {
    /* Create Hyperslab object ...*/
    HDF5Hyperslab slab (start,block,selection);
    /* ... and apply it to the dataset */
    return setHyperslab (slab,true);
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
    /* Create Hyperslab object ...*/
    HDF5Hyperslab slab (start,stride,count,block,selection);
    /* ... and apply it to the dataset */
    return setHyperslab (slab,true);
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
    os << "[HDF5Dataset] Summary of internal parameters"       << std::endl;
    os << "-- Dataset name           = " << name_p             << std::endl;
    os << "-- Dataset ID             = " << location_p         << std::endl;
    os << "-- Dataspace ID           = " << dataspace_p        << std::endl;
    os << "-- Datatype ID            = " << datatype_p         << std::endl;
    os << "-- Dataset rank           = " << rank()             << std::endl;
    os << "-- Dataset shape          = " << shape_p            << std::endl;
    os << "-- Layout of the raw data = " << layout_p           << std::endl;
    os << "-- Chunk size             = " << chunking_p         << std::endl;
    os << "-- nof. datapoints        = " << nofDatapoints()    << std::endl;
    os << "-- nof. active hyperslabs = " << hyperslab_p.size() << std::endl;
  }
  
} // end namespace DAL
