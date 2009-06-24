/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1964 2008-09-06 17:52:38Z baehren                   $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                  *
 *   Lars Baehren (<mail>)                                                     *
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

#include <TabularCoordinate.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                            TabularCoordinate
  
  TabularCoordinate::TabularCoordinate ()
    : Coordinate(Coordinate::Linear,
		 1)
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                            TabularCoordinate
  
  TabularCoordinate::TabularCoordinate (std::vector<std::string> const &axisNames,
					std::vector<std::string> const &axisUnits,
					std::vector<double> const &pixelValues,
					std::vector<double> const &worldValues)
    : Coordinate(Coordinate::Linear,
		 1)
  {
    setAxisNames (axisNames);
    setAxisUnits (axisUnits);
    setPixelValues (pixelValues);
    setWorldValues (worldValues);
  }

  //_____________________________________________________________________________
  //                                                            TabularCoordinate
  
  TabularCoordinate::TabularCoordinate (TabularCoordinate const &other)
    : Coordinate (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  TabularCoordinate::~TabularCoordinate ()
  {
    destroy();
  }
  
  void TabularCoordinate::destroy ()
  {}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  TabularCoordinate& TabularCoordinate::operator= (TabularCoordinate const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  void TabularCoordinate::copy (TabularCoordinate const &other)
  {
    Coordinate::copy (other);

    pixelValues_p.resize(other.pixelValues_p.size());
    worldValues_p.resize(other.worldValues_p.size());
    
    pixelValues_p = other.pixelValues_p;
    worldValues_p = other.worldValues_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                               setPixelValues
  
  bool TabularCoordinate::setPixelValues (std::vector<double> const &pixelValues)
  {
    bool status (true);

    if (pixelValues.size() == pixelValues_p.size()) {
      pixelValues_p = pixelValues;
    } else {
      status = false;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                               setWorldValues
  
  bool TabularCoordinate::setWorldValues (std::vector<double> const &worldValues)
  {
    bool status (true);

    if (worldValues.size() == worldValues_p.size()) {
      worldValues_p = worldValues;
    } else {
      status = false;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                setAxisValues
  
  bool TabularCoordinate::setAxisValues (std::vector<double> const &pixelValues,
					 std::vector<double> const &worldValues)
  {
    bool status (true);
    
    if (pixelValues.size() == worldValues.size()) {
      // adjust array sizes
      pixelValues_p.resize(pixelValues.size());
      worldValues_p.resize(worldValues.size());
      // copy values
      pixelValues_p = pixelValues;
      worldValues_p = worldValues;
    } else {
      status = false;
    }

    return status;
  }

  void TabularCoordinate::summary (std::ostream &os)
  {
    os << "[TabularCoordinate] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  void TabularCoordinate::h5write (hid_t const &groupID)
  {}
  
  void TabularCoordinate::h5write (hid_t const &locationID,
				   std::string const &name)
  {}
  
  void TabularCoordinate::h5read (hid_t const &groupID)
  {}
  
  void TabularCoordinate::h5read (hid_t const &locationID,
				  std::string const &name)
  {}
  
  
} // Namespace DAL -- end
