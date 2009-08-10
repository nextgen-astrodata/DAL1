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

#ifndef DALBASETYPES_H
#include "dalBaseTypes.h"
#endif

namespace DAL {
  
  /*!
    \class dalArray
    \ingroup DAL
    \brief Represents an n-dimensional array.
    \author Joseph Masters
    
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
    hid_t array_id;  //!< hdf5 object id for array
    hid_t file_id;   //!< hdf5 file_id
    string name;     //!< name of the array
    
  public:

    //! Default constructor
    dalArray();
    //! Get the shap of the array
    vector<int> dims();
    std::string getName();
    int open( void * file, string arrayname );
    bool close();
    hid_t getId();
    int getRank ();
    bool getAttributes();
    
    // ---------------------------------------------------------- getAttribute
    
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
      bool getAttribute( std::string attrname, T &value )
      {
        return h5get_attribute( array_id, attrname, value );
      }

      bool setAttribute( std::string attrname, char * data, int size=1 );
      bool setAttribute( std::string attrname, short * data, int size=1 );
      bool setAttribute( std::string attrname, int * data, int size=1 );
      bool setAttribute( std::string attrname, uint * data, int size=1 );
      bool setAttribute( std::string attrname, long * data, int size=1 );
      bool setAttribute( std::string attrname, float * data, int size=1 );
      bool setAttribute( std::string attrname, double * data, int size=1 );
      bool setAttribute( std::string attrname, std::string data );
      bool setAttribute( std::string attrname, std::string * data, int size=1 );

      bool extend( vector<int> dims );
      bool write(int offset, short data[], int arraysize);
      bool write(int offset, int data[], int arraysize);
      bool write( int offset, complex<float> data[], int arraysize );
      bool write( int offset, complex<Int16> data[], int arraysize );

      /************************************************************************
       *
       * The following functions are boost wrappers to allow some previously
       *   defined functions to be easily called from a python prompt.
       *
       ************************************************************************/
#ifdef PYTHON

      void sai_boost( std::string attrname, int data );
      void saf_boost( std::string attrname, float data );
      void extend_boost( bpl::list pydims );

      dalArray * csa_boost_list( std::string arrayname, bpl::list dims, bpl::list data);
      dalArray * cia_boost_list( std::string arrayname, bpl::list dims, bpl::list data);
      dalArray * cfa_boost_list( std::string arrayname, bpl::list dims, bpl::list data );

      bpl::numeric::array ria_boost( std::string arrayname );

      bool setAttribute_char( std::string attrname, char data );
      bool setAttribute_short( std::string attrname, short data );
      bool setAttribute_int( std::string attrname, int data );
      bool setAttribute_uint( std::string attrname, uint data );
      bool setAttribute_long( std::string attrname, long data );
      bool setAttribute_float( std::string attrname, float data );
      bool setAttribute_double( std::string attrname, double data );
      bool setAttribute_string( std::string attrname, std::string data );
      bool setAttribute_char_vector( std::string attrname, bpl::list data );
      bool setAttribute_short_vector( std::string attrname, bpl::list data );
      bool setAttribute_int_vector( std::string attrname, bpl::list data );
      bool setAttribute_uint_vector( std::string attrname, bpl::list data );
      bool setAttribute_long_vector( std::string attrname, bpl::list data );
      bool setAttribute_float_vector( std::string attrname, bpl::list data );
      bool setAttribute_double_vector( std::string attrname, bpl::list data );
      bool setAttribute_string_vector( std::string attrname, bpl::list data );
      
#endif
    };

  class dalShortArray: public dalArray
    {

    public:
      dalShortArray( hid_t obj_id, std::string arrayname, std::vector<int> dims,
                     short data[], std::vector<int>chnkdims);
      short * readShortArray( hid_t obj_id, std::string arrayname );
    };

  class dalIntArray: public dalArray
    {

    public:
      dalIntArray( hid_t obj_id, string arrayname, vector<int> dims,
                   int data[], vector<int>chnkdims );
      int * readIntArray( hid_t obj_id, string arrayname );
    };

  class dalFloatArray: public dalArray
    {

    public:
      dalFloatArray( hid_t obj_id, string arrayname, vector<int> dims,
                     float data[], vector<int>chnkdims);
    };

  class dalComplexArray_float32: public dalArray
    {

    public:
      dalComplexArray_float32( hid_t objfile, string arrayname,
                               vector<int> dims, complex<float> data[],
                               vector<int>chnkdims);
    };

  class dalComplexArray_int16: public dalArray
    {

    public:
      dalComplexArray_int16( hid_t objfile, string arrayname,
                             vector<int> dims, complex<Int16> data[],
                             vector<int>chnkdims);
    };

} // end namespace DAL

#endif // end #ifndef DALARRAY_H
