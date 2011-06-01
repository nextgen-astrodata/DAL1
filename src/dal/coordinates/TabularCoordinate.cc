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

#include <coordinates/TabularCoordinate.h>

namespace DAL {  // Namespace DAL -- begin

  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  template <>
  TabularCoordinate<std::string>::TabularCoordinate ()
  {
    TabularCoordinate<std::string>::init();
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  /*!
    \param other -- Other TabularCoordinate object to make copy of.
  */
  template <class T>
  void TabularCoordinate<T>::copy (TabularCoordinate const &other)
  {
    CoordinateBase<T>::copy (other);
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  template <>
  void TabularCoordinate<std::string>::init (std::vector<std::string> const &axisNames,
					     std::vector<std::string> const &axisUnits,
					     std::vector<double> const &pixelValues,
					     std::vector<std::string> const &worldValues)
  {
    /* Initialize base class */
    CoordinateBase<std::string>::init (Coordinate::TABULAR,
					    1,
					    Coordinate::TABULAR);
    CoordinateBase<std::string>::setAxisNames (axisNames);
    CoordinateBase<std::string>::setAxisUnits (axisUnits);
    setAxisValues (pixelValues, worldValues);
  }
  
  //_____________________________________________________________________________
  //                                                             exportCoordinate
  
#ifdef DAL_WITH_CASA
  template <class T>
  void TabularCoordinate<T>::exportCoordinate (casa::TabularCoordinate &coord)
  {
    unsigned int nelem = this->itsPixelValues.size();
    casa::Vector<double> pixel (nelem);
    casa::Vector<T> world (nelem);
    
    for (unsigned int n(0); n<nelem; ++n) {
      pixel(n) = this->itsPixelValues[n];
      world(n) = this->itsWorldValues[n];
    }
    
    coord = casa::TabularCoordinate (pixel,
				     world,
				     this->axisUnits_p[0],
				     this->axisNames_p[0]);
  }
#endif
  
} // Namespace DAL -- end
