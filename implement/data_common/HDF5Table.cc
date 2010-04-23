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

#include <HDF5Table.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    HDF5Table
  
  HDF5Table::HDF5Table ()
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                                    HDF5Table
  
  HDF5Table::HDF5Table (hid_t const &location,
			std::string const &name)
  {
    //! Initialize internal parameters
    init();
    // Open the table
    open (location,name,false);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Table::~HDF5Table ()
  {
    destroy();
  }
  
  void HDF5Table::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5Table::copy (HDF5Table const &other)
  {
    columnNames_p.resize(other.columnNames_p.size());

    tableName_p   = other.tableName_p;
    columnNames_p = other.columnNames_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void HDF5Table::summary (std::ostream &os)
  {
    os << "[HDF5Table] Summary of internal parameters."  << std::endl;
    os << "-- Table name   = " << tableName_p            << std::endl;
    os << "-- Column names = " << columnNames_p          << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void HDF5Table::init ()
  {
    tableName_p = "";

    columnNames_p.clear();
  }

  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void HDF5Table::setAttributes ()
  {
    attributes_p.clear();
    
    /* Attributes of an Table Dataset */
    attributes_p.insert("CLASS");
    attributes_p.insert("VERSION");
    attributes_p.insert("TITLE");
  }

  //_____________________________________________________________________________
  //                                                                         open

  bool HDF5Table::open (hid_t const &location,
			std::string const &name,
			bool const &create)
  {
    bool status = create;

    /* Set up the list of attributes attached to the group */
    setAttributes();
    
    if (H5Lexists (location, name.c_str(), H5P_DEFAULT)) {
      // Open the dataset
      location_p = H5Dopen (location,
			    name.c_str(),
			    H5P_DEFAULT);
    } else {
      location_p = 0;
    }
    
    return status;
  }
    
  //_____________________________________________________________________________
  //                                                                 openEmbedded

  bool HDF5Table::openEmbedded (bool const &create)
  {
    bool status = create;
    return status;
  }

  //_____________________________________________________________________________
  //                                                                        shape
  
  std::vector<hid_t> HDF5Table::shape()
  {
    std::vector<hid_t> shape (2);

    return shape;
  }


} // Namespace DAL -- end
