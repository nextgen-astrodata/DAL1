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

#include <core/MS_Table.h>

namespace DAL { // Namespace DAL -- begin
  
#ifdef DAL_WITH_CASA

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                     MS_Table
  
  MS_Table::MS_Table ()
    : dalObjectBase (dalFileType::CASA_MS)
  {
    init ();
  }

  //_____________________________________________________________________________
  //                                                                     MS_Table
  
  /*!
    \param name  -- Name of the MeasurementSet.
    \param flags -- I/O mode flags.
  */
  MS_Table::MS_Table (std::string const &name,
		      IO_Mode const &flags)
    : dalObjectBase (dalFileType::CASA_MS)
  {
    init();
    open (name,flags);
  }

  //_____________________________________________________________________________
  //                                                                     MS_Table
  
  /*!
    \param table    -- Host table within which the other is contained.
    \param subtable -- Name of the sub-table to be opened.
    \param flags    -- I/O mode flags.
  */
  MS_Table::MS_Table (std::string const &table,
		      std::string const &subtable,
		      IO_Mode const &flags)
    : dalObjectBase (dalFileType::CASA_MS)
  {
    init();

    try {
      // open the root table
      casa::Table tab (table);
      if (tab.isNull()) {
	std::cerr << "[MS_Table::MS_Table] Failed to open table " << table << endl;
      } else {
	open (tab, subtable, flags);
      }
    } catch (casa::AipsError x) {
      std::cerr << "[MS_Table::MS_Table] " << x.getMesg() << endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                     MS_Table
  
  /*!
    \param table    -- Host table within which the other is contained.
    \param subtable -- Name of the sub-table to be opened.
    \param flags    -- I/O mode flags.
  */
  MS_Table::MS_Table (casa::Table const &table,
		      std::string const &subtable,
		      IO_Mode const &flags)
    : dalObjectBase (dalFileType::CASA_MS)
  {
    init();
    open (table, subtable, flags);
  }
  
  //_____________________________________________________________________________
  //                                                                     MS_Table
  
  /*!
    \param other -- Another MS_Table object from which to create this new
           one.
  */
  MS_Table::MS_Table (MS_Table const &other)
    : dalObjectBase (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    ~MS_Table
  
  MS_Table::~MS_Table ()
  {
    try {
      destroy();
    } catch (casa::AipsError x) {
      std::cerr << "[MS_Table::~MS_Table]" << x.getMesg() << std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                      destroy
  
  void MS_Table::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another MS_Table object from which to make a copy.
  */
  MS_Table& MS_Table::operator= (MS_Table const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  /*!
    \param other -- Another MS_Table object from which to make a copy.
  */
  void MS_Table::copy (MS_Table const &other)
  {
    if (other.itsTable.isNull()) {
      init();
    } else {
      itsTable          = casa::Table (other.itsTable);
      itsColumnNames    = other.itsColumnNames;
      itsTableNames     = other.itsTableNames;
      itsExpressionNode = other.itsExpressionNode;
      itsTableSelection = itsTable (itsExpressionNode);
    }
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
    \param showColumns -- Also show summary of the table columns?
  */
  void MS_Table::summary (std::ostream &os,
			  bool const &showColumns)
  {
    casa::TableDesc tableDesc          = tableDescription();
    unsigned int nofRows               = itsTableSelection.nrow();
    casa::Vector<casa::String> columns = tableDesc.columnNames();

    os << "[MS_Table] Summary of internal parameters."         << std::endl;
    os << "-- File type           = " << itsFiletype.name()    << std::endl;
    os << "-- I/O mode flags      = " << itsFlags.names()      << std::endl;
    os << "-- Selection is active = " << hasSelection()        << std::endl;
    os << "-- Table name          = " << itsName               << std::endl;
    os << "-- nof. sub-tables     = " << itsTableNames.size()  << std::endl;
    os << "-- Sub-table names     = " << itsTableNames         << std::endl;
    os << "-- nof. table rows     = " << nofRows               << std::endl;
    os << "-- nof. table columns  = " << itsColumnNames.size() << std::endl;
    
    if (!itsTableSelection.isNull()) {
      if ((columns.nelements()>0) && showColumns) {
	os << "-- Column descriptions :" << std::endl;
	std::cout << std::setw(5)  << "#"
		  << std::setw(25) << "name"
		  << std::setw(10) << "ndim"
		  << std::setw(10) << "dtype"
		  << std::setw(10) << "scalar"
		  << std::setw(10) << "array"
		  << std::setw(10) << "table"
		  << std::setw(10) << "shape"
		  << std::endl;
	for (unsigned int n=0; n<columns.nelements(); ++n) {
	  // Get column description
	  casa::ColumnDesc columnDesc = tableDesc.columnDesc(columns(n));
	  // Display column properties
	  std::cout << std::setw(5)  << n 
		    << std::setw(25) << columnDesc.name()
		    << std::setw(10) << columnDesc.ndim()
		    << std::setw(10) << columnDesc.dataType()
		    << std::setw(10) << columnDesc.isScalar()
		    << std::setw(10) << columnDesc.isArray()
		    << std::setw(10) << columnDesc.isTable()
		    << std::setw(10) << columnDesc.shape()
		    << std::endl;
	}
      }
    }
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param name  -- Name of the MeasurementSet.
    \param flags -- I/O mode flags.
  */
  bool MS_Table::open (std::string const &name,
		       IO_Mode const &flags)
  {
    bool status = true;
    string absoluteName;

    // Resolve the name for the location of the MeasurementSet table
    if (!resolveName(absoluteName,name)) {
      return false;
    }
    
    try {
      casa::Table table (absoluteName);
      // Check if the table is ok; if yes, then set internal variables
      if (table.isNull()) {
	status = false;
      } else {
	// Set internal variables
	itsTable = casa::Table (table);
	itsName  = itsTable.tableName();
	itsFlags = flags;
	// Scan for embedded objects
	status = openEmbedded();
      }
    } catch (casa::AipsError x) {
      std::cerr << "[MS_Table::open] " << x.getMesg() << std::endl;
      status = false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param table    -- Name of the host table.
    \param subtable -- Name of the sub-table to be opened.
    \param flags    -- I/O mode flags.
  */
  bool MS_Table::open (casa::Table const &table,
		       std::string const &subtable,
		       IO_Mode const &flags)
  {
    bool status = true;

    if (table.isNull()) {
      std::cerr << "[MS_Table::open] Master table is Null object!" << std::endl;
      return false;
    } else {
      try {
	// Set internal variables
	itsTable = casa::Table (table.keywordSet().asTable(subtable));
	itsName  = itsTable.tableName();
	itsFlags = flags;
	// Scan for embedded objects
	status = openEmbedded();
      } catch (casa::AipsError x) {
	std::cerr << "[MS_Table::open] " << x.getMesg() << std::endl;
	status = false;
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                    hasColumn
  
  /*!
    \param name       -- Name of the table column.
    \return hasColumn -- Returns \e true if the table contains a column of the 
            given \e name.
   */
  bool MS_Table::hasColumn (std::string const &name)
  {
    std::set<std::string>::iterator it = itsColumnNames.find(name);

    if (it==itsColumnNames.end()) {
      return false;
    } else {
      return true;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                     hasTable
  
  /*!
    \param name       -- Name of the sub-table.
    \return hasColumn -- Returns \e true if the table contains a column of the 
            given \e name.
   */
  bool MS_Table::hasTable (std::string const &name)
  {
    std::set<std::string>::iterator it = itsTableNames.find(name);

    if (it==itsTableNames.end()) {
      return false;
    } else {
      return true;
    }
  }
  
  //_____________________________________________________________________________
  //                                                              columnDataTypes
  
  std::map<casa::String,casa::DataType> MS_Table::columnDataTypes ()
  {
    std::map<casa::String,casa::DataType> dataTypes;
    
    if (!itsTableSelection.isNull()) {
      casa::TableDesc tableDesc = itsTableSelection.tableDesc ();
      casa::Vector<casa::String> columnNames = tableDesc.columnNames();
      std::string name;
      
      for (unsigned int n=0; n<columnNames.nelements(); ++n) {
	casa::ColumnDesc columnDesc = tableDesc.columnDesc(columnNames(n));
	//
	dataTypes[columnNames(n)] = columnDesc.dataType();
      }
    }

    return dataTypes;
  }

  //_____________________________________________________________________________
  //                                                                 hasSelection

  /*!
    \return hasSelection -- Returns \e true in case there is an active selection
            applied to the table.
  */
  bool MS_Table::hasSelection ()
  {
    if (itsExpressionNode.isNull()) {
      return false;
    } else {
      return true;
    }
  }
  
  //_____________________________________________________________________________
  //                                                               clearSelection
  
  /*!
    \return status   -- Status of the operation; returns \e false in case an
            error was encountered, such as e.g. the file does not exist.
  */
  bool MS_Table::clearSelection ()
  {
    bool status = true;
    
    try {
      itsExpressionNode = casa::TableExprNode();
      itsTableSelection = itsTable(itsExpressionNode);
    } catch (casa::AipsError x) {
      std::cerr << "[MS_Table::clearSelection] Error clearing previous selection!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }

  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================

  /*!
    \retval absoluteName -- Absolute name to location of the MeasurementSet;
            returns empty string in case the file does not exist.
    \param name          -- Name of the MeasurementSet to open.
    \return status       -- Status of the operation; returns \e false in case an
            error was encountered, such as e.g. the file does not exist.
  */
  bool MS_Table::resolveName (std::string &absoluteName,
			      std::string const &name)
    {
      casa::File msfile (name);
      
      /*____________________________________________________
	Check if the file exits
      */
      if (!msfile.exists()) {
	std::cerr << "[MS_Table::resolveName]" 
		  << " Unable to resolve name for file " << name
		  << " - does not exist!"
		  << std::endl;
	absoluteName = "";
	return false;
      }

      /*____________________________________________________
	Resolve file name
      */
      if (msfile.isSymLink()) {
	casa::SymLink link (msfile);
	casa::Path realFileName = link.followSymLink();
	absoluteName = realFileName.absoluteName();
      } else {
	absoluteName = name;
      }
      
      return true;
    }
  
  // ============================================================================
  //
  //  Private methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init

  void MS_Table::init ()
  {
    itsColumnNames.clear();
    itsTableNames.clear();
    itsTable          = casa::Table();
    itsExpressionNode = casa::TableExprNode();
    itsTableSelection = casa::Table();
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  /*!
    \return status -- Status of the operation; returns \e false in case an error
            was encountered while opening embedded objects (columns and
	    sub-tables).
  */
  bool MS_Table::openEmbedded ()
  {
    // Initialize interal variables
    itsColumnNames.clear();
    itsTableNames.clear();
    
    try {
      // Set table selection
      itsExpressionNode = casa::TableExprNode();
      itsTableSelection = itsTable(itsExpressionNode);

      // Get table description
      casa::TableDesc desc               = itsTableSelection.tableDesc ();
      casa::TableRecord rec              = itsTableSelection.keywordSet();
      casa::Vector<casa::String> columns = desc.columnNames();
      casa::RecordDesc recDesc           = rec.description();
      
      // Store column names
      for (unsigned int n=0; n<columns.nelements(); ++n) {
	itsColumnNames.insert(columns[n]);
      }

      // Store sub-table names
      for (unsigned int n=0; n<recDesc.nfields(); ++n) {
	if (recDesc.isTable (n)) {
	  itsTableNames.insert(recDesc.name(n));
	}
      }

    } catch (casa::AipsError x) {
      std::cerr << "[MS_Table::openEmbedded] " << x.getMesg() << std::endl;
      return false;
    }

    return true;
  }

  //_____________________________________________________________________________
  //                                                                 setSelection
  
  /*!
    \brief Set expression node for selection of table contents
    \param selection -- Table expression node for table selection.
    \param overwrite -- Overwrite existing selection? If set \e true, any
           previously existing table selection will be cleared; by dedault the
	   new selection is being appended to the already existing one.
  */
  bool MS_Table::setSelection (casa::TableExprNode const &exprNode,
			       bool const &overwrite)
  {    
    // Update table expression node defining selection
    if (overwrite || itsExpressionNode.isNull()) {
      itsExpressionNode = exprNode;
    } else {
      casa::TableExprNode tmp (itsExpressionNode && exprNode);
      itsExpressionNode = tmp;
    }
    
    // Apply selection to table
    try {
      itsTableSelection = itsTable(itsExpressionNode);
    } catch (casa::AipsError x) {
      std::cerr << "[MS_Table::setSelection] " << x.getMesg() << std::endl;
      return false;
    }
    
    return true;
  }
  
#else
  
  MS_Table::MS_Table ()
    : dalObjectBase ()
  {
  }

#endif  

} // Namespace DAL -- end
