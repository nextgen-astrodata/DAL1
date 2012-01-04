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
  \file pydal_core_dalDataset.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::dalDataset class

  \author Lars B&auml;hren
*/

#include "pydal.h"
#include <core/dalDataset.h>

using std::string;
using DAL::dalDataset;
using DAL::dalTable;
using DAL::dalArray;

// ==============================================================================
//
//                                                     Additional Python wrappers
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                     cia_boost1

dalArray * dalDataset::cia_boost1 (std::string arrayname,
				   boost::python::list pydims,
				   boost::python::list pydata )
{
  boost::python::list cdims;
  
  for (int ii=0; ii<boost::python::len(pydims); ii++) {
    cdims.append(10);
  }
  
  dalArray * array;
  array = cia_boost2( arrayname, pydims, pydata, cdims );
  
  /* Return pointer to the created array */
  return array;
}

//_______________________________________________________________________________
//                                                                     cia_boost2

dalArray * dalDataset::cia_boost2 (std::string arrayname,
				   boost::python::list pydims,
				   boost::python::list pydata,
				   boost::python::list cdims )
{
  std::vector<int> dims;
  std::vector<int> chnkdims;
  
  for (int ii=0; ii<boost::python::len(pydims); ii++) {
    dims.push_back(boost::python::extract<int>(pydims[ii]));
  }
  
  for (int ii=0; ii<boost::python::len(cdims); ii++) {
    chnkdims.push_back(boost::python::extract<int>(cdims[ii]));
  }
  
  long size = boost::python::len(pydata);
  int * data = NULL;
  data = new int[size];
  
  for (int ii=0; ii<size; ii++) {
    data[ii] = boost::python::extract<int>(pydata[ii]);
  }
  
  dalArray * array = createIntArray(arrayname, dims, data, chnkdims);
  
  /* Release allocated memory */
  delete [] data;
  /* Return pointer to created array */
  return array;
}

//_______________________________________________________________________________
//                                                            cia_boost_numarray1

dalArray * dalDataset::cia_boost_numarray1 (std::string arrayname,
					    boost::python::list pydims,
					    boost::python::numeric::array pydata )
{
  boost::python::list cdims;
  
  for (int ii=0; ii<boost::python::len(pydims); ii++) {
    cdims.append(10);
  }
  
  dalArray * array;
  array = cia_boost_numarray2(arrayname, pydims, pydata, cdims);
  
  /* Return pointer to created array */
  return array;
}

//_______________________________________________________________________________
//                                                            cia_boost_numarray2

dalArray * dalDataset::cia_boost_numarray2 (std::string arrayname,
					    boost::python::list pydims,
					    boost::python::numeric::array pydata,
					    boost::python::list cdims )
{
  std::vector<int> dims;
  std::vector<int> chnkdims;
  
  for (int ii=0; ii<boost::python::len(pydims); ii++) {
    dims.push_back(boost::python::extract<int>(pydims[ii]));
  }
  
  for (int ii=0; ii<boost::python::len(cdims); ii++) {
    chnkdims.push_back(boost::python::extract<int>(cdims[ii]));
  }
  
  int size = num_util::size(pydata);
  int * data = NULL;
  data = new int[size];
  
  for (int ii=0; ii<size; ii++) {
    data[ii] = boost::python::extract<int>(pydata[ii]);
  }
  
  dalArray * array = createIntArray(arrayname, dims, data, chnkdims);
  
  /* Release allocated memory */
  delete [] data;
  /* Return pointer to created array */
  return array;
}

//_______________________________________________________________________________
//                                                                      cfa_boost

dalArray * dalDataset::cfa_boost (std::string arrayname,
				  boost::python::list pydims,
				  boost::python::list pydata,
				  boost::python::list cdims)
{
  std::vector<int> dims;
  std::vector<int> chnkdims;
  
  for (int ii=0; ii<boost::python::len(pydims); ii++) {
    dims.push_back(boost::python::extract<int>(pydims[ii]));
  }
  
  for (int ii=0; ii<boost::python::len(cdims); ii++) {
    chnkdims.push_back(boost::python::extract<int>(cdims[ii]));
  }
  
  long size = boost::python::len(pydata);
  float * data = NULL;
  data = new float[size];
  
  for (int ii=0; ii<size; ii++) {
    data[ii] = boost::python::extract<float>(pydata[ii]);
  }
  
  dalArray * array = createFloatArray( arrayname, dims, data, chnkdims );
  
  /* Release allocated memory */
  delete [] data;
  /* Return pointer to created array */
  return array;
}

// ------------------------------------------------------ cfa_boost_numarray

dalArray * dalDataset::cfa_boost_numarray (std::string arrayname,
					   boost::python::list pydims,
					     boost::python::numeric::array pydata,
					     boost::python::list cdims )
  {
    std::vector<int> dims;
    std::vector<int> chnkdims;

    for (int ii=0; ii<boost::python::len(pydims); ii++)
      dims.push_back(boost::python::extract<int>(pydims[ii]));

    for (int ii=0; ii<boost::python::len(cdims); ii++)
      chnkdims.push_back(boost::python::extract<int>(cdims[ii]));

    int size = num_util::size(pydata);
    float * data = NULL;
    data = new float[size];

    for (int ii=0; ii<size; ii++)
      data[ii] = boost::python::extract<float>(pydata[ii]);

    dalArray * array = createFloatArray(arrayname, dims, data, chnkdims);

    /* Release allocated memory */
    delete [] data;
    /* Return pointer to created array */
    return array;
  }

  // ---------------------------------------------------------- ria_boost

  boost::python::numeric::array dalDataset::ria_boost (std::string arrayname )
  {
    hid_t lclfile;
    hid_t  status;

    // get the dataspace
    lclfile = H5Dopen (h5fh_p, arrayname.c_str(), H5P_DEFAULT);
    hid_t filespace = H5Dget_space(lclfile);

    // what is the rank of the array?
    hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
    hsize_t dims[ data_rank ];
    status = H5Sget_simple_extent_dims(filespace, dims, NULL);

    int size = 1;
    std::vector<int> dimensions;
    for (int ii=0; ii<data_rank; ii++) {
      size *= dims[ii];
      dimensions.push_back(dims[ii]);
    }
    
    int * data = NULL;
    data = new int[size];
    /* Read data from HDF5 file */
    status = H5LTread_dataset_int( h5fh_p, arrayname.c_str(), data );
    /* Convert data array */
    boost::python::numeric::array nadata = num_util::makeNum( (int*)data, dimensions );
    
    /* Release allocated memory */
    delete data;
    /* Return result */
    return nadata;
  }
  
  // ---------------------------------------------------------- rfa_boost
  
  boost::python::numeric::array dalDataset::rfa_boost (std::string arrayname )
  {
    hid_t lclfile;
    hid_t status;
    
    // get the dataspace
    lclfile = H5Dopen (h5fh_p, arrayname.c_str(), H5P_DEFAULT);
    hid_t filespace = H5Dget_space(lclfile);
    
    // what is the rank of the array?
    hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
    hsize_t dims[ data_rank ];

    status = H5Sget_simple_extent_dims(filespace, dims, NULL);

    int size = 1;
    std::vector<int> dimensions;

    for (int ii=0; ii<data_rank; ii++)
      {
        size *= dims[ii];
        dimensions.push_back(dims[ii]);
      }

    float * data = NULL;
    data = new float[size];

    status = H5LTread_dataset_float( h5fh_p, arrayname.c_str(), data );
    boost::python::numeric::array nadata = num_util::makeNum( (float*)data, dimensions );

    /* Release allocated memory */
    delete data;
    /* Return result */
    return nadata;
  }

  /******************************************************
   * wrappers for setFilter
   ******************************************************/

  // -------------------------------------------------------- listTables_boost

  boost::python::list dalDataset::listTables_boost()
  {
    std::vector<std::string> lcltabs = listTables();
    boost::python::list lcllist;
    for (uint idx=0; idx<lcltabs.size(); idx++) {
      lcllist.append( lcltabs[idx] );
    }
    return lcllist;
  }

//_______________________________________________________________________________
//                                                                   setAttribute

  bool dalDataset::setAttribute_char_vector (std::string attrname, boost::python::list data )
  {
    int size = boost::python::len(data);
    std::vector<char> mydata;

    for (int ii=0; ii<boost::python::len(data); ii++)
      mydata.push_back(boost::python::extract<char>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<char*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_short_vector (std::string attrname, boost::python::list data )
  {
    int size = boost::python::len(data);
    std::vector<short> mydata;

    for (int ii=0; ii<boost::python::len(data); ii++)
      mydata.push_back(boost::python::extract<short>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<short*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_int_vector (std::string attrname, boost::python::list data )
  {
    int size = boost::python::len(data);
    std::vector<int> mydata;

    for (int ii=0; ii<boost::python::len(data); ii++)
      mydata.push_back(boost::python::extract<int>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<int*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_uint_vector (std::string attrname, boost::python::list data )
  {
    int size = boost::python::len(data);
    std::vector<uint> mydata;

    for (int ii=0; ii<boost::python::len(data); ii++)
      mydata.push_back(boost::python::extract<uint>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<uint*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_long_vector (std::string attrname, boost::python::list data )
  {
    int size = boost::python::len(data);
    std::vector<long> mydata;

    for (int ii=0; ii<boost::python::len(data); ii++)
      mydata.push_back(boost::python::extract<long>(data[ii]));

#ifndef WORDSIZE_IS_64
    return setAttribute (attrname, reinterpret_cast<int64_t*>(&mydata[0]), size );
#else
    return setAttribute (attrname, reinterpret_cast<long*>(&mydata[0]), size );
#endif
  }
  bool dalDataset::setAttribute_float_vector (std::string attrname, boost::python::list data )
  {
    int size = boost::python::len(data);
    std::vector<float> mydata;

    for (int ii=0; ii<boost::python::len(data); ii++)
      mydata.push_back(boost::python::extract<float>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<float*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_double_vector (std::string attrname, boost::python::list data )
  {
    int size = boost::python::len(data);
    std::vector<double> mydata;

    for (int ii=0; ii<boost::python::len(data); ii++)
      mydata.push_back(boost::python::extract<double>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<double*>(&mydata[0]), size );
  }
  bool dalDataset::setAttribute_string_vector (std::string attrname, boost::python::list data )
  {
    int size = boost::python::len(data);
    std::vector<std::string> mydata;

    for (int ii=0; ii<boost::python::len(data); ii++)
      mydata.push_back(boost::python::extract<std::string>(data[ii]));

    return setAttribute (attrname, reinterpret_cast<std::string*>(&mydata[0]), size );
  }

  /*  getAttribute calls for all types, for single values */
  boost::python::numeric::array dalDataset::getAttribute_float_boost ( std::string attrname )
  {
    std::vector<float> value;
    HDF5Attribute::read (h5fh_p, attrname.c_str(), value );
    std::cerr << value << std::endl;
    std::vector<int> dims;
    dims.push_back( value.size() );

     boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<float*>(&value[0]), dims );
     return arr;
  }
  boost::python::numeric::array dalDataset::getAttribute_double_boost ( std::string attrname )
  {
    std::vector<double> value;
    HDF5Attribute::read (h5fh_p, attrname.c_str(), value );
    std::cerr << value << std::endl;
    std::vector<int> dims;
    dims.push_back( value.size() );
    
    boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<double*>(&value[0]), dims );
    return arr;
  }
boost::python::numeric::array dalDataset::getAttribute_long_boost ( std::string attrname )
{
  std::vector<long> value;
  HDF5Attribute::read (h5fh_p, attrname.c_str(), value );
  std::cerr << value << std::endl;
  std::vector<int> dims;
  dims.push_back( value.size() );
  	 
     boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<long*>(&value[0]), dims );
     return arr;
  }
  boost::python::numeric::array dalDataset::getAttribute_short_boost ( std::string attrname )
  {
    std::vector<short> value;
    HDF5Attribute::read (h5fh_p, attrname.c_str(), value );
    std::cerr << value << std::endl;
    std::vector<int> dims;
    dims.push_back( value.size() );
    
    boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<short*>(&value[0]), dims );
     return arr;
  }
  boost::python::numeric::array dalDataset::getAttribute_int_boost ( std::string attrname )
  {
    std::vector<int> value;
    HDF5Attribute::read ( h5fh_p, attrname.c_str(), value );
    std::cerr << value << std::endl;
    std::vector<int> dims;
    dims.push_back( value.size() );
    
    boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<int*>(&value[0]), dims );
     return arr;
  }
  boost::python::numeric::array dalDataset::getAttribute_uint_boost ( std::string attrname )
  {
    std::vector<uint> value;
    HDF5Attribute::read ( h5fh_p, attrname.c_str(), value );
    std::cerr << value << std::endl;
    std::vector<int> dims;
    dims.push_back( value.size() );
  	 
     boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<uint*>(&value[0]), dims );
     return arr;
  }
  boost::python::list dalDataset::getAttribute_string_boost ( std::string attrname )
  {
     boost::python::list data;
     std::vector<string> value;
     HDF5Attribute::read (h5fh_p, attrname.c_str(), value );
     std::cerr << value << std::endl;
     std::vector<int> dims;
     dims.push_back( value.size() );
  	        
     for ( uint ii=0; ii < value.size() ; ii++ )
        data.append( value[ii].c_str() );

     return data;
  }
  
// ==============================================================================
//
//                                                      Wrapper for class methods
//
// ==============================================================================

void export_dalDataset ()
{
  //________________________________________________________
  // Specialisation of overloaded methods

  void (dalDataset::*setFilter1)(std::string const &) 
    = &dalDataset::setFilter;
  void (dalDataset::*setFilter2)(std::string const &,
				 std::string const &) 
    = &dalDataset::setFilter;
  dalTable * (dalDataset::*createTable1)(std::string) 
    = &dalDataset::createTable;
  dalTable * (dalDataset::*createTable2)(std::string,std::string) 
    = &dalDataset::createTable;
  dalTable * (dalDataset::*openTable1)(std::string const &) 
    = &dalDataset::openTable;
  dalTable * (dalDataset::*openTable2)(std::string const &,
				       std::string const &) 
    = &dalDataset::openTable;
  dalArray * (dalDataset::*openArray1)(std::string const &) 
    = &dalDataset::openArray;
  dalArray * (dalDataset::*openArray2)(std::string const &,
				       std::string const &)
    = &dalDataset::openArray;
  
  //________________________________________________________
  // Bindings for class and its methods

  boost::python::class_<dalDataset>("dalDataset")
    .def( boost::python::init<std::string const &,
		    DAL::dalFileType const &,
		    DAL::IO_Mode const &>())
    .def( boost::python::init<std::string const &,
		    DAL::dalFileType::Type const &,
		    DAL::IO_Mode const &>())
    .def( boost::python::init<std::string const &,
		    std::string const &,
		    DAL::IO_Mode const &>())
    .def( "setAttribute_char", &dalDataset::setAttribute_char_vector,
	  "Set an attribute from a list of chars." )
    .def( "setAttribute_short", &dalDataset::setAttribute_short_vector,
	  "Set an attribute from a list of shorts." )
    .def( "setAttribute_int", &dalDataset::setAttribute_int_vector,
	  "Set an attribute from a list of integers." )
    .def( "setAttribute_uint", &dalDataset::setAttribute_uint_vector,
	  "Set an attribute from a list of unsigned integers." )
    .def( "setAttribute_long", &dalDataset::setAttribute_long_vector,
	  "Set an attribute from a list of longs." )
    .def( "setAttribute_float", &dalDataset::setAttribute_float_vector,
	  "Set an attribute from a list of floats." )
    .def( "setAttribute_double", &dalDataset::setAttribute_double_vector,
	  "Set an attribute from a list of doubles." )
    .def( "setAttribute_string", &dalDataset::setAttribute_string_vector,
	  "Set a string attribute" )
    .def( "getAttribute_float", &dalDataset::getAttribute_float_boost,
	  "Return a dalDataset float attribute into a numpy array." )
    .def( "getAttribute_double", &dalDataset::getAttribute_double_boost,
	  "Return a dalDataset double attribute into a numpy array." )
    .def( "getAttribute_long", &dalDataset::getAttribute_long_boost,
	  "Return a dalDataset long attribute into a numpy array." )
    .def( "getAttribute_short", &dalDataset::getAttribute_short_boost,
	  "Return a dalDataset short attribute into a numpy array." )
    .def( "getAttribute_int", &dalDataset::getAttribute_int_boost,
	  "Return a dalDataset int attribute into a numpy array." )
    .def( "getAttribute_uint", &dalDataset::getAttribute_uint_boost,
	  "Return a dalDataset uint attribute into a numpy array." )
    .def( "getAttribute_string", &dalDataset::getAttribute_string_boost,
	  "Return a dalDataset string attribute into a numpy array." )
    .def( "open", &dalDataset::open,
	  ( boost::python::arg("dataset_name") ),
	  "Opens a dataset." )
    .def( "close", &dalDataset::close,
	  "Closes a dataset." )
    .def( "getType", &dalDataset::getType,
	  "Get the file type of dataset." )
    .def( "createTable", createTable1,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  ( boost::python::arg("table_name") ),
	  "Create a new table in the dataset." )
    .def( "createTable", createTable2,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  ( boost::python::arg("table_name"), boost::python::arg("group_name") ),
	  "Create a new table in the dataset." )
    .def( "createGroup", &dalDataset::createGroup,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  ( boost::python::arg("group_name") ),
	  "Create a new group in the dataset." )
    .def( "openTable", openTable1,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Open a table in the dataset.")
    .def( "openTable", openTable2,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Open a table in the dataset.")
    .def( "openGroup", &dalDataset::openGroup,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Open a group in the dataset.")
    .def( "openArray", openArray1,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Open an array in the dataset.")
    .def( "openArray", openArray2,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Open an array in the dataset.")
    .def( "createIntArray", &dalDataset::cia_boost1,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create an integer array in the dataset." )
    .def( "createIntArray", &dalDataset::cia_boost2,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create an integer array in the dataset." )
    .def( "createIntArray", &dalDataset::cia_boost_numarray1,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create an integer array in the dataset." )
    .def( "createIntArray", &dalDataset::cia_boost_numarray2,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create an integer array in the dataset." )
    .def( "createFloatArray", &dalDataset::cfa_boost,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create an floating-point array in the dataset." )
    .def( "createFloatArray", &dalDataset::cfa_boost_numarray,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create an floating-point array in the dataset." )
    .def( "readIntArray", &dalDataset::ria_boost,
	  "Read an integer array from the dataset." )
    .def( "readFloatArray", &dalDataset::rfa_boost,
	  "Read a floating-point array from the dataset." )
    .def( "createArray", &dalDataset::createArray,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create an array from a dalData object" )
    .def( "setFilter", setFilter1,
	  ( boost::python::arg("columns") ),
	  "Set a filter on the dataset (casa only)." )
    .def( "setFilter", setFilter2,
	  ( boost::python::arg("columns"), boost::python::arg("conditons") ),
	  "Set a filter on the dataset (casa only)." )
    .def( "listTables", &dalDataset::listTables_boost,
	  "Return a list of the tables in the dataset." )
    ;
}

