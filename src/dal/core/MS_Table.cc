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
  }

  //_____________________________________________________________________________
  //                                                                     MS_Table
  
  /*!
    \param 
    \param 
  */
  MS_Table::MS_Table (std::string const &name,
		      IO_Mode const &flags)
    : dalObjectBase (dalFileType::CASA_MS)
  {
    open (name,flags);
  }
  
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
  
  MS_Table::~MS_Table ()
  {
    destroy();
  }
  
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
    itsTable = casa::Table (other.itsTable);
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
  */
  void MS_Table::summary (std::ostream &os)
  {
    casa::TableDesc tableDesc = tableDescription();

    os << "[MS_Table] Summary of internal parameters."    << std::endl;
    os << "-- File type       = " << itsFiletype.name()   << std::endl;
    os << "-- I/O mode flags  = " << itsFlags.names()     << std::endl;
    os << "-- Object name     = " << itsName              << std::endl;
    os << "-- Table name      = " << itsTable.tableName() << std::endl;
    os << "-- nof. table rows = " << itsTable.nrow()      << std::endl;
    os << "-- Column names    = " << tableDesc.columnNames() << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  bool MS_Table::open (std::string const &name,
		       IO_Mode const &flags)
  {
    bool status = true;
    string absoluteName;

    // Resolve the name for the location of the MeasurementSet table
    if (!resolveName(absoluteName,name)) {
      return false;
    }
    
    casa::Table table (absoluteName);
    itsTable = casa::Table (table);
    
    return status;
  }
  
  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================

  // ============================================================================
  //
  //  Private methods
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
  
  
#else

  MS_Table::MS_Table ()
    : dalObjectBase ()
  {
  }

#endif  

} // Namespace DAL -- end
