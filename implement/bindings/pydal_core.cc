/*-------------------------------------------------------------------------*
 | $Id:: pywrapper.cc 4184 2010-02-18 14:14:48Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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
  \file pydal_core.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings DAL C++ classes in the \e core directory

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <dalArray.h>
#include <dalColumn.h>
#include <dalData.h>

//_______________________________________________________________________________
//                                                                       dalArray

void export_dalArray ()
{  
  bpl::class_<dalArray>("dalArray")
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

//_______________________________________________________________________________
//                                                                      dalColumn

void export_dalColumn ()
{
  bpl::class_<dalColumn>("dalColumn")
    .def( bpl::init<>())
    .def( bpl::init<string>())
    .def( bpl::init<string,string>())
    .def( "addMember", &dalColumn::addMember,
	  "This method is useful for hdf5 files when creating a column \n"
	  "with a compound datatype.  For example, use this method if an \n"
	  "individual column needs to contain an int, a float, and a short." )
    .def( "getName", &dalColumn::getName,
	  "Return the name of the column." )
    .def( "getSize", &dalColumn::getSize,
	  "Return the size of the column for any 'normal'\n"
	  "type (i.e. not compound)." )
    .def( "data", &dalColumn::data_boost1,
	  "Returns the column data into a numpy array.  With no parameters \n"
	  "it tries to return the entire column." )
    .def( "data", &dalColumn::data_boost2,
	  "Returns the column data into a numpy array.  With one parameter, it \n"
	  "returns N elements [data(N)]." )
    .def( "data", &dalColumn::data_boost3,
	  "Returns the column data into a numpy array.  With three parameters, \n"
	  "it returns N elements beginning at offset O [data(O,N)]." )
#ifdef HAVE_CASA
    .def( "getDataType", &dalColumn::getDataType,
	  "Return the datatype of the column (casa only)." )
    .def( "ndims", &dalColumn::ndims,
	  "Return the number of column dimensions. (casa only)" )
    .def( "shape", &dalColumn::shape_boost,
	  "Return the shape of the column (casa only)" )
    .def( "isScalar", &dalColumn::isScalar,
	  "Returns true of the column is a scalar type." )
    .def( "isArray", &dalColumn::isArray,
	  "Returns true of the column is an array type." )
#endif
    ;
  
}

//_______________________________________________________________________________
//                                                                        dalData

void export_dalData () 
{
  bpl::class_<dalData>("dalData")
    .def( "get", &dalData::get_boost1, "Get the data." )
    .def( "get", &dalData::get_boost2, "Get the data." )
    .def( "get", &dalData::get_boost3, "Get the data." )
    ;
}
