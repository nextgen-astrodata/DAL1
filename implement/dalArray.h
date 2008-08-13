/*-------------------------------------------------------------------------*
 | $Id:: dalDataset.h 389 2007-06-13 08:47:09Z masters                   $ |
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

namespace DAL
  {

  /*!
    \class dalArray
    \ingroup DAL
    \brief Represents an n-dimensional array.
    \author Joseph Masters

    The dalArray object holds an n-dimensional array of a single datatype.
  */

  class dalArray
    {

      int rank;        // number of dimensions
      string datatype; // array datatype identifier
      herr_t status;   // hdf5 return status

    protected:
      hid_t array_id;  //!< hdf5 object id for array
      hid_t file_id;   //!< hdf5 file_id
      string name;     //!< name of the array

    public:

      dalArray();
      vector<int> dims();
      std::string getName();
      int open( void * file, string arrayname );
      bool close();
      bool getAttributes();
      void * getAttribute( string attrname );
      bool setAttribute_string( string attrname, string data );
      bool setAttribute_string( string attrname, string * data, int size=1 );
      bool setAttribute_int( string attrname, int * data );
      bool setAttribute_uint( string attrname, unsigned int * data );
      bool setAttribute_float( string attrname, float * data );
      bool setAttribute_double( string attrname, double * data, int size );
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

      void sai_boost( string attrname, int data );
      void saf_boost( string attrname, float data );
      void extend_boost( bpl::list pydims );

#endif
    };

  class dalShortArray: public dalArray
    {

    public:
      dalShortArray( hid_t obj_id, string arrayname, vector<int> dims,
                     short data[], vector<int>chnkdims);
      short * readShortArray( hid_t obj_id, string arrayname );
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
