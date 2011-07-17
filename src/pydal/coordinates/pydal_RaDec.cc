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
  \file pydal_coordinates.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::RaDec class

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <coordinates/RaDec.h>

using DAL::RaDec;

// ==============================================================================
//
//                                                                          RaDec
//
// ==============================================================================

void export_RaDec ()
{
  //________________________________________________________
  // Specialisation of overloaded methods

  void (RaDec::*summary1)() 
    = &RaDec::summary;
  void (RaDec::*summary2)(std::ostream &) 
    = &RaDec::summary;

  //________________________________________________________
  // Bindings for class and its methods

  boost::python::class_<RaDec>("RaDec")
    // Construction
    .def( boost::python::init<>())
    .def( boost::python::init<double const &, double const &, bool const &>())
    .def( boost::python::init<RaDec>())
    // Parameter access
    .def("setRA",    &RaDec::setRA,    "Set Right Ascension angle.")
    .def("setDec",   &RaDec::setDec,    "Set Declination angle.")
    .def("rad",      &RaDec::rad,      "Get (RA,Dec) in radian.")
    .def("deg",      &RaDec::deg,      "Get (RA,Dec) in degrees.")
    .def("hms",      &RaDec::hms,      "Get (RA,Dec) as formatted string (HH:MM:SS).")
    .def("className",
	 &RaDec::className,
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
