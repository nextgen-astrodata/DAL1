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

#include <coordinates/StokesCoordinate.h>

namespace DAL {  // Namespace DAL -- begin

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

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
  
  StokesCoordinate::StokesCoordinate ()
  {
    std::vector<DAL::Stokes::Component> stokes (1,DAL::Stokes::I);
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
    : CoordinateBase<std::string> (other)
  {
    *this = other;
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
  
  /*!
    \param other -- Another StokesCoordinate object from which to make a copy.
  */
  StokesCoordinate& StokesCoordinate::operator= (StokesCoordinate const &other)
  {
    if (this != &other) {
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
      /* Coordinate values along the pixel axis */
      if (itsPixelValues.empty()) {
	itsPixelValues.clear();
      } else {
	itsPixelValues.resize(other.itsPixelValues.size());
	itsPixelValues = other.itsPixelValues;
      }
      /* Coordinate values along the world axis */
      if (itsWorldValues.empty()) {
	itsWorldValues.clear();
      } else {
	itsWorldValues.resize(other.itsWorldValues.size());
	itsWorldValues = other.itsWorldValues;
      }
    }
    // copy variables handles by this class
    init (other.itsValues);
    // Return pointer to object
    return *this;
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
    os << "-- Coordinate type  = " << type() << " / " <<  name() << std::endl;
    os << "-- Storage type     = " << storageType_p.type()
       << " / " <<  storageType_p.name() << std::endl;
    os << "-- nof. axes        = " << nofAxes_p        << std::endl;
    os << "-- World axis names = " << axisNames_p      << std::endl;
    os << "-- World axis units = " << axisUnits_p      << std::endl;
    os << "-- Reference value  = " << refValue_p       << std::endl;
    os << "-- Reference pixel  = " << refPixel_p       << std::endl;
    os << "-- Increment        = " << increment_p      << std::endl;
    os << "-- PC               = " << pc_p             << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void StokesCoordinate::init (std::vector<DAL::Stokes::Component> const &values)
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
    // store the input list of Stokes values
    itsValues.resize (values.size());
    itsValues = values;

    /* Initialize the size of the internal arrays */
    
    axisNames_p.clear();
    axisUnits_p.clear();
    refValue_p.clear();
    refPixel_p.clear();
    increment_p.clear();
    pc_p.clear();
    itsPixelValues.clear();
    itsWorldValues.clear();
    
    /* Initialize internal variables storing coordinate parameters */

    nofAxes_p     = 1;
    coord_p       = DAL::Coordinate(Coordinate::STOKES);
    storageType_p = DAL::Coordinate(Coordinate::TABULAR);
    
    if (nofAxes_p > 0) {
      // Adjust the size of the internal arrays
      axisNames_p.resize(nofAxes_p);
      axisUnits_p.resize(nofAxes_p);
      refValue_p.resize(nofAxes_p);
      refPixel_p.resize(nofAxes_p);
      increment_p.resize(nofAxes_p);
      // Fill in default values for the WCS parameters
      for (unsigned int n(0); n<nofAxes_p; ++n) {
	axisNames_p[n] = "UNDEFINED";
	axisUnits_p[n] = "UNDEFINED";
	refValue_p[n]  = "UNDEFINED";
	refPixel_p[n]  = 0;
	increment_p[n] = 0;
      }
      // Transformation is identity matrix
      DAL::IdentityMatrix (pc_p,nofAxes_p);
    } else {
      // set the number of coordinate axes
      nofAxes_p = 0;
    }
    
    /* Set up the basic set of attributes */
    setAttributes ();

  }
  
  //_____________________________________________________________________________
  //                                                                  stokesTypes
  
  std::vector<Stokes::Component> StokesCoordinate::stokesTypes ()
  {
    unsigned int nelem = itsValues.size();
    std::vector<DAL::Stokes::Component> val (nelem);
    
    for (unsigned int n(0); n<nelem; ++n) {
      val[n] = itsValues[n].type();
    }
    
    return val;
  }
  
  //_____________________________________________________________________________
  //                                                                  stokesNames
  
  std::vector<std::string> StokesCoordinate::stokesNames ()
  {
    unsigned int nelem = itsValues.size();
    std::vector<std::string> val (nelem);

    for (unsigned int n(0); n<nelem; ++n) {
      val[n] = itsValues[n].name();
    }
    
    return val;
  }
  
  // ============================================================================
  //
  //  Methods (using HDF5)
  //
  // ============================================================================

#ifdef DAL_WITH_HDF5

  //_____________________________________________________________________________
  //                                                                    read_hdf5

  /*!
    \param locationID -- Identifier of the HDF5 object to which the coordinate
           group is attached.
    \param name       -- Name of the HDF5 group containing the coordinate
           parameters.
  */
  void StokesCoordinate::read_hdf5 (hid_t const &locationID,
				    std::string const &name)
  {
    hid_t groupID (0);
    
    groupID = H5Gopen (locationID,
		       name.c_str(),
		       H5P_DEFAULT);
    
    if (groupID) {
      read_hdf5 (groupID);
    }
    else {
      std::cerr << "[StokesCoordinate::read_hdf5] Error opening group "
		<< name
		<< std::endl;
    }
    
    H5Gclose (groupID);
  }
  
  //_____________________________________________________________________________
  //                                                                    read_hdf5
  
  /*!
    \param groupID -- Identifier of the HDF5 group containing the coordinate 
           parameters.
  */
  void StokesCoordinate::read_hdf5 (hid_t const &groupID)
  {
    std::string coordinate_type;
    unsigned int nof_axes;
    std::vector<double> pixel;
    std::vector<double> world;

    /* Read the attributes from the HDF5 file */
    HDF5Attribute::read (groupID, "COORDINATE_TYPE",   coordinate_type );
    HDF5Attribute::read (groupID, "NOF_AXES",          nof_axes );
    HDF5Attribute::read (groupID, "AXIS_VALUES_PIXEL", pixel    );
    HDF5Attribute::read (groupID, "AXIS_VALUES_WORLD", world    );
    
    /* Store the retrieved values */
    if (DAL::Coordinate::getType(coordinate_type) == DAL::Coordinate::STOKES) {
      // basic parameters
      coord_p   = DAL::Coordinate::getType(coordinate_type);
      nofAxes_p = nof_axes;
    }
    else {
      std::cerr << "[StokesCoordinate::read_hdf5]"
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
    HDF5Attribute::write (groupID, "COORDINATE_TYPE",  name() );
    HDF5Attribute::write (groupID, "NOF_AXES",         nofAxes_p );
    HDF5Attribute::write (groupID, "AXIS_NAMES",       axisNames_p );
    HDF5Attribute::write (groupID, "AXIS_UNITS",       axisUnits_p );
    HDF5Attribute::write (groupID, "REFERENCE_PIXEL",  refPixel_p );
    HDF5Attribute::write (groupID, "REFERENCE_VALUE",  refValue_p );
    HDF5Attribute::write (groupID, "INCREMENT",        increment_p );
    HDF5Attribute::write (groupID, "PC",               pc_p );
  }

#endif
  
  // ============================================================================
  //
  //  Methods (using casacore)
  //
  // ============================================================================
  
#ifdef DAL_WITH_CASA

  //_____________________________________________________________________________
  //                                                                    read_casa

  bool StokesCoordinate::read_casa (casa::StokesCoordinate const &coord)
  {
    bool status (true);

    nofAxes_p = coord.nPixelAxes();

    return status;
  }

  //_____________________________________________________________________________
  //                                                                   write_casa

  bool StokesCoordinate::write_casa (casa::StokesCoordinate &coord)
  {
    bool status (true);
    casa::Vector<casa::Int> components (1,casa::Stokes::I);

    coord = casa::StokesCoordinate(components);

    return status;
  }
#endif

} // Namespace DAL -- end
