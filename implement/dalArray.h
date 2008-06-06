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

#ifndef DALARRAY_H
#define DALARRAY_H

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALATTRIBUTE_H
#include "dalAttribute.h"
#endif

namespace DAL {

/*!
  \class dalArray
  
  \ingroup DAL

  \brief Represents an n-dimensional array.

  \author Joseph Masters

  The dalArray object holds an n-dimensional array of a single datatype.
*/

class dalArray{

	int rank;        //!< number of dimensions
	string datatype; //!< array datatype identifier
	herr_t status;   //!< hdf5 return status
	
  protected:
	hid_t array_id;  //!< hdf5 object id for array
	hid_t file_id;   //!< hdf5 file_id
	string name;     //!< name of the array

  public:

	/*!
          \brief Default constructor.

          Default constructor.
         */
	dalArray();

	vector<int> dims();

        std::string getName();

	/*!
          \brief Open an existing array.

          Open an existing array.

          \param file A pointer to the file.
          \param arrayname The name of the array you want to open.

          \return An identifier for the array.
         */
	int open( void * file, string arrayname );

	/*!
          \brief Close an existing array.

          Close an existing array.

         */
	int close();

        /*!
	  \brief Print the attributes of the array.
	  
	  Print the attributes of the array.
	 */
	void getAttributes();
	
        /*!
          \brief Get the value of an attribute.

          Get the value of an attribute.

          \param attrname The name of the attribute you want to retrieve.

          \return void * A pointer to the data in the attribute. 
        */
	void * getAttribute( string attrname );

	/*!
	  \brief Add a string attribute.
	  
	  Add a string attribute to the array.
	  
	  \param attrname The name of the attribute you want to add.
	  \param data The value of the attribute you want to add.
	 */
	void setAttribute_string( string attrname, string data );
	void setAttribute_string( string attrname, string * data, int size=1 );

	/*!
	  \brief Add an integer attribute.
	  
	  Add an integer attribute to the array.
	  
	  \param attrname The name of the attribute you want to add.
	  \param data The value of the attribute you want to add.
	 */
	void setAttribute_int( string attrname, int * data );

	/*!
	  \brief Add an unsigned integer attribute.
	  
	  Add an unsigned integer attribute to the array.
	  
	  \param attrname The name of the attribute you want to add.
	  \param data The value of the attribute you want to add.
	 */
	void setAttribute_uint( string attrname, unsigned int * data );

	/*!
	  \brief Add a floating point attribute.
	  
	  Add a floating point attribute to the array.
	  
	  \param attrname The name of the attribute you want to add.
	  \param data The value of the attribute you want to add.
	 */
	void setAttribute_float( string attrname, float * data );

	/*!
	  \brief Add a double-precision floating point attribute.
	  
	  Add a double-precision floating point attribute to the array.
	  
	  \param attrname The name of the attribute you want to add.
	  \param data The value of the attribute you want to add.
	 */
// 	void setAttribute_double( string attrname, double * data );
        void setAttribute_double( string attrname, double * data, int size );

	/*!
	  \brief Extend an array.
	  
	  Increase the dimensions of the array.
	  
	  \param dims The new desired dimensions of the array.  The extend method
	              should normally be followed by a write.
	 */
	void extend( vector<int> dims );
	
	/*!
	  \brief Write short data to an array.
	  
	  Write data to an array, usually after extending it's dimensions.
	 */
	void write(int offset, short data[], int arraysize);

	/*!
	  \brief Write int to an array.
	  
	  Write data to an array, usually after extending it's dimensions.
	 */
	void write(int offset, int data[], int arraysize);

	/*!
	  \brief Write complex to an array.
	  
	  Write data to an array, usually after extending it's dimensions.
	 */
	void write( int offset, complex<float> data[], int arraysize );
        void write( int offset, complex<Int16> data[], int arraysize );

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

class dalShortArray: public dalArray {

  public:
    /*!
	  \brief Default destructor.
	  
	  Default destructor.
	 */
	~dalShortArray();
	
	/*!
	  \brief Constructor for a fixed-size integer array.
	  
	  Constructor for a fixed-size integer array.  This is usually called from
	  the dataset object and not directly by the developer.
	  
	  \param obj_id An identifier for the dataset object.
	  \param arrayname The name of the array you want to create.
	  \param dims The dimensions of the array you want to create.
	  \param data A structure containing the data you want to write to the
	              array.  The size of the structure should match the dimensions
				  of the array.
	 */
	dalShortArray( hid_t obj_id, string arrayname, vector<int> dims,
		     short data[] );

	/*!
	  \brief Constructor for extendible integer array.
	  
	  Constructor for an extendible integer array.  This is usually called from
	  the dataset object and not directly by the developer.
	  
	  \param obj_id An identifier for the dataset object.
	  \param arrayname The name of the array you want to create.
	  \param dims The dimensions of the array you want to create.
	  \param data A structure containing the data you want to write to the
	              array.  The size of the structure should match the dimensions
				  of the array.
	  \param chnkdims Specifies the chunk size for extendible arrays.
	 */
	dalShortArray( hid_t obj_id, string arrayname, vector<int> dims,
		     short data[], vector<int>chnkdims);

    /*!
	  \brief Read an integer array.
	  
	  Read the values from an array of integers.  This is usually called from
	  the dataset object and not directly by the developer.
	  
	  \param obj_id An identifier for the dataset object.
	  \param arrayname The name of the array you want to read.
	 */
	short * readShortArray( hid_t obj_id, string arrayname );
};

class dalIntArray: public dalArray {

  public: 
    /*!
	  \brief Default destructor.
	  
	  Default destructor.
	 */
	~dalIntArray();
	
	/*!
	  \brief Constructor for a fixed-size integer array.
	  
	  Constructor for a fixed-size integer array.  This is usually called from
	  the dataset object and not directly by the developer.
	  
	  \param obj_id An identifier for the dataset object.
	  \param arrayname The name of the array you want to create.
	  \param dims The dimensions of the array you want to create.
	  \param data A structure containing the data you want to write to the
	              array.  The size of the structure should match the dimensions
				  of the array.
	 */
	dalIntArray( hid_t obj_id, string arrayname, vector<int> dims,
		     int data[] );

	/*!
	  \brief Constructor for extendible integer array.
	  
	  Constructor for an extendible integer array.  This is usually called from
	  the dataset object and not directly by the developer.
	  
	  \param obj_id An identifier for the dataset object.
	  \param arrayname The name of the array you want to create.
	  \param dims The dimensions of the array you want to create.
	  \param data A structure containing the data you want to write to the
	              array.  The size of the structure should match the dimensions
				  of the array.
	  \param chnkdims Specifies the chunk size for extendible arrays.
	 */
	dalIntArray( hid_t obj_id, string arrayname, vector<int> dims,
		     int data[], vector<int>chnkdims);

    /*!
	  \brief Read an integer array.
	  
	  Read the values from an array of integers.  This is usually called from
	  the dataset object and not directly by the developer.
	  
	  \param obj_id An identifier for the dataset object.
	  \param arrayname The name of the array you want to read.
	 */
	int * readIntArray( hid_t obj_id, string arrayname );
};

class dalFloatArray: public dalArray {

  public:
    /*!
	  \brief Default destructor.
	  
	  Default destructor.
	 */
	~dalFloatArray();

	/*!
	  \brief Constructor for extendible floating point array.
	  
	  Constructor for an extendible floating point array.  This is usually
	  called from the dataset object and not directly by the developer.
	  
	  \param obj_id An identifier for the dataset object.
	  \param arrayname The name of the array you want to create.
	  \param dims The dimensions of the array you want to create.
	  \param data A structure containing the data you want to write to the
	              array.  The size of the structure should match the dimensions
				  of the array.
	  \param chnkdims Specifies the chunk size for extendible arrays.
	 */
	dalFloatArray( hid_t obj_id, string arrayname, vector<int> dims,
			 float data[], vector<int>chnkdims);
};

class dalComplexArray_float32: public dalArray {
  
  public:
    /*!
	  \brief Default destructor.
	  
	  Default destructor.
	 */
	~dalComplexArray_float32();

	/*!
	  \brief Constructor for extendible complex floating point array.
	  
	  Constructor for an extendible complex floating point array.  This is
	  usually called from the dataset object and not directly by the developer.
	  
	  \param voidfile An pointer to the file.
	  \param arrayname The name of the array you want to create.
	  \param dims The dimensions of the array you want to create.
	  \param data A structure containing the data you want to write to the
	              array.  The size of the structure should match the dimensions
				  of the array.
	  \param chnkdims Specifies the chunk size for extendible arrays.
	 */
	dalComplexArray_float32( hid_t objfile, string arrayname,
                                 vector<int> dims, complex<float> data[],
                                 vector<int>chnkdims);
};

class dalComplexArray_int16: public dalArray {

  public:
    /*!
	  \brief Default destructor.
	  
	  Default destructor.
	 */
	~dalComplexArray_int16();

	/*!
	  \brief Constructor for extendible complex floating point array.
	  
	  Constructor for an extendible complex floating point array.  This is
	  usually called from the dataset object and not directly by the developer.
	  
	  \param voidfile An pointer to the file.
	  \param arrayname The name of the array you want to create.
	  \param dims The dimensions of the array you want to create.
	  \param data A structure containing the data you want to write to the
	              array.  The size of the structure should match the dimensions
				  of the array.
	  \param chnkdims Specifies the chunk size for extendible arrays.
	 */
	dalComplexArray_int16( hid_t objfile, string arrayname,
                               vector<int> dims, complex<Int16> data[],
                               vector<int>chnkdims);
};

} // end namespace DAL

#endif // end #ifndef DALARRAY_H
