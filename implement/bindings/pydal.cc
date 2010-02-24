/*------------------------------------------------------------------------*
| $Id::                                                                 $ |
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

#define PY_ARRAY_UNIQUE_SYMBOL PyArrayHandle

#include "dal.h"
#include "bindings/pydal.h"

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
  bpl::scope().attr("__doc__") =
    "Routines for python bindings to the data access library (DAL)."
    ;
  
  import_array();
  bpl::numeric::array::set_module_and_type("numpy", "ndarray");
  
  def( "mjd2unix", &mjd2unix_boost,
       "Convert Modified Julian Date (mjd) to unix time.\n"
       "The Unix base date is MJD 40587 and 1 mjd Day = 24 hours \n"
       "or 1440 minutes or 86400 seconds so: \n"
       "(unix seconds) = (mjd seconds) - ( unix base date in seconds )." );
  
  // ============================================================================
  //
  //  [core] Core classes
  //
  // ============================================================================

  export_dalArray ();
  export_dalColumn ();
  export_dalData ();
  export_dalDataset ();
  export_dalGroup ();
  export_dalTable ();  

  // ============================================================================
  //
  //  [data_common] Common dataset support
  //
  // ============================================================================
  
  export_Filename ();
  export_CommonAttributes ();
  export_Timestamp ();
  export_SAS_Settings ();
  export_HDF5Hyperslab ();
  
  // ============================================================================
  //
  //  [data_hl] High-level interfaces to specific data
  //
  // ============================================================================
  
  export_BeamFormed ();
  export_BeamGroup ();
  export_TBB_Timeseries ();
  export_TBB_StationGroup ();
  export_TBB_DipoleDataset ();  
  export_LOPES_EventFile ();
  
}
