/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 3829 2009-12-09 13:04:56Z baehren                   $ |
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

#include <HDF5Hyperslab.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab

  HDF5Hyperslab::HDF5Hyperslab ()
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab

  /*!
    \param shape -- Shape of the dataset
  */
  HDF5Hyperslab::HDF5Hyperslab (std::vector<hsize_t> const &shape)
  {
    init();
    shape_p = shape;
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab
  
  /*!
    \param shape     -- Shape of the dataset.
    \param start     -- Offset of the starting element of the specified hyperslab.
    \param block     -- The size of the block selected from the dataspace.
    \param selection --  
  */
  HDF5Hyperslab::HDF5Hyperslab (std::vector<hsize_t> const &shape,
				std::vector<int> const &start,
				std::vector<int> const &block,
				H5S_seloper_t const &selection)
  {
    init();
    
    shape_p     = shape;
    setStart (start);
    setBlock (block);
    selection_p = selection;
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab
  
  /*!
    \param shape     -- Shape of the dataset.
    \param start     -- Offset of the starting element of the specified hyperslab.
    \param stride    -- Number of elements to separate each element or block to
           be selected.
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param block     -- The size of the block selected from the dataspace.
    \param selection --  
  */
  HDF5Hyperslab::HDF5Hyperslab (std::vector<hsize_t> const &shape,
				std::vector<int> const &start,
				std::vector<int> const &stride,
				std::vector<int> const &count,
				std::vector<int> const &block,
				H5S_seloper_t const &selection)
  {
    init();
    
    shape_p     = shape;
    selection_p = selection;

    setStart  (start);
    setStride (stride);
    setCount  (count);
    setBlock  (block);
  }

  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab

  HDF5Hyperslab::HDF5Hyperslab (HDF5Hyperslab const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Hyperslab::~HDF5Hyperslab ()
  {
    destroy();
  }
  
  void HDF5Hyperslab::destroy ()
  {
    shape_p.clear();
    start_p.clear();
    stride_p.clear();
    count_p.clear();
    block_p.clear();
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  HDF5Hyperslab& HDF5Hyperslab::operator= (HDF5Hyperslab const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5Hyperslab::copy (HDF5Hyperslab const &other)
  {
    shape_p.clear();
    start_p.clear();
    stride_p.clear();
    count_p.clear();
    block_p.clear();

    shape_p     = other.shape_p;
    start_p     = other.start_p;
    stride_p    = other.stride_p;
    count_p     = other.count_p;
    block_p     = other.block_p;
    selection_p = other.selection_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                     setStart
  
  bool HDF5Hyperslab::setStart (std::vector<int> const &start,
				bool const &ignoreShape)
  {
    bool status (true);

    if (ignoreShape) {
      if (start.size() == start_p.size()) {
	start_p = start;
      } else {
	status = false;
      }
    } else {
      if (shape_p.empty()) {
	std::cerr << "[HDF5Hyperslab::setStart] Shape undefined!" << std::endl;
	status = false;
      } else {
	unsigned int nelem = shape_p.size();
	if (nelem == start.size()) {
	  start_p.resize(nelem);
	  for (unsigned int n(0); n<nelem; ++n) {
	    if (start[n] < int(shape_p[n])) {
	      start_p[n] = start[n];
	    } else {
	      start_p[n] = 0;
	      std::cerr << "[HDF5Hyperslab::setStart] Start position outside shape!"
			<< std::endl;
	      status = false;
	    }
	  }
	} else {
	  std::cerr << "[HDF5Hyperslab::setStart] Rank mismatch!" << std::endl;
	  std::cerr << "-- Shape : " << shape_p << std::endl;
	  std::cerr << "-- Start : " << start   << std::endl;
	  status = false;
	}
      }
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                    setStride
  
  bool HDF5Hyperslab::setStride (std::vector<int> const &stride,
				 bool const &ignoreShape)
  {
    bool status (true);
    unsigned int nelem = stride.size();

    if (ignoreShape) {
      if (nelem == stride_p.size()) {
	stride_p = stride;
      } else {
	status = false;
      }
    } else {
      if (shape_p.empty()) {
	std::cerr << "[HDF5Hyperslab::setStride] Shape undefined!" << std::endl;
	status = false;
      } else {
	if (nelem == shape_p.size()) {
	  stride_p = stride;
	} else {
	  std::cerr << "[HDF5Hyperslab::setStride] Rank mismatch!" << std::endl;
	  std::cerr << "-- Shape  : " << shape_p << std::endl;
	  std::cerr << "-- Stride : " << stride  << std::endl;
	  status = false;
	}
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     setCount
  
  bool HDF5Hyperslab::setCount (std::vector<int> const &count,
				bool const &ignoreShape)
  {
    bool status (true);
    unsigned int nelem = count.size();
    
    if (ignoreShape) {
      if (nelem == count_p.size()) {
	count_p = count;
      } else {
	status = false;
      }
    } else {
      if (shape_p.empty()) {
	std::cerr << "[HDF5Hyperslab::setCount] Shape undefined!" << std::endl;
	status = false;
      } else {
	if (nelem == shape_p.size()) {
	  count_p = count;
	} else {
	  std::cerr << "[HDF5Hyperslab::setCount] Rank mismatch!" << std::endl;
	  std::cerr << "-- Shape : " << shape_p << std::endl;
	  std::cerr << "-- Count : " << count   << std::endl;
	  status = false;
	}
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     setBlock
  
  bool HDF5Hyperslab::setBlock (std::vector<int> const &block,
				bool const &ignoreShape)
  {
    bool status (true);
    unsigned int nelem = block.size();
    
    if (ignoreShape) {
      if (nelem == block_p.size()) {
	block_p = block;
      } else {
	status = false;
      }
    } else {
      if (shape_p.empty()) {
	std::cerr << "[HDF5Hyperslab::setBlock] Shape undefined!" << std::endl;
	status = false;
      } else {
	if (nelem == shape_p.size()) {
	  /* Resize the internal array */
	  block_p.resize(nelem);
	  /* Check the individual entries before accepting them */
	  for (unsigned int n(0); n<nelem; ++n) {
	    if (block[n] > int(shape_p[n])) {
	      block_p[n] = int(shape_p[n]);
	      std::cerr << "[HDF5Hyperslab::setStart] Start position outside shape!"
			<< std::endl;
	      status = false;
	    } else {
	      block_p[n] = block[n];
	    }
	  }
	} else {
	  std::cerr << "[HDF5Hyperslab::setBlock] Rank mismatch!" << std::endl;
	  std::cerr << "-- Shape : " << shape_p << std::endl;
	  std::cerr << "-- Block : " << block   << std::endl;
	  status = false;
	}
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void HDF5Hyperslab::summary (std::ostream &os)
  {
    os << "[HDF5Hyperslab] Summary of internal parameters." << std::endl;
    os << "-- Shape           = " << shape_p         << std::endl;
    os << "-- Start           = " << start_p         << std::endl;
    os << "-- Stride          = " << stride_p        << std::endl;
    os << "-- Count           = " << count_p         << std::endl;
    os << "-- Block           = " << block_p         << std::endl;
    os << "-- nof. datapoints = " << nofDatapoints() << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void HDF5Hyperslab::init ()
  {
    shape_p.clear();
    start_p.clear();
    stride_p.clear();
    count_p.clear();
    block_p.clear();
    selection_p=H5S_SELECT_SET;
  }
  
  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param location -- HDF5 object identifier for the dataset or dataspace to
           to which the Hyperslab is going to be applied.

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t const &location)
  {
    return setHyperslab (location,
			 start_p,
			 stride_p,
			 count_p,
			 block_p,
			 selection_p);
  }

  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param location  -- HDF5 object identifier for the dataset or dataspace to
           to which the Hyperslab is going to be applied.
    \param start     -- Offset of the starting element of the specified hyperslab
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param selection -- Selection operator to determine how the new selection is
           to be combined with the already existing selection for the dataspace.

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t const &location,
				    std::vector<int> const &start,
				    std::vector<int> const &block,
				    H5S_seloper_t const &selection)
  {
    std::vector<int> stride;
    std::vector<int> count;

    return setHyperslab (location,
			 start,
			 stride,
			 count,
			 block,
			 selection);
  }
  
  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param location  -- HDF5 object identifier for the dataset or dataspace to
           to which the Hyperslab is going to be applied.
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
  bool HDF5Hyperslab::setHyperslab (hid_t const &location,
				    std::vector<int> const &start,
				    std::vector<int> const &stride,
				    std::vector<int> const &count,
				    std::vector<int> const &block,
				    H5S_seloper_t const &selection)
  {
    bool status (true);
    herr_t h5error;

    //____________________________________________
    // Check the object to work with
    
    if (!H5Iis_valid(location)) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Provided location not a valid"
		<< " HDF5 object!"
		<< std::endl;
      return false;
    }
    
    //__________________________________________
    // Get object identifier for the dataspace

    hid_t dataspaceID (0);
    H5I_type_t objectType = H5Iget_type (location);
    bool locationIsDataset (true);
    
    if (objectType == H5I_DATASPACE) {
      locationIsDataset = false;
      dataspaceID       = location;
    }
    else if (objectType == H5I_DATASET) {
      locationIsDataset = true;
      dataspaceID       = H5Dget_space (location);
    }
    else {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Unusable location;"
		<< " expecting dataset or dataspace!"
		<< std::endl;
      return false;
    }

    // Check parameters for the Hyperslab ________

    unsigned int nelem;
    std::vector<hsize_t> shape;
    bool haveStride (true);
    bool haveCount (true);

    DAL::h5get_dataspace_shape (location,shape);
    nelem = shape.size();
    
    if (start.size() != nelem) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Parameter mismatch: start - shape."
		<< std::endl;
      std::cerr << "-- start = " << start << std::endl;
      std::cerr << "-- shape = " << shape << std::endl;
      return false;
    }
    else if (block.size() != nelem) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Parameter mismatch: block - shape."
		<< std::endl;
      std::cerr << "-- block = " << block << std::endl;
      std::cerr << "-- shape = " << shape << std::endl;
      return false;
    }
    
    if (stride.size() != nelem || stride.empty()) {
      haveStride = false;
    }
    
    if (count.size() != nelem || count.empty()) {
      haveCount = false;
    }

    // Adjust the size of the dataset ____________

    std::vector<unsigned int> endHyperslab = end (start,stride,count,block);
    bool extendDataset (false);

    for (unsigned int n(0); n<nelem; ++n) {
      if (endHyperslab[n]>shape[n]) {
	extendDataset=true;
      }
    }
    
    if (extendDataset) {
      if (objectType == H5I_DATASET) {
	hsize_t tmpSize [nelem];
	for (unsigned int n(0); n<nelem; ++n) {
	  tmpSize[n] = endHyperslab[n];
	}
	//
	std::cout << "[HDF5Hyperslab::setHyperslab] Extending dataset: "
		  << shape << " -> " << endHyperslab
		  << std::endl;
	// Extend the size of the dataset
	h5error = H5Dset_extent (location, tmpSize);
      }
      else {
	std::cerr << "[HDF5Hyperslab::setHyperslab] Unable to extend dataset size;"
		  << " provided HDF5 object is not a dataset!"
		  << std::endl;
      }
    }
    
    // Set up the hyperslab ______________________
    
    hsize_t rank (shape.size());
    hsize_t tmpStart [rank];
    hsize_t tmpStride[rank];
    hsize_t tmpCount [rank];
    hsize_t tmpBlock [rank];
    
    /* Copy the input parameters to be passed on */
    for (unsigned int n(0); n<nelem; ++n) {
      tmpStart[n]  = start[n];
      tmpBlock[n]  = block[n];
    }
    
    if (haveStride) {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpStride[n] = stride[n];
      }
    } else {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpStride[n] = 1;
      }
    }
    
    if (haveCount) {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpCount[n] = count[n];
      }
    } else {
      for (unsigned int n(0); n<nelem; ++n) {
	tmpCount[n] = 1;
      }
    }
    
    /* Select the hyperslab */
    h5error = H5Sselect_hyperslab (dataspaceID,
				   selection,
				   tmpStart,      // start
				   tmpStride,     // stride
				   tmpCount,      // count
				   tmpBlock);     // block

    if (!H5Sselect_valid (dataspaceID)) {
      std::cerr << "[HDF5Hyperslab::setHyperslab]"
		<< " Selection is not contained within the extent of the dataspace."
		<< std::endl;
      //
      for (unsigned int n(0); n<nelem; ++n) {
	std::cout << "\t[ " << tmpStart[n]
		  << " .. " << tmpStart[n]+tmpCount[n]*tmpBlock[n]
		  << " ]" << std::endl;
      }
      //
      status = false;
    }
    
    return status;
  }  

  //_____________________________________________________________________________
  //                                                                nofDatapoints
  
  unsigned int HDF5Hyperslab::nofDatapoints ()
  {
    return nofDatapoints (count_p,
			  block_p);
  }
  
  //_____________________________________________________________________________
  //                                                                nofDatapoints
  
  /*!
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param block     -- The size of the block selected from the dataspace.

    \return nofDatapoints -- The number of datapoints returned using a given 
            hyperslab.
  */
  unsigned int HDF5Hyperslab::nofDatapoints (std::vector<int> const &count,
					     std::vector<int> const &block)
  {
    unsigned int sizeCount = count.size();
    unsigned int sizeBlock = block.size();
    unsigned int nelem (1);
    
    if (sizeCount) {
      if (sizeBlock) {
	if (sizeCount == sizeBlock) {
	  for (unsigned int n(0); n<sizeCount; ++n) {
	    nelem *= count[n]*block[n];
	  }
	} else {
	  std::cerr << "[HDF5Hyperslab::nofDatapoints] Size mismatch!" << std::endl;
	  std::cerr << "-- shape(count) = " << sizeCount << std::endl;
	  std::cerr << "-- shape(block) = " << sizeBlock << std::endl;
	}
      } else {
	for (unsigned int n(0); n<sizeCount; ++n) {
	  nelem *= count[n];
	}
      }
    }  //  end : if (sizeCount)
    else {
      if (sizeBlock) {
	for (unsigned int n(0); n<sizeBlock; ++n) {
	  nelem *= block[n];
	}
      } else {
	nelem = 0;
      }
    }
    
    return nelem;
  }
  
  //_____________________________________________________________________________
  //                                                                          end

  std::vector<unsigned int> HDF5Hyperslab::end ()
  {
    return end (start_p,
		stride_p,
		count_p,
		block_p);
  }

  //_____________________________________________________________________________
  //                                                                          end

  /*!
    \param start     -- Offset of the starting element of the specified hyperslab,
           \f$ N_{\rm start} \f$ .
    \param stride    -- Number of elements to separate each element or block to
           be selected, \f$ N_{\rm stride} \f$ .
    \param count     -- The number of elements or blocks to select along each
           dimension, \f$ N_{\rm count} \f$ .
    \param block     -- The size of the block selected from the dataspace,
           \f$ N_{\rm block} \f$ .
    
    \return end -- The offset of the last element of the specified hyperslab; given
            above input parameters the range of elements defined by the hyperslab
	    becomes
	    \f[ \left[ n_{\rm start} , n_{\rm start} + n_{\rm block} \cdot
	    n_{\rm stride} \cdot n_{\rm count} - 1 \right]
	    \f]
  */
  std::vector<unsigned int> HDF5Hyperslab::end (std::vector<int> const &start,
						std::vector<int> const &stride,
						std::vector<int> const &count,
						std::vector<int> const &block)
  {
    unsigned int sizeStart  = start.size();
    unsigned int sizeStride = stride.size();
    unsigned int sizeCount  = count.size();
    unsigned int sizeBlock  = block.size();
    std::vector<int> varStride (sizeStart,1);
    
    std::vector<unsigned int> pos;
    
    // Check input parameters ____________________
    
    if (sizeStart  != sizeBlock) {
      std::cerr << "[HDF5Hyperslab::end] Size mismatch!" << std::endl;
      std::cerr << "-- start = " << start << std::endl;
      std::cerr << "-- block = " << block << std::endl;
      return pos;
    }
    
    
    // Compute end position ______________________

    pos.resize(sizeStart);
    
    if (sizeCount != sizeStart || count.empty()) {
      if (sizeStride != sizeStart || stride.empty()) {
	// count=1, stride=1
	for (unsigned int n(0); n<sizeStart; ++n) {
	  pos[n] = start[n] + block[n] - 1;
	}
      }
      else {
	// count=1
	for (unsigned int n(0); n<sizeStart; ++n) {
	  pos[n] = start[n] + block[n]*stride[n] - 1;
	}
      }
    }
    else {
      if (sizeStride != sizeStart || stride.empty()) {
	// stride=1
	for (unsigned int n(0); n<sizeStart; ++n) {
	  pos[n] = start[n] + block[n]*count[n] - 1;
	}
      }
      else {
	for (unsigned int n(0); n<sizeStart; ++n) {
	  pos[n] = start[n] + block[n]*count[n]*stride[n] - 1;
	}
      }
    }

    return pos;
  }
  
} // Namespace DAL -- end
