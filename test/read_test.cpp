/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef DALARRAY_H
#include <dalArray.h>
#endif

#ifndef DALGROUP_H
#include <dalGroup.h>
#endif

#define FILENAME "rw.h5"

int main()
{
   dalDataset * ds = new dalDataset();
   int ret;
   ret = ds->open( FILENAME );

   string id = "001000004";
   int start = 25;
   int len = 25;
   int data[len];
   ds->read_tbb(id, start, len, data);

   printf("\n");
   printf("Dataset: \n");
   for (int jj = 0; jj < len; jj++)
   {
      printf("%d ", data[jj]);
      printf("\n");
   }
    
   ds->close();
   
   cout << "\nSUCCESS" << endl;
   return SUCCESS;
}

