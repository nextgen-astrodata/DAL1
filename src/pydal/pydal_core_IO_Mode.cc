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
  \file pydal_core_IO_Mode.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings DAL C++ classes in the \e core directory

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <core/IO_Mode.h>

// namespace usage
using DAL::IO_Mode;

// ==============================================================================
//
//                                                                        IO_Mode
//
// ==============================================================================

void export_IO_Mode ()
{
  //________________________________________________________
  // Enumeration: I/O mode flags

  boost::python::enum_<IO_Mode::Flags>("Flags")
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

  boost::python::class_<IO_Mode>("IO_Mode")
    // Construction
    .def( boost::python::init<>())
    .def( boost::python::init<IO_Mode::Flags>())
    .def( boost::python::init<int>())
    .def( boost::python::init<IO_Mode>())
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
