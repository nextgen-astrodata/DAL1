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

#include <CoordinateConversion.h>

namespace DAL { // Namespace DAL -- begin

  // ============================================================================
  //
  //  Conversion between angles and degrees
  //
  // ============================================================================
  
  /*!
    \param deg  -- Angle in degrees
    \return rad -- Angle in radian
  */
  double deg2rad (double const &deg)
  {
    return deg*DAL::PI/180.0;
  }

  /*!
    \retval rad -- Angle in radian
    \param deg  -- Angle in degrees
  */
  void deg2rad (double &rad,
		double const &deg)
  {
    rad = deg2rad (deg);
  }
  
  /*!
    \param deg -- Vector with angles given in degrees
    
    \return rad -- Vector with angles given in radian
  */
  vector<double> deg2rad (vector<double> const &deg)
  {
    unsigned nelem = deg.size();
    vector<double> rad (nelem);
    for (unsigned int n(0); n<nelem; n++) {
      rad[n] = deg2rad(deg[n]);
    }
    return rad;
  }

  /*!    
    \param rad  -- Angle in radian
    \return deg -- Angle in degrees
  */
  double rad2deg (double const &rad)
  {
    return rad*180/DAL::PI;
  }

  /*!
    \retval deg -- Angle in degrees
    \param rad -- Angle in radian
  */
  void rad2deg (double &deg,
		double const &rad)
  {
    deg = rad2deg (rad);
  }
  
  /*!
    \param rad  -- Vector with angles given in radian
    \return deg -- Vector with angles given in degree
  */
  vector<double> rad2deg (vector<double> const &rad)
  {
    vector<double> deg (rad.size());
    for (unsigned int n(0); n<rad.size(); n++) {
      deg[n] = rad2deg(rad[n]);
    }
    return deg;
  }
  
  //_____________________________________________________________________________
  //                                                                    RArad2deg

  std::string RArad2deg (double const &rad)
  {
    std::string deg = "";
    double deg_hour = ( rad * (180./DAL::PI) ) / 15.;
    int   hour = (int)deg_hour;
    double deg_min = (deg_hour - hour) * 60.;
    int    min = int( deg_min );
    double  sec = (deg_min - min) * 60;
    
    //sprintf(ra_string,"%02d:%02d:%02.4f",  , min, sec);
    std::stringstream rass;
    rass << hour << ":" << min << ":" << sec;
    rass >> deg;
    
    return deg; 
  }
  
  
} // Namespace DAL -- end
