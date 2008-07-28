/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
 *   J.S.Masters@uva.nl                                                    *
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

#ifndef COMMON_H
#define COMMON_H

#ifndef DAL_H
#include "dal.h"
#endif

#include <iostream>
#include <vector>
#include <string>

namespace DAL
  {

  // check if an object exists in a vector
  template <class T>
  bool it_exists( std::vector<T> vec, T item );
  bool it_exists_str( std::vector<std::string> vec, std::string name );

  template <class T>
  void list_vector_members( std::vector<T> vec );

  long double julday(time_t seconds,long *intmjd, long double *fracmjd);

  double mjd2unix( double mjd_time );

#ifdef PYTHON
  bpl::numeric::array mjd2unix_boost( bpl::numeric::array mjd_time );
#endif

} // namespace DAL

#endif // COMMON_H
