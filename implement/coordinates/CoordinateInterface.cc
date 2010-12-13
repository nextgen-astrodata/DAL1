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
  //  Operators
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         copy

  template <class T>
  void CoordinateInterface<T>::copy (CoordinateInterface const &other)
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

    /* Coordinate values along the pixel axis */
    if (pixelValues_p.empty()) {
      pixelValues_p.clear();
    } else {
      pixelValues_p.resize(other.pixelValues_p.size());
      pixelValues_p = other.pixelValues_p;
    }

    /* Coordinate values along the world axis */
    if (worldValues_p.empty()) {
      worldValues_p.clear();
    } else {
      worldValues_p.resize(other.worldValues_p.size());
      worldValues_p = other.worldValues_p;
    }
  }

  // ============================================================================
  //
  //  Parameter access
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                 setAxisNames
  
  template <class T>
  bool CoordinateInterface<T>::setAxisNames (std::vector<std::string> const &axisNames)
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
  
  template <class T>
  bool CoordinateInterface<T>::setAxisUnits (std::vector<std::string> const &axisUnits)
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
  template <class T>
  bool CoordinateInterface<T>::setRefValue (std::vector<T> const &refValue)
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
  template <class T>
  bool CoordinateInterface<T>::setRefPixel (std::vector<double> const &refPixel)
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
  template <class T>
  bool CoordinateInterface<T>::setIncrement (std::vector<double> const &increment)
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
  //                                                                        setPc
  
  /*!
    \param pc -- The transformation matrix, in row-wise ordering, e.g.
           [00,01,10,11]
    \return status   -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  template <class T>
  bool CoordinateInterface<T>::setPc (std::vector<double> const &pc)
  {
    bool status (true);

    switch (storageType_p.type()) {
    case Coordinate::DIRECTION:
    case Coordinate::LINEAR:
      {
	// Get the number expected matrix elements
	unsigned int nelem = nofAxes_p*nofAxes_p;
	// Check shape of the provided matrix
	if (pc.size() == nelem) {
	  pc_p = pc;
	} else {
	  std::cerr << "[CoordinateInterface::setPc]"
		    << " Error in number of matrix elements!"
		    << std::endl;
	  status = false;
	}
      }
      break;
    case Coordinate::TABULAR:
      {
	pc_p.resize(1);
	pc_p[0] = 1.0;
      }
      break;
    default:
      {
	std::cerr << "[CoordinateInterface::setPc]"
		  << " Trnasformation matrix not defined for storage type "
		  << storageType_p.name() << "!" 
		  << std::endl;
	status = false;
      }
      break;
    };
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                               setPixelValues

  template <class T>
  bool CoordinateInterface<T>::setPixelValues (std::vector<double> const &values)
  {
    bool status (true);
    unsigned int nelem = values.size();

    if (pixelValues_p.empty()) {
      if (worldValues_p.empty()) {
	/* If both pixel and world values are not defined yet, we can simply
	   accept the provided values, as there is no further reference. */
	pixelValues_p.resize(nelem);
	pixelValues_p = values;
      } else {
	/* If the world axis values have been set already, enforce match in the
	   size of the arrays. */
	if (nelem = worldValues_p.size()) {
	  pixelValues_p.resize(nelem);
	  pixelValues_p = values;
	} else {
	  /* Reject input values if number of elements does not match that of
	     the already assigned world axis values. */
	  std::cerr << "[CoordinateInterface::setPixelValues]"
		    << " Wrong number of elements in the provided array!"
		    << std::endl;
	  status = false;
	}
      }
    } else {
      if (nelem = pixelValues_p.size()) {
	pixelValues_p = values;
      } else {
	std::cerr << "[CoordinateInterface::setPixelValues]"
		  << " Wrong number of elements in the provided array!"
		  << std::endl;
	status = false;
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                               setWorldValues

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
  template <class T>
  void CoordinateInterface<T>::init (DAL::Coordinate const &coord,
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
	refValue_p[n]  = 0;
	refPixel_p[n]  = 0;
	increment_p[n] = 0;
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

  template <class T>
  void CoordinateInterface<T>::setAttributes ()
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
      attributes_p.insert("AXIS_VALUES_PIXEL");
      attributes_p.insert("AXIS_VALUES_WORLD");
    }
  }
  
} // Namespace DAL -- end
