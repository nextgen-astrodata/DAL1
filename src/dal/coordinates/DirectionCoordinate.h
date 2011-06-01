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

#ifndef DIRECTIONCOORDINATE_H
#define DIRECTIONCOORDINATE_H

// DAL header files
#include <coordinates/CoordinateBase.h>

#ifdef DAL_WITH_CASA
#include <coordinates/Coordinates/DirectionCoordinate.h>
#endif

namespace DAL {   // Namespace DAL -- begin

  /*!
    \class DirectionCoordinate

    \ingroup DAL
    \ingroup coordinates

    \brief Brief description for class DirectionCoordinate

    \author Lars B&auml;hren

    \date 2009/06/24

    \test tDirectionCoordinate.cpp

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>[start filling in your text here]
    </ul>

    <h3>Synopsis</h3>

    <h3>Example(s)</h3>

  */
  class DirectionCoordinate : public CoordinateBase<double> {
    
    //! Reference code for the celestial coordinate system
    std::string system_p;
    //! Reference code for the spherical map projection
    std::string projection_p;
    //! Optional parameters to the projection
    std::vector<double> projectionParam_p;
    //! Longpole
    double longpole_p;
    //! Latpole
    double latpole_p;
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    DirectionCoordinate (std::string const &system="J2000",
			 std::string const &projection="SIN");
    //! Argumented constructor
    DirectionCoordinate (std::vector<std::string> const &axisNames,
			 std::vector<std::string> const &axisUnits,
			 std::vector<double> const &refValue,
			 std::vector<double> const &refPixel,
			 std::vector<double> const &increment,
			 std::vector<double> const &pc,
			 std::string const &system="J2000",
			 std::string const &projection="SIN");
    //! Copy constructor
    DirectionCoordinate (DirectionCoordinate const &other);
    
    // -------------------------------------------------------------- Destruction
    
    //! Destructor
    ~DirectionCoordinate ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another DirectionCoordinate object from which to make a copy.
    */
    DirectionCoordinate& operator= (DirectionCoordinate const &other);
    
    // === Parameter access =====================================================
    
    //! Get the projection parameters
    inline std::vector<double> projectionParameters () {
      return projectionParam_p;
    }
    //! Set the projection parameters
    inline void setProjectionParameters (std::vector<double> const &param) {
      projectionParam_p.resize(param.size());
      projectionParam_p = param;
    }
    
    inline double longpole () {
      return longpole_p;
    }
    inline void setLongpole (double const &longpole) {
      longpole_p = longpole;
    }
    inline double latpole () {
      return latpole_p;
    }
    inline void setLatpole (double const &latpole) {
      latpole_p = latpole;
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, DirectionCoordinate.
    */
    inline std::string className () const {
      return "DirectionCoordinate";
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
    
    // === Public Methods =======================================================
    
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
    void importCoordinate (casa::DirectionCoordinate const &coord);
    //! Create casa::Coordinate object from coordinate parameters
    void exportCoordinate (casa::DirectionCoordinate &coord);
#endif
    
  private:
    
    //! Initialize internal parameters
    void init (std::string const &system,
	       std::string const &projection);
    
    //! Set the attributes attached to the coordinate
    inline void setAttributes ()
    {
      attributes_p.insert("LONGPOLE");
      attributes_p.insert("LATPOLE");
    }

    //! Unconditional copying
    void copy (DirectionCoordinate const &other);
    
    //! Unconditional deletion
    void destroy(void);
    
  }; // Class DirectionCoordinate -- end
  
} // Namespace DAL -- end

#endif /* DIRECTIONCOORDINATE_H */

