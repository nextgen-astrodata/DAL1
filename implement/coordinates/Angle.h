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

#ifndef ANGLE_H
#define ANGLE_H

// Standard library header files
#include <iostream>
#include <string>

#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Angle
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Class to describe the properties of an angle
    
    \author Lars B&auml;hren

    \date 2010/05/11

    \test tAngle.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */
  class Angle {

    //! Angle representation in Radian
    double rad_p;
    //! Angle representation in degrees
    double deg_p;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    Angle ();
    
    //! Default constructor
    Angle (double const &val,
	   bool const &angleInDegrees=false);
    
    /*!
      \brief Copy constructor
      
      \param other -- Another Angle object from which to create this new
             one.
    */
    Angle (Angle const &other);
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another Angle object from which to make a copy.
    */
    Angle& operator= (Angle const &other); 
    
    // === Parameter access =====================================================
    
    //! Set the angle
    void setAngle (double const &val,
		   bool const &angleInDegrees=false);
    
    //! Get the angle in radian
    inline double rad () const {
      return rad_p;
    }
    
    //! Get the angle in degrees
    inline double deg () const {
      return deg_p;
    }
    
    //! Get the angle as formatted string (HH:MM:SS)
    std::string hms ();
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, Angle.
    */
    inline std::string className () const {
      return "Angle";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================

    /* Convert radian to degree */
    
    //! Convert radian to degrees
    static double rad2deg (double const &rad);
    //! Convert radian to degrees
    static void rad2deg (double &deg,
		  double const &rad);
    //! Convert radian to degrees
    static vector<double> rad2deg (vector<double> const &rad);

    /* Convert degree to radian */
    
    //! Convert radian to degrees
    static double deg2rad (double const &deg);  
    //! Convert radian to degrees
    static void deg2rad (double &rad,
			 double const &deg);
    //! Convert radian to degrees
    static vector<double> deg2rad (vector<double> const &deg);

    /* Convert angle to formatted string */
    
    //! Convert angle to formatted string (H:M:S)
    static std::string angle2hms (double const &angle,
				  bool const &angleInDegrees=false);
    //! Convert angle to formatted string (H:M:S)
    static void angle2hms (int &hour,
			   int &min,
			   double &sec,
			   double const &angle,
			   bool const &angleInDegrees=false);
    
    //! Convert angle (in rad) to formatted string (H:M:S)
    static std::string rad2hms (double const &rad);
    //! Convert angle (in rad) to formatted string (H:M:S)
    static void rad2hms (std::string &hms,
		  double const &rad);
    
    //! Convert angle (in deg) to formatted string (H:M:S)
    static std::string deg2hms (double const &deg);
    //! Convert angle (in deg) to formatted string (H:M:S)
    static void deg2hms (std::string &hms,
			 double const &deg);

  private:
    
    //! Unconditional copying
    void copy (Angle const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class Angle -- end
  
} // Namespace DAL -- end

#endif /* ANGLE_H */
