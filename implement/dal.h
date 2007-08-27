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

  \author Joseph Masters
*/

#ifndef DAL_H
#define DAL_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>  // for cout field width
#include <cstdio>
#include "H5LT.h"

extern "C" {
#include "fitsio.h"
}

#include "convert.h"  // for stringify function

#ifndef DALBASETYPES_H
#include "dalBaseTypes.h"
#endif

using namespace std;

/*
   if creating python bindings
*/
#ifdef PYTHON
// #include <boost/python.hpp>
// #include <boost/python/list.hpp>
// namespace bpl = boost::python;
#include <boost/python.hpp>
#include <boost/python/object.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
namespace bpl = boost::python;
#endif

/*
   if there is a casa installation
*/
#ifdef WITH_CASA
#include <casa/aips.h>
#include <tables/Tables.h>
#include <tables/Tables/Table.h>
#include <ms/MeasurementSets.h>
#include <ms/MeasurementSets/MSReader.h>
#include <iostream>
#include <casa/aipstype.h>
#include <casa/complex.h>
#include <casa/BasicMath/Math.h>
#endif


const string H5TYPE = "HDF5";
const string FITSTYPE = "FITS";
const string MSCASATYPE = "MSCASA";

const int SUCCESS = 0;
const int FAIL = 1;
const int CHUNK_SIZE = 5000;
const short MAX_COL_NAME_SIZE = 256;

const string dal_CHAR = "dalCHAR";
const string dal_STRING = "dalSTRING";

const string dal_INT = "dalINT";
const string dal_UINT = "dalINT";
const string dal_SHORT = "dalSHORT";
const string dal_LONG = "dalLONG";

const string dal_FLOAT = "dalFLOAT";
const string dal_DOUBLE = "dalDOUBLE";

const string dal_COMPLEX = "dalCOMPLEX";
const string dal_DCOMPLEX = "dalDCOMPLEX";

typedef struct dalcomplex {
	double r;  // real
	double i;  // imaginary
} dalcomplex;

#endif
