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
  \file pydal_core.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings DAL C++ classes in the \e core directory

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"

// namespace usage
using DAL::dalArray;
using DAL::dalData;
using DAL::dalDataset;
using DAL::dalGroup;
using DAL::dalTable;

// ==============================================================================
//
//                                                                      dalCommon
//
// ==============================================================================

void export_dalCommon ()
{
  def( "mjd2unix", &DAL::mjd2unix_boost,
       "Convert Modified Julian Date (mjd) to unix time.\n"
       "The Unix base date is MJD 40587 and 1 mjd Day = 24 hours \n"
       "or 1440 minutes or 86400 seconds so: \n"
       "(unix seconds) = (mjd seconds) - ( unix base date in seconds )." );
//   def( "BigEndian", &BigEndian,
//        "Test of the system is big endian." );
}

// ==============================================================================
//
//                                                                 dalConversions
//
// ==============================================================================

namespace DAL {

  //__________________________________________________________________
  //                                                        array2list

  /*!
    \param data  -- Array data values
    \param nelem -- Number of elements inside the array.
    \return list -- Python list containing a copy of the \e data.
   */
  template <class T>
  boost::python::list array2list (T *data,
				  unsigned int const &nelem)
  {
    boost::python::list mylist;
    
    for (unsigned int n=0; n<nelem; ++n) {
      mylist.append(data[n]);
    }      

    return mylist;
  }
  
  //__________________________________________________________________
  //                                                       vector2list
  
  template <class T>
  boost::python::list vector2list (std::vector<T> vec)
  {
    boost::python::list mylist;
    
    typename std::vector<T>::iterator it;
    for ( it=vec.begin(); it < vec.end(); it++ )
      mylist.append( *it );
    
    return mylist;
  }
  
  //__________________________________________________________________
  //                                                    mjd2unix_boost

  /*!
    - The Unix base date is MJD 40587.
    - 1 mjd Day = 24 hours or 1440 minutes or 86400 seconds
    - (unix seconds) = (mjd seconds) - ( unix base date in seconds )
    
    \param mjd_time The time as Modified Julian Date.
  */
  bpl::numeric::array mjd2unix_boost ( bpl::numeric::array mjd_time )
  {
    int array_size           = bpl::len( mjd_time );
    double unix_base_time    = 40587;
    double seconds_per_day   = 86400;
    double adjustment_factor = unix_base_time*seconds_per_day;
    
    for ( int idx=0; idx < array_size; idx++ ) {
      mjd_time[ idx ] = bpl::extract<double>( mjd_time[ idx ] ) - adjustment_factor;
    }
    
    return mjd_time;
  }
  
}

// ==============================================================================
//
//                                                                       dalArray
//
// ==============================================================================

void DAL::dalArray::extend_boost (bpl::list pydims )
{
  std::vector<int> dims;
  
  for (int ii=0; ii<bpl::len(pydims); ii++) {
    dims.push_back(bpl::extract<int>(pydims[ii]));
  }
  
  extend( dims );
}

bool DAL::dalArray::setAttribute_char( std::string attrname,
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

bool DAL::dalArray::setAttribute_char_vector (std::string attrname, bpl::list data )
{
  int size = bpl::len(data);
  std::vector<char> mydata;
  
  for (int ii=0; ii<bpl::len(data); ii++)
    mydata.push_back(bpl::extract<char>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<char*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_short_vector (std::string attrname, bpl::list data )
{
  int size = bpl::len(data);
  std::vector<short> mydata;
  
  for (int ii=0; ii<bpl::len(data); ii++)
    mydata.push_back(bpl::extract<short>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<short*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_int_vector (std::string attrname, bpl::list data )
{
  int size = bpl::len(data);
  std::vector<int> mydata;
  
  for (int ii=0; ii<bpl::len(data); ii++)
    mydata.push_back(bpl::extract<int>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<int*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_uint_vector (std::string attrname, bpl::list data )
{
  int size = bpl::len(data);
  std::vector<uint> mydata;
  
  for (int ii=0; ii<bpl::len(data); ii++)
    mydata.push_back(bpl::extract<uint>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<uint*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_long_vector (std::string attrname, bpl::list data )
{
  int size = bpl::len(data);
  std::vector<long> mydata;
  
  for (int ii=0; ii<bpl::len(data); ii++)
    mydata.push_back(bpl::extract<long>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<long*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_float_vector (std::string attrname, bpl::list data )
{
  int size = bpl::len(data);
  std::vector<float> mydata;
  
  for (int ii=0; ii<bpl::len(data); ii++)
    mydata.push_back(bpl::extract<float>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<float*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_double_vector (std::string attrname, bpl::list data )
{
  int size = bpl::len(data);
  std::vector<double> mydata;
  
  for (int ii=0; ii<bpl::len(data); ii++)
    mydata.push_back(bpl::extract<double>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<double*>(&mydata[0]), size );
}

bool DAL::dalArray::setAttribute_string_vector (std::string attrname, bpl::list data )
{
  int size = bpl::len(data);
  std::vector<std::string> mydata;
  
  for (int ii=0; ii<bpl::len(data); ii++)
    mydata.push_back(bpl::extract<std::string>(data[ii]));
  
  return setAttribute (attrname, reinterpret_cast<std::string*>(&mydata[0]), size );
}

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

// ==============================================================================
//
//                                                                       dalData
//
// ==============================================================================

bpl::numeric::array DAL::dalData::get_boost1()
{
  return get_boost3(0,-1);
}

bpl::numeric::array DAL::dalData::get_boost2( int32_t length )
{
  return get_boost3(0,length);
}

bpl::numeric::array DAL::dalData::get_boost3( int64_t offset, int32_t length )
{
  bpl::list data_list;
  std::vector<int> mydims;
  
  unsigned int hh = 0;
  
  if (length>0) {
    mydims.push_back(length);
    hh=1;
  }
  for (; hh<shape.size(); hh++) {
    mydims.push_back(shape[hh]);
  }
  
  if ( dal_CHAR == dataType_p ) {
    return num_util::makeNum( ((char*)data) + offset, mydims );
  }
  else if ( dal_BOOL == dataType_p ) {
    return num_util::makeNum( ((unsigned char*)data) + offset, mydims );
  }
  else if ( dal_INT == dataType_p )
    {
      return num_util::makeNum( ((int*)data) + offset, mydims );
    }
  else if ( dal_FLOAT == dataType_p ) {
    return num_util::makeNum(((float*)data)+offset,mydims);
  }
  else if ( dal_DOUBLE == dataType_p ) {
    return num_util::makeNum(((double*)data)+offset,mydims);
  }
  else if ( dal_COMPLEX == dataType_p ) {
    return num_util::makeNum(((std::complex<float>*)data)+offset,mydims);
  }
  else if ( dal_COMPLEX_CHAR == dataType_p ) {
    return num_util::makeNum(((std::complex<char>*)data)+offset,mydims);
  }
  else if ( dal_COMPLEX_SHORT == dataType_p ) {
    return num_util::makeNum(((std::complex<short>*)data)+offset,mydims);
  }
  else if ( dal_STRING == dataType_p ) {
    bpl::list data_list;
    
    if ( 1 == shape.size() ) // 1D case
      {
	for (int ii=0; ii<nrows; ii++)
	  {
	    data_list.append( (*((std::string*)get(ii))) );
	  }
      }
    else if ( 2 == shape.size() ) // 2D case
      {
	for ( int xx=0; xx<shape[0]; xx++)
	  for ( int yy=0; yy<shape[1]; yy++)
	    data_list.append( (*((std::string*)get(xx,yy))) );
      }
    else if ( 3 == shape.size() ) // 3D case
      {
	for ( int xx=0; xx<shape[0]; xx++)
	  for ( int yy=0; yy<shape[1]; yy++)
	    for ( int zz=0; zz<shape[2]; zz++)
	      data_list.append( (*((std::string*)get(xx,yy,zz))) );
      }
    else {
      std::cerr << "ERROR: string array rank > 3 not supported. "
		<< "dalData::get_boost()\n";
    }
    
    bpl::numeric::array narray = num_util::makeNum(data_list);
    return narray;
  }
  else {
    std::cerr << "ERROR:  Datatype '" << dataType_p
	      << "' not yet supported.  (dalData::get_boost)\n";
    
    for (int ii=0; ii<1; ii++)
      data_list.append(0);
    
    bpl::numeric::array nadata( data_list );
    
    return nadata;
  }
}

void export_dalData () 
{
  bpl::class_<dalData>("dalData")
    .def( bpl::init<>())
    .def( "filetype", &dalData::filetype,
	  "Get the filetype." )
    .def( "datatype", &dalData::datatype,
	  "Get the type of data held by the object." )
    .def( "arrayOrder", &dalData::arrayOrder,
	  "Get the axis ordering of the data array." )
    .def( "fortran_index", &dalData::fortran_index,
	  "Get the fortran index value of up to a three-dimensional array." )
    .def( "c_index", &dalData::c_index,
	  "Get the C index value of up to a three-dimensional array." )
    .def( "get", &dalData::get_boost1,
	  "Get the data.")
    .def( "get", &dalData::get_boost2,
	  "Get the data.")
    .def( "get", &dalData::get_boost3,
	  "Get the data.")
    ;
}

