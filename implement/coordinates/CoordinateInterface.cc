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

#include <CoordinateInterface.h>

namespace DAL {
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   Coordinate
  
  CoordinateInterface::CoordinateInterface ()
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                          CoordinateInterface
  
  /*!
    \param other -- Another Coordinate object from which to create this new
           one.
  */
  CoordinateInterface::CoordinateInterface (CoordinateInterface const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  CoordinateInterface::~CoordinateInterface ()
  {
    destroy();
  }

  void CoordinateInterface::destroy ()
  {
    attributes_p.clear();
  }

  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                    operator=

  /*!
    \param other -- Another Coordinate object from which to make a copy.
  */
  CoordinateInterface& CoordinateInterface::operator= (CoordinateInterface const &other)
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

  void CoordinateInterface::copy (CoordinateInterface const &other)
  {
    /* Copy basic attributes */
    coord_p   = other.coord_p;
    nofAxes_p = other.nofAxes_p;

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
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void CoordinateInterface::summary (std::ostream &os)
  {
    os << "[CoordinateInterface] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type  = " << type() << " / " <<  name() << std::endl;
    os << "-- nof. axes        = " << nofAxes_p        << std::endl;
    os << "-- World axis names = " << axisNames_p      << std::endl;
    os << "-- World axis units = " << axisUnits_p      << std::endl;
    os << "-- Reference value  = " << refValue_p       << std::endl;
    os << "-- Reference pixel  = " << refPixel_p       << std::endl;
    os << "-- Increment        = " << increment_p      << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init

  /*!
    \param coord   -- Type of coordinate.
    \param nofAxes -- The number of coordinate axes.
  */
  void CoordinateInterface::init (DAL::Coordinate const &coord,
				  unsigned int const &nofAxes)
  {
    /* Initialize internal variables storing coordinate parameters */

    coord_p = coord;

    if (nofAxes <= 0) {
      // Set the number of coordinate axes
      nofAxes_p = 0;
      // Reset the size of the internal arrays
      axisNames_p.clear();
      axisUnits_p.clear();
      refValue_p.clear();
      refPixel_p.clear();
      increment_p.clear();
      pc_p.clear();
    } else {
      // set the number of coordinate axes
      nofAxes_p = nofAxes;
      // Adjust the size of the internal arrays
      axisNames_p.resize(nofAxes);
      axisUnits_p.resize(nofAxes);
      refValue_p.resize(nofAxes);
      refPixel_p.resize(nofAxes);
      increment_p.resize(nofAxes);
      // Fill in default values for the WCS parameters
      axisNames_p = std::vector<std::string> (nofAxes,"UNDEFINED");
      axisUnits_p = std::vector<std::string> (nofAxes,"UNDEFINED");
      refValue_p  = std::vector<double> (nofAxes,0.0);
      refPixel_p  = std::vector<double> (nofAxes,0.0);
      increment_p = std::vector<double> (nofAxes,0.0);

      std::cout << "-- Initializing PC ..." << std::endl;
      DAL::IdentityMatrix (pc_p,nofAxes);
    }

    /* Set up the basic set of attributes */
    setAttributes ();
  };

  //_____________________________________________________________________________
  //                                                                setAttributes

  void CoordinateInterface::setAttributes ()
  {
    /* clear data container */
    attributes_p.clear();

    /* set list of attributes common to all coordinates */
    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("COORDINATE_TYPE");
    attributes_p.insert("NOF_COORDINATES");
    attributes_p.insert("AXIS_NAMES");
    attributes_p.insert("AXIS_UNITS");
    attributes_p.insert("REFERENCE_VALUE");
    attributes_p.insert("REFERENCE_PIXEL");
    attributes_p.insert("INCREMENT");
    attributes_p.insert("PC");
  }

  // ============================================================================
  //
  //  Methods: HDF5 I/O
  //
  // ============================================================================

#ifdef HAVE_HDF5
  
  //_____________________________________________________________________________
  //                                                                      h5write

  void CoordinateInterface::h5write (hid_t const &locationID,
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
    h5write (groupID);
    // close the group after write
    H5Gclose (groupID);
  }

  //_____________________________________________________________________________
  //                                                                      h5write

  void CoordinateInterface::h5write (hid_t const &groupID)
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
