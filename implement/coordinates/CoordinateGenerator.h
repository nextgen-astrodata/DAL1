/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#ifndef COORDINATEGENERATOR_H
#define COORDINATEGENERATOR_H

// Standard library header files
#include <iostream>
#include <map>
#include <string>
#include <vector>

#ifdef HAVE_CASA
#ifndef WCSLIB_GETWCSTAB
#define WCSLIB_GETWCSTAB 1
#endif
#include <casa/Arrays/Matrix.h>
#include <casa/Arrays/Vector.h>
#include <casa/BasicSL/String.h>
#include <casa/Quanta/Quantum.h>
#include <coordinates/Coordinates/Coordinate.h>
#include <coordinates/Coordinates/DirectionCoordinate.h>
#include <coordinates/Coordinates/LinearCoordinate.h>
#include <coordinates/Coordinates/StokesCoordinate.h>
#include <coordinates/Coordinates/SpectralCoordinate.h>
#include <coordinates/Coordinates/TabularCoordinate.h>

using casa::Quantum;
using casa::Vector;
#endif

#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class CoordinateGenerator
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief A collection of methods to create coordinate objects
    
    \author Lars B&auml;hren

    \date 2010/06/08

    \test tCoordinateGenerator.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>E. W. Greisen & M. R. Calabretta (2002) Representations of world
      coordinates in FITS. A&A, \b 395, p. 1061-1075.
      <li>M. R. Calabretta & E. W. Greisen (2002) Representations of celestial
      coordinates in FITS, A&A, \b 395, p. 1077-1122.
      <li>E. W. Greisen, M. R. Calabretta, F. G. Valdes, & S. L. Allen, (2006)
      Representations of spectral coordinates in FITS. A&A, \b 446, p. 747-771.
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class CoordinateGenerator {
    
    //! Do we have CASA or casacore?
    bool haveCASA_p;
    //! Map with the reference codes and descriptions of projects
    std::map<std::string,std::string> projections_p;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    CoordinateGenerator ();
    
    //! Copy constructor
    CoordinateGenerator (CoordinateGenerator const &other);
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    CoordinateGenerator& operator= (CoordinateGenerator const &other); 
    
    // === Parameter access =====================================================
    
    //! Have CASA/casacore?
    inline bool haveCASA () const  {
      return haveCASA_p;
    }

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, CoordinateGenerator.
    */
    inline std::string className () const {
      return "CoordinateGenerator";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================

    //! Get the list of projection names/refcodes
    std::vector<std::string> projectionNames ();

    //! Get the descriptions of the projections
    std::vector<std::string> projectionDescriptions ();

    // === Static methods =======================================================
    
#ifdef HAVE_CASA

    //___________________________________________________________________________
    //                                             Creation of coordinate objects

    //! Create a casa::DirectionCoordinate object
    /* static bool makeCoordinate (casa::DirectionCoordinate &coord, */
    /* 				std::string const &refcode, */
    /* 				std::string const &projection, */
    /* 				casa::Vector<casa::Quantum<double> > const &refValue, */
    /* 				casa::Vector<casa::Quantum<double> > const &increment, */
    /* 				casa::Vector<double> const &refPixel); */

    //! Create a casa::DirectionCoordinate object
    static bool makeCoordinate (casa::DirectionCoordinate &coord,
				casa::MDirection::Types const &refcode,
				casa::Projection::Type const &projection,
				casa::Vector<casa::Quantum<double> > const &refValue,
				casa::Vector<casa::Quantum<double> > const &increment,
				casa::Vector<double> const &refPixel);

    //! Create a casa::LinearCoordinate object
    static bool makeCoordinate (casa::LinearCoordinate &coord,
				unsigned int const &nofAxes,
				casa::Vector<casa::String> const &names,
				casa::Vector<casa::String> const &units);
    //! Create a casa::LinearCoordinate object
    static bool makeCoordinate (casa::LinearCoordinate &coord,
				unsigned int const &nofAxes=1,
				casa::String const &name="Length",
				casa::String const &unit="m",
				double const &refValue=0.0,
				double const &increment=0.0,
				double const &refPixel=1.0);
    //! Create a casa::LinearCoordinate object
    static bool makeCoordinate (casa::LinearCoordinate &coord,
				casa::Vector<casa::String> const &names,
				casa::Vector<casa::String> const &units,
				casa::Vector<double> const &refValue,
				casa::Vector<double> const &increment,
				casa::Vector<double> const &refPixel);

    //! Create a casa::SpectralCoordinate object
    static bool makeCoordinate (casa::SpectralCoordinate &coord,
				double const &refValue=0,
				double const &increment=0,
				double const &refPixel=0);
    
    //! Create a casa::StokesCoordinate object
    static bool makeCoordinate (casa::StokesCoordinate &coord);
    
    
#endif 
    
  private:

    //! Initialize the internal parameters
    void init ();

    //! Set up the map with the projections
    void setProjections ();
    
    //! Unconditional copying
    void copy (CoordinateGenerator const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class CoordinateGenerator -- end
  
} // Namespace DAL -- end

#endif /* COORDINATEGENERATOR_H */
  
