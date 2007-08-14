/*-------------------------------------------------------------------------*
| $Id:: cs1.h 389 2007-06-13 08:47:09Z baehren                          $ |
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

#ifndef dalArray_H
#include "dalArray.h"
#endif

dalIntArray::dalIntArray(){}

dalIntArray::dalIntArray( void * voidfile, string arrayname, vector<int> dims,
			  int data[], vector<int> chnkdims ) {
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	hid_t array, datatype, dataspace;
	unsigned int rank = dims.size();
	hsize_t mydims[rank];
	hsize_t maxdims[rank];
	hid_t status_lcl;

	hsize_t chunk_dims[ rank ];
	for (unsigned int ii=0; ii<rank; ii++)
	{
		mydims[ii] = dims[ii];
		maxdims[ii] = H5S_UNLIMITED;
	}

	for (unsigned int ii=0; ii<chnkdims.size(); ii++)
	{
		chunk_dims[ii] = chnkdims[ii];
	}

	datatype = H5Tcopy(H5T_NATIVE_INT);
	if ( chnkdims.size()>0 )
	{
	   dataspace = H5Screate_simple(rank,mydims,maxdims);
	   hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
	   status_lcl = H5Pset_chunk( cparms, rank, chunk_dims );
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, cparms);
	}
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	}

	H5Dwrite(array, datatype, dataspace, dataspace, H5P_DEFAULT, data);
	H5Sclose( dataspace );
	H5Tclose( datatype );
	H5Dclose( array );
}

dalIntArray::~dalIntArray() {
}

dalFloatArray::dalFloatArray( void * voidfile, string arrayname,
		 vector<int> dims, float data[], vector<int> chnkdims ) {
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	hid_t array, datatype, dataspace;
	int rank = dims.size();
	hsize_t mydims[rank];
	hsize_t maxdims[rank];
	hsize_t chunk_dims[ rank ];
	hid_t status_lcl;

	for (int ii=0; ii<rank; ii++)
	{
		mydims[ii] = dims[ii];
		maxdims[ii] = H5S_UNLIMITED;
	}

	for (unsigned int ii=0; ii<chnkdims.size(); ii++)
	{
		chunk_dims[ii] = chnkdims[ii];
	}

	datatype = H5Tcopy(H5T_NATIVE_FLOAT);
	if ( chnkdims.size()>0 )
	{
	   dataspace = H5Screate_simple(rank,mydims,maxdims);
	   hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
	   status_lcl = H5Pset_chunk( cparms, rank, chunk_dims );
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, cparms);
	}
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	}

	H5Dwrite(array, datatype, dataspace, dataspace, H5P_DEFAULT, data);
	H5Sclose( dataspace );
	H5Tclose( datatype );
	H5Dclose( array );
}

dalFloatArray::~dalFloatArray() {
}

int * dalIntArray::readIntArray( void * voidfile, string arrayname )
{
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
// 	hid_t array, datatype, dataspace;
	herr_t      status;

	// get the dataspace
	hid_t filespace = H5Dget_space(file_id);

	// what is the rank of the array?
	hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
	hsize_t dims[ data_rank ];
	status = H5Sget_simple_extent_dims(filespace, dims, NULL);

	int size = 1;
	for (int ii=0; ii<data_rank; ii++)
	  size *= dims[0];	
	int * data = NULL;
	data = new int[size];

	status = H5LTread_dataset_int( file_id, arrayname.c_str(), data );
	hid_t memspace = H5Screate_simple (data_rank,dims,NULL);
	status = H5Dread (file_id, H5T_NATIVE_INT, memspace, filespace,
		H5P_DEFAULT, data);

	return data;
}

dalComplexArray::dalComplexArray( void * voidfile, string arrayname, vector<int> dims,
			  dalcomplex data[], vector<int> chnkdims ) {
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	hid_t array, datatype, dataspace;
	unsigned int rank = dims.size();
	hsize_t mydims[rank];
	hsize_t maxdims[rank];
	hid_t status_lcl;

	hsize_t chunk_dims[ rank ];
	for (unsigned int ii=0; ii<rank; ii++)
	{
		mydims[ii] = dims[ii];
		maxdims[ii] = H5S_UNLIMITED;
	}

	for (unsigned int ii=0; ii<chnkdims.size(); ii++)
	{
		chunk_dims[ii] = chnkdims[ii];
	}

hid_t complex_id = H5Tcreate (H5T_COMPOUND, sizeof(dalcomplex));
H5Tinsert (complex_id, "real", HOFFSET(dalcomplex,r), H5T_NATIVE_DOUBLE);
H5Tinsert (complex_id, "imaginary", HOFFSET(dalcomplex,i), H5T_NATIVE_DOUBLE);

	datatype = H5Tcopy( complex_id );
	if ( chnkdims.size()>0 )
	{
	   dataspace = H5Screate_simple(rank,mydims,maxdims);
	   hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
	   status_lcl = H5Pset_chunk( cparms, rank, chunk_dims );
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, cparms);
	}
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	}

	H5Dwrite(array, datatype, dataspace, dataspace, H5P_DEFAULT, data);
	H5Sclose( dataspace );
	H5Tclose( datatype );
	H5Dclose( array );
}
