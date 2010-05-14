/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#include <Angle.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                        Angle
  
  Angle::Angle ()
  {
    rad_p = 0.0;
    deg_p = 0.0;
  }
  
  //_____________________________________________________________________________
  //                                                                        Angle
  
  Angle::Angle (double const &val,
<<<<<<< HEAD
		bool const &angleInDegrees)
  {
    setAngle (val, angleInDegrees);
=======
		bool const &AngleInDegrees)
  {
    if (AngleInDegrees) {
      deg_p = val;
      rad_p = deg2rad (val);
    } else {
      rad_p = val;
      deg_p = rad2deg (val);
    }
>>>>>>> New class for the representation of an angle and to generate various ways to represent it.
  }
  
  //_____________________________________________________________________________
  //                                                                        Angle
  
  Angle::Angle (Angle const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
<<<<<<< HEAD
=======
  Angle::~Angle ()
  {
    destroy();
  }
  
>>>>>>> New class for the representation of an angle and to generate various ways to represent it.
  void Angle::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  Angle& Angle::operator= (Angle const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void Angle::copy (Angle const &other)
  {
    rad_p = other.rad_p;
    deg_p = other.deg_p;
  }

  // ============================================================================
  //
  //  Parameter access
  //
  // ============================================================================
  
  void Angle::setAngle (double const &val,
			bool const &angleInDegrees)
  {
    if (angleInDegrees) {
      deg_p = val;
      rad_p = deg2rad (val);
    } else {
      rad_p = val;
      deg_p = rad2deg (val);
    }
  }
  
  //_____________________________________________________________________________
  //                                                                         hms
  
  std::string Angle::hms ()
  {
    return rad2hms (rad_p);
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void Angle::summary (std::ostream &os)
  {
    os << "[Angle] Summary of internal parameters." << std::endl;
    os << "-- Angle in radian   = " << rad() << std::endl;
    os << "-- Angle in degrees  = " << deg() << std::endl;
    os << "-- Angle in HH:MM:SS = " << hms() << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  

  // ============================================================================
  //
  //  Static public methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      rad2deg
  
  /*!    
    \param rad  -- Angle in radian
    \return deg -- Angle in degrees
  */
  double Angle::rad2deg (double const &rad)
  {
    return rad*180/DAL::PI;
  }

  //_____________________________________________________________________________
  //                                                                      rad2deg
  
  /*!
    \retval deg -- Angle in degrees
    \param rad -- Angle in radian
  */
  void Angle::rad2deg (double &deg,
		double const &rad)
  {
    deg = rad2deg (rad);
  }
  
  //_____________________________________________________________________________
  //                                                                      rad2deg
  
  /*!
    \param rad  -- Vector with angles given in radian
    \return deg -- Vector with angles given in degree
  */
  vector<double> Angle::rad2deg (vector<double> const &rad)
  {
    vector<double> deg (rad.size());
    for (unsigned int n(0); n<rad.size(); n++) {
      deg[n] = rad2deg(rad[n]);
    }
    return deg;
  }
  
  //_____________________________________________________________________________
  //                                                                      deg2rad
  
  /*!
    \param deg  -- Angle in degrees
    \return rad -- Angle in radian
  */
  double Angle::deg2rad (double const &deg)
  {
    return deg*DAL::PI/180.0;
  }
  
  //_____________________________________________________________________________
  //                                                                      deg2rad
  
  /*!
    \retval rad -- Angle in radian
    \param deg  -- Angle in degrees
  */
  void Angle::deg2rad (double &rad,
		       double const &deg)
  {
    rad = deg2rad (deg);
  }
  
  //_____________________________________________________________________________
  //                                                                      deg2rad
  
  /*!
    \param deg -- Vector with angles given in degrees
    
    \return rad -- Vector with angles given in radian
  */
  vector<double> Angle::deg2rad (vector<double> const &deg)
  {
    unsigned nelem = deg.size();
    vector<double> rad (nelem);
    for (unsigned int n(0); n<nelem; n++) {
      rad[n] = deg2rad(deg[n]);
    }
    return rad;
  }

  //_____________________________________________________________________________
  //                                                                    angle2hms

  std::string Angle::angle2hms (double const &angle,
				bool const &angleInDegrees)
  {
    int hour;
    int min;
    double sec;
    std::string hms;
    
    if (angleInDegrees) {
      angle2hms (hour, min, sec, deg2rad(angle));
    } else {
      angle2hms (hour, min, sec, angle);
    }

    std::stringstream ss;
    ss << hour << ":" << min << ":" << sec;
    ss >> hms;
    
    return hms;
  }
  
  //_____________________________________________________________________________
  //                                                                    angle2hms

  void Angle::angle2hms (int &hour,
			 int &min,
			 double &sec,
			 double const &angle,
			 bool const &angleInDegrees)
  {
    double rad;
    double deg_hour;
    double deg_min;

    if (angleInDegrees) {
      rad = deg2rad (angle);
    } else {
      rad = angle;
    }
    
    deg_hour = ( rad * (180./PI) ) / 15.;
    hour     = (int)deg_hour;
    deg_min  = (deg_hour - hour) * 60.;
    min      = int( deg_min );
    sec      = (deg_min - min) * 60;
  }
  
  //_____________________________________________________________________________
  //                                                                      rad2hms
  
  /*!
    \param rad -- Angle in radian
    \return hms -- Angle as formatted string (H:M:S)
  */
  std::string Angle::rad2hms (double const &rad)
  {
    return angle2hms (rad, false);
  }
  
  //_____________________________________________________________________________
  //                                                                      rad2hms
  
  /*!
    \retval hms -- Angle as formatted string (H:M:S)
    \param rad -- Angle in radian
  */
  void Angle::rad2hms (std::string &hms,
		       double const &rad)
  {
    hms = rad2hms (rad);
  }

  //_____________________________________________________________________________
  //                                                                      deg2hms
  
  std::string Angle::deg2hms (double const &rad)
  {
    return rad2hms (deg2rad(rad));
  }

  //_____________________________________________________________________________
  //                                                                      deg2hms
  
  void Angle::deg2hms (std::string &hms,
		       double const &rad)
  {
    hms = deg2hms (rad);
  }  
  
} // Namespace DAL -- end
