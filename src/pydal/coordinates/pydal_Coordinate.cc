/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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
  \file pydal_Coordinate.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::Coordinate class

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <coordinates/Coordinate.h>

using DAL::Coordinate;

// ==============================================================================
//
//                                                                DAL::Coordinate
//
// ==============================================================================

void export_Coordinate ()
{
  //________________________________________________________
  // Enumeration: Cordinate type

  boost::python::enum_<Coordinate::Type>("Type")
    .value("DIRECTION", DAL::Coordinate::DIRECTION)
    .value("LINEAR",    DAL::Coordinate::LINEAR)
    .value("TABULAR",   DAL::Coordinate::TABULAR)
    .value("STOKES",    DAL::Coordinate::STOKES)
    .value("SPECTRAL",  DAL::Coordinate::SPECTRAL)
    .value("UNDEFINED", DAL::Coordinate::UNDEFINED)
    ;
  
  //________________________________________________________
  // Bindings for class and its methods


}
