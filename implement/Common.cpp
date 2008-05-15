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
#include "Common.h"
#endif

namespace DAL {

// check if an object exists in a vector
template <class T>
bool it_exists( std::vector<T> vec, T item )
{
  typename std::vector<T>::iterator it;
  for ( it=vec.begin() ; it < vec.end(); it++ )
  {
    if ( item == *it )
      return true;
  }
  return false;
}

template <class T>
void list_vector_members( std::vector<T> vec )
{
  typename std::vector<T>::iterator it;
  for ( it=vec.begin() ; it < vec.end(); it++ )
  {
     std::cerr << *it << std::endl;
  }
}

long double julday(time_t seconds,long *intmjd, long double *fracmjd)
{
   long double dayfrac, jd, sec;
   int year, yday;
   int hour, min;
   struct tm *ptr = NULL;

   unsigned int nd;

   ptr = gmtime(&seconds);

   hour = ptr->tm_hour;
   min = ptr->tm_min;
   sec = (long double)ptr->tm_sec;

   year = ptr->tm_year;
   yday = ptr->tm_yday + 1;

   dayfrac = ( (sec/60.0L + (long double) min)/60.0L + \
          (long double)hour)/24.0L;
   nd = year * 365;
   nd += (year - 1)/4;
   nd += yday + 2415020;

   *intmjd = nd - 2400001;
   *fracmjd = dayfrac;

   jd = (long double)nd + dayfrac - 0.5L;

   return jd;
}

} // namespace DAL
