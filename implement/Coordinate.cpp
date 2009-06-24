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

#include <Coordinate.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   Coordinate

  Coordinate::Coordinate ()
  {
    coordinateType_p = Coordinate::NONE;
    nofAxes_p        = 1;
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                   Coordinate

  /*!
    \param coordinateType -- Type of coordinate for which the object is being
           created.
    \param nofAxes        -- Number of coordinate axes.
  */
  Coordinate::Coordinate (Coordinate::Type const &coordinateType,
			  double const &nofAxes)
  {
    coordinateType_p = coordinateType;
    nofAxes_p        = nofAxes;
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                   Coordinate
  
  Coordinate::Coordinate (Coordinate::Type const &coordinateType,
			  double const &nofAxes,
			  std::vector<std::string> const &axisNames,
			  std::vector<std::string> const &axisUnits,
			  std::vector<double> const &refValue,
			  std::vector<double> const &refPixel,
			  std::vector<double> const &increment,
			  std::vector<double> const &pc)
  {
    // store basic variables
    coordinateType_p = coordinateType;
    nofAxes_p        = nofAxes;
    // store WCS parameters
    setAxisNames (axisNames);
    setAxisUnits (axisUnits);
    setRefValue  (refValue);
    setRefPixel  (refPixel);
    setIncrement (increment);
    setPc        (pc);
  }
  
  //_____________________________________________________________________________
  //                                                                   Coordinate
  
  /*!
    \param other -- Another Coordinate object from which to create this new
           one.
  */
  Coordinate::Coordinate (Coordinate const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  Coordinate::~Coordinate ()
  {
    destroy();
  }
  
  void Coordinate::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  Coordinate& Coordinate::operator= (Coordinate const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  void Coordinate::copy (Coordinate const &other)
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

  std::string Coordinate::name ()
  {
    std::string coordinateName;

    switch (coordinateType_p) {
    case DAL::Coordinate::Direction:
      coordinateName="Direction";
      break;
    case DAL::Coordinate::Frequency:
      coordinateName="Frequency";
      break;
    case DAL::Coordinate::Linear:
      coordinateName="Linear";
      break;
    case DAL::Coordinate::Stokes:
      coordinateName="Stokes";
      break;
    case DAL::Coordinate::Tabular:
      coordinateName="Tabular";
      break;
    case DAL::Coordinate::NONE:
      coordinateName="NONE";
      break;
    };

    return coordinateName;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary

  void Coordinate::summary (std::ostream &os)
  {
    os << "[Coordinate] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type  = " << type()         << std::endl;
    os << "-- Coordinate name  = " << name()         << std::endl;
    os << "-- nof. axes        = " << nofAxes_p      << std::endl;
    os << "-- World axis names = " << axisNames_p    << std::endl;
    os << "-- World axis units = " << axisUnits_p    << std::endl;
    os << "-- Reference value  = " << refValue_p     << std::endl;
    os << "-- Reference pixel  = " << refPixel_p     << std::endl;
    os << "-- Increment        = " << increment_p    << std::endl;
  }
  
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init

  void Coordinate::init ()
  {
    /* Resize internal arrays */
    axisNames_p.resize(nofAxes_p);
    axisUnits_p.resize(nofAxes_p);
    refValue_p.resize(nofAxes_p);
    refPixel_p.resize(nofAxes_p);
    increment_p.resize(nofAxes_p);

    /* Assign values */
    for (unsigned int n(0); n<nofAxes_p; n++) {
      axisNames_p[n] = "UNDEFINED";
      axisUnits_p[n] = "UNDEFINED";
      refValue_p[n]  = 0;
      refPixel_p[n]  = 0;
      increment_p[n] = 0;
    }
  };
  
  //_____________________________________________________________________________
  //                                                                      h5write

  void Coordinate::h5write (hid_t const &locationID)
  {
    DAL::h5set_attribute( locationID, "COORDINATE_TYPE", name()    );
    DAL::h5set_attribute( locationID, "NOF_AXES",        nofAxes_p );
    DAL::h5set_attribute( locationID, "AXIS_NAMES",      axisNames_p );
    DAL::h5set_attribute( locationID, "AXIS_UNITS",      axisUnits_p );
    DAL::h5set_attribute( locationID, "CRPIX",           refPixel_p );
    DAL::h5set_attribute( locationID, "CRVAL",           refValue_p );
    DAL::h5set_attribute( locationID, "CDELT",           increment_p );
    DAL::h5set_attribute( locationID, "PC",              pc_p );
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write

  void Coordinate::h5write (hid_t const &locationID,
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
  
} // Namespace DAL -- end
