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

#ifndef DALARRAY_H
#include "dalArray.h"
#endif

namespace DAL {

dalArray::dalArray()
{
  array_id = 0;
  file_id = 0;
  rank = 0;
  datatype = "";
  status = 0;
  name = "";
}

int dalArray::open( void * voidfile, string arrayname ) {
	name = arrayname;

	hid_t * lclfile = (hid_t*)voidfile; // H5File object
	file_id = *lclfile;  // get the file handle
	
	string fullarrayname = arrayname;
	array_id = H5Dopen( file_id, fullarrayname.c_str() );
	return( array_id );
}

int dalArray::close()
{
	status = H5Dclose(array_id);
	if ( status < 0 )
	  cout << "ERROR: dalArray::close() failed.\n";
	return status;
}

void dalArray::write( int offset, int data[], int arraysize )
{
    hsize_t      dims[1] = { arraysize };
    int rank=1;
    hsize_t off[1] = { offset };

    /* Select a hyperslab  */
    hid_t filespace = H5Dget_space( array_id );
    status = H5Sselect_hyperslab( filespace, H5S_SELECT_SET, off, NULL,
                                  dims, NULL);  

    /* Define memory space */
    hid_t dataspace = H5Screate_simple (rank, dims, NULL); 

    /* Write the data to the hyperslab  */
    status = H5Dwrite (array_id, H5T_NATIVE_INT, dataspace, filespace,
                       H5P_DEFAULT, data);
}

void dalArray::write( int offset, short data[], int arraysize )
{
    hsize_t      dims[1] = { arraysize };
    int rank=1;
    hsize_t off[1] = { offset };

    /* Select a hyperslab  */
    hid_t filespace = H5Dget_space( array_id );
    status = H5Sselect_hyperslab( filespace, H5S_SELECT_SET, off, NULL,
                                  dims, NULL);

    /* Define memory space */
    hid_t dataspace = H5Screate_simple (rank, dims, NULL);

    /* Write the data to the hyperslab  */
    status = H5Dwrite (array_id, H5T_NATIVE_SHORT, dataspace, filespace,
                       H5P_DEFAULT, data);
}

void dalArray::write( int offset, complex<float> data[], int arraysize )
{
    hsize_t      dims[1] = { arraysize };
    hsize_t off[1] = { offset };

    /* Select a hyperslab  */
    hid_t filespace = H5Dget_space( array_id );
    status = H5Sselect_hyperslab( filespace, H5S_SELECT_SET, off, NULL,
                                  dims, NULL);

    /* Define memory space */
	typedef struct {
	double re;   /*real part*/
	double im;   /*imaginary part*/
	} complex_t;
	
	hid_t complex_id = H5Tcreate (H5T_COMPOUND, sizeof(complex_t));
	H5Tinsert (complex_id, "real", HOFFSET(complex_t,re),
		H5T_NATIVE_DOUBLE);
	H5Tinsert (complex_id, "imaginary", HOFFSET(complex_t,im),
		H5T_NATIVE_DOUBLE);
		

    /* Write the data to the hyperslab  */
    status = H5Dwrite (array_id, complex_id, filespace, filespace, H5P_DEFAULT, data);
}

void dalArray::write( int offset, complex<Int16> data[], int arraysize )
{
    hsize_t      dims[1] = { arraysize };
    hsize_t off[1] = { offset };

    /* Select a hyperslab  */
    hid_t filespace = H5Dget_space( array_id );
    status = H5Sselect_hyperslab( filespace, H5S_SELECT_SET, off, NULL,
                                  dims, NULL);

    /* Define memory space */
    typedef struct {
      Int16 re;   /*real part*/
      Int16 im;   /*imaginary part*/
    } complex_t;

    hid_t complex_id = H5Tcreate (H5T_COMPOUND, sizeof(complex_t));
    H5Tinsert ( complex_id, "real", HOFFSET(complex_t,re),
                H5T_NATIVE_DOUBLE );
    H5Tinsert ( complex_id, "imaginary", HOFFSET(complex_t,im),
                H5T_NATIVE_DOUBLE );

    /* Write the data to the hyperslab  */
    status = H5Dwrite( array_id, complex_id, filespace, filespace,
                       H5P_DEFAULT, data );
}

vector<int> dalArray::dims()
{
    vector<int> return_values;
    hid_t dataspace = H5Dget_space( array_id );    /* dataspace identifier */
    int rank      = H5Sget_simple_extent_ndims(dataspace);
    hsize_t      dims_out[rank];
    status  = H5Sget_simple_extent_dims(dataspace, dims_out, NULL);
    for (int ii=0; ii<rank; ii++)
    {
       return_values.push_back( dims_out[ii] );
    }
    return return_values;
}

std::string dalArray::getName()
{
   return name;
}

void dalArray::extend( vector<int> newdims )
{
   unsigned int rank = newdims.size();
   hsize_t lcldims[ rank ];
   for ( unsigned int ii=0; ii < rank; ii++ )
      lcldims[ ii ] = newdims[ ii ];

   status = H5Dextend( array_id, lcldims );
}

void dalArray::getAttributes()
{

   //status = H5Aget_num_attrs(group_id);
   //printf ("H5Aget_num_attrs returns: %i\n", status);
   status = H5Aiterate( array_id, NULL, attr_info, NULL );
   //printf ("\nH5Aiterate returns: %i\n", status);

}

/****************************************************************
 *  (currently assumes hdf5) return the value of the (scalar) attribute
 *
 *****************************************************************/
void * dalArray::getAttribute( string attrname ) {

/*   if ( type == H5TYPE )
   {*/
	hsize_t dims;
	H5T_class_t type_class;
	size_t type_size;

	// Check if attribute exists
	if ( H5LT_find_attribute( array_id, attrname.c_str() ) <= 0 ) {
		return NULL;
	}
	
	string fullname = name;
	int rank;
	H5LTget_attribute_ndims( file_id, fullname.c_str(),
	  attrname.c_str(), &rank );
	H5LTget_attribute_info( file_id, fullname.c_str(), attrname.c_str(),
				&dims, &type_class, &type_size );

	if ( H5T_FLOAT == type_class ) {
		float * data;
		data = new float[1];
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(), 
		  attrname.c_str(), H5T_NATIVE_FLOAT, data) )
		  return NULL;
		else{
		  return data;}
	}
	else if ( H5T_INTEGER == type_class ) {
		int * data;
		data = new int[1];
		if ( 0 < H5LTget_attribute(file_id, fullname.c_str(),
		  attrname.c_str(),
			H5T_NATIVE_INT, data) )
		  return NULL;
		else
		  return reinterpret_cast<int*>(data);
	}
	else if ( H5T_STRING == type_class ) {
		char * data;
		string fullname = "/" + name;
		data = new char[ 256 ];
		if ( 0 < H5LTget_attribute_string( file_id, fullname.c_str(),
			  attrname.c_str(),data) )
		  return NULL;
		else
		  return data;
	}
	else
	{
		return NULL;
	}
/*   } else {
     cout << "Operation not yet supported for type " << type << ".  Sorry.\n";
     return NULL;
   }*/
}

void dalArray::setAttribute_string( string attrname, string data )
{
   /* Create string attribute.  */
   herr_t ret;
   hid_t aid1;
   hid_t atype;
   hsize_t  dims[1] = {1};
   aid1  = H5Screate_simple (1, dims, NULL);
   atype = H5Tcopy(H5T_C_S1);
   ret = H5Tset_size(atype, data.length());
   ret = H5Tset_strpad(atype,H5T_STR_NULLTERM);
   hid_t attr1 = H5Acreate(array_id, attrname.c_str(), atype, aid1, H5P_DEFAULT);
   ret = H5Awrite(attr1, atype, data.c_str());
   if (ret < 0)
     cout << "ERROR: could not set attribute " << attrname << endl;
}
 
void dalArray::setAttribute_int( string attrname, int * data/*, int size*/ )
{
   /* Create scalar attribute.  */
   hid_t   attr1; /* Attribute identifier */
   hid_t   aid1;  /* Attribute dataspace identifier */
   herr_t  ret;   /* Return value */
   aid1  = H5Screate(H5S_SCALAR);
   attr1 = H5Acreate(array_id, attrname.c_str(), H5T_NATIVE_INT, aid1, H5P_DEFAULT);
   ret = H5Awrite(attr1, H5T_NATIVE_INT, data);
   if (ret < 0)
     cout << "ERROR: could not set attribute " << attrname << endl;
}

void dalArray::setAttribute_uint( string attrname, unsigned int * data/*, int size*/ )
{
   /* Create scalar attribute.  */
   hid_t   attr1; /* Attribute identifier */
   hid_t   aid1;  /* Attribute dataspace identifier */
   herr_t  ret;   /* Return value */
   aid1  = H5Screate(H5S_SCALAR);
   attr1 = H5Acreate(array_id, attrname.c_str(), H5T_NATIVE_UINT, aid1, H5P_DEFAULT);
   ret = H5Awrite(attr1, H5T_NATIVE_UINT, data);
   if (ret < 0)
     cout << "ERROR: could not set attribute " << attrname << endl;
}

void dalArray::setAttribute_float( string attrname, float * data/*, int size*/ )
{
   /* Create scalar attribute.  */
   hid_t   attr1; /* Attribute identifier */
   hid_t   aid1;  /* Attribute dataspace identifier */
   herr_t  ret;   /* Return value */
   aid1  = H5Screate(H5S_SCALAR);
   attr1 = H5Acreate(array_id, attrname.c_str(), H5T_NATIVE_FLOAT, aid1, H5P_DEFAULT);
   ret = H5Awrite(attr1, H5T_NATIVE_FLOAT, data);
   if (ret < 0)
     cout << "ERROR: could not set attribute " << attrname << endl;
}

void dalArray::setAttribute_double( string attrname, double * data/*, int size*/ )
{
   /* Create scalar attribute.  */
   hid_t   attr1; /* Attribute identifier */
   hid_t   aid1;  /* Attribute dataspace identifier */
   herr_t  ret;   /* Return value */
   aid1  = H5Screate(H5S_SCALAR);
   attr1 = H5Acreate(array_id, attrname.c_str(), H5T_NATIVE_DOUBLE, aid1, H5P_DEFAULT);
   ret = H5Awrite(attr1, H5T_NATIVE_DOUBLE, data);
   if (ret < 0)
     cout << "ERROR: could not set attribute " << attrname << endl;
}

dalShortArray::dalShortArray(){}
dalIntArray::dalIntArray(){}

dalShortArray::dalShortArray( hid_t obj_id, string arrayname,
			  vector<int> dims, short data[] )
{
   vector<int> chnkdims;

   for( unsigned int ii=0; ii < dims.size(); ii++)
      chnkdims.push_back(10);

	hid_t datatype, dataspace;  // declare a few h5 variables

	name = arrayname;  // set the private name variable to the array name

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
	datatype = H5Tcopy(H5T_NATIVE_SHORT);

	// if there are chunk dimensions, write the data this way
	if ( chnkdims.size()>0 )
	{
	   dataspace = H5Screate_simple(rank,mydims,maxdims);
	   hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
	   status_lcl = H5Pset_chunk( cparms, rank, chunk_dims );
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype, dataspace, cparms);
	}
	// otherwise, write the data this way
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
			dataspace, H5P_DEFAULT);
	}

	// write the data
	H5Dwrite(array_id, datatype, dataspace, dataspace, H5P_DEFAULT, data);

	// close local hdf5 objects
	H5Sclose( dataspace );
	H5Tclose( datatype );
}

dalIntArray::dalIntArray( hid_t obj_id, string arrayname,
			  vector<int> dims, int data[] )
{
   vector<int> chnkdims;

   for( unsigned int ii=0; ii < dims.size(); ii++)
      chnkdims.push_back(10);

// 	hid_t * lclfile = (hid_t*)voidfile;
// 	file_id = *lclfile;  // get the file handle

	hid_t datatype, dataspace;  // declare a few h5 variables

	name = arrayname;  // set the private name variable to the array name

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
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype, dataspace, cparms);
	}
	// otherwise, write the data this way
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	}

	// write the data
	H5Dwrite(array_id, datatype, dataspace, dataspace, H5P_DEFAULT, data);

	// close local hdf5 objects
	H5Sclose( dataspace );
	H5Tclose( datatype );
// 	H5Dclose( array );

}

/********************************************************************
 *  dalShortArray constructor creates an n-dimensional
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
dalShortArray::dalShortArray( hid_t obj_id, string arrayname,
			vector<int> dims, short data[], vector<int> chnkdims )
{
	hid_t datatype, dataspace;  // declare a few h5 variables

	name = arrayname;  // set the private name variable to the array name

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
	datatype = H5Tcopy(H5T_NATIVE_SHORT);

	// if there are chunk dimensions, write the data this way
	if ( chnkdims.size()>0 )
	{
	   dataspace = H5Screate_simple(rank,mydims,maxdims);
	   hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
	   status_lcl = H5Pset_chunk( cparms, rank, chunk_dims );
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
				dataspace, cparms);
	}
	// otherwise, write the data this way
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
			dataspace, H5P_DEFAULT);
	}

	// write the data
	H5Dwrite(array_id, datatype, dataspace, dataspace, H5P_DEFAULT, data);

	// close local hdf5 objects
	H5Sclose( dataspace );
	H5Tclose( datatype );
}

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
dalIntArray::dalIntArray( hid_t obj_id/*void * voidfile*/, string arrayname,
			  vector<int> dims, int data[], vector<int> chnkdims )
{
// 	hid_t * lclfile = (hid_t*)voidfile;
// 	file_id = *lclfile;  // get the file handle

	hid_t datatype, dataspace;  // declare a few h5 variables

	name = arrayname;  // set the private name variable to the array name

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
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
				dataspace, cparms);
	}
	// otherwise, write the data this way
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
			dataspace, H5P_DEFAULT);
	}

	// write the data
	H5Dwrite(array_id, datatype, dataspace, dataspace, H5P_DEFAULT, data);

	// close local hdf5 objects
	H5Sclose( dataspace );
	H5Tclose( datatype );
}

dalIntArray::~dalIntArray() {}

dalShortArray::~dalShortArray() {}

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
dalFloatArray::dalFloatArray( hid_t obj_id, string arrayname,
		 vector<int> dims, float data[], vector<int> chnkdims ) {
/*	hid_t * lclfile = (hid_t*)voidfile;
	file_id = *lclfile;  // get the file handle*/
	hid_t datatype, dataspace;  // declare a few h5 variables

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
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype, dataspace, cparms);
	}
        // otherwise, write the data this way
	else
	{
	   dataspace = H5Screate_simple(rank,mydims,NULL);
	   array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype, dataspace, H5P_DEFAULT);
	}

        // write the data
	H5Dwrite(array_id, datatype, dataspace, dataspace, H5P_DEFAULT, data);

        // close local hdf5 objects
	H5Sclose( dataspace );
	H5Tclose( datatype );
// 	H5Dclose( array );
}

dalFloatArray::~dalFloatArray() {
}

/********************************************************************
 *  readShortArray reads an array of Shorts.
 *
 *  arguments:
 *    obj_id (I) - file or group ID
 *    arrayname (I) - name of the array to read
 *
 *  returns:
 *    pointer to an short array
 *
 ********************************************************************/
short * dalShortArray::readShortArray( hid_t obj_id, string arrayname )
{
	herr_t      lcl_status;

	hid_t filespace = H5Dget_space(obj_id); // get the dataspace

	// retrieve the rank of the array
	hid_t data_rank = H5Sget_simple_extent_ndims(filespace);

	hsize_t dims[ data_rank ];  // create a dims c-array using the rank

	// retrieve the dimensions of the array
	lcl_status = H5Sget_simple_extent_dims(filespace, dims, NULL);

	long size = 1;  // the size of the array

	// from the dimensions, compute the full size of the array
	for (int ii=0; ii<data_rank; ii++)
	  size *= dims[ii];

	short * data = NULL;  // declare a pointer to the data
	data = new short[size];  // allocate space for the data

	// read the data into the local array
	lcl_status = H5LTread_dataset_short( obj_id, arrayname.c_str(), data );

	return data;  // return the data pointer
}

/********************************************************************
 *  readIntArray reads an array of Integers.
 *
 *  arguments:
 *    obj_id (I) - file or group ID
 *    arrayname (I) - name of the array to read
 *
 *  returns:
 *    pointer to an integer array
 *
 ********************************************************************/
int * dalIntArray::readIntArray( hid_t obj_id, string arrayname )
{
/*	hid_t * lclfile = (hid_t*)voidfile;
	file_id = *lclfile;  // get the file handle*/
	herr_t      lcl_status;

	hid_t filespace = H5Dget_space(obj_id); // get the dataspace

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
	lcl_status = H5LTread_dataset_int( obj_id, arrayname.c_str(), data );

	// create a simple dataspace based on the data rank and dimensions
	//hid_t memspace = H5Screate_simple (data_rank,dims,NULL);

	// read the data into the local array
	//lcl_status = H5Dread (file_id, H5T_NATIVE_INT, memspace, filespace,
	//	H5P_DEFAULT, data);

	return data;  // return the data pointer
}

/********************************************************************
 *  dalComplexArray_float32 constructor creates an n-dimensional
 *    array of Complexs (defined in the DAL).
 *
 *  arguments:
 *    obj_id (I) - dataset file handle
 *    arrayname (I) - name of the array to create
 *    dims (I) - vector of the array dimensions
 *    data (I) - complex<float> vector of data to write
 *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
 *      if the size of the array is fixed.
 *
 ********************************************************************/
dalComplexArray_float32::dalComplexArray_float32( hid_t obj_id,
                                                  string arrayname,
                                                  vector<int> dims,
                                                  complex<float> data[],
                                                  vector<int> chnkdims )
{
  hid_t datatype, dataspace;  // declare a few h5 variables
  name = arrayname;  // set the private name variable to the array name

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

  // create a new hdf5 datatype for complex values
  hid_t complex_id = H5Tcreate (H5T_COMPOUND, sizeof(dalcomplex_float32));
  H5Tinsert (complex_id, "real", HOFFSET(dalcomplex_float32,r), H5T_NATIVE_FLOAT);
  H5Tinsert (complex_id, "imag", HOFFSET(dalcomplex_float32,i), H5T_NATIVE_FLOAT);
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
     array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
                           dataspace, cparms);
  }
  // otherwise, write the data this way
  else
  {
     dataspace = H5Screate_simple(rank,mydims,NULL);
     array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
                           dataspace, H5P_DEFAULT);
  }

  // write the data
  H5Dwrite(array_id, datatype, dataspace, dataspace, H5P_DEFAULT, data);

  // close local hdf5 objects
  H5Sclose( dataspace );
  H5Tclose( datatype );

}

/********************************************************************
 *  dalComplexArray_int16 constructor creates an n-dimensional
 *    array of Complexs (defined in the DAL).
 *
 *  arguments:
 *    obj_id (I) - dataset file handle
 *    arrayname (I) - name of the array to create
 *    dims (I) - vector of the array dimensions
 *    data (I) - complex<float> vector of data to write
 *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
 *      if the size of the array is fixed.
 *
 ********************************************************************/
dalComplexArray_int16::dalComplexArray_int16( hid_t obj_id,
                                              string arrayname,
                                              vector<int> dims,
                                              complex<Int16> data[],
                                              vector<int> chnkdims )
{
  hid_t datatype, dataspace;  // declare a few h5 variables
  name = arrayname;  // set the private name variable to the array name

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

  // create a new hdf5 datatype for complex values
  hid_t complex_id = H5Tcreate (H5T_COMPOUND, sizeof(dalcomplex_int16));
  H5Tinsert (complex_id, "real", HOFFSET(dalcomplex_int16,r), H5T_NATIVE_SHORT);
  H5Tinsert (complex_id, "imag", HOFFSET(dalcomplex_int16,i), H5T_NATIVE_SHORT);
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
     array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
                           dataspace, cparms);
  }
  // otherwise, write the data this way
  else
  {
     dataspace = H5Screate_simple(rank,mydims,NULL);
     array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
                           dataspace, H5P_DEFAULT);
  }

  // write the data
  H5Dwrite(array_id, datatype, dataspace, dataspace, H5P_DEFAULT, data);

  // close local hdf5 objects
  H5Sclose( dataspace );
  H5Tclose( datatype );

}

#ifdef PYTHON
/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
 
/******************************************************
 * wrapper for dalArray::setAttribute_int
 ******************************************************/
void dalArray::sai_boost( string attrname, int data )
{
   setAttribute_int( attrname, &data/*, 1*/ );
}
 
/******************************************************
 * wrapper for dalArray::setAttribute_float
 ******************************************************/
void dalArray::saf_boost( string attrname, float data )
{
   setAttribute_float( attrname, &data/*, 1*/ );
}

/******************************************************
 * wrapper for dalArray::extend
 ******************************************************/
void dalArray::extend_boost( bpl::list pydims )
{
  vector<int> dims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  extend( dims );
}

/******************************************************
 * wrapper for dalIntArray::write
 ******************************************************/
// void dalIntArray::write( bpl::list pyoffset, bpl::list pywrite_dims, int data)
// {
// 
// }

#endif // end #ifdef PYTHON

} // end namespace DAL
