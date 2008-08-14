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

namespace DAL
  {

  // ---------------------------------------------------------- dalFilter

  /*!
    \brief Default constructor.

    Default constructor.
   */
  dalFilter::dalFilter()
  {
    filterstring = "";
    filetype= "";
    is_set = false;
  }

  // ---------------------------------------------------------- dalFilter

  /*!
    \brief Constructor.
    \param type The type of file (i.e. H5TYPE, MSCASATYPE).
    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").

    Restrict the opening of a table to particular columns.
   */
  dalFilter::dalFilter( std::string type, std::string cols )
  {
    if ( type == MSCASATYPE )
      {
        filetype == type;
        filterstring = "Select " + cols + " from $1";
      }
    else
      std::cerr << "Operation not yet supported for type: " +
                type + ". Sorry.\n";
  }


  // ---------------------------------------------------------- dalFilter

  /*!
    \brief Constructor.
    \param type The type of file (i.e. H5TYPE, MSCASATYPE).
    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
                      (i.e. "ANTENNA1=1 AND ANTENNA2=10")

    Restrict the opening of a table to particular columns and conditions.
   */
  dalFilter::dalFilter( std::string type,
                        std::string cols,
                        std::string conditions )
  {
    if ( type == MSCASATYPE )
      {
        filetype = type;
        filterstring = "Select " + cols + " from $1 where " + conditions;
      }
    else
      std::cerr << "Operation not yet supported for type: " +
                type + ". Sorry.\n";
  }

  // ---------------------------------------------------------- set

  /*!
    \brief Restrict to certain columns.

    Restrict the opening of a table to particular columns.

    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").
   */
  void dalFilter::set( std::string cols )
  {
    if ( filetype == MSCASATYPE )
      {
        filterstring = "Select " + cols + " from $1";
        is_set = true;
      }
    else
      std::cerr << "Operation not yet supported for type: " + filetype + ". Sorry.\n";
  }


  // ---------------------------------------------------------- set

  /*!
    \brief Restrict to certain columns and conditions.

    Restrict the opening of a table to particular columns and conditions.

    \param columns A comma-separated list of the column names that you
                   want to pass the filter (i.e. "TIME,DATA,ANTENNA").
    \param conditions A list of the conditions you want to apply.
                      (i.e. "ANTENNA1=1 AND ANTENNA2=10")
   */
  void dalFilter::set( std::string cols, std::string conditions )
  {
    if ( filetype == MSCASATYPE )
      {
        filterstring = "Select " + cols + " from $1 where " + conditions;
        is_set = true;
      }
    else
      std::cerr << "Operation not yet supported for type: " + filetype + ". Sorry.\n";
  }

  // ---------------------------------------------------------- setFiletype

  /*!
    \brief Declare the type of the file.

    Declare the type of the file (i.e. "MSCAS", "HDF5", etc.)

    \param type The type of the file (i.e. "MSCAS", "HDF5", etc.)
   */
  void dalFilter::setFiletype( std::string type )
  {
    filetype = type;
  }


  // ---------------------------------------------------------- isSet

  /*!
    \brief Check to see if the filter is defined.

    Check to see if the filter is defined.

    \return True if the filter is set.  False if filter is not set.
   */
  bool dalFilter::isSet()
  {
    if (is_set)
      return true;
    else
      return false;
  }


  // ---------------------------------------------------------- get

  /*!
    \brief Retrieve the filter string.

    Retrieve the filter string.

    \return A string representing the filter.
   */
  std::string dalFilter::get()
  {
    return filterstring;
  }


} // DAL namespace
