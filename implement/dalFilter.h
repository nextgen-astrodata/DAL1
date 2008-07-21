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

#ifndef DAL_H
#include "dal.h"
#endif


namespace DAL
  {

  /*!
    \class dalFilter

    \brief Superclass of dalDSFilter, dalColumnFilter, dalTableFilter)

    \ingroup DAL

    \author Joseph Masters
  */
  class dalFilter
    {
      string filterstring;  // Table filter string
      string filetype;  // MSCASA, HDF5, FITS, etc.
      bool is_set;
    public:

      dalFilter();
      void set( string columns );
      void set( string columns, string conditions );
      void setFiletype( string type );
      bool isSet();
      string get();
    };


} // DAL namespace

#endif  // DALFILTER_H
