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

#include <LinearCoordinate.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                             LinearCoordinate
  
  LinearCoordinate::LinearCoordinate ()
    : Coordinate(Coordinate::Linear,
		 1)
  {
  }

  //_____________________________________________________________________________
  //                                                             LinearCoordinate
  
  LinearCoordinate::LinearCoordinate (double const &nofAxes)
    : Coordinate(Coordinate::Linear,
		 nofAxes)
  {
  }
  
  //_____________________________________________________________________________
  //                                                             LinearCoordinate
  
  LinearCoordinate::LinearCoordinate (double const &nofAxes,
				      std::vector<std::string> const &axisNames,
				      std::vector<std::string> const &axisUnits,
				      std::vector<double> const &refValue,
				      std::vector<double> const &refPixel,
				      std::vector<double> const &increment,
				      std::vector<double> const &pc)
    : Coordinate(Coordinate::Linear,
		 nofAxes,
		 axisNames,
		 axisUnits,
		 refValue,
		 refPixel,
		 increment,
		 pc)
  {}

  //_____________________________________________________________________________
  //                                                             LinearCoordinate
  
  /*!
    \param other -- Another LinearCoordinate object from which to create this new
           one.
  */
  LinearCoordinate::LinearCoordinate (LinearCoordinate const &other)
    : Coordinate(other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  LinearCoordinate::~LinearCoordinate ()
  {
    destroy();
  }
  
  void LinearCoordinate::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  LinearCoordinate& LinearCoordinate::operator= (LinearCoordinate const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  void LinearCoordinate::copy (LinearCoordinate const &other)
  {
    /* Copy basic attributes */
    coordinateType_p = other.coordinateType_p;
    nofAxes_p        = other.nofAxes_p;

    /* Resize internal arrays */
    axisNames_p.resize(nofAxes_p);
    axisUnits_p.resize(nofAxes_p);
    refValue_p.resize(nofAxes_p);
    refPixel_p.resize(nofAxes_p);
    increment_p.resize(nofAxes_p);

    /* Copy the values */
    axisNames_p = other.axisNames_p;
    axisUnits_p = other.axisUnits_p;
    refValue_p  = other.refValue_p;
    refPixel_p  = other.refPixel_p;
    increment_p = other.increment_p;    
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void LinearCoordinate::summary (std::ostream &os)
  {
    os << "[LinearCoordinate] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type       = " << type()         << std::endl;
    os << "-- Coordinate name       = " << name()         << std::endl;
    os << "-- nof. axes             = " << nofAxes_p      << std::endl;
    os << "-- World axis names      = " << axisNames_p    << std::endl;
    os << "-- World axis units      = " << axisUnits_p    << std::endl;
    os << "-- Reference value       = " << refValue_p     << std::endl;
    os << "-- Reference pixel       = " << refPixel_p     << std::endl;
    os << "-- Increment             = " << increment_p    << std::endl;
    os << "-- Transformation matrix = " << pc_p           << std::endl;
  }
  
  
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                       h5read
  
  void LinearCoordinate::h5read (hid_t const &groupID)
  {
    std::string coordinate_type;
    unsigned int nof_axes;
    std::vector<std::string> axis_names;
    std::vector<std::string> axis_units;
    std::vector<double> refvalue;
    std::vector<double> refpixel;
    std::vector<double> increment;
    std::vector<double> pc;

    /* Read the attributes from the HDF5 file */
    DAL::h5get_attribute( groupID, "COORDINATE_TYPE", coordinate_type );
    DAL::h5get_attribute( groupID, "NOF_AXES",        nof_axes );
    DAL::h5get_attribute( groupID, "AXIS_NAMES",      axis_names );
    DAL::h5get_attribute( groupID, "AXIS_UNITS",      axis_units );
    DAL::h5get_attribute( groupID, "CRVAL",           refvalue );
    DAL::h5get_attribute( groupID, "CRPIX",           refpixel );
    DAL::h5get_attribute( groupID, "CDELT",           increment );
    DAL::h5get_attribute( groupID, "PC",              pc );

    /* Store the retrieved values */
    if (getType(coordinate_type) == Coordinate::Linear) {
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
    } else {
    }
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  void LinearCoordinate::h5write (hid_t const &groupID)
  {
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE", name() );
    DAL::h5set_attribute( groupID, "NOF_AXES",        nofAxes_p );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",      axisNames_p );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",      axisUnits_p );
    DAL::h5set_attribute( groupID, "CRPIX",           refPixel_p );
    DAL::h5set_attribute( groupID, "CRVAL",           refValue_p );
    DAL::h5set_attribute( groupID, "CDELT",           increment_p );
    DAL::h5set_attribute( groupID, "PC",              pc_p );
  }
  
} // Namespace DAL -- end
