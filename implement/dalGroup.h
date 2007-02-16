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
#ifndef DALGROUP_H
#define DALGROUP_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALTABLE_H
#include "dalTable.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif


#ifndef DALFILTER_H
#include "dalFilter.h"
#endif

class dalGroup{

	void * file; /// can be HDF5File, FITS, MS
	string name; /// group name
	H5::Group * group;
	vector<dalTable> tables; /// list of tables within groups
	vector<dalAttribute> attributes; /// list of group attributes
	dalFilter filter; /// filter associated with group
	hid_t file_id; /// hdf5 file_id
	hid_t group_id; /// hdf5 group_id
	herr_t	status; /// hdf5 return status
	
 public:
 	
	dalGroup();  /// constructor
	dalGroup( void * file, string groupname );  /// constructor with group name
	void open( void * file, string groupname ); /// open an existing group
	string getName(); /// get group name
	bool setName( string ); /// set group name
	void listTables();  /// print a list of table names within the group
	int getTable();  /// retrieve a dalTable by name
	int getTableNumber(); /// retrieve a dalTable by index
	int advanceTable(); /// sequentially step through the tables in a group
	int rename();  /// rename a group
	int remove(); /// delete a group
	void listAttributes(); /// return a list of group attributes
	int getAttribute(); /// return a dalAttribute object retrieved by name

	void getAttribute( string attrname );
	void getAttribute_string( string attrname );
    void setAttribute( string attrname, void * data, string coltype );
    void setAttribute_string( string attrname, string data );
    void setAttribute_int( string attrname, int * data, int size=1 );
    void setAttribute_uint( string attrname, unsigned int * data, int size=1 );
    void setAttribute_double( string attrname, double * data, int size=1 );
};

#endif
