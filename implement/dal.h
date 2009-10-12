/*-------------------------------------------------------------------------*
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

/*!
  \file dal.h

  \brief Common header file for the Data Access Library (DAL)

  \defgroup DAL Data Access Library (DAL)

  \author Joseph Masters, Lars B&auml;hren

  <h3>Synopsis</h3>

  The Data Access Library (DAL) is a library that abstracts the underlying
  file format from the user when working with radio astronomy data. Underlying
  formats may include \ref dal_hdf5, AIPS++/CASA tables, \ref dal_fits or specific
  raw formats. This library is available to both the C/C++ developer and the
  Python user via the \e pydal module.

  <table border="0">
    <tr>
      <td class="indexkey">Core library classes</td>
      <td class="indexkey">Common dataset support</td>
      <td class="indexkey">High-level dataset interfaces</td>
      <td class="indexkey">Application tools</td>
    </tr>
    <tr valign="top">
      <td>
      - DAL::dalArray
      - DAL::dalColumn
      - DAL::dalData
      - DAL::dalDataset
      - DAL::dalFilter
      - DAL::dalGroup
      - DAL::dalTable
      </td>
      <td>
      - DAL::CommonAttributes
      - DAL::Coordinate
      - DAL::DirectionCoordinate
      - DAL::LinearCoordinate
      - DAL::StokesCoordinate
      - DAL::TabularCoordinate
      - DAL::CoordinatesGroup
      </td>
      <td>
      - DAL::BeamFormed
      - DAL::BeamGroup
      - DAL::BeamSubband
      - DAL::TBB_Timeseries
      - DAL::TBB_StationGroup
      - DAL::TBB_DipoleDataset
      </td>
      <td>
      - bf2h5.cpp
      - tbb2h5.cpp
      </td>
    </tr>
  </table>

  Officially, the DAL supports three file types:
  
  <ul>
    <li><b>HDF5</b> <br>
    Hierarchical Data Format Version 5 (HDF5), is a relatively recent format
    designed with particular attention to efficient operation on extremely large
    data sets.  HDF5 is the intended format for LOFAR data products.  The reasons
    for this decision are given in "Proposed Data Formats"<br>
    HDF5 is organized much like a unix file system.  The internal structure consists
    of a root group, '/', which may recursively contain subgroups.  Each of these
    groups may, in turn, contain other groups or n-dimensional arrays of simple or
    complex datatypes.  HDF5 tables are particular case of a HDF5 array and thus are
    treated in a similar manner.  Almost all objects in a HDF5 file can hold
    associated metadata in the form of an attribute.  For an abbreviated list of
    HDF5 features see "HDF5 vs. CASA Tables".  For a complete
    list see the hdfgroup website.<br>
    HDF5 files can be both read and written by the DAL.

    <li><b>CASA Tables</b> <br>
    CASA Tables in general, and Measurement Sets in particular, are the primary data
    product of the Westerbork Synthesis Radio Telescope (WSRT).  As WSRT is also
    based at ASTRON (where there is a large community of experienced CASA users),
    there is naturally a strong interest and advantage in building upon this
    expertise for LOFAR. <br>
    CASA tables can currently be read, but not written, by the DAL.

    <li><b>FITS</b> <br>
    FITS is the standard astronomical image format.  Among its advantages for
    inclusion in the DAL is its widespread use and extensive toolset.  Among its
    limitations is the fact that it was not designed for large and complex
    datasets. <br>
    FITS is the lowest of priority of the three supported formats and currently
    has almost no actual support in the DAL.
  </ul>

  At present, each of these file types have varying levels of support.

  <h3>Related pages</h3>

  - \ref dal_coding_guidelines
  - \ref dal_coding_cpp
    - \ref dal_code_tree
    - \ref dal_compile_link
  - \ref dal_coding_python
  - \ref dal_changes

*/

#ifndef DAL_H
#define DAL_H

#ifndef DALCOMMON_H
#include "dalCommon.h"
#endif

#ifndef BEAMFORMED_H
#include "BeamFormed.h"
#endif

#ifndef DALDATASET_H
#include "dalDataset.h"
#endif

#ifdef HAVE_MYSQL
#ifndef DATABASE_H
#include "Database.h"
#endif
#endif

#ifndef TBB_TIMESERIES_H
#include "TBB_Timeseries.h"
#endif

#endif // DAL_H
