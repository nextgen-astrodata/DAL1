/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

/*!
  \file pydal_core_dalTable.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::dalTable class

  \author Lars B&auml;hren
*/

#include "pydal.h"
#include <core/dalTable.h>

using DAL::dalTable;

// ==============================================================================
//
//                                                     Additional Python wrappers
//
// ==============================================================================

//_____________________________________________________________________________
//                                                             append_row_boost

//! Wrapper for appendRow (hdf5)
bool dalTable::append_row_boost( boost::python::object data )
{
#ifdef DAL_DEBUGGING_MESSAGES
  printf("list size: %d\n",PyList_Size( data.ptr() ) );
#endif
  return append_rows_boost(data,1);
}

//_____________________________________________________________________________
//                                                            append_rows_boost

//! Wrapper for appendRows (hdf5)
bool dalTable::append_rows_boost (boost::python::object data,
				  long nrows)
{
  long list_length = boost::python::len(data);
  PyObject* list_item;
  PyTypeObject* pytype;
  const char * type_name;
  char * buf;
  size_t totalsize = 0;
  size_t offset = 0;
  buf = (char*) malloc ( 1 );
  int int_value;
  float  float_value;
  double double_value;
  short  short_value;
  long   long_value;
  //     char*  char_value;
  
  for (int idx=0; idx<list_length; idx++)
    {
      list_item = PyList_GetItem(data.ptr(), idx);
      pytype = list_item->ob_type;
      type_name = pytype->tp_name;
      if ( 0 == strcmp( type_name, "int") )
	{
	  offset = totalsize;
	  totalsize += sizeof(int);
	  buf = (char*) realloc ( buf, totalsize );
	  int_value = boost::python::extract<int>(data[idx]);
	  memcpy ( (int*)(buf+offset), &int_value, sizeof(int) );
	}
      else if ( 0 == strcmp( type_name, "float") )
	{
	  offset = totalsize;
	  totalsize += sizeof(float);
	  buf = (char*) realloc ( buf, totalsize );
	  float_value = boost::python::extract<float>(data[idx]);
	  memcpy ( (float*)(buf+offset), &float_value, sizeof(float) );
	}
      else if ( 0 == strcmp( type_name, "short") )
	{
	  offset = totalsize;
	  totalsize += sizeof(short);
	  buf = (char*) realloc ( buf, totalsize );
	  short_value = boost::python::extract<short>(data[idx]);
	  memcpy ( (short*)(buf+offset), &short_value, sizeof(short) );
	}
      else if ( 0 == strcmp( type_name, "double") )
	{
	  offset = totalsize;
	  totalsize += sizeof(double);
	  buf = (char*) realloc ( buf, totalsize );
	  double_value = boost::python::extract<double>(data[idx]);
	  memcpy ( (double*)(buf+offset), &double_value, sizeof(double) );
	}
      else if ( 0 == strcmp( type_name, "long") )
	{
	  offset = totalsize;
	  totalsize += sizeof(long);
	  buf = (char*) realloc ( buf, totalsize );
	  long_value = boost::python::extract<long>(data[idx]);
	  memcpy ( (long*)(buf+offset), &long_value, sizeof(long) );
	}
      //else if ( 0 == strcmp( type_name, "char") )
      //{
      //    offset = totalsize;
      //    totalsize += sizeof(char);
      //    buf = (char*) realloc ( buf, totalsize );
      //    char_value = boost::python::extract<char>(data[idx]);
      //    memcpy ( (char*)(buf+offset), &char_value, sizeof(char) );
      //}
      else
	{
          cerr << "datatype not supported by python appendRows!" << endl;
          cerr << "  it is a " << type_name << endl;
	}
    }
  appendRows(buf, nrows);
  free(buf);
  return PyList_Check(data.ptr());
}

//_____________________________________________________________________________
//                                                               readRows_boost

PyObject* dalTable::readRows_boost (int start,
				    int nrecs )
{
  PyObject* py_list = PyList_New( 0 );
  PyObject *py_item;
  void * value;
  if (itsFiletype.type()==DAL::dalFileType::HDF5) {
    char * data_out;
    data_out = (char*) malloc ( 1 );
    
    size_t * field_sizes   = NULL;
    size_t * field_offsets = NULL;
    size_t * size_out      = NULL;
    
    // retrieve the input fields needed for the append_records call
    H5TBget_table_info ( itsFileID, itsName.c_str(), &nfields, &nofRecords_p );
    
    field_sizes = (size_t*)malloc( nfields * sizeof(size_t) );
    field_offsets = (size_t*)malloc( nfields * sizeof(size_t) );
    size_out = (size_t*)malloc( sizeof(size_t) );
    itsFieldNames = (char**)malloc( nfields * sizeof(char*) );
    for (unsigned int ii=0; ii<nfields; ii++) {
      itsFieldNames[ii] = (char*)malloc(MAX_COL_NAME_SIZE*sizeof(char));
    }

    /* Retrieve information about table field */
    status = H5TBget_field_info (itsFileID,
				 itsName.c_str(),
				 itsFieldNames,
				 field_sizes,
				 field_offsets,
				 size_out);
    /* Allocate memory to retrieve table entries */
    data_out = (char*) realloc (data_out,
				(*size_out)*nrecs);
    /* Read record entries from the table */
    status = H5TBread_records( itsFileID,
			       itsName.c_str(),
			       start,
			       nrecs,
			       size_out[0],
			       field_offsets,
			       field_sizes,
			       data_out);
    
    for (int rec_idx=0; rec_idx<nrecs; rec_idx++) {
      for (unsigned int field_idx=0; field_idx<nfields; field_idx++) {
	hid_t table_type_id = H5Dget_type( itsTableID );
	H5T_class_t field_type = H5Tget_member_class( table_type_id, field_idx );
	
	switch( field_type ) {
	case H5T_FLOAT:
	  {
	    value = (float*)(data_out+field_offsets[field_idx]);
	    py_item = PyFloat_FromDouble(*((float*)value));
	    if (0 != PyList_Append( py_list, py_item ) )
	      cerr << "Could not append item to list in readRows_boost." << endl;
	    break;
	  }
	case H5T_INTEGER:
	  {
	    hid_t member_type = H5Tget_member_type (table_type_id, field_idx);
	    hid_t native_type = H5Tget_native_type (member_type, H5T_DIR_ASCEND);
	    if  ( H5Tequal( native_type, H5T_NATIVE_CHAR ) > 0 ) {
	      cerr << "native char" << endl;
	    }
	    else if  ( H5Tequal( native_type, H5T_NATIVE_SHORT ) > 0 ) {
	      value = (short*)(data_out+field_offsets[field_idx]);
	      py_item = PyInt_FromSsize_t( *((short*)value) );
	      if (0 != PyList_Append( py_list, py_item ) )
		cerr << "Could not append item to list in readRows_boost." << endl;
	    }
	    else if  ( H5Tequal( native_type, H5T_NATIVE_INT ) > 0 ) {
	      value = (int*)(data_out+field_offsets[field_idx]);
	      py_item = PyInt_FromSsize_t( *((int*)value) );
	      if (0 != PyList_Append( py_list, py_item ) )
		cerr << "Could not append item to list in readRows_boost." << endl;
	    }
	    else {
	      cerr << "member type not recognized in readRows_boost" << endl;
	    }
	    
	    if ( H5Tclose( member_type  ) < 0 ) {
	      cerr << "could not close member_type in readRows_boost" << endl;
	    }
	    if ( H5Tclose( native_type  ) < 0 ) {
	      cerr << "could not close native_type in readRows_boost" << endl;
	    }
	    
	    break;
	  }
	default:
	  cerr << "field type is unknown in readRows_boost" << endl;
	}
	if ( H5Tclose( table_type_id  ) < 0 ) {
	  cerr << "could not close table type in readRows_boost" << endl;
	}
      }
    }
    
    free(field_sizes);
    free(field_offsets);
    free(size_out);
    for (unsigned int ii=0; ii<nfields; ii++) {
      free(itsFieldNames[ii]);
    }
    free(itsFieldNames);
    if (status < 0) {
      std::cerr << "ERROR: Problem reading records. Row buffer may be too big."
		<< " Make sure the buffer is smaller than the size of the "
		<< "table." << endl;
    }
  }
  else {
    std::cerr << "Operation not yet supported for type " << itsFiletype.name()
	      << ".  Sorry.\n";
  }
  
  return py_list;
}

//_____________________________________________________________________________
//                                                     write_col_by_index_boost

void dalTable::write_col_by_index_boost (boost::python::numeric::array data,
					 int index,
					 int rownum,
					 long nrecords )
{
  void * mydata = num_util::data(data);
  writeDataByColNum( mydata, index, rownum, nrecords );
}

//  bool dalTable::append_row_boost( boost::python::numeric::array data )
//  {
//  	void * mydata = num_util::data(data);
//    appendRow(mydata);
//    return true;
//  }

//_____________________________________________________________________________
//                                                            listColumns_boost

boost::python::list dalTable::listColumns_boost()
{
  std::vector<std::string> cols = listColumns();
  std::vector<int> mydims;
  mydims.push_back( cols.size() );
  boost::python::list mylist;
  for ( uint idx=0; idx < cols.size(); idx++ )
    mylist.append( cols[ idx ].c_str() );
  return mylist;
}


bool dalTable::setAttribute_char( std::string attrname, char data )
{
  return setAttribute( attrname, &data );
}
bool dalTable::setAttribute_string( std::string attrname, std::string data )
{
  return setAttribute( attrname, &data );
}

#ifdef DAL_WITH_CASA

void dalTable::ot_nonMStable( std::string const &tablename )
{
  openTable( tablename );
}

void dalTable::setFilter_boost1( std::string columns )
{
  setFilter( columns );
}

void dalTable::setFilter_boost2( std::string columns, std::string conditions )
{
  setFilter( columns, conditions );
}

#endif // DAL_WITH_CASA

bool dalTable::setAttribute_char_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<char> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<char>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<char*>(&mydata[0]), size );
}
bool dalTable::setAttribute_short_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<short> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<short>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<short*>(&mydata[0]), size );
}
bool dalTable::setAttribute_int_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<int> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<int>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<int*>(&mydata[0]), size );
}
bool dalTable::setAttribute_uint_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<uint> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<uint>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<uint*>(&mydata[0]), size );
}
bool dalTable::setAttribute_long_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<long> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<long>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<long*>(&mydata[0]), size );
}
bool dalTable::setAttribute_float_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<float> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<float>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<float*>(&mydata[0]), size );
}
bool dalTable::setAttribute_double_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<double> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<double>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<double*>(&mydata[0]), size );
}
bool dalTable::setAttribute_string_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<std::string> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<std::string>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<std::string*>(&mydata[0]), size );
}

//_____________________________________________________________________________
//                                                           getAttribute_boost

boost::python::numeric::array dalTable::getAttribute_boost(std::string attrname)
{
  std::vector<int> mydims;
  mydims.push_back(1);
  float * dta = (float*)getAttribute(attrname);
  return num_util::makeNum(((float*)dta),mydims);
}

// ==============================================================================
//
//                                                      Wrapper for class methods
//
// ==============================================================================

void export_dalTable ()
{
  //________________________________________________________
  // Specialisation of overloaded methods
  
  void (dalTable::*openTableHDF5)(void * voidfile,
				  std::string const &,
				  std::string const &) 
    = &dalTable::openTable;
  void (dalTable::*summary1)() 
    = &dalTable::summary;
  void (dalTable::*summary2)(std::ostream &) 
    = &dalTable::summary;
  
  bool (dalTable::*setAttribute_short)(std::string const &,
				       short const *,
				       unsigned int const &) 
    = &dalTable::setAttribute;
  bool (dalTable::*setAttribute_int)(std::string const &,
				     int const *,
				     unsigned int const &) 
    = &dalTable::setAttribute;
  bool (dalTable::*setAttribute_uint)(std::string const &,
				      uint const *,
				      unsigned int const &) 
    = &dalTable::setAttribute;
  bool (dalTable::*setAttribute_long)(std::string const &,
				      long const *,
				      unsigned int const &) 
    = &dalTable::setAttribute;
  bool (dalTable::*setAttribute_float)(std::string const &,
				       float const *,
				       unsigned int const &) 
    = &dalTable::setAttribute;
  bool (dalTable::*setAttribute_double)(std::string const &,
					double const *,
					unsigned int const &) 
    = &dalTable::setAttribute;
  
  //________________________________________________________
  // Bindings for class and its methods
  
  boost::python::class_<dalTable>("dalTable")
    .def( boost::python::init<DAL::dalFileType const &>())
    .def( boost::python::init<DAL::dalFileType::Type const &>())
    // Public methods
    .def("summary",
	 summary1,
	 "Summary of the object's internal parameters and status.")
    .def("summary",
	 summary2,
	 "Summary of the object's internal parameters and status.")
    .def( "setAttribute_char", &dalTable::setAttribute_char,
	  "Set a character attribute" )
    .def( "setAttribute_short", setAttribute_short,
	  "Set a short integer attribute" )
    .def( "setAttribute_int", setAttribute_int,
	  "Set a integer attribute" )
    .def( "setAttribute_uint", setAttribute_uint,
	  "Set a unsigned integer attribute" )
    .def( "setAttribute_long", setAttribute_long,
	  "Set a long integer attribute" )
    .def( "setAttribute_float", setAttribute_float,
	  "Set a floating point attribute" )
    .def( "setAttribute_double", setAttribute_double,
	  "Set a double precision floating point attribute" )
    .def( "setAttribute_char", &dalTable::setAttribute_char_vector,
	  "Set an attribute from a list of chars." )
    .def( "setAttribute_short", &dalTable::setAttribute_short_vector,
	  "Set an attribute from a list of shorts." )
    .def( "setAttribute_int", &dalTable::setAttribute_int_vector,
	  "Set an attribute from a list of integers." )
    .def( "setAttribute_uint", &dalTable::setAttribute_uint_vector,
	  "Set an attribute from a list of unsigned integers." )
    .def( "setAttribute_long", &dalTable::setAttribute_long_vector,
	  "Set an attribute from a list of longs." )
    .def( "setAttribute_float", &dalTable::setAttribute_float_vector,
	  "Set an attribute from a list of floats." )
    .def( "setAttribute_double", &dalTable::setAttribute_double_vector,
	  "Set an attribute from a list of doubles." )
    .def( "setAttribute_string", &dalTable::setAttribute_string,
	  "Set a string attribute" )
    .def( "setAttribute_string", &dalTable::setAttribute_string_vector,
	  "Set a string attribute" )
    .def( "openTable", openTableHDF5,
	  "Open an hdf5 table object." )
    .def( "createTable", &dalTable::createTable,
	  "Create a table object." )
    .def( "addColumn", &dalTable::addColumn,
	  "Add a column to the table." )
    .def( "addComplexColumn", &dalTable::addComplexColumn,
	  "Add a complex column to the table." )
    .def( "removeColumn", &dalTable::removeColumn,
	  "Remove a column from the table." )
    .def( "writeDataByColNum", &dalTable::writeDataByColNum,
	  "Write column data using an index." )
    .def( "appendRow", &dalTable::append_row_boost,
	  "Append a row to the table." )
    .def( "appendRows", &dalTable::append_rows_boost,
	  "Append multiple rows to the table." )
    .def("write_col_by_index_boost", &dalTable::write_col_by_index_boost,
	 "Write data to a column with a numpy array as input")
    .def( "printColumns", &dalTable::printColumns,
	  "Print the column tables to the screen." )
    .def( "listColumns", &dalTable::listColumns_boost,
	  "Return a list of the table columns." )
    .def( "readRows", &dalTable::readRows_boost,
    	  "Read table rows." )
    .def( "getAttribute", &dalTable::getAttribute_boost,
	  "Return the value of a column attribute." )
    .def( "findAttribute", &dalTable::findAttribute,
	  "Return true if the attribute exists for the table." )
    .def( "getNumberOfRows", &dalTable::getNumberOfRows,
	  "Return the number of rows in the table." )
#ifdef DAL_WITH_CASA
    .def( "openTable", &dalTable::ot_nonMStable,
	  "Open a casa table." )
    .def( "tableName", &dalTable::tableName,
	  "Get the name of a casa table." )
    .def( "setFilter", &dalTable::setFilter_boost1,
	  "Set a filter on a casa table." )
    .def( "setFilter", &dalTable::setFilter_boost2,
	  "Set a filter on a casa table." )
    .def( "getColumn", &dalTable::getColumn,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Return a casa column object." )
    .def( "getColumn_Float32", &dalTable::getColumn_Float32,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Return a 32-bit floating point casa column." )
    .def( "getColumn_complexFloat32", &dalTable::getColumn_complexFloat32,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Return a complex 32-bit floating point casa column." )
#endif
    ;
}
