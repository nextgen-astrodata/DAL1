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
  \file pydal_TBB_DipoleDataset.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::TBB_DipoleDataset class

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <data_hl/TBB_DipoleDataset.h>

using DAL::TBB_DipoleDataset;

// ==============================================================================
//
//                                                              TBB_DipoleDataset
//
// ==============================================================================

void export_TBB_DipoleDataset()
{
  void (TBB_DipoleDataset::*summary1)()                = &TBB_DipoleDataset::summary;
  void (TBB_DipoleDataset::*summary2)(std::ostream &)  = &TBB_DipoleDataset::summary;
  bool (TBB_DipoleDataset::*open1)(hid_t const &)      = &TBB_DipoleDataset::open;
  bool (TBB_DipoleDataset::*open2)(hid_t const &,
				   std::string const &,
				   DAL::IO_Mode const &) = &TBB_DipoleDataset::open;
  bool (TBB_DipoleDataset::*open3)(hid_t const &,
				   uint const &,
				   uint const &,
				   uint const &,
				   std::vector<hsize_t> const &,
				   hid_t const &)      = &TBB_DipoleDataset::open;
  int (TBB_DipoleDataset::*dipoleNumber1)()      = &TBB_DipoleDataset::dipoleNumber;
  std::string (TBB_DipoleDataset::*dipoleName1)()      = &TBB_DipoleDataset::dipoleName;
//   std::string (TBB_DipoleDataset::*dipoleName2)(unsigned int const &,
// 					     unsigned int const &,
// 					     unsigned int const &)
//     = &TBB_DipoleDataset::dipoleName;
  
  boost::python::class_<TBB_DipoleDataset>("TBB_DipoleDataset")
    /* Construction */
    .def( boost::python::init<>())
    .def( boost::python::init<hid_t const &, std::string const &>())
    .def( boost::python::init<hid_t const &, uint const &, uint const &, uint const &>())
    /* Access to internal parameters */
    .def( "nofAttributes", &TBB_DipoleDataset::nofAttributes,
	  "Get the number of attributes attached to the dataset." )
    .def( "julianDay", &TBB_DipoleDataset::julianDay,
	  "Get the time as Julian Day." )
    .def("summary", summary1,
	 "Provide a summary of the internal status.")
    .def("summary", summary2,
	 "Provide a summary of the internal status.")
    .def("open", open1,
	 "Open a dipole dataset.")
    .def("open", open2,
	 "Open a dipole dataset.")
    .def("open", open3,
	 "Open a dipole dataset.")
    .def( "dipoleNumber", dipoleNumber1,
	  "Get the unique channel/dipole identifier." )
    .def( "getName", dipoleName1,
	  "Get the unique channel/dipole identifier." )
//     .def( "getName", dipoleName2,
// 	  "Get the unique channel/dipole identifier." )
    ;
}
