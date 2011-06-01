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

#ifndef COORDINATEBASE_H
#define COORDINATEBASE_H

/* DAL header files */
#include <core/dalCommon.h>
#include <core/HDF5Attribute.h>
#include <coordinates/Coordinate.h>

/* casacore header files */
#ifdef DAL_WITH_CASA
#include <coordinates/Coordinates/DirectionCoordinate.h>
#endif

namespace DAL {   // Namespace DAL -- begin

  /*!
    \class CoordinateBase

    \ingroup DAL
    \ingroup coordinates

    \brief Definition of common interface for coordinate classes

    \author Lars B&auml;hren

    \date 2009/06/23

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>LOFAR Data Format ICD: Representations of World Coordinates.
          LOFAR-USG-ICD-002.
    </ul>

    <h3>Synopsis</h3>

    This class defines the basic interface and data contents for a coordinate
    object as being part of a HDF5-based LOFAR data-set (most prominently an
    image).

    <h3>Example(s)</h3>

    As CoordinateBase defines an abstract base class, it cannot be used
    directly.

  */
  template <class T> class CoordinateBase {
    
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
    std::vector<double> itsPixelValues;
    //! List of world values
    std::vector<T> itsWorldValues;
    
    // === Protected Methods ====================================================

    //___________________________________________________________________________
    //                                                                       init
    //! Initilize the internal set of parameters
    void init (DAL::Coordinate const &coord=DAL::Coordinate(),
	       unsigned int const &nofAxes=0,
	       DAL::Coordinate const &storageType=DAL::Coordinate())
    {
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
	  refValue_p[n]  = T();
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
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    CoordinateBase (DAL::Coordinate const &coord=DAL::Coordinate(),
		    unsigned int const &nofAxes=0,
		    DAL::Coordinate const &storageType=DAL::Coordinate()) {
      init (coord, nofAxes, storageType);
    }
    //! Copy constructor
    CoordinateBase (CoordinateBase const &other) {
      *this = other;
    }
    
    // === Destruction ==========================================================
    
    //! Destructor
    virtual ~CoordinateBase () {
      destroy();
    }
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    CoordinateBase<T>& operator= (CoordinateBase<T> const &other) {
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
    bool setAxisNames (std::string const &names)
    {
      if (nofAxes_p > 0) {
	std::vector<std::string> tmp (nofAxes_p,names);
	return setAxisNames (tmp);
      } else {
	return false;
      }

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
	std::cerr << "[CoordinateBase::setAxisNames]"
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
    bool setAxisUnits (std::string const &units)
    {
      if (nofAxes_p > 0) {
	std::vector<std::string> tmp (nofAxes_p,units);
	return setAxisUnits (tmp);
      } else {
	return false;
      }

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
	std::cerr << "[CoordinateBase::setAxisUnits]"
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
	    std::cerr << "[CoordinateBase::setRefValue]"
		      << " Error in length of input vector!"
		      << std::endl;
	    status = false;
	  }
	}
	break;
      case Coordinate::TABULAR:
	{
	  if (!itsWorldValues.empty()) {
	    refValue_p.resize(1);
	    refValue_p[0] = itsWorldValues[0];
	  } else {
	    status = false;
	  }
	}
	break;
      default:
	{
	  std::cerr << "[CoordinateBase::setRefValue]"
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
	    std::cerr << "[CoordinateBase::setRefPixel]"
		      << " Error in length of input vector!"
		      << std::endl;
	    status = false;
	  }
	}
	break;
      case Coordinate::TABULAR:
	{
	  if (!itsPixelValues.empty()) {
	    refPixel_p.resize(1);
	    refPixel_p[0] = itsPixelValues[0];
	  } else {
	    status = false;
	  }
	}
	break;
      default:
	{
	  std::cerr << "[CoordinateBase::setRefPixel]"
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
	    std::cerr << "[CoordinateBase::setIncrement]"
		      << " Error in length of input vector!"
		      << std::endl;
	    status = false;
	  }
	}
	break;
      default:
	{
	  std::cerr << "[CoordinateBase::setIncrement]"
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
	  std::cerr << "[CoordinateBase::setPc]"
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
	std::cerr << "[CoordinateBase::setPc]"
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
      return itsPixelValues;
    }
    //___________________________________________________________________________
    //                                                             setPixelValues
    //! Set the tabulated values along the pixel axis
    virtual bool setPixelValues (std::vector<double> const &values)
    {
      bool status (true);
      unsigned int nelem = values.size();
      
      if (itsPixelValues.empty()) {
	if (itsWorldValues.empty()) {
	  /* If both pixel and world values are not defined yet, we can simply
	     accept the provided values, as there is no further reference. */
	  itsPixelValues.resize(nelem);
	  itsPixelValues = values;
	} else {
	  /* If the world axis values have been set already, enforce match in the
	     size of the arrays. */
	  if ((nelem = itsWorldValues.size())) {
	    itsPixelValues.resize(nelem);
	    itsPixelValues = values;
	  } else {
	    /* Reject input values if number of elements does not match that of
	       the already assigned world axis values. */
	    std::cerr << "[CoordinateBase::setPixelValues]"
		      << " Wrong number of elements in the provided array!"
		      << std::endl;
	    status = false;
	  }
	}
      } else {
	if ((nelem = itsPixelValues.size())) {
	  itsPixelValues = values;
	} else {
	  std::cerr << "[CoordinateBase::setPixelValues]"
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
      return itsWorldValues;
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
      os << "[CoordinateBase] Summary of internal parameters." << std::endl;
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
    
#ifdef DAL_WITH_HDF5
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
      
      switch (storageType_p.type()) {
      case Coordinate::LINEAR:
      case Coordinate::DIRECTION:
	attributes_p.insert("REFERENCE_VALUE");
	attributes_p.insert("REFERENCE_PIXEL");
	attributes_p.insert("INCREMENT");
	attributes_p.insert("PC");
	break;
      case Coordinate::TABULAR:
	attributes_p.insert("AXIS_LENGTH");
	attributes_p.insert("AXIS_VALUES_PIXEL");
	attributes_p.insert("AXIS_VALUES_WORLD");
	break;
      default:
	break;
      };
      
    }
    //! Unconditional deletion
    void destroy(void) {
      attributes_p.clear();
    }
    
  }; // Class Coordinate -- end
  
} // Namespace DAL -- end

#endif /* COORDINATE_H */
