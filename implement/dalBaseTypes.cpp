/*------------------------------------------------------------------------*
| $Id:: cs1.h 389 2007-06-13 08:47:09Z masters                          $ |
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

namespace DAL
  {

  bool BigEndian( void )
  {
    unsigned char SwapTest[2] = { 1, 0 };

    if ( *(short *) SwapTest == 1 )
      {
        //little endian
        return false;
      }
    else
      {
        //big endian
        return true;
      }
  }

  void swapbytes(char *addr, int8_t nbytes)
  {
    char buf;
    int i;

    /* byte swap routine - more cryptic - but it is */
    /* generic. Does the swap shown below */

    //buf1[0] = buf2[7];  buf1[1] = buf2[6];
    //buf1[2] = buf2[5];  buf1[3] = buf2[4];
    //buf1[4] = buf2[3];  buf1[5] = buf2[2];
    //buf1[6] = buf2[1];  buf1[7] = buf2[0];

    //  buf3=buf2[0];buf2[0]=buf2[7];buf2[7]=buf3;
    //  buf3=buf2[1];buf2[1]=buf2[6];buf2[6]=buf3;
    //  buf3=buf2[2];buf2[2]=buf2[5];buf2[5]=buf3;
    //  buf3=buf2[3];buf2[3]=buf2[4];buf2[4]=buf3;

    for (i=0;i<nbytes/2;i++)
      {
        buf=addr[i];
        addr[i] = addr[nbytes - (i+1)];
        addr[nbytes - (i+1)] = buf;
      }
  }

} // namespace DAL
