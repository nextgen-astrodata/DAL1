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
#include "dalArray.h"
#endif

namespace DAL
  {

// ------------------------------------------------------------ dalArray
  /*!
    \brief Default constructor.

    Default constructor.
   */
  dalArray::dalArray()
  {
    array_id = 0;
    file_id = 0;
    rank = 0;
    datatype = "UNKNOWN";
    status = 0;
    name = "UNKNOWN";
  }


// ------------------------------------------------------------ open
  /*!
   \brief Open an existing array.
   \param file A pointer to the file.
   \param arrayname The name of the array you want to open.
   \return An identifier for the array.

   Open an existing array.
  */
  int dalArray::open( void * voidfile, string arrayname )
  {
    name = arrayname;
    hid_t * lclfile = (hid_t*)voidfile; // H5File object
    file_id = *lclfile;  // get the file handle

    if ( ( array_id = H5Dopen( file_id, name.c_str(), H5P_DEFAULT ) ) < 0 )
      std::cerr << "ERROR: could not open array '" << arrayname << "'.\n";

    return( array_id );
  }

// ------------------------------------------------------------ getId

  /*!
    \brief Get the array ID.

    Retrieve the identifier for the array.

    \return The array identifier as an integer.
  */
  hid_t dalArray::getId()
  {
    return array_id;
  }

// ------------------------------------------------------------ close
  /*!
   \brief Close an existing array.

   Close an existing array.

   \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalArray::close()
  {
    if ( H5Dclose(array_id) < 0 )
      {
        std::cerr << "ERROR: dalArray::close() failed.\n";
        return DAL::FAIL;
      }

    return DAL::SUCCESS;
  }


// ------------------------------------------------------------ write (int)
  /*!
    \brief Write int to an array.

    Write data to an array, usually after extending it's dimensions.

   \param offset Position to begin writing array.
   \param data Data array to write.
   \param arraysize Size of the array to write.
   \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalArray::write( int offset, int data[], int arraysize )
  {
    hsize_t      dims[1] = { arraysize };
    int32_t      rank    = 1;
    hsize_t      off[1]  = { offset };
    hid_t filespace      = 0;
    hid_t dataspace      = 0;

    /* Select a hyperslab  */
    if ( ( filespace = H5Dget_space( array_id ) ) < 0 )
      {
        std::cerr << "ERROR: Could not get filespace for array.\n";
        return DAL::FAIL;
      }

    if ( H5Sselect_hyperslab( filespace, H5S_SELECT_SET, off, NULL,
                              dims, NULL) < 0 )
      {
        std::cerr << "ERROR: Could not select hyperslab for array.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }
    
    /* Define memory space */
    if ( ( dataspace = H5Screate_simple (rank, dims, NULL) ) < 0 )
      {
        std::cerr << "ERROR: Could not create dataspace for array.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }
    
    /* Write the data to the hyperslab  */
    if ( H5Dwrite( array_id, H5T_NATIVE_INT, dataspace, filespace,
                   H5P_DEFAULT, data ) < 0 )
      {
        std::cerr << "ERROR: Could not write integer array.\n";
	H5Sclose(dataspace);
	H5Sclose(filespace);
        return DAL::FAIL;
      }
    
    /* Close the memory- and data-space */
    H5Sclose(dataspace);
    H5Sclose(filespace);

    return DAL::SUCCESS;
  }


// ------------------------------------------------------------ write (short)
  /*!
    \brief Write short data to an array.

    Write data to an array, usually after extending it's dimensions.

   \param offset Position to begin writing array.
   \param data Data array to write.
   \param arraysize Size of the array to write.
   \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalArray::write( int offset, short data[], int arraysize )
  {
    hsize_t      dims[1] = { arraysize };
    int32_t      rank    = 1;
    hsize_t      off[1]  = { offset };
    hid_t filespace      = 0;
    hid_t dataspace      = 0;

    /* Select a hyperslab  */
    if ( ( filespace = H5Dget_space( array_id ) ) < 0 )
      {
        std::cerr << "ERROR: Could not get filespace for array.\n";
        return DAL::FAIL;
      }
    
    if ( H5Sselect_hyperslab( filespace, H5S_SELECT_SET, off, NULL,
                              dims, NULL ) < 0 )
      {
        std::cerr << "ERROR: Could not select hyperslab for array.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }
    
    /* Define memory space */
    if ( ( dataspace = H5Screate_simple (rank, dims, NULL) ) < 0 )
      {
        std::cerr << "ERROR: Could not create dataspace for array.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }
    
    /* Write the data to the hyperslab  */
    if ( H5Dwrite (array_id, H5T_NATIVE_SHORT, dataspace, filespace,
                   H5P_DEFAULT, data ) < 0 )
      {
        std::cerr << "ERROR: Could not write short array.\n";
	H5Sclose(dataspace);
	H5Sclose(filespace);
        return DAL::FAIL;
      }
    
    /* Close the memory- and data-space */
    H5Sclose(dataspace);
    H5Sclose(filespace);
    
    return DAL::SUCCESS;
  }


// --------------------------------------------------- write (complex<float>)
  /*!
    \brief Write complex to an array.

    Write data to an array, usually after extending it's dimensions.

   \param offset Position to begin writing array.
   \param data Data array to write.
   \param arraysize Size of the array to write.
   \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalArray::write( int offset, complex<float> data[], int arraysize )
  {
    hsize_t      dims[1] = { arraysize };
    hsize_t      off[1]  = { offset };
    hid_t filespace      = 0;
    hid_t complex_id     = 0;

    /* Select a hyperslab  */
    if ( ( filespace = H5Dget_space( array_id ) ) < 0 )
      {
        std::cerr << "ERROR: Could not get filespace for array.\n";
        return DAL::FAIL;
      }

    if ( H5Sselect_hyperslab( filespace, H5S_SELECT_SET, off, NULL,
                              dims, NULL) < 0 )
      {
        std::cerr << "ERROR: Could not select hyperslab for array.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    /* Define memory space */
    typedef struct
      {
        double re;   /*real part*/
        double im;   /*imaginary part*/
      } complex_t;

    if ( ( complex_id = H5Tcreate (H5T_COMPOUND, sizeof(complex_t)) ) < 0 )
      {
        std::cerr << "ERROR: Could not create complex datatype.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    if ( H5Tinsert( complex_id, "real", HOFFSET(complex_t,re),
                    H5T_NATIVE_DOUBLE ) < 0 )
      {
        std::cerr << "ERROR: Could not insert element into compound datatype.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    if ( H5Tinsert( complex_id, "imaginary", HOFFSET(complex_t,im),
                    H5T_NATIVE_DOUBLE ) < 0 )
      {
        std::cerr << "ERROR: Could not insert element into compound datatype.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    /* Write the data to the hyperslab  */
    if ( H5Dwrite( array_id, complex_id, filespace, filespace,
                   H5P_DEFAULT, data ) < 0 )
      {
        std::cerr << "ERROR: Could not write complex<float> array.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    H5Sclose(filespace);
    return DAL::SUCCESS;
  }


// -------------------------------------------------- write (complex<Int16>)
  /*!
    \brief Write complex to an array.

    Write data to an array, usually after extending it's dimensions.

   \param offset Position to begin writing array.
   \param data Data array to write.
   \param arraysize Size of the array to write.
   \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalArray::write( int offset, complex<Int16> data[], int arraysize )
  {
    hsize_t      dims[1] = { arraysize };
    hsize_t      off[1]  = { offset };
    hid_t filespace      = 0;
    hid_t complex_id     = 0;

    if ( ( filespace = H5Dget_space( array_id ) ) < 0 )
      {
        std::cerr << "ERROR: Could not get filespace for array.\n";
        return DAL::FAIL;
      }

    /* Select a hyperslab  */
    if ( H5Sselect_hyperslab( filespace, H5S_SELECT_SET, off, NULL,
                              dims, NULL ) < 0 )
      {
        std::cerr << "ERROR: Could not select hyperslab for array.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    /* Define memory space */
    typedef struct
      {
        Int16 re;   /*real part*/
        Int16 im;   /*imaginary part*/
      } complex_t;

    if ( ( complex_id = H5Tcreate (H5T_COMPOUND, sizeof(complex_t)) ) < 0 )
      {
        std::cerr << "ERROR: Could not create complex datatype.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    if ( H5Tinsert ( complex_id, "real", HOFFSET(complex_t,re),
                     H5T_NATIVE_SHORT ) < 0 )
      {
        std::cerr << "ERROR: Could not insert element into compound datatype.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    if ( H5Tinsert ( complex_id, "imaginary", HOFFSET(complex_t,im),
                     H5T_NATIVE_SHORT ) < 0 )
      {
        std::cerr << "ERROR: Could not insert element into compound datatype.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    /* Write the data to the hyperslab  */
    if ( H5Dwrite( array_id, complex_id, filespace, filespace,
                   H5P_DEFAULT, data ) < 0 )
      {
        std::cerr << "ERROR: Could not write complex<Int16> array.\n";
	H5Sclose(filespace);
        return DAL::FAIL;
      }

    H5Sclose(filespace);

    return DAL::SUCCESS;
  }

// ------------------------------------------------------------ dims
  /*!
    \brief Retrieve the dimensions of an array
    \return The dimensions of the array
   */
  vector<int> dalArray::dims()
  {
    vector<int> return_values;
    hid_t dataspace = H5Dget_space( array_id );    /* dataspace identifier */
    int32_t rank        = H5Sget_simple_extent_ndims(dataspace);
    hsize_t  dims_out[rank];

    if ( H5Sget_simple_extent_dims(dataspace, dims_out, NULL) < 0 )
      std::cerr << "ERROR: Could not get array dimensions.\n";

    for (int32_t ii=0; ii<rank; ii++)
      return_values.push_back( dims_out[ii] );
    
    H5Sclose(dataspace);
    return return_values;
  }

// ------------------------------------------------------------ getName
  /*!
    \brief Retrieve the name of an array
    \return The name of the array
   */
  std::string dalArray::getName()
  {
    return name;
  }


// ------------------------------------------------------------ extend
  /*!
    \brief Extend an array.

    Increase the dimensions of the array.

    \param dims The new desired dimensions of the array.  The extend method
                should normally be followed by a write.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalArray::extend( vector<int> newdims )
  {
    uint32_t rank = newdims.size();
    hsize_t lcldims[ rank ];

    for ( uint32_t ii=0; ii < rank; ii++ )
      lcldims[ ii ] = newdims[ ii ];

    if ( H5Dextend( array_id, lcldims ) )
      {
        std::cerr << "ERROR: Could not extend array dimensions.\n";
        return DAL::FAIL;
      }

    return DAL::SUCCESS;

  }


// ------------------------------------------------------------ getAttributes
  /*!
   \brief Print the attributes of the array.

    Print the attributes of the array.

   \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalArray::getAttributes()
  {

    /*
       The following function calls the attr_info function for each
       attribute associated with the object.
     */
    if ( H5Aiterate1( array_id, NULL, attr_info, NULL ) < 0 )
      {
        std::cerr << "ERROR: Could not iterate over array attributes.\n";
        return DAL::FAIL;
      }

    return DAL::SUCCESS;
  }


  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a char attribute.

    Add a char attribute to the array.

    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalArray::setAttribute( std::string attrname, char * data, int size )
  {
    return h5set_attribute( H5T_NATIVE_CHAR, array_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a short integer attribute.

    Add a short integer attribute to the array.

    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalArray::setAttribute( std::string attrname, short * data, int size )
  {
    return h5set_attribute( H5T_NATIVE_SHORT, array_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add an integer attribute.

    Add an integer attribute to the array.

    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalArray::setAttribute( std::string attrname, int * data, int size )
  {
    return h5set_attribute( H5T_NATIVE_INT, array_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a unsigned integer attribute.

    Add a unsigned integer attribute to the array.

    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalArray::setAttribute( std::string attrname, uint * data, int size )
  {
    return h5set_attribute( H5T_NATIVE_UINT, array_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a long integer attribute.

    Add a long integer attribute to the array.

    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalArray::setAttribute( std::string attrname, long * data, int size )
  {
    return h5set_attribute( H5T_NATIVE_LONG, array_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a floating point attribute.

    Add a floating point attribute to the array.

    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalArray::setAttribute( std::string attrname, float * data, int size )
  {
    return h5set_attribute( H5T_NATIVE_FLOAT, array_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute

  /*!
    \brief Add a double precision floating point attribute.

    Add a double precision floating point attribute to the array.

    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \return bool -- DAL::FAIL or DAL::SUCCESS
   */
  bool dalArray::setAttribute( std::string attrname, double * data, int size )
  {
    return h5set_attribute( H5T_NATIVE_DOUBLE, array_id, attrname, data, size );
  }

  // ---------------------------------------------- setAttribute_string

  /*!
    \brief Define a string attribute.

    Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalArray::setAttribute( std::string attrname, std::string data )
  {
    return h5setAttribute_string( array_id, attrname, &data, 1 );
  }

  // ---------------------------------------------- setAttribute_string

  /*!
    \brief Define a string attribute.

    Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \return bool -- DAL::FAIL or DAL::SUCCESS
  */
  bool dalArray::setAttribute( std::string attrname, std::string * data,
                                 int size )
  {
    return h5setAttribute_string( array_id, attrname, data, size );
  }

// ------------------------------------------------------------ dalShortArray
  /********************************************************************
   *  dalShortArray constructor creates an n-dimensional
   *    array of Integers.
   *
   *  arguments:
   *    voidfile (I) - dataset file handle
   *    arrayname (I) - name of the array to create
   *    dims (I) - vector of the array dimensions
   *    data (I) - array of data to write
   *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
   *      if the size of the array is fixed.
   *
   ********************************************************************/
  /*!
    \brief Constructor for extendible short array.

    Constructor for an extendible short array.  This is usually called from
    the dataset object and not directly by the developer.

    \param obj_id An identifier for the dataset object.
    \param arrayname The name of the array you want to create.
    \param dims The dimensions of the array you want to create.
    \param data A structure containing the data you want to write to the
                array.  The size of the structure should match the dimensions
                of the array.
    \param chnkdims Specifies the chunk size for extendible arrays.
   */
  dalShortArray::dalShortArray( hid_t obj_id, string arrayname,
                                vector<int> dims, short data[],
                                vector<int> chnkdims )
  {
    hid_t datatype  = 0;
    hid_t dataspace = 0;  // declare a few h5 variables
    name = arrayname;  // set the private name variable to the array name

    // determine the rank from the size of the dimensions vector
    unsigned int rank = dims.size();
    hsize_t mydims[rank];  // declare a dimensions c-array
    hsize_t maxdims[rank]; // declare a maximum dimensions c-array
    hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array

    // set the c-array dimensions and maxiumum dimensions
    for (unsigned int ii=0; ii<rank; ii++)
      {
        mydims[ii] = dims[ii];  // vector to c-array
        maxdims[ii] = H5S_UNLIMITED;
      }

    // set the c-array chunk dimensions from the chunk dims vector
    for (unsigned int ii=0; ii<chnkdims.size(); ii++)
      {
        chunk_dims[ii] = chnkdims[ii];
      }

    // set the datatype to write
    if ( ( datatype = H5Tcopy(H5T_NATIVE_SHORT) ) < 0 )
      {
        std::cerr << "ERROR: Could not set array datatype.\n";
      }

    // if there are chunk dimensions, write the data this way
    if ( chnkdims.size()>0 )
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,maxdims) ) < 0 )
          {
            std::cerr << "ERROR: Could not set array dataspace.\n";
          }

        hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
        if ( cparms < 0 )
          {
            std::cerr << "ERROR: Could not set array propertylist.\n";
          }

        if ( H5Pset_chunk( cparms, rank, chunk_dims ) < 0 )
          {
            std::cerr << "ERROR: Could not set array chunk size.\n";
          }

        if ( ( array_id = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
                                      dataspace, cparms) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array.\n";
          }

      }
    // otherwise, write the data this way
    else
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,NULL) ) < 0 )
          {
            std::cerr << "ERROR: Could not set array dataspace.\n";
          }

        if ( ( array_id = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
                                      dataspace, H5P_DEFAULT ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array.\n";
          }
      }

    // write the data
    if ( H5Dwrite( array_id, datatype, dataspace, dataspace, H5P_DEFAULT,
                   data) < 0 )
      {
        std::cerr << "ERROR: Could not write array.\n";
      }


    // close local hdf5 objects
    if ( H5Sclose( dataspace ) < 0 )
      {
        std::cerr << "ERROR: Could not close array dataspace.\n";
      }

    if ( H5Tclose( datatype ) < 0 )
      {
        std::cerr << "ERROR: Could not close array datatype.\n";
      }

  }

// ------------------------------------------------------------ readShortArray

  /********************************************************************
   *  readShortArray reads an array of Shorts.
   *
   *  arguments:
   *    obj_id (I) - file or group ID
   *    arrayname (I) - name of the array to read
   *
   *  returns:
   *    pointer to an short array
   *
   ********************************************************************/

  /*!
    \brief Read the array.

    Reads an array of type short.

    \param obj_id An identifier for the dataset object.
    \param arrayname The name of the array you want to create.
    \return Pointer to array values.
   */
  short * dalShortArray::readShortArray( hid_t obj_id, string arrayname )
  {

    hid_t filespace = 0;
    hid_t data_rank = 0;
    long size       = 1;  // the size of the array

    // get the dataspace
    if ( ( filespace = H5Dget_space(obj_id) ) < 0 )
      {
        std::cerr << "ERROR: Could not get filespace for array '"
                  << arrayname << "'.\n";
        return NULL;
      }

    // retrieve the rank of the array
    if ( ( data_rank = H5Sget_simple_extent_ndims( filespace ) ) < 0 )
      {
        std::cerr << "ERROR: Could not get rank for array '"
                  << arrayname << "'.\n";
        return NULL;
      }


    hsize_t dims[ data_rank ];  // create a dims c-array using the rank

    // retrieve the dimensions of the array
    if ( H5Sget_simple_extent_dims(filespace, dims, NULL) < 0 )
      {
        std::cerr << "ERROR: Could not get dimensions for array '"
                  << arrayname << "'.\n";
        return NULL;
      }

    // from the dimensions, compute the full size of the array
    for ( int32_t ii = 0; ii < data_rank; ii++ )
      size *= dims[ii];

    short * data = NULL;  // declare a pointer to the data

    try
      {
        data = new short[size];  // allocate space for the data
      }
    catch ( bad_alloc )
      {
        std::cerr << "Can't allocate memory for reading short array '"
                  << arrayname << "'.\n";
        return NULL;
      }

    // read the data into the local array
    if ( ( H5LTread_dataset_short( obj_id, arrayname.c_str(), data ) ) < 0 )
      {
        std::cerr << "ERROR: Could not read array '" << arrayname << "'.\n";
        return NULL;
      }

    return data;  // return the data pointer

  }


// ------------------------------------------------------------ dalIntArray

  /********************************************************************
   *  dalIntArray constructor creates an n-dimensional
   *    array of Integers.
   *
   *  arguments:
   *    obj_id (I) - hdf5 object identifier
   *    arrayname (I) - name of the array to create
   *    dims (I) - vector of the array dimensions
   *    data (I) - array of data to write
   *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
   *      if the size of the array is fixed.
   *
   ********************************************************************/

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
  dalIntArray::dalIntArray( hid_t obj_id, string arrayname,
                            vector<int> dims, int data[], vector<int> chnkdims )
  {
    hid_t datatype  = 0;
    hid_t dataspace = 0;  // declare a few h5 variables

    name = arrayname;  // set the private name variable to the array name

    // determine the rank from the size of the dimensions vector
    unsigned int rank = dims.size();
    hsize_t mydims[rank];  // declare a dimensions c-array
    hsize_t maxdims[rank]; // declare a maximum dimensions c-array
    hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array

    // set the c-array dimensions and maxiumum dimensions
    for (unsigned int ii=0; ii<rank; ii++)
      {
        mydims[ii] = dims[ii];  // vector to c-array
        maxdims[ii] = H5S_UNLIMITED;
      }

    // set the c-array chunk dimensions from the chunk dims vector
    for (unsigned int ii=0; ii<chnkdims.size(); ii++)
      {
        chunk_dims[ii] = chnkdims[ii];
      }

    // set the datatype to write
    if ( ( datatype = H5Tcopy(H5T_NATIVE_INT) ) < 0 )
      {
        std::cerr << "ERROR: Could not set array datatype.\n";
      }

    // if there are chunk dimensions, write the data this way
    if ( chnkdims.size()>0 )
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,maxdims) ) < 0 )
          {
            std::cerr << "ERROR: Could not set array dataspace.\n";
          }

        hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
        if ( cparms < 0 )
          {
            std::cerr << "ERROR: Could not set array propertylist.\n";
          }

        if ( H5Pset_chunk( cparms, rank, chunk_dims ) < 0 )
          {
            std::cerr << "ERROR: Could not set array chunk size.\n";
          }

        if ( ( array_id = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
                                      dataspace, cparms) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array.\n";
          }

      }
    // otherwise, write the data this way
    else
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,NULL) ) < 0 )
          {
            std::cerr << "ERROR: Could not set array dataspace.\n";
          }

        if ( ( array_id = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
                                      dataspace, H5P_DEFAULT ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array.\n";
          }
      }

    // write the data
    if ( H5Dwrite( array_id, datatype, dataspace, dataspace, H5P_DEFAULT,
                   data) < 0 )
      {
        std::cerr << "ERROR: Could not write array.\n";
      }


    // close local hdf5 objects
    if ( H5Sclose( dataspace ) < 0 )
      {
        std::cerr << "ERROR: Could not close array dataspace.\n";
      }

    if ( H5Tclose( datatype ) < 0 )
      {
        std::cerr << "ERROR: Could not close array datatype.\n";
      }
  }

// ------------------------------------------------------------ readIntArray

  /*!
    \brief Read an integer array.

    Read the values from an array of integers.  This is usually called from
    the dataset object and not directly by the developer.

    \param obj_id An identifier for the dataset object.
    \param arrayname The name of the array you want to read.
    \return Pointer to array data.
   */
  int * dalIntArray::readIntArray( hid_t obj_id, string arrayname )
  {

    hid_t filespace  = 0;
    hid_t data_rank  = 0;
    long size = 1;  // the size of the array

    // get the dataspace
    if ( ( filespace = H5Dget_space(obj_id) ) < 0 )
      {
        std::cerr << "ERROR: Could not get filespace for array '"
                  << arrayname << "'.\n";
        return NULL;
      }

    // retrieve the rank of the array
    if ( ( data_rank = H5Sget_simple_extent_ndims(filespace) ) < 0 )
      {
        std::cerr << "ERROR: Could not get rank for array '"
                  << arrayname << "'.\n";
        return NULL;
      }

    hsize_t dims[ data_rank ];  // create a dims c-array using the rank

    // retrieve the dimensions of the array
    if ( H5Sget_simple_extent_dims(filespace, dims, NULL) < 0 )
      {
        std::cerr << "ERROR: Could not get dimensions for array '"
                  << arrayname << "'.\n";
        return NULL;
      }


    // from the dimensions, compute the full size of the array
    for ( int32_t ii = 0; ii < data_rank; ii++ )
      size *= dims[ii];

    int * data = NULL;  // declare a pointer to the data

    try
      {
        data = new int[size];  // allocate space for the data
      }
    catch ( bad_alloc )
      {
        std::cerr << "Can't allocate memory for reading array '"
                  << arrayname << "'.\n";
        return NULL;
      }

    // read the data into the local array
    if ( H5LTread_dataset_int( obj_id, arrayname.c_str(), data ) < 0 )
      {
        std::cerr << "ERROR: Could not read array '" << arrayname << "'.\n";
        return NULL;
      }

    return data;  // return the data pointer
  }

  /********************************************************************
   *  dalFloatArray constructor creates an n-dimensional
   *    array of Floats.
   *
   *  arguments:
   *    voidfile (I) - dataset file handle
   *    arrayname (I) - name of the array to create
   *    dims (I) - vector of the array dimensions
   *    data (I) - array of data to write
   *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
   *      if the size of the array is fixed.
   *
   ********************************************************************/

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
  dalFloatArray::dalFloatArray( hid_t obj_id, string arrayname,
                                vector<int> dims, float data[], vector<int> chnkdims )
  {
    hid_t datatype  = 0;
    hid_t dataspace = 0;  // declare a few h5 variables

    name = arrayname;  // set the private name variable to the array name

    // determine the rank from the size of the dimensions vector
    unsigned int rank = dims.size();
    hsize_t mydims[rank];  // declare a dimensions c-array
    hsize_t maxdims[rank]; // declare a maximum dimensions c-array
    hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array

    // set the c-array dimensions and maxiumum dimensions
    for (unsigned int ii=0; ii<rank; ii++)
      {
        mydims[ii] = dims[ii];  // vector to c-array
        maxdims[ii] = H5S_UNLIMITED;
      }

    // set the c-array chunk dimensions from the chunk dims vector
    for (unsigned int ii=0; ii<chnkdims.size(); ii++)
      {
        chunk_dims[ii] = chnkdims[ii];
      }

    // set the datatype to write
    if ( ( datatype = H5Tcopy( H5T_NATIVE_FLOAT ) ) < 0 )
      {
        std::cerr << "ERROR: Could not set array datatype.\n";
      }

    // if there are chunk dimensions, write the data this way
    if ( chnkdims.size()>0 )
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,maxdims) ) < 0 )
          {
            std::cerr << "ERROR: Could not set array dataspace.\n";
          }

        hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
        if ( cparms < 0 )
          {
            std::cerr << "ERROR: Could not set array propertylist.\n";
          }

        if ( H5Pset_chunk( cparms, rank, chunk_dims ) < 0 )
          {
            std::cerr << "ERROR: Could not set array chunk size.\n";
          }

        if ( ( array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
                                      dataspace, cparms, 0, 0 ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array.\n";
          }

      }
    // otherwise, write the data this way
    else
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,NULL) ) < 0 )
          {
            std::cerr << "ERROR: Could not set array dataspace.\n";
          }

        if ( ( array_id = H5Dcreate( obj_id, arrayname.c_str(), datatype,
                                     dataspace, H5P_DEFAULT, 0,
                                     0 ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array.\n";
          }
      }

    // write the data
    if ( H5Dwrite( array_id, datatype, dataspace, dataspace, H5P_DEFAULT,
                   data ) < 0 )
      {
        std::cerr << "ERROR: Could not write array.\n";
      }


    // close local hdf5 objects
    if ( H5Sclose( dataspace ) < 0 )
      {
        std::cerr << "ERROR: Could not close array dataspace.\n";
      }

    if ( H5Tclose( datatype ) < 0 )
      {
        std::cerr << "ERROR: Could not close array datatype.\n";
      }

  }


// -------------------------------------------------- dalComplexArray_float32

  /********************************************************************
   *  dalComplexArray_float32 constructor creates an n-dimensional
   *    array of Complexs (defined in the DAL).
   *
   *  arguments:
   *    obj_id (I) - dataset file handle
   *    arrayname (I) - name of the array to create
   *    dims (I) - vector of the array dimensions
   *    data (I) - complex<float> vector of data to write
   *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
   *      if the size of the array is fixed.
   *
   ********************************************************************/
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
  dalComplexArray_float32::dalComplexArray_float32(
    hid_t obj_id,
    string arrayname,
    vector<int> dims,
    complex<float> data[],
    vector<int> chnkdims )
  {
    // declare a few h5 variables
    hid_t datatype  = 0;
    hid_t dataspace = 0;
    name = arrayname;  // set the private name variable to the array name
    hid_t complex_id = 0;

    // determine the rank from the size of the dimensions vector
    unsigned int rank = dims.size();
    hsize_t mydims[rank];  // declare a dimensions c-array
    hsize_t maxdims[rank]; // declare a maximum dimensions c-array
    hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array

    // set the c-array dimensions and maxiumum dimensions
    for (unsigned int ii=0; ii<rank; ii++)
      {
        mydims[ii] = dims[ii];  // vector to c-array
        maxdims[ii] = H5S_UNLIMITED;
      }

    // set the c-array chunk dimensions from the chunk dims vector
    for (unsigned int ii=0; ii<chnkdims.size(); ii++)
      {
        chunk_dims[ii] = chnkdims[ii];
      }

    // create a new hdf5 datatype for complex values
    if ( ( complex_id = H5Tcreate( H5T_COMPOUND, sizeof(dalcomplex_float32 )) )
         < 0 )
      {
        std::cerr << "ERROR: Could not create compound datatype.\n";
      }

    if ( H5Tinsert( complex_id, "real", HOFFSET(dalcomplex_float32,r),
                    H5T_NATIVE_FLOAT ) < 0 )
      {
        std::cerr << "ERROR: Could not create compound datatype (r).\n";
      }

    if ( H5Tinsert ( complex_id, "imag", HOFFSET(dalcomplex_float32,i),
                     H5T_NATIVE_FLOAT) < 0 )
      {
        std::cerr << "ERROR: Could not create compound datatype (i).\n";
      }

    // set the datatype to write
    if ( ( datatype = H5Tcopy( complex_id ) ) < 0 )
      {
        std::cerr << "ERROR: Could not create datatype.\n";
      }

    // if there are chunk dimensions, write the data this way
    if ( chnkdims.size()>0 )
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,maxdims) ) < 0 )
          {
            std::cerr << "ERROR: Could not create dataspace for '"
                      << arrayname << "'.\n";
          }

        hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
        if ( cparms < 0 )
          {
            std::cerr << "ERROR: Could not create property list for '"
                      << arrayname << "'.\n";
          }

        if ( ( H5Pset_chunk( cparms, rank, chunk_dims ) ) < 0 )
          {
            std::cerr << "ERROR: Could not set chunk size for '"
                      << arrayname << "'.\n";
          }

        if ( ( array_id = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
                                      dataspace, cparms) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array '" << arrayname << "'.\n";
          }
      }
    // otherwise, write the data this way
    else
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,NULL) ) < 0 )
          {
            std::cerr << "ERROR: Could not create dataspace for '"
                      << arrayname << "'.\n";
          }

        if ( ( array_id = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
                                      dataspace, H5P_DEFAULT ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array '" << arrayname << "'.\n";
          }

      }

    // write the data
    if ( H5Dwrite( array_id, datatype, dataspace, dataspace, H5P_DEFAULT,
                   data ) < 0 )
      {
        std::cerr << "ERROR: Could not write array '" << arrayname << "'\n";
      }

    // close local hdf5 objects
    if ( H5Sclose( dataspace ) < 0 )
      {
        std::cerr << "ERROR: Could not close dataspace for '"
                  << arrayname << "'.\n";
      }

    if ( H5Tclose( datatype ) < 0 )
      {
        std::cerr << "ERROR: Could not close datatype for '"
                  << arrayname << "'.\n";
      }

  }

// ---------------------------------------------------- dalComplexArray_int16
  /********************************************************************
   *  dalComplexArray_int16 constructor creates an n-dimensional
   *    array of Complexs (defined in the DAL).
   *
   *  arguments:
   *    obj_id (I) - dataset file handle
   *    arrayname (I) - name of the array to create
   *    dims (I) - vector of the array dimensions
   *    data (I) - complex<float> vector of data to write
   *    chnkdims (I) - resizing (chunking) dimensions. Empty vector
   *      if the size of the array is fixed.
   *
   ********************************************************************/

  /*!
    \brief Constructor for extendible complex int16 array.

    Constructor for an extendible complex int16 array.  This is
    usually called from the dataset object and not directly by the developer.

    \param voidfile An pointer to the file.
    \param arrayname The name of the array you want to create.
    \param dims The dimensions of the array you want to create.
    \param data A structure containing the data you want to write to the
                array.  The size of the structure should match the dimensions
               of the array.
    \param chnkdims Specifies the chunk size for extendible arrays.
   */
  dalComplexArray_int16::dalComplexArray_int16( hid_t obj_id,
      string arrayname,
      vector<int> dims,
      complex<Int16> data[],
      vector<int> chnkdims )
  {

    // declare a few h5 variables
    hid_t datatype  = 0;
    hid_t dataspace = 0;
    name = arrayname;  // set the private name variable to the array name
    hid_t complex_id = 0;

    // determine the rank from the size of the dimensions vector
    unsigned int rank = dims.size();
    hsize_t mydims[rank];  // declare a dimensions c-array
    hsize_t maxdims[rank]; // declare a maximum dimensions c-array
    hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array

    // set the c-array dimensions and maxiumum dimensions
    for (unsigned int ii=0; ii<rank; ii++)
      {
        mydims[ii] = dims[ii];  // vector to c-array
        maxdims[ii] = H5S_UNLIMITED;
      }

    // set the c-array chunk dimensions from the chunk dims vector
    for (unsigned int ii=0; ii<chnkdims.size(); ii++)
      {
        chunk_dims[ii] = chnkdims[ii];
      }

    // create a new hdf5 datatype for complex values
    if ( ( complex_id = H5Tcreate( H5T_COMPOUND, sizeof( dalcomplex_int16 )) )
         < 0 )
      {
        std::cerr << "ERROR: Could not create compound datatype.\n";
      }

    if ( H5Tinsert( complex_id, "real", HOFFSET(dalcomplex_int16,r),
                    H5T_NATIVE_SHORT) < 0 )
      {
        std::cerr << "ERROR: Could not create compound datatype (r).\n";
      }

    if ( H5Tinsert( complex_id, "imag", HOFFSET(dalcomplex_int16,i),
                    H5T_NATIVE_SHORT) < 0 )
      {
        std::cerr << "ERROR: Could not create compound datatype (i).\n";
      }

    // set the datatype to write
    if ( ( datatype = H5Tcopy( complex_id ) ) < 0 )
      {
        std::cerr << "ERROR: Could not create datatype.\n";
      }

    // if there are chunk dimensions, write the data this way
    if ( chnkdims.size()>0 )
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,maxdims) ) < 0 )
          {
            std::cerr << "ERROR: Could not create dataspace for '"
                      << arrayname << "'.\n";
          }

        hid_t cparms;
        if ( ( cparms = H5Pcreate( H5P_DATASET_CREATE ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create property list for '"
                      << arrayname << "'.\n";
          }

        if ( ( H5Pset_chunk( cparms, rank, chunk_dims ) ) < 0 )
          {
            std::cerr << "ERROR: Could not set chunk size for '"
                      << arrayname << "'.\n";
          }

        if ( ( array_id = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
                                      dataspace, cparms ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array '" << arrayname << "'.\n";
          }
      }
    // otherwise, write the data this way
    else
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,NULL ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create dataspace for '"
                      << arrayname << "'.\n";
          }

        if ( ( array_id = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
                                      dataspace, H5P_DEFAULT ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array '" << arrayname << "'.\n";
          }

      }

    // write the data
    if ( H5Dwrite( array_id, datatype, dataspace, dataspace, H5P_DEFAULT,
                   data) < 0 )
      {
        std::cerr << "ERROR: Could not write array '" << arrayname << "'\n";
      }

    // close local hdf5 objects
    if ( H5Sclose( dataspace ) < 0 )
      {
        std::cerr << "ERROR: Could not close dataspace for '"
                  << arrayname << "'.\n";
      }

    if ( H5Tclose( datatype ) < 0 )
      {
        std::cerr << "ERROR: Could not close datatype for '"
                  << arrayname << "'.\n";
      }

  }

#ifdef PYTHON
  /************************************************************************
   *
   * The following functions are boost wrappers to allow some previously
   *   defined functions to be easily called from a python prompt.
   *
   ************************************************************************/

// ------------------------------------------------------------ extend_boost
  /******************************************************
   * wrapper for dalArray::extend
   ******************************************************/
  void dalArray::extend_boost( bpl::list pydims )
  {
    vector<int> dims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    extend( dims );
  }

  bool dalArray::setAttribute_char( std::string attrname, char data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalArray::setAttribute_short( std::string attrname, short data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalArray::setAttribute_int( std::string attrname, int data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalArray::setAttribute_uint( std::string attrname, uint data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalArray::setAttribute_long( std::string attrname, long data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalArray::setAttribute_float( std::string attrname, float data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalArray::setAttribute_double( std::string attrname, double data )
  {
     return setAttribute( attrname, &data );
  }
  bool dalArray::setAttribute_string( std::string attrname, std::string data )
  {
     return setAttribute( attrname, &data );
  }
#endif // end #ifdef PYTHON

} // end namespace DAL
