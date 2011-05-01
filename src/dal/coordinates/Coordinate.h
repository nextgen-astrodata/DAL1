/***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#ifndef COORDINATE_H
#define COORDINATE_H

// Standard library header files
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#ifdef DAL_WITH_CASA
#include <measures/Measures/MDirection.h>
#ifdef DAL_WITH_WCSLIB
#include <coordinates/Coordinates/Projection.h>
#endif
#endif

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Coordinate
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Basic properties of a coordinate
    
    \author Lars B&auml;hren

    \date 2010/11/01

    \test tCoordinate.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class Coordinate {
    
  public:
    
    //! Types of coordinates; for definition and attributes see LOFAR-USG-ICD-002
    enum Type {
      //! Direction coordinate
      DIRECTION,
      //! Linear coordinate
      LINEAR,
      //! Tabulated coordinate
      TABULAR,
      //! Stokes parameters coordinate
      STOKES,
      //! Spectral coordinate,
      SPECTRAL,
      //! Undefined coordinate type
      UNDEFINED
    };

    // === Construction =========================================================
    
    //! Argumented constructor
    Coordinate (Coordinate::Type const &type=DAL::Coordinate::UNDEFINED);
    //! Copy constructor
    Coordinate (Coordinate const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~Coordinate ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    Coordinate& operator= (Coordinate const &other); 
    
    // === Parameter access =====================================================

    //! Get the type of coordinate
    inline Coordinate::Type type () {
      return coord_p->first;
    }
    //! Get the type of coordinate as name
    inline std::string name () {
      return coord_p->second;
    }
    //! Set the type of coordinate
    bool setType (Coordinate::Type const &type);
    //! Set the type of coordinate
    bool setType (std::string const &type);
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, Coordinate.
    */
    inline std::string className () const {
      return "Coordinate";
    }
    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================

    //! Does this coordinate contain a map projection?
    inline bool hasProjection () {
      return hasProjection(coord_p->first);
    }

    //! Is this a tabular coordinate?
    inline bool isTabular () {
      return isTabular(coord_p->first);
    }

    // === Static methods =======================================================

    //! Get map of component types and names
    static std::map<Coordinate::Type,std::string> coordinatesMap ();
    //! Get list of component types
    static std::vector<Coordinate::Type> coordinatesType ();
    //! Get list of component names
    static std::vector<std::string> coordinatesName ();
    //! Get name type of the coordinate as name
    static Coordinate::Type getType (std::string const &name);
    //! Get name type of the coordinate as name
    static std::string getName (Coordinate::Type const &type);
    //! Does this coordinate contain a map projection?
    static bool hasProjection (Coordinate::Type const &type);
    //! Is this a tabular coordinate?
    static bool isTabular (Coordinate::Type const &type);

#ifdef DAL_WITH_CASA
#ifdef DAL_WITH_WCSLIB
    //! Get the type of a reference system from its name
    static casa::MDirection::Types systemType (casa::String const &refcode) {
      // Local variables
      bool ok (true);
      casa::MDirection md;
      casa::MDirection::Types tp;
      // Get the type of the MDirection
      ok = md.getType(tp,refcode);
      // Return the result
      return tp;
    }
    //! Get the type of a projection from its name
    static casa::Projection::Type projectionType (casa::String const &refcode) {
      casa::Projection prj;	
      return prj.type(refcode);
    }
#endif
#endif

  private:

    // === Private  variables ===================================================
    
    //! List of coordinate types
    std::map<Coordinate::Type,std::string> coordTypes_p;
    //! Stokes component represented by this object
    std::map<Coordinate::Type,std::string>::iterator coord_p;

    // === Private methods ======================================================

    //! Initialize the object's internal parameters
    void init (Coordinate::Type const &type=Coordinate::UNDEFINED);
    
    //! Unconditional copying
    void copy (Coordinate const &other);
    
    //! Unconditional deletion 
    void destroy(void);

  }; // Class Coordinate -- end
  
} // Namespace DAL -- end

#endif /* COORDINATE_H */
  
