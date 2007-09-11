/*-------------------------------------------------------------------------*
 | $Id:: dalDataset.h 389 2007-06-13 08:47:09Z baehren                   $ |
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
#define dalArray_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

/*!
  \class dalIntArray
  
  \ingroup DAL

  \author Joseph Masters

  \date 07-02-07

  The dalIntArray object holds an n-dimensional array of a single datatype.
*/

class dalArray{

	int rank;        /// number of dimensions
	string datatype; /// array datatype identifier
	herr_t status;   /// hdf5 return status
  protected:
	hid_t array_id;  /// hdf5 object id for array
	hid_t file_id;   /// hdf5 file_id
	string name;     /// name of the array

  public:
	void getAttributes();
// 	void printAttribute( string attrname );
// 	void * getAttribute( string attrname );

	void setAttribute_string( string attrname, string data );
	void setAttribute_int( string attrname, int * data/*, int size=1*/ );
// 	void setAttribute_uint( string attrname, unsigned int * data,int size=1 );
	void setAttribute_float( string attrname, float * data/*, int size=1*/ );
	void extend( vector<int> dims );

/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
#ifdef PYTHON
	void sai_boost( string attrname, int data );
	void saf_boost( string attrname, float data );
	void extend_boost( bpl::list pydims );
#endif
};

class dalIntArray: public dalArray {

  public:
	dalIntArray();
	dalIntArray( hid_t obj_id, string arrayname, vector<int> dims,
		     int data[] );
	dalIntArray( hid_t obj_id, string arrayname, vector<int> dims,
		     int data[], vector<int>chnkdims);
	int * readIntArray( hid_t obj_id, string arrayname );
// 	void write( vector<int> offset, vector<int> write_dims, int * data);
	~dalIntArray();
};

class dalFloatArray: public dalArray {

  public:
	dalFloatArray( hid_t obj_id, string arrayname, vector<int> dims,
			 float data[], vector<int>chnkdims);
	~dalFloatArray();
};

class dalComplexArray: public dalArray {
  
  public:
	dalComplexArray( void* voidfile, string arrayname, vector<int> dims,
			 /*vector< complex<float> >*/complex<float> data[], vector<int>chnkdims);
	~dalComplexArray();
};
#endif
