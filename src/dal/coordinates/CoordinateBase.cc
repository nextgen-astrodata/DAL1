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

#include <coordinates/CoordinateBase.h>

namespace DAL {
  
  //_____________________________________________________________________________
  //                                                                         init
  
  template <>
  void CoordinateBase<std::string>::init (DAL::Coordinate const &coord,
					  unsigned int const &nofAxes,
					  DAL::Coordinate const &storageType)
  {
    std::string undefined ("UNDEFINED");
    
    /* Initialize the size of the internal arrays */
    
    axisNames_p.clear();
    axisUnits_p.clear();
    refValue_p.clear();
    refPixel_p.clear();
    increment_p.clear();
    pc_p.clear();
    itsPixelValues.clear();
    itsWorldValues.clear();
    
    /* Initialize internal variables storing coordinate parameters */
    
    coord_p       = coord;
    storageType_p = storageType;
    
    if (nofAxes > 0) {
      // set the number of coordinate axes
      nofAxes_p = nofAxes;
      // Adjust the size of the internal arrays
      axisNames_p.resize(nofAxes);
      axisUnits_p.resize(nofAxes);
      refValue_p.resize(nofAxes);
      refPixel_p.resize(nofAxes);
      increment_p.resize(nofAxes);
      // Fill in default values for the WCS parameters
      for (unsigned int n(0); n<nofAxes; ++n) {
	axisNames_p[n] = undefined;
	axisUnits_p[n] = undefined;
	refValue_p[n]  = undefined;
	refPixel_p[n]  = 0;
	increment_p[n] = 0;
      }
      // Transformation is identity matrix
      DAL::IdentityMatrix (pc_p,nofAxes);
    } else {
      // set the number of coordinate axes
      nofAxes_p = 0;
    }
    
    /* Set up the basic set of attributes */
    setAttributes ();
  };
  
} // Namespace DAL -- end
