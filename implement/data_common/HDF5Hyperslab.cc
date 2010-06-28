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
    \param rank -- Rank of the dataset, i.e. the number of array axes.
  */
  HDF5Hyperslab::HDF5Hyperslab (int const &rank)
    : rank_p (rank)
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab
  
  /*!
    \param start     -- Offset of the starting element of the specified hyperslab.
    \param block     -- The size of the block selected from the dataspace.
    \param selection --  
  */
  HDF5Hyperslab::HDF5Hyperslab (std::vector<int> const &start,
				std::vector<int> const &block,
				H5S_seloper_t const &selection)
  {
    init();
    
    setStart (start);
    setBlock (block);
    selection_p = selection;
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab
  
  /*!
    \param start     -- Offset of the starting element of the specified hyperslab.
    \param stride    -- Number of elements to separate each element or block to
           be selected.
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param block     -- The size of the block selected from the dataspace.
    \param selection --  
  */
  HDF5Hyperslab::HDF5Hyperslab (std::vector<int> const &start,
				std::vector<int> const &stride,
				std::vector<int> const &count,
				std::vector<int> const &block,
				H5S_seloper_t const &selection)
  {
    init();
    
    selection_p = selection;

    setStart  (start);
    setBlock  (block);
    setCount  (count);
    setStride (stride);
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
    start_p.clear();
    stride_p.clear();
    count_p.clear();
    block_p.clear();

    rank_p      = other.rank_p;
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
  
  bool HDF5Hyperslab::setStart (std::vector<int> const &start)
  {
    bool status = true;
    int nelem   = start.size();

    /* Check if input is empty vector; in that case clear the internally stored
       value. */
    if (start.empty()) {
      start_p.clear();
      return status;
    } else {
      /* Check if rank has been initialized. */
      if (rank_p<0) {
	rank_p = nelem;
      }
    }

    /* Process non-empty input vector. */
    if (nelem == rank_p) {
      start_p = start;
    } else if (nelem > rank_p) {
      for (int n(0); n< rank_p; ++n) {
	start_p[n] = start[n];
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setStart] Input array has too few elements!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                    setStride

  /*!
    \param stride -- Number of elements to separate each element or block to be
           selected. If provided an empty vector, the stride will be set to 1 along
	   each axis.
    
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool HDF5Hyperslab::setStride (std::vector<int> const &stride)
  {
    bool status = true;
    int nelem   = stride.size();

    /* Check if input is empty vector; in that case clear the internally stored
       value. */
    if (stride.empty()) {
      stride_p.clear();
      return status;
    } else {
      /* Check if rank has been initialized. */
      if (rank_p<0) {
	rank_p = nelem;
      }
    }

    /* Process non-empty input vector. */
    if (nelem == rank_p) {
      stride_p = stride;
    } else if (nelem > rank_p) {
      for (int n(0); n< rank_p; ++n) {
	stride_p[n] = stride[n];
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setStride] Input array has too few elements!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     setCount
  
  bool HDF5Hyperslab::setCount (std::vector<int> const &count)
  {
    bool status = true;
    int nelem   = count.size();

    /* Check if input is empty vector; in that case clear the internally stored
       value. */
    if (count.empty()) {
      count_p.clear();
      return status;
    } else {
      /* Check if rank has been initialized. */
      if (rank_p<0) {
	rank_p = nelem;
      }
    }

    /* Process non-empty input vector. */
    if (nelem == rank_p) {
      count_p = count;
    } else if (nelem > rank_p) {
      for (int n(0); n< rank_p; ++n) {
	count_p[n] = count[n];
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setCount] Input array has too few elements!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     setBlock
  
  bool HDF5Hyperslab::setBlock (std::vector<int> const &block)
  {
    bool status (true);
    int nelem = block.size();

    /* First check if the rank information has been initialized */
    if (rank_p<0) {
      rank_p = nelem;
    }
    
    if (block.empty()) {
      block_p.clear();
      block_p.resize(rank_p);
    }
    
    if (nelem == rank_p) {
      block_p = block;
    } else if (nelem > rank_p) {
      for (int n(0); n< rank_p; ++n) {
	block_p[n] = block[n];
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setBlock] Input array has too few elements!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                          gap
  
  /*!
    \return gap -- The size of the gap between two subsequent blocks, i.e.
            \f$ N_{\rm stride} - N_{\rm block} \f$.
  */
  std::vector<int> HDF5Hyperslab::gap ()
  {
    std::vector<int> val (rank_p);
    
    if (stride_p.empty()) {
      for (int n(0); n<rank_p; ++n) {
	val[n] = 1;
      }
    } else {
      for (int n(0); n<rank_p; ++n) {
	val[n] = stride_p[n];
      }
    }
    
    if (block_p.empty()) {
      for (int n(0); n<rank_p; ++n) {
	val[n] -= 1;
      }
    } else {
      for (int n(0); n<rank_p; ++n) {
	val[n] -= block_p[n];
      }
    }
    
    return val;
  }
  
  //_____________________________________________________________________________
  //                                                                       setGap
  
  /*!
    \param gap -- The size of the gap between two subsequent blocks, i.e.
           \f$ N_{\rm stride} - N_{\rm block} \f$.

    \return status -- Status of the operation; return \e false in case an error
            was encountered, e.g. that there was a rank mismatch.
  */
  bool HDF5Hyperslab::setGap (std::vector<int> const &gap)
  {
    int nelem = gap.size();
    bool status (true);
    
    if (nelem == rank_p) {
      
      if (stride_p.empty()) {
	stride_p.resize(nelem);
      }
      
      if (block_p.empty()) {
	for (int n(0); n<nelem; ++n) {
	  stride_p[n] = 1 + gap[n];
	}
      } else {
	for (int n(0); n<nelem; ++n) {
	  stride_p[n] = block_p[n] + gap[n];
	}
      }
      
    } else {
      std::cerr << "[HDF5Hyperslab::setGap] Rank mismatch!" << std::endl;
      std::cerr << "-- Rank : " << rank_p << std::endl;
      std::cerr << "-- Gap  : " << gap    << std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \brief os -- Output stream to which the summary is going to be written.
  */
  void HDF5Hyperslab::summary (std::ostream &os)
  {
    os << "[HDF5Hyperslab] Summary of internal parameters." << std::endl;
    os << "-- Rank            = " << rank_p          << std::endl;
    os << "-- Start           = " << start_p         << std::endl;
    os << "-- Stride          = " << stride_p        << std::endl;
    os << "-- Count           = " << count_p         << std::endl;
    os << "-- Block           = " << block_p         << std::endl;
    os << "-- End position    = " << end()           << std::endl;
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
    rank_p      = -1;
    selection_p = H5S_SELECT_SET;
    start_p.clear();
    stride_p.clear();
    count_p.clear();
    block_p.clear();
  }
  
  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param location -- HDF5 object identifier for the dataset or dataspace to
           to which the Hyperslab is going to be applied.
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t const &location,
				    bool const &resizeDataset)
  {
    return setHyperslab (location,
			 selection_p,
			 start_p,
			 stride_p,
			 count_p,
			 block_p,
			 resizeDataset);
  }

  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param location  -- HDF5 object identifier for the dataset or dataspace to
           to which the Hyperslab is going to be applied.
    \param selection -- Selection operator to determine how the new selection is
           to be combined with the already existing selection for the dataspace.
    \param start     -- Offset of the starting element of the specified hyperslab
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t const &location,
				    H5S_seloper_t const &selection,
				    std::vector<int> const &start,
				    std::vector<int> const &block,
				    bool const &resizeDataset)
  {
    std::vector<int> stride;
    std::vector<int> count;
    
    return setHyperslab (location,
			 selection,
			 start,
			 stride,
			 count,
			 block,
			 resizeDataset);
  }
  
  //_____________________________________________________________________________
  //                                                                 setHyperslab
  
  /*!
    \param location  -- HDF5 object identifier for the dataset or dataspace to
           to which the Hyperslab is going to be applied.
    \param selection -- Selection operator to determine how the new selection is
           to be combined with the already existing selection for the dataspace.
    \param start     -- Offset of the starting element of the specified hyperslab
    \param stride    -- Number of elements to separate each element or block to
           be selected
    \param count     -- The number of elements or blocks to select along each
           dimension.
    \param block     -- The size of the block selected from the dataspace
    \param resizeDataset -- Resize the dataset to the dimensions defined by the 
           Hyperslab?

    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
  */
  bool HDF5Hyperslab::setHyperslab (hid_t const &location,
				    H5S_seloper_t const &selection,
				    std::vector<int> const &start,
				    std::vector<int> const &stride,
				    std::vector<int> const &count,
				    std::vector<int> const &block,
				    bool const &resizeDataset)
  {
    //______________________________________________________
    // Initial check and general local variables
    
    if (!H5Iis_valid(location)) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Provided location not a valid"
		<< " HDF5 object!"
		<< std::endl;
      return false;
    }

    bool status           = true;
    hid_t dataspaceID     = 0;
    herr_t h5error        = 0;
    H5I_type_t objectType = H5Iget_type (location);

    // Check parameters for the Hyperslab __________________

    unsigned int nelem;
    std::vector<hsize_t> shape;
    bool haveStride (true);
    bool haveCount (true);

    DAL::h5get_dataspace_shape (location,shape);
    nelem = shape.size();
    
    if (start.size() != nelem) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Parameter mismatch: start - shape."
		<< std::endl << std::flush;
      std::cerr << "-- start = " << start << std::endl << std::flush;
      std::cerr << "-- shape = " << shape << std::endl << std::flush;
      return false;
    }
    else if (block.size() != nelem) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Parameter mismatch: block - shape."
		<< std::endl << std::flush;
      std::cerr << "-- block = " << block << std::endl << std::flush;
      std::cerr << "-- shape = " << shape << std::endl << std::flush;
      return false;
    }
    
    if (stride.size() != nelem || stride.empty()) {
      haveStride = false;
    }
    
    if (count.size() != nelem || count.empty()) {
      haveCount = false;
    }

    // Adjust the size of the dataset ______________________

    std::vector<hsize_t> endHyperslab = end (start,stride,count,block);
    bool extendDataset (false);

    for (unsigned int n(0); n<nelem; ++n) {
      if (endHyperslab[n]>shape[n]) {
	extendDataset=true;
      }
    }
    
    DAL::h5get_dataspace_shape (location,shape);
    std::cout << "--- shape[old] = " << shape << std::endl;

    if (extendDataset && resizeDataset) {
      //
      DAL::h5get_dataspace_shape (location,shape);
      std::cout << "--- shape[old] = " << shape << std::endl << std::flush;
      //
      if (objectType == H5I_DATASET) {
	hsize_t tmpSize [nelem];
	for (unsigned int n(0); n<nelem; ++n) {
	  tmpSize[n] = endHyperslab[n];
	}
	h5error = H5Dset_extent (location, tmpSize);
      }
      else {
	std::cerr << "[HDF5Hyperslab::setHyperslab] Unable to extend dataset size;"
		  << " provided HDF5 object is not a dataset!"
		  << std::endl << std::flush;
      }
    DAL::h5get_dataspace_shape (location,shape);
    std::cout << "--- shape[new] = " << shape << std::endl << std::flush;
    }

    // Set up the hyperslab parameters _____________________
    
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
    
    // Select hyperslab from dataspace _____________________

    if (objectType == H5I_DATASPACE) {
      dataspaceID       = location;
    }
    else if (objectType == H5I_DATASET) {
      dataspaceID       = H5Dget_space (location);
    }
    else {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Unusable location;"
		<< " expecting dataset or dataspace!"
		<< std::endl << std::flush;
      return false;
    }

    if (H5Iis_valid(dataspaceID)) {
      /* Assign the Hyperslab selection to the dataspace. */
      h5error = H5Sselect_hyperslab (dataspaceID,
				     selection,
				     tmpStart,      // start
				     tmpStride,     // stride
				     tmpCount,      // count
				     tmpBlock);     // block
      
      if (h5error<0) {
	std::cerr << "[HDF5Hyperslab::setHyperslab] Error selecting hyperslab!"
		  << std::endl << std::flush;
	status = false;
      } else {
	htri_t selectValid = H5Sselect_valid (dataspaceID);
	/*
	 * Returns a positive value, for TRUE, if the selection is contained
	 * within the extent or 0 (zero), for FALSE, if it is not. Returns a
	 * negative value on error conditions such as the selection or extent
	 * not being defined.
	 */
	if (selectValid==0) {
	  std::cerr << "[HDF5Hyperslab::setHyperslab]"
		    << " Selection is NOT contained within the extend of the dataset!"
		    << std::endl << std::flush;
	  status = false;
	} else if (selectValid<0) {
	  std::cerr << "[HDF5Hyperslab::setHyperslab]"
		    << " Selection parameters NOT defined properly!"
		    << std::endl << std::flush;
	  status = false;
	}
      }
    } else {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Invalid dataset object"
		<< " -- skipping selection of hypeslab!" 
		<< std::endl << std::flush;
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

  std::vector<hsize_t> HDF5Hyperslab::end ()
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
	    \f[
	    N_{\rm end} \ = \ N_{\rm start} + N_{\rm block} \cdot n_{\rm count} - 1
	    + (N_{\rm block}-N_{\rm stride}) \cdot (N_{\rm count}-1)
	    \f]
	    For this we have two special cases:
	    \f[
	    N_{\rm end} \ = \ \left\{  \begin{array}{lcl}
	    N_{\rm start} + N_{\rm block} -1 & \hbox{if} &  N_{\rm count} \equiv 1 \\[3mm]
	    N_{\rm start} + \left( 2 N_{\rm block} - 1 \right) \cdot N_{\rm count}
	    - N_{\rm block} & \hbox{if} &  N_{\rm stride} \equiv 1
	    \end{array} \right.
	    \f]
  */
  std::vector<hsize_t> HDF5Hyperslab::end (std::vector<int> const &start,
					   std::vector<int> const &stride,
					   std::vector<int> const &count,
					   std::vector<int> const &block)
  {
    unsigned int sizeStart  = start.size();
    unsigned int sizeStride = stride.size();
    unsigned int sizeCount  = count.size();
    unsigned int sizeBlock  = block.size();
    std::vector<int> tmpStride (sizeStart);
    std::vector<int> tmpCount (sizeStart);
    
    std::vector<hsize_t> pos;
    
    // Check input parameters ____________________
    
    if (sizeStart  != sizeBlock) {
      std::cerr << "[HDF5Hyperslab::end] Size mismatch!" << std::endl;
      std::cerr << "-- start = " << start << std::endl;
      std::cerr << "-- block = " << block << std::endl;
      return pos;
    }
    
    if (sizeStride != sizeStart || stride.empty()) {
      tmpStride = std::vector<int> (sizeStart,1);
    } else {
      tmpStride = stride;
    }
    
    if (sizeCount != sizeStart || count.empty()) {
      tmpCount = std::vector<int> (sizeStart,1);
    } else {
      tmpCount = count;
    }
    
    // Compute end position ______________________

    pos.resize(sizeStart);
    
    for (unsigned int n(0); n<sizeStart; ++n) {
      pos[n] = start[n];
      pos[n] += tmpCount[n]*block[n] - 1 + (block[n]-tmpStride[n])*(tmpCount[n]-1);
    }
    
    return pos;
  }
  
} // Namespace DAL -- end
