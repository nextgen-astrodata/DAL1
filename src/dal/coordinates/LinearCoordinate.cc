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

#include <coordinates/LinearCoordinate.h>

namespace DAL {  // Namespace DAL -- begin

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             LinearCoordinate
  
  LinearCoordinate::LinearCoordinate ()
  {
    init(1);
  }
  
  //_____________________________________________________________________________
  //                                                             LinearCoordinate
  
  /*!
    \param nofAxes   -- Number of coordinate axes.
  */
  LinearCoordinate::LinearCoordinate (unsigned int const &nofAxes)
  {
    init(nofAxes);
  }
  
  //_____________________________________________________________________________
  //                                                             LinearCoordinate
  
  /*!
    \param nofAxes   -- Number of coordinate axes.
    \param axisNames -- World axis names.
    \param axisUnits -- World axis units.
  */
  LinearCoordinate::LinearCoordinate (unsigned int const &nofAxes,
                                      std::vector<std::string> const &axisNames,
                                      std::vector<std::string> const &axisUnits)
  {
    init(nofAxes);
    setAxisNames (axisNames);
    setAxisUnits (axisUnits);
  }
  
  //_____________________________________________________________________________
  //                                                             LinearCoordinate
  
  /*!
    \param nofAxes   -- Number of coordinate axes.
    \param axisNames -- World axis names.
    \param axisUnits -- World axis units.
    \param refValue  -- Reference value.
    \param refPixel  -- Reference pixel.
    \param increment -- Coordinate axis increment.
    \param pc        -- Transformation matrix.
   */
  LinearCoordinate::LinearCoordinate (unsigned int const &nofAxes,
                                      std::vector<std::string> const &axisNames,
                                      std::vector<std::string> const &axisUnits,
                                      std::vector<double> const &refValue,
                                      std::vector<double> const &refPixel,
                                      std::vector<double> const &increment,
                                      std::vector<double> const &pc)
  {
    init(nofAxes);
    setAxisNames (axisNames);
    setAxisUnits (axisUnits);
    setRefValue (refValue);
    setRefPixel (refPixel);
    setIncrement (increment);
    setPc (pc);
  }
  
  //_____________________________________________________________________________
  //                                                             LinearCoordinate

  /*!
    \param other -- Another LinearCoordinate object from which to create this new
           one.
  */
  LinearCoordinate::LinearCoordinate (LinearCoordinate const &other)
    : CoordinateBase<double> (other)
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
  {
    ;
  }

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
  //  Parameter access
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary

  void LinearCoordinate::summary (std::ostream &os)
  {
    os << "[LinearCoordinate] Summary of internal parameters." << std::endl;
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

  void LinearCoordinate::init (unsigned int const &nofAxes)
  {
    /* Initialize base class; no further initialization required, as no additional
       parameters are getting defined here. */
    CoordinateBase<double>::init (Coordinate::LINEAR,
				  nofAxes,
				  Coordinate::LINEAR);
  }
  
#ifdef DAL_WITH_HDF5
  
  //_____________________________________________________________________________
  //                                                                    read_hdf5
  
  /*!
    \param locationID -- Identifier of the HDF5 object to which the coordinate
           group is attached.
    \param name       -- Name of the HDF5 group containing the coordinate
           parameters.
  */
  void LinearCoordinate::read_hdf5 (hid_t const &locationID,
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
      std::cerr << "[LinearCoordinate::read_hdf5] Error opening group "
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
  void LinearCoordinate::read_hdf5 (hid_t const &groupID)
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
    HDF5Attribute::read (groupID, "COORDINATE_TYPE",  coordinate_type );
    HDF5Attribute::read (groupID, "NOF_AXES",         nof_axes );
    HDF5Attribute::read (groupID, "AXIS_NAMES",       axis_names );
    HDF5Attribute::read (groupID, "AXIS_UNITS",       axis_units );
    HDF5Attribute::read (groupID, "REFERENCE_VALUE",  refvalue );
    HDF5Attribute::read (groupID, "REFERENCE_PIXEL",  refpixel );
    HDF5Attribute::read (groupID, "INCREMENT",        increment );
    HDF5Attribute::read (groupID, "PC",               pc );

    /* Store the retrieved values */
    if (Coordinate::getType(coordinate_type) == Coordinate::LINEAR) {
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
    }
    else {
      std::cerr << "[LinearCoordinate::read_hdf5]"
		<< " Encountered coordinate does not match expected type!"
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   write_hdf5
  
  void LinearCoordinate::write_hdf5 (hid_t const &locationID,
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

  void LinearCoordinate::write_hdf5 (hid_t const &groupID)
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
  
  //_____________________________________________________________________________
  //                                                                    read_casa

#ifdef DAL_WITH_CASA
  void LinearCoordinate::read_casa (casa::LinearCoordinate const &coord) 
  {
    /* Get the number of axes associated with the coordinate */
    nofAxes_p = (unsigned int)coord.worldAxisNames().nelements();
    /* Adjust the size of the arrays storing the coordinate axis parameters */
    axisNames_p.resize(nofAxes_p);
    axisUnits_p.resize(nofAxes_p);
    refValue_p.resize(nofAxes_p);
    refPixel_p.resize(nofAxes_p);
    increment_p.resize(nofAxes_p);
    pc_p.resize(nofAxes_p*nofAxes_p);

    casa::Vector<casa::String> names = coord.worldAxisNames();
    casa::Vector<casa::String> units = coord.worldAxisUnits();
    casa::Vector<double> crval       = coord.referenceValue();
    casa::Vector<double> crpix       = coord.referencePixel();
    casa::Vector<double> cdelt       = coord.increment();
    casa::Matrix<double> pc          = coord.linearTransform();

    unsigned int i (0);
    for (unsigned int n(0); n<nofAxes_p; n++) {
      axisNames_p[n] = names(n);
      axisUnits_p[n] = units(n);
      refValue_p[n]  = crval(n);
      refPixel_p[n]  = crpix(n);
      increment_p[n] = cdelt(n);
      for (unsigned int m(0); m<nofAxes_p; m++) {
	pc_p[i] = pc(n,m);
	i++;
      }
    }
  }
#endif
  
  //_____________________________________________________________________________
  //                                                                   write_casa
  
#ifdef DAL_WITH_CASA
  void LinearCoordinate::write_casa (casa::LinearCoordinate &coord) 
  {
    casa::Vector<casa::String> names (nofAxes_p);
    casa::Vector<casa::String> units (nofAxes_p);
    casa::Vector<double> crval (nofAxes_p);
    casa::Vector<double> cdelt (nofAxes_p);
    casa::Vector<double> crpix (nofAxes_p);
    casa::Matrix<double> pc (nofAxes_p,nofAxes_p);
    
    /* Copy data from internal storage to casa array objects */
    
    unsigned int i (0);
    for (unsigned int n(0); n<nofAxes_p; n++) {
      names(n) = axisNames_p[n];
      units(n) = axisUnits_p[n];
      crval(n) = refValue_p[n];
      crpix(n) = refPixel_p[n];
      cdelt(n) = increment_p[n];
      for (unsigned int m(0); m<nofAxes_p; m++) {
	pc(n,m) = pc_p[i];
	i++;
      }
    }
    
    coord = casa::LinearCoordinate (names,
				    units,
				    crval,
				    cdelt,
				    pc,
				    crpix);
  }
#endif
  
} // Namespace DAL -- end
