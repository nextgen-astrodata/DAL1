/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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
  \file pydal_HDF5Hyperslab.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::HDF5Hyperslab class

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <core/HDF5Hyperslab.h>

// namespace usage
using DAL::HDF5Hyperslab;


void export_HDF5Hyperslab ()
{

  //________________________________________________________
  // Specialisation of overloaded methods

  void (HDF5Hyperslab::*summary1)() 
    = &HDF5Hyperslab::summary;
  void (HDF5Hyperslab::*summary2)(std::ostream &) 
    = &HDF5Hyperslab::summary;
  
  //________________________________________________________
  // Bindings for class and its methods

  boost::python::class_<HDF5Hyperslab>("HDF5Hyperslab")
    .def( boost::python::init<>())
    .def( boost::python::init<int const &>())
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
