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

#ifndef STOKESCOORDINATE_H
#define STOKESCOORDINATE_H

// DAL header files
#include <coordinates/Stokes.h>
#include <coordinates/CoordinateBase.h>

#ifdef DAL_WITH_CASA
#include <coordinates/Coordinates/StokesCoordinate.h>
#endif

namespace DAL {  // Namespace DAL -- begin
  
  /*!
    \class StokesCoordinate
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Brief description for class StokesCoordinate
    
    \author Lars B&auml;hren
    
    \date 2009/09/10
    
    \test tStokesCoordinate.cc
    
    <h3>Prerequisite</h3>
    
    <ul>
      <li>DAL::Stokes -- Components of a Stokes coordinate.
    </ul>

    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */
  class StokesCoordinate : public CoordinateBase<std::string> {

    //! Set of Stokes components represented by this coordinate
    std::vector<DAL::Stokes> itsValues;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    StokesCoordinate ();
    //! Argumented constructor
    StokesCoordinate (Stokes::Component const &value);
    //! Argumented constructor
    StokesCoordinate (DAL::Stokes const &value);
    //! Argumented constructor
    StokesCoordinate (std::vector<DAL::Stokes::Component> const &values);
    //! Argumented constructor
    StokesCoordinate (std::vector<DAL::Stokes> const &values);
    //! Copy constructor
    StokesCoordinate (StokesCoordinate const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~StokesCoordinate ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    StokesCoordinate& operator= (StokesCoordinate const &other);
    
    // === Parameter access =====================================================

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, StokesCoordinate.
    */
    inline std::string className () const {
      return "StokesCoordinate";
    }
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    
    //! Provide a summary of the internal status
    void summary (std::ostream &os);
    
    // === Methods ==============================================================

    //! Get the Stokes values represented by the coordinate
    inline std::vector<DAL::Stokes> stokesValues () const {
      return itsValues;
    }

    //! Get the Stokes component types represented by the coordinate
    std::vector<DAL::Stokes::Component> stokesTypes ();

    //! Get the Stokes component names represented by the coordinate
    std::vector<std::string> stokesNames ();
    
#ifdef DAL_WITH_HDF5
    //! Read the coordinate object from a HDF5 file
    void read_hdf5 (hid_t const &groupID);
    
    //! Read the coordinate object from a HDF5 file
    void read_hdf5 (hid_t const &locationID,
		    std::string const &name);
    
    //! Write the coordinate object to a HDF5 file
    void write_hdf5 (hid_t const &groupID);
    
    //! Write the coordinate object to a HDF5 file
    void write_hdf5 (hid_t const &locationID,
		     std::string const &name);
#endif
    
#ifdef DAL_WITH_CASA
    //! Create coordinate from casa::Coordinate object
    bool read_casa (casa::StokesCoordinate const &coord);
    //! Create casa::Coordinate object from coordinate parameters
    bool write_casa (casa::StokesCoordinate &coord);
#endif
    
  private:

    //! Initialize the internal parameters of the object
    void init (std::vector<DAL::Stokes::Component> const &values);
    //! Initialize the internal parameters of the object
    void init (std::vector<DAL::Stokes> const &values);
    //! Set the attributes attached to the coordinate
    inline void setAttributes ()
    {
      /* Nothing else to do here. */
    }
    //! Unconditional deletion
    void destroy(void);
    
  }; // Class StokesCoordinate -- end
  
} // Namespace DAL -- end

#endif /* STOKESCOORDINATE_H */

