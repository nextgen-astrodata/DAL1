/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#ifndef COORDINATECONVERSION_H
#define COORDINATECONVERSION_H

// Standard library header files
#include <iostream>
#include <string>
#include <sstream>

#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \file CoordinateConversion.h
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief A collection of methods for the conversion of/between coordinates
    
    \author Lars B&auml;hren

    \date 2010/04/23

    \test tCoordinateConversion.cc
    
    <h3>Synopsis</h3>

    There are a number of commonly used conversions between different types and 
    representations of coordinates; one of the simplest is the conversion between
    angles and degrees, but of course there are more complex ones.
    
    <h3>Example(s)</h3>
    
  */  
  
  // === Conversion between angles and degrees ==================================

  //! Convert radian to degrees
  double deg2rad (double const &deg);  
  //! Convert radian to degrees
  void deg2rad (double &rad,
		double const &deg);
  //! Convert radian to degrees
  vector<double> deg2rad (vector<double> const &deg);

  //! Convert radian to degrees
  double rad2deg (double const &rad);
  //! Convert radian to degrees
  void rad2deg (double &deg,
		double const &rad);
  //! Convert radian to degrees
  vector<double> rad2deg (vector<double> const &rad);

  std::string RArad2deg (double const &rad);


} // Namespace DAL -- end

#endif /* COORDINATECONVERSION_H */
  
