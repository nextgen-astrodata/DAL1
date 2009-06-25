/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1964 2008-09-06 17:52:38Z baehren                   $ |
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

#include <DirectionCoordinate.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                          DirectionCoordinate
  
  DirectionCoordinate::DirectionCoordinate (std::string const &system,
					    std::string const &projection)
    : Coordinate(Coordinate::Direction,
		 2)
  {
    init (system,
	  projection);
  }
  
  //_____________________________________________________________________________
  //                                                          DirectionCoordinate
  
  DirectionCoordinate::DirectionCoordinate (std::vector<std::string> const &axisNames,
					    std::vector<std::string> const &axisUnits,
					    std::vector<double> const &refValue,
					    std::vector<double> const &refPixel,
					    std::vector<double> const &increment,
					    std::vector<double> const &pc,
					    std::string const &system,
					    std::string const &projection)
    : Coordinate(Coordinate::Direction,
		 2,
		 axisNames,
		 axisUnits,
		 refValue,
		 refPixel,
		 increment,
		 pc)
  {
    init (system,
	  projection);
  }
  
  //_____________________________________________________________________________
  //                                                          DirectionCoordinate
  
  /*!
    \param other -- Another DirectionCoordinate object from which to create this new
           one.
  */
  DirectionCoordinate::DirectionCoordinate (DirectionCoordinate const &other)
    : Coordinate(other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  DirectionCoordinate::~DirectionCoordinate ()
  {
    destroy();
  }
  
  void DirectionCoordinate::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  DirectionCoordinate& DirectionCoordinate::operator= (DirectionCoordinate const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void DirectionCoordinate::copy (DirectionCoordinate const &other)
  {
    Coordinate::copy (other);
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void DirectionCoordinate::summary (std::ostream &os)
  {
    os << "[DirectionCoordinate] Summary of internal parameters." << std::endl;
    // Common coordinate parameters
    os << "-- Coordinate type          = " << type() << " / " <<  name() << std::endl;
    os << "-- nof. axes                = " << nofAxes_p      << std::endl;
    os << "-- World axis names         = " << axisNames_p    << std::endl;
    os << "-- World axis units         = " << axisUnits_p    << std::endl;
    os << "-- Reference value          = " << refValue_p     << std::endl;
    os << "-- Reference pixel          = " << refPixel_p     << std::endl;
    os << "-- Increment                = " << increment_p    << std::endl;
    os << "-- Transformation matrix    = " << pc_p           << std::endl;
    // Specific add-ons
    os << "-- Reference system         = " << system_p           << std::endl;
    os << "-- Spherical map projection = " << projection_p       << std::endl;
    os << "-- Projection parameters    = " << projectionParam_p  << std::endl;
    os << "-- Longpole                 = " << longpole_p         << std::endl;
    os << "-- Latpole                  = " << latpole_p          << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  void DirectionCoordinate::h5read (hid_t const &locationID,
				 std::string const &name)
  {
    hid_t groupID (0);
    
    groupID = H5Gopen1 (locationID,
			name.c_str());
    
    if (groupID) {
      h5read (groupID);
    } else {
      std::cerr << "[DirectionCoordinate::h5read] Error opening group "
		<< name 
		<< std::endl;
    }
    
    H5Gclose (groupID);
  }
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  void DirectionCoordinate::h5read (hid_t const &groupID)
  {
    std::string coordinate_type;
    unsigned int nof_axes;
    std::vector<std::string> axis_names;
    std::vector<std::string> axis_units;
    std::vector<double> refvalue;
    std::vector<double> refpixel;
    std::vector<double> increment;
    std::vector<double> pc;
    std::string refname;
    std::string projection;
    std::vector<double> param;
    double longpole;
    double latpole;
    
    /* Read the attributes from the HDF5 file */
    DAL::h5get_attribute( groupID, "COORDINATE_TYPE",  coordinate_type );
    DAL::h5get_attribute( groupID, "NOF_AXES",         nof_axes );
    DAL::h5get_attribute( groupID, "AXIS_NAMES",       axis_names );
    DAL::h5get_attribute( groupID, "AXIS_UNITS",       axis_units );
    DAL::h5get_attribute( groupID, "CRVAL",            refvalue );
    DAL::h5get_attribute( groupID, "CRPIX",            refpixel );
    DAL::h5get_attribute( groupID, "CDELT",            increment );
    DAL::h5get_attribute( groupID, "PC",               pc );
    DAL::h5get_attribute( groupID, "SYSTEM",           refname );
    DAL::h5get_attribute( groupID, "PROJECTION",       projection );
    DAL::h5get_attribute( groupID, "PROJECTION_PARAM", param );
    DAL::h5get_attribute( groupID, "LONGPOLE",         longpole );
    DAL::h5get_attribute( groupID, "LATPOLE",          latpole );

    /* Store the retrieved values */
    if (getType(coordinate_type) == Coordinate::Direction) {
      // basic parameters
      coordinateType_p = getType(coordinate_type);
      nofAxes_p        = nof_axes;
      // WCS parameters
      setAxisNames (axis_names);
      setAxisUnits (axis_units);
      setRefValue  (refvalue);
      setRefPixel  (refpixel);
      setIncrement (increment);
      setPc        (pc);
      //
      system_p     = refname;
      projection_p = projection;
      setProjectionParameters(param);
      setLongpole(longpole);
      setLatpole(latpole);
    } else {
    }
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  void DirectionCoordinate::h5write (hid_t const &locationID,
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
  
  void DirectionCoordinate::h5write (hid_t const &groupID)
  {
    /* Basic common parameters */
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE",  name() );
    DAL::h5set_attribute( groupID, "NOF_AXES",         nofAxes_p );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",       axisNames_p );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",       axisUnits_p );
    DAL::h5set_attribute( groupID, "CRPIX",            refPixel_p );
    DAL::h5set_attribute( groupID, "CRVAL",            refValue_p );
    DAL::h5set_attribute( groupID, "CDELT",            increment_p );
    DAL::h5set_attribute( groupID, "PC",               pc_p );
    /* Add-on for direction coordinate */
    DAL::h5set_attribute( groupID, "SYSTEM",           system_p );
    DAL::h5set_attribute( groupID, "PROJECTION",       projection_p );
    DAL::h5set_attribute( groupID, "PROJECTION_PARAM", projectionParam_p );
    DAL::h5set_attribute( groupID, "LONGPOLE",         longpole_p );
    DAL::h5set_attribute( groupID, "LATPOLE",          latpole_p );
  }
  
} // Namespace DAL -- end
