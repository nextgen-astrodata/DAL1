/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1964 2008-09-06 17:52:38Z baehren                   $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

#ifndef DATATYPE_H
#include "DataType.h"
#endif

namespace DAL   // Namespace DAL -- begin
  {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  DataType::DataType (DataType::Types const &type)
  {
    setType (type);
  }

  DataType::DataType (DataType const &other)
  {
    copy (other);
  }

  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  DataType::~DataType ()
  {
    destroy();
  }

  void DataType::destroy ()
  {
    ;
  }

  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================

  DataType& DataType::operator= (DataType const &other)
  {
    if (this != &other)
      {
        destroy ();
        copy (other);
      }
    return *this;
  }

  void DataType::copy (DataType const &other)
  {
    type_p = other.type_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         name

  std::string DataType::name ()
  {
    std::string typeName;

    switch (type_p)
      {
      case DataType::VIS:
        typeName="VIS";
        break;
      case DataType::SKY:
        typeName="SKY";
        break;
      case DataType::RMC:
        typeName="RMC";
        break;
      case DataType::RMM:
        typeName="RMM";
        break;
      case DataType::DYS:
        typeName="DYS";
        break;
      case DataType::CRC:
        typeName="CRC";
        break;
      case DataType::BFS:
        typeName="BFS";
        break;
      case DataType::BFT:
        typeName="BFT";
        break;
      case DataType::TBB:
        typeName="TBB";
        break;
      };

    return typeName;
  }

  //_____________________________________________________________________________
  //                                                                      summary

  void DataType::summary (std::ostream &os)
  {
    os << "[DataType] Summary of internal parameters." << std::endl;
    os << "-- Type           = " << type() << std::endl;
    os << "-- Name           = " << name() << std::endl;
    os << "-- Is Visibility? = " << isVisibility() << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  bool DataType::isVisibility ()
  {
    switch (type_p)
      {
      case DataType::VIS:
        return true;
        break;
      default:
        return false;
        break;
      };
  }

} // Namespace DAL -- end
