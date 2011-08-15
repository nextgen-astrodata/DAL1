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

#include <data_hl/MS_Dataset.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   MS_Dataset
  
  MS_Dataset::MS_Dataset ()
    : MS_Table ()
  {;}
  
  //_____________________________________________________________________________
  //                                                                   MS_Dataset
  
  MS_Dataset::MS_Dataset (std::string const &name,
			  IO_Mode const &flags)
    : MS_Table (name, flags)
  {
  }
    
  //_____________________________________________________________________________
  //                                                                   MS_Dataset
  
  /*!
    \param other -- Another MS_Dataset object from which to create this new
           one.
  */
  MS_Dataset::MS_Dataset (MS_Dataset const &other)
    : MS_Table (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  MS_Dataset::~MS_Dataset ()
  {
    destroy();
  }
  
  void MS_Dataset::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another MS_Dataset object from which to make a copy.
  */
  MS_Dataset& MS_Dataset::operator= (MS_Dataset const &other)
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
    \param other -- Another MS_Dataset object from which to make a copy.
  */
  void MS_Dataset::copy (MS_Dataset const &other)
  {;}

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
  void MS_Dataset::summary (std::ostream &os)
  {
    casa::TableDesc tableDesc = itsTable.tableDesc();
    unsigned int nofRows      = itsTable.nrow();

    os << "[MS_Dataset] Summary of internal parameters."       << std::endl;
    os << "-- File type           = " << itsFiletype.name()    << std::endl;
    os << "-- I/O mode flags      = " << itsFlags.names()      << std::endl;
    os << "-- Table name          = " << itsName               << std::endl;
    os << "-- nof. sub-tables     = " << itsTableNames.size()  << std::endl;
    os << "-- Sub-table names     = " << itsTableNames         << std::endl;
    os << "-- nof. table columns  = " << itsColumnNames.size() << std::endl;
    os << "-- Column names        = " << itsColumnNames        << std::endl;
    os << "-- nof. table rows     = " << nofRows               << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                               exposureValues
  
  /*
    \return data -- Values contained within the 'EXPOSURE' column of the MS 'MAIN'
            table; returns \e false in case an error was encountered.
  */
  bool MS_Dataset::exposureValues (std::vector<double> &data)
  {
    return readData(data, "EXPOSURE");
  }

  //_____________________________________________________________________________
  //                                                                   timeValues
  
  /*
    \return data -- Values contained within the 'TIME' column of the MS 'MAIN'
            table; returns \e false in case an error was encountered.
  */
  bool MS_Dataset::timeValues (std::vector<double> &data)
  {
    return readData(data, "TIME");
  }

  //_____________________________________________________________________________
  //                                                                    uvwValues
  
  /*
    \return data -- Values contained within the 'UVW' column of the MS 'MAIN'
            table; returns \e false in case an error was encountered.
  */
  bool MS_Dataset::uvwValues (std::vector<double> &data)
  {
    return readData(data, "UVW");
  }

  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  

} // Namespace DAL -- end
