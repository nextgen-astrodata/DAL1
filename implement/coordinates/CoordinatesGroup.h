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

#ifndef COORDINATESGROUP_H
#define COORDINATESGROUP_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <DirectionCoordinate.h>
#include <LinearCoordinate.h>
#include <StokesCoordinate.h>
#include <TabularCoordinate.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class CoordinatesGroup
    
    \ingroup DAL
    
    \brief Brief description for class CoordinatesGroup
    
    \author Lars B&auml;hren

    \date 2009/09/13

    \test tCoordinatesGroup.cpp
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>DirectionCoordinate
      <li>LinearCoordinate
      <li>StokesCoordinate
      <li>TabularCoordinate
    </ul>
    
    <h3>Synopsis</h3>

    The coordinates group, as defined through LOFAR-USG-ICD-004, works as 
    a container to collect coordinate descriptions as attached to an image or
    some other LOFAR data-set.

    <table border=0>
      <tr>
        <td class="indexkey">Field/Keyword
	<td class="indexkey">H5Type
	<td class="indexkey">Type
	<td class="indexkey">Unit
	<td class="indexkey">Description
      </tr>
      <tr>
        <td>GROUPTYPE
	<td>Attribute
	<td>string
	<td>`Coordinates'
	<td>This is a <tt>Coordinates Group</tt>
      </tr>
      <tr>
        <td>EQUINOX
	<td>Attribute
	<td>string
	<td>`J2000'
	<td>Equinox of the observation
      </tr>
      <tr>
        <td>SYSTEM_RADEC
	<td>Attribute
	<td>string
	<td>`FK5'
	<td>System Ra and Dec
      </tr>
      <tr>
        <td>REFERENCE_LOCATION_VALUE
	<td>Attribute
	<td>array<double,1>
	<td>---
	<td>Numerical value(s) of the reference location
      </tr>
      <tr>
        <td>REFERENCE_LOCATION_UNIT
	<td>Attribute
	<td>array<string,1>
	<td>---
	<td>Physical unit(s) for the reference location
      </tr>
      <tr>
        <td>REFERENCE_LOCATION_FRAME
	<td>Attribute
	<td>string
	<td>---
	<td>Identifier for the reference frame of the reference position
      </tr>
      <tr>
        <td>NOF_COORDINATES
	<td>Attribute
	<td>int
	<td>---
	<td>N of coordinate objects
      </tr>
      <tr>
        <td>NOF_AXES
	<td>Attribute
	<td>int
	<td>---
	<td>N of coordinate axes
      </tr>
      <tr>
        <td>COORDINATE_TYPES
	<td>Attribute
	<td>array <string,1>
	<td>---
	<td>embedded coordinate object types
      </tr>
      <tr>
        <td>COORDINATE{N}
	<td>Group
	<td>---
	<td>---
	<td>coordinate object container
      </tr>
    </table>

    Missing methods:
    - addCoordinate -- add a further coordinate object to the coordinates group.
    - getCoordinate -- retrieve an individual coordinate embedded within this group
    - write coordinates group to a HDF5 file
    - cast embedded information into a casa::CoordinateSystem object; this
    however will be incomplete, as the latter only stores a subset of the
    information this classes handles.
    - get/set reference lcoation as casa::MPosition
    
    <h3>Example(s)</h3>
    
  */  
  class CoordinatesGroup {

    //! Type of LOFAR group, "Coordinates"
    std::string grouptype_p;
    //! Equinox of the observation
    std::string equinox_p;
    //! Reference system for Ra/Dec
    std::string system_radec_p;

    //! Numerical value of the reference location
    std::vector<double> refLocationValue_p;
    //! Units attached to the numerical values of the reference location
    std::vector<std::string> refLocationUnit_p;
    //! Identifier for the reference location frame
    std::string refLocationFrame_p;

    //! container holding the actual coordinates which are parts of this group
    std::vector<Coordinate*> coordinates_p;
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    CoordinatesGroup ();
    
    /*!
      \brief Copy constructor
      
      \param other -- Another CoordinatesGroup object from which to create this new
             one.
    */
    CoordinatesGroup (CoordinatesGroup const &other);
    
    // -------------------------------------------------------------- Destruction

    //! Destructor
    ~CoordinatesGroup ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another CoordinatesGroup object from which to make a copy.
    */
    CoordinatesGroup& operator= (CoordinatesGroup const &other); 
    
    // --------------------------------------------------------------- Parameters

    //! Get the Equinox of the observation
    inline std::string equinox () {
      return equinox_p;
    }

    //! Set the Equinox of the observation
    inline void setEquinox (std::string const &equinox) {
      equinox_p = equinox;
    }

    //! Get the numerical value of the reference location
    inline std::vector<double> referenceLocationValue () {
      return refLocationValue_p;
    }

    //! Set the numerical value of the reference location
    bool setReferenceLocationValue (std::vector<double> const &value);
    
    //! Get the units attached to the numerical values of the reference location
    inline std::vector<std::string> referenceLocationUnit () {
      return refLocationUnit_p;
    }

    //! Set the units attached to the numerical values of the reference location
    bool setReferenceLocationUnit (std::vector<double> const &unit);
    
    //! Get the identifier for the reference location frame
    inline std::string referenceLocationFrame () {
      return refLocationFrame_p;
    }
    
    //! Get the identifier for the reference location frame
    inline void setReferenceLocationFrame (std::string const &frame) {
      refLocationFrame_p = frame;
    }
    
    //! Get the number of coordinates embedded within the group
    inline int nofCoordinates () {
      return coordinates_p.size();
    }

    //! Get the number of coordinate axes
    int nofAxes ();

    //! Get the types of the embedded coordinates
    std::vector<Coordinate::Type> coordinateTypes ();

    //! Get the names of the embedded coordinates
    std::vector<std::string> coordinateNames ();

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, CoordinatesGroup.
    */
    inline std::string className () const {
      return "CoordinatesGroup";
    }

    /*!
      \brief Provide a summary of the internal status
    */
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // ------------------------------------------------------------------ Methods
    
#ifdef HAVE_HDF5
    //! Read the coordinate object from a HDF5 file
    void h5read (hid_t const &groupID);
    
    //! Read the coordinate object from a HDF5 file
    void h5read (hid_t const &locationID,
		 std::string const &name);
    
    //! Write the coordinate object to a HDF5 file
    void h5write (hid_t const &groupID);
    
    //! Write the coordinate object to a HDF5 file
    void h5write (hid_t const &locationID,
		  std::string const &name);
#endif

    
  private:

    //! Initialize the internal parameters of the object
    void init ();
    
    //! Unconditional copying
    void copy (CoordinatesGroup const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class CoordinatesGroup -- end
  
} // Namespace DAL -- end

#endif /* COORDINATESGROUP_H */
  
