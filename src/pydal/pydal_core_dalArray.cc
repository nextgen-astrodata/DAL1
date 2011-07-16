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
  \file pydal_core_dalArray.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::dalArray class

  \author Lars B&auml;hren
*/

#include "pydal.h"
#include <core/dalArray.h>

using DAL::dalArray;

// ==============================================================================
//
//                                                     Additional Python wrappers
//
// ==============================================================================

void DAL::dalArray::extend_boost (boost::python::list pydims )
{
  std::vector<int> dims;
  
  for (int ii=0; ii<boost::python::len(pydims); ii++) {
    dims.push_back(boost::python::extract<int>(pydims[ii]));
  }
  
  extend( dims );
}

bool DAL::dalArray::setAttribute_char (std::string attrname,
				       char data )
{
  return setAttribute( attrname, &data );
}

bool DAL::dalArray::setAttribute_short (std::string const &name,
					short const &data)
{
  return HDF5Attribute::write (itsDatasetID, name, data);
}

bool DAL::dalArray::setAttribute_int (std::string const &name,
				      int const &data)
{
  return HDF5Attribute::write (itsDatasetID, name, data);
}

bool DAL::dalArray::setAttribute_uint (std::string const &name,
				       uint const &data)
{
  return HDF5Attribute::write (itsDatasetID, name, data);
}

bool DAL::dalArray::setAttribute_long (std::string const &name,
				       long const &data)
{
  return HDF5Attribute::write (itsDatasetID, name, data);
}

bool DAL::dalArray::setAttribute_float (std::string const &name,
					float const &data)
{
  return HDF5Attribute::write (itsDatasetID, name, data);
}

bool DAL::dalArray::setAttribute_double (std::string const &name,
					 double const &data)
{
  return HDF5Attribute::write (itsDatasetID, name, data);
}

bool DAL::dalArray::setAttribute_string (std::string attrname,
					 std::string data)
{
  return setAttribute( attrname, &data );
}

bool DAL::dalArray::setAttribute_char_vector (std::string attrname,
					      boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<char> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<char>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<char*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_short_vector (std::string attrname,
					       boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<short> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<short>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<short*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_int_vector (std::string attrname,
					     boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<int> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<int>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<int*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_uint_vector (std::string attrname,
					      boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<uint> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<uint>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<uint*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_long_vector (std::string attrname,
					      boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<long> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<long>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<long*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_float_vector (std::string attrname,
					       boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<float> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<float>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<float*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_double_vector (std::string attrname,
						boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<double> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<double>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<double*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_string_vector (std::string attrname,
						boost::python::list data )
{
  int size = boost::python::len(data);
  std::vector<std::string> mydata;
  
  for (int ii=0; ii<boost::python::len(data); ii++)
    mydata.push_back(boost::python::extract<std::string>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<std::string*>(&mydata[0]), size );
}

// ==============================================================================
//
//                                                      Wrapper for class methods
//
// ==============================================================================

void export_dalArray ()
{  
  boost::python::class_<dalArray>("dalArray")
    .def( "setAttribute_char", &dalArray::setAttribute_char,
	  "Set a character attribute" )
    .def( "setAttribute_char", &dalArray::setAttribute_char_vector,
	  "Set an attribute from a list of chars." )
    .def( "setAttribute_short", &dalArray::setAttribute_short,
	  "Set a short integer attribute" )
    .def( "setAttribute_short", &dalArray::setAttribute_short_vector,
	  "Set an attribute from a list of shorts." )
    .def( "setAttribute_int", &dalArray::setAttribute_int,
	  "Set a integer attribute" )
    .def( "setAttribute_int", &dalArray::setAttribute_int_vector,
	  "Set an attribute from a list of integers." )
    .def( "setAttribute_uint", &dalArray::setAttribute_uint,
	  "Set a unsigned integer attribute" )
    .def( "setAttribute_uint", &dalArray::setAttribute_uint_vector,
	  "Set an attribute from a list of unsigned integers." )
    .def( "setAttribute_long", &dalArray::setAttribute_long,
	  "Set a long integer attribute" )
    .def( "setAttribute_long", &dalArray::setAttribute_long_vector,
	  "Set an attribute from a list of longs." )
    .def( "setAttribute_float", &dalArray::setAttribute_float,
	  "Set a floating point attribute" )
    .def( "setAttribute_float", &dalArray::setAttribute_float_vector,
	  "Set an attribute from a list of floats." )
    .def( "setAttribute_double", &dalArray::setAttribute_double,
	  "Set a double precision floating point attribute" )
    .def( "setAttribute_double", &dalArray::setAttribute_double_vector,
	  "Set an attribute from a list of doubles." )
    .def( "setAttribute_string", &dalArray::setAttribute_string,
	  "Set a string attribute" )
    .def( "setAttribute_string", &dalArray::setAttribute_string_vector,
	  "Set a string attribute" )
    .def( "getAttributes", &dalArray::getAttributes,
	  "Print the attributes of the array." )
    .def( "extend", &dalArray::extend_boost,
	  "Extend an array." )
    .def( "close", &dalArray::close,
          "Close an array." )
    ;
}

