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

    <table border=0>
      <tr>
        <td class="indexkey"></td>
        <td class="indexkey">DirectionCoordinate</td>
        <td class="indexkey">LinearCoordinate</td>
        <td class="indexkey">TabularCoordinate</td>
        <td class="indexkey">StokesCoordinate</td>
        <td class="indexkey">SpectralCoordinate</td>
      </tr>
      <tr>
        <td class="indexkey">GROUPTYPE</td>
        <td>CoordDirection</td>
        <td>CoordLinear</td>
        <td>CoordTabular</td>
        <td>CoordStokes</td>
        <td>CoordSpectral</td>
      </tr>
      <tr>
        <td class="indexkey">COORDINATE_TYPE</td>
        <td>DIRECTION</td>
        <td>LINEAR</td>
        <td>TABULAR</td>
        <td>STOKES</td>
        <td>SPECTRAL</td>
      </tr>
      <tr>
        <td class="indexkey">NOF_AXES</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes</td>
      </tr>
      <tr>
        <td class="indexkey">AXIS_NAMES</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes</td>
      </tr>
      <tr>
        <td class="indexkey">AXIS_UNITS</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes</td>
      </tr>
      <tr>
        <td class="indexkey">REFERENCE_PIXEL</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes, refPixel[0]</td>
        <td>\e yes</td>
        <td>\e yes</td>
      </tr>
      <tr>
        <td class="indexkey">REFERENCE_VALUE</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes, refValue[0]</td>
        <td>\e yes</td>
        <td>\e yes</td>
      </tr>
      <tr>
        <td class="indexkey">PC</td>
        <td>\e yes</td>
        <td>\e yes</td>
        <td>\e yes, pc[0]=1.0</td>
        <td>\e yes</td>
        <td>\e yes</td>
      </tr>
      <tr>
        <td class="indexkey">PIXEL_VALUES</td>
        <td>\e --</td>
        <td>\e ---</td>
        <td>\e ---</td>
        <td>\e yes</td>
        <td>\e --</td>
      </tr>
      <tr>
        <td class="indexkey">WORLD_VALUES</td>
        <td>\e --</td>
        <td>\e ---</td>
        <td>\e ---</td>
        <td>\e yes</td>
        <td>\e --</td>
      </tr>
    </table>

    <h3>Example(s)</h3>

  */
  class CoordinateInterface {
    
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
    std::vector<double> refValue_p;
    //! Reference pixel (CRPIX)
    std::vector<double> refPixel_p;
    //! Coordinate axis increment (CDELT)
    std::vector<double> increment_p;
    //! Transformation matrix (PC)
    std::vector<double> pc_p;
    //! List of pixel values
    std::vector<double> pixelValues_p;
    //! List of world values
    std::vector<double> worldValues_p;
    
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
    CoordinateInterface ();
    //! Copy constructor
    CoordinateInterface (CoordinateInterface const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    virtual ~CoordinateInterface ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    CoordinateInterface& operator= (CoordinateInterface const &other);
    
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
    void setAxisNames (std::vector<std::string> const &axisNames) {
      if (axisNames.size() == nofAxes_p) {
	axisNames_p = axisNames;
      }
    }
    //! Get the world axis units
    inline std::vector<std::string> axisUnits () const {
      return axisUnits_p;
    }
    //! Set the world axis units
    void setAxisUnits (std::vector<std::string> const &axisUnits) {
      if (axisUnits.size() == nofAxes_p) {
	axisUnits_p = axisUnits;
      }
    }
    
    //! Get the reference value
    inline std::vector<double> refValue () const {
      return refValue_p;
    }
    //! Set the reference value
    virtual bool setRefValue (std::vector<double> const &refValue);
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
    /*!
      \brief Set the coordinate axis increment
      \param increment -- The increment along the coordinate axes
    */
    virtual void setIncrement (std::vector<double> const &increment) {
      if (increment.size() == nofAxes_p) {
	increment_p = increment;
      }
    }
    /*!
      \brief Get the transformation matrix
      \return pc -- The transformation matrix, in row-wise ordering, e.g.
              [00,01,10,11]
    */
    inline std::vector<double> pc () const {
      return pc_p;
    }
    /*!
      \brief Set the transformation matrix
      \param pc -- The transformation matrix, in row-wise ordering, e.g.
             [00,01,10,11]
    */
    virtual void setPc (std::vector<double> const &pc) {
      unsigned int nelem = nofAxes_p*nofAxes_p;
      if (pc.size() == nelem) {
	pc_p = pc;
      }
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
    void summary (std::ostream &os);
    
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
    void destroy(void);
    
  }; // Class Coordinate -- end
  
} // Namespace DAL -- end

#endif /* COORDINATE_H */
