/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#include "HDF5Common.h"

using std::cerr;
using std::cout;
using std::endl;

#ifdef HAVE_CASA
using casa::MDirection;
using casa::MPosition;
using casa::Quantity;
#endif

namespace DAL {  // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Inspection and Feedback
  //
  // ============================================================================
  
  // ============================================================================
  //
  //  Access attribute values
  //
  // ============================================================================

  // ------------------------------------------------------------ h5set_attribute

  bool h5set_attribute (hid_t const &location_id,
                        std::string const &name,
                        uint const &val)
  {
    bool status (true);
    hid_t attribute_id (0);
    hid_t dataspace_id (0);
    herr_t h5error;

    /* [1] Check if the attribute already exists */
    try
      {
        attribute_id = H5Aopen_name(location_id,
                                    name.c_str());
      }
    catch (std::string message)
      {
        cerr << "[h5set_attribute] " << message << std::endl;
        status = false;
      }

    /* [2] If the attribute does not exist yet, we need to create it first */

    if (attribute_id > 0)
      {
        cout << "-- Attribute " << name << " already exists." << endl;
        // Retrieve the ID of the associated dataspace
        dataspace_id = H5Aget_space (attribute_id);
        // Check if the value matches the type of the existing dataspace
      }
    else
      {
        cout << "-- Attribute "
             << name
             << " does not exist yet; creating it now ..."
             << endl;
        // Create dataspace associated with the attribute
        dataspace_id = H5Screate (H5S_SCALAR);
        // Create the attribute itself
        attribute_id = H5Acreate1 (location_id,
                                   name.c_str(),
                                   H5T_NATIVE_UINT,
                                   dataspace_id,
                                   H5P_DEFAULT);
      }

    // Summary
    cout << "-- Location ID     = " << location_id  << std::endl;
    cout << "-- Attribute name  = " << name         << std::endl;
    cout << "-- Attribute value = " << val          << std::endl;
    cout << "-- Attribute ID    = " << attribute_id << std::endl;
    cout << "-- Dataspace ID    = " << dataspace_id << std::endl;

    // Close dataspace and attribute
    if (dataspace_id > 0)
      {
        h5error = H5Dclose (dataspace_id);
      }

    if (attribute_id > 0)
      {
        h5error = H5Aclose (attribute_id);
      }

    return status;
  }
  
} // Namespace DAL -- end
