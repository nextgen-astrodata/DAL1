/***************************************************************************
 *   Copyright (C) 2011                                                    *
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
  \file pydal_core_dalGroup.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::dalGroup class

  \author Lars B&auml;hren
*/

#include "pydal.h"
#include <core/dalGroup.h>

using DAL::dalGroup;
using DAL::dalArray;

// ==============================================================================
//
//                                                     Additional Python wrappers
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                 csa_boost_list

dalArray * dalGroup::csa_boost_list( std::string arrayname,
				     boost::python::list pydims,
				     boost::python::list pydata )
{
  std::vector<int> dims;
  std::vector<int> chnkdims;
  
  for (int ii=0; ii<boost::python::len(pydims); ii++)
    dims.push_back(boost::python::extract<int>(pydims[ii]));
  
  long size = boost::python::len(pydata);
  short * data = NULL;
  data = new short[size];
  
  for (int ii=0; ii<size; ii++)
    data[ii] = boost::python::extract<short>(pydata[ii]);
  
  dalArray * array = createShortArray(arrayname, dims, data, chnkdims);
  
  delete [] data;
  data = NULL;
  
  return array;
}

//_______________________________________________________________________________
//                                                                 cia_boost_list

dalArray * dalGroup::cia_boost_list( std::string arrayname,
				     boost::python::list pydims,
				     boost::python::list pydata )
{
  std::vector<int> dims;
  std::vector<int> chnkdims;
  
  for (int ii=0; ii<boost::python::len(pydims); ii++)
    dims.push_back(boost::python::extract<int>(pydims[ii]));
  
  long size = boost::python::len(pydata);
  int * data = NULL;
  data = new int[size];
  
  for (int ii=0; ii<size; ii++)
    data[ii] = boost::python::extract<int>(pydata[ii]);
  
  dalArray * array = createIntArray(arrayname, dims, data, chnkdims);
  
  delete [] data;
  data = NULL;
  
  return array;
}

//_______________________________________________________________________________
//                                                                      ria_boost

boost::python::numeric::array dalGroup::ria_boost( std::string arrayname )
{
  hid_t lclfile;
  hid_t  status;
  
  // get the dataspace
  lclfile = H5Dopen ( itsGroupID, arrayname.c_str(), H5P_DEFAULT );
  hid_t filespace = H5Dget_space(lclfile);
  
  // what is the rank of the array?
  hid_t data_rank = H5Sget_simple_extent_ndims(filespace);
  hsize_t dims[ data_rank ];
#ifdef DAL_DEBUGGING_MESSAGES
  std::cerr << "data rank: " << data_rank << endl;
#endif
  status = H5Sget_simple_extent_dims(filespace, dims, NULL);
  
  int size = 1;
  std::vector<int> dimsvec;
  for (int ii=0; ii<data_rank; ii++)
    {
#ifdef DAL_DEBUGGING_MESSAGES
      std::cerr << "dims["  << ii << "]: " << dims[ii] << endl;
#endif
      size *= dims[ii];
      dimsvec.push_back( dims[ii] );
    }
  
#ifdef DAL_DEBUGGING_MESSAGES
  std::cerr << "size: " << size << endl;
#endif
  
  int * data = NULL;
  data = new int[size];
  
  status = H5LTread_dataset_int( itsGroupID, arrayname.c_str(), data );
  
#ifdef DAL_DEBUGGING_MESSAGES
  for (int ii=0; ii<size; ii++)
    {
      std::cerr << data[ii] << endl;
    }
#endif
  
  return num_util::makeNum( data, dimsvec );
}

//_______________________________________________________________________________
//                                                                 cfa_boost_list

dalArray * dalGroup::cfa_boost_list( std::string arrayname,
				     boost::python::list pydims,
				     boost::python::list pydata )
{
  
  std::vector<int> dims;
  std::vector<int> chnkdims;

    for (int ii=0; ii<boost::python::len(pydims); ii++)
      dims.push_back(boost::python::extract<int>(pydims[ii]));

    long size = boost::python::len(pydata);
    float * data = NULL;
    data = new float[size];

    for (int ii=0; ii<size; ii++)
      data[ii] = boost::python::extract<float>(pydata[ii]);

    dalArray * array = createFloatArray(arrayname, dims, data, chnkdims);

    delete [] data;
    data = NULL;

    return array;
}

bool dalGroup::setAttribute_char (std::string const &name,
				  char const &data)
{
  return HDF5Attribute::write (itsGroupID,
			       name,
			       &data,
			       1);
}
bool dalGroup::setAttribute_short (std::string const &name,
				   short const &data)
{
  return HDF5Attribute::write (itsGroupID,
			       name,
			       &data,
			       1);
}
bool dalGroup::setAttribute_int (std::string const &name,
				 int const &data)
{
  return HDF5Attribute::write (itsGroupID,
			       name,
			       &data,
			       1);
}
bool dalGroup::setAttribute_uint (std::string const &name,
				  uint const &data)
{
  return HDF5Attribute::write (itsGroupID,
			       name,
			       &data,
			       1);
}
bool dalGroup::setAttribute_long (std::string const &name,
				  long const &data)
{
  return HDF5Attribute::write (itsGroupID,
			       name,
			       &data,
			       1);
}
bool dalGroup::setAttribute_float (std::string const &name,
				   float const &data)
{
  return HDF5Attribute::write (itsGroupID,
			       name,
			       &data,
			       1);
}
bool dalGroup::setAttribute_double (std::string const &name,
				    double const &data)
{
  return HDF5Attribute::write (itsGroupID,
			       name,
			       &data,
			       1);
}
bool dalGroup::setAttribute_string (std::string const &name,
				    std::string const &data)
{
  return HDF5Attribute::write (itsGroupID,
			       name,
			       &data,
			       1);
}
bool dalGroup::setAttribute_char_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<char> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<char>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<char*>(&mydata[0]), size );
}
bool dalGroup::setAttribute_short_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<short> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<short>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<short*>(&mydata[0]), size );
}
bool dalGroup::setAttribute_int_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<int> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<int>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<int*>(&mydata[0]), size );
}
bool dalGroup::setAttribute_uint_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<uint> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<uint>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<uint*>(&mydata[0]), size );
}
bool dalGroup::setAttribute_long_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<long> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<long>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<long*>(&mydata[0]), size );
}
bool dalGroup::setAttribute_float_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<float> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<float>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<float*>(&mydata[0]), size );
}
bool dalGroup::setAttribute_double_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<double> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<double>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<double*>(&mydata[0]), size );
}
bool dalGroup::setAttribute_string_vector (std::string attrname, boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<std::string> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<std::string>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<std::string*>(&mydata[0]), size );
}

//  bool setAttribute_char_vector( std::string attrname, boost::python::list data );
//  bool setAttribute_short_vector( std::string attrname, boost::python::list data );
//  bool setAttribute_int_vector( std::string attrname, boost::python::list data );
//  bool setAttribute_uint_vector( std::string attrname, boost::python::list data );
//  bool setAttribute_long_vector( std::string attrname, boost::python::list data );
//  bool setAttribute_float_vector( std::string attrname, boost::python::list data );
//  bool setAttribute_double_vector( std::string attrname, boost::python::list data );
//  bool setAttribute_string_vector( std::string attrname, boost::python::list data )
//    {
//    std::vector<std::string> mydata;
//
//    for (int ii=0; ii<boost::python::len(data); ii++)
//      mydata.push_back(boost::python::extract<std::string>(data[ii]));
//
//    return setAttribute_string( attrname, mydata );
//  }

boost::python::numeric::array dalGroup::getAttribute_float_boost ( std::string attrname )
{
  std::vector<float> value;
  HDF5Attribute::write (itsGroupID, attrname.c_str(), value );
  std::cerr << value << std::endl;
  std::vector<int> dims;
  dims.push_back( value.size() );
  
  //  	 float * values = NULL;
  
  
  //     std::vector<int> mydims;
  //     mydims.push_back( value.size() );
  //
  //     for (int idx=0; idx++; idx<value.size() )
  //        values
  //     
  //      boost::python::numeric::array narray = num_util::makeNum( values, mydims );
  //      delete [] values;
  //      values = NULL;
  //      return narray;
  
  
  boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<float*>(&value[0]), dims );
  return arr;
}
boost::python::numeric::array dalGroup::getAttribute_double_boost ( std::string attrname )
{
  std::vector<double> value;
  HDF5Attribute::write( itsGroupID, attrname.c_str(), value );
  std::cerr << value << std::endl;
  std::vector<int> dims;
  dims.push_back( value.size() );
  
  boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<double*>(&value[0]), dims );
  return arr;
}
boost::python::numeric::array dalGroup::getAttribute_long_boost ( std::string attrname )
{
  std::vector<long> value;
  HDF5Attribute::write( itsGroupID, attrname.c_str(), value );
  std::cerr << value << std::endl;
  std::vector<int> dims;
  dims.push_back( value.size() );
  
  boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<long*>(&value[0]), dims );
  return arr;
}
boost::python::numeric::array dalGroup::getAttribute_short_boost ( std::string attrname )
{
  std::vector<short> value;
  HDF5Attribute::write( itsGroupID, attrname.c_str(), value );
  std::cerr << value << std::endl;
  std::vector<int> dims;
  dims.push_back( value.size() );
  
  boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<short*>(&value[0]), dims );
  return arr;
}
boost::python::numeric::array dalGroup::getAttribute_int_boost ( std::string attrname )
{
  std::vector<int> value;
  HDF5Attribute::write( itsGroupID, attrname.c_str(), value );
  std::cerr << value << std::endl;
  std::vector<int> dims;
  dims.push_back( value.size() );
  
  boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<int*>(&value[0]), dims );
  return arr;
}
boost::python::numeric::array dalGroup::getAttribute_uint_boost ( std::string attrname )
{
  std::vector<uint> value;
  HDF5Attribute::write( itsGroupID, attrname.c_str(), value );
  std::cerr << value << std::endl;
  std::vector<int> dims;
  dims.push_back( value.size() );
  
  boost::python::numeric::array arr = num_util::makeNum( reinterpret_cast<uint*>(&value[0]), dims );
  return arr;
}
boost::python::list dalGroup::getAttribute_string_boost ( std::string attrname )
{
  boost::python::list data;
  std::vector<std::string> value;
  HDF5Attribute::write( itsGroupID, attrname.c_str(), value );
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

void export_dalGroup () 
{
  boost::python::class_<dalGroup>("dalGroup")
    .def( boost::python::init<>())
    .def( boost::python::init<char*, void*>())
    .def( boost::python::init<hid_t,char*>())
    .def( "setAttribute_char", &dalGroup::setAttribute_char,
	  "Set a character attribute" )
    .def( "setAttribute_char", &dalGroup::setAttribute_char_vector,
	  "Set an attribute from a list of chars." )
    .def( "setAttribute_short", &dalGroup::setAttribute_short,
	  "Set a short integer attribute" )
    .def( "setAttribute_short", &dalGroup::setAttribute_short_vector,
	  "Set an attribute from a list of shorts." )
    .def( "setAttribute_int", &dalGroup::setAttribute_int,
	  "Set a integer attribute" )
    .def( "setAttribute_int", &dalGroup::setAttribute_int_vector,
	  "Set an attribute from a list of integers." )
    .def( "setAttribute_uint", &dalGroup::setAttribute_uint,
	  "Set a unsigned integer attribute" )
    .def( "setAttribute_uint", &dalGroup::setAttribute_uint_vector,
	  "Set an attribute from a list of unsigned integers." )
    .def( "setAttribute_long", &dalGroup::setAttribute_long,
	  "Set a long integer attribute" )
    .def( "setAttribute_long", &dalGroup::setAttribute_long_vector,
	  "Set an attribute from a list of longs." )
    .def( "setAttribute_float", &dalGroup::setAttribute_float,
	  "Set a floating point attribute" )
    .def( "setAttribute_float", &dalGroup::setAttribute_float_vector,
	  "Set an attribute from a list of floats." )
    .def( "setAttribute_double", &dalGroup::setAttribute_double,
	  "Set a double precision floating point attribute" )
    .def( "setAttribute_double", &dalGroup::setAttribute_double_vector,
	  "Set an attribute from a list of doubles." )
    .def( "setAttribute_string", &dalGroup::setAttribute_string,
	  "Set a string attribute" )
    .def( "setAttribute_string", &dalGroup::setAttribute_string_vector,
	  "Set a string attribute" )
    .def( "getAttribute_float", &dalGroup::getAttribute_float_boost,
	  "Return a dalGroup float attribute into a numpy array." )
    .def( "getAttribute_double", &dalGroup::getAttribute_double_boost,
	  "Return a dalGroup double attribute into a numpy array." )
    .def( "getAttribute_long", &dalGroup::getAttribute_long_boost,
	  "Return a dalGroup long attribute into a numpy array." )
    .def( "getAttribute_short", &dalGroup::getAttribute_short_boost,
	  "Return a dalGroup short attribute into a numpy array." )
    .def( "getAttribute_int", &dalGroup::getAttribute_int_boost,
	  "Return a dalGroup int attribute into a numpy array." )
    .def( "getAttribute_uint", &dalGroup::getAttribute_uint_boost,
	  "Return a dalGroup uint attribute into a numpy array." )
    .def( "getAttribute_string", &dalGroup::getAttribute_string_boost,
	  "Return a dalGroup string attribute into a numpy array." )
    .def( "setName", &dalGroup::setName,
	  "Set the name of the group." )
    .def( "getId", &dalGroup::getId,
	  "Return the group identifier." )
    .def( "createShortArray", &dalGroup::csa_boost_list,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create a short array in the group." )
    .def( "createIntArray", &dalGroup::cia_boost_list,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create an integer array in the group." )
    .def( "createFloatArray", &dalGroup::cfa_boost_list,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Create a floating point array in the group." )
    .def( "readIntArray", &dalGroup::ria_boost,
	  "Read an integer array from the group." )
    ;
  
}
