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

#ifndef DALDATA_H
#define DALDATA_H

#ifndef DAL_H
#include "dal.h"
#endif


namespace DAL {

/*!
  \class dalData

  \brief Represents container of data.

  \ingroup DAL

  \author Joseph Masters

  <h3>Synopsis</h3>
  The dalData object represents an n-dimensional array of data regardless of
  the underlying format.  In the case of CASA measurement sets, the data
  is stored in row-major (FORTRAN) order.  For HDF5, and probably FITS, the
  data is stored in column-major (C) order.  The dalData object abstracts this
  away from the user and developer.  The data object might hold data for a
  single table column, or perhaps another data structure.

  There will also be a way for the developer to get access to the c-array,
  exactly as it is stored.
*/
class dalData{
	string datatype;  //!< i.e. "dal_COMPLEX", "dal_INT", "dal_FLOAT"
	string filetype;  //!< i.e. "MSCASA", "FITS", "HDF5"
        string array_order; //!< i.e. "fortran", "c"

public:

	void * data;  // pointer to the actual c-array data
	void * data2;  // used to convert one datatype to another
	vector<int> shape;
	long nrows;
	string get_datatype();

	/*!
          \brief Default constructor.

          Defulat data object constructor.
         */
        dalData();

	/*!
          \brief Default destructor.

          Defulat data object destructor.
         */
        ~dalData();

	/*!
          \brief Constructor with a specific file type.

          Constructor with a specific file type.

          \param lclfiletype The file type (i.e. "MSCASA", "HDF5", etc.)
          \param lcldatatype The data type this instance of the class will
                             contain.
          \param lclshape The shape of the data object.
          \param lclnrows The number of rows in the data object.
         */
        dalData(string lclfiletype, string lcldatatype,
                vector<int> lclshape, long lclnrows);

        /*!
          \brief Get the fortran index value of a three-dimensional array.

          This is a helper function that is usually called by the dataset
          object and not by the developer.  Its purpose is to find a single
          index value within a multi-dimensional array, but that index depends
          on the storage method of the underlying file.  CASA arrays are stored
          in FORTAN order.  HDF5 arrays are stored in C order.

          \param idx1 Specifies the first index.
          \param idx2 Specifies the second index.
          \param idx3 Specifies the third index.

          \return A single value index for the multi-dimensional array.
         */
	unsigned long fortran_index(long idx1, long idx2, long idx3);

        /*!
          \brief Get the C index value of a three-dimensional array.

          This is a helper function that is usually called by the dataset
          object and not by the developer.  Its purpose is to find a single
          index value within a multi-dimensional array, but that index depends
          on the storage method of the underlying file.  CASA arrays are stored
          in FORTAN order.  HDF5 arrays are stored in C order.

          \param idx1 Specifies the first index.
          \param idx2 Specifies the second index.
          \param idx3 Specifies the third index.

          \return A single value index for the multi-dimensional array.
         */
	unsigned long c_index(long idx1, long idx2, long idx3);

        /*!
          \brief Get the data.

          Retrieve the data out of the object.

          \param idx1 Optional parameter specifying the first index.
          \param idx2 Optional parameter specifying the second index.
          \param idx3 Optional parameter specifying the third index.

          \return void * Pointer to an arbitrary data structure.
         */
	void * get(long idx1=-1, long idx2=-1, long idx3=-1);

         /*!
          \brief Converts array values to floating point values.

          Converts array values to floating point values.
         */
	void toFloat();

         /*!
          \brief Get file type ("MSCASA", "FITS", "HDF5", etc.)

          Get file type ("MSCASA", "FITS", "HDF5", etc.)

	  \return string -- filetype
         */
	string getFileType();
	
#ifdef PYTHON
	bpl::numeric::array get_boost();
#endif
};

} // DAL namespace

#endif // DALDATA_H
