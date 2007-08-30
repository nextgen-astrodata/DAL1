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

#ifndef DALDATASET_H
#define DALDATASET_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALARRAY_H
#include "dalArray.h"
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
	string type;  /// "HDF5", "MSCASA" or "FITS"; for example
	string name;  /// dataset name
	vector<string> files;  /// list of files
	vector<dalTable> tables; /// list of tables
	vector<dalGroup> groups; /// list of groups
	vector<dalAttribute> attrs;  /// list of attributes
	
	dalFilter filter;	/// dataset filter

	// hdf5-specific variables
	hid_t h5fh;   /// hdf5 file handle

#ifdef WITH_CASA
	// casa-specific variables
	casa::MeasurementSet * ms;
	casa::MSReader * ms_reader;
	casa::Vector<casa::String> ms_tables;
#endif

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
	/// create a new array in the root group
	dalArray * createIntArray(
				string arrayname,
				vector<int> dims,
				int data[]);
	dalArray * createIntArray(
				string arrayname,
				vector<int> dims,
				int data[],
				vector<int>cdims);
	dalArray * createFloatArray(
				string arrayname,
				vector<int> dims,
				float data[],
				vector<int>cdims);
	dalArray * createComplexArray(
				string arrayname,
				vector<int> dims,
				vector< complex<float> > data,
				vector<int>cdims);


	/// create a new table in the root group
	dalTable * createTable( string tablename );
	/// create a new table in a specified group
	dalTable * createTable( string tablename, string groupname );

//	int createImage();  /// create a new table outside of a group
	dalGroup * createGroup( char* groupname );  /// create a new group
//	int getName();  /// retrieve the name of the dataset
//	int rename();  /// rename the dataset
	dalTable * openTable( string tablename );  /// return a dalTable object

#ifdef WITH_CASA
	dalTable * openFilteredTable( string tablename, string parse_string );
#endif
	dalTable * openTable( string tablename, string groupname );

	dalGroup * openGroup( string groupname );  /// return a dalGroup object
//	int listGroups();  /// return a list of groups within the dataset
// return a list of tables not contained in any groups
	void listTables();
// list general attributes attached to the dataset
//	int listAttributes();;
//	int remove();  /// delete the file and all data within it
//	int advanceTable();  /// sequentially step through the dataset tables
//	int advanceGroup();  /// sequentially step through the dataset groups
//	int getTableNumber();  /// return a dalTable by index
//	int getGroupNumber(); /// return a dalGroup by index
	string getType(); /// retrieve the dataset type

/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
#ifdef PYTHON
	// create[]Array wrappers
	dalArray * cia_boost1(string arrayname, bpl::list dims, bpl::list data);
	dalArray * cia_boost2(string arrayname, bpl::list dims, bpl::list data,
				 bpl::list cdims );
	dalArray * cia_boost_numarray1( string arrayname, bpl::list dims,
			bpl::numeric::array data );
	dalArray * cia_boost_numarray2( string arrayname, bpl::list dims,
			bpl::numeric::array data, bpl::list cdims );
	dalArray * cfa_boost( string arrayname, bpl::list dims, bpl::list data,
				 bpl::list cdims );
	dalArray * cfa_boost_numarray( string arrayname, bpl::list dims,
			 bpl::numeric::array data, bpl::list cdims );

// 	void sfe(bpl::numeric::array& y, int value);

	bpl::numeric::array ria_boost( string arrayname );
	bpl::numeric::array rfa_boost( string arrayname );

	// createTable wrappers
	dalTable * ct1_boost( string tablename );
	dalTable * ct2_boost( string tablename, string groupname );

	// openTable wrappers
	dalTable * ot1_boost( string tablename );
	dalTable * ot2_boost( string tablename, string groupname );
#endif
};
#endif
