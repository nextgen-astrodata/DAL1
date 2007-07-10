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
	  cout << " failed" << endl;
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
  
  if ( 0 == openFITS( filename ) )
  {
//     file = (fitsfile*)myfile;
    lcltype = FITSTYPE;
    cout << lcltype << " opened FITS!" << endl;
	exit(10);
  } else if ( (h5fh = openHDF5( filename )) >= 0 ) {
//     cout << " opened HDF5!" << endl;
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
      //cout << "Try to close HDF5... ";

       if ( H5Fclose(*(hid_t*)file ) >= 0 )
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
 * @param type 
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

/**
 * 
 * @return 
 */
dalDataset::~dalDataset()
{
}

/**
 * 
 * @param arrayname 
 */
dalArray * dalDataset::createArray( string arrayname, vector<int> dims, int data[])
{
   if ( type == H5TYPE )
   {
	   dalIntArray * la = new dalIntArray( file, arrayname, dims, data );
	   return la;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::createArray FITS Type" << endl;
	return NULL;
   }
   else
   	return NULL;
}

dalArray * dalDataset::createArray( string arrayname, vector<int> dims, float data[])
{
   if ( type == H5TYPE )
   {
	   dalFloatArray * la = new dalFloatArray( file, arrayname, dims, data );
	   return la;
   }
   else if ( type == FITSTYPE )
   {
	cout << "dalDataset::createArray FITS Type" << endl;
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
