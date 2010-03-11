/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#ifndef DALFILTER_H
#include "dalFilter.h"
#endif

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
    \param type -- The type of file (i.e. HDF5, MSCASA).
    \param columns A comma-separated list of the column names that you want to
           pass the filter (i.e. "TIME,DATA,ANTENNA").
   */
  dalFilter::dalFilter (DAL::dalFileType const &type,
			std::string columns)
  {
    init ();             /* Initialize internal parameters */
    setFiletype(type);   /* Set file type                  */
    
    set (columns);
  }

  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  /*!
    \param type The type of file (i.e. H5TYPE, MSCASATYPE).
    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").

    Restrict the opening of a table to particular columns.
   */
  dalFilter::dalFilter (std::string type,
			std::string columns)
  {
    init ();             /* Initialize internal parameters */
    setFiletype(type);   /* Set file type                  */
    
    set (columns);
  }
  
  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  /*!
    \param type -- The type of file (i.e. HDF5, MSCASA).
    \param columns A comma-separated list of the column names that you want to
           pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
           (i.e. "ANTENNA1=1 AND ANTENNA2=10")

    Restrict the opening of a table to particular columns and conditions.
   */
  dalFilter::dalFilter (DAL::dalFileType const &type,
                        std::string cols,
                        std::string conditions )
  {
    init ();             /* Initialize internal parameters */
    setFiletype(type);   /* Set file type                  */

    set (cols,conditions);
  }

  //_____________________________________________________________________________
  //                                                                    dalFilter
  
  /*!
    \param type The type of file (i.e. H5TYPE, MSCASATYPE).
    \param columns A comma-separated list of the column names that you want to
           pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
           (i.e. "ANTENNA1=1 AND ANTENNA2=10")

    Restrict the opening of a table to particular columns and conditions.
   */
  dalFilter::dalFilter( std::string type,
                        std::string cols,
                        std::string conditions )
  {
    init ();             /* Initialize internal parameters */
    setFiletype(type);   /* Set file type                  */

    set (cols,conditions);
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  void dalFilter::init ()
  {
    filterstring_p = "";
    filetype_p     = DAL::UNDEFINED;
    is_set         = false;
  }

  //_____________________________________________________________________________
  //                                                                          set
  
  /*!
    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").
   */
  bool dalFilter::set ( std::string cols )
  {
    bool status (true);
    
    switch (filetype_p) {
    case DAL::MSCASA:
      filterstring_p = "Select " + cols + " from $1";
      is_set         = true;
      break;
    default:
      std::cerr << "Operation not yet supported for type: "
		<< fileType(filetype_p) << ". Sorry.\n";
      status = false;
      break;
    };
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                          set
  
  /*!
    \param columns -- Names of the columns to select.
  */
  bool dalFilter::set (std::vector<std::string> const &columns)
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
      return set (names);
    }
  }
  
  //_____________________________________________________________________________
  //                                                                          set
  
  /*!
    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
                      (i.e. "ANTENNA1=1 AND ANTENNA2=10")
   */
  void dalFilter::set (std::string cols,
		       std::string conditions)
  {
    switch (filetype_p) {
    case DAL::MSCASA:
      break;
        filterstring_p = "Select " + cols + " from $1 where " + conditions;
        is_set         = true;
    default:
      std::cerr << "Operation not yet supported for type: " 
		<< fileType (filetype_p)
		<< ". Sorry.\n";
      break;
    };
  }

  //_____________________________________________________________________________
  //                                                                  setFiletype
  
  /*!
    \param type The type of the file (i.e. "MSCAS", "HDF5", etc.)
   */
  bool dalFilter::setFiletype (std::string const &type)
  {
    DAL::dalFileType t = fileType (type);
    return setFiletype (t);
  }

  //_____________________________________________________________________________
  //                                                                  setFiletype
  
  bool dalFilter::setFiletype (DAL::dalFileType const &type)
  {
    bool status (true);

    switch (type) {
    case DAL::HDF5:
    case DAL::FITS:
    case DAL::MSCASA:
      filetype_p = type;
      break;
    default:
      filetype_p = DAL::UNDEFINED;
      status     = false;
      break;
    };

    return true;
  }

  //_____________________________________________________________________________
  //                                                                        isSet
  
  /*!
    \return True if the filter is set.  False if filter is not set.
   */
  bool dalFilter::isSet ()
  {
    if (is_set) {
      return true;
    }
    else {
      return false;
    }
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  void dalFilter::summary (std::ostream &os)
  {
    os << "[dalFilter] Summary of internal parameters."   << std::endl;
    os << "-- Filter string = " << filterstring_p         << std::endl;
    os << "-- File type     = " << fileType(filetype_p) << std::endl;
    os << "-- Filter is set = " << is_set                 << std::endl;
  }
  
} // DAL namespace
