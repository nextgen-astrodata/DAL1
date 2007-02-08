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

#ifndef DALCOLUMN_H
#define DALCOLUMN_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

#ifndef DALFILTER_H
#include "dalFilter.h"
#endif

/*!
  \class calColumn

  \ingroup DAL

  \brief t.b.w.

  \author Joseph Masters
 */
class dalColumn {

	string name; /// column name
	string datatype;  /// column datatype
	int size; /// datatype size
//	hsize_t size;  /// hdf5 size
	
	vector<dalAttribute> attributes; /// list of column attributes
	dalFilter filter; /// filter associated with column
	
	  public:
	
	/*!
	  \brief Default constructor 
	*/
	dalColumn();
	/*!
	  \brief Argumented constructor

	  \param colname -- 
	  \param coltype -- 
	*/
	dalColumn( string colname, string coltype );
	/*!
	  \brief Argumented constructor
	  
	  \brief complexcolname -- 
	*/
	dalColumn( string complexcolname/*, void * dataformat*/ );
	/*!
	  \brief Add a member to a column

	  \param member_name -- 
	  \param type        -- 
	*/
	void addMember( string member_name, string type );
	/*!
	  \brief Get the name of the column

	  \return name -- The name of the column
	*/
	string getName();
	/*!
	  \brief Get the type in the column

	  \return type -- The type in the column
	*/
	string getType();
	/*!
	  \brief Get the size of the column

	  \return size -- The size of the column
	*/
	int getSize();

	/*!
	  \brief Get the index of a column

	  \return number -- The index of a column
	*/
	int getNumber();
	/*!
	  \brief Add an array to the column
	*/
	void addArray();
	/*!
	  \brief Insert a new column after column specified by name or number
	  
	  \return status -- Status/error code of the operation
	*/
	int insertAfter();
	/*!
	  \brief Insert a new column before column specified by name or number
	  
	  \return status -- Status/error code of the operation
	*/
	int insertBefore();
	/*!
	  \brief Replace a column with a new one
	*/
	int replace();
	/*!
	  \brief Remove a column
	*/
	int remove();
};
#endif
