/*-------------------------------------------------------------------------*
 | $Id::                                                                $ |
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


namespace DAL {
  
  /*!
    \class dalData
    \ingroup DAL
    \ingroup core
    \brief Represents container of data.
    
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

  class dalData {
    
    //! Type of the data,  i.e. "dal_COMPLEX", "dal_INT", "dal_FLOAT"
    std::string dataType_p;
    //! i.e. "MSCASA", "FITS", "HDF5"
    std::string filetype_p;
    std::string array_order; // i.e. "fortran", "c"
    
    public:

      //! Pointer to the actual c-array data
      void * data;
      //! Used to convert one datatype to another
      void * data2;
      //! Get the shape of the data array
      std::vector<int> shape;
      //! Get the number of rows inside the array
      long nrows;

      // === Construction =======================================================

      //! Default constructor
      dalData();
      //! Argumented constructor with a specific file type.
      dalData (std::string lclfiletype,
	       std::string lcldatatype,
               std::vector<int> lclshape,
	       long lclnrows);

      // === Destruction ========================================================

      //! Destructor
      ~dalData() {
	if ( data )
	  free(data);
      }
      
      // === Parameter access ===================================================
      
      //! Get the type of data held by the object, i.e. dal_INT, dal_FLOAT, etc.
      inline std::string datatype() {
        return dataType_p;
      }
      
      //! Get the filetype, i.e. "MSCASA", "FITS", "HDF5"
      inline std::string filetype() {
        return filetype_p;
      }
      
      //! Get the axis ordering of the data array, i.e. "fortran", "c"
      inline std::string arrayOrder () {
        return array_order;
      }
      
      // === Methods ============================================================
      
      //! Get the fortran index value of up to a three-dimensional array.
      unsigned long fortran_index (long idx1,
				   long idx2,
				   long idx3);

      //! Get the C index value of up to a three-dimensional array
      unsigned long c_index (long idx1,
			     long idx2,
			     long idx3);
      
      //! Get the value of an individual data array element
      void * get (long idx1=-1,
		  long idx2=-1,
		  long idx3=-1);
      
#ifdef PYTHON
      bpl::numeric::array get_boost1();
      bpl::numeric::array get_boost2( int32_t length );
      bpl::numeric::array get_boost3( int64_t offset, int32_t length );
#endif

    };

} // DAL namespace

#endif // DALDATA_H
