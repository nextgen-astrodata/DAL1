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
  \file pydal_data_hl.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings DAL C++ classes in the data_hl directory

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <data_hl/SysLog.h>
#include <data_common/CommonAttributes.h>
#include <data_hl/TBB_StationGroup.h>
#include <data_hl/LOPES_EventFile.h>

using DAL::LOPES_EventFile;
using DAL::TBB_StationGroup;

// ==============================================================================
//
//                                                               TBB_StationGroup
//
// ==============================================================================

void export_TBB_StationGroup () 
{
  boost::python::class_<TBB_StationGroup>("TBB_StationGroup")
    /* Construction */
    .def( boost::python::init<>())
    .def( boost::python::init<uint,string>())
    .def( boost::python::init<uint>())
    /* Access to internal parameters */
    .def( "group_name", &TBB_StationGroup::group_name,
	  "Get the name for this group within the HDF5 file.")
    ;
}

// ==============================================================================
//
//                                                                LOPES_EventFile
//
// ==============================================================================

void export_LOPES_EventFile ()
{
  /* Enumeration: Event type */
  boost::python::enum_<LOPES_EventFile::EvType>("EvType")
    .value("Unspecified",LOPES_EventFile::Unspecified)
    .value("Cosmic",LOPES_EventFile::Cosmic)
    .value("Simulation",LOPES_EventFile::Simulation)
    .value("Test",LOPES_EventFile::Test)
    .value("SolarFlare",LOPES_EventFile::SolarFlare)
    .value("Other",LOPES_EventFile::Other)
    ;
  
  /* Enumeration: Observatory */
  boost::python::enum_<LOPES_EventFile::Observatory>("Observatory")
    .value("LOPES",LOPES_EventFile::LOPES)
    .value("LORUN",LOPES_EventFile::LORUN)
    ;
  
  boost::python::class_<LOPES_EventFile>("LOPES_EventFile")
    .def( boost::python::init<>())
    .def( boost::python::init<string>())
    .def( "filename", &LOPES_EventFile::filename,
	  "Get the name of the data file." )
    .def( "samplerate", &LOPES_EventFile::samplerate,
	  "Get the samplerate of the A/D conversion." )
    .def( "nyquistZone", &LOPES_EventFile::nyquistZone,
	  "Get the Nyquist zone in which the data are sampled." )
    .def( "nofAntennas", &LOPES_EventFile::nofAntennas,
	  "Get the number of antennas in the data set." )
#ifdef DAL_WITH_CASACORE
    .def( "nofDatapoints", &LOPES_EventFile::nofDatapoints,
	  "Get the number of data points stored." )
#endif
    ;
}
