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

#ifndef PYDAL_CONVERSIONS_H
#define PYDAL_CONVERSIONS_H

#include "pydal.h"

/*!
  \file pydal_conversions.h
  
  \ingroup DAL
  \ingroup pyrap
  
  \brief Routines for conversion between C/C++ arrays and Python structures
  
  \author Lars B&auml;hren

  <h3>Synopsis</h3>

*/

namespace DAL {   //   BEGIN -- namespace DAL

  //! Convert array to Boost.Python numeric array
  template <class T>
    boost::python::numeric::array toNumericArray (T *data,
						  unsigned int const &nelem)
    {
      std::vector<int> mydims (1,nelem);
      boost::python::numeric::array narray = num_util::makeNum (data, mydims );
      return narray;
    }
  
  //! Convert std::vector<T> to Boost.Python numeric array
  template <class T>
    boost::python::numeric::array toNumericArray (std::vector<T> const &data)
    {
      std::vector<int> mydims (1,data.size());
      boost::python::numeric::array narray = num_util::makeNum (&data[0], mydims );
      return narray;
    }

};   //   END -- namespace DAL

#endif
