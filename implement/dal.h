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

  \ingroup DAL

  \brief Common header file for the Data Access Library (DAL)

  \author Joseph Masters, Lars B&auml;hren
*/

#ifndef DAL_H
#define DAL_H

//_______________________________________________________________________________
//                                                               DAL header files

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

#ifndef TBB_STATIONGROUP_H
#include "TBB_StationGroup.h"
#endif

//_______________________________________________________________________________
//                                         HDF5 compatibility macros to version 2

#ifdef HDF5_MACROS_VERS2

// Attribute interface

#ifdef H5Acreate_vers
#undef H5Acreate_vers
#endif
#define H5Acreate_vers = 2

#ifdef H5Adelete_vers
#undef H5Adelete_vers
#endif
#define H5Adelete_vers = 2

#ifdef H5Aiterate_vers
#undef H5Aiterate_vers
#endif
#define H5Aiterate_vers = 2

#ifdef H5Arename_vers
#undef H5Arename_vers
#endif
#define H5Arename_vers = 2

// Dataset interface

#ifdef H5Dcreate_vers
#undef H5Dcreate_vers
#endif
#define H5Dcreate_vers = 2

#ifdef H5Dopen_vers
#undef H5Dopen_vers
#endif
#define H5Dopen_vers = 2

// Error interface

#ifdef H5Eset_auto_vers
#undef H5Eset_auto_vers
#endif
#define H5Eset_auto_vers = 2

#ifdef H5Eclear_vers
#undef H5Eclear_vers
#endif
#define H5Eclear_vers = 2

#ifdef H5Eprint_vers
#undef H5Eprint_vers
#endif
#define H5Eprint_vers = 2

#ifdef H5Eget_auto_vers
#undef H5Eget_auto_vers
#endif
#define H5Eget_auto_vers = 2

#ifdef H5Ewalk_vers
#undef H5Ewalk_vers
#endif
#define H5Ewalk_vers = 2

// Group interface

#ifdef H5Gcreate_vers
#undef H5Gcreate_vers
#endif
#define H5Gcreate_vers = 2

#ifdef H5Gopen_vers
#undef H5Gopen_vers
#endif
#define H5Gopen_vers = 2

// Properties interface

#ifdef H5Pinsert_vers
#undef H5Pinsert_vers
#endif
#define H5Pinsert_vers = 2

#ifdef H5Pregister_vers
#undef H5Pregister_vers
#endif
#define H5Pregister_vers = 2

#ifdef H5Pget_filter_vers
#undef H5Pget_filter_vers
#endif
#define H5Pget_filter_vers = 2

// Register interface

#ifdef H5Rget_obj_type_vers
#undef H5Rget_obj_type_vers
#endif
#define H5Rget_obj_type_vers = 2

// Datatype interface

#ifdef H5Tarray_create_vers
#undef H5Tarray_create_vers
#endif
#define H5Tarray_create_vers = 2

#ifdef H5Tget_array_dims_vers
#undef H5Tget_array_dims_vers
#endif
#define H5Tget_array_dims_vers = 2

#ifdef H5Tcommit_vers
#undef H5Tcommit_vers
#endif
#define H5Tcommit_vers = 2

#ifdef H5Topen_vers
#undef H5Topen_vers
#endif
#define H5Topen_vers = 2 

#endif

#endif // DAL_H
