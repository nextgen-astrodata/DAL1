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

/*!
  \file dalDataset.cpp
  \ingroup DAL
  \author Joseph Masters
  \date 12-04-06
*/

#ifndef DALDATASET_H
#include "dalDataset.h"
#endif

/**
 * 
 * @param fname 
 * @return 
 */
int openFITS( char * fname )
{
	fitsfile * fptr;
	int status = 0;  /* MUST initialize status */
	int nkeys, ii;
	char card[500]; 
	//cout << "Try to open FITS...";
	
	fits_open_file(&fptr, fname, READWRITE, &status);
	
	//cout << "fits open file: " << status << endl;
	
	if (status)          /* print any error messages */
	{
	  //fits_report_error(stderr, status);
// 	  cout << " failed" << endl;
	  return status;
	}
	else
	{
	  cout << "opened a FITS file, closing" << endl;
        fits_get_hdrspace(fptr, &nkeys, NULL, &status);

        for (ii = 1; ii <= nkeys; ii++)  { 
          fits_read_record(fptr, ii, card, &status); /* read keyword */
          printf("%s\n", card);
        }
        printf("END\n\n");  /* terminate listing with END */

	  fits_close_file(fptr, &status);
	
	  return 0;
	}
}

/**
 * 
 * @param fname 
 * @return 
 */
hid_t openHDF5( char * fname )
{
	// the following return an integer file handle
	hid_t fh = H5Fopen(fname, H5F_ACC_RDWR, H5P_DEFAULT);
	if (fh >= 0)
		return fh;
	else
		return -1;
}

/**
 * 
 * @param filename 
 * @return 
 */
int dalDataset::open( char * filename )
{
  string lcltype;
//  void * myfile;
  
// cout << "trying to open fits..." << endl;
  if ( 0 == openFITS( filename ) )
  {
//     file = (fitsfile*)myfile;
    lcltype = FITSTYPE;
    cout << lcltype << " opened FITS!" << endl;
	exit(10);
  } else if ( (h5fh = openHDF5( filename )) >= 0 ) {
//      cout << " opened HDF5!" << endl;
     file = &h5fh;
     lcltype = H5TYPE;
	//cout << "returning " << SUCCESS << endl;
  }
  else
  {
    cout << "Unable to open file \"" << filename << "\"."
    	 << endl << "Please check permissions and file type." << endl;  
	return FAIL;
  }
  type = lcltype;
  return SUCCESS;
}

int dalDataset::close()
{
   if ( type == H5TYPE )
   {
      // << "Try to close HDF5... ";

//       if ( H5Fclose(*(hid_t*)file ) > 0 )
// cout << "h5fh: " << h5fh << endl;
      if ( H5Fclose( h5fh ) > 0 )
        return 0;
      else
	return -1;
    }
    return -2;
}

/**
 * 
 * @param gname 
 * @return 
 */
dalGroup * dalDataset::createGroup( char* gname )
{
    if ( type == H5TYPE )
   {
	hid_t * lclfile = &h5fh;
	   dalGroup * lg = new dalGroup( gname, lclfile );
	   return lg;
   }
   else
   	return NULL;  
}
dalDataset::dalDataset()
{}

/**
 * 
 * @param name 
 * @param filetype 
 * @return 
 */
dalDataset::dalDataset( char * name, string filetype )
{
   type = filetype;  // set the global class variable: type
   if ( filetype == H5TYPE )
   {

	h5fh = H5Fcreate(name, H5F_ACC_TRUNC, H5P_DEFAULT,
				   H5P_DEFAULT);
        file = &h5fh;
	
   } else if ( filetype == FITSTYPE )
   {
	fitsfile *fptr; /* pointer to the FITS file; defined in fitsio.h */
	int status;
	cout << "dalDataset::dalDataset FITS Type" << endl;
	fits_create_file(&fptr, name, &status); /*create new file*/
// h5fh = fits_create_file(fitsfile **fptr, char *filename, > int *status);
   }
   else {
   	cout << "Data format \'" << type << "\' not supported." << endl;
   	exit(99);
   }
}

dalDataset::~dalDataset()
{
}

dalArray * dalDataset::createIntArray( string arrayname, vector<int> dims, int data[], vector<int> cdims)
{
   if ( type == H5TYPE )
   {
	   dalIntArray * la = new dalIntArray( h5fh, arrayname, dims, data, cdims );
	   return la;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::createIntArray FITS Type" << endl;
	return NULL;
   }
   else
   	return NULL;
}

dalArray * dalDataset::createFloatArray( string arrayname, vector<int> dims, float data[], vector<int> cdims)
{
   if ( type == H5TYPE )
   {
	   dalFloatArray * la = new dalFloatArray( h5fh, arrayname, dims, data, cdims );
	   return la;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::createFloatArray FITS Type" << endl;
	return NULL;
   }
   else
   	return NULL;
}

dalArray * dalDataset::createComplexArray( string arrayname, vector<int> dims, vector< complex<float> > data/*dalcomplex data[]*/, vector<int> cdims)
{
   if ( type == H5TYPE )
   {
	   dalComplexArray * la = new dalComplexArray( file, arrayname, dims, data, cdims );
	   return la;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::createComplexArray FITS Type" << endl;
	return NULL;
   }
   else
   	return NULL;
}


/**
 * 
 * @param tablename 
 */
dalTable * dalDataset::createTable( string tablename )
{
   if ( type == H5TYPE )
   {
	   dalTable * lt = new dalTable();
	   lt->createTable( file, tablename, "/" );
	   return lt;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::createTable FITS Type" << endl;
	return NULL;
   }
   else
   	return NULL;
}

dalTable * dalDataset::createTable( string tablename, string groupname )
{
   if ( type == H5TYPE )
   {
	   dalTable * lt = new dalTable();
	   lt->createTable( file, tablename, '/' + groupname );
	   return lt;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::createTable FITS Type" << endl;
	return NULL;
   }
   else
   	return NULL;
}

/**
 * 
 * @param tablename 
 */
dalTable * dalDataset::openTable( string tablename )
{
   if ( type == H5TYPE )
   {
	   dalTable * lt = new dalTable();
	   lt->openTable( file, tablename, "/" );
	   return lt;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::openTable FITS Type" << endl;
	return NULL;
   }
   else
   	return NULL;
}

dalTable * dalDataset::openTable( string tablename, string groupname )
{
   if ( type == H5TYPE )
   {
	   dalTable * lt = new dalTable();
	   //cout << "Trying to open table " << endl; 
	   lt->openTable( file, tablename, '/' + groupname );
	   return lt;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::openTable FITS Type" << endl;
	return NULL;
   }
   else
   	return NULL;
}

dalGroup * dalDataset::openGroup( string groupname )
{
   if ( type == H5TYPE )
   {
	   dalGroup * group = new dalGroup();
	   //cout << "Trying to open group " << groupname << endl; 
	   int retval = group->open( file, groupname );
	   if ( retval < 0 )
		return NULL;
	   else
	   	return group;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::openGroup FITS Type" << endl;
	return NULL;
   }
   else
   {
   	cout << "dalDataset::openGroup type not supported" << endl;
	return NULL;
   }
}

string dalDataset::getType()
{
	return type;
}

#ifdef PYTHON
/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/

/******************************************************
 * wrappers for createTable
 ******************************************************/

dalTable * dalDataset::ct1_boost(string a) {
	dalTable * ret;
	ret = dalDataset::createTable(a);
	return ret;
}

dalTable * dalDataset::ct2_boost(string a, string b) {
	dalTable * ret;
	ret = dalDataset::createTable(a,b);
	return ret;
}

/******************************************************
 * wrappers for openTable
 ******************************************************/

dalTable * dalDataset::ot1_boost(string a) {
	dalTable * ret;
	ret = dalDataset::openTable(a);
	return ret;
}

dalTable * dalDataset::ot2_boost(string a, string b) {
	dalTable * ret;
	ret = dalDataset::openTable(a,b);
	return ret;
}

/******************************************************
 * wrappers for createIntArray
 ******************************************************/

dalArray * dalDataset::cia_boost1( string arrayname, bpl::list pydims,
				  bpl::list pydata ){
  bpl::list cdims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
     cdims.append(10);

  dalArray * array;
  array = cia_boost2( arrayname, pydims, pydata, cdims );

  return array;
}

dalArray * dalDataset::cia_boost2(
	string arrayname,
	bpl::list pydims,
	bpl::list pydata,
	bpl::list cdims ){

  vector<int> dims;
  vector<int> chnkdims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  for(int ii=0; ii<bpl::len(cdims); ii++)
    chnkdims.push_back(bpl::extract<int>(cdims[ii]));

  long size = bpl::len(pydata);
  int * data = NULL;
  data = new int[size];

  for(int ii=0; ii<size; ii++)
    data[ii] = bpl::extract<int>(pydata[ii]);

  dalArray * array = createIntArray(arrayname, dims, data, chnkdims);

  delete [] data;

  return array;
}

dalArray * dalDataset::cia_boost_numarray1(
	string arrayname, 
	bpl::list pydims,
	bpl::numeric::array pydata ){

  bpl::list cdims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
     cdims.append(10);

  dalArray * array;
  array = cia_boost_numarray2(arrayname, pydims, pydata, cdims);

  return array;
}

dalArray * dalDataset::cia_boost_numarray2(
	string arrayname, 
	bpl::list pydims,
	bpl::numeric::array pydata,
	bpl::list cdims ){

  vector<int> dims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  long size = bpl::len(pydata);
  int * data = NULL;
  data = new int[size];

// cout << "cia_boost_numarray "<< pydata.nelements() << endl;
  pydata.setshape( pydata.nelements() );
  bpl::object flat_data = pydata.getflat();
  bpl::list list_data( flat_data );

  dalArray * array = cia_boost2(arrayname, pydims, list_data, cdims);

  return array;
}

/******************************************************
 * wrappers for createFloatArray
 ******************************************************/
dalArray * dalDataset::cfa_boost( string arrayname, bpl::list pydims, bpl::list pydata, bpl::list cdims ){

  vector<int> dims;
  vector<int> chnkdims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  for(int ii=0; ii<bpl::len(cdims); ii++)
    chnkdims.push_back(bpl::extract<int>(cdims[ii]));

  long size = bpl::len(pydata);
  float * data = NULL;
  data = new float[size];

  for(int ii=0; ii<size; ii++)
    data[ii] = bpl::extract<float>(pydata[ii]);

  dalArray * array = dalDataset::createFloatArray(arrayname, dims, data, chnkdims);

  delete [] data;

  return array;
}

dalArray * dalDataset::cfa_boost_numarray( string arrayname, 
	bpl::list pydims,
	bpl::numeric::array pydata, bpl::list cdims ){

  vector<int> dims;

  for(int ii=0; ii<bpl::len(pydims); ii++)
    dims.push_back(bpl::extract<int>(pydims[ii]));

  long size = bpl::len(pydata);
  float * data = NULL;
  data = new float[size];

// cout << "cia_boost_numarray "<< pydata.nelements() << endl;
  pydata.setshape( pydata.nelements() );
  bpl::object flat_data = pydata.getflat();
  bpl::list list_data( flat_data );

  dalArray * array = dalDataset::cfa_boost(arrayname, pydims, list_data, cdims);

  return array;
}

// void dalDataset::sfe(bpl::numeric::array& y, int value)
// {
//     y[bpl::make_tuple(0,0)] = value;
// }

/******************************************************
 * wrapper for readIntArray
 ******************************************************/

bpl::numeric::array dalDataset::ria_boost( string arrayname )
{
	hid_t lclfile;
	hid_t  status;
// 	hid_t datatype, dataspace;

	// get the dataspace
	lclfile = H5Dopen(h5fh, arrayname.c_str());
	hid_t filespace = H5Dget_space(lclfile);

	// what is the rank of the array?
	hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
	hsize_t dims[ data_rank ];
// cout << "data rank: " << data_rank << endl;
	status = H5Sget_simple_extent_dims(filespace, dims, NULL);

	int size = 1;
	bpl::list dims_list;
	for (int ii=0; ii<data_rank; ii++)
	{
// cout << "dims["  << ii << "]: " << dims[ii] << endl;
	  size *= dims[ii];
	  dims_list.append(dims[ii]);
	}
// cout << "size: " << size << endl;

	int * data = NULL;
	data = new int[size];

	status = H5LTread_dataset_int( h5fh, arrayname.c_str(), data );
// 	for (int ii=0; ii<size; ii++)
// 	{
// 	  cout << data[ii] << endl;
// 	}

	bpl::list data_list;
	// for each dimension
	for (int ii=0; ii<size; ii++)
	{
	    data_list.append(data[ii]);
	}
	bpl::numeric::array nadata(
          bpl::make_tuple(
	    bpl::make_tuple(data_list)
	  )
	);
// 	dims_list.reverse();
	nadata.setshape(dims_list);
	return nadata;
}

/******************************************************
 * wrapper for readFloatArray
 ******************************************************/

bpl::numeric::array dalDataset::rfa_boost( string arrayname )
{
	hid_t lclfile;
	hid_t status;
// 	hid_t datatype, dataspace;

	// get the dataspace
	lclfile = H5Dopen(h5fh, arrayname.c_str());
	hid_t filespace = H5Dget_space(lclfile);

	// what is the rank of the array?
	hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
	hsize_t dims[ data_rank ];

	status = H5Sget_simple_extent_dims(filespace, dims, NULL);

	int size = 1;
	bpl::list dims_list;
	for (int ii=0; ii<data_rank; ii++)
	{
	  size *= dims[ii];
	  dims_list.append(dims[ii]);
	}

	float * data = NULL;
	data = new float[size];

	status = H5LTread_dataset_float( h5fh, arrayname.c_str(), data );

	bpl::list data_list;
	// for each dimension
	for (int ii=0; ii<size; ii++)
	{
	    data_list.append(data[ii]);
	}
	bpl::numeric::array nadata(
          bpl::make_tuple(
	    bpl::make_tuple(data_list)
	  )
	);
	nadata.setshape(dims_list);
	return nadata;
}
#endif
