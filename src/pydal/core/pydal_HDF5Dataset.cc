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
  \file pydal_HDF5Dataset.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::HDF5Dataset class

  \author Lars B&auml;hren
*/

// DAL headers
#include <pydal.h>
#include <core/HDF5Dataset.h>

using DAL::HDF5Dataset;

// ==============================================================================
//
//                                                     Additional Python wrappers
//
// ==============================================================================

// ==============================================================================
//
//                                                      Wrapper for class methods
//
// ==============================================================================

void export_HDF5Dataset ()
{
  void (HDF5Dataset::*summary1)() 
    = &HDF5Dataset::summary;
  void (HDF5Dataset::*summary2)(std::ostream &) 
    = &HDF5Dataset::summary;
  
  boost::python::class_<HDF5Dataset>("HDF5Dataset")
    // Construction
    .def( boost::python::init<>())
    .def( boost::python::init<hid_t const &,
			      std::string const &,
			      DAL::IO_Mode const &>())
    .def( boost::python::init<hid_t const &,
			      std::string const &,
			      std::vector<hsize_t> const &,
			      hid_t const &,
			      DAL::IO_Mode const &>())
    .def( boost::python::init<hid_t const &,
			      std::string const &,
			      std::vector<hsize_t> const &,
			      std::vector<hsize_t> const &,
			      hid_t const &,
			      DAL::IO_Mode const &>())
    .def( boost::python::init<HDF5Dataset>())
    // Parameter access
    .def("name",
	 &HDF5Dataset::name,
	 "Get the name of the dataset.")
    .def("shape",
	 &HDF5Dataset::shape,
	 "Get the shape of the dataset.")
    .def("layout",
	 &HDF5Dataset::layout,
	 "Get the layout of the raw data of the dataset.")
    .def("chunking",
	 &HDF5Dataset::chunking,
	 "Get the chunking size.")
    .def("rank",
	 &HDF5Dataset::rank,
	 "Get the rank (i.e. the number of axes) of the dataset.")
    .def("nofAxes",
	 &HDF5Dataset::nofAxes,
	 "Get the number of axes (i.e. the rank) of the dataset.")
    .def("nofDatapoints",
	 &HDF5Dataset::nofDatapoints,
	 "Get the nof. datapoints (i.e. array elements) of the dataset.")
    .def("dataspaceID",
	 &HDF5Dataset::dataspaceID,
	 "Get the dataspace identifier.")
    .def("datatypeID",
	 &HDF5Dataset::datatypeID,
	 "Get the datatype identifier.")
    .def("className",
	 &HDF5Dataset::className,
	 "Get the name of the class.")
    // Methods
    .def("summary",
	 summary1,
	 "Summary of the object's internal parameters and status.")
    .def("summary",
	 summary2,
	 "Summary of the object's internal parameters and status.")
    // Static Public Member Functions
    .staticmethod("offset")
    ;
}
