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

#ifndef DALDATASET_H
#define DALDATASET_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALTABLE_H
#include "dalTable.h"
#endif

#ifndef DALGROUP_H
#include "dalGroup.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

const string H5TYPE = "HDF5";
const string FITSTYPE = "FITS";

/*!
  \class dalDataset
  
  \ingroup DAL

  \author Joseph Masters

  \date 12-04-06

  The dalDataset is the highest level container for dalData.  It may
  consist of one or more files on disk, each of which contain multiple tables, 
  images and attributes.  These tables and images can be gruoped.
*/
class dalDataset{

	void * file;  /// can be HDF5File, FITS, MS
	hid_t h5fh;   /// hdf5 file handle
	string type;  /// "HDF5", "MS" or "FITS"; for example
	string name;  /// dataset name
	vector<string> files;  /// list of files
	vector<dalTable> tables; /// list of tables
	vector<dalGroup> groups; /// list of groups
	vector<dalAttribute> attrs;  /// list of attributes
	
	dalFilter filter;	/// dataset filter
  
  public:
  	/// The constructor
  	dalDataset();
	/// The destructor
	~dalDataset();
	
	/// Another constructor with two arguments
	/**	\param a A charachter string
		\param b Another string
	*/
  	dalDataset( char * name, string filetype="HDF5" );
	
	int open( char * datasetname ); /// open the dataset
	int close(); /// close the dataset
	 /// return a list of filenames contained within the dataset
//	int getfiles();
//	int checkaccess(); /// check read/write access of specified file
//	int copy(); /// create an exact copy of the data
//	int create();  /// define basic characteristics of a new dataset
	/// create a new table in the root group
	dalTable * createTable( string tablename );
	/// create a new table in a specified group
	dalTable * createTable( string tablename, string groupname );
//	int createImage();  /// create a new table outside of a group
	dalGroup * createGroup( char* groupname );  /// create a new group
//	int getName();  /// retrieve the name of the dataset
//	int rename();  /// rename the dataset
	dalTable * openTable( string tablename );  /// return a dalTable object
	dalTable * openTable( string tablename, string groupname );
	dalGroup * openGroup( string groupname );  /// return a dalGroup object
//	int listGroups();  /// return a list of groups within the dataset
	/// return a list of tables not contained in any groups
//	int listTables();
	/// list general attributes attached to the dataset
//	int listAttributes();;
//	int remove();  /// delete the file and all data within it
//	int advanceTable();  /// sequentially step through the dataset tables
//	int advanceGroup();  /// sequentially step through the dataset groups
//	int getTableNumber();  /// return a dalTable by index
//	int getGroupNumber(); /// return a dalGroup by index
	string getType(); /// retrieve the dataset type
    
};
#endif
