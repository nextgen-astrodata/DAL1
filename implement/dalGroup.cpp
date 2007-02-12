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

#ifndef dalGROUP_H
#include "dalGroup.h"
#endif

dalGroup::dalGroup() {
    H5::Group group;
    hid_t foo = group.getLocId();
    cout << "group id == " << foo << endl;
}

dalGroup::dalGroup( void * voidfile, string gname ) {

	H5::H5File * lclfile = (H5::H5File*)voidfile; // H5File object
	file = lclfile;
	file_id = lclfile->getLocId();  // get the file handle

    name = gname;
    H5::Group * lclgroup = new H5::Group( lclfile->createGroup( "/" + gname ));
    group = lclgroup;  // set local to global
}

bool dalGroup::setName ( string gname ) {
   if ( gname.length() > 0 )
   {
     name = gname;
     return SUCCESS;
   }
   else
   {
     cout << "Error:  Group name must not be empty." << endl;
     return FAIL;
   }
}

string dalGroup::getName () {
   return name;
}

/*
void dalGroup::setAttribute( string attrname, void * data, string coltype )
{
	const int size = 1;
	if ( dal_INT == coltype ) {
			status = H5LTset_attribute_int( file_id, name.c_str(), attrname.c_str(), (const int*)data, size );
	}
	else if ( dal_FLOAT == coltype ) {
			status = H5LTset_attribute_float( file_id, name.c_str(), attrname.c_str(), (const float*)data, size );
	}
	else if ( dal_DOUBLE == coltype ) {
			status = H5LTset_attribute_double( file_id, name.c_str(), attrname.c_str(), (const double*)data, size );
	}
	else {
			cout << "ERROR: datatype " << coltype << " not supported for setAttribute." << endl;
	}			
}
*/
void dalGroup::setAttribute_string( string attrname, string data ) {
	status = H5LTset_attribute_string( file_id, name.c_str(), attrname.c_str(), data.c_str() );
}

void dalGroup::setAttribute_int( string attrname, int * data, int size ) {
	status = H5LTset_attribute_int( file_id, name.c_str(), attrname.c_str(), data, size );
}

void dalGroup::setAttribute_uint( string attrname, unsigned int * data, int size ) {
	status = H5LTset_attribute_uint( file_id, name.c_str(), attrname.c_str(), data, size );
}

void dalGroup::setAttribute_double( string attrname, double * data, int size ) {
	status = H5LTset_attribute_double( file_id, name.c_str(), attrname.c_str(), data, size );
}
