/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#include <StokesCoordinate.h>

namespace DAL {  // Namespace DAL -- begin

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                             StokesCoordinate

  StokesCoordinate::StokesCoordinate ()
      : Coordinate(Coordinate::Stokes,
                   1)
  {
    std::vector<StokesCoordinate::Stokes> values(1);
    values[0] = StokesCoordinate::I;
    init (values);
  }

  //_____________________________________________________________________________
  //                                                             StokesCoordinate

  StokesCoordinate::StokesCoordinate (StokesCoordinate::Stokes const &value)
  {
    std::vector<StokesCoordinate::Stokes> values(1);
    values[0] = value;
    init (values);
  }

  //_____________________________________________________________________________
  //                                                             StokesCoordinate

  StokesCoordinate::StokesCoordinate (std::vector<StokesCoordinate::Stokes> const &values)
  {
    init (values);
  }

  //_____________________________________________________________________________
  //                                                             StokesCoordinate

  /*!
    \param other -- Another StokesCoordinate object from which to create this new
           one.
  */
  StokesCoordinate::StokesCoordinate (StokesCoordinate const &other)
    : Coordinate(other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  StokesCoordinate::~StokesCoordinate ()
  {
    destroy();
  }

  void StokesCoordinate::destroy ()
  {
    ;
  }

  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  StokesCoordinate& StokesCoordinate::operator= (StokesCoordinate const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  void StokesCoordinate::copy (StokesCoordinate const &other)
  {
    // copy variables handled by the base class
    Coordinate::copy (other);
    // copy variables handles by this class
    values_p.resize(other.values_p.size());
    values_p = other.values_p;
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void StokesCoordinate::summary (std::ostream &os)
  {
    os << "[StokesCoordinate] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type       = " << type() << " / " <<  name() << std::endl;
    os << "-- nof. axes             = " << nofAxes_p      << std::endl;
  }



  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  void StokesCoordinate::init (std::vector<StokesCoordinate::Stokes> const &values)
  {
    // variables maintained by the base class
    coordinateType_p = Coordinate::Stokes;
    nofAxes_p        = 1;
    Coordinate::init();
    // store the input list of Stokes values
    values_p.resize (values.size());
    values_p = values;
  }

  //_____________________________________________________________________________
  //                                                                      getName

  std::string StokesCoordinate::getName (StokesCoordinate::Stokes const &type)
  {
    std::string name;

    switch (type) {
    case StokesCoordinate::I:
      name = "I";
      break;
    case StokesCoordinate::Q:
      name = "Q";
      break;
    case StokesCoordinate::U:
      name = "U";
      break;
    case StokesCoordinate::V:
      name = "V";
      break;
    case StokesCoordinate::RR:
      name = "RR";
      break;
    case StokesCoordinate::LL:
      name = "LL";
      break;
    case StokesCoordinate::RL:
      name = "RL";
      break;
    case StokesCoordinate::LR:
      name = "LR";
      break;
    case StokesCoordinate::XX:
      name = "XX";
      break;
    case StokesCoordinate::YY:
      name = "YY";
      break;
    case StokesCoordinate::XY:
      name = "XY";
      break;
    case StokesCoordinate::YX:
      name = "YX";
      break;
    default:
      name = "UNDEFINED";
      break;
    };

    return name;
  }

  //_____________________________________________________________________________
  //                                                                      getType

  StokesCoordinate::Stokes StokesCoordinate::getType (std::string const &name)
  {
    StokesCoordinate::Stokes type;

    if (name == "I") {
      type = StokesCoordinate::I;
    }
    else if (name == "Q") {
      type = StokesCoordinate::Q;
    }
    else if (name == "U") {
      type = StokesCoordinate::U;
    }
    else if (name == "V") {
      type = StokesCoordinate::V;
    }
    else if (name == "RR") {
      type = StokesCoordinate::RR;
    }
    else if (name == "LL") {
      type = StokesCoordinate::LL;
    }
    else if (name == "RL") {
      type = StokesCoordinate::RL;
    }
    else if (name == "LR") {
      type = StokesCoordinate::LR;
    }
    else {
      std::cerr << "[StokesCoordinate::getType] Unknow name of Stokes value!"
		<< std::endl;
    }

    return type;
  }

#ifdef HAVE_HDF5

  //_____________________________________________________________________________
  //                                                                       h5read

  /*!
    \param locationID -- Identifier of the HDF5 object to which the coordinate
           group is attached.
    \param name       -- Name of the HDF5 group containing the coordinate
           parameters.
  */
  void StokesCoordinate::h5read (hid_t const &locationID,
                                 std::string const &name)
  {
    hid_t groupID (0);

    groupID = H5Gopen1 (locationID,
                        name.c_str());

    if (groupID) {
      h5read (groupID);
    }
    else {
      std::cerr << "[StokesCoordinate::h5read] Error opening group "
		<< name
		<< std::endl;
    }
    
    H5Gclose (groupID);
  }
  
  //_____________________________________________________________________________
  //                                                                       h5read

  /*!
    \param groupID -- Identifier of the HDF5 group containing the coordinate 
           parameters.
   */
  void StokesCoordinate::h5read (hid_t const &groupID)
  {
    std::string coordinate_type;
    unsigned int nof_axes;

    /* Read the attributes from the HDF5 file */
    DAL::h5get_attribute( groupID, "COORDINATE_TYPE",  coordinate_type );
    DAL::h5get_attribute( groupID, "NOF_AXES",         nof_axes );
    
    /* Store the retrieved values */
    if (Coordinate::getType(coordinate_type) == Coordinate::Stokes) {
      // basic parameters
      coordinateType_p = Coordinate::getType(coordinate_type);
      nofAxes_p        = nof_axes;
    }
    else {
      std::cerr << "[StokesCoordinate::h5read]"
		<< " Encountered coordinate does not match expected type!"
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  void StokesCoordinate::h5write (hid_t const &locationID,
                                  std::string const &name)
  {
    hid_t groupID (0);
    // create HDF5 group
    groupID = H5Gcreate( locationID,
                         name.c_str(),
                         H5P_DEFAULT,
                         H5P_DEFAULT,
                         H5P_DEFAULT );
    // write coordinate attributes
    h5write (groupID);
    // close the group after write
    H5Gclose (groupID);
  }

  //_____________________________________________________________________________
  //                                                                      h5write

  void StokesCoordinate::h5write (hid_t const &groupID)
  {
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE",  name() );
    DAL::h5set_attribute( groupID, "NOF_AXES",         nofAxes_p );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",       axisNames_p );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",       axisUnits_p );
    DAL::h5set_attribute( groupID, "REFERENCE_PIXEL",  refPixel_p );
    DAL::h5set_attribute( groupID, "REFERENCE_VALUE",  refValue_p );
    DAL::h5set_attribute( groupID, "INCREMENT",        increment_p );
    DAL::h5set_attribute( groupID, "PC",               pc_p );
  }

#endif
  
} // Namespace DAL -- end
