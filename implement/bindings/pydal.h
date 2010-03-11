/*------------------------------------------------------------------------*
| $Id:: pywrapper.cc 4174 2010-02-17 11:48:38Z baehren                  $ |
*-------------------------------------------------------------------------*
***************************************************************************
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

// Boost.Python headers
#include <boost/python.hpp>

#include <dal.h>
#include <dalCommon.h>

using namespace DAL;

/*!
  \file pydal.h
  
  \ingroup DAL
  \ingroup pyrap
  
  \brief Python bindings for the C++ layer of the Data Acccess Library
  
  \author Joseph Masters, Lars B&auml;hren

  <h3>Synopsis</h3>
  
*/

// === core =====================================================================

//! Bindings for the methods in dalCommon
void export_dalCommon();

//! Bindings for DAL::dalArray
void export_dalArray ();
//! Bindings for DAL::dalColumn
void export_dalColumn ();
//! Bindings for DAL::dalData
void export_dalData ();
//! Bindings for DAL::dalDataset
void export_dalDataset ();
//! Bindings for DAL::dalGroup
void export_dalGroup ();
//! Bindings for DAL::dalTable
void export_dalTable ();

// === coordinates ==============================================================



// === data_common ==============================================================

//! Bindings for DAL::Filename
void export_Filename ();
//! Bindings for DAL::CommonAttributes
void export_CommonAttributes ();
//! Bindings for DAL::Timestamp
void export_Timestamp ();
//! Bindings for DAL::SAS_Settings
void export_SAS_Settings ();
//! Bindings for DAL::HDF5Hyperslab
void export_HDF5Hyperslab ();

// === data_hl ==================================================================

//! Bindings for DAL::BeamFormed
void export_BeamFormed();
//! Bindings for DAL::BeamGroup
void export_BeamGroup();
//! Bindings for DAL::TBB_Timeseries
void export_TBB_Timeseries();
//! Bindings for DAL::TBB_StationGroup
void export_TBB_StationGroup();
//! Bindings for DAL::TBB_DipoleDataset
void export_TBB_DipoleDataset();
//! Bindings for DAL::LOPES_EventFile
void export_LOPES_EventFile();
