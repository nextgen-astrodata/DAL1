/*-------------------------------------------------------------------------*
 | $Id:: dalData.h 660 2007-08-30 16:55:31Z masters                     $ |
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

/*!
  \file dalData.h
  
  \ingroup DAL

  \author Joseph Masters

  \date 09-05-07
*/

#ifndef DALDATA_H
#define DALDATA_H

#ifndef DAL_H
#include "dal.h"
#endif


/*!
The dalData object represents an n-dimensional array of data regardless of
the underlying format.  In the case of CASA measurement sets, the data
is stored in row-major (FORTRAN) order.  For HDF5, and probably FITS, the
data is stored in column-major (C) order.  The dalData object abstracts this
away from the user and developer.

There will also be a way for the developer to get access to the c-array,
exactly as it is stored.
*/
class dalData{
	string datatype;  /// i.e. "dal_COMPLEX", "dal_INT", "dal_FLOAT"
	string filetype;  /// i.e. "MSCASA", "FITS", "HDF5"

public:
	//void * data;  /// pointer to the actual c-array data
// 	complex<float> * data;
	void * data;
	vector<int> shape;

	dalData();  /// default constructor
	dalData(string, string, vector<int>);  /// constructor with a specific file type
// 	void * get(long);
// 	void * get(long, long);
	void * get(long idx1=-1, long idx=-1, long idx3=-1);
	void setData(void *);
#ifdef PYTHON
//    bpl::numeric:array get_boost();
#endif
};
#endif
