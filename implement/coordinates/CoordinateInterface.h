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

    //! Unconditional copying
    void copy (CoordinateInterface const &other);
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
    //! Set the world axis names
    bool setAxisNames (std::vector<std::string> const &axisNames);
    //! Get the world axis units
    inline std::vector<std::string> axisUnits () const {
      return axisUnits_p;
    }
    //! Set the world axis units
    bool setAxisUnits (std::vector<std::string> const &axisUnits);
    //! Get the reference value
    inline std::vector<T> refValue () const {
      return refValue_p;
    }
    //! Set the reference value
    virtual bool setRefValue (std::vector<T> const &refValue);
    //! Get the reference pixel
    inline std::vector<double> refPixel () const {
      return refPixel_p;
    }
    //! Set the reference pixel
    virtual bool setRefPixel (std::vector<double> const &refPixel);
    /*!
      \brief Get the coordinate axis increment
      \return increment -- The increment along the coordinate axes
    */
    inline std::vector<double> increment () const {
      return increment_p;
    }
    //! Set the coordinate axis increment
    virtual bool setIncrement (std::vector<double> const &increment);
    /*!
      \brief Get the transformation matrix
      \return pc -- The transformation matrix, in row-wise ordering, e.g.
              [00,01,10,11]
    */
    inline std::vector<double> pc () const {
      return pc_p;
    }
    //! Set the transformation matrix
    virtual bool setPc (std::vector<double> const &pc);
    //! Get the tabulated values along the pixel axis
    inline std::vector<double> pixelValues () const {
      return pixelValues_p;
    }
    //! Set the tabulated values along the pixel axis
    virtual bool setPixelValues (std::vector<double> const &values);
    //! Get tabulated world values
    inline std::vector<T> worldValues () const {
      return worldValues_p;
    }
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

    //! Set the attributes attached to the storage structure
    void setAttributes ();
    //! Unconditional deletion
    void destroy(void) {
      attributes_p.clear();
    }
    
  }; // Class Coordinate -- end
  
} // Namespace DAL -- end

#endif /* COORDINATE_H */
