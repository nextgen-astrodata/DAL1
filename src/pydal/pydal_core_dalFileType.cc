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
  \file pydal_core_dalArray.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::dalFileType class

  \author Lars B&auml;hren
*/

#include "pydal.h"
#include <core/dalFileType.h>

using DAL::dalFileType;

// ==============================================================================
//
//                                                      Wrapper for class methods
//
// ==============================================================================

void export_dalFileType ()
{  
  //________________________________________________________
  // Enumeration: Stokes component

  boost::python::enum_<dalFileType::Type>("Type")
    .value("UNDEFINED",  dalFileType::UNDEFINED)
    .value("FITS",       dalFileType::FITS)
    .value("HDF5",       dalFileType::HDF5)
    .value("HDF5",       dalFileType::HDF5)
    .value("CASA_MS",    dalFileType::CASA_MS)
    .value("CASA_IMAGE", dalFileType::CASA_IMAGE)
    ;

  //________________________________________________________
  // Specialisation of overloaded methods

  bool (dalFileType::*setType1)(dalFileType::Type const &) 
    = &dalFileType::setType;
  bool (dalFileType::*setType2)(std::string const &) 
    = &dalFileType::setType;
  void (dalFileType::*summary1)() 
    = &dalFileType::summary;
  void (dalFileType::*summary2)(std::ostream &) 
    = &dalFileType::summary;

  //________________________________________________________
  // Bindings for class and its methods

  boost::python::class_<dalFileType>("dalFileType")
    // Construction
    .def( boost::python::init<>())
    .def( boost::python::init<dalFileType::Type const &>())
    .def( boost::python::init<std::string const &>())
    .def( boost::python::init<dalFileType const &>())
    // Parameter access
    .def("type",
	 &dalFileType::type,
	 "Get file type.")
    .def("setType",
	 setType1,
	 "Set the file type.")
    .def("setType",
	 setType2,
	 "Set the file type.")
    .def("name",
	 &dalFileType::name,
	 "Get file type as name.")
    .def("isFITS",
    	 &dalFileType::isFITS,
    	 "Is the file of type FITS?")
    .def("isHDF5",
    	 &dalFileType::isHDF5,
    	 "Is the file of type HDF5?")
    .def("isCASA",
    	 &dalFileType::isCASA,
    	 "Is the file of type CASA?")
    .def("className",
	 &dalFileType::className,
	 "Get the name of the class.")
    // Public methods
    .def("summary",
	 summary1,
	 "Summary of the object's internal parameters and status.")
    .def("summary",
	 summary2,
	 "Summary of the object's internal parameters and status.")
    // Static methods 
    .staticmethod("getMap")
    .staticmethod("getType")
    .staticmethod("getName")
    ;
}

