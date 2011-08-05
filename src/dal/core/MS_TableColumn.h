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

#ifndef MS_TABLECOLUMN_H
#define MS_TABLECOLUMN_H

// Standard library header files
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// DAL header files
#include <core/MS_Table.h>

#ifdef DAL_WITH_CASA
#include <casa/Arrays/Slicer.h>
#include <tables/Tables/TableColumn.h>
#endif

namespace DAL { // Namespace DAL -- begin

  /* Forward declaration of MS_Table class */
  class MS_Table;
  
  /*!
    \class MS_TableColumn
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class DAL::MS_TableColumn
    
    \author Lars B&auml;hren

    \date 2011-08-02

    \test tMS_TableColumn.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class MS_TableColumn : public dalObjectBase {

#ifdef DAL_WITH_CASA

    //! Table column object
    casa::TableColumn itsTableColumn;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    MS_TableColumn ();
    
    //! Argumented constructor
    MS_TableColumn (MS_Table const &table,
		    std::string const &name,
		    IO_Mode const &flags=IO_Mode());
    
    //! Copy constructor
    MS_TableColumn (MS_TableColumn const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~MS_TableColumn ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    MS_TableColumn& operator= (MS_TableColumn const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, MS_TableColumn.
    */
    inline std::string className () const {
      return "MS_TableColumn";
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
    void copy (MS_TableColumn const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
#else 

  public:
    MS_TableColumn ();
    
#endif

  }; // Class MS_TableColumn -- end
  
} // Namespace DAL -- end

#endif /* MS_TABLECOLUMN_H */
  
