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

/********************************************************************
 *  dalIntArray constructor creates an n-dimensional
 *    array of Integers.
 *
 *  arguments:
 *    voidfile (I) - dataset file handle
 *    arrayname (I) - name of the array to create
 *    dims (I) - vector of the array dimensions
 *    data (I) - array of data to write
 *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
 *      if the size of the array is fixed.
 *
 ********************************************************************/
dalIntArray::dalIntArray( void * voidfile, string arrayname, vector<int> dims,
			  int data[], vector<int> chnkdims ) {
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	hid_t array, datatype, dataspace;  // declare a few h5 variables

	// determine the rank from the size of the dimensions vector
	unsigned int rank = dims.size();

	hsize_t mydims[rank];  // declare a dimensions c-array
	hsize_t maxdims[rank]; // declare a maximum dimensions c-array
	hid_t status_lcl;  // declare a local return status

	hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array

	// set the c-array dimensions and maxiumum dimensions
	for (unsigned int ii=0; ii<rank; ii++)
	{
		mydims[ii] = dims[ii];  // vector to c-array
		maxdims[ii] = H5S_UNLIMITED;
	}

	// set the c-array chunk dimensions from the chunk dims vector
	for (unsigned int ii=0; ii<chnkdims.size(); ii++)
	{
		chunk_dims[ii] = chnkdims[ii];
	}

	// set the datatype to write
	datatype = H5Tcopy(H5T_NATIVE_INT);

	// if there are chunk dimensions, write the data this way
	if ( chnkdims.size()>0 )
	{
	   dataspace = H5Screate_simple(rank,mydims,maxdims);
	   hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
	   status_lcl = H5Pset_chunk( cparms, rank, chunk_dims );
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, cparms);
	}
	// otherwise, write the data this way
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	}

	// write the data
	H5Dwrite(array, datatype, dataspace, dataspace, H5P_DEFAULT, data);

	// close local hdf5 objects
	H5Sclose( dataspace );
	H5Tclose( datatype );
	H5Dclose( array );
}

dalIntArray::~dalIntArray() {
}

/********************************************************************
 *  dalFloatArray constructor creates an n-dimensional
 *    array of Floats.
 *
 *  arguments:
 *    voidfile (I) - dataset file handle
 *    arrayname (I) - name of the array to create
 *    dims (I) - vector of the array dimensions
 *    data (I) - array of data to write
 *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
 *      if the size of the array is fixed.
 *
 ********************************************************************/
dalFloatArray::dalFloatArray( void * voidfile, string arrayname,
		 vector<int> dims, float data[], vector<int> chnkdims ) {
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	hid_t array, datatype, dataspace;  // declare a few h5 variables

	// determine the rank from the size of the dimensions vector
	int rank = dims.size();

	hsize_t mydims[rank];  // declare a dimensions c-array
	hsize_t maxdims[rank]; // declare a maximum dimensions c-array
	hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array
	hid_t status_lcl;  // declare a local return status

	// set the c-array dimensions and maxiumum dimensions
	for (int ii=0; ii<rank; ii++)
	{
		mydims[ii] = dims[ii];
		maxdims[ii] = H5S_UNLIMITED;
	}

	// set the c-array chunk dimensions from the chunk dims vector
	for (unsigned int ii=0; ii<chnkdims.size(); ii++)
	{
		chunk_dims[ii] = chnkdims[ii];
	}

        // set the datatype to write
	datatype = H5Tcopy(H5T_NATIVE_FLOAT);

        // if there are chunk dimensions, write the data this way
	if ( chnkdims.size()>0 )
	{
	   dataspace = H5Screate_simple(rank,mydims,maxdims);
	   hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
	   status_lcl = H5Pset_chunk( cparms, rank, chunk_dims );
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, cparms);
	}
        // otherwise, write the data this way
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	}

        // write the data
	H5Dwrite(array, datatype, dataspace, dataspace, H5P_DEFAULT, data);

        // close local hdf5 objects
	H5Sclose( dataspace );
	H5Tclose( datatype );
	H5Dclose( array );
}

dalFloatArray::~dalFloatArray() {
}

/********************************************************************
 *  readIntArray reads an array of Integers.
 *
 *  arguments:
 *    voidfile (I) - dataset file handle
 *    arrayname (I) - name of the array to read
 *
 *  returns:
 *    pointer to an integer array
 *
 ********************************************************************/
int * dalIntArray::readIntArray( void * voidfile, string arrayname )
{
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	herr_t      lcl_status;

	hid_t filespace = H5Dget_space(file_id); // get the dataspace

	// retrieve the rank of the array
	hid_t data_rank = H5Sget_simple_extent_ndims(filespace);

	hsize_t dims[ data_rank ];  // create a dims c-array using the rank

	// retrieve the dimensions of the array
	lcl_status = H5Sget_simple_extent_dims(filespace, dims, NULL);

	long size = 1;  // the size of the array

	// from the dimensions, compute the full size of the array
	for (int ii=0; ii<data_rank; ii++)
	  size *= dims[ii];

	int * data = NULL;  // declare a pointer to the data
	data = new int[size];  // allocate space for the data

	// read the data into the local array
	lcl_status = H5LTread_dataset_int( file_id, arrayname.c_str(), data );

	// create a simple dataspace based on the data rank and dimensions
	//hid_t memspace = H5Screate_simple (data_rank,dims,NULL);

	// read the data into the local array
	//lcl_status = H5Dread (file_id, H5T_NATIVE_INT, memspace, filespace,
	//	H5P_DEFAULT, data);

	return data;  // return the data pointer
}

/********************************************************************
 *  dalComplexArray constructor creates an n-dimensional
 *    array of Complexs (defined in the DAL).
 *
 *  arguments:
 *    voidfile (I) - dataset file handle
 *    arrayname (I) - name of the array to create
 *    dims (I) - vector of the array dimensions
 *    data (I) - array of data to write
 *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
 *      if the size of the array is fixed.
 *
 ********************************************************************/
dalComplexArray::dalComplexArray( void * voidfile, string arrayname, vector<int> dims,
			  vector< complex<float> > data/*dalcomplex data[]*/, vector<int> chnkdims ) {
	hid_t * lclfile = (hid_t*)voidfile;
	hid_t file_id = *lclfile;  // get the file handle
	hid_t array, datatype, dataspace;  // declare a few h5 variables

        // determine the rank from the size of the dimensions vector
	unsigned int rank = dims.size();

	hsize_t mydims[rank];  // declare a dimensions c-array
	hsize_t maxdims[rank]; // declare a maximum dimensions c-array
	hid_t status_lcl;  // declare a local return status

	hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array

	// set the c-array dimensions and maxiumum dimensions
	for (unsigned int ii=0; ii<rank; ii++)
	{
		mydims[ii] = dims[ii];
		maxdims[ii] = H5S_UNLIMITED;
	}

	// set the c-array chunk dimensions from the chunk dims vector
	for (unsigned int ii=0; ii<chnkdims.size(); ii++)
	{
		chunk_dims[ii] = chnkdims[ii];
	}

	////////////////////////////////////////////////////////
	// THE FOLLOWING SHOULD BE MOVED TO A COMMON HEADER FILE
	//
	// declare a new datatype to hold complex values
	typedef struct mydalcomplex {
		float r;  // real
		float i;  // imaginary
	} mydalcomplex;
	// create a new hdf5 datatype for complex values
	hid_t complex_id = H5Tcreate (H5T_COMPOUND, sizeof(mydalcomplex));
	H5Tinsert (complex_id, "real", HOFFSET(mydalcomplex,r), H5T_NATIVE_FLOAT);
	H5Tinsert (complex_id, "imag", HOFFSET(mydalcomplex,i), H5T_NATIVE_FLOAT);
	//
	////////////////////////////////////////////////////////

	// set the datatype to write
	datatype = H5Tcopy( complex_id );

	// if there are chunk dimensions, write the data this way
	if ( chnkdims.size()>0 )
	{
	   dataspace = H5Screate_simple(rank,mydims,maxdims);
	   hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
	   status_lcl = H5Pset_chunk( cparms, rank, chunk_dims );
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, cparms);
	}
        // otherwise, write the data this way
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array = H5Dcreate( file_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	}

	// write the data
	H5Dwrite(array, datatype, dataspace, dataspace, H5P_DEFAULT, &data[0]);

	// close local hdf5 objects
	H5Sclose( dataspace );
	H5Tclose( datatype );
	H5Dclose( array );
}
