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

#ifndef DALBASETYPES_H
#include "dalBaseTypes.h"
#endif


namespace DAL
  {

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

  class dalData
    {
      std::string dtype;  // i.e. "dal_COMPLEX", "dal_INT", "dal_FLOAT"
      std::string filetype;  // i.e. "MSCASA", "FITS", "HDF5"
      std::string array_order; // i.e. "fortran", "c"

    public:

      void * data;  // pointer to the actual c-array data
      void * data2;  // used to convert one datatype to another
      std::vector<int> shape;
      long nrows;

      dalData();
      dalData( std::string lclfiletype,  std::string lcldatatype,
               std::vector<int> lclshape, long lclnrows);
      ~dalData();

      unsigned long fortran_index(long idx1, long idx2, long idx3);
      unsigned long c_index(long idx1, long idx2, long idx3);

      /*!
        \brief Get datatype.

        Get the type of data held by dalData object.

        \return string -- i.e. dal_INT, dal_FLOAT, etc.
       */
      inline std::string datatype()
      {
        return dtype;
      }
      void * get(long idx1=-1, long idx2=-1, long idx3=-1);

#ifdef PYTHON
      bpl::numeric::array get_boost1();
      bpl::numeric::array get_boost2( int32_t length );
      bpl::numeric::array get_boost3( int64_t offset, int32_t length );

#endif
    };

} // DAL namespace

#endif // DALDATA_H
