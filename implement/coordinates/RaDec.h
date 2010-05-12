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

#ifndef RADEC_H
#define RADEC_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

// DAL header files
#include <Angle.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class RaDec
    
    \ingroup DAL
    \ingroup coordinates
    
    \brief Brief description for class RaDec
    
    \author Lars B&auml;hren

    \date 2010/05/11

    \test tRaDec.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class RaDec {

    std::vector<DAL::Angle> radec_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    RaDec ();
    
    //! Argumented constructor
    RaDec (double const &ra,
	   double const &dec,
	   bool const &angleInDegrees=false);
    
    //! Argumented constructor
    RaDec (std::vector<DAL::Angle> const &angles);
    
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
    
    //! Set Right Ascension angle
    bool setRA (double const &ra,
		bool const &angleInDegrees=false);
    
    //! Set Declination angle
    bool setDec (double const &dec,
		 bool const &angleInDegrees=false);
    
    //! Set both Right Ascension (RA) and Declination (Dec) angle
    bool setAngles (double const &ra,
		    double const &dec,
		    bool const &angleInDegrees=false);
    
    //! Set both Right Ascension (RA) and Declination (Dec) angle
    bool setAngles (std::vector<DAL::Angle> const &angles);
    
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
    
    
    
  private:
    
    //! Unconditional copying
    void copy (RaDec const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class RaDec -- end
  
} // Namespace DAL -- end

#endif /* RADEC_H */
  
