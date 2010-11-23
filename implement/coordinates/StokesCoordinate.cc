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
  {
    std::vector<Stokes::Component> stokes (1,Stokes::I);
    init (stokes);
  }

  //_____________________________________________________________________________
  //                                                             StokesCoordinate

  StokesCoordinate::StokesCoordinate (Stokes::Component const &value)
  {
    std::vector<Stokes::Component> stokes (1,value);
    init (stokes);
  }

  //_____________________________________________________________________________
  //                                                             StokesCoordinate

  StokesCoordinate::StokesCoordinate (DAL::Stokes const &value)
  {
    std::vector<DAL::Stokes> stokes (1,value);
    init (stokes);
  }

  //_____________________________________________________________________________
  //                                                             StokesCoordinate

  StokesCoordinate::StokesCoordinate (std::vector<DAL::Stokes::Component> const &values)
  {
    init (values);
  }

  //_____________________________________________________________________________
  //                                                             StokesCoordinate

  StokesCoordinate::StokesCoordinate (std::vector<DAL::Stokes> const &values)
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
    : CoordinateInterface(other)
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
    CoordinateInterface::copy (other);
    // copy variables handles by this class
    init (other.values_p);
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  /*!
    \param os -- Output stream to which the summary is written.
  */
  void StokesCoordinate::summary (std::ostream &os)
  {
    os << "[StokesCoordinate] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type        = " << type() << " / " <<  name() << std::endl;
    os << "-- nof. axes              = " << nofAxes_p       << std::endl;
    os << "-- nof. Stokes components = " << values_p.size() << std::endl;
    os << "-- Stokes component names = " << stokesNames()   << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  void StokesCoordinate::init (std::vector<Stokes::Component> const &values)
  {
    unsigned int nelem = values.size();
    std::vector<DAL::Stokes> stokes (nelem);

    for (unsigned int n(0); n<nelem; ++n) {
      stokes[n] = DAL::Stokes(values[n]);
    }

    init (stokes);
  }

  //_____________________________________________________________________________
  //                                                                         init
  
  void StokesCoordinate::init (std::vector<DAL::Stokes> const &values)
  {
    /* Initialize base class */
    CoordinateInterface::init (Coordinate::STOKES,
			       1);

    // store the input list of Stokes values
    values_p.resize (values.size());
    values_p = values;
  }

  //_____________________________________________________________________________
  //                                                                  stokesTypes
  
  std::vector<Stokes::Component> StokesCoordinate::stokesTypes ()
  {
    unsigned int nelem = values_p.size();
    std::vector<DAL::Stokes::Component> val (nelem);

    for (unsigned int n(0); n<nelem; ++n) {
      val[n] = values_p[n].type();
    }
    
    return val;
  }
  
  //_____________________________________________________________________________
  //                                                                  stokesNames
  
  std::vector<std::string> StokesCoordinate::stokesNames ()
  {
    unsigned int nelem = values_p.size();
    std::vector<std::string> val (nelem);

    for (unsigned int n(0); n<nelem; ++n) {
      val[n] = values_p[n].name();
    }
    
    return val;
  }
  
  // ============================================================================
  //
  //  Methods (using HDF5 library)
  //
  // ============================================================================

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
    if (DAL::Coordinate::getType(coordinate_type) == DAL::Coordinate::STOKES) {
      // basic parameters
      coord_p   = DAL::Coordinate::getType(coordinate_type);
      nofAxes_p = nof_axes;
    }
    else {
      std::cerr << "[StokesCoordinate::h5read]"
		<< " Encountered coordinate does not match expected type!"
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   write_hdf5
  
  void StokesCoordinate::write_hdf5 (hid_t const &locationID,
                                  std::string const &name)
  {
    hid_t groupID (0);
    // create HDF5 group
    if (H5Lexists (locationID, name.c_str(), H5P_DEFAULT)) {
      groupID = H5Gopen (locationID,
			 name.c_str(),
			 H5P_DEFAULT);
      
    } else {
      groupID = H5Gcreate( locationID,
			   name.c_str(),
			   H5P_DEFAULT,
			   H5P_DEFAULT,
			   H5P_DEFAULT );
    }
    // write coordinate attributes
    write_hdf5 (groupID);
    // close the group after write
    H5Gclose (groupID);
  }

  //_____________________________________________________________________________
  //                                                                   write_hdf5

  void StokesCoordinate::write_hdf5 (hid_t const &groupID)
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
