/*-------------------------------------------------------------------------*
 | $Id:: Stokescoordinate.h 2987 2009-09-04 12:43:48Z baehren            $ |
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

#ifndef STOKESCOORDINATE_H
#define STOKESCOORDINATE_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <Coordinate.h>

#ifdef HAVE_CASA
#include <coordinates/Coordinates/StokesCoordinate.h>
#endif

namespace DAL {  // Namespace DAL -- begin
  
  /*!
    \class StokesCoordinate
    
    \ingroup DAL
    
    \brief Brief description for class StokesCoordinate
    
    \author Lars B&auml;hren

    \date 2009/09/10

    \test tStokesCoordinate.cpp

    <h3>Prerequisite</h3>

    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */
  class StokesCoordinate : public Coordinate {
    
  public:
    
    //! Conventional Stokes values
    enum Stokes {
      //! Standard Stokes unpolarized
      I,
      //! Standard Stokes linear
      Q,
      //! Standard Stokes linear
      U,
      //! Standard Stokes circular
      V,
      //! Right-right circular
      RR,
      //! Left-left circular
      LL,
      //! Right-left circular
      RL,
      //! Left-right circular
      LR,
      //! X parallel linear
      XX,
      //! Y parallel linear
      YY,
      //! XY cross linear
      XY,
      //! YX cross linear
      YX
    };
    
  private:

    std::vector<StokesCoordinate::Stokes> values_p;

  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    StokesCoordinate ();
    //! Argumented constructor
    StokesCoordinate (StokesCoordinate::Stokes const &value);
    //! Argumented constructor
    StokesCoordinate (std::vector<StokesCoordinate::Stokes> const &values);
#ifdef HAVE_CASA
    //! Construction from casa::StokesCoordinate object
    StokesCoordinate (casa::StokesCoordinate const &coord) {
      importCoordinate (coord);
    }
#endif
    //! Copy constructor
    StokesCoordinate (StokesCoordinate const &other);
    
    // -------------------------------------------------------------- Destruction
    
    //! Destructor
    ~StokesCoordinate ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another StokesCoordinate object from which to make a copy.
    */
    StokesCoordinate& operator= (StokesCoordinate const &other);
    
    // --------------------------------------------------------------- Parameters

    //! Get the types of Stokes values for this coordinate
    inline std::vector<StokesCoordinate::Stokes> getTypes () {
      return values_p;
    }
    
    //! Get the names of Stokes values for this coordinate
    inline std::vector<std::string> getName () {
      std::vector<std::string> names;
      for (unsigned int (n); n<values_p.size(); n++) {
	names.push_back (getName(values_p[n]));
      }
      return names;
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, StokesCoordinate.
    */
    inline std::string className () const {
      return "StokesCoordinate";
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

    //! Get the name from the type of a Stokes value
    static std::string getName (StokesCoordinate::Stokes const &type);

    //! Get the type from the name of a Stokes value
    static StokesCoordinate::Stokes getType (std::string const &name);

    //! Get the types of Stokes values stored within this coordinate
    inline std::vector<StokesCoordinate::Stokes> stokesTypes () {
      return values_p;
    }

    //! Get the names of Stokes values stored within this coordinate
    std::vector<std::string> stokesNames ();
    
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
    void importCoordinate (casa::StokesCoordinate const &coord);
    //! Create casa::Coordinate object from coordinate parameters
    void exportCoordinate (casa::StokesCoordinate &coord);
#endif
    
  private:

    //! Initialize the internal parameters of the object
    void init (std::vector<StokesCoordinate::Stokes> const &polarisation);
    
    //! Unconditional copying
    void copy (StokesCoordinate const &other);
    
    //! Unconditional deletion
    void destroy(void);
    
  }; // Class StokesCoordinate -- end
  
} // Namespace DAL -- end

#endif /* STOKESCOORDINATE_H */

