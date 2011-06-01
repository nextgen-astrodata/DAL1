/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars Baehren <bahren@astron.nl>                                       *
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

#ifndef RM_SOURCETABLE_H
#define RM_SOURCETABLE_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class RM_SourceTable
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Brief description for class RM_SourceTable
    
    \author Lars Bauml;hren

    \date 2010/10/12

    \test tRM_SourceTable.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class RM_SourceTable {

    //! Name of the table columns
    std::vector<std::string> itsTableColumns;
    
  public:
    
    // === Construction =========================================================
    
    //! Copy constructor
    RM_SourceTable (RM_SourceTable const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~RM_SourceTable ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another RM_SourceTable object from which to make a copy.
    */
    RM_SourceTable& operator= (RM_SourceTable const &other); 
    
    // === Parameter access =====================================================

    //! Name of the table columns
    inline std::vector<std::string> tableColumns () const {
      return itsTableColumns;
    }

    inline unsigned int nofColumns () const {
      return itsTableColumns.size();
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, RM_SourceTable.
    */
    inline std::string className () const {
      return "RM_SourceTable";
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
    void copy (RM_SourceTable const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class RM_SourceTable -- end
  
} // Namespace DAL -- end

#endif /* RM_SOURCETABLE_H */
  
