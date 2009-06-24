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
    return getName (coordinateType_p);
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
    pc_p.resize(nofAxes_p*nofAxes_p);

    refValue_p  = std::vector<double> (refValue_p.size(),0.0);
    refPixel_p  = std::vector<double> (refPixel_p.size(),0.0);
    increment_p = std::vector<double> (increment_p.size(),0.0);
    pc_p        = std::vector<double> (pc_p.size(),1.0);

    /* Assign values */
    for (unsigned int n(0); n<nofAxes_p; n++) {
      axisNames_p[n] = "UNDEFINED";
      axisUnits_p[n] = "UNDEFINED";
    }
  };

  //_____________________________________________________________________________
  //                                                                      getName

  std::string Coordinate::getName (Coordinate::Type const &type)
  {
    std::string coordinateName;
    
    switch (type) {
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
  //                                                                      getName

  Coordinate::Type Coordinate::getType (std::string const &name)
  {
    Coordinate::Type coordinateType (Coordinate::NONE);

    if (name == "Direction") {
      coordinateType = Coordinate::Direction;
    }
    else if (name == "Frequency") {
      coordinateType = Coordinate::Frequency;
    }
    else if (name == "Linear") {
      coordinateType = Coordinate::Linear;
    }
    else if (name == "Stokes") {
      coordinateType = Coordinate::Stokes;
    }
    else if (name == "Tabular") {
      coordinateType = Coordinate::Tabular;
    }

    return coordinateType;
  }

  //_____________________________________________________________________________
  //                                                                       h5read
  
  void Coordinate::h5read (hid_t const &locationID,
			   std::string const &name)
  {
    hid_t groupID (0);
    
    groupID = H5Gopen1 (locationID,
			name.c_str());
    
    if (groupID) {
      h5read (groupID);
    } else {
      std::cerr << "[Coordinate::h5read] Error opening group "
		<< name 
		<< std::endl;
    }
    
    H5Gclose (groupID);
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
