/***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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

#define PY_ARRAY_UNIQUE_SYMBOL PyArrayHandle

#include "pydal.h"

/*!
  \file pydal.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the C++ layer of the Data Acccess Library

  \author Joseph Masters, Lars B&auml;hren

  <h3>Synopsis</h3>

*/

BOOST_PYTHON_MODULE(pydal)
{
  boost::python::scope().attr("__doc__") =
    "Routines for python bindings to the data access library (DAL)."
    ;
  
  Py_Initialize();
  import_array();
  boost::python::numeric::array::set_module_and_type("numpy", "ndarray");

  // ============================================================================
  //
  //  [core] Core classes
  //
  // ============================================================================

  export_dalCommon();
  export_dalArray ();
  export_dalColumn ();
  export_dalData ();
  export_dalDataset ();
  export_dalGroup ();
  export_dalTable ();
  export_IO_Mode ();

  // ============================================================================
  //
  //  [coordinates] Representations of World Coordinates
  //
  // ============================================================================

  export_Angle ();  
  export_RaDec ();
  export_Stokes ();  
  export_Coordinate ();

  // ============================================================================
  //
  //  [data_common] Common dataset support
  //
  // ============================================================================
  
  export_CommonAttributes ();
  export_Filename ();
  export_HDF5Measure ();
  export_HDF5Quantity ();
  export_SAS_Settings ();
  export_Timestamp ();
  
  // ============================================================================
  //
  //  [data_hl] High-level interfaces to specific data
  //
  // ============================================================================
  
  export_BeamFormed ();
  export_BeamGroup ();
  export_BF_BeamGroup ();
  export_TBB_Timeseries ();
  export_TBB_StationGroup ();
  export_TBB_DipoleDataset ();  
  export_LOPES_EventFile ();
  
}
