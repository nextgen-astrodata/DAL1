/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars Baehren (lbaehren@gmail.com)                                     *
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

#ifndef MS_COLUMN_H
#define MS_COLUMN_H

// Standard library header files
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// DAL header files
#include <core/dalObjectBase.h>

#ifdef DAL_WITH_CASA
#include <casa/Arrays/Slicer.h>
#include <tables/Tables/TableColumn.h>
#endif

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class MS_Column
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class DAL::MS_Column
    
    \author Lars B&auml;hren

    \date 2011-08-02

    \test tMS_Column.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class MS_Column : public dalObjectBase {

    //! Table column object
    casa::TableColumn itsTableColumn;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    MS_Column ();
    
    //! Copy constructor
    MS_Column (MS_Column const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~MS_Column ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    MS_Column& operator= (MS_Column const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, MS_Column.
    */
    inline std::string className () const {
      return "MS_Column";
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
    void copy (MS_Column const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class MS_Column -- end
  
} // Namespace DAL -- end

#endif /* MS_COLUMN_H */
  
