/***************************************************************************
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

#ifndef DALARRAY_H
#include "dalArray.h"
#endif

dalArray::dalArray( void * voidfile, string arrayname ) {
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	hid_t array, datatype, dataspace;
	hsize_t dims[2];
	dims[0] = 3;
	dims[1] = 4;
	dataspace = H5Screate_simple(2,dims,NULL);
	datatype = H5Tcopy(H5T_NATIVE_INT);
	array = H5Dcreate( file_id, "fooArray", datatype, dataspace, H5P_DEFAULT);
}

dalArray::~dalArray() {
}
