/*-------------------------------------------------------------------------*
| $Id:: cs1.h 389 2007-06-13 08:47:09Z baehren                          $ |
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

bool BigEndian( void )
{
  unsigned char SwapTest[2] = { 1, 0 };

  if( *(short *) SwapTest == 1 ) {
    //little endian
    return false;
  } else {
    //big endian
    return true;
  }
}

Int64 Int64Swap (Int64 ff)
{
  unsigned char b1, b2, b3, b4, b5, b6, b7, b8;

  b1 = ( ff       ) & 255;
  b2 = ( ff >>  8 ) & 255;
  b3 = ( ff >> 16 ) & 255;
  b4 = ( ff >> 24 ) & 255;
  b5 = ( ff >> 32 ) & 255;
  b6 = ( ff >> 40 ) & 255;
  b7 = ( ff >> 48 ) & 255;
  b8 = ( ff >> 56 ) & 255;

  return ((Int64)b1 << 56) + ((Int64)b2 << 48) + ((Int64)b3 << 40) +
         ((Int64)b4 << 32) + ((Int64)b5 << 24) + ((Int64)b6 << 16) +
         ((Int64)b7 <<  8) + b8;
}

Int32 Int32Swap (Int32 ii)
{
  unsigned char b1, b2, b3, b4;

  b1 = ( ii       ) & 255;
  b2 = ( ii >>  8 ) & 255;
  b3 = ( ii >> 16 ) & 255;
  b4 = ( ii >> 24 ) & 255;

  return ((Int32)b1 << 24) + ((Int32)b2 << 16) + ((Int32)b3 << 8) + b4;
}

Int16 Int16Swap( Int16 ss )
{
  unsigned char b1, b2;

  b1 = ( ss      ) & 255;
  b2 = ( ss >> 8 ) & 255;

  return (b1 << 8) + b2;
}
