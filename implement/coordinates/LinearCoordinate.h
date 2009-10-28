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

#ifndef LINEARCOORDINATE_H
#define LINEARCOORDINATE_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <Coordinate.h>

#ifdef HAVE_CASA
#include <coordinates/Coordinates/LinearCoordinate.h>
#endif

namespace DAL {  // Namespace DAL -- begin
  
  /*!
    \class LinearCoordinate
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Brief description for class LinearCoordinate
    
    \author Lars B&auml;hren

    \date 2009/06/24

    \test tLinearCoordinate.cpp

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>M.R. Calabretta, E.W. Greisen. <i>Representation of celestial
      coordinates in FITS</i>. A&A 395, 1077-1122 (2002)
    </ul>

    <h3>Synopsis</h3>

    Consider a simple image (or dynammic spectrum), where we are dealing with both
    a regular time and frequency axis:
    \verbatim
    NOF_AXES         =  2
    AXIS_NAMES       = [ "Time", "Frequency" ]
    AXIS_UNITS       = [ "s", "Hz" ]
    REFERENCE_PIXEL  = [ 0.0, 00.0 ]
    REFERENCE_VALUE  = [ 0.1, 100.0 ]
    INCREMENT        = [ 0.05, 10.0 ]
    PC               = [ 1.0, 0.0, 0.0, 1.0 ]
    \endverbatim
    The same information stored as FITS header keywords would be
    \verbatim
    NAXES = 2
    CTYPE1 = "Time"
    CTYPE2 = "Frequency"
    CRPIX1 = 0.0
    CRPIX2 = 0.0
    CRVAL1 = 0.1
    CRVAL2 = 100.0
    CDELT1 = 0.05
    CDELT2 = 10.0
    \endverbatim
    
    <h3>Example(s)</h3>

  */
  class LinearCoordinate : public Coordinate {
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    LinearCoordinate ();
    //! Argumented constructor
    LinearCoordinate (double const &nofAxes);
    //! Argumented constructor
    LinearCoordinate (double const &nofAxes,
		      std::vector<std::string> const &axisNames,
		      std::vector<std::string> const &axisUnits,
		      std::vector<double> const &refValue,
		      std::vector<double> const &refPixel,
		      std::vector<double> const &increment,
		      std::vector<double> const &pc);
#ifdef HAVE_CASA
    //! Construction from casa::LinearCoordinate object
    LinearCoordinate (casa::LinearCoordinate const &coord) {
      importCoordinate (coord);
    }
#endif
    //! Copy constructor
    LinearCoordinate (LinearCoordinate const &other);
    
    // -------------------------------------------------------------- Destruction
    
    //! Destructor
    ~LinearCoordinate ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another LinearCoordinate object from which to make a copy.
    */
    LinearCoordinate& operator= (LinearCoordinate const &other);
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, LinearCoordinate.
    */
    inline std::string className () const {
      return "LinearCoordinate";
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

#ifdef HAVE_CASA
    //! Create coordinate from casa::Coordinate object
    void importCoordinate (casa::LinearCoordinate const &coord);
    //! Create casa::Coordinate object from coordinate parameters
    void exportCoordinate (casa::LinearCoordinate &coord);
#endif
    
  private:
    
    //! Unconditional copying
    void copy (LinearCoordinate const &other);
    
    //! Unconditional deletion
    void destroy(void);
    
  }; // Class LinearCoordinate -- end
  
} // Namespace DAL -- end

#endif /* LINEARCOORDINATE_H */

