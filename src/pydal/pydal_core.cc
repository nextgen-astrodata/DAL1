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
using DAL::dalData;
using DAL::dalDataset;
using DAL::dalGroup;
using DAL::IO_Mode;
using DAL::HDF5Hyperslab;

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
  for (; hh<itsShape.size(); hh++) {
    mydims.push_back(itsShape[hh]);
  }
  
  if ( dal_CHAR == itsDatatype ) {
    return num_util::makeNum( ((char*)data) + offset, mydims );
  }
  else if ( dal_BOOL == itsDatatype ) {
    return num_util::makeNum( ((unsigned char*)data) + offset, mydims );
  }
  else if ( dal_INT == itsDatatype )
    {
      return num_util::makeNum( ((int*)data) + offset, mydims );
    }
  else if ( dal_FLOAT == itsDatatype ) {
    return num_util::makeNum(((float*)data)+offset,mydims);
  }
  else if ( dal_DOUBLE == itsDatatype ) {
    return num_util::makeNum(((double*)data)+offset,mydims);
  }
  else if ( dal_COMPLEX == itsDatatype ) {
    return num_util::makeNum(((std::complex<float>*)data)+offset,mydims);
  }
  else if ( dal_COMPLEX_CHAR == itsDatatype ) {
    return num_util::makeNum(((std::complex<char>*)data)+offset,mydims);
  }
  else if ( dal_COMPLEX_SHORT == itsDatatype ) {
    return num_util::makeNum(((std::complex<short>*)data)+offset,mydims);
  }
  else if ( dal_STRING == itsDatatype ) {
    bpl::list data_list;
    
    if ( 1 == itsShape.size() ) // 1D case
      {
	for (int ii=0; ii<nrows; ii++)
	  {
	    data_list.append( (*((std::string*)get(ii))) );
	  }
      }
    else if ( 2 == itsShape.size() ) // 2D case
      {
	for ( int xx=0; xx<itsShape[0]; xx++)
	  for ( int yy=0; yy<itsShape[1]; yy++)
	    data_list.append( (*((std::string*)get(xx,yy))) );
      }
    else if ( 3 == itsShape.size() ) // 3D case
      {
	for ( int xx=0; xx<itsShape[0]; xx++)
	  for ( int yy=0; yy<itsShape[1]; yy++)
	    for ( int zz=0; zz<itsShape[2]; zz++)
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
    std::cerr << "ERROR:  Datatype '" << itsDatatype
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

// ==============================================================================
//
//                                                                        IO_Mode
//
// ==============================================================================

void export_IO_Mode ()
{
  //________________________________________________________
  // Enumeration: I/O mode flags

  bpl::enum_<IO_Mode::Flags>("Flags")
    .value("ReadOnly",     IO_Mode::ReadOnly)
    .value("ReadWrite",    IO_Mode::ReadWrite)
    .value("WriteOnly",    IO_Mode::WriteOnly)
    .value("Open",         IO_Mode::Open)
    .value("OpenOrCreate", IO_Mode::OpenOrCreate)
    .value("Create",       IO_Mode::Create)
    .value("CreateNew",    IO_Mode::CreateNew)
    .value("Truncate",     IO_Mode::Truncate)
    ;
  
  //________________________________________________________
  // Specialisation of overloaded methods

  // std::vector<std::string> (IO_Mode::*names1)() 
  //   = &IO_Mode::names;
  // std::vector<std::string> (IO_Mode::*names2)(IO_Mode::Flags const &) 
  //   = &IO_Mode::names;
  // std::vector<std::string> (IO_Mode::*names3)(int const &) 
  //   = &IO_Mode::names;
  void (IO_Mode::*summary1)() 
    = &IO_Mode::summary;
  void (IO_Mode::*summary2)(std::ostream &) 
    = &IO_Mode::summary;
  
  //________________________________________________________
  // Bindings for class and its methods

  bpl::class_<IO_Mode>("IO_Mode")
    // Construction
    .def( bpl::init<>())
    .def( bpl::init<IO_Mode::Flags>())
    .def( bpl::init<int>())
    .def( bpl::init<IO_Mode>())
    // Parameter access
    .def("flags",
	 &IO_Mode::flags,
	 "Get object I/O mode flags.")
    .def("setFlag",
	 &IO_Mode::setFlag,
	 "Set object I/O mode flag.")
    .def("setFlags",
	 &IO_Mode::setFlags,
	 "Set object I/O mode flags. ")
    .def("addFlag",
	 &IO_Mode::addFlag,
	 "Add flag to the current seetings.")
    .def("removeFlag",
	 &IO_Mode::removeFlag,
	 "Remove flag from the current seetings.")
    .def("resetFlags",
	 &IO_Mode::resetFlags,
	 "Reset the object I/O mode flags.")
    // .def("names",
    // 	 &names1,
    // 	 "Get array containing the available flag names.")
    .def("className",
	 &IO_Mode::className,
	 "Get the name of the class.")
    // Methods
    .def("summary",
	 summary1,
	 "Summary of the object's internal parameters and status.")
    .def("summary",
	 summary2,
	 "Summary of the object's internal parameters and status.")
    ;
}

// ==============================================================================
//
//                                                                  HDF5Hyperslab
//
// ==============================================================================

void export_HDF5Hyperslab ()
{
  void (HDF5Hyperslab::*summary1)() 
    = &HDF5Hyperslab::summary;
  void (HDF5Hyperslab::*summary2)(std::ostream &) 
    = &HDF5Hyperslab::summary;
  
  bpl::class_<HDF5Hyperslab>("HDF5Hyperslab")
    .def( bpl::init<>())
    .def( bpl::init<int const &>())
    // Parameter access
    .def( "shape", &HDF5Hyperslab::rank,
	  "Get the rank of the array to which the hyperslab is applied.")
    .def( "start", &HDF5Hyperslab::start,
	  "Get the offset of the starting element of the specified hyperslab.")
    .def( "setStart", &HDF5Hyperslab::setStart,
	  "Set the offset of the starting element of the specified hyperslab.")
    .def( "stride", &HDF5Hyperslab::stride,
	  "Get the number of elements to separate each element or block.")
    .def( "setStride", &HDF5Hyperslab::setStride,
	  "Set the number of elements to separate each element or block.")
    .def( "count", &HDF5Hyperslab::count,
	  "Get the number of elements or blocks to select along each dimension.")
    .def( "setCount", &HDF5Hyperslab::setCount,
	  "Set the number of elements or blocks to select along each dimension.")
    .def( "block", &HDF5Hyperslab::block,
	  "Get the size of the element block selected from the dataspace.")
    .def( "setBlock", &HDF5Hyperslab::setBlock,
	  "Set the size of the element block selected from the dataspace.")
    // Methods
    .def( "className", &HDF5Hyperslab::className,
	  "Get the name of the class.")
    .def("summary", summary1)
    .def("summary", summary2)
    ; 
}
