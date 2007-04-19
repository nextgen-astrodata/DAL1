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
	
	//cout << "Try to open FITS...";
	
	fits_open_file(&fptr, fname, READONLY, &status);
	
	//cout << "fits open file: " << status << endl;
	
	if (status)          /* print any error messages */
	{
	  //fits_report_error(stderr, status);
	  //cout << " failed" << endl;
	  return status;
	}
	else
	{
	  //cout << "opened a FITS file, closing" << endl;
	  fits_close_file(fptr, &status);
	
	  return 0;
	}
}

/**
 * 
 * @param fname 
 * @return 
 */
void* openHDF5( char * fname )
{
	//cout << "Try to open HDF5... ";

	try {
	 
	 /*
          * Turn off the auto-printing when failure occurs so that we can
          * handle the errors appropriately
          */
	  H5::Exception::dontPrint();
	  
	  // Try to open the file as HDF5
	  H5::H5File* file = new H5::H5File( fname, H5F_ACC_RDWR );
	  
	  if ( (*file).isHdf5( fname )  )
	  {
	    //cout << "YES - is HDF5." << endl;
	    //cout << "success" << endl;
	    return (H5::H5File*)file;
	  }
	  else
	  {
	    //cout << "NO - is not HDF5." << endl;
	    //cout << "failed" << endl;
	    return NULL;
	  }
	  
	  /*
	  string h5name = (*file).getFileName();
	  cout << "Name: " << h5name << endl;
	  hsize_t h5size = (*file).getFileSize();
	  cout << "Size: " << h5size << endl;
	  int obcount = (*file).getObjCount();
	  cout << "Object count: " << obcount << endl;

	  H5::Group root = (*file).openGroup( "/" );
	  H5::Group sub = (*file).createGroup( "sub2" );  // careful!
	  
  	  H5::FileAccPropList plist = (*file).getAccessPlist();
	  
	 */
	 }

	 // catch failure caused by the H5File operations
	catch( H5::FileIException error )
	{
	  error.printError();
	  return NULL;
	}
	
	return NULL;
}

/**
 * 
 * @param filename 
 * @return 
 */
int dalDataset::open( char * filename )
{
  string lcltype;
  void * myfile;
  
  if ( 0 == openFITS( filename ) )
  {
    //cout << " opened FITS!" << endl;
    file = (fitsfile*)myfile;
    lcltype = FITSTYPE;
  } else if ( NULL != (myfile = openHDF5( filename )) ) {
    //cout << " opened HDF5!" << endl;
    file = (H5::H5File*)myfile;
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
      try {
  	  /* Turn off the auto-printing when failure occurs so that we can
          * handle the errors appropriately
          */
	  H5::Exception::dontPrint();
	  
	  H5::H5File * f = (H5::H5File*)file;
	  (*f).close();
	  //cout << "success" << endl;
	  
      }
      catch( H5::FileIException error )
	{
	  //cout << "failed" << endl;
	  error.printError();
	  return -1;
	}
    }
    return 0;
}

/**
 * 
 * @param gname 
 * @return 
 */
dalGroup * dalDataset::createGroup( string gname )
{
    if ( type == H5TYPE )
   {
   	   H5::H5File* lclfile = (H5::H5File*)file;
	   dalGroup * lg = new dalGroup( lclfile, gname);
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

        H5::H5File* lclfile = new H5::H5File( name, H5F_ACC_TRUNC );
		file = (H5::H5File*)lclfile;
		// 	cout << (H5::H5File*)file.getFileName() << endl;
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
   if ( type == H5TYPE )
   {
      H5::H5File * lclfile = (H5::H5File*)file;
      delete lclfile;
   }
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
   else
   	return NULL;
}

string dalDataset::getType()
{
	return type;
}
