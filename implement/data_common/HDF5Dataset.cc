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
    H5Sclose (dataspaceID_p);
    H5Tclose (datatypeID_p);
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
    location_p    = 0;
    dataspaceID_p = 0;
    datatypeID_p  = 0;
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
      if (create && dataspaceID_p && datatypeID_p) {
	location_p = H5Dcreate (location,
				name_p.c_str(),
				datatypeID_p,
				dataspaceID_p,
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
      std::cerr << "[BF_PrimaryPointing::open] Skip opening embedded groups!"
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
    dataspaceID_p = H5Screate_simple (rank,dimensions,NULL);

    // Set the datatype for the elements within the Dataset
    datatypeID_p  = H5Tcopy (datatype);

    // Create the Dataset
    location_p = H5Dcreate (location,
			    name_p.c_str(),
			    datatypeID_p,
			    dataspaceID_p,
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
