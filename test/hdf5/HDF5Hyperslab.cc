/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1964 2008-09-06 17:52:38Z baehren                   $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#include "HDF5Hyperslab.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  HDF5Hyperslab::HDF5Hyperslab ()
  {;}
  
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
    /* Release HDF5 object handler */
    H5Dclose (datasetID_p);
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  HDF5Hyperslab& HDF5Hyperslab::operator= (HDF5Hyperslab const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  void HDF5Hyperslab::copy (HDF5Hyperslab const &other)
  {;}

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void HDF5Hyperslab::summary (std::ostream &os)
  {
    os << "[HDF5Hyperslab] Summary of internal parameters." << std::endl;
  }
  
  
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                  openDataset
  
  bool HDF5Hyperslab::openDataset (hid_t const &location,
				 std::string const &name)
  {
    bool status (true);
    hid_t dataset_id (0);
    
    try {
      dataset_id = H5Dopen (location,
			    name.c_str(),
			    H5P_DEFAULT);
    }
    catch (std::string message) {
      std::cerr << "[HDF5Hyperslab::openDataset] " << message << std::endl;
      status = false;
    }
    
    if (dataset_id > 0) {
      datasetID_p = dataset_id;
    }
    else {
      datasetID_p = 0;
      status = false;
    }
    
    return status;
  }
  
} // Namespace DAL -- end
