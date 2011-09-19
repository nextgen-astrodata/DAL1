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

#ifndef PYDAL_H
#define PYDAL_H

#include "num_util.h"

/*!
  \file pydal.h
  
  \ingroup DAL
  \ingroup pyrap
  
  \brief Python bindings for the C++ layer of the Data Acccess Library
  
  \author Joseph Masters, Lars B&auml;hren

  <h3>Synopsis</h3>

*/

  // ============================================================================
  //
  //  Conversion routines between different array types
  //
  // ============================================================================

namespace DAL {   //   BEGIN -- namespace DAL

  //! Convert array to Boost.Python numeric array
  template <class T>
    boost::python::numeric::array toNumericArray (T *data,
						  unsigned int const &nelem)
    {
      std::vector<int> mydims (1,nelem);
      boost::python::numeric::array narray = num_util::makeNum (data, mydims );
      return narray;
    }
  
  //! Convert std::vector<T> to Boost.Python numeric array
  template <class T>
    boost::python::numeric::array toNumericArray (std::vector<T> const &data)
    {
      std::vector<int> mydims (1,data.size());
      boost::python::numeric::array narray = num_util::makeNum (&data[0], mydims );
      return narray;
    }

};   //   END -- namespace DAL

  // ============================================================================
  //
  //  [core] Core classes
  //
  // ============================================================================

#include <core/dalData.h>

//! Bindings for DAL::dalArray
void export_dalArray ();
//! Bindings for DAL::dalColumn
void export_dalColumn ();
//! Bindings for the methods in dalCommon
void export_dalCommon();
//! Bindings for DAL::dalData
void export_dalData ();
//! Bindings for DAL::dalDataset
void export_dalDataset ();
//! Bindings for DAL::dalGroup
void export_dalGroup ();
//! Bindings for DAL::dalTable
void export_dalTable ();
//! Bindings for DAL::IO_Mode
void export_IO_Mode ();

  // ============================================================================
  //
  //  [coordinates] Representations of World Coordinates
  //
  // ============================================================================

//! Bindings for DAL::Angle
void export_Angle ();
//! Bindings for DAL::RaDec
void export_RaDec ();
//! Bindings for DAL::Stokes
void export_Stokes ();
//! Bindings for DAL::Coordinate
void export_Coordinate ();

  // ============================================================================
  //
  //  [data_common] Common dataset support
  //
  // ============================================================================
  
//! Bindings for DAL::CommonAttributes
void export_CommonAttributes ();
//! Bindings for DAL::Filename
void export_Filename ();
//! Bindings for DAL::HDF5Measure
void export_HDF5Measure ();
//! Bindings for DAL::HDF5Quantity
void export_HDF5Quantity ();
//! Bindings for DAL::SAS_Settings
void export_SAS_Settings ();
//! Bindings for DAL::Timestamp
void export_Timestamp ();

  // ============================================================================
  //
  //  [data_hl] High-level interfaces to specific data
  //
  // ============================================================================
  
//! Bindings for DAL::BeamFormed
void export_BeamFormed();
//! Bindings for DAL::BeamGroup
void export_BeamGroup();
//! Bindings for DAL::BF_BeamGroup
void export_BF_BeamGroup();
//! Bindings for DAL::TBB_Timeseries
void export_TBB_Timeseries();
//! Bindings for DAL::TBB_StationGroup
void export_TBB_StationGroup();
//! Bindings for DAL::TBB_DipoleDataset
void export_TBB_DipoleDataset();
//! Bindings for DAL::LOPES_EventFile
void export_LOPES_EventFile();

#define PY_ARRAY_UNIQUE_SYMBOL PyArrayHandle
#define NO_IMPORT_ARRAY

#endif
