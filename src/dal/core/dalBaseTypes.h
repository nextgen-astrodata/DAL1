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

/*!
  \file dalBaseTypes.h

  \ingroup DAL
  \ingroup core

  \brief Basic type definitions and byte swapping routines.

  \author Joseph Masters, Lars B&auml;hren

  \test tdalBaseTypes.cc
*/

#ifndef DALBASETYPES_H
#define DALBASETYPES_H

/* Standard header files */
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>  // for cout field width
#include <cstdio>
#include <fstream>  // for file handle
#include <complex>  // for complex datatypes
#include <time.h>   // for local time conversion

#include <dal_config.h>
#include <core/convert.h>
#include <core/dalCommon.h>

/*
   if creating python bindings
*/
#ifdef PYTHON
#include <Python.h>
namespace bpl = boost::python;
#endif

// CHUNK_SIZE arbitrarily chosen, more research needed on this feature
const int32_t CHUNK_SIZE        = 5000;
const int32_t MAX_COL_NAME_SIZE = 256;

#endif // DALBASETYPES_H
