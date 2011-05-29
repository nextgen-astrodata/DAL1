/***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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
#include <core/HDF5Object.h>
#include <coordinates/DirectionCoordinate.h>
#include <coordinates/LinearCoordinate.h>
#include <coordinates/TabularCoordinate.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class CoordinatesGroup
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief High-level interface to the system logs attached to a beamformed dataset
    
    \author Lars B&auml;hren

    \date 2009/11/26

    \test tCoordinatesGroup.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\ref dal_icd_002
      <li>\ref dal_icd_003
      <li>\ref dal_icd_004
    </ul>
    
    \todo addCoordinate -- add a further coordinate object to the coordinates group.
    \todo getCoordinate -- retrieve an individual coordinate embedded within this
          group write coordinates group to a HDF5 file
    \todo cast embedded information into a casa::CoordinateSystem object; this
          however will be incomplete, as the latter only stores a subset of the
	  information this classes handles.
    \todo get/set reference lcoation as casa::MPosition
    
    <h3>Synopsis</h3>

    The coordinates group, as defined through LOFAR-USG-ICD-002, works as a
    container to collect coordinate descriptions as attached to an image or
    some other LOFAR data-set.

    \verbatim
    .                                 Group
    |-- GROUPTYPE                     Attr.          string
    |-- REF_LOCATION_VALUE            Attr.          array<double,1>
    |-- REF_LOCATION_UNIT             Attr.          array<string,1>
    |-- REF_LOCATION_FRAME            Attr.          array<string,1>
    |-- REF_TIME_VALUE                Attr.          double
    |-- REF_TIME_UNIT                 Attr.          string
    |-- REF_TIME_FRAME                Attr.          string
    |-- NOF_COORDINATES               Attr.          int
    |-- NOF_AXES                      Attr.          int
    |-- COORDINATE_TYPES              Attr.          array<string,1>
    |-- COORDINATE0                   Group
    |   ...
    `-- COORDINATE{N}                 Group
    \endverbatim

    <h3>Example(s)</h3>
    
  */  
  class CoordinatesGroup {

    std::set<std::string> itsAttributes;

    //! Group type descriptor
    std::string itsGroupType;
    //! Reference location value
    std::vector<double> itsRefLocationValue;
    //! Reference location unit
    std::vector<std::string> itsRefLocationUnits;
    //! Reference location frame
    std::string itsRefLocationFrame;
    //! Reference time value
    double itsRefTimeValue;
    //! Reference time unit
    std::string itsRefTimeUnits;
    //! Reference time frame
    std::string itsRefTimeFrame;
    //! nof. coordinate axes
    int itsNofAxes;
    //! Container for book-keeping on the embedded coordinate objects
    std::vector<std::string> itsCoordinateTypes;
    //! Container for the coordinate objects embedded within this group
    std::vector<Coordinate> itsCoordinates;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    CoordinatesGroup ();
    
    //! Argumented constructor
    CoordinatesGroup (hid_t const &location);
    
    //! Argumented constructor
    CoordinatesGroup (hid_t const &location,
		      std::string const &name);

    // === Destruction =========================================================
    
    //! Default destructor
    virtual ~CoordinatesGroup ();
    
    // === Parameter access =====================================================
    
    //! Get the group type identifier
    inline std::string groupType () const {
      return itsGroupType;
    }

    //! Get the reference location
    bool referenceLocation (std::vector<double> &value,
			    std::vector<std::string> &units,
			    std::string &frame);
    //! Set the reference location
    bool setReferenceLocation (std::vector<double> const &value,
			       std::vector<std::string> const &units,
			       std::string const &frame);

    //! Get the reference time
    bool referenceTime (double &value,
			std::string &units,
			std::string &frame);
    //! Set the reference time
    bool setReferenceTime (double const &value,
			   std::string const &units,
			   std::string const &frame);

    //! Get the nof. embedded coordinate objects
    inline int nofCoordinates () const {
      return itsCoordinates.size();
    }
    //! Get the nof. coordinate axes
    int nofAxes ();

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, CoordinatesGroup.
    */
    inline std::string className () const {
      return "CoordinatesGroup";
    }
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the internal status
    void summary (std::ostream &os);    
    
    // === Methods ==============================================================

    //! Read coordinates group from HDF5 file
    bool h5read (hid_t const &location);

    //! Read coordinates group from HDF5 file
    bool h5read (hid_t const &location,
		 std::string const &name);

    //! Write coordinates group to HDF5 file
    bool h5write (hid_t const &location);

    //! Write coordinates group to HDF5 file
    bool h5write (hid_t const &location,
		  std::string const &name);
    
  private:

    //! Initialize the internal parameter of the object
    bool init ();
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class CoordinatesGroup -- end
  
} // Namespace DAL -- end

#endif /* COORDINATESGROUP_H */

