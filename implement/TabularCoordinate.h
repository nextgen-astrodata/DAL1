/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 2286 2009-02-03 10:50:48Z baehren                    $ |
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

#ifndef TABULARCOORDINATE_H
#define TABULARCOORDINATE_H

// Standard library header files
#include <iostream>
#include <string>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class TabularCoordinate
    
    \ingroup Coordinates
    
    \brief Brief description for class TabularCoordinate
    
    \author Lars Baehren

    \date 2009/06/24

    \test tTabularCoordinate.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class TabularCoordinate {
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    TabularCoordinate ();
    
    /*!
      \brief Copy constructor
      
      \param other -- Another TabularCoordinate object from which to create this new
             one.
    */
    TabularCoordinate (TabularCoordinate const &other);
    
    // -------------------------------------------------------------- Destruction

    //! Destructor
    ~TabularCoordinate ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TabularCoordinate object from which to make a copy.
    */
    TabularCoordinate& operator= (TabularCoordinate const &other); 
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TabularCoordinate.
    */
    inline std::string className () const {
      return "TabularCoordinate";
    }

    /*!
      \brief Provide a summary of the internal status
    */
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // ------------------------------------------------------------------ Methods
    
    
    
  private:
    
    //! Unconditional copying
    void copy (TabularCoordinate const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class TabularCoordinate -- end
  
} // Namespace DAL -- end

#endif /* TABULARCOORDINATE_H */
  
