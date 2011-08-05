/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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

#include <core/dalFilter.h>

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  dalFilter::dalFilter()
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  /*!
    \param type -- The type of file (i.e. HDF5, CASA_MS).
    \param columns A comma-separated list of the column names that you want to
           pass the filter (i.e. "TIME,DATA,ANTENNA").
   */
  dalFilter::dalFilter (dalFileType const &type,
			std::string columns)
  {
    init (type);
    setFilter (columns);
  }

  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  /*!
    \param type -- The type of file (i.e. HDF5, CASA_MS).
    \param columns A comma-separated list of the column names that you want to
           pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
           (i.e. "ANTENNA1=1 AND ANTENNA2=10")

    Restrict the opening of a table to particular columns and conditions.
   */
  dalFilter::dalFilter (dalFileType const &type,
                        std::string columns,
                        std::string conditions )
  {
    init (type);
    setFilter (columns,conditions);
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  void dalFilter::init (dalFileType const &type)
  {
    itsFilterString = "";
    itsFiletype     = type;
    itsFilterIsSet  = false;
  }

  //_____________________________________________________________________________
  //                                                                    setFilter
  
  /*!
    \param columns A comma-separated list of the column names that you
           want to pass the filter (i.e. "TIME,DATA,ANTENNA").
    \return status -- Status of the operation; returns \e false in case an error
            was encountered, e.g. because the operation is not supported for the
	    file type.
  */
  bool dalFilter::setFilter (std::string const &columns)
  {
    bool status = true;
    
    switch (itsFiletype.type()) {
    case dalFileType::CASA_MS:
      itsFilterString = "Select " + columns + " from $1";
      itsFilterIsSet   = true;
      break;
    default:
      {
	std::cerr << "[dalFilter::setFilter] Operation not yet supoorted for type "
		  << itsFiletype.name()
		  << std::endl;
	status = false;
      }
      break;
    };
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                    setFilter
  
  /*!
    \param columns -- Names of the columns to select.
  */
  bool dalFilter::setFilter (std::vector<std::string> const &columns)
  {
    if (columns.empty()) {
      std::cerr << "[dalFilter::set] Empty list of column names!" << std::endl;
      return false;
    } else {
      std::string names (columns[0]);
      /* Write column names into comma-separated list */
      for (unsigned int n(1); n<columns.size(); ++n) {
	names += ",";
	names += columns[n];
      }
      /* Set the column selection */
      return setFilter (names);
    }
  }
  
  //_____________________________________________________________________________
  //                                                                    setFilter
  
  /*!
    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
                      (i.e. "ANTENNA1=1 AND ANTENNA2=10")
   */
  bool dalFilter::setFilter (std::string const &cols,
			     std::string const &conditions)
  {
    bool status = true;
    
    switch (itsFiletype.type()) {
    case dalFileType::CASA_MS:
      {
        itsFilterString = "Select " + cols + " from $1 where " + conditions;
        itsFilterIsSet  = true;
      }
      break;
    default:
      {
	std::cerr << "[dalFilter::setFilter] Operation not yet supoorted for type "
		  << itsFiletype.name()
		  << std::endl;
	status = false;
      }
      break;
    };
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  void dalFilter::summary (std::ostream &os)
  {
    os << "[dalFilter] Summary of internal parameters."   << std::endl;
    os << "-- Filter string = " << itsFilterString         << std::endl;
    os << "-- File type     = " << itsFiletype.name()      << std::endl;
    os << "-- Filter is set = " << itsFilterIsSet          << std::endl;
  }
  
} // DAL namespace
