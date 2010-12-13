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

#ifndef TABULARCOORDINATE_H
#define TABULARCOORDINATE_H

// DAL header files
#include "CoordinateInterface.h"

#ifdef HAVE_CASA
#include <coordinates/Coordinates/TabularCoordinate.h>
#endif

namespace DAL {  // Namespace DAL -- begin
  
  /*!
    \class TabularCoordinate
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Brief description for class TabularCoordinate
    
    \author Lars B&auml;hren

    \date 2009/06/24

    \test tTabularCoordinate.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>[start filling in your text here]
    </ul>

    <h3>Synopsis</h3>

    <h3>Example(s)</h3>

    <ul>
      <li>One of the cases where a tabular axis can become of use is for a
      non-contiguous time-axis (e.g. due to the fact that parts of an observation
      were dropped during transfer).
      \verbatim
      COORDINATE_TYPE    = Tabular
      NOF_AXES           = 1
      AXIS_NAMES         = "Time"
      AXIS_UNITS         = "s"
      AXIS_LENGTH        = 8
      AXIS_VALUES_PIXEL  = [0,1,2,3,4,5,6,7]
      AXIS_VALUES_WORLD  = [0,1,2,5,10,20,50,100]
      \endverbatim
    </ul>
    
  */
  template <class T>
  class TabularCoordinate : public CoordinateInterface<T> {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    TabularCoordinate () {
      init();
    }
    //! Argumented constructor
    TabularCoordinate (std::string const &axisNames,
		       std::string const &axisUnits);
    //! Argumented constructor
    TabularCoordinate (std::string const &axisNames,
		       std::string const &axisUnits,
		       std::vector<double> const &pixelValues,
		       std::vector<T> const &worldValues);
    //! Copy constructor
    TabularCoordinate (TabularCoordinate<T> const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~TabularCoordinate () {
      destroy();
    }
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TabularCoordinate object from which to make a copy.
    */
    TabularCoordinate<T>& operator= (TabularCoordinate<T> const &other) {
      if (this != &other) {
	destroy ();
	copy (other);
      }
      return *this;
    }
    
    // === Parameter access =====================================================

    //! Set world axis names
    bool setAxisNames (std::string const &names);
    //! Set world axis units
    bool setAxisUnits (std::string const &units);
    //! Set the values along the pixel and world axis
    bool setAxisValues (std::vector<double> const &pixelValues,
			std::vector<T> const &worldValues) {
      bool status (true);
      
      if (pixelValues.size() == worldValues.size()) {
	// adjust array sizes
	this->pixelValues_p.resize(pixelValues.size());
	this->worldValues_p.resize(worldValues.size());
	// copy values
	this->pixelValues_p = pixelValues;
	this->worldValues_p = worldValues;
      }
      else {
	status = false;
      }
      
      return status;
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TabularCoordinate.
    */
    inline std::string className () const {
      return "TabularCoordinate";
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
    
    // === Public methods =======================================================

#ifdef HAVE_HDF5    
    //! Write the coordinate object to a HDF5 file
    void write_hdf5 (hid_t const &groupID);
    
    //! Write the coordinate object to a HDF5 file
    void write_hdf5 (hid_t const &locationID,
		  std::string const &name);
    
    //! Read the coordinate object from a HDF5 file
    void read_hdf5 (hid_t const &groupID);
    
    //! Read the coordinate object from a HDF5 file
    void read_hdf5 (hid_t const &locationID,
		    std::string const &name);
#endif
    
#ifdef HAVE_CASA
    //! Create coordinate from casa::Coordinate object
    void importCoordinate (casa::TabularCoordinate const &coord);
    //! Create casa::Coordinate object from coordinate parameters
    void exportCoordinate (casa::TabularCoordinate &coord);
#endif

  private:
    
    //! Initialize internal parameters
    void init (std::string const &axisNames="UNDEFINED",
	       std::string const &axisUnits="UNDEFINED") {
      // arrays to be passed on
      std::vector<std::string> names (1,axisNames);
      std::vector<std::string> units (1,axisUnits);
      std::vector<double> pixel;
      std::vector<T> world;
      // forward function call
      init (names, units, pixel, world);
    }

    //! Initialize internal parameters
    void init (std::vector<std::string> const &axisNames,
	       std::vector<std::string> const &axisUnits,
	       std::vector<double> const &pixelValues,
	       std::vector<T> const &worldValues);
    
    //! Unconditional copying
    void copy (TabularCoordinate const &other);
    
    //! Unconditional deletion
    void destroy(void) {}
    
  }; // Class TabularCoordinate -- end
  
} // Namespace DAL -- end

#endif /* TABULARCOORDINATE_H */

