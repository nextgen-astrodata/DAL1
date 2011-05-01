/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include "HDF5Dataspace.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  HDF5Dataspace::HDF5Dataspace ()
    : HDF5Object()
  {;}

  HDF5Dataspace::HDF5Dataspace (hid_t const &location)
    : HDF5Object()
  {
    open (location);
  }
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  HDF5Dataspace::HDF5Dataspace (HDF5Dataspace const &other)
    : HDF5Object (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Dataspace::~HDF5Dataspace ()
  {
    destroy();
  }
  
  void HDF5Dataspace::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another HDF5Dataspace object from which to make a copy.
  */
  HDF5Dataspace& HDF5Dataspace::operator= (HDF5Dataspace const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5Dataspace::copy (HDF5Dataspace const &other)
  {
    itsLocation = other.itsLocation;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void HDF5Dataspace::summary (std::ostream &os)
  {
    os << "[HDF5Dataspace] Summary of internal parameters." << std::endl;
    os << "-- Location ID " << itsLocation << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Location of the object of which to open the dataspace;
           supported object types are \c DATASPACE, \c DATASET and \c ATTR.
    \return status  -- Status of the operation; returns \e false in case an error
            was encountered, e.g. because the \e location points to an invalid
	    pbject or to an object with no dataspace attached.
   */
  bool HDF5Dataspace::open (hid_t const &location)
  {
    bool status = true;
    
    switch (HDF5Object::objectType(location)) {
    case H5I_DATASPACE:
      itsLocation = H5Scopy (location);
      break;
    case H5I_DATASET:
      itsLocation = H5Dget_space (location);
      break;
    case H5I_ATTR:
      itsLocation = H5Aget_space (location);
      break;
    default:
      status = false;
      break;
    }
    
    return status;
  }
  
  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                     isSimple
  
  /*!
    \param location -- HDF5 object identifier; can point to dataspace, dataset or
           attribute.
   */
  htri_t HDF5Dataspace::isSimple (hid_t const location)
  {
    htri_t result = 0;

    switch (HDF5Object::objectType(location)) {
    case H5I_DATASPACE:
      {
	result = H5Sis_simple (location);
      }
      break;
    case H5I_DATASET:
      {
	/* If location points to dataspace, open dataspace first. */
	hid_t dataspaceID = H5Dget_space (location);
	result = H5Sis_simple (dataspaceID);
	H5Sclose (dataspaceID);
      }
      break;
    case H5I_ATTR:
      {
	/* If location points to an attribute, open dataspace first. */
	hid_t dataspaceID = H5Aget_space (location);
	result = H5Sis_simple (dataspaceID);
	H5Sclose (dataspaceID);
      }
      break;
    default:
      result = 0;
      break;
    }
    
    return result;
  }
  
  //_____________________________________________________________________________
  //                                                                        shape
  
  bool HDF5Dataspace::shape (hid_t const &location,
			     std::vector<hsize_t> &dimensions,
			     bool const &getMaximum)
  {
    std::vector<hsize_t> maxDimensions;
    
    /* If a) just a single set of dimensions is to be returned, and b)
       the maximum dimensions are selected, the internal variables are passed
       on in reversed order.*/
    if (getMaximum) {
      return shape (location,
		    maxDimensions,
		    dimensions);
    } else {
      return shape (location,
		    dimensions,
		    maxDimensions);
    }
  }
  
  //_____________________________________________________________________________
  //                                                                        shape
  
  bool HDF5Dataspace::shape (hid_t const &location,
			     std::vector<hsize_t> &dimensions,
			     std::vector<hsize_t> &maxDimensions)
  {
    bool status = true;
    
    //____________________________________________
    // Check the object to work with
    
    if (!H5Iis_valid(location)) {
      std::cerr << "[HDF5Dataspace::shape] Invalid object ID!"
		<< std::endl;
      return false;
    }
    
    //____________________________________________
    // Get object identifier for the dataspace
    
    hid_t dataspaceID = 0;
    
    switch (HDF5Object::objectType(location)) {
    case H5I_DATASPACE:
      dataspaceID = H5Scopy (location);
      break;
    case H5I_DATASET:
      dataspaceID = H5Dget_space (location);
      break;
    case H5I_ATTR:
      dataspaceID = H5Aget_space (location);
      break;
    default:
      std::cerr << "[HDF5Dataspace::shape] Unusable location;"
		<< " expecting dataset or dataspace!"
		<< std::endl;
      return false;
      break;
    };

    if (!H5Iis_valid(dataspaceID)) {
      return false;
    }
    
    //____________________________________________
    // Extract the shape of the dataspace
    
    int rank = H5Sget_simple_extent_ndims (dataspaceID);

    if (rank > 0) {
      /* Update array sizes */
      dimensions.resize(rank);
      maxDimensions.resize(rank);
      /* Get dataspace dimensions */
      H5Sget_simple_extent_dims(dataspaceID,
				&dimensions[0],
				&maxDimensions[0]);
    }
    
    H5Sclose (dataspaceID);
    
    return status;
  }
  
} // Namespace DAL -- end
