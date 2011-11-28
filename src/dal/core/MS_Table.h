/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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
#include <set>
#include <string>
#include <vector>

// DAL header files
#include <core/dalObjectBase.h>
#include <core/Operator.h>

#ifdef DAL_WITH_CASA
#include <casa/Arrays/Slicer.h>
#include <ms/MeasurementSets.h>
#include <tables/Tables/Table.h>
#include <tables/Tables/TableDesc.h>
#include <tables/Tables/TableRecord.h>
#include <tables/Tables/ExprNode.h>
#endif

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class MS_Table
    
    \ingroup DAL
    \ingroup core
    
    \brief Interface to MeasurementSet table
    
    \author Lars B&auml;hren

    \date 2011-07-18

    \test tMS_Table.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\c casa::Table - Main interface class to a read/write table.
      <li>\c casa::TableExprNode - Handle class for a table column expression tree.
      <li>\c casa::ROArrayColumn<T> - Readonly access to an array table column
      with arbitrary data type.
      \code
      // Get the data from a particular cell in the table column
      casa::ROArrayColumn<T>::get (uInt rownr, Array<T> &array, Bool resize=False);
      // Get the data from a set of cells in the table column
      void casa::ROArrayColumn< T >::getColumnRange (const Slicer &rowRange,
                                                     Array<T> &arr,
                                                     Bool resize = False)
      \endcode
      <li>\c casa::ROScalarColumn<T> - Readonly access to a scalar table column
      with arbitrary data type.
      \code
      // Get the data from a particular cell in the table column
      casa::ROScalarColumn<T>::get (uInt rownr, T &value);
      // Get the data from a set of cells in the table column
      void casa::ROScalarColumn<T>::getColumnRange (const Slicer &rowRange,
                                                    Vector<T> &vec,
                                                    Bool resize=False)
      \endcode
      <li>\c casa::Slicer - Specify which elements to extract from an n-dimensional
      array.
      <li><a href="http://casa.nrao.edu/Memos/msselection/index.html">MeasurementSet
      Selection Syntax</a> (CASA Memo)
      <li><a href="http://www.astron.nl/aips++/docs/notes/199">AIPS++ Note 
      199</a>: Table Query Language
    </ul>
    
    <h3>Synopsis</h3>

    The MAIN table of the MS contains the bulk of all data. For each interferometer
    and for each sample time. The ordering is usually time-baseline, i.e. the MAIN
    table is divided in subsequent time-blocks and within each time block there is
    an ordering based on the interferometer antenna pair. Each interferometer pair
    will appear only once per sample time. Auto- and crosscorrelations are usually
    mixed. The MAIN table links directly to many other tables through index numbers
    in several of its columns.

    \verbatim
    /  MAIN                        Table     Data of all samples for individual interferometers
    |-- UVW                        Column    
    |-- FLAG_CATEGORY              Column    
    |-- WEIGHT                     Column    
    |-- SIGMA                      Column    
    |-- ANTENNA1                   Column    
    |-- ANTENNA2                   Column    
    |-- ARRAY_ID                   Column    
    |-- DATA_DESC_ID               Column    
    |-- EXPOSURE                   Column    
    |-- ANTENNA                    Table     Antenna information
    |-- DATA_DESCRIPTION           Table
    |-- FEED                       Table     Feed (Frontend) related information
    |-- FLAG_CMD                   Table     Flag information
    |-- FIELD                      Table     Information on observed positions
    |-- HISTORY                    Table     History log of MS
    |-- OBSERVATION                Table     General observation information
    |-- POINTING                   Table     Antenna pointing information
    |-- POLARIZATION               Table     Polarization information description
    |-- PROCESSOR                  Table     Correlator information
    |-- SPECTRAL_WINDOW            Table     Frequency/IF information
    `-- STATE
    \endverbatim

    <b>Selection of columns and column rows.</b> casacore, through the Table
    Query Language (TaQL), provides the means to perform SQL-like selections on
    the contents of a table.
    
    <h3>Example(s)</h3>

    <ol>
      <li>Simple argumented constructor, taking name of the table as input:
      \code
      MS_Table ms (table);
      \endcode
      <li>Argumented constructor, allowing to directly open up sub-table
      (\e subtable) below the root table (\e table):
      \code
      MS_Table ms (table, subtable);
      \endcode
      <li>Select table entries for baselines including a specific antenna:
      \code
      ms.setSelection ("ANTENNA1", DAL::Operator::Equal, 1);
      \endcode
    </ol>
    
  */  
  class MS_Table : public dalObjectBase {

#ifdef DAL_WITH_CASA

  protected:

    //! Table object
    casa::Table itsTable;
    //! Table expression node for selection
    casa::TableExprNode itsExpressionNode;
    //! Table selection (reference into the original table)
    casa::Table itsTableSelection;
    //! Name of the columns within this table
    std::set<std::string> itsColumnNames;
    //! Name of the sub-tables within this table
    std::set<std::string> itsTableNames;

  public:
    
    // === Construction =========================================================

    //! Default constructor
    MS_Table ();
    
    //! Argumented constructor to open table of given \e name
    MS_Table (std::string const &name,
	      IO_Mode const &flags=IO_Mode());
    
    //! Argumented constructor to open sub-table contained within \e table
    MS_Table (std::string const &table,
	      std::string const &subtable,
	      IO_Mode const &flags=IO_Mode());
    
    //! Argumented constructor to open sub-table contained within \e table
    MS_Table (casa::Table const &table,
	      std::string const &subtable,
	      IO_Mode const &flags=IO_Mode());
    
    //! Copy constructor
    MS_Table (MS_Table const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    virtual ~MS_Table ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    MS_Table& operator= (MS_Table const &other); 
    
    // === Parameter access =====================================================

    /*!
      \brief Get the name of the class
      \return className -- The name of the class, MS_Table.
    */
    virtual inline std::string className () const {
      return "MS_Table";
    }

    /*!
      \brief Provide a summary of the object's internal parameters and status
      \param showColumns -- Also show summary of the table columns?
    */
    virtual inline void summary (bool const &showColumns=true) {
      summary (std::cout,showColumns);
    }
    
    //! Provide a summary of the object's internal parameters and status
    virtual void summary (std::ostream &os,
			  bool const &showColumns=true);    
    
    // === Public methods =======================================================
    
    //! Open table by \e name
    virtual bool open (std::string const &name,
		       IO_Mode const &flags=IO_Mode());
    
    //! Open sub-table by \e name
    virtual bool open (casa::Table const &table,
		       std::string const &subtable,
		       IO_Mode const &flags=IO_Mode());
    
    //! Test if the table is the root table
    inline bool isRootTable () {
      try {
	return itsTable.isRootTable();
      } catch (casa::AipsError x) {
	return false;
      }
    }
    
    //! Get a table description object for the table
    inline casa::TableDesc tableDescription () {
      return itsTableSelection.tableDesc ();
    }

    //! Test if a column with this \e name exists. 
    bool hasColumn (std::string const &name);

    //! Get the names of the table columns
    inline std::set<std::string> columnNames () {
      return itsColumnNames;
    }

    //! Get the names of the sub-tables
    inline std::set<std::string> tableNames () {
      return itsTableNames;
    }

    //! Test if a sub-table with this \e name exists. 
    bool hasTable (std::string const &name);

    //! Get column data-types
    std::map<casa::String,casa::DataType> columnDataTypes ();

    //! Get the table expression node used for selection of table contents
    inline casa::TableExprNode expressionNode () const {
      return itsExpressionNode;
    }

    //! Does the table have an active selection applied to it?
    bool hasSelection ();

    /*!
      \brief Set expression node for selection of table contents
      \param column -- Name of the column, to which the selection will be applied.
      \param op     -- Operator definition the condition for the application of the
             \e value.
      \param value  -- Selection value.
      \param overwrite -- Overwrite existing selection? If set \e true, any
             previously existing table selection will be cleared; by dedault the
	     new selection is being appended to the already existing one.
    */
    template <class T>
      bool setSelection (std::string const &column,
			 DAL::Operator::Types const &op,
			 T const &value,
			 bool const &overwrite=false)
      {
	if (!hasColumn(column)) {
	  std::cerr << "[MS_Table::setSelection] No such column " << column
		    << " - unable to perform selection!" 
		    << std::endl;
	  return false;
	}

	casa::TableExprNode sel;
	
	switch (op) {
	case Operator::Equal:
	  sel = casa::TableExprNode (itsTable.col(column) == value);
	  break;
	case Operator::Not:
	  sel = casa::TableExprNode (itsTable.col(column) != value);
	  break;
	case Operator::Greater:
	  sel = casa::TableExprNode (itsTable.col(column) > value);
	  break;
	case Operator::GreaterEqual:
	  sel = casa::TableExprNode (itsTable.col(column) >= value);
	  break;
	case Operator::Lesser:
	  sel = casa::TableExprNode (itsTable.col(column) < value);
	  break;
	case Operator::LesserEqual:
	  sel = casa::TableExprNode (itsTable.col(column) <= value);
	  break;
	default:
	  std::cerr << "[MS_Table::setSelection] Unsupported operator type!"
		    << std::endl;
	  return false;
	  break;
	};

	return setSelection (sel, overwrite);
      }
    
    //! Set expression node for selection of table contents
    bool clearSelection ();
    
    /*!
      \brief Read data from a table column.
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

	// Check if the requested column does exist
	if (hasColumn(column)) {

	  // Get column description
	  casa::TableDesc tableDesc   = itsTableSelection.tableDesc();
	  casa::ColumnDesc columnDesc = tableDesc.columnDesc(column);
	  casa::IPosition cellShape   = columnDesc.shape();

	  // Resize array returning the data
	  {
	    unsigned int rank = cellShape.nelements()+1;
	    casa::IPosition dataShape (rank,0);
	    
	    for (unsigned int n=0; n<(rank-1); ++n) {
	      dataShape(n) = cellShape(n);
	    }
	    dataShape(rank-1) = itsTableSelection.nrow();
	    
	    data.resize(dataShape);
	  }
	  
	  // Set up reader object for the column and retrieve the data
	  if (columnDesc.isScalar()) {
	    casa::ROScalarColumn<T> columReader (itsTableSelection, column);
	    data = columReader.getColumn();
	  } else if (columnDesc.isArray()) {
	    casa::ROArrayColumn<T> columReader (itsTableSelection, column);
	    data = columReader.getColumn();
	  } else {
	    std::cerr << "[MS_Table::readData] Unsupported type of column data!"
		      << std::endl;
	    status = false;
	  }
	  
	} else {
	  std::cerr << "[MS_Table::readData] No such column : " << column
		    << std::endl;
	  status = false;
	}
	
	return status;
      }
    
    /*!
      \brief Read data from a selected number of rows in a table column.
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column    -- Name of the \e column from which to read the \e data.
      \param selection -- Specification of elements to selected from the table
             column.
      \return status   -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (casa::Array<T> &data,
		     std::string const &column,
		     casa::Slicer const &selection)
      {
	bool status = true;
	
	try {
	  casa::TableDesc tableDesc   = itsTableSelection.tableDesc();
	  casa::ColumnDesc columnDesc = tableDesc.columnDesc(column);
	  casa::IPosition cellShape   = columnDesc.shape();
	  
	  {
	    unsigned int rank = cellShape.nelements()+1;
	    casa::IPosition dataShape (rank,0);
	    
	    for (unsigned int n=0; n<(rank-1); ++n) {
	      dataShape(n) = cellShape(n);
	    }
	    dataShape(rank-1) = selection.length()(0);
	    
	    data.resize(dataShape);
	  }
	  
	  if (columnDesc.isScalar()) {
	    // Set up reader object for the column ...
	    casa::ROScalarColumn<T> columReader (itsTableSelection, column);
	    // .... and retrieve the data
	    data = columReader.getColumnRange (selection);
	  } else if (columnDesc.isArray()) {
	    // Set up reader object for the column ...
	    casa::ROArrayColumn<T> columReader (itsTableSelection, column);
	    // .... and retrieve the data
	    columReader.getColumnRange (selection, data, true);
	  } else {
	    std::cerr << "[MS_Table::readData] Unsupported type of column data!"
		      << std::endl;
	    status = false;
	  }
	} catch (casa::AipsError x) {
	  std::cerr << "[MSTable::readData] " << x.getMesg() << std::endl;
	  status = false;
	}
	return status;
      }
    
    /*!
      \brief Read data from a selected number of rows in a table column.
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column  -- Name of the \e column from which to read the \e data.
      \param start   -- Index of the row from which to start reading.
      \param nofRows -- Number of rows from which to read data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (casa::Array<T> &data,
		     std::string const &column,
		     unsigned int const &start,
		     unsigned int const &nofRows=1)
      {
	bool status = true;
	
	try {
	  // Define slicer
	  casa::Slicer selection (casa::IPosition(1,start),
				  casa::IPosition(1,nofRows),
				  casa::IPosition(1,1));
	  // Read data from table column
	  status = readData (data, column, selection);
	} catch (casa::AipsError x) {
	  std::cerr << "[MSTable::readData] " << x.getMesg() << std::endl;
	  status = false;
	}
	
	return status;
      }
    
    /*!
      \brief Read data from a table column.
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column  -- Name of the \e column from which to read the \e data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (std::vector<T> &data,
		     std::string const &column)
      {
	bool status = true;
	casa::Array<T> buffer;
	
	try {
	  status = readData (buffer,column);
	} catch (casa::AipsError x) {
	  std::cerr << "[MSTable::readData] " << x.getMesg() << std::endl;
	  return false;
	}
	
	if (status) {
	  unsigned int nelem = buffer.nelements();
	  // resize the array returning the data
	  data.resize(nelem);
	  // get the data from the buffer array
	  data.assign(buffer.data(),buffer.data()+nelem);
	}
	
	return status;
      }
    
    /*!
      \brief Read data from a selected number of rows in a table column.
      \retval data   -- Array returning the data stored inside the designated
              table \e column.
      \param column  -- Name of the \e column from which to read the \e data.
      \param start   -- Index of the row from which to start reading.
      \param nofRows -- Number of rows from which to read data.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered.
    */
    template <class T>
      bool readData (std::vector<T> &data,
		     std::string const &column,
		     unsigned int const &start,
		     unsigned int const &nofRows=1)
      {
	// Check if table is ok
	if (itsTableSelection.isNull()) {
	  return false;
	}

	casa::Array<T> buffer;
	// read column data
	if (readData (buffer,column,start,nofRows)) {
	  unsigned int nelem = buffer.nelements();
	  // resize the array returning the data
	  data.resize(nelem);
	  // get the data from the buffer array
	  data.assign(buffer.data(),buffer.data()+nelem);
	} else {
	  data.clear();
	  return false;
	}
	
	return true;
      }
    
    // === Static methods =======================================================
    
    //! Resolve the \e name for the MS table
    static bool resolveName (std::string &absoluteName,
			     std::string const &name);

  protected:
    
    //! Unconditional copying
    void copy (MS_Table const &other);
    
  private:

    //! Initialize internal parameters
    void init ();
    //! Open embedded structures
    bool openEmbedded ();
    //! Set table selection
    bool setSelection (casa::TableExprNode const &exprNode,
		       bool const &overwrite=false);
    //! Unconditional deletion 
    void destroy(void);
    
#else 
    
  public:
    //! Default constructor
    MS_Table ();
    //! Test if a column with this \e name exists. 
    bool hasColumn (std::string const &name)
      {
	std::cerr << "[MS_Table::readData] Unable to check for column "
	<< name << " - missing casacore!" << std::endl;
	return false;
      }
    //! Read data from MS
    template <class T>
      bool readData (std::vector<T> &data,
		     std::string const &column)
      {
	std::cerr << "[MS_Table::readData] Unable to read data from column "
	<< column << " - missing casacore!" << std::endl;
	data.clear();
	return false;
      }
    //! Read data from MS
    template <class T>
      bool readData (std::vector<T> &data,
		     std::string const &column,
		     unsigned int const &start,
		     unsigned int const &nofRows=1)
      {
	std::cerr << "[MS_Table::readData] Unable to read data from column "
	<< column << " - missing casacore!" << std::endl;
	data.clear();
	return false;
      }
#endif
    
  }; // Class MS_Table -- end
  
} // Namespace DAL -- end

#endif /* MS_TABLE_H */

