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
  \file pydal_Angle.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::Angle class

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <coordinates/Angle.h>

using DAL::Angle;

// ==============================================================================
//
//                                                                          Angle
//
// ==============================================================================

void export_Angle ()
{
  //________________________________________________________
  // Specialisation of overloaded methods

  void (Angle::*summary1)() 
    = &Angle::summary;
  void (Angle::*summary2)(std::ostream &) 
    = &Angle::summary;

  //________________________________________________________
  // Bindings for class and its methods

  boost::python::class_<Angle>("Angle")
    // Construction
    .def( boost::python::init<>())
    .def( boost::python::init<double const &, bool const &>())
    .def( boost::python::init<Angle>())
    // Parameter access
    .def("setAngle", &Angle::setAngle, "Set the angle.")
    .def("rad",      &Angle::rad,      "Get the angle in radian.")
    .def("deg",      &Angle::deg,      "Get the angle in degrees.")
    .def("hms",      &Angle::hms,      "Get the angle as formatted string (HH:MM:SS)")
    .def("className",
	 &Angle::className,
	 "Get the name of the class.")
    // Methods
    .def("summary",
	 summary1,
	 "Summary of the object's internal parameters and status.")
    .def("summary",
	 summary2,
	 "Summary of the object's internal parameters and status.")
    // Static Public Member Functions
    ;
}
