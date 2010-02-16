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

  HDF5Hyperslab::HDF5Hyperslab (std::vector<hsize_t> const &shape)
  {
    init();
    shape_p = shape;
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab
  
  HDF5Hyperslab::HDF5Hyperslab (std::vector<hsize_t> const &shape,
				std::vector<int> const &start,
				std::vector<int> const &count,
				H5S_seloper_t const &selection)
  {
    init();
    
    shape_p     = shape;
    setStart (start);
    count_p     = count;
    selection_p = selection;
  }
  
  //_____________________________________________________________________________
  //                                                                HDF5Hyperslab
  
  HDF5Hyperslab::HDF5Hyperslab (std::vector<hsize_t> const &shape,
				std::vector<int> const &start,
				std::vector<int> const &stride,
				std::vector<int> const &count,
				std::vector<int> const &block,
				H5S_seloper_t const &selection)
  {
    init();
    
    shape_p     = shape;
    setStart (start);
    stride_p    = stride;
    count_p     = count;
    block_p     = block;
    selection_p = selection;
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
  {;}
  
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
  
  bool HDF5Hyperslab::setStart (std::vector<int> const &start)
  {
    bool status (true);

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
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  void HDF5Hyperslab::summary (std::ostream &os)
  {
    os << "[HDF5Hyperslab] Summary of internal parameters." << std::endl;
    os << "-- Shape  = " << shape_p   << std::endl;
    os << "-- Start  = " << start_p   << std::endl;
    os << "-- Stride = " << stride_p  << std::endl;
    os << "-- Count  = " << count_p   << std::endl;
    os << "-- Block  = " << block_p   << std::endl;
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
    \param location  -- Identifier for the HDF5 object to work with
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
				    std::vector<int> const &count,
				    H5S_seloper_t const &selection)
  {
    std::vector<int> stride;
    std::vector<int> block;

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
    \param location  -- Identifier for the HDF5 object to work with
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

    //____________________________________________
    // Check parameters for the Hyperslab

    bool haveStride (true);
    bool haveBlock (true);
    std::vector<hsize_t> shape;

    DAL::h5get_dataspace_shape (location,shape);
    
    if (start.size() != count.size()) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Parameter mismatch: start - count."
		<< std::endl;
      std::cerr << "-- start = " << start << std::endl;
      std::cerr << "-- count = " << count << std::endl;
      return false;
    }
    else {
      if (start.size() != shape.size()) {
      std::cerr << "[HDF5Hyperslab::setHyperslab] Parameter mismatch: start - shape."
		<< std::endl;
      std::cerr << "-- start = " << start << std::endl;
      std::cerr << "-- shape = " << shape << std::endl;
      return false;
      }
    }
    
    if (stride.size() != shape.size()) {
      haveStride = false;
    }
    
    if (block.size() != shape.size()) {
      haveBlock = false;
    }
    
    //____________________________________________
    // Set up the hyperslab
    
    herr_t h5error;
    hsize_t rank (shape.size());
    hsize_t tmpStart [rank];
    hsize_t tmpStride[rank];
    hsize_t tmpCount [rank];
    hsize_t tmpBlock [rank];

    for (unsigned int n(0); n<shape.size(); ++n) {
      tmpStart[n] = start[n];
      tmpCount[n] = count[n];
    }
    
    if (haveStride) {
      for (unsigned int n(0); n<shape.size(); ++n) {
	tmpStride[n] = stride[n];
      }
    }
    
    if (haveBlock) {
      for (unsigned int n(0); n<shape.size(); ++n) {
	tmpBlock[n] = block[n];
      }
    }
    
    if (haveStride) {
      if (haveBlock) {
	// stride=true, block=true
	h5error = H5Sselect_hyperslab (dataspaceID,
				       selection,
				       tmpStart,      // start
				       tmpStride,     // stride
				       tmpCount,      // count
				       tmpBlock);     // block
      } else {
	// stride=true, block=false
	h5error = H5Sselect_hyperslab (dataspaceID,
				       selection,
				       tmpStart,      // start
				       tmpStride,     // stride
				       tmpCount,      // count
				       NULL);         // block
      }
    } else {
      if (haveBlock) {
	// stride=false, block=true
	h5error = H5Sselect_hyperslab (dataspaceID,
				       selection,
				       tmpStart,      // start
				       NULL,          // stride
				       tmpCount,      // count
				       tmpBlock);     // block
      } else {
	// stride=false, block=false
	h5error = H5Sselect_hyperslab (dataspaceID,
				       selection,
				       tmpStart,      // start
				       NULL,          // stride
				       tmpCount,      // count
				       NULL);         // block
      }
    }
    
    return status;
  }


  
} // Namespace DAL -- end
