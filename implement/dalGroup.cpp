/*-------------------------------------------------------------------------*
 | $Id:: cs1.h 389 2007-06-13 08:47:09Z baehren                          $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
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

#ifndef DALGROUP_H
#include "dalGroup.h"
#endif

namespace DAL
  {

// ==============================================================================
//
//  Construction
//
// ==============================================================================

  /*!
          \brief Default constructor.

          Default constructor.
         */
  dalGroup::dalGroup()
  {
    file = NULL;
    groupname = "";
    groupname_full = "";
    group = NULL;
    tables.clear();
    attributes.clear();
    filter = NULL;
    file_id = 0;
    group_id = 0;
    status = 0;
    subgroup = NULL;
  }


  /*!
          \brief Create a group in a certain file.

          Create a group in a certain file.

          \param groupname The name of the group to create.
          \param file A pointer to the file where you want to create the group.
         */
  dalGroup::dalGroup( const char * gname, void * voidfile )
  {

    dalGroup();

    hid_t * lclfile = (hid_t*)voidfile; // H5File object
    file = lclfile;
    file_id = *lclfile;  // get the file handle

    groupname = gname;
    groupname_full = "/" + stringify(gname);
    group_id = H5Gcreate1(*(hid_t*)file, groupname_full.c_str(), 0);
  }


  /*!
          \brief Create a subgroup.

          Create a subgroup in an existing group.

          \param group_id The parent group identifier.
          \param gname The name of the subgroup.
         */
  dalGroup::dalGroup( hid_t group_id, const char * gname )
  {
    dalGroup();

    group_id = H5Gcreate1(group_id, gname, 0);
  }


  /*!
          \brief Open an existing group.

          Open an existing group.

          \param file A pointer to the file.
          \param groupname The name of the group you want to open.

          \return An identifier for the new group.
         */
  int dalGroup::open( void * voidfile, string gname )
  {

    groupname = gname;

    hid_t * lclfile = (hid_t*)voidfile; // H5File object
    file = lclfile;
    file_id = *lclfile;  // get the file handle

    groupname_full = "/" + groupname;
    group_id = H5Gopen1( file_id, groupname_full.c_str() );
    return( group_id );
  }


  /*!
          \brief Default destructor.

          Default destructor.
         */
  dalGroup::~dalGroup()
  {
    if ( 0 != group_id )
      if ( H5Gclose(group_id) < 0 )
        cout << "ERROR: dalGroup::close() failed.\n";
  }


  /*!
    \brief Get the group ID.

    Retrieve the identifier for the group.

    \return The group identifier as an integer.
  */
  hid_t dalGroup::getId()
  {
    return group_id;
  }

  /*
   * Operator function.
   */
  herr_t dalGroup_file_info(hid_t loc_id, const char *name, void *opdata)
  {
    H5G_stat_t statbuf;
    string myname;

    /*
     * Get type of the object and display its name and type.
     * The name of the object is passed to this function by
     * the Library. Some magic :-)
     */
    H5Gget_objinfo(loc_id, name, false, &statbuf);
    switch (statbuf.type)
      {
      case H5G_DATASET:
        myname = string(name);
        (*(vector<string>*)opdata).push_back( myname );
        break;
      case H5G_GROUP:
      case H5G_TYPE:
      default:
        break;
      }
    return 0;
  }


  /*!
    \brief Retrieve the array or table member names.

    Retrief the array or table member names from the group.

    \return A vector of strings representing the member names.
   */
  vector<string> dalGroup::getMemberNames()
  {
    vector<string> member_names;
    H5Giterate(file_id, groupname.c_str(), NULL, dalGroup_file_info, &member_names);
    return member_names;
  }


  /*!
          \brief Set group name.

          Set the name of the group.

          \param gname The name of the group.

          \return Zero on success. Non-zero on failure.
         */
  bool dalGroup::setName ( string gname )
  {
    if ( gname.length() > 0 )
      {
        groupname = gname;
        return SUCCESS;
      }
    else
      {
        cout << "Error:  Group name must not be empty." << endl;
        return FAIL;
      }
  }


  /*!
    \brief Create an array of shorts within the group.

    Create an array of shorts with any dimensions.  This is usually
    called from the dataset object and not from the developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createShortArray( string arrayname,
                              vector<int> dims,
                              short data[],
                              vector<int> cdims )
  {
    dalShortArray * la;
    la = new dalShortArray( group_id, arrayname, dims, data, cdims );
    return la;
  }



  /*!
    \brief Create an array of integers within the group.

    Create an array of integers with any dimensions.  This is usually
    called from the dataset object and not from the developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createIntArray( string arrayname,
                            vector<int> dims,
                            int data[],
                            vector<int> cdims )
  {
    dalIntArray * la;
    la = new dalIntArray( group_id, arrayname, dims, data, cdims );
    return la;
  }


  /*!
    \brief Create an array of floating point values within the group.

    Create an array of floating point values with any dimensions.  This
    is usually called from the dataset object and not from the developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createFloatArray( string arrayname,
                              vector<int> dims,
                              float data[],
                              vector<int> cdims )
  {
    dalFloatArray * la;
    la = new dalFloatArray( group_id, arrayname, dims, data, cdims );
    return la;
  }


  /*!
    \brief Create an array of complex floating point values within the group.

    Create an array of complex floating point values with any dimensions.
    This is usually called from the dataset object and not from the
    developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createComplexFloatArray( string arrayname,
                                     vector<int> dims,
                                     complex<float> data[],
                                     vector<int> cdims )
  {
    dalComplexArray_float32 * la;
    la = new dalComplexArray_float32( group_id, arrayname, dims, data, cdims );
    return la;
  }


  /*!
    \brief Create an array of complex int16 values within the group.

    Create an array of complex int16 values with any dimensions.
    This is usually called from the dataset object and not from the
    developer.

    \param arrayname A string containing he name of the array.
    \param dims A vector specifying the array dimensions.
    \param data A structure containing the data to be written.  The size
                of the data must match the provided dimensions.
    \param cdims The chunk dimensions for an extendible array.

    \return dalArray * A pointer to an array object.
  */
  dalArray *
  dalGroup::createComplexShortArray( string arrayname,
                                     vector<int> dims,
                                     complex<Int16> data[],
                                     vector<int> cdims )
  {
    dalComplexArray_int16 * la;
    la = new dalComplexArray_int16( group_id, arrayname, dims, data, cdims );
    return la;
  }

  /*!
          \brief Get group name.

          Retrieve the name of the group object.

          \return The name of the group.
         */
  string dalGroup::getName ()
  {
    return groupname;
  }


  /*!
    \brief Define a string attribute.

    Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size The size of the array.
  */
  void dalGroup::setAttribute_string( string attrname, string * data, int size )
  {
    hid_t att = 0;
    hid_t dataspace = 0;
    hsize_t dims[1] = { size };

    char ** string_attr = (char**)malloc( size * sizeof(char*) );
    for ( int ii = 0; ii < size; ii++ )
      {
        string_attr[ii] = (char*)malloc(MAX_COL_NAME_SIZE * sizeof(char));
        strcpy( string_attr[ii], data[ii].c_str() );
        cerr << string_attr[ii] << endl;
      }

    hid_t type = H5Tcopy (H5T_C_S1);
    status = H5Tset_size (type, H5T_VARIABLE);
    dataspace = H5Screate_simple(1, dims, NULL);
    att = H5Acreate1( group_id, attrname.c_str(), type, dataspace, H5P_DEFAULT );
    status = H5Awrite( att, type, string_attr ) ;
    status = H5Aclose( att );

    for ( int ii = 0; ii < size; ii++ )
      {
        free( string_attr[ii] );
      }
    free( string_attr );

  }


  /*!
    \brief Define a string attribute.

    Define a string attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
  */
  void dalGroup::setAttribute_string( string attrname, string data )
  {
    if ( H5LTset_attribute_string( file_id, groupname.c_str(),
                                   attrname.c_str(), data.c_str() ) < 0 )
      {
        cout << "ERROR: could not set attribute " << attrname << endl;
      }
  }


  /*!
    \brief Define a integer attribute.

    Define a integer attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
  */
  void dalGroup::setAttribute_int( string attrname, int * data, int size )
  {
    if ( H5LTset_attribute_int( file_id, groupname.c_str(),
                                attrname.c_str(), data, size ) < 0 )
      {
        cout << "ERROR: could not set attribute " << attrname << endl;
      }
  }


  /*!
    \brief Define an unsigned integer attribute.

    Define an unsigned integer attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
  */
  void dalGroup::setAttribute_uint( string attrname, unsigned int * data, int size )
  {
    if ( H5LTset_attribute_uint( file_id, groupname.c_str(),
                                 attrname.c_str(), data, size ) < 0 )
      {
        cout << "ERROR: could not set attribute " << attrname << endl;
      }
  }


  /*!
    \brief Define a double precision floating point attribute.

    Define a double precision floating point attribute.

    \param attrname The name of the attribute you want to create.
    \param data The value of the attribute you want to create.
    \param size Optional parameter specifying the array size of the
                attribute.  Default is scalar.
  */
  void dalGroup::setAttribute_double( string attrname, double * data, int size )
  {
    if ( H5LTset_attribute_double( file_id, groupname.c_str(),
                                   attrname.c_str(), data, size ) < 0 )
      {
        cout << "ERROR: could not set attribute " << attrname << endl;
      }
  }


  /*!
    \brief Print the group attributes.

    Print the group attributes.
  */
  void dalGroup::getAttributes()
  {

    //status = H5Aget_num_attrs(group_id);
    //printf ("H5Aget_num_attrs returns: %i\n", status);
    //unsigned number = 0;
    //unsigned * idx = &number;
    //status = H5Aiterate( group_id, idx, attr_info, NULL );
    //printf ("\nH5Aiterate returns: %i\n", status);

  }


  /*!
    \brief Print the value of a specified attribute.

    Print the value of a specified attribute.

    \param attrname The name of the attribute you want to print.
  */
  void dalGroup::printAttribute( string attrname )
  {

    hsize_t * dims;
    H5T_class_t type_class;
    size_t type_size;

    // Check if attribute exists
    if ( H5Aexists(group_id, attrname.c_str()) <= 0 )
      {
        cerr << "Attribute " << attrname << " not found." << endl;
        return;
      }

    groupname_full = "/" + groupname;

    int rank;
    H5LTget_attribute_ndims(file_id, groupname_full.c_str(), attrname.c_str(),
                            &rank );

    dims = (hsize_t *)malloc(rank * sizeof(hsize_t));

    H5LTget_attribute_info( file_id, groupname_full.c_str(), attrname.c_str(),
                            dims, &type_class, &type_size );

    if ( H5T_FLOAT == type_class )
      {
        double data[*dims];
        H5LTget_attribute(file_id, groupname_full.c_str(), attrname.c_str(),
                          H5T_NATIVE_DOUBLE, data);
        cout << attrname << " = ";
        for (unsigned int ii=0; ii<*dims; ii++)
          {
            cout << data[ii];
            if (ii < (*dims)-1)
              cout << ',';
            else
              cout << endl;
          }
      }
    else if ( H5T_INTEGER == type_class )
      {
        int data[*dims];
        H5LTget_attribute(file_id, groupname_full.c_str(), attrname.c_str(),
                          H5T_NATIVE_INT, data);
        cout << attrname << " = ";
        for (unsigned int ii=0; ii<*dims; ii++)
          {
            cout << data[ii];
            if (ii < (*dims)-1)
              cout << ',';
            else
              cout << endl;
          }
      }
    else if ( H5T_STRING == type_class )
      {
        char* data;
        groupname_full = "/" + groupname;
        data = (char *)malloc(rank * sizeof(char));
        H5LTget_attribute_string( file_id, groupname_full.c_str(),
                                  attrname.c_str(),data);
        cout << attrname << " = " << data << endl;
      }
    else
      {
        cout << "Attribute " << attrname << " type unknown." << endl;
      }
  }


  /*!
    \brief Get the value of an attribute.

    Get the value of an attribute.  This is different from printAttribute
    because the value of the attribute is returned into a structure
    instead of simply printing.

    \param attrname The name of the attribute you want to retrieve.

    \return void * A pointer to the data in the attribute.
  */
  void * dalGroup::getAttribute( string attrname )
  {

    hsize_t dims;
    H5T_class_t type_class;
    size_t type_size;

    // Check if attribute exists
    if ( H5Aexists(group_id, attrname.c_str()) <= 0 )
      {
        return NULL;
      }

    groupname_full = "/" + groupname;

    int rank;
    H5LTget_attribute_ndims(file_id, groupname_full.c_str(), attrname.c_str(),
                            &rank );

    H5LTget_attribute_info( file_id, groupname_full.c_str(), attrname.c_str(),
                            &dims, &type_class, &type_size );

    if ( H5T_FLOAT == type_class )
      {
        double * data;
        data = new double[1];
        if ( 0 < H5LTget_attribute(file_id, groupname_full.c_str(), attrname.c_str(),
                                   H5T_NATIVE_DOUBLE, data) )
          return NULL;
        else
          return reinterpret_cast<double*>(data);
      }
    else if ( H5T_INTEGER == type_class )
      {
        int * data;
        data = new int[1];
        if ( 0 < H5LTget_attribute(file_id, groupname_full.c_str(), attrname.c_str(),
                                   H5T_NATIVE_INT, data) )
          return NULL;
        else
          return reinterpret_cast<int*>(data);
      }
    else if ( H5T_STRING == type_class )
      {
        char * data;
        groupname_full = "/" + groupname;
        data = new char[256];
        if ( 0 < H5LTget_attribute_string( file_id, groupname_full.c_str(),
                                           attrname.c_str(),data) )
          return NULL;
        else
          return data;
      }
    else
      {
        return NULL;
      }
  }

#ifdef PYTHON
  /************************************************************************
   *
   * The following functions are boost wrappers to allow some previously
   *   defined functions to be easily called from a python prompt.
   *
   ************************************************************************/

  /******************************************************
   * wrappers for createIntArray
   ******************************************************/

  dalArray * dalGroup::cia_boost1( string arrayname, bpl::list pydims,
                                   bpl::list pydata )
  {
    bpl::list cdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      cdims.append(10);

    dalArray * array;
    array = cia_boost2( arrayname, pydims, pydata, cdims );

    return array;
  }

  dalArray * dalGroup::cia_boost2(
    string arrayname,
    bpl::list pydims,
    bpl::list pydata,
    bpl::list cdims )
  {

    vector<int> dims;
    vector<int> chnkdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    for (int ii=0; ii<bpl::len(cdims); ii++)
      chnkdims.push_back(bpl::extract<int>(cdims[ii]));

    long size = bpl::len(pydata);
    int * data = NULL;
    data = new int[size];

    for (int ii=0; ii<size; ii++)
      data[ii] = bpl::extract<int>(pydata[ii]);

    dalArray * array = createIntArray(arrayname, dims, data, chnkdims);

    delete [] data;
    data = NULL;

    return array;
  }

  dalArray * dalGroup::cia_boost_numarray1(
    string arrayname,
    bpl::list pydims,
    bpl::numeric::array pydata )
  {

    bpl::list cdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      cdims.append(10);

    dalArray * array;
    array = cia_boost_numarray2(arrayname, pydims, pydata, cdims);

    return array;
  }

  dalArray * dalGroup::cia_boost_numarray2(
    string arrayname,
    bpl::list pydims,
    bpl::numeric::array pydata,
    bpl::list cdims )
  {

    vector<int> dims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    pydata.setshape( pydata.nelements() );
    bpl::object flat_data = pydata.getflat();
    bpl::list list_data( flat_data );

    dalArray * array = cia_boost2(arrayname, pydims, list_data, cdims);

    return array;
  }

  /******************************************************
   * wrapper for readIntArray
   ******************************************************/

  bpl::numeric::array dalGroup::ria_boost( string arrayname )
  {
    hid_t lclfile;
    hid_t  status;

    // get the dataspace
    lclfile = H5Dopen1( group_id, arrayname.c_str() );
    hid_t filespace = H5Dget_space(lclfile);

    // what is the rank of the array?
    hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
    hsize_t dims[ data_rank ];
#ifdef DEBUGGING_MESSAGES
    cout << "data rank: " << data_rank << endl;
#endif
    status = H5Sget_simple_extent_dims(filespace, dims, NULL);

    int size = 1;
    bpl::list dims_list;
    for (int ii=0; ii<data_rank; ii++)
      {
#ifdef DEBUGGING_MESSAGES
        cout << "dims["  << ii << "]: " << dims[ii] << endl;
#endif
        size *= dims[ii];
        dims_list.append(dims[ii]);
      }

#ifdef DEBUGGING_MESSAGES
    cout << "size: " << size << endl;
#endif

    int * data = NULL;
    data = new int[size];

    status = H5LTread_dataset_int( group_id, arrayname.c_str(), data );

#ifdef DEBUGGING_MESSAGES
    for (int ii=0; ii<size; ii++)
      {
        cout << data[ii] << endl;
      }
#endif

    bpl::list data_list;
    // for each dimension
    for (int ii=0; ii<size; ii++)
      {
        data_list.append(data[ii]);
      }

    delete [] data;
    data = NULL;

    bpl::numeric::array nadata(
      bpl::make_tuple(
        bpl::make_tuple(data_list)
      )
    );

    nadata.setshape(dims_list);
    return nadata;
  }

  /******************************************************
   * wrappers for createFloatArray
   ******************************************************/
  dalArray * dalGroup::cfa_boost( string arrayname, bpl::list pydims, bpl::list pydata, bpl::list cdims )
  {

    vector<int> dims;
    vector<int> chnkdims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    for (int ii=0; ii<bpl::len(cdims); ii++)
      chnkdims.push_back(bpl::extract<int>(cdims[ii]));

    long size = bpl::len(pydata);
    float * data = NULL;
    data = new float[size];

    for (int ii=0; ii<size; ii++)
      data[ii] = bpl::extract<float>(pydata[ii]);

    dalArray * array = createFloatArray(arrayname, dims, data, chnkdims);

    delete [] data;
    data = NULL;

    return array;
  }

  dalArray * dalGroup::cfa_boost_numarray( string arrayname,
      bpl::list pydims,
      bpl::numeric::array pydata, bpl::list cdims )
  {

    vector<int> dims;

    for (int ii=0; ii<bpl::len(pydims); ii++)
      dims.push_back(bpl::extract<int>(pydims[ii]));

    pydata.setshape( pydata.nelements() );
    bpl::object flat_data = pydata.getflat();
    bpl::list list_data( flat_data );

    dalArray * array = cfa_boost(arrayname, pydims, list_data, cdims);

    return array;
  }


#endif // end #ifdef PYTHON

} // end namespace DAL
