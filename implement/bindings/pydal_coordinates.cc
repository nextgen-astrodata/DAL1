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
  \file pydal_coordinates.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings DAL C++ classes in the \e coordinates directory

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <Stokes.h>

// ==============================================================================
//
//                                                                         Stokes
//
// ==============================================================================

void export_Stokes ()
{
  //________________________________________________________
  // Enumeration: Stokes component

  bpl::enum_<Stokes::Component>("Component")
    .value("I", Stokes::I)
    .value("Q", Stokes::Q)
    .value("U", Stokes::U)
    .value("V", Stokes::V)
    .value("R", Stokes::R)
    .value("L", Stokes::L)
    .value("RR",Stokes::RR)
    .value("LL",Stokes::LL)
    .value("RL",Stokes::RL)
    .value("LR",Stokes::LR)
    .value("X", Stokes::X)
    .value("Y", Stokes::Y)
    .value("XX",Stokes::XX)
    .value("YY",Stokes::YY)
    .value("XY",Stokes::XY)
    .value("YX",Stokes::YX)
    ;
  
  //________________________________________________________
  // Specialisation of overloaded methods

  bool (Stokes::*setType1)(Stokes::Component const &) 
    = &Stokes::setType;
  bool (Stokes::*setType2)(std::string const &) 
    = &Stokes::setType;
  void (Stokes::*summary1)() 
    = &Stokes::summary;
  void (Stokes::*summary2)(std::ostream &) 
    = &Stokes::summary;
  bool (Stokes::*isLinear1)() 
    = &Stokes::isLinear;
  // bool (Stokes::*isLinear2)(Stokes::Component const &) 
  //   = &Stokes().isLinear;
  bool (Stokes::*isCircular1)() 
    = &Stokes::isCircular;
  // static bool (Stokes::*isCircular2)(Stokes::Component const &) 
  //   = &Stokes::isCircular;
  bool (Stokes::*isParallel1)() 
    = &Stokes::isParallel;
  // static bool (Stokes::*isParallel2)(Stokes::Component const &) 
  //   = &Stokes::isParallel;
  
  //________________________________________________________
  // Bindings for class and its methods

  bpl::class_<Stokes>("Stokes")
    // Construction
    .def( bpl::init<>())
    .def( bpl::init<Stokes::Component>())
    .def( bpl::init<Stokes>())
    // Parameter access
    .def("type",
	 &Stokes::type,
	 "Get the type of the Stokes component.")
    .def("setType",
	 setType1,
	 "Set the Stokes component by type.")
    .def("setType",
	 setType2,
	 "Set the Stokes component by name.")
    .def("name",
	 &Stokes::name,
	 "Get the name of the Stokes component.")
    .def("className",
	 &Stokes::className,
	 "Get the name of the class.")
    // Methods
    .def("summary",
	 summary1,
	 "Summary of the object's internal parameters and status.")
    .def("summary",
	 summary2,
	 "Summary of the object's internal parameters and status.")
    .def("isLinear",
	 isLinear1,
	 "Is the Stokes component linear?")
    // .def("isLinear",
    // 	 isLinear2,
    // 	 "Is the Stokes component linear?")
    .def("isCircular",
	 isCircular1,
	 "Is the Stokes component circular?")
    .def("isParallel",
	 isParallel1,
	 "Is this a parallel combination of Stokes components?")
    ;
  
}
