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
#ifndef DALFILTER_H
#define DALFILTER_H

#ifndef DALBASETYPES_H
#include "dalBaseTypes.h"
#endif

namespace DAL
  {

  /*!
    \class dalFilter
    \brief Class representing a filter that can be applied to a table.
    \ingroup DAL
  */

  class dalFilter
    {
      std::string filterstring;  // Table filter std::string
      std::string filetype;  // MSCASA, HDF5, FITS, etc.
      bool is_set;

    public:

      dalFilter();

      dalFilter( std::string type,
                 std::string columns );

      dalFilter( std::string type,
                 std::string columns,
                 std::string conditions );

      void set( std::string columns );
      void set( std::string columns, std::string conditions );
      void setFiletype( std::string type );
      bool isSet();
      std::string get();
    };


} // DAL namespace

#endif  // DALFILTER_H
