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
  \ingroup DAL

  \brief Basic type definitions and byte swapping routines.

  \author Joseph Masters

  \date 15-Mar-07
*/
 
#ifndef DALBASETYPES_H
#define DALBASETYPES_H
 
 // reading a complete binary file
#include <iostream> // for cout
#include <iomanip>  // for cout field width
#include <fstream>  // for file handle
#include <complex>  // for complex datatypes
#include <time.h>		// for local time conversion

using namespace std;
using std::complex;

// define a few datatypes
typedef unsigned char   UInt8;
typedef unsigned short UInt16;
typedef short           Int16;
typedef unsigned int   UInt32;
typedef int             Int32;
typedef float         Float32;
typedef long            Int64;
typedef double        Float64;

bool BigEndian( void );

Int16 Int16Swap ( Int16 ss );
Int32 Int32Swap ( Int32 ii );
Int64 Int64Swap ( Int64 ff );

#endif
