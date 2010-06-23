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

#ifndef DALARRAY_H
#define DALARRAY_H

#include "dalBaseTypes.h"

namespace DAL {
  
  /*!
    \class dalArray

    \ingroup DAL
    \ingroup core

    \brief Represents an n-dimensional array.

    \author Joseph Masters, Lars B&auml;hren
    
    The dalArray object holds an n-dimensional array of a single datatype.
  */
  
  class dalArray {
    
    //! Number of dimensions
    int rank_p;
    //! Array datatype identifier
    string datatype;
    //! HDF5 return status
    herr_t status;
    
  protected:
    
    //! HDF5 object ID for array
    hid_t datasetID_p;
    //! HDF5 object ID for file
    hid_t fileID_p;
    //! Name of the array
    string name;
    
  public:

    // === Construction =========================================================

    //! Default constructor
    dalArray();

    // === Parameter access =====================================================

    //! Retrieve the dimensions of an array
    vector<int> dims();
    //! Retrieve the name of an array
    inline std::string getName() {
      return name;
    }
    //! Open an existing array.
    int open (void * file,
	      string arrayname);
    //! Close an existing array.
    bool close();
    //! Get the array ID.
    inline hid_t getId() {
      return datasetID_p;
    }
    //! Get the rank of the array, i.e. the number of dimensions
    inline int getRank () {
      return rank_p;
    }
    
    bool getAttributes();

    // === Methods ==============================================================

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    
    //! Provide a summary of the internal status
    void summary (std::ostream &os);
    
    /*!
      \brief Get the value of an attribute.
      
      Get the value of an attribute.  This is different from printAttribute
      because the value of the attribute is returned into a structure
      instead of simply printing.
      
      \param attrname The name of the attribute you want to retrieve.
      \param value Attribute value to return.
      \return bool -- DAL::FAIL or DAL::SUCCESS
    */
    template <class T>
      bool getAttribute( std::string attrname,
			 T &value )
      {
        return h5get_attribute( datasetID_p, attrname, value );
      }
    
    //! Set attribute of \e char type
    bool setAttribute( std::string attrname, char * data, int size=1 );
    //! Set attribute of \e short type
    bool setAttribute( std::string attrname, short * data, int size=1 );
    //! Set attribute of \e int type
    bool setAttribute( std::string attrname, int * data, int size=1 );
    //! Set attribute of \e uint type
    bool setAttribute( std::string attrname, uint * data, int size=1 );
    //! Set attribute of \e long type
    bool setAttribute( std::string attrname, long * data, int size=1 );
    //! Set attribute of \e float type
    bool setAttribute( std::string attrname, float * data, int size=1 );
    //! Set attribute of \e double type
    bool setAttribute( std::string attrname, double * data, int size=1 );
    //! Set attribute of \e string type
    bool setAttribute( std::string attrname, std::string data );
    //! Set attribute of \e string array type
    bool setAttribute( std::string attrname, std::string * data, int size=1 );
    //! Increase the dimensions of the array.
    bool extend (vector<int> dims);
    //! Write \e data of type \e short.
    bool write(int offset, short data[], int arraysize);
    //! Write \e data of type \e int.
    bool write(int offset, int data[], int arraysize);
    //! Write \e data of type \e complex<float>.
    bool write( int offset, std::complex<float> data[], int arraysize );
    //! Write \e data of type \e complex<Int16>.
    bool write( int offset, std::complex<Int16> data[], int arraysize );
    
    // === Python wrapper functions =============================================

#ifdef PYTHON
    
    void sai_boost( std::string attrname, int data );
    void saf_boost( std::string attrname, float data );
    void extend_boost( bpl::list pydims );
    
    dalArray * csa_boost_list( std::string arrayname, bpl::list dims, bpl::list data);
    dalArray * cia_boost_list( std::string arrayname, bpl::list dims, bpl::list data);
    dalArray * cfa_boost_list( std::string arrayname, bpl::list dims, bpl::list data );
    
    bpl::numeric::array ria_boost( std::string arrayname );
    
    //! Set attribute of type \e char
    bool setAttribute_char (std::string attrname, char data);
    //! Set attribute of type \e short
    bool setAttribute_short( std::string attrname, short data );
    //! Set attribute of type \e int
    bool setAttribute_int( std::string attrname, int data );
    //! Set attribute of type \e uint
    bool setAttribute_uint( std::string attrname, uint data );
    //! Set attribute of type \e long
    bool setAttribute_long( std::string attrname, long data );
    //! Set attribute of type \e float
    bool setAttribute_float( std::string attrname, float data );
    //! Set attribute of type \e double
    bool setAttribute_double( std::string attrname, double data );
    //! Set attribute of type \e string
    bool setAttribute_string( std::string attrname, std::string data );
    bool setAttribute_char_vector( std::string attrname, bpl::list data );
    bool setAttribute_short_vector( std::string attrname, bpl::list data );
    //! Set attribute of type \e vector<int>
    bool setAttribute_int_vector( std::string attrname, bpl::list data );
    //! Set attribute of type \e vector<uint>
    bool setAttribute_uint_vector( std::string attrname, bpl::list data );
    //! Set attribute of type \e vector<long>
    bool setAttribute_long_vector( std::string attrname, bpl::list data );
    //! Set attribute of type \e vector<float>
    bool setAttribute_float_vector( std::string attrname, bpl::list data );
    //! Set attribute of type \e vector<double>
    bool setAttribute_double_vector( std::string attrname, bpl::list data );
    //! Set attribute of type \e vector<string>
    bool setAttribute_string_vector( std::string attrname, bpl::list data );
    
#endif
  };
  
  /*!
    \class dalIntArray
    \ingroup DAL
    \ingroup  core
    \brief Represents an n-dimensional array of type \e int
  */
  class dalIntArray: public dalArray
  {
    
  public:
    dalIntArray (hid_t obj_id,
		 string arrayname,
		 vector<int> dims,
		 int data[],
		 vector<int>chnkdims );
    int * readIntArray (hid_t obj_id,
			string arrayname);
  };
  
  /*!
    \class dalFloatArray
    \ingroup DAL
    \ingroup  core
    \brief Represents an n-dimensional array of type \e float
  */
  class dalFloatArray: public dalArray
  {
    
  public:
    dalFloatArray (hid_t obj_id,
		   string arrayname,
		   vector<int> dims,
		   float data[],
		   vector<int>chnkdims);
  };
  
  class dalComplexArray_float32: public dalArray
  {
    
  public:
    dalComplexArray_float32( hid_t objfile,
			     string arrayname,
			     vector<int> dims,
			     std::complex<float> data[],
			     vector<int>chnkdims);
  };
  
  class dalComplexArray_int16: public dalArray
  {
    
  public:
    dalComplexArray_int16( hid_t objfile, string arrayname,
			   vector<int> dims,
			   std::complex<Int16> data[],
			   vector<int>chnkdims);
  };
  
} // end namespace DAL

#endif // end #ifndef DALARRAY_H
