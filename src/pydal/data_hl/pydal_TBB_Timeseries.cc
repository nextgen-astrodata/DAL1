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
  \file pydal_TBB_Timeseries.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::TBB_Timeseries class

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <data_hl/TBB_Timeseries.h>

using DAL::TBB_Timeseries;

// ==============================================================================
//
//                                                                 TBB_Timeseries
//
// ==============================================================================

void export_TBB_Timeseries ()
{  
  boost::python::class_<TBB_Timeseries>("TBB_Timeseries")
    /* Construction */
    .def( boost::python::init<>())
    .def( boost::python::init<std::string>())
    /* Access to internal parameters */
    .def( "filename", &TBB_Timeseries::filename,
	  "Get the name of the data file." )
    .def( "locationID", &TBB_Timeseries::locationID,
	  "Get the object identifier for the data file." )
    .def( "nofStationGroups", &TBB_Timeseries::nofStationGroups,
	  "Get the number of station groups collected into this file." )
    .def( "nofDipoleDatasets", &TBB_Timeseries::nofDipoleDatasets,
	  "Get the number of dipole datasets collected into this file." )
    ;
}
