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
  \class dalBaseTypes
  \ingroup DAL
  \brief Basic type definitions and byte swapping routines.
*/

#ifndef DALBASETYPES_H
#define DALBASETYPES_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>  // for cout field width
#include <cstdio>
#include <fstream>  // for file handle
#include <complex>  // for complex datatypes
#include <time.h>   // for local time conversion

#ifdef HAVE_HDF5
#include <hdf5.h>
#include <hdf5_hl.h>
#endif

#ifdef HAVE_CFITSIO
extern "C"
  {
#include <fitsio.h>
  }
#endif

/*
   if creating python bindings
*/
#ifdef PYTHON
#include <Python.h>
#include <boost/python.hpp>
#include <boost/python/object.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
namespace bpl = boost::python;
#include <num_util.h>
#endif

/*
   if there is a casa installation
*/
#ifdef HAVE_CASA
#include <casa/aips.h>
#include <tables/Tables.h>
#include <tables/Tables/Table.h>
#include <ms/MeasurementSets.h>
#include <ms/MeasurementSets/MSReader.h>
#include <casa/aipstype.h>
#include <casa/complex.h>
#include <casa/BasicMath/Math.h>
#include <tables/Tables/ScalarColumn.h>
#include <tables/Tables/TableParse.h>
#include <tables/Tables/TableKeyword.h>
#endif

// CHUNK_SIZE arbitrarily chosen, more research needed on this feature
const int32_t CHUNK_SIZE = 5000;
const int32_t MAX_COL_NAME_SIZE = 256;

// ----------------------------------------------------- USG code

namespace DAL
  {

  using namespace std;

  const std::string H5TYPE     = "HDF5";
  const std::string FITSTYPE   = "FITS";
  const std::string MSCASATYPE = "MSCASA";

  const bool SUCCESS = true;
  const bool FAIL = false;

  const std::string dal_CHAR   = "dalCHAR";
  const std::string dal_STRING = "dalSTRING";
  const std::string dal_BOOL   = "dalBOOL";

  const std::string dal_SHORT         = "dalSHORT";
  const std::string dal_INT           = "dalINT";
  const std::string dal_UINT          = "dalINT";
  const std::string dal_LONG          = "dalLONG";

  const std::string dal_FLOAT         = "dalFLOAT";
  const std::string dal_DOUBLE        = "dalDOUBLE";

  const std::string dal_COMPLEX       = "dalCOMPLEX";
  const std::string dal_DCOMPLEX      = "dalDCOMPLEX";
  const std::string dal_COMPLEX_CHAR  = "dalCOMPLEX_CHAR";
  const std::string dal_COMPLEX_SHORT = "dalCOMPLEX_SHORT";

  // define a few datatypes
  typedef unsigned char   UInt8;
  typedef unsigned short UInt16;
  typedef short           Int16;
  typedef unsigned int   UInt32;
  typedef int             Int32;
  typedef float         Float32;
  typedef long long       Int64;
  typedef double        Float64;

  struct dalcomplex
    {
      Float64 r;  // real
      Float64 i;  // imaginary
    };

  struct dalcomplex_float32
    {
      Float32 r;  // real
      Float32 i;  // imaginary
    };

  struct dalcomplex_int16
    {
      Int16 r;  // real
      Int16 i;  // imaginary
    };

  struct dalcomplex_char
    {
      char r;  // real
      char i;  // imaginary
    };

  bool BigEndian( void );
  void swapbytes(char *addr, int8_t nbytes);

} // DAL namespace

#ifndef CONVERT_H
#include <convert.h>  // for stringify function
#endif

#ifndef DALCOMMON_H
#include <dalCommon.h>
#endif

#endif // DALBASETYPES_H
