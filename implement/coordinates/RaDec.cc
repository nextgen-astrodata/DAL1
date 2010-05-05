/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 3829 2009-12-09 13:04:56Z baehren                   $ |
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

#include <RaDec.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                        RaDec
  
  RaDec::RaDec ()
  {
    init (0.0, 0.0);
  }
  
  //_____________________________________________________________________________
  //                                                                        RaDec
  
  RaDec::RaDec (double const &ra,
		double const &dec)
  {
    init (ra, dec);
  }
  
  //_____________________________________________________________________________
  //                                                                        RaDec
  
  RaDec::RaDec (std::vector<double> const &direction)
  {
    if (direction.size() == 2) {
      init (direction[0], direction[1]);
    } else {
      // Issue warning
      std::cerr << "[RaDec] Wrong number of elements in input vector!"
		<< std::endl;
      // Set default values
      init (0.0, 0.0);
    }
  }

  //_____________________________________________________________________________
  //                                                                        RaDec
  
  RaDec::RaDec (RaDec const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  RaDec::~RaDec ()
  {
    destroy();
  }
  
  void RaDec::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  RaDec& RaDec::operator= (RaDec const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void RaDec::copy (RaDec const &other)
  {
    radian_p.resize(other.radian_p.size());
    degrees_p.resize(other.degrees_p.size());

    radian_p  = other.radian_p;
    degrees_p = other.degrees_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void RaDec::summary (std::ostream &os)
  {
    os << "[RaDec] Summary of internal parameters." << std::endl;
    os << "-- Direction in radian  = " << radian_p  << std::endl;
    os << "-- Direction in degrees = " << degrees_p << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                        setRA
  
  bool RaDec::setRA (double const &ra)
  {
    bool status (true);

    // set representation in radian
    radian_p[0] = ra;
    // Set representation in degrees
    degrees_p[0] = rad2deg (ra);

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                       setDec
  
  bool RaDec::setDec (double const &dec)
  {
    bool status (true);

    // set representation in radian
    radian_p[1] = dec;
    // Set representation in degrees
    degrees_p[1] = rad2deg (dec);

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                         init
  
  bool RaDec::init (double const &ra,
		    double const &dec)
  {
    bool status (true);

    radian_p.clear();
    degrees_p.clear();

    radian_p.resize(2);
    degrees_p.resize(2);

    status *= setRA (ra);
    status *= setDec (dec);

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      rad2deg
  
  /*!
    \param rad -- Direction in radian.

    \return deg -- Direction in degrees, HH:MM:SS
  */
  std::string RaDec::rad2deg (double const &rad)
  {
    int hour;
    int min;
    double sec;
    std::string deg;

    rad2deg (hour, min, sec, rad);
    
    std::stringstream ss;
    ss << hour << ":" << min << ":" << sec;
    ss >> deg;
    
    return deg;
  }

  //_____________________________________________________________________________
  //                                                                      rad2deg
  
  /*!
    \retval hour -- Hour angle.
    \retval min  -- Minute angle.
    \retval sec  -- Seconds angle.
    \param rad   -- Direction in radian.
  */
  void RaDec::rad2deg (int &hour,
		       int &min,
		       double &sec,
		       double const &rad)
  {
    double deg_hour;
    double deg_min;

    deg_hour = ( rad * (180./PI) ) / 15.;
    hour     = (int)deg_hour;
    deg_min  = (deg_hour - hour) * 60.;
    min      = int( deg_min );
    sec      = (deg_min - min) * 60;
  }
  
  //_____________________________________________________________________________
  //                                                                  toDegreesRA
  
  /*!
    \param da -- Right Ascension in radian.

    \return deg -- Right Ascension in degrees, HH:MM:SS
  */
  std::string RaDec::toDegreesRA (double const &ra)
  {
    return rad2deg (ra);
  }

  //_____________________________________________________________________________
  //                                                                  toDegreesRA
  
  /*!
    \retval hour -- Hour angle.
    \retval min  -- Minute angle.
    \retval sec  -- Seconds angle.
    \param rad   -- Right Ascension in radian.
  */
  void RaDec::toDegreesRA (int &hour,
			   int &min,
			   double &sec,
			   double const &ra)
  {
    rad2deg (hour, min, sec, ra);
  }
  
  //_____________________________________________________________________________
  //                                                                 toDegreesDec
  
  /*!
    \param dec -- Declination in radian.

    \return deg -- Declination in degrees, HH:MM:SS
  */
  std::string RaDec::toDegreesDec (double const &dec)
  {
    return rad2deg (dec);
  }

  //_____________________________________________________________________________
  //                                                                 toDegreesDec
  
  /*!
    \retval hour -- Hour angle.
    \retval min  -- Minute angle.
    \retval sec  -- Seconds angle.
    \param rad   -- Declination in radian.
  */
  void RaDec::toDegreesDec (int &hour,
			   int &min,
			   double &sec,
			   double const &dec)
  {
    rad2deg (hour, min, sec, dec);
  }
  
} // Namespace DAL -- end
