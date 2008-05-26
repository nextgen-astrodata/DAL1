/*-------------------------------------------------------------------------*
| $Id:: cs1.h 389 2007-06-13 08:47:09Z baehren                          $ |
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

/**
 * \file dalColumn.cpp
 * \author Joseph Masters
 * \date 12-11-06
 */
 
#ifndef DALCOLUMN_H
#include "dalColumn.h"
#endif

dalColumn::dalColumn()
{}

dalColumn::dalColumn( hid_t fileid,
                      hid_t tableid,
                      string lcl_filetype,
                      string lcl_tablename,
                      string colname,
                      string coldatatype )
{
  file_id = fileid;
  table_id = tableid;
  filetype = lcl_filetype;
  tablename = lcl_tablename;
  name = colname;
  dal_datatype = coldatatype;
}

dalColumn::dalColumn( string colname, string type )
{
	name = colname;
	dal_datatype = type;
}


dalColumn::dalColumn( string complexcolname )
{
	name = complexcolname;
	dal_datatype = DAL::dal_COMPLEX;
}

#ifdef WITH_CASA
dalColumn::dalColumn(casa::Table table, string colname)
{
    filetype = DAL::MSCASATYPE;
    try
    {
       casa_column = new casa::ROTableColumn( table, colname );
    } catch (casa::AipsError x) {
       cout << "ERROR: " << x.getMesg() << endl;
       exit(-4);
    }
    casa_col_desc = casa_column->columnDesc();
    casa_datatype = getDataType();
}
#endif

void dalColumn::close()
{
#ifdef WITH_CASA
	delete casa_column;
#endif
}

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
string dalColumn::getDataType()
{
  if ( DAL::MSCASATYPE == filetype )
  {
#ifdef WITH_CASA
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
    default:
      casa_datatype = "Unsupported type";
  }
  return casa_datatype;
#else
  cerr << "ERROR: CASA not enabled [dalColumn::getDataType()]" << endl;
  return("");
#endif
  }
  else
  {
    cout << "dalColumn::getDataType not yet supported for filetype " <<
	  filetype << endl;
	return "";
  }
}

int dalColumn::isScalar()
{
  if ( DAL::MSCASATYPE == filetype )
  {
#ifdef WITH_CASA
   if ( casa_col_desc.isScalar() )
      return 1;
   else
      return 0;
#else
  cerr << "ERROR: CASA not enabled [dalColumn::isScalar()]" << endl;
  return(0);
#endif
  }
  else
  {
    cout << "dalColumn::isScalar not yet supported for filetype " <<
	  filetype << endl;
	return -1;
  }
}

int dalColumn::isArray()
{
  if ( DAL::MSCASATYPE == filetype )
  {
#ifdef WITH_CASA
   if ( casa_col_desc.isArray() )
      return 1;
   else
      return 0;
#else
  cerr << "ERROR: CASA not enabled [dalColumn::isArray()]" << endl;
  return(0);
#endif
  }
  else
  {
    cout << "dalColumn::isArray not yet supported for filetype " <<
	  filetype << endl;
	return -1;
  }
}

vector<int> dalColumn::shape()
{
  vector<int> shape_vals;
  if ( DAL::MSCASATYPE == filetype )
  {
#ifdef WITH_CASA
    if ( isArray() )
    {
      //cout << casa_column->shapeColumn();

      try
      {
	    casa::IPosition ipos = casa_column->shape(0);
	    casa::Vector<casa::Int> col_shape = ipos.asVector();
	    col_shape.tovector( shape_vals );
	    shape_vals.push_back( nrows() );
	    std::reverse(shape_vals.begin(), shape_vals.end());
      } catch (casa::AipsError x) {
         cout << "ERROR: " << x.getMesg() << endl;
         exit(-4);
      }
      return shape_vals;
    }
    else
    {
      shape_vals.push_back( nrows() );
      return shape_vals;
    }
#else
  cerr << "ERROR: CASA not enabled [dalColumn::shape()]" << endl;
  return(shape_vals);
#endif
  }
  else
  {
    cout << "dalColumn::shape not yet supported for filetype " <<
	  filetype << endl;
	return shape_vals;
  }
}

unsigned int dalColumn::ndims()
{
  if ( DAL::MSCASATYPE == filetype )
  {
#ifdef WITH_CASA
    if ( isArray() )
     return casa_column->ndimColumn();
    else
    {
	  return 0;
    }
#else
  cerr << "ERROR: CASA not used [dalColumn::ndims]" << endl;
  return(-1);
#endif
  }
  else
  {
    cout << "dalColumn::ndims not yet supported for filetype " <<
	  filetype << endl;
	return -1;
  }
}

void dalColumn::open( std::string colname )
{
  cerr << "dalColumn::open( " << colname << " )" << endl;
}

void dalColumn::setName(string colname)
{
  name = colname;
}

void dalColumn::setFileType( string type )
{
  filetype = type;
}

string dalColumn::getName()
{
  return name;
}

unsigned int dalColumn::nrows()
{
  if ( DAL::MSCASATYPE == filetype )
  {
#ifdef WITH_CASA
    num_of_rows = casa_column->nrow();
	return num_of_rows;
#else
    cerr << "ERROR: casacore not installed" << endl;
	return(0);
#endif
  }
  else
  {
    cout << "dalColumn::nrows() File type " << filetype
	  << " not yet supported."; 
    return 0;
  }
}

string dalColumn::getType()
{
  return dal_datatype;
}

dalData * dalColumn::data()
{
  int start = -1;
  int length = -1;
  return data( start, length );
}

dalData * dalColumn::data( int start, int &length )
{
  if ( DAL::MSCASATYPE == filetype )
  {
#ifdef WITH_CASA
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
	   data_object = new dalData( filetype, DAL::dal_INT, shape(), nrows() );
	   data_object->data = (int *)scalar_vals_int.getStorage(deleteIt);
	   return data_object;
         }
	 break;
	 case casa::TpDouble:
	 {
	   rosc_dbl = new casa::ROScalarColumn<casa::Double>( *casa_column );
	   scalar_vals_dbl = rosc_dbl->getColumn();
	   data_object = new dalData( filetype, DAL::dal_DOUBLE, shape(), nrows() );
	   data_object->data = (double *)scalar_vals_dbl.getStorage(deleteIt);
	   return data_object;
         }
	 break;
	 case casa::TpComplex:
	 {
	   rosc_comp = new casa::ROScalarColumn<casa::Complex>( *casa_column );
	   scalar_vals_comp = rosc_comp->getColumn();
	   data_object = new dalData( filetype, DAL::dal_COMPLEX, shape(), nrows() );
	   data_object->data =
	     (complex<float> *)scalar_vals_comp.getStorage(deleteIt);
	   return data_object;
         }
	 break;
	 case casa::TpString:
	 {
	   rosc_string = new casa::ROScalarColumn<casa::String>( *casa_column );
	   scalar_vals_string = rosc_string->getColumn();
	   data_object = new dalData( filetype, DAL::dal_STRING, shape(), nrows() );
	   data_object->data =
	     (string *)scalar_vals_string.getStorage(deleteIt);
	   return data_object;
         }
	 break;
/************************************
 * ADD MORE TYPES HERES
 ************************************/
	 default:
	 {
	   cout << "dalColumn::data() Column type not yet supported."
	     << endl;
           return NULL;
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
	    data_object = new dalData( filetype, DAL::dal_INT, shape(), nrows() );
	    data_object->data = (int *)array_vals_int.getStorage(deleteIt);
	    return data_object;
	  }
	  break;
	  case casa::TpDouble:
	  {
	    roac_dbl = new casa::ROArrayColumn<casa::Double>( *casa_column );
            array_vals_dbl = roac_dbl->getColumn();
	    data_object = new dalData( filetype, DAL::dal_DOUBLE, shape(), nrows() );
	    data_object->data = (double *)array_vals_dbl.getStorage(deleteIt);
	    return data_object;
	  }
	  break;
	  case casa::TpComplex:
	  {
// 	    casa::IPosition start (2,cell1,cell2);
// 	    casa::Slicer slicer (start);
	    dal_datatype = DAL::dal_COMPLEX;
	    vector< complex< float > > ret_valvec;
	    try
	    {
	       roac_comp = new casa::ROArrayColumn<casa::Complex>( *casa_column );
            } catch (casa::AipsError x) {
               cout << "ERROR: " << x.getMesg() << endl;
	       exit(-4);
            }
            array_vals_comp = roac_comp->getColumn(/*slicer*/);
	    data_object = new dalData( filetype, DAL::dal_COMPLEX, shape(), nrows() );
	    data_object->data =
		  (complex<float> *)array_vals_comp.getStorage(deleteIt);
	    return data_object;

	  }
	  break;
	  case casa::TpString:
	  {
	    roac_string = new casa::ROArrayColumn<casa::String>( *casa_column );
	    array_vals_string = roac_string->getColumn();
	    data_object = new dalData( filetype, DAL::dal_STRING, shape(), nrows() );
	    data_object->data = (string *)array_vals_string.getStorage(deleteIt);
	    return data_object;
	  }
	  break;
/************************************
 * ADD MORE TYPES HERES
 ************************************/
	    default:
	    {
		cout << "dalColumn::data() Column type not yet supported."
		  << endl;
                return NULL;
            }
           }	
       }
       else
       {
	 cout << "dalColumn::data() Column is neither scalar nor array.  "
	   << "Do not know how to handle." << endl;
         return NULL;
       }
    } catch (casa::AipsError x) {
       cout << "ERROR: " << x.getMesg() << endl;
       exit(-4);
    }
#endif
   }

   else if ( DAL::H5TYPE == filetype )
   {
	char  ** field_names;
	size_t * field_sizes = NULL;
	size_t * field_offsets = NULL;
	size_t * size_out = NULL;
        bool column_in_table = false;
	
	// retrieve the input fields needed for the append_records call
	if ( H5TBget_table_info ( file_id, tablename.c_str(), &nfields, &nrecords )
	  < 0 )
	   return NULL;
	
	field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
	field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
	size_out = (size_t*)malloc( sizeof(size_t) );

        /* Alocate space */
        field_names = (char**)malloc( sizeof(char*) * (size_t)nfields );
        for ( hsize_t ii = 0; ii < nfields; ii++)
          field_names[ii] = (char*)malloc( sizeof(char) * HLTB_MAX_FIELD_LEN );

	if ( H5TBget_field_info( file_id, tablename.c_str(), field_names,
             field_sizes, field_offsets, size_out ) < 0 )
	  return NULL;

        for ( hsize_t ii = 0; ii < nfields; ii++)
        {
          if ( 0 == strcmp( field_names[ii], name.c_str() ) )
            column_in_table = true;
        }

        for ( hsize_t ii = 0; ii < nfields; ii++)
          free( field_names[ii] );

        free( field_names );
	

        if ( !column_in_table )  // if column isn't found in the table
          return NULL;

        if ( start < 0 )
	  start = 0;
	if ( length < 0 )
	  length = nrecords;

	if ( DAL::dal_COMPLEX_SHORT == getType() )
        {
		DAL::dalcomplex_int16 * data = NULL;
		try
		{
		  data = (DAL::dalcomplex_int16*)malloc(sizeof(DAL::dalcomplex_int16)*length);
		}
		catch ( bad_alloc )
		{
			cerr << "Could not allocate memory buffer for dalColumn" << endl;
			exit(3);
		}

		if ( H5TBread_fields_name (file_id, tablename.c_str(), name.c_str(),
		  start, length, sizeof(DAL::dalcomplex_int16), field_offsets, field_sizes,
		  data ) < 0 )
			return NULL;

		vector<int> shape(1);
		
		data_object = new dalData( filetype, DAL::dal_COMPLEX_SHORT, shape, length );
		data_object->data = (DAL::dalcomplex_int16 *)data;
	}
	else if ( DAL::dal_FLOAT == getType() )
        {
		float * data = NULL;
		try
		{
		  data = (float*)malloc(sizeof(float)*length);
		}
		catch ( bad_alloc )
		{
			cerr << "Could not allocate memory buffer for dalColumn" << endl;
			exit(3);
		}
		
		if ( H5TBread_fields_name (file_id, tablename.c_str(), name.c_str(),
				start, length, sizeof(float), field_offsets, field_sizes,
				data ) < 0 )
			return NULL;
		
		vector<int> shape(1);
		
		data_object = new dalData( filetype, DAL::dal_FLOAT, shape, length );
		data_object->data = (float *)data;
	}
        else
        {
           cerr << "ERROR: datatype not supported [dalColumn.data]" << endl;
        }

	free( field_sizes );
	free( field_offsets );
	free( size_out );

	return data_object;

   }

   return NULL;
}

int dalColumn::getSize()
{
	if ( DAL::dal_CHAR== getType() ) {
		return sizeof( char );
	}
	else if ( DAL::dal_INT == getType() ) {
		return sizeof( int );
	}
	else if ( DAL::dal_SHORT == getType() ) {
		return sizeof( short );
	}
	else if ( DAL::dal_FLOAT == getType() ) {
		return sizeof( float );
	}
	else if ( DAL::dal_DOUBLE == getType() ) {
		return sizeof( double );
	}
	else if ( DAL::dal_COMPLEX == getType() ) {
		return sizeof( DAL::dalcomplex );
	}
	else if ( DAL::dal_COMPLEX_SHORT == getType() ) {
		return sizeof( DAL::dalcomplex_int16 );
	}
	else if ( DAL::dal_COMPLEX_CHAR == getType() ) {
		return sizeof( DAL::dalcomplex_char );
	}
	else
	{
	  cerr << getType() << endl;
	  return -1;
	}
}

void dalColumn::addMember( string member_name, string member_type )
{
/*    array_tid = H5Tarray_create(H5T_NATIVE_CHAR, ARRAY_RANK,
		    array_dim, NULL);
*/
  if ( DAL::H5TYPE == filetype )
  {
    if ( member_type == DAL::dal_CHAR )	{
      status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_CHAR );
    }
    else if ( member_type == DAL::dal_INT )	{
      status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_INT );
    }
    else if ( member_type == DAL::dal_UINT )	{
      status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_UINT );
    }
    else if ( member_type == DAL::dal_SHORT )	{
      status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_SHORT );
    }
    else if ( member_type == DAL::dal_FLOAT )	{
      status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_FLOAT );
    }
    else if ( member_type == DAL::dal_DOUBLE )	{
      status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_DOUBLE );
    }
    else if ( member_type == DAL::dal_STRING )	{
      status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_C_S1 );
    }
    else {					     
      cout << "ERROR: addMember " << member_name << " "
        << member_type << " not supported." << endl;
    }
  }
  else
  {
     cout << "dalColumn::addMember not yet supported for filetype " <<
	   filetype << endl;
  }
}

/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
#ifdef PYTHON
bpl::tuple dalColumn::shape_boost()
{
  vector<int> lclvals;
  lclvals = shape();
  bpl::list lcllist;

  vector<int>::iterator iter;
  for (iter=lclvals.begin(); iter < lclvals.end(); iter++)
  {
    lcllist.append(*iter);
  }

   bpl::tuple lcl_tuple;
   lcl_tuple = bpl::make_tuple(lcllist);

   return lcl_tuple;
}

bpl::numeric::array dalColumn::data_boost()
{
  if ( DAL::MSCASATYPE == filetype )
  {
#ifdef WITH_CASA
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
	   data_object = new dalData( filetype, DAL::dal_INT, shape(), nrows() );
	   data_object->data = (int *)scalar_vals_int.getStorage(deleteIt);
	   return data_object->get_boost();
         }
	 break;
	 case casa::TpDouble:
	 {
	   rosc_dbl = new casa::ROScalarColumn<casa::Double>( *casa_column );
	   scalar_vals_dbl = rosc_dbl->getColumn();
	   data_object = new dalData( filetype, DAL::dal_DOUBLE, shape(), nrows() );
	   data_object->data = (double *)scalar_vals_dbl.getStorage(deleteIt);
	   return data_object->get_boost();
         }
	 break;
	 case casa::TpComplex:
	 {
	   rosc_comp = new casa::ROScalarColumn<casa::Complex>( *casa_column );
	   scalar_vals_comp = rosc_comp->getColumn();
	   data_object = new dalData( filetype, DAL::dal_COMPLEX, shape(), nrows() );
	   data_object->data =
	     (complex<float> *)scalar_vals_comp.getStorage(deleteIt);
	   return data_object->get_boost();
         }
	 break;
	 case casa::TpString:
	 {
	   rosc_string = new casa::ROScalarColumn<casa::String>( *casa_column );
	   scalar_vals_string = rosc_string->getColumn();
	   data_object = new dalData( filetype, DAL::dal_STRING, shape(), nrows() );
	   data_object->data =
	     (string *)scalar_vals_string.getStorage(deleteIt);
	   return data_object->get_boost();
         }
	 break;
/************************************
 * ADD MORE TYPES HERES
 ************************************/
	 default:
	 {
	   cout << "dalColumn::data() Column type not yet supported."
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
	    data_object = new dalData( filetype, DAL::dal_INT, shape(), nrows() );
	    data_object->data = (int *)array_vals_int.getStorage(deleteIt);
	    return data_object->get_boost();
	    }
	    break;
	  case casa::TpDouble:
	  {
	    roac_dbl = new casa::ROArrayColumn<casa::Double>( *casa_column );
            array_vals_dbl = roac_dbl->getColumn();
	    data_object = new dalData( filetype, DAL::dal_DOUBLE, shape(), nrows() );
	    data_object->data = (double *)array_vals_dbl.getStorage(deleteIt);
	    return data_object->get_boost();
	    }
	    break;
	  case casa::TpComplex:
	  {
// 	    casa::IPosition start (2,cell1,cell2);
// 	    casa::Slicer slicer (start);
	    dal_datatype = DAL::dal_COMPLEX;
	    vector< complex< float > > ret_valvec;
	    try
	    {
	       roac_comp = new casa::ROArrayColumn<casa::Complex>( *casa_column );
            } catch (casa::AipsError x) {
               cout << "ERROR: " << x.getMesg() << endl;
	       exit(-4);
            }
            array_vals_comp = roac_comp->getColumn(/*slicer*/);
	    data_object = new dalData( filetype, DAL::dal_COMPLEX, shape(), nrows() );
	    data_object->data =
		  (complex<float> *)array_vals_comp.getStorage(deleteIt);
	    return data_object->get_boost();

	    }
	    break;
	  case casa::TpString:
	  {
	    roac_string = new casa::ROArrayColumn<casa::String>( *casa_column );
            array_vals_string = roac_string->getColumn();
	    data_object = new dalData( filetype, DAL::dal_STRING, shape(), nrows() );
	    data_object->data = (string *)array_vals_string.getStorage(deleteIt);
	    return data_object->get_boost();
	    }
	    break;
/************************************
 * ADD MORE TYPES HERES
 ************************************/
	    default:
	    {
		cout << "dalColumn::data() Column type not yet supported."
		  << endl;
                bpl::list tmp_list;
                tmp_list.append(0);
                bpl::numeric::array nadata(tmp_list);
                return nadata;
            }
           }	
       }
       else
       {
	 cout << "dalColumn::data() Column is neither scalar nor array.  "
	   << "Do not know how to handle." << endl;
         bpl::list tmp_list;
         tmp_list.append(0);
         bpl::numeric::array nadata(tmp_list);
         return nadata;
       }
    } catch (casa::AipsError x) {
       cout << "ERROR: " << x.getMesg() << endl;
       exit(-4);
    }
#endif // WITH_CASA
   }
   else if ( DAL::H5TYPE == filetype )
   {
     cerr << "ERROR: hdf5 not supported [dalColumn.data - python]" << endl;
     int start = 0;
     int length = -1;
     data_object = data(start,length);
     return data_object->get_boost();
   }
   else
   {
     cerr << "ERROR: filetype not supported [dalColumn.data - python]" << endl;
   }

   bpl::list tmp_list;
   tmp_list.append(0);
   bpl::numeric::array nadata(tmp_list);
   return nadata;
}

#endif
