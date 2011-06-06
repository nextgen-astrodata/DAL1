/***************************************************************************
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

#include <coordinates/DirectionCoordinate.h>

namespace DAL {   // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                          DirectionCoordinate
  
  DirectionCoordinate::DirectionCoordinate (std::string const &system,
					    std::string const &projection)
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
  {
    // Initialize the basic parameters
    init (system,
          projection);
    // Store WCS parameters
    setAxisNames (axisNames);
    setAxisUnits (axisUnits);
    setRefPixel  (refPixel);
    setIncrement (increment);
    setPc        (pc);
    setRefValue  (refValue);
  }
  
  //_____________________________________________________________________________
  //                                                          DirectionCoordinate
  
  /*!
    \param other -- Another DirectionCoordinate object from which to create this
           new one.
  */
  DirectionCoordinate::DirectionCoordinate (DirectionCoordinate const &other)
    : CoordinateBase<double>(other)
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
  {
    ;
  }

  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                    operator=

  DirectionCoordinate& DirectionCoordinate::operator= (DirectionCoordinate const &other)
  {
    if (this != &other)
      {
        destroy ();
        copy (other);
      }
    return *this;
  }

  //_____________________________________________________________________________
  //                                                                         copy
  
  void DirectionCoordinate::copy (DirectionCoordinate const &other)
  {
    /* Copy basic attributes */
    coord_p       = other.coord_p;
    nofAxes_p     = other.nofAxes_p;
    storageType_p = other.storageType_p;
    
    /* Resize internal arrays */
    attributes_p.clear();
    axisNames_p.resize(nofAxes_p);
    axisUnits_p.resize(nofAxes_p);
    refValue_p.resize(nofAxes_p);
    refPixel_p.resize(nofAxes_p);
    increment_p.resize(nofAxes_p);
    pc_p.resize(nofAxes_p*nofAxes_p);
    
    /* Copy the values */
    attributes_p = other.attributes_p;
    axisNames_p  = other.axisNames_p;
    axisUnits_p  = other.axisUnits_p;
    refValue_p   = other.refValue_p;
    refPixel_p   = other.refPixel_p;
    increment_p  = other.increment_p;
    pc_p         = other.pc_p;
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
    os << "-- Coordinate type          = " << type()
       << " / " <<  name() << std::endl;
    os << "-- Storage type             = " << storageType_p.type()
       << " / " <<  storageType_p.name() << std::endl;
    os << "-- nof. axes                = " << nofAxes_p        << std::endl;
    os << "-- World axis names         = " << axisNames_p      << std::endl;
    os << "-- World axis units         = " << axisUnits_p      << std::endl;
    os << "-- Reference value          = " << refValue_p       << std::endl;
    os << "-- Reference pixel          = " << refPixel_p       << std::endl;
    os << "-- Increment                = " << increment_p      << std::endl;
    os << "-- Transformation matrix    = " << pc_p             << std::endl;
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
  //                                                                         init

  void DirectionCoordinate::init (std::string const &system,
				  std::string const &projection)
  {
    CoordinateBase<double>::init (Coordinate::DIRECTION,
				  2,
				  Coordinate::DIRECTION);
    
    system_p          = system;
    projection_p      = projection;
    projectionParam_p = std::vector<double>(1,0.0);
    longpole_p        = 0.0;
    latpole_p         = 0.0;
  }
  
#ifdef DAL_WITH_HDF5
  
  //_____________________________________________________________________________
  //                                                                    read_hdf5
  
  void DirectionCoordinate::read_hdf5 (hid_t const &locationID,
				       std::string const &name)
  {
    hid_t groupID (0);
    
    groupID = H5Gopen (locationID,
		       name.c_str(),
		       H5P_DEFAULT);
    
    if (groupID)
      {
        read_hdf5 (groupID);
      }
    else
      {
        std::cerr << "[DirectionCoordinate::read_hdf5] Error opening group "
                  << name
                  << std::endl;
      }

    H5Gclose (groupID);
  }

  //_____________________________________________________________________________
  //                                                                    read_hdf5

  void DirectionCoordinate::read_hdf5 (hid_t const &groupID)
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
    HDF5Attribute::read (groupID, "COORDINATE_TYPE",  coordinate_type );
    HDF5Attribute::read (groupID, "NOF_AXES",         nof_axes );
    HDF5Attribute::read (groupID, "AXIS_NAMES",       axis_names );
    HDF5Attribute::read (groupID, "AXIS_UNITS",       axis_units );
    HDF5Attribute::read (groupID, "REFERENCE_VALUE",  refvalue );
    HDF5Attribute::read (groupID, "REFERENCE_PIXEL",  refpixel );
    HDF5Attribute::read (groupID, "INCREMENT",        increment );
    HDF5Attribute::read (groupID, "PC",               pc );
    HDF5Attribute::read (groupID, "SYSTEM",           refname );
    HDF5Attribute::read (groupID, "PROJECTION",       projection );
    HDF5Attribute::read (groupID, "PROJECTION_PARAM", param );
    HDF5Attribute::read (groupID, "LONGPOLE",         longpole );
    HDF5Attribute::read (groupID, "LATPOLE",          latpole );

    /* Store the retrieved values */
    if (DAL::Coordinate::getType(coordinate_type) == Coordinate::DIRECTION)
      {
        // basic parameters
        coord_p   = DAL::Coordinate::getType(coordinate_type);
        nofAxes_p = nof_axes;
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
      }
    else
      {
      }
  }

  //_____________________________________________________________________________
  //                                                                   write_hdf5

  void DirectionCoordinate::write_hdf5 (hid_t const &locationID,
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

  void DirectionCoordinate::write_hdf5 (hid_t const &groupID)
  {
    /* Basic common parameters */
    HDF5Attribute::write( groupID, "COORDINATE_TYPE",  name() );
    HDF5Attribute::write( groupID, "NOF_AXES",         nofAxes_p );
    HDF5Attribute::write( groupID, "AXIS_NAMES",       axisNames_p );
    HDF5Attribute::write( groupID, "AXIS_UNITS",       axisUnits_p );
    HDF5Attribute::write( groupID, "REFERENCE_PIXEL",  refPixel_p );
    HDF5Attribute::write( groupID, "REFERENCE_VALUE",  refValue_p );
    HDF5Attribute::write( groupID, "INCREMENT",        increment_p );
    HDF5Attribute::write( groupID, "PC",               pc_p );
    /* Add-on for direction coordinate */
    HDF5Attribute::write( groupID, "SYSTEM",           system_p );
    HDF5Attribute::write( groupID, "PROJECTION",       projection_p );
    HDF5Attribute::write( groupID, "PROJECTION_PARAM", projectionParam_p );
    HDF5Attribute::write( groupID, "LONGPOLE",         longpole_p );
    HDF5Attribute::write( groupID, "LATPOLE",          latpole_p );
  }
    
#endif
    
    //___________________________________________________________________________
    //                                                           exportCoordinate
  
#ifdef DAL_WITH_CASA
  void DirectionCoordinate::exportCoordinate (casa::DirectionCoordinate &coord)
  {
    casa::MDirection::Types system    = Coordinate::systemType(system_p);
    casa::Projection::Type projection = Coordinate::projectionType(projection_p);
    casa::Vector<casa::Quantum<double> > refValue (nofAxes_p);
    casa::Vector<casa::Quantum<double> > increment (nofAxes_p);
    casa::Matrix<double> xform (nofAxes_p,nofAxes_p);
    
    /* Copy data from internal storage to casa array objects */
    
    refValue(0) = casa::Quantum<double>(refValue_p[0],
					casa::String(axisUnits_p[0]));
    refValue(1) = casa::Quantum<double>(refValue_p[1],
					casa::String(axisUnits_p[1]));
    increment(0) = casa::Quantum<double>(increment_p[0],
					 casa::String(axisUnits_p[0]));
    increment(1) = casa::Quantum<double>(increment_p[1],
					 casa::String(axisUnits_p[1]));
    xform(0,0) = pc_p[0];
    xform(0,1) = pc_p[1];
    xform(1,0) = pc_p[2];
    xform(1,1) = pc_p[3];
    
    coord = casa::DirectionCoordinate (system,
				       casa::Projection(projection),
				       refValue(0),
				       refValue(1),
				       increment(0),
				       increment(1),
				       xform,
				       refPixel_p[0],
				       refPixel_p[1]);
  }
#endif
  
  
} // Namespace DAL -- end
