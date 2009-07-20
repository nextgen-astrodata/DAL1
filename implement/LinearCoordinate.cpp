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

#include <LinearCoordinate.h>

namespace DAL {  // Namespace DAL -- begin

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
    Coordinate::copy (other);
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void LinearCoordinate::summary (std::ostream &os)
  {
    os << "[LinearCoordinate] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type       = " << type() << " / " <<  name() << std::endl;
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

#ifdef HAVE_HDF5

  //_____________________________________________________________________________
  //                                                                       h5read

  /*!
    \param locationID -- Identifier of the HDF5 object to which the coordinate
           group is attached.
    \param name       -- Name of the HDF5 group containing the coordinate
           parameters.
  */
  void LinearCoordinate::h5read (hid_t const &locationID,
                                 std::string const &name)
  {
    hid_t groupID (0);

    groupID = H5Gopen1 (locationID,
                        name.c_str());

    if (groupID) {
      h5read (groupID);
    }
    else {
      std::cerr << "[LinearCoordinate::h5read] Error opening group "
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
    DAL::h5get_attribute( groupID, "COORDINATE_TYPE",  coordinate_type );
    DAL::h5get_attribute( groupID, "NOF_AXES",         nof_axes );
    DAL::h5get_attribute( groupID, "AXIS_NAMES",       axis_names );
    DAL::h5get_attribute( groupID, "AXIS_UNITS",       axis_units );
    DAL::h5get_attribute( groupID, "REFERENCE_VALUE",  refvalue );
    DAL::h5get_attribute( groupID, "REFERENCE_PIXEL",  refpixel );
    DAL::h5get_attribute( groupID, "INCREMENT",        increment );
    DAL::h5get_attribute( groupID, "PC",               pc );

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
    }
    else {
    }
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  void LinearCoordinate::h5write (hid_t const &locationID,
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

  void LinearCoordinate::h5write (hid_t const &groupID)
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
  
  //_____________________________________________________________________________
  //                                                             importCoordinate

#ifdef HAVE_CASA
  void LinearCoordinate::importCoordinate (casa::LinearCoordinate const &coord) 
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
  //                                                             exportCoordinate
  
#ifdef HAVE_CASA
  void LinearCoordinate::exportCoordinate (casa::LinearCoordinate &coord) 
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
