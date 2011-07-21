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

#include <core/dalColumn.h>

namespace DAL {

  // ============================================================================
  //
  //  Constructors
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                    dalColumn

  dalColumn::dalColumn()
    : dalObjectBase()
  {
    init ();
  }

  //_____________________________________________________________________________
  //                                                                    dalColumn

  /*!
    Create a new column with a complex floating point datatype.

    \param name -- Name of the column you want to create.
  */
  dalColumn::dalColumn (std::string const &name)
    : dalObjectBase(name)
  {
    init ();
    itsDatatype = dal_COMPLEX;
  }

  //_____________________________________________________________________________
  //                                                                    dalColumn

  /*!
    \param name    -- The name of the column you want to create.
    \param coltype -- The datatype of the column you want to craete (i.e.
    dalINT, dalFLOAT, dalSTRING, etc.)
  */
  dalColumn::dalColumn (std::string const &name,
			std::string const &type)
    : dalObjectBase(name)
  {
    init ();
    itsDatatype = type;
  }

  //_____________________________________________________________________________
  //                                                                    dalColumn

  /*!
    \param fileid      -- Object identifier for the file.
    \param tableid     -- Object identifier for the table.
    \param filetype    -- Type of dataset/file.
    \param tablename   -- Name of the table hosting the column.
    \param colname     -- Name of the column.
    \param coldatatype -- Type of the data stored inside the table column.
  */
  dalColumn::dalColumn (hid_t const &fileid,
                        hid_t const &tableid,
                        dalFileType const &filetype,
                        std::string const &tablename,
                        std::string const &colname,
                        std::string const &coldatatype)
    : dalObjectBase(colname,filetype)
  {
    init ();
    itsFileID    = fileid;
    itsTableID   = tableid;
    itsTablename = tablename;
    itsDatatype  = coldatatype;
  }

  //_____________________________________________________________________________
  //                                                                    dalColumn

#ifdef DAL_WITH_CASA
  /*!
    \param table   -- A casa table object.
    \param colname -- Name of the column.
  */
  dalColumn::dalColumn (casa::Table table,
			std::string colname)
    : dalObjectBase(colname, dalFileType(dalFileType::CASA_MS))
  {
    init ();
    bool error = false;
    try {
      itsROTableColumn = new casa::ROTableColumn (table, colname);
    }
    catch (casa::AipsError x) {
      std::cerr << "ERROR: " << x.getMesg() << endl;
      error = true;
    }
    
    if (!error) {
      itsColumnDesc   = itsROTableColumn->columnDesc();
      itsCasaDatatype = getDataType();
    }
    else {
      itsCasaDatatype = "unknown";
    }
  }
#endif
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init

  void dalColumn::init ()
  {
    itsTablename  = "";
    itsDatatype   = "";
    size          = 0;
    itsNofRows    = 0;
    itsFileID     = 0;
    itsTableID    = 0;
    itsNofFields  = 0;
    itsNofRecords = 0;
    coltype       = 0;
    status        = 0;
    itsColumnData = NULL;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- The output stream to which the summary is going to be written
  */
  void dalColumn::summary(std::ostream &os)
  {
    os << "\n[dalColumn] Summary of object properties"  << endl;
    os << "-- File type                = " << itsFiletype.name() << std::endl;
    os << "-- HDF5 file object ID      = " << itsFileID        << std::endl;
    os << "-- HDF5 table object ID     = " << itsTableID       << std::endl;
    os << "-- Table name               = " << itsTablename        << std::endl;
    os << "-- Column name              = " << itsName             << std::endl;
    os << "-- Column datatype          = " << itsDatatype      << std::endl;
    os << "-- Datatype size            = " << size             << std::endl;
    os << "-- nof. rows per column     = " << itsNofRows      << std::endl;
    os << "-- Is column of scalar type = " << isScalar()       << std::endl;
    os << "-- Is column of array type  = " << isArray()        << std::endl;
  }

  //_____________________________________________________________________________
  //                                                              getCasaDataType

#ifdef DAL_WITH_CASA

  /*!
    \return type -- Name of the CASA datatype.
  */
  std::string dalColumn::getCasaDataType ()
  {
    switch ( itsColumnDesc.dataType() )
      {
      case ( casa::TpDouble ):
        itsCasaDatatype = "double";
        break;
      case ( casa::TpComplex ):
        itsCasaDatatype = "complex";
        break;
      case ( casa::TpInt ):
        itsCasaDatatype = "int";
        break;
      case ( casa::TpString ):
        itsCasaDatatype = "string";
        break;
      case ( casa::TpBool ):
        itsCasaDatatype = "bool";
        break;
      default:
        itsCasaDatatype = "Unsupported type";
      }
    return itsCasaDatatype;
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
    if (itsFiletype.type() == dalFileType::CASA_MS)
      {
#ifdef DAL_WITH_CASA
        return getCasaDataType();
#else
        std::cerr << "ERROR: CASA not enabled [dalColumn::getDataType()]\n";
        return("");
#endif
      }
    else
      {
        std::cerr << "dalColumn::getDataType not yet supported for filetype " <<
	  itsFiletype.name() << endl;
        return "";
      }
  }
  
  //_____________________________________________________________________________
  //                                                                     isScalar

  /*!
    \return Non-zero if the column is scalar.
  */
  bool dalColumn::isScalar()
  {
    if (itsFiletype.type()==dalFileType::CASA_MS)
      {
#ifdef DAL_WITH_CASA
        if ( itsColumnDesc.isScalar() )
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
	  itsFiletype.name() << endl;
        return false;
      }
  }

  //_____________________________________________________________________________
  //                                                                      isArray

  /*!
    \brief Is the column an array?

    Check to see if the column is an array.

    \return Non-zero if the column is an array.
  */
  bool dalColumn::isArray()
  {
    if (itsFiletype.type()==dalFileType::CASA_MS)
      {
#ifdef DAL_WITH_CASA
        if ( itsColumnDesc.isArray() )
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
        std::cerr << "[dalColumn::isArray] Not yet supported for filetype "
		  << itsFiletype.name() << endl;
        return false;
      }
  }

  //_____________________________________________________________________________
  //                                                                        shape

  /*!
    \brief Get the shape of the column.

    Get the shape of the column.

    \return A vector of integers specifying the shape of the column.
  */
  std::vector<int> dalColumn::shape()
  {
    std::vector<int> shape_vals;
    if (itsFiletype.type()==dalFileType::CASA_MS) {
#ifdef DAL_WITH_CASA
      if ( isArray() )
	{
	  //std::cerr << itsROTableColumn->shapeColumn();
	  
	  try {
	    casa::IPosition ipos = itsROTableColumn->shape(0);
	    casa::Vector<casa::Int> col_shape = ipos.asVector();
	    col_shape.tovector( shape_vals );
	    shape_vals.push_back( nofRows() );
	    std::reverse(shape_vals.begin(), shape_vals.end());
	  }
	  catch (casa::AipsError x) {
	    std::cerr << "ERROR: " << x.getMesg() << endl;
	  }
	  return shape_vals;
	}
      else {
	shape_vals.push_back( nofRows() );
	return shape_vals;
      }
#else
      std::cerr << "ERROR: CASA not enabled [dalColumn::shape()]\n";
      return(shape_vals);
#endif
    }
    else {
      std::cerr << "dalColumn::shape not yet supported for filetype "
		<< itsFiletype.name()
		<< endl;
      return shape_vals;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                        ndims
  
  /*!
    \return ndims -- An integer specifying the number of column dimensions.
  */
  int dalColumn::ndims()
  {
    if (itsFiletype.type()==dalFileType::CASA_MS)
      {
#ifdef DAL_WITH_CASA
        if ( isArray() )
          return itsROTableColumn->ndimColumn();
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
	  itsFiletype.name() << endl;
        return -1;
      }
  }

  //_____________________________________________________________________________
  //                                                                      nofRows

  /*!
    \return rows -- A number specifying the number of column rows.
  */
  uint dalColumn::nofRows()
  {
    if (itsFiletype.type()==dalFileType::CASA_MS)
      {
#ifdef DAL_WITH_CASA
        itsNofRows = itsROTableColumn->nrow();
        return itsNofRows;
#else
        std::cerr << "ERROR: casacore not installed\n";
        return 0;
#endif
      }
    else
      {
        std::cerr << "dalColumn::nofRows() File type " << itsFiletype.name()
                  << " not yet supported.";
        return 0;
      }
  }
  
  //_____________________________________________________________________________
  //                                                                        close

  void dalColumn::close()
  {
#ifdef DAL_WITH_CASA
    delete itsROTableColumn;
#endif
  }
  
  //_____________________________________________________________________________
  //                                                              CasaData_scalar

#ifdef DAL_WITH_CASA

  dalData * dalColumn::CasaData_scalar( )
  {
    switch ( itsColumnDesc.dataType() )
      {
      case casa::TpInt:
      {
        rosc_int = new casa::ROScalarColumn<casa::Int>( *itsROTableColumn );
        casa::Vector<int> data = rosc_int->getColumn();
        itsColumnData = new dalData( itsFiletype, dal_INT, shape(), nofRows() );
        itsColumnData->data = (int *)data.getStorage(deleteIt);
        return itsColumnData;
      }
      break;
      case casa::TpDouble:
      {
        rosc_dbl = new casa::ROScalarColumn<casa::Double>( *itsROTableColumn );
	casa::Vector<double> data = rosc_dbl->getColumn();
        itsColumnData = new dalData( itsFiletype, dal_DOUBLE, shape(), nofRows() );
        itsColumnData->data = (double *)data.getStorage(deleteIt);
        return itsColumnData;
      }
      break;
      case casa::TpComplex:
      {
        rosc_comp = new casa::ROScalarColumn<casa::Complex>( *itsROTableColumn );
        casa::Vector<casa::Complex> data = rosc_comp->getColumn();
        itsColumnData = new dalData( itsFiletype, dal_COMPLEX, shape(), nofRows() );
        itsColumnData->data =
          (std::complex<float> *)data.getStorage(deleteIt);
        return itsColumnData;
      }
      break;
      case casa::TpString:
      {
        rosc_string = new casa::ROScalarColumn<casa::String>( *itsROTableColumn );
        casa::Vector<casa::String> data = rosc_string->getColumn();
        itsColumnData = new dalData( itsFiletype, dal_STRING, shape(), nofRows() );
        itsColumnData->data =
          (std::string *)data.getStorage(deleteIt);
        return itsColumnData;
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

  //_____________________________________________________________________________
  //                                                               CasaData_array

#ifdef DAL_WITH_CASA

  dalData * dalColumn::CasaData_array( )
  {
    switch ( itsColumnDesc.dataType() ) {
    case casa::TpInt:
      {
        roac_int = new casa::ROArrayColumn<casa::Int>( *itsROTableColumn );
	casa::Array<int> data = roac_int->getColumn();
        itsColumnData = new dalData (itsFiletype, dal_INT, shape(), nofRows() );
        itsColumnData->data = (int *)data.getStorage(deleteIt);
        return itsColumnData;
      }
      break;
    case casa::TpDouble:
      {
        roac_dbl = new casa::ROArrayColumn<casa::Double>( *itsROTableColumn );
        casa::Array<double> data = roac_dbl->getColumn();
        itsColumnData = new dalData (itsFiletype, dal_DOUBLE, shape(), nofRows() );
        itsColumnData->data = (double *)data.getStorage(deleteIt);
        return itsColumnData;
      }
      break;
    case casa::TpComplex:
      {
        itsDatatype = dal_COMPLEX;
	std::vector< std::complex< float > > ret_valvec;
        try {
	  roac_comp = new casa::ROArrayColumn<casa::Complex>( *itsROTableColumn );
	}
        catch (casa::AipsError x) {
	  std::cerr << "ERROR: " << x.getMesg() << endl;
	  return NULL;
	}
        casa::Array<casa::Complex> data = roac_comp->getColumn( );
        itsColumnData = new dalData( itsFiletype, dal_COMPLEX, shape(), nofRows() );
        itsColumnData->data =
          (std::complex<float> *)data.getStorage(deleteIt);
        return itsColumnData;
      }
      break;
    case casa::TpString:
      {
        roac_string = new casa::ROArrayColumn<casa::String>( *itsROTableColumn );
        casa::Array<casa::String> data = roac_string->getColumn();
        itsColumnData = new dalData( itsFiletype, dal_STRING, shape(), nofRows() );
        itsColumnData->data = (std::string *)data.getStorage(deleteIt);
        return itsColumnData;
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
  
  //_____________________________________________________________________________
  //                                                                       H5data

  dalData * dalColumn::H5data (int &start,
                               int &length)
  {
    char  ** field_names;
    size_t * field_sizes   = NULL;
    size_t * field_offsets = NULL;
    size_t * size_out      = NULL;
    bool column_in_table   = false;
    
    // retrieve the input fields needed for the append_records call
    if ( H5TBget_table_info ( itsFileID, itsTablename.c_str(), &itsNofFields, &itsNofRecords )
         < 0 )
      return NULL;

    field_sizes = (size_t*)malloc( itsNofFields * sizeof(size_t) );
    field_offsets = (size_t*)malloc( itsNofFields * sizeof(size_t) );
    size_out = (size_t*)malloc( sizeof(size_t) );

    /* Alocate space */
    field_names = (char**)malloc( sizeof(char*) * (size_t)itsNofFields );
    for ( hsize_t ii = 0; ii < itsNofFields; ii++)
      field_names[ii] = (char*)malloc( sizeof(char) * MAX_COL_NAME_SIZE );

    if ( H5TBget_field_info( itsFileID, itsTablename.c_str(), field_names,
                             field_sizes, field_offsets, size_out ) < 0 ) {
      return NULL;
    }

    for ( hsize_t ii = 0; ii < itsNofFields; ii++) {
      if ( 0 == strcmp( field_names[ii], itsName.c_str() ) ) {
	column_in_table = true;
      }
    }
    
    /* Release memory allocated for the columns of the table */
    for ( hsize_t ii = 0; ii < itsNofFields; ii++) {
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
      length = itsNofRecords;
    }

    if ( dal_COMPLEX_SHORT == getType() )
      {
        DAL::Complex_Int16 * data = NULL;
        try
          {
            data = (DAL::Complex_Int16*)malloc(sizeof(DAL::Complex_Int16)*length);
          }
        catch ( std::bad_alloc ) {
	  std::cerr <<
	    "ERROR: Could not allocate memory buffer for dalColumn\n";
	  return NULL;
	}
	
        if ( H5TBread_fields_name (itsFileID,
				   itsTablename.c_str(),
				   itsName.c_str(),
				   start,
				   length,
				   sizeof(DAL::Complex_Int16),
				   field_offsets,
				   field_sizes,
				   data ) < 0 )
          {
            std::cerr << "ERROR: H5TBread_fields_name failed.\n";
            return NULL;
          }
	
        std::vector<int> shape(1);
	
        itsColumnData = new dalData( itsFiletype,
				     dal_COMPLEX_SHORT,
				     shape,
				     length);
        itsColumnData->data = (DAL::Complex_Int16 *)data;
      }
    else if ( dal_FLOAT == getType() ) {
      float * data = NULL;
      try {
	data = (float*)malloc(sizeof(float)*length);
      }
      catch ( std::bad_alloc ) {
	std::cerr << "ERROR: Could not allocate memory buffer for " <<
	  "dalColumn\n";
	return NULL;
      }
      
      if ( H5TBread_fields_name (itsFileID,
				 itsTablename.c_str(),
				 itsName.c_str(),
				 start,
				 length,
				 sizeof(float),
				 field_offsets,
				 field_sizes,
				 data ) < 0 )
	{
	  std::cerr << "ERROR: H5TBread_fields_name failed.\n";
	  return NULL;
	}
      
      std::vector<int> shape(1);
      
      itsColumnData = new dalData (itsFiletype, dal_FLOAT, shape, length );
      itsColumnData->data = (float *)data;
    }
    else {
      std::cerr << "ERROR: datatype not supported [dalColumn.data]\n";
      free( field_sizes );
      free( field_offsets );
      free( size_out );
      return NULL;
    }

    // Release allocated memory
    free( field_sizes );
    free( field_offsets );
    free( size_out );
    
    return itsColumnData;
    
  }
  
  //_____________________________________________________________________________
  //                                                                         data

  /*!
    \brief Get column data.

    Get the data object for the column.

    \param start Start cell number for column.
    \param length Number of cells to retrieve into the data object.
    \return A dalData object containing the column data.
   */
  
  //_____________________________________________________________________________
  //                                                                      getSize
  
  /*!
    \brief Get the size of the column.
    \return An integer describing the size, in bytes, of the column.
   */
  int dalColumn::getSize()
  {

    if ( dal_CHAR == getType() ) {
      return sizeof( char );
    }
    else if ( dal_INT == getType() ) {
      return sizeof( int );
    }
    else if ( dal_SHORT == getType() ) {
      return sizeof( short );
    }
    else if ( dal_FLOAT == getType() ) {
      return sizeof( float );
    }
    else if ( dal_DOUBLE == getType() ) {
      return sizeof( double );
    }
    else if ( dal_COMPLEX == getType() ) {
      return sizeof( DAL::Complex_Double );
    }
    else if ( dal_COMPLEX_SHORT == getType() ) {
        return sizeof( DAL::Complex_Int16 );
      }
    else if ( dal_COMPLEX_CHAR == getType() ) {
      return sizeof( DAL::Complex_Char );
    }
    else {
      std::cerr << "WARNING: " << getType() << " not handled. Please "
		<< "notify developers.\n";
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                    addMember
  
  /*!
    Add a member to a compound column.  A compound column is made up of
    several simple datatypes.  For example a compound column might be two
    integers and a floating point.
    
    \param member_name The name of the member you want to add.
    \param type The datatype of the column member you  want to add.
  */
  void dalColumn::addMember (std::string const &member_name,
			     std::string const &member_type)
  {
    /*    array_tid = H5Tarray_create(H5T_NATIVE_CHAR, ARRAY_RANK,
	  array_dim, NULL);
    */
    if (itsFiletype.type()==dalFileType::HDF5)
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
    else {
      std::cerr << "dalColumn::addMember not yet supported for filetype "
		<< itsFiletype.name() << endl;
    }
  }
  
} // DAL namespace
