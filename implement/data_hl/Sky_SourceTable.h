/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#ifndef SKY_SOURCETABLE_H
#define SKY_SOURCETABLE_H

// Standard library header files
#include <iostream>
#include <string>
#include <map>

/* DAL header files */
#include <dal_config.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Sky_SourceTable
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Brief description for class Sky_SourceTable
    
    \author Lars B&auml;hren

    \date 2011/02/14

    \test tSky_SourceTable.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\ref dal_icd_004
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class Sky_SourceTable {

    //! nof. rows in the table
    unsigned int itsNofRows;
    //! nof. columns in the table
    unsigned int itsNofColumns;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    Sky_SourceTable ();
    
    //! Copy constructor
    Sky_SourceTable (Sky_SourceTable const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~Sky_SourceTable ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    Sky_SourceTable& operator= (Sky_SourceTable const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, Sky_SourceTable.
    */
    inline std::string className () const {
      return "Sky_SourceTable";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    
    
    // === Static methods =======================================================
    
    
    
  private:
    
    //! Unconditional copying
    void copy (Sky_SourceTable const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class Sky_SourceTable -- end
  
} // Namespace DAL -- end

#endif /* SKY_SOURCETABLE_H */
  
