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

#include <TabularCoordinate.h>

namespace DAL {  // Namespace DAL -- begin

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                            TabularCoordinate
  
  TabularCoordinate::TabularCoordinate ()
    : Coordinate(Coordinate::Tabular,
		 1)
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                            TabularCoordinate
  
  TabularCoordinate::TabularCoordinate (std::vector<std::string> const &axisNames,
                                        std::vector<std::string> const &axisUnits,
                                        std::vector<double> const &pixelValues,
                                        std::vector<double> const &worldValues)
      : Coordinate(Coordinate::Tabular,
                   1)
  {
    setAxisNames (axisNames);
    setAxisUnits (axisUnits);
    setAxisValues (pixelValues,
                   worldValues);
  }

  //_____________________________________________________________________________
  //                                                            TabularCoordinate

  TabularCoordinate::TabularCoordinate (TabularCoordinate const &other)
      : Coordinate (other)
  {
    copy (other);
  }

  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  TabularCoordinate::~TabularCoordinate ()
  {
    destroy();
  }

  void TabularCoordinate::destroy ()
  {}

  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  TabularCoordinate& TabularCoordinate::operator= (TabularCoordinate const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  void TabularCoordinate::copy (TabularCoordinate const &other)
  {
    Coordinate::copy (other);
    
    pixelValues_p.resize(other.pixelValues_p.size());
    worldValues_p.resize(other.worldValues_p.size());
    
    pixelValues_p = other.pixelValues_p;
    worldValues_p = other.worldValues_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                               setPixelValues

  bool TabularCoordinate::setPixelValues (std::vector<double> const &pixelValues)
  {
    bool status (true);

    if (pixelValues.size() == pixelValues_p.size()) {
      pixelValues_p = pixelValues;
    }
    else {
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                               setWorldValues

  bool TabularCoordinate::setWorldValues (std::vector<double> const &worldValues)
  {
    bool status (true);

    if (worldValues.size() == worldValues_p.size()) {
      worldValues_p = worldValues;
    }
    else {
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                setAxisValues

  bool TabularCoordinate::setAxisValues (std::vector<double> const &pixelValues,
                                         std::vector<double> const &worldValues)
  {
    bool status (true);

    if (pixelValues.size() == worldValues.size()) {
      // adjust array sizes
      pixelValues_p.resize(pixelValues.size());
      worldValues_p.resize(worldValues.size());
      // copy values
      pixelValues_p = pixelValues;
      worldValues_p = worldValues;
    }
    else {
      status = false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  void TabularCoordinate::summary (std::ostream &os)
  {
    os << "[TabularCoordinate] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type       = " << type() << " / " <<  name() << std::endl;
    os << "-- nof. axes             = " << nofAxes_p      << std::endl;
    os << "-- World axis names      = " << axisNames_p    << std::endl;
    os << "-- World axis units      = " << axisUnits_p    << std::endl;
//     os << "-- Reference value       = " << refValue_p     << std::endl;
//     os << "-- Reference pixel       = " << refPixel_p     << std::endl;
//     os << "-- Increment             = " << increment_p    << std::endl;
//     os << "-- Transformation matrix = " << pc_p           << std::endl;
    os << "-- Pixel values          = " << pixelValues_p  << std::endl;
    os << "-- World values          = " << worldValues_p  << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

#ifdef HAVE_HDF5

  //_____________________________________________________________________________
  //                                                                       h5read

  void TabularCoordinate::h5read (hid_t const &groupID)
  {
    std::string coordinateTypeName;

    DAL::h5get_attribute( groupID, "COORDINATE_TYPE",  coordinateTypeName );
    DAL::h5get_attribute( groupID, "NOF_AXES",         nofAxes_p );
    DAL::h5get_attribute( groupID, "AXIS_NAMES",       axisNames_p );
    DAL::h5get_attribute( groupID, "AXIS_UNITS",       axisUnits_p );
//     DAL::h5get_attribute( groupID, "INCREMENT",        increment_p );
    DAL::h5get_attribute( groupID, "REFERENCE_PIXEL",  refPixel_p );
    DAL::h5get_attribute( groupID, "REFERENCE_VALUE",  refValue_p );
  }

  //_____________________________________________________________________________
  //                                                                       h5read

  void TabularCoordinate::h5read (hid_t const &locationID,
                                  std::string const &name)
  {
    hid_t groupID (0);

    groupID = H5Gopen1 (locationID,
                        name.c_str());

    if (groupID) {
      h5read (groupID);
    }
    else {
      std::cerr << "[TabularCoordinate::h5read] Error opening group "
		<< name
		<< std::endl;
    }
    
    H5Gclose (groupID);
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  void TabularCoordinate::h5write (hid_t const &groupID)
  {
    DAL::h5set_attribute( groupID, "COORDINATE_TYPE",  name() );
    DAL::h5set_attribute( groupID, "NOF_AXES",         nofAxes_p );
    DAL::h5set_attribute( groupID, "AXIS_NAMES",       axisNames_p );
    DAL::h5set_attribute( groupID, "AXIS_UNITS",       axisUnits_p );
//     DAL::h5set_attribute( groupID, "REFERENCE_PIXEL",  refPixel_p );
//     DAL::h5set_attribute( groupID, "REFERENCE_VALUE",  refValue_p );
//     DAL::h5set_attribute( groupID, "INCREMENT",        increment_p );
//     DAL::h5set_attribute( groupID, "PC",               pc_p );
    DAL::h5set_attribute( groupID, "PIXEL_VALUES",     pixelValues_p );
    DAL::h5set_attribute( groupID, "WORLD_VALUES",     worldValues_p );
  }

  //_____________________________________________________________________________
  //                                                                      h5write

  void TabularCoordinate::h5write (hid_t const &locationID,
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

#endif

  //_____________________________________________________________________________
  //                                                             exportCoordinate

#ifdef HAVE_CASA
  void TabularCoordinate::exportCoordinate (casa::TabularCoordinate &coord)
  {
    unsigned int nelem = pixelValues_p.size();
    casa::Vector<double> pixel (nelem);
    casa::Vector<double> world (nelem);

    for (unsigned int n(0); n<nelem; ++n) {
      pixel(n) = pixelValues_p[n];
      world(n) = worldValues_p[n];
    }

    coord = casa::TabularCoordinate (pixel,
				     world,
				     axisUnits_p[0],
				     axisNames_p[0]);
  }
#endif


} // Namespace DAL -- end
