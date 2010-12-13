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

#ifndef COORDINATEINTERFACE_H
#define COORDINATEINTERFACE_H

/* DAL header files */
#include <dalCommon.h>
#include "Coordinate.h"

/* casacore header files */
#ifdef HAVE_CASA
#include <coordinates/Coordinates/DirectionCoordinate.h>
#endif

namespace DAL {   // Namespace DAL -- begin

  /*!
    \class CoordinateInterface

    \ingroup DAL
    \ingroup coordinates

    \brief Definition of common interface for coordinate classes

    \author Lars B&auml;hren

    \date 2009/06/23

    \test tCoordinateInterface.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>Representations of World Coordinates. LOFAR-USG-ICD-002.
    </ul>

    <h3>Synopsis</h3>

    This class defines the basic interface and data contents for a coordinate
    object as being part of a HDF5-based LOFAR data-set (most prominently an
    image).

    <h3>Example(s)</h3>

    As CoordinateInterface defines an abstract base class, it cannot be used
    directly.

  */
  template <class T> class CoordinateInterface {
    
  protected:

    // === Protected attributes ==================================================

    //! The type of coordinate
    DAL::Coordinate coord_p;
    //! Storage type for the coordinate
    DAL::Coordinate storageType_p;
    //! Attributes attached to the storage structure (typically HDF5)
    std::set<std::string> attributes_p;

    //! The number of coordinate axes
    unsigned int nofAxes_p;
    //! World axis names
    std::vector<std::string> axisNames_p;
    //! World axis units
    std::vector<std::string> axisUnits_p;
    //! Reference value (CRVAL)
    std::vector<T> refValue_p;
    //! Reference pixel (CRPIX)
    std::vector<double> refPixel_p;
    //! Coordinate axis increment (CDELT)
    std::vector<double> increment_p;
    //! Transformation matrix (PC)
    std::vector<double> pc_p;
    //! List of pixel values
    std::vector<double> pixelValues_p;
    //! List of world values
    std::vector<T> worldValues_p;
    
    // === Protected Methods ====================================================

    //___________________________________________________________________________
    //                                                                       copy
    //! Unconditional copying
    void copy (CoordinateInterface const &other)
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
    //! Initilize the internal set of parameters
    void init (DAL::Coordinate const &coord=DAL::Coordinate(),
	       unsigned int const &nofAxes=0,
	       DAL::Coordinate const &storageType=DAL::Coordinate());
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    CoordinateInterface () {
      init ();
    }
    //! Copy constructor
    CoordinateInterface (CoordinateInterface const &other) {
      copy (other);
    }
    
    // === Destruction ==========================================================
    
    //! Destructor
    virtual ~CoordinateInterface () {
      destroy();
    }
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    CoordinateInterface& operator= (CoordinateInterface const &other) {
      if (this != &other)
	{
	  destroy ();
	  copy (other);
	}
      return *this;
    }
    
    // === Parameter access =====================================================
    
    //! Get the coordinate type
    inline DAL::Coordinate::Type type () {
      return coord_p.type();
    }
    //! Get the coordinate type as name
    inline std::string name () {
      return coord_p.name();
    }
    //! Get the storage type of the coordinate
    inline DAL::Coordinate::Type storageType () {
      return storageType_p.type();
    }
    //! Get the names of the attributes attached to the storage structure
    inline std::set<std::string> attributes () const {
      return attributes_p;
    }
    //! Get the number of coordinate axes
    inline int nofAxes () const {
      return nofAxes_p;
    }
    //! Get the world axis names
    inline std::vector<std::string> axisNames () const {
      return axisNames_p;
    }
    //___________________________________________________________________________
    //                                                               setAxisNames
    //! Set the world axis names
    bool setAxisNames (std::vector<std::string> const &axisNames)
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
    //! Get the world axis units
    inline std::vector<std::string> axisUnits () const {
      return axisUnits_p;
    }
    //___________________________________________________________________________
    //                                                               setAxisUnits
    //! Set the world axis units
    bool setAxisUnits (std::vector<std::string> const &axisUnits)
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
    //___________________________________________________________________________
    //                                                                   refValue
    //! Get the reference value
    inline std::vector<T> refValue () const {
      return refValue_p;
    }
    //___________________________________________________________________________
    //                                                                setRefValue
    //! Set the reference value
    virtual bool setRefValue (std::vector<T> const &refValue)
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
    //! Get the reference pixel
    inline std::vector<double> refPixel () const {
      return refPixel_p;
    }
    //___________________________________________________________________________
    //                                                                setRefPixel
    //! Set the reference pixel
    virtual bool setRefPixel (std::vector<double> const &refPixel)
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
    /*!
      \brief Get the coordinate axis increment
      \return increment -- The increment along the coordinate axes
    */
    inline std::vector<double> increment () const {
      return increment_p;
    }
    //___________________________________________________________________________
    //                                                               setIncrement
    //! Set the coordinate axis increment
    virtual bool setIncrement (std::vector<double> const &increment)
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
    /*!
      \brief Get the transformation matrix
      \return pc -- The transformation matrix, in row-wise ordering, e.g.
              [00,01,10,11]
    */
    inline std::vector<double> pc () const {
      return pc_p;
    }
    //___________________________________________________________________________
    //                                                                      setPc
    /*!
      \brief Set the transformation matrix
      \param pc -- The transformation matrix, in row-wise ordering, e.g.
             [00,01,10,11]
      \return status   -- Status of the operation; returns \e false in case an error
              was encountered.
    */
    virtual bool setPc (std::vector<double> const &pc)
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
    //! Get the tabulated values along the pixel axis
    inline std::vector<double> pixelValues () const {
      return pixelValues_p;
    }
    //___________________________________________________________________________
    //                                                             setPixelValues
    //! Set the tabulated values along the pixel axis
    virtual bool setPixelValues (std::vector<double> const &values)
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
	  if ((nelem = worldValues_p.size())) {
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
	if ((nelem = pixelValues_p.size())) {
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
    //___________________________________________________________________________
    //                                                                worldValues
    //! Get tabulated world values
    inline std::vector<T> worldValues () const {
      return worldValues_p;
    }
    //___________________________________________________________________________
    //                                                             setWorldValues
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, Coordinate.
    */
    inline std::string className () const {
      return "Coordinate";
    }
    
    // === Public methods =======================================================
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    
    //! Provide a summary of the internal status
    void summary (std::ostream &os) {
      os << "[CoordinateInterface] Summary of internal parameters." << std::endl;
      os << "-- Coordinate type  = " << type() << " / " <<  name() << std::endl;
      os << "-- Storage type     = " << storageType_p.type()
	 << " / " <<  storageType_p.name() << std::endl;
      os << "-- nof. axes        = " << nofAxes_p     << std::endl;
      os << "-- World axis names = " << axisNames_p   << std::endl;
      os << "-- World axis units = " << axisUnits_p   << std::endl;
      os << "-- Reference value  = " << refValue_p    << std::endl;
      os << "-- Reference pixel  = " << refPixel_p    << std::endl;
      os << "-- Increment        = " << increment_p   << std::endl;
      os << "-- PC               = " << pc_p          << std::endl;
    }
    
#ifdef HAVE_HDF5
    //! Write the coordinate object to a HDF5 file
    virtual void write_hdf5 (hid_t const &locationID) = 0;
    
    //! Write the coordinate object to a HDF5 file
    virtual void write_hdf5 (hid_t const &locationID,
			     std::string const &name) = 0;
    
    //! Read the coordinate object from a HDF5 file
    virtual void read_hdf5 (hid_t const &locationID) = 0;
    
    //! Read the coordinate object from a HDF5 file
    virtual void read_hdf5 (hid_t const &groupID,
			    std::string const &name) = 0;
#endif
    
    // === Private Methods ======================================================
    
  private:

    //___________________________________________________________________________
    //                                                              setAttributes
    //! Set the attributes attached to the storage structure
    void setAttributes ()
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
    //! Unconditional deletion
    void destroy(void) {
      attributes_p.clear();
    }
    
  }; // Class Coordinate -- end
  
} // Namespace DAL -- end

#endif /* COORDINATE_H */
