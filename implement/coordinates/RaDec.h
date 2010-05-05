/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 4027 2010-01-21 13:05:37Z baehren                    $ |
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

#ifndef RADEC_H
#define RADEC_H

// Standard library header files
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <dalCommon.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class RaDec
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Representation of a RA-DEC coordinate
    
    \author Lars B&auml;hren

    \date 2010/05/05

    \test tRaDec.cc
    
    <h3>Synopsis</h3>

    A simple container for holding the components of a (RA,Dec) position.
    
    <h3>Example(s)</h3>

    <ol>
      <li>Use public static functions to convert position from radian to degrees:
      \code
      double RA_radian (2.0);
      std::string RA_degrees;

      RA_degrees = RaDec::toDegreesRA (RA_radian);
      \endcode
      Instead of retrieving the position in degrees as a formatted string, it also
      is possible to retrieve the individual components:
      \code
      double RA_radian (2.0);
      int hour;
      int min;
      double sec;

      RaDec::toDegreesRA (hour, min, sec, RA_radian);
      \endcode
    </ol>
    
  */  
  class RaDec {

    //! (RA,Dec) direction in radian
    std::vector<double> radian_p;
    //! (RA,Dec) direction in degrees
    std::vector<std::string> degrees_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    RaDec ();
    
    //! Argumented constructor
    RaDec (double const &ra,
	   double const &dec);
    
    //! Argumented constructor
    RaDec (std::vector<double> const &direction);
    
    /*!
      \brief Copy constructor
      
      \param other -- Another RaDec object from which to create this new
             one.
    */
    RaDec (RaDec const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~RaDec ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another RaDec object from which to make a copy.
    */
    RaDec& operator= (RaDec const &other); 
    
    // === Parameter access =====================================================

    //! Get (RA,Dec) direction (in radian)
    std::vector<double> radian () const {
      return radian_p;
    }

    //! Get (RA,Dec) direction (in degrees)
    std::vector<std::string> degrees () const {
      return degrees_p;
    }

    //! Get the Right Ascension (in radian)
    inline double RA () const {
      return radian_p[0];
    }

    //! Set the Right Ascension
    bool setRA (double const &ra);

    //! Get the Declination (in radian)
    inline double dec () const {
      return radian_p[1];
    }

    //! Set the Declination
    bool setDec (double const &dec);

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, RaDec.
    */
    inline std::string className () const {
      return "RaDec";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================

    //! Convert Right Ascension from radian to degrees
    static std::string toDegreesRA (double const &ra);
    //! Convert Right Ascension from radian to degrees
    static void toDegreesRA (int &hour,
			     int &min,
			     double &sec,
			     double const &ra);
    //! Convert Declination from radian to degrees
    static std::string toDegreesDec (double const &dec);
    //! Convert Declination from radian to degrees
    static void toDegreesDec (int &hour,
			     int &min,
			     double &sec,
			     double const &dec);
    
  private:

    //! Initialize the internal parameters
    bool init (double const &ra,
	       double const &dec);

    //! Convert representation from radian to degrees
    static std::string rad2deg (double const &rad);
    //! Convert representation from radian to degrees
    static void rad2deg (int &hour,
			 int &min,
			 double &sec,
			 double const &rad);
    
    //! Unconditional copying
    void copy (RaDec const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class RaDec -- end
  
} // Namespace DAL -- end

#endif /* RADEC_H */

