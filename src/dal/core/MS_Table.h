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

#ifndef MS_TABLE_H
#define MS_TABLE_H

// Standard library header files
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// DAL header files
#include <core/dalObjectBase.h>

#ifdef DAL_WITH_CASA
#include <ms/MeasurementSets.h>
#include <tables/Tables/Table.h>
#include <tables/Tables/TableDesc.h>
#endif

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class MS_Table
    
    \ingroup DAL
    \ingroup core
    
    \brief Brief description for class DAL::MS_Table
    
    \author Lars B&auml;hren

    \date 2011-07-18

    \test tMS_Table.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class MS_Table : public dalObjectBase {

#ifdef DAL_WITH_CASA

    //! Table object
    casa::Table itsTable;

  public:
    
    // === Construction =========================================================

    //! Default constructor
    MS_Table ();
    
    //! Default constructor
    MS_Table (std::string const &name,
	      IO_Mode const &flags=IO_Mode());
    
    //! Copy constructor
    MS_Table (MS_Table const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~MS_Table ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    MS_Table& operator= (MS_Table const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, MS_Table.
    */
    inline std::string className () const {
      return "MS_Table";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Public methods =======================================================
    
    //! Open table by \e name
    bool open (std::string const &name,
	       IO_Mode const &flags=IO_Mode());
    
    //! Open sub-table by \e name
    bool open (MS_Table &table,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode());
    

    //! Get a table description object for the table
    inline casa::TableDesc tableDescription () {
      return itsTable.tableDesc ();
    }

    //! Get the names of the table columns
    std::vector<std::string> columnNames ();

    /*!
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column  -- Name of the \e column from which to read the \e data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (casa::Array<T> &data,
		     std::string const &column)
      {
	bool status = true;
	casa::TableDesc tableDesc   = itsTable.tableDesc ();
	casa::ColumnDesc columnDesc = tableDesc.columnDesc(column);

	/*
	  Get the shape of the colum in order to resize the array returning
	  the data.
	*/
	data.resize(columnDesc.shape());
	
	if (columnDesc.isScalar()) {
	  // Set up reader object for the column ...
	  casa::ROScalarColumn<T> columReader (itsTable, column);
	  // .... and retrieve the data
	  data = columReader.getColumn();
	} else if (columnDesc.isArray()) {
	  // Set up reader object for the column ...
	  casa::ROArrayColumn<T> columReader (itsTable, column);
	  // .... and retrieve the data
	  data = columReader.getColumn();
	} else {
	  std::cerr << "[MS_Table::readData] Unsupported type of column data!"
		    << std::endl;
	  status = false;
	}

	return status;
      }

    // === Static methods =======================================================
    
    
    
  private:

    //! Resolve the \e name for the MS table
    bool resolveName (std::string &absoluteName,
		      std::string const &name);

    //! Unconditional copying
    void copy (MS_Table const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
#else 

  public:
    MS_Table ();
    
#endif
    
  }; // Class MS_Table -- end
  
} // Namespace DAL -- end

#endif /* MS_TABLE_H */
  
