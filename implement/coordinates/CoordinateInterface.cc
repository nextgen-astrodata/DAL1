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
  //  Parameter access
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                 setAxisNames
  
  bool CoordinateInterface::setAxisNames (std::vector<std::string> const &axisNames)
  {
    bool status (true);
    
    if (axisNames.size() == nofAxes_p) {
      axisNames_p = axisNames;
    } else {
      std::cerr << "[CoordinateInterface::setAxisNames]"
		<< " Error in length of input vector!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 setAxisUnits
  
  bool CoordinateInterface::setAxisUnits (std::vector<std::string> const &axisUnits)
  {
    bool status (true);
    
    if (axisUnits.size() == nofAxes_p) {
      axisUnits_p = axisUnits;
    } else {
      std::cerr << "[CoordinateInterface::setAxisUnits]"
		<< " Error in length of input vector!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                  setRefValue

  /*!
    \param refValue -- Reference value (CRVAL).
    \return status  -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool CoordinateInterface::setRefValue (std::vector<double> const &refValue)
  {
    bool status (true);
    
    switch (storageType_p.type()) {
    case Coordinate::DIRECTION:
    case Coordinate::LINEAR:
      {
	if (refValue.size() == nofAxes_p) {
	  refValue_p = refValue;
	} else {
	  std::cerr << "[CoordinateInterface::setRefValue]"
		    << " Error in length of input vector!"
		    << std::endl;
	  status = false;
	}
      }
      break;
    case Coordinate::TABULAR:
      {
	if (!worldValues_p.empty()) {
	  refValue_p.resize(1);
	  refValue_p[0] = worldValues_p[0];
	} else {
	  status = false;
	}
      }
      break;
    default:
      {
	std::cerr << "[CoordinateInterface::setRefValue]"
		  << " Reference value not defined for storage type "
		  << storageType_p.name() << "!" 
		  << std::endl;
	status = false;
      }
      break;
    };
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                  setRefPixel

  /*!
    \param refPixel -- Reference pixel (CRPIX).
    \return status  -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool CoordinateInterface::setRefPixel (std::vector<double> const &refPixel)
  {
    bool status (true);
    
    switch (storageType_p.type()) {
    case Coordinate::DIRECTION:
    case Coordinate::LINEAR:
      {
	if (refPixel.size() == nofAxes_p) {
	  refPixel_p = refPixel;
	} else {
	  std::cerr << "[CoordinateInterface::setRefPixel]"
		    << " Error in length of input vector!"
		    << std::endl;
	  status = false;
	}
      }
      break;
    case Coordinate::TABULAR:
      {
	if (!pixelValues_p.empty()) {
	  refPixel_p.resize(1);
	  refPixel_p[0] = pixelValues_p[0];
	} else {
	  status = false;
	}
      }
      break;
    default:
      {
	std::cerr << "[CoordinateInterface::setRefPixel]"
		  << " Reference pixel not defined for storage type "
		  << storageType_p.name() << "!" 
		  << std::endl;
	status = false;
      }
      break;
    };
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                 setIncrement
  
  /*!
    \param increment -- Coordinate axis increment (CDELT).
    \return status   -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool CoordinateInterface::setIncrement (std::vector<double> const &increment)
  {
    bool status (true);
    
    switch (storageType_p.type()) {
    case Coordinate::DIRECTION:
    case Coordinate::LINEAR:
      {
	if (increment.size() == nofAxes_p) {
	  increment_p = increment;
	} else {
	  std::cerr << "[CoordinateInterface::setIncrement]"
		    << " Error in length of input vector!"
		    << std::endl;
	  status = false;
	}
      }
      break;
    case Coordinate::TABULAR:
      {
	/* Minimum of two pixel values required for interpolation! */
	if (pixelValues_p.size() < 2) {
	  status = false;
	} else {
	  increment_p.resize(1);
	  for (size_t n=1; n<pixelValues_p.size(); ++n) {
	    increment_p[0] += pixelValues_p[n]-pixelValues_p[n-1];
	  }
	  increment_p[0] /= pixelValues_p.size()-1;
	}
      }
      break;
    default:
      {
	std::cerr << "[CoordinateInterface::setIncrement]"
		  << " Reference pixel not defined for storage type "
		  << storageType_p.name() << "!" 
		  << std::endl;
	status = false;
      }
      break;
    };
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void CoordinateInterface::summary (std::ostream &os)
  {
    os << "[CoordinateInterface] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type  = " << type() << " / " <<  name() << std::endl;
    os << "-- Storage type     = " << storageType_p.type()
       << " / " <<  storageType_p.name() << std::endl;
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
    \param coord       -- Type of coordinate.
    \param nofAxes     -- The number of coordinate axes.
    \parma storageType -- Storage type of the coordinate. i.e. container
           internally used to represent the coordinate.
  */
  void CoordinateInterface::init (DAL::Coordinate const &coord,
				  unsigned int const &nofAxes,
				  DAL::Coordinate const &storageType)
  {
    /* Initialize the size of the internal arrays */

    axisNames_p.clear();
    axisUnits_p.clear();
    refValue_p.clear();
    refPixel_p.clear();
    increment_p.clear();
    pc_p.clear();
    pixelValues_p.clear();
    worldValues_p.clear();
    
    /* Initialize internal variables storing coordinate parameters */

    coord_p       = coord;
    storageType_p = storageType;

    if (nofAxes > 0) {
      // set the number of coordinate axes
      nofAxes_p = nofAxes;
      // Adjust the size of the internal arrays
      axisNames_p.resize(nofAxes);
      axisUnits_p.resize(nofAxes);
      refValue_p.resize(nofAxes);
      refPixel_p.resize(nofAxes);
      increment_p.resize(nofAxes);
      // Fill in default values for the WCS parameters
      for (unsigned int n(0); n<nofAxes; ++n) {
	axisNames_p[n] = "UNDEFINED";
	axisUnits_p[n] = "UNDEFINED";
	refValue_p[n]  = 0.0;
	refPixel_p[n]  = 0.0;
	increment_p[n] = 0.0;
      }
      // Transformation is identity matrix
      DAL::IdentityMatrix (pc_p,nofAxes);
    } else {
      // set the number of coordinate axes
      nofAxes_p = 0;
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
    attributes_p.insert("STORAGE_TYPE");
    attributes_p.insert("NOF_COORDINATES");
    attributes_p.insert("AXIS_NAMES");
    attributes_p.insert("AXIS_UNITS");
    attributes_p.insert("REFERENCE_VALUE");
    attributes_p.insert("REFERENCE_PIXEL");
    attributes_p.insert("INCREMENT");
    attributes_p.insert("PC");

    if (storageType_p.type() == Coordinate::TABULAR) {
      attributes_p.insert("PIXEL_VALUES");
      attributes_p.insert("WORLD_VALUES");
    }
  }
  
} // Namespace DAL -- end
