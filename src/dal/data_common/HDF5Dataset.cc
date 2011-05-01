/***************************************************************************
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

#include "HDF5Dataset.h"

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                  HDF5Dataset

  HDF5Dataset::HDF5Dataset ()
    : HDF5Object ()
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                                  HDF5Dataset
  
  /*!
    \param location  -- Object identifier for the location below which the
           dataset \c name is to be found.
    \param name      -- Name of the dataset.
  */
  HDF5Dataset::HDF5Dataset (hid_t const &location,
			    std::string const &name)
  {
    init ();
    open (location, name);
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
    // Create the dataset
    create (location,
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
    // create the dataset
    create (location,
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
    itsShape.clear();
    itsChunking.clear();
    itsHyperslab.clear();

    HDF5Object::close (itsDatatype);
    HDF5Object::close (itsDataspace);
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
    bool status = true;
    size_t rank = shape.size();

    // Store the shape of the dataset ______________________

    if (shape.empty()) {
      std::cerr << "[HDF5Dataset::setShape] Provided shape parameter empty!"
		<< std::endl;
      return false;
    } else {
      if (itsShape.empty() || itsShape.size() != rank) {
	itsShape.resize(rank);
      }
      
      itsShape = shape;
    }
    
    // Store the chunksize of the dataset __________________
    
    /* Check dimensions of the internal array */
    if (itsChunking.empty() || itsChunking.size() != rank) {
      itsChunking.resize(rank);
    }

    /* Check dimensions of the input array */
    if (chunksize.empty() || chunksize.size() != rank) {
      itsChunking = itsShape;
    } else {
      itsChunking = chunksize;
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

  /*!
    Initialization of internal parameters to default values. HDF5 identifiers 
    (location, dataspace, datatype) are set to "-1", to later be able to probe,
    whether the ID points to a valid HDF5 object.
  */
  void HDF5Dataset::init ()
  {
    itsName        = "Dataset";
    itsLocation     = -1;
    itsDataspace   = -1;
    itsDatatype    = -1;
    itsLayout      = H5D_COMPACT;
    itsShape.clear();
    itsChunking.clear();
    itsHyperslab.clear();
  }

  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.

    \return status -- Status of the operation; returns \c false in case an error
            was encountered.
  */
  bool HDF5Dataset::open (hid_t const &location,
			  std::string const &name,
			  bool const &createNew)
  {
    bool status  = true;
    htri_t h5err = 0;
    
    /*________________________________________________________________
      Try opening existing dataset
    */

    if (H5Iis_valid(location)) {
      /* Check if link for dataset exists */
      h5err = H5Lexists (location,
			 name.c_str(),
			 H5P_DEFAULT);
      if (h5err>0) {
	/* Open existing dataset */
	itsLocation = H5Dopen (location,
			      name.c_str(),
			      H5P_DEFAULT);
	/* Check if opening of the dataset was successful */
	if (H5Iis_valid(itsLocation)) {
	  // Assign internal parameters
	  itsName      = name;
	  itsDataspace = H5Dget_space (itsLocation);
	  itsDatatype  = H5Dget_type (itsLocation);
	  // Get the shape of the dataset array
	  HDF5Dataspace::shape (itsLocation, itsShape);
	  // Retrieve the size of chunks for the raw data
	  status = getChunksize ();
	} else {
	  std::cerr << "[HDF5Dataset::open] Error opening dataset "
		    << name << std::endl;
	  status = false;
	}
      }   //   END -- H5Lexists(location,name)
      else {
	std::cerr << "[HDF5Dataset::open]"
		  << " Object " << name << " not found at provided location!"
		  << std::endl;
	return false;
      }
    } else {
      std::cerr << "[HDF5Dataset::open]"
		<< " Identifier does not point to valid object!"
		<< std::endl;
      return false;
    }

    //______________________________________________________
    // Try creating dataset, if not yet existing
    
    if (H5Iis_valid(itsLocation)) {
      status = true;
    } else {
      /* If failed to open the group, check if we are supposed to create one */
      if (createNew) {
	status = create (location,
			 itsName,
			 itsShape,
			 itsDatatype);
      } else {
	std::cerr << "[HDF5Dataset::open] Failed to open dataset "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                       create
  
  /*!
    \param location -- Identifier for the location at which the dataset is about
           to be created.
    \param name     -- Name of the dataset.
    \param shape    -- Shape of the dataset.
    \param datatype -- Datatype for the elements within the Dataset
    \return status  -- Status of the operation; returns \c false in case an error
            was encountered.
  */
  bool HDF5Dataset::create (hid_t const &location,
			    std::string const &name,
			    std::vector<hsize_t> const &shape,
			    hid_t const &datatype)
  {
    std::vector<hsize_t> chunksize;
    
    if (!itsChunking.empty()) {
      chunksize = itsChunking;
    }
    
    return create (location,
		   name,
		   shape,
		   chunksize,
		   datatype);
  }
    
  //_____________________________________________________________________________
  //                                                                       create
  
  /*!
    \param location  -- Identifier for the location at which the dataset is about
           to be created.
    \param name      -- Name of the dataset.
    \param shape     -- Shape of the dataset.
    \param chunksize -- Chunk size for extendible array.
    \param datatype  -- Datatype for the elements within the Dataset
    \return status   -- Status of the operation; returns \c false in case an
            error was encountered.
  */
  bool HDF5Dataset::create (hid_t const &location,
			    std::string const &name,
			    std::vector<hsize_t> const &shape,
			    std::vector<hsize_t> const &chunksize,
			    hid_t const &datatype)
  {
    bool status (true);
    
    //____________________________________________
    // Check input parameters
    
    if (shape.empty()) {
      std::cerr << "[HDF5Dataset::create] Undefined dataset shape!" << std::endl;
      return false;
    } else {
      itsName     = name;
      itsDatatype = H5Tcopy (datatype);
      
      setShape (shape, chunksize);
    }
    
    //____________________________________________
    // Create Dataspace
    
    int rank = shape.size();
    hsize_t dims [rank];
    hsize_t maxdims [rank];
    hsize_t chunkdims [rank];
    hid_t creationProperties;
    herr_t h5error;

    for (int n(0); n<rank; ++n) {
      dims[n]      = itsShape[n];
      maxdims[n]   = H5S_UNLIMITED;
      chunkdims[n] = itsChunking[n];
    }

    // Set up the dataspace
    itsDataspace = H5Screate_simple (rank, dims, maxdims);
    if (!H5Iis_valid(itsDataspace)) {
      std::cerr << "[HDF5Dataset::create] Failed to create dataspace!"
		  << std::endl;
      return false;
    }
    // Create the dataset creation property list
    creationProperties  = H5Pcreate (H5P_DATASET_CREATE);
    // Set the chunk size
    h5error     = H5Pset_chunk (creationProperties, rank, chunkdims);
    // Create the Dataset ...
    itsLocation  = H5Dcreate (location,
			      itsName.c_str(),
			      itsDatatype,
			      itsDataspace,
			      H5P_DEFAULT,
			      creationProperties,
			      H5P_DEFAULT);
    // ... and check it
    if (!H5Iis_valid(itsLocation)) {
      std::cerr << "[HDF5Dataset::create] Failed to create dataset!"
		  << std::endl;
      return false;
    }
    
    // Release HDF5 object identifiers
    if (H5Iis_valid(creationProperties)) {
      H5Pclose (creationProperties);
    } else {
      status = false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                 getChunksize

  /*!
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool HDF5Dataset::getChunksize ()
  {
    bool status      = true;
    hid_t propertyID = 0;
    int rank         = itsShape.size();
    int nofAxes      = rank;
    hsize_t chunksize[rank];

    /* Get the creation property list for the dataset */
    if (H5Iis_valid(itsLocation)) {
      propertyID = H5Dget_create_plist (itsLocation);
    } else {
      return false;
    }
    
    /* Return the layout of the raw data for a dataset. */
    if (H5Iis_valid(propertyID)) {
      itsLayout = H5Pget_layout (propertyID);
    } else {
      std::cerr << "[HDF5Dataset::getChunksize]"
		<< " Failed to get creation property list for the dataset!"
		<< std::endl;
      return false;
    }
    
    /* Retrieve the size of chunks for the raw data of a chunked layout dataset */
    if (itsLayout == H5D_CHUNKED) {
      rank  = H5Pget_chunk (propertyID, nofAxes, chunksize);
    } else {
      rank = -1;
    }

    /* Process the result from the inspection of the dataset */
    if (rank<0) {
      itsChunking.clear();
    } else {
      if (nofAxes>0) {
	itsChunking.resize(nofAxes);
	for (int n(0); n<nofAxes; ++n) {
	  itsChunking[n] = chunksize[n];
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
    size_t datatypeSize = H5Tget_size (itsDatatype);
    size_t rank         = itsChunking.size();
    uint64_t chunkSize  = datatypeSize;

    /* Check the number of datapoints per chunk */
    for (size_t n(0); n<rank; ++n) {
      chunkSize *= (uint64_t)itsChunking[n];
    }

    /* Check for chunk larger than can be represented in 32-bits */
    if (chunkSize > (uint64_t)0xffffffff) {
      for (size_t n(0); n<rank; ++n) {
	itsChunking[n] /= 2;
      }
      adjustChunksize ();
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                nofDatapoints
  
  unsigned int HDF5Dataset::nofDatapoints ()
  {
    unsigned int nofPoints (1);

    for (unsigned int n(0); n<itsShape.size(); ++n) {
      nofPoints *= itsShape[n];
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
    bool status = true;

    if (H5Iis_valid(itsLocation)) {
      if (H5Iis_valid(itsDataspace)) {
	
	/*____________________________________________________________
	  Assign the Hyperslab selection
	*/
	status = slab.setHyperslab (itsLocation,
				    itsDataspace,
				    resizeDataset);
	/* Book-keeping: store the assigned hyperslab for later inspection. */
	
	switch (slab.selection()) {
	case H5S_SELECT_SET:
	  itsHyperslab.empty();
	  itsHyperslab.push_back(slab);
	  break;
	default:
	  itsHyperslab.push_back(slab);
	  break;
	};
	
	/* As the dataset might have been resized, the "shape" parameter needs to be
	   updated to the current value.
	*/
	HDF5Dataspace::shape(itsLocation, itsShape);
      } else {
	std::cerr << "[HDF5Dataset::setHyperslab]"
		  << " Unable to select hyperslab - invalid HDF5 dataspace!"
		  << std::endl;
      }
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
  
  //! Read data of type \c bool (H5T_NATIVE_HBOOL)
  template <> bool HDF5Dataset::readData (bool data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_HBOOL);
  }
  
  //! Read data of type \c int (H5T_NATIVE_INT)
  template <> bool HDF5Dataset::readData (int data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_INT);
  }
  
  //! Read data of type \c uint (H5T_NATIVE_UINT)
  template <> bool HDF5Dataset::readData (uint data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_UINT);
  }
  
  //! Read data of type \c short (H5T_NATIVE_SHORT)
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
  
  template <> bool HDF5Dataset::readData (long long data[],
					  HDF5Hyperslab &slab)
  {
    return readData (data, slab, H5T_NATIVE_LLONG);
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
  
  template <> bool HDF5Dataset::writeData (long long const data[],
					  HDF5Hyperslab &slab)
  {
    return writeData (data, slab, H5T_NATIVE_LLONG);
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
    os << "[HDF5Dataset] Summary of internal parameters"        << std::endl;
    os << "-- Dataset name           = " << itsName             << std::endl;
    os << "-- Dataset ID             = " << itsLocation         << std::endl;
    os << "-- Dataspace ID           = " << itsDataspace        << std::endl;
    os << "-- Datatype ID            = " << itsDatatype         << std::endl;
    os << "-- Dataset rank           = " << rank()              << std::endl;
    os << "-- Dataset shape          = " << itsShape            << std::endl;
    os << "-- Layout of the raw data = " << itsLayout           << std::endl;
    os << "-- Chunk size             = " << itsChunking         << std::endl;
    os << "-- nof. datapoints        = " << nofDatapoints()     << std::endl;
    os << "-- nof. active hyperslabs = " << itsHyperslab.size() << std::endl;
  }
  
  // ============================================================================
  //
  //  Static functions
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                       offset
  
  /*!
    \param location -- Dataset identifier.
    \return offset  -- Returns the offset in bytes; otherwise returns
            \c HADDR_UNDEF, a negative value. 
  */
  haddr_t HDF5Dataset::offset (hid_t const &location)
  {
    if (objectType(location) == H5I_DATASET) {
      return H5Dget_offset (location);
    } else {
      return HADDR_UNDEF;
    }
  }
  
} // end namespace DAL
