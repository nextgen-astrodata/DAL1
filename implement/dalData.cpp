/*-------------------------------------------------------------------------*
 | $Id:: dalData.h 660 2007-08-30 16:55:31Z masters                     $ |
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

/*!
  \file dalData.h
  
  \ingroup DAL

  \author Joseph Masters

  \date 09-05-07
*/

#ifndef DALDATA_H
#include "dalData.h"
#endif

/*void * dalData::get( long idx1 )
{
  return NULL;
}
void * dalData::get( long idx1 )
{
  return NULL;
}*/
void * dalData::get( long idx1, long idx2, long idx3 )
{
   vector<long> indices;
//    vector<long>::iterator it;
   if( idx1>-1 )
     indices.push_back( idx1 );
   if( idx2>-1 )
     indices.push_back( idx2 );
   if( idx3>-1 )
     indices.push_back( idx3 );

   if (indices.size() != (shape.size()+1) )
   {
      cout << "ERROR: Number of indices do not match shape of column." << endl;
      exit(-1);
   }
// cout << "ind size: " << indices.size() << endl;
// cout << "shape size: " << shape.size() << endl;
   unsigned long index = 0;
   long bb = 1;

   if ( "MSCASA" == filetype )
   {
//      it = shape.begin();
     shape.insert( shape.begin(), 1 );
     for (unsigned int dim=0; dim<shape.size(); dim++)
     {
//         index = idx1 + (idx2*shape[0]) + (idx3*(shape[0]*shape[1]));
        for (unsigned int ss=dim; ss>0; ss--)
	   bb *= shape[ ss ];

        index += indices[dim]*bb;
	bb=1;
     }
     shape.erase( shape.begin() );

     if ( dal_COMPLEX == datatype )
        return (&(((complex<float>*)data)[ index ]));

     else if ( dal_DOUBLE == datatype )
        return (&(((double*)data)[ index ]));
   }
   return NULL;
}

dalData::dalData() {}

dalData::dalData(string lclfiletype, string lcldatatype,
   vector<int> lclshape)
{
    filetype = lclfiletype;
    datatype = lcldatatype;
    shape = lclshape;
}

// void dalData::setData(void * lcldata)
// {
//    data = lcldata;
// }

#ifdef PYTHON
#endif
