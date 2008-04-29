/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
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
#define DALFILTER_H

#ifndef DAL_H
#include "dal.h"
#endif

/*!
  \class dalFilter
  
  \brief Superclass of dalDSFilter, dalColumnFilter, dalTableFilter)
  
  \ingroup DAL
  
  \author Joseph Masters
*/
class dalFilter {
	string filterstring;  // Table filter string
	string filetype;  // MSCASA, HDF5, FITS, etc.
	bool is_set;
  public:

        /*!
          \brief Default constructor.

          Default constructor.
         */
	dalFilter();

        /*!
          \brief Default destructor.

          Default destructor.
         */
	~dalFilter();

        /*!
          \brief Restrict to certain columns.

          Restrict the opening of a table to particular columns.

          \param columns A comma-separated list of the column names that you
                         want to pass the filter (i.e. "TIME,DATA,ANTENNA").
         */
	void set( string columns );

        /*!
          \brief Restrict to certain columns and conditions.

          Restrict the opening of a table to particular columns and conditions.

          \param columns A comma-separated list of the column names that you
                         want to pass the filter (i.e. "TIME,DATA,ANTENNA").
          \param conditions A list of the conditions you want to apply.
                            (i.e. "ANTENNA1=1 AND ANTENNA2=10")
         */
	void set( string columns, string conditions );

        /*!
          \brief Declare the type of the file.

          Declare the type of the file (i.e. "MSCAS", "HDF5", etc.)

          \param type The type of the file (i.e. "MSCAS", "HDF5", etc.)
         */
	void setFiletype( string type );

        /*!
          \brief Check to see if the filter is defined.

          Check to see if the filter is defined.

          \return True if the filter is set.  False if filter is not set.
         */
	bool isSet();

        /*!
          \brief Retrieve the filter string.

          Retrieve the filter string.

          \return A string representing the filter.
         */
	string get();
};
#endif
