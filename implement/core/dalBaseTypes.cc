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

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALBASETYPES_H
#include <dalBaseTypes.h>
#endif

namespace DAL {

  //_____________________________________________________________________________
  //                                                                     fileType
  
  /*!
    \param type  -- File type to convert to corresponding string
    \return name -- File type name.
  */
  std::string fileType (DAL::dalFileType const &type)
  {
    std::string name;

    switch (type) {
    case DAL::HDF5:
      name = "HDF5";
      break;
    case DAL::FITS:
      name = "FITS";
      break;
    case DAL::MSCASA:
      name = "MSCASA";
      break;
    default:
      name = "UNDEFINED";
      break;
    };
    
    return name;
  }

  //_____________________________________________________________________________
  //                                                                     fileType
  
  DAL::dalFileType fileType (std::string const &name)
  {
    DAL::dalFileType type = DAL::UNDEFINED;

    if (name == "HDF5") {
      type = DAL::HDF5;
    } else if (name == "FITS") {
      type = DAL::FITS;
    } else if (name == "MSCASA") {
      type = DAL::MSCASA;
    } else {
      type = DAL::UNDEFINED;
    }

    return type;
  }
  
}
