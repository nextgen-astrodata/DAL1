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
    init();
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
    H5Sclose (dataspaceID_p);
    H5Tclose (datatypeID_p);
    H5Dclose (datasetID_p);
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
    name_p        = "Dataset";
    dataspaceID_p = 0;
    datatypeID_p  = 0;
    datasetID_p   = 0;
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
    dataspaceID_p = H5Screate_simple (rank,dimensions,NULL);

    // Set the datatype for the elements within the Dataset
    datatypeID_p  = H5Tcopy (datatype);

    // Create the Dataset
    datasetID_p = H5Dcreate (location,
			     name_p.c_str(),
			     datatypeID_p,
			     dataspaceID_p,
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT);

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary

  /*
    \param os -- Output stream to which the summary is written.
  */
  void HDF5Dataset::summary (std::ostream &os)
  {
    os << "[HDF5Dataset]" << std::endl;
    os << "-- Dataset name  = " << name_p  << std::endl;
    os << "-- Dataset shape = " << shape_p << std::endl;
  }
  
} // end namespace DAL
