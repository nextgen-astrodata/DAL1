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

#ifndef dalArray_H
#include "dalArray.h"
#endif

dalIntArray::dalIntArray( void * voidfile, string arrayname, vector<int> dims, int data[] ) {
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	hid_t array, datatype, dataspace;
//	hsize_t dims[rank];
/*	dims[0] = 3;
	dims[1] = 4;*/
	hsize_t mydims[dims.size()];
	for (unsigned int ii=0; ii<dims.size(); ii++)
		mydims[ii] = dims[ii];

//	float mydata[3];

// 	for (int ii=0; ii<dims[0]; ii++)
// 	  for (int jj=0; jj<dims[1]; jj++)
// 	    for (int kk=0; kk<dims[2]; kk++)
// 		mydata[ii][jj][kk] = ii + jj + kk;

	dataspace = H5Screate_simple(dims.size(),mydims,NULL);
	datatype = H5Tcopy(H5T_NATIVE_INT);
	array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	H5Dwrite(array, datatype, dataspace, dataspace, H5P_DEFAULT, data);
}

dalIntArray::~dalIntArray() {
}

dalFloatArray::dalFloatArray( void * voidfile, string arrayname, vector<int> dims, float data[] ) {
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	hid_t array, datatype, dataspace;
	hsize_t mydims[dims.size()];
	for (unsigned int ii=0; ii<dims.size(); ii++)
		mydims[ii] = dims[ii];

	dataspace = H5Screate_simple(dims.size(),mydims,NULL);
	datatype = H5Tcopy(H5T_NATIVE_FLOAT);
	array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	H5Dwrite(array, datatype, dataspace, dataspace, H5P_DEFAULT, data);
}

dalFloatArray::~dalFloatArray() {
}
