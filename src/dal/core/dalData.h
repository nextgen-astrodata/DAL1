/***************************************************************************
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

#include <complex>
#include <core/dalCommon.h>
#include <core/dalObjectBase.h>

#ifdef PYTHON
#include <pydal/num_util.h>
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
  
  class dalData : public dalObjectBase {
    
    //! Type of the data,  i.e. "dal_COMPLEX", "dal_INT", "dal_FLOAT"
    std::string itsDatatype;
    //! Ordering of the array elements: "fortran", "c"
    std::string itsArrayOrder;
    
  public:
    
    //! Pointer to the actual c-array data
    void * data;
    //! Used to convert one datatype to another
    void * data2;
    //! Shape of the data array
    std::vector<int> itsShape;
    //! Number of rows inside the array.
    long itsNofRows;
    
    // === Construction =======================================================
    
    //! Default constructor
    dalData();
    
    //! Argumented constructor with a specific file type.
    dalData (dalFileType::Type const &filetype,
	     std::string const &datatype,
	     std::vector<int> const &shape,
	     long const &nofRows);
    
    //! Argumented constructor with a specific file type.
    dalData (dalFileType const &filetype,
	     std::string const &datatype,
	     std::vector<int> const &shape,
	     long const &nofRows);
    
    // === Destruction ========================================================
    
    //! Destructor
    ~dalData() {
      if ( data )
	free(data);
    }
    
    // === Parameter access ===================================================
    
    //! Get the type of data held by the object, i.e. dal_INT, dal_FLOAT, etc.
    inline std::string datatype() {
      return itsDatatype;
    }
    
    //! Get the axis ordering of the data array, i.e. "fortran", "c"
    inline std::string arrayOrder () {
      return itsArrayOrder;
    }
    
    // === Public methods =====================================================
    
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
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    
    //! Provide a summary of the internal status
    void summary (std::ostream &os);
    
    // === Python bindings ======================================================
    
#ifdef PYTHON
    boost::python::numeric::array get_boost1 ();
    boost::python::numeric::array get_boost2 (int32_t length );
    boost::python::numeric::array get_boost3 (int64_t offset, int32_t length );
#endif
    
    // === Private methods ====================================================
    
    //! Initialize internal variables
    void init ();
    
  };
  
} // DAL namespace

#endif // DALDATA_H
