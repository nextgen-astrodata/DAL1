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

#ifndef DALCOLUMN_H
#include "dalColumn.h"
#endif

namespace DAL {

  // ============================================================================
  //
  //  Constructors
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                    dalColumn

  dalColumn::dalColumn()
  {}

  //_____________________________________________________________________________
  //                                                                    dalColumn

  /*!
    Create a new column with a complex floating point datatype.

    \param complexcolname Name of the column you want to create.
  */
  dalColumn::dalColumn( std::string complexcolname )
  {
    name = complexcolname;
    dal_datatype = dal_COMPLEX;
  }

  //_____________________________________________________________________________
  //                                                                    dalColumn

  /*!
    \param fileid   -- Object identifier for the file
    \param tableid  -- Object identifier for the table
    \param filetype -- Type of dataset/file
    \param lcl_tablename
    \param colname
    \param coldatatype
  */
  dalColumn::dalColumn (hid_t fileid,
                        hid_t tableid,
                        std::string lcl_filetype,
                        std::string lcl_tablename,
                        std::string colname,
                        std::string coldatatype )
  {
    fileID_p     = fileid;
    tableID_p    = tableid;
    filetype     = lcl_filetype;
    tablename    = lcl_tablename;
    name         = colname;
    dal_datatype = coldatatype;
  }

  //_____________________________________________________________________________
  //                                                                    dalColumn

  /*!
    \brief Create a new column object.

    Create a new column object.

    \param colname The name of the column you want to create.
    \param coltype The datatype of the column you want to craete (i.e.
    dalINT, dalFLOAT, dalSTRING, etc.)
  */
  dalColumn::dalColumn( std::string colname, std::string type )
  {
    name = colname;
    dal_datatype = type;
  }

  //_____________________________________________________________________________
  //                                                                    dalColumn

#ifdef HAVE_CASA
  /*!
    \brief Create a new column object from a Casa table.

    Create a new column object from a Casa table.

    \param table A casa table object.
    \param colname Name of the column.
  */
  dalColumn::dalColumn(casa::Table table, std::string colname)
  {
    filetype = MSCASATYPE;
    bool error = false;
    try
      {
        casa_column = new casa::ROTableColumn( table, colname );
      }
    catch (casa::AipsError x)
      {
        std::cerr << "ERROR: " << x.getMesg() << endl;
        error = true;
      }

    if (!error)
      {
        casa_col_desc = casa_column->columnDesc();
        casa_datatype = getDataType();
      }
    else
      {
        casa_datatype = "unknown";
      }
  }
#endif

  //_____________________________________________________________________________
  //                                                                        close

  void dalColumn::close()
  {
#ifdef HAVE_CASA
    delete casa_column;
#endif
  }
  
  //_____________________________________________________________________________
  //                                                              getCasaDataType

#ifdef HAVE_CASA

  std::string dalColumn::getCasaDataType ()
  {
    switch ( casa_col_desc.dataType() )
      {
      case ( casa::TpDouble ):
        casa_datatype = "double";
        break;
      case ( casa::TpComplex ):
        casa_datatype = "complex";
        break;
      case ( casa::TpInt ):
        casa_datatype = "int";
        break;
      case ( casa::TpString ):
        casa_datatype = "string";
        break;
      case ( casa::TpBool ):
        casa_datatype = "bool";
        break;
      default:
        casa_datatype = "Unsupported type";
      }
    return casa_datatype;
  }

#endif

  // ------------------------------------------------------- getDataType

  // enum DataType {TpBool,         TpChar,          TpUChar,
  //              TpShort,        TpUShort,        TpInt,        TpUInt,
  //              TpFloat,        TpDouble,
  //              TpComplex,      TpDComplex,      TpString,
  //              TpTable,
  //              TpArrayBool,    TpArrayChar,     TpArrayUChar,
  //              TpArrayShort,   TpArrayUShort,   TpArrayInt,   TpArrayUInt,
  //              TpArrayFloat,   TpArrayDouble,
  //              TpArrayComplex, TpArrayDComplex, TpArrayString,
  //              TpRecord, TpOther,
  //              TpNumberOfTypes
  //               };

  /*!
    \brief Get the column datatype.

    Get the datatype of the column.

    \return A string describing the datatype of the column.
  */
  std::string dalColumn::getDataType()
  {
    if ( MSCASATYPE == filetype )
      {
#ifdef HAVE_CASA
        return getCasaDataType();
#else
        std::cerr << "ERROR: CASA not enabled [dalColumn::getDataType()]\n";
        return("");
#endif
      }
    else
      {
        std::cerr << "dalColumn::getDataType not yet supported for filetype " <<
                  filetype << endl;
        return "";
      }
  }


  // ------------------------------------------------------- isScalar

  /*!
    \brief Is the column a scalar?

    Check to see if the column is scalar.

    \return Non-zero if the column is scalar.
  */
  bool dalColumn::isScalar()
  {
    if ( MSCASATYPE == filetype )
      {
#ifdef HAVE_CASA
        if ( casa_col_desc.isScalar() )
          return true;
        else
          return false;
#else
        std::cerr << "ERROR: CASA not enabled [dalColumn::isScalar()]\n";
        return false;
#endif
      }
    else
      {
        std::cerr << "dalColumn::isScalar not yet supported for filetype " <<
                  filetype << endl;
        return false;
      }
  }


  // ------------------------------------------------------- isArray

  /*!
    \brief Is the column an array?

    Check to see if the column is an array.

    \return Non-zero if the column is an array.
  */
  bool dalColumn::isArray()
  {
    if ( MSCASATYPE == filetype )
      {
#ifdef HAVE_CASA
        if ( casa_col_desc.isArray() )
          return true;
        else
          return false;
#else
        std::cerr << "ERROR: CASA not enabled [dalColumn::isArray()]\n";
        return false;
#endif
      }
    else
      {
        std::cerr << "dalColumn::isArray not yet supported for filetype " <<
                  filetype << endl;
        return false;
      }
  }


  // ------------------------------------------------------- shape

  /*!
    \brief Get the shape of the column.

    Get the shape of the column.

    \return A vector of integers specifying the shape of the column.
  */
  vector<int> dalColumn::shape()
  {
    vector<int> shape_vals;
    if ( MSCASATYPE == filetype )
      {
#ifdef HAVE_CASA
        if ( isArray() )
          {
            //std::cerr << casa_column->shapeColumn();

            try
              {
                casa::IPosition ipos = casa_column->shape(0);
                casa::Vector<casa::Int> col_shape = ipos.asVector();
                col_shape.tovector( shape_vals );
                shape_vals.push_back( nrows() );
                std::reverse(shape_vals.begin(), shape_vals.end());
              }
            catch (casa::AipsError x)
              {
                std::cerr << "ERROR: " << x.getMesg() << endl;
              }
            return shape_vals;
          }
        else
          {
            shape_vals.push_back( nrows() );
            return shape_vals;
          }
#else
        std::cerr << "ERROR: CASA not enabled [dalColumn::shape()]\n";
        return(shape_vals);
#endif
      }
    else
      {
        std::cerr << "dalColumn::shape not yet supported for filetype " <<
                  filetype << endl;
        return shape_vals;
      }
  }


  // ------------------------------------------------------- ndims

  /*!
    \return ndims -- An integer specifying the number of column dimensions.
  */
  int dalColumn::ndims()
  {
    if ( MSCASATYPE == filetype )
      {
#ifdef HAVE_CASA
        if ( isArray() )
          return casa_column->ndimColumn();
        else
          {
            return 0;
          }
#else
        std::cerr << "ERROR: CASA not used [dalColumn::ndims]\n";
        return(-1);
#endif
      }
    else
      {
        std::cerr << "dalColumn::ndims not yet supported for filetype " <<
                  filetype << endl;
        return -1;
      }
  }


  // ------------------------------------------------------- setName

  /*!
    \param colname -- The name of the column.
  */
  void dalColumn::setName(std::string colname)
  {
    name = colname;
  }


  // ------------------------------------------------------- setFileType

  /*!
    \param -- type The type of file.
  */
  void dalColumn::setFileType( std::string type )
  {
    filetype = type;
  }


  // ------------------------------------------------------- getName

  /*!
    \return name -- The name of the column.
  */
  std::string dalColumn::getName()
  {
    return name;
  }



  // ------------------------------------------------------- nrows

  /*!
    \return A number specifying the number of column rows.
  */
  uint dalColumn::nrows()
  {
    if ( MSCASATYPE == filetype )
      {
#ifdef HAVE_CASA
        num_of_rows = casa_column->nrow();
        return num_of_rows;
#else
        std::cerr << "ERROR: casacore not installed\n";
        return 0;
#endif
      }
    else
      {
        std::cerr << "dalColumn::nrows() File type " << filetype
                  << " not yet supported.";
        return 0;
      }
  }


  // ------------------------------------------------------- getType

  /*!
    \brief Get the data type.

    Retrieve the datatype of the column.

    \return A string describing the column datatype.
  */
  std::string dalColumn::getType()
  {
    return dal_datatype;
  }


  // ------------------------------------------------------- data

  /*!
    \return A dalData object containing the column data.
  */
  dalData * dalColumn::data()
  {
    int start = -1;
    int length = -1;
    return data( start, length );
  }


  // ------------------------------------------------------- CasaData_scalar

#ifdef HAVE_CASA

  dalData * dalColumn::CasaData_scalar( )
  {
    switch ( casa_col_desc.dataType() )
      {
      case casa::TpInt:
      {
        rosc_int = new casa::ROScalarColumn<casa::Int>( *casa_column );
        scalar_vals_int = rosc_int->getColumn();
        data_object = new dalData( filetype, dal_INT, shape(), nrows() );
        data_object->data = (int *)scalar_vals_int.getStorage(deleteIt);
        return data_object;
      }
      break;
      case casa::TpDouble:
      {
        rosc_dbl = new casa::ROScalarColumn<casa::Double>( *casa_column );
        scalar_vals_dbl = rosc_dbl->getColumn();
        data_object = new dalData( filetype, dal_DOUBLE, shape(), nrows() );
        data_object->data = (double *)scalar_vals_dbl.getStorage(deleteIt);
        return data_object;
      }
      break;
      case casa::TpComplex:
      {
        rosc_comp = new casa::ROScalarColumn<casa::Complex>( *casa_column );
        scalar_vals_comp = rosc_comp->getColumn();
        data_object = new dalData( filetype, dal_COMPLEX, shape(), nrows() );
        data_object->data =
          (std::complex<float> *)scalar_vals_comp.getStorage(deleteIt);
        return data_object;
      }
      break;
      case casa::TpString:
      {
        rosc_string = new casa::ROScalarColumn<casa::String>( *casa_column );
        scalar_vals_string = rosc_string->getColumn();
        data_object = new dalData( filetype, dal_STRING, shape(), nrows() );
        data_object->data =
          (std::string *)scalar_vals_string.getStorage(deleteIt);
        return data_object;
      }
      break;
      /************************************
       * ADD MORE TYPES HERES
       ************************************/
      default:
      {
        std::cerr << "dalColumn::data() Column type not yet supported."
                  << endl;
        return NULL;
      }
      }

  }
#endif

  // ------------------------------------------------------- CasaData_array

#ifdef HAVE_CASA

  dalData * dalColumn::CasaData_array( )
  {
    switch ( casa_col_desc.dataType() ) {
    case casa::TpInt:
      {
        roac_int = new casa::ROArrayColumn<casa::Int>( *casa_column );
        array_vals_int = roac_int->getColumn();
        data_object = new dalData( filetype, dal_INT, shape(), nrows() );
        data_object->data = (int *)array_vals_int.getStorage(deleteIt);
        return data_object;
      }
      break;
    case casa::TpDouble:
      {
        roac_dbl = new casa::ROArrayColumn<casa::Double>( *casa_column );
        array_vals_dbl = roac_dbl->getColumn();
        data_object = new dalData( filetype, dal_DOUBLE, shape(), nrows() );
        data_object->data = (double *)array_vals_dbl.getStorage(deleteIt);
        return data_object;
      }
      break;
    case casa::TpComplex:
      {
        dal_datatype = dal_COMPLEX;
	std::vector< std::complex< float > > ret_valvec;
        try
          {
            roac_comp = new casa::ROArrayColumn<casa::Complex>( *casa_column );
          }
        catch (casa::AipsError x)
          {
            std::cerr << "ERROR: " << x.getMesg() << endl;
            return NULL;
          }
        array_vals_comp = roac_comp->getColumn( );
        data_object = new dalData( filetype, dal_COMPLEX, shape(), nrows() );
        data_object->data =
          (std::complex<float> *)array_vals_comp.getStorage(deleteIt);
        return data_object;
      }
      break;
    case casa::TpString:
      {
        roac_string = new casa::ROArrayColumn<casa::String>( *casa_column );
        array_vals_string = roac_string->getColumn();
        data_object = new dalData( filetype, dal_STRING, shape(), nrows() );
        data_object->data = (std::string *)array_vals_string.getStorage(deleteIt);
        return data_object;
      }
      break;
      /************************************
       * ADD MORE TYPES HERES
       ************************************/
    default:
      {
        std::cerr << "dalColumn::data() Column type not yet supported."
                  << endl;
        return NULL;
      }
    }
  }
#endif
  
  // ------------------------------------------------------- H5data
  
  dalData * dalColumn::H5data (int &start,
                               int &length)
  {
    char  ** field_names;
    size_t * field_sizes = NULL;
    size_t * field_offsets = NULL;
    size_t * size_out = NULL;
    bool column_in_table = false;
    
    // retrieve the input fields needed for the append_records call
    if ( H5TBget_table_info ( fileID_p, tablename.c_str(), &nofFields_p, &nofRecords_p )
         < 0 )
      return NULL;

    field_sizes = (size_t*)malloc( nofFields_p * sizeof(size_t) );
    field_offsets = (size_t*)malloc( nofFields_p * sizeof(size_t) );
    size_out = (size_t*)malloc( sizeof(size_t) );

    /* Alocate space */
    field_names = (char**)malloc( sizeof(char*) * (size_t)nofFields_p );
    for ( hsize_t ii = 0; ii < nofFields_p; ii++)
      field_names[ii] = (char*)malloc( sizeof(char) * MAX_COL_NAME_SIZE );

    if ( H5TBget_field_info( fileID_p, tablename.c_str(), field_names,
                             field_sizes, field_offsets, size_out ) < 0 )
      return NULL;

    for ( hsize_t ii = 0; ii < nofFields_p; ii++)
      {
        if ( 0 == strcmp( field_names[ii], name.c_str() ) )
          column_in_table = true;
      }
    
    /* Release memory allocated for the columns of the table */
    for ( hsize_t ii = 0; ii < nofFields_p; ii++) {
      free( field_names[ii] );
    }
    
    free( field_names );
    
    
    if ( !column_in_table )  // if column isn't found in the table
      {
        std::cerr << "ERROR: column not found in table.\n";
        return NULL;
      }
    
    if ( start < 0 ) {
      start = 0;
    }
    if ( length < 0 ) {
      length = nofRecords_p;
    }

    if ( dal_COMPLEX_SHORT == getType() )
      {
        dalcomplex_int16 * data = NULL;
        try
          {
            data = (dalcomplex_int16*)malloc(sizeof(dalcomplex_int16)*length);
          }
        catch ( std::bad_alloc )
          {
            std::cerr <<
                      "ERROR: Could not allocate memory buffer for dalColumn\n";
            return NULL;
          }

        if ( H5TBread_fields_name ( fileID_p, tablename.c_str(),
                                    name.c_str(), start, length,
                                    sizeof(dalcomplex_int16),
                                    field_offsets, field_sizes,
                                    data ) < 0 )
          {
            std::cerr << "ERROR: H5TBread_fields_name failed.\n";
            return NULL;
          }

        vector<int> shape(1);

        data_object = new dalData( filetype, dal_COMPLEX_SHORT,
                                   shape, length );
        data_object->data = (dalcomplex_int16 *)data;
      }
    else if ( dal_FLOAT == getType() )
      {
        float * data = NULL;
        try
          {
            data = (float*)malloc(sizeof(float)*length);
          }
        catch ( std::bad_alloc )
          {
            std::cerr << "ERROR: Could not allocate memory buffer for " <<
                      "dalColumn\n";
            return NULL;
          }

        if ( H5TBread_fields_name (fileID_p, tablename.c_str(), name.c_str(),
                                   start, length, sizeof(float), field_offsets, field_sizes,
                                   data ) < 0 )
          {
            std::cerr << "ERROR: H5TBread_fields_name failed.\n";
            return NULL;
          }

        vector<int> shape(1);

        data_object = new dalData( filetype, dal_FLOAT, shape, length );
        data_object->data = (float *)data;
      }
    else
      {
        std::cerr << "ERROR: datatype not supported [dalColumn.data]\n";
        free( field_sizes );
        free( field_offsets );
        free( size_out );
        return NULL;
      }

    free( field_sizes );
    free( field_offsets );
    free( size_out );

    return data_object;

  }

// ------------------------------------------------------- data

  /*!
    \brief Get column data.

    Get the data object for the column.

    \param start Start cell number for column.
    \param length Number of cells to retrieve into the data object.
    \return A dalData object containing the column data.
   */
  dalData * dalColumn::data (int &start,
                             int &length)
  {
    if ( MSCASATYPE == filetype )
      {
#ifdef HAVE_CASA
        try
          {
            if ( isScalar() )
              {
                return CasaData_scalar( );
              }
            else if ( isArray() )
              {
                return CasaData_array( );
              }
            else
              {
                std::cerr << "dalColumn::data() Column is neither "
                          << "scalar nor array.  "
                          << "Do not know how to handle.\n";
                return NULL;
              }
          }
        catch (casa::AipsError x)
          {
            std::cerr << "ERROR: " << x.getMesg() << endl;
            return NULL;
          }

#else
        std::cerr << "ERROR: casacore not installed.\n";
        return NULL;
#endif
      }
    else if ( H5TYPE == filetype )
      {
        return H5data( start, length );
      }
    else
      {
        std::cerr << "ERROR: file format not recognized.\n";
        return NULL;
      }
  }

// ------------------------------------------------------- getSize

  /*!
    \brief Get the size of the column.

    Get the size of the column, in bytes.

    \return An integer describing the size, in bytes, of the column.
   */
  int dalColumn::getSize()
  {

    if ( dal_CHAR == getType() )
      {
        return sizeof( char );
      }
    else if ( dal_INT == getType() )
      {
        return sizeof( int );
      }
    else if ( dal_SHORT == getType() )
      {
        return sizeof( short );
      }
    else if ( dal_FLOAT == getType() )
      {
        return sizeof( float );
      }
    else if ( dal_DOUBLE == getType() )
      {
        return sizeof( double );
      }
    else if ( dal_COMPLEX == getType() )
      {
        return sizeof( dalcomplex );
      }
    else if ( dal_COMPLEX_SHORT == getType() )
      {
        return sizeof( dalcomplex_int16 );
      }
    else if ( dal_COMPLEX_CHAR == getType() )
      {
        return sizeof( dalcomplex_char );
      }
    else
      {
        std::cerr << "WARNING: " << getType() << " not handled. Please "
                  << "notify developers.\n";
        return 0;
      }
  }


// ------------------------------------------------------- addMember

  /*!
   \brief Add a member to a compound column.

   Add a member to a compound column.  A compound column is made up of
   several simple datatypes.  For example a compound column might be two
   integers and a floating point.

   \param member_name The name of the member you want to add.
   \param type The datatype of the column member you  want to add.
   */
  void dalColumn::addMember( std::string member_name, std::string member_type )
  {
    /*    array_tid = H5Tarray_create(H5T_NATIVE_CHAR, ARRAY_RANK,
    		    array_dim, NULL);
    */
    if ( H5TYPE == filetype )
      {
        if ( member_type == dal_CHAR )
          {
            status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_CHAR );
          }
        else if ( member_type == dal_INT )
          {
            status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_INT );
          }
        else if ( member_type == dal_UINT )
          {
            status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_UINT );
          }
        else if ( member_type == dal_SHORT )
          {
            status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_SHORT );
          }
        else if ( member_type == dal_FLOAT )
          {
            status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_FLOAT );
          }
        else if ( member_type == dal_DOUBLE )
          {
            status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_DOUBLE );
          }
        else if ( member_type == dal_STRING )
          {
            status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_C_S1 );
          }
        else
          {
            std::cerr << "ERROR: addMember " << member_name << " "
                      << member_type << " not supported.\n";
          }
      }
    else
      {
        std::cerr << "dalColumn::addMember not yet supported for filetype " <<
                  filetype << endl;
      }
  }

  // ============================================================================
  //
  //  Boost.Python wrapper functions
  //
  // ============================================================================
  
#ifdef PYTHON

  //_____________________________________________________________________________
  //                                                                  shape_boost
  
  bpl::tuple dalColumn::shape_boost()
  {
    vector<int> lclvals;
    lclvals = shape();
    bpl::list lcllist;
    
    vector<int>::iterator iter;
    for (iter=lclvals.begin(); iter < lclvals.end(); iter++) {
      lcllist.append(*iter);
    }
    
    bpl::tuple lcl_tuple;
    lcl_tuple = bpl::make_tuple(lcllist);
    
    return lcl_tuple;
  }
  
  // ------------------------------------------------------- data_boost1
  
  bpl::numeric::array dalColumn::data_boost1()
  {
    return data_boost3( 0, -1 );
  }
  
  // ------------------------------------------------------- data_boost2
  
  bpl::numeric::array dalColumn::data_boost2( int32_t length )
  {
    return data_boost3( 0, length );
    
  }
  
  // ------------------------------------------------------- data_boost3
  
  bpl::numeric::array dalColumn::data_boost3( int64_t offset, int32_t length )
  {
    if ( MSCASATYPE == filetype ) {
#ifdef HAVE_CASA
      
      if ( "unknown" == casa_datatype )
	{
	  bpl::list lcllist;
	  return num_util::makeNum(lcllist);
	}
      
      try
	{
	  if ( isScalar() )
	    {
	      switch ( casa_col_desc.dataType() )
		{
		case casa::TpInt:
                  {
                    rosc_int = new casa::ROScalarColumn<casa::Int>( *casa_column );
                    scalar_vals_int = rosc_int->getColumn();
                    data_object = new dalData( filetype, dal_INT, shape(), nrows() );
                    data_object->data = (int *)scalar_vals_int.getStorage(deleteIt);
                    return data_object->get_boost3( offset, length );
                  }
                  break;
		case casa::TpBool:
		  {
		    rosc_bool = new casa::ROScalarColumn<bool>( *casa_column );
		    scalar_vals_bool = rosc_bool->getColumn();
		    data_object = new dalData( filetype, dal_BOOL, shape(), nrows() );
		    data_object->data = (int *)scalar_vals_bool.getStorage(deleteIt);
		    return data_object->get_boost3( offset, length );
		  }
		  break;
		case casa::TpDouble:
		  {
		    rosc_dbl = new casa::ROScalarColumn<casa::Double>( *casa_column );
                    scalar_vals_dbl = rosc_dbl->getColumn();
                    data_object = new dalData( filetype, dal_DOUBLE, shape(), nrows() );
                    data_object->data = (double *)scalar_vals_dbl.getStorage(deleteIt);
                    return data_object->get_boost3( offset, length );
                  }
                  break;
		case casa::TpComplex:
                  {
                    rosc_comp = new casa::ROScalarColumn<casa::Complex>( *casa_column );
                    scalar_vals_comp = rosc_comp->getColumn();
                    data_object = new dalData( filetype, dal_COMPLEX, shape(), nrows() );
                    data_object->data =
                      (std::complex<float> *)scalar_vals_comp.getStorage(deleteIt);
                    return data_object->get_boost3( offset, length );
                  }
                  break;
		case casa::TpString:
                  {
                    rosc_string = new casa::ROScalarColumn<casa::String>( *casa_column );
                    scalar_vals_string = rosc_string->getColumn();
                    data_object = new dalData( filetype, dal_STRING, shape(), nrows() );
                    data_object->data =
                      (std::string *)scalar_vals_string.getStorage(deleteIt);
                    return data_object->get_boost3( offset, length );
                  }
                  break;
                  /************************************
                   * ADD MORE TYPES HERES
                   ************************************/
		default:
                  {
                    std::cerr << "dalColumn::data() Column type not yet supported."
                              << endl;
                    bpl::list tmp_list;
                    tmp_list.append(0);
                    bpl::numeric::array nadata(tmp_list);
                    return nadata;
                  }
		}
	    }
	  else if ( isArray() )
	    {
	      switch ( casa_col_desc.dataType() )
		{
		case casa::TpInt:
                  {
                    roac_int = new casa::ROArrayColumn<casa::Int>( *casa_column );
                    array_vals_int = roac_int->getColumn();
                    data_object = new dalData( filetype, dal_INT, shape(), nrows() );
                    data_object->data = (int *)array_vals_int.getStorage(deleteIt);
                    return data_object->get_boost3( offset, length );
                  }
                  break;
		case casa::TpDouble:
                  {
                    roac_dbl = new casa::ROArrayColumn<casa::Double>( *casa_column );
                    array_vals_dbl = roac_dbl->getColumn();
                    data_object = new dalData( filetype, dal_DOUBLE, shape(), nrows() );
                    data_object->data = (double *)array_vals_dbl.getStorage(deleteIt);
                    return data_object->get_boost3( offset, length );
                  }
                  break;
		case casa::TpComplex:
                  {
                    roac_comp = new casa::ROArrayColumn<casa::Complex>( *casa_column );
                    array_vals_comp = roac_comp->getColumn();
                    data_object = new dalData( filetype, dal_COMPLEX, shape(), nrows() );
                    data_object->data =
                      (std::complex<float> *)array_vals_comp.getStorage(deleteIt);
                    return data_object->get_boost3( offset, length );
                  }
                  break;
		case casa::TpString:
                  {
                    roac_string = new casa::ROArrayColumn<casa::String>( *casa_column );
                    array_vals_string = roac_string->getColumn();
                    data_object = new dalData( filetype, dal_STRING, shape(), nrows() );
                    data_object->data = (std::string *)array_vals_string.getStorage(deleteIt);
                    return data_object->get_boost3( offset, length );
                  }
                  break;
                  /************************************
                   * ADD MORE TYPES HERES
                   ************************************/
		default:
                  {
                    std::cerr <<
		      "dalColumn::data() Column type not yet supported.\n";
		    
                    bpl::list tmp_list;
                    tmp_list.append(0);
                    bpl::numeric::array nadata(tmp_list);
                    return nadata;
                  }
		}
	    }
	  else
	    {
	      
	      std::cerr
		<< "dalColumn::data() Column is neither scalar nor array.  "
		<< "Do not know how to handle.\n";
	      
	      bpl::list tmp_list;
	      tmp_list.append(0);
	      bpl::numeric::array nadata(tmp_list);
	      return nadata;
	    }
	}
      catch (casa::AipsError x)
	{
	  std::cerr << "ERROR: " << x.getMesg() << endl;
	  bpl::list tmp_list;
	  tmp_list.append(0);
	  bpl::numeric::array nadata(tmp_list);
	  return nadata;
	}
#endif // HAVE_CASA
    }
    else if ( H5TYPE == filetype ) {
      std::cerr << "ERROR: hdf5 not supported [dalColumn.data - python]\n";
      int start = 0;
      int length = -1;
      data_object = data(start,length);
      return data_object->get_boost3( offset, length );
    }
    else {
      std::cerr <<
	"ERROR: filetype not supported [dalColumn.data - python]\n";
    }
    
    bpl::list tmp_list;
    tmp_list.append(0);
    bpl::numeric::array nadata(tmp_list);
    return nadata;
  }
    
#endif // PYTHON
    
} // DAL namespace
