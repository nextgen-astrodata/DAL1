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

using namespace DAL;

int main(int argc, char *argv[])
{
   // parameter check
   if ( argc < 3 )
   {
     cout << endl << "Too few parameters..." << endl << endl;
     cout << "The first parameter is the tbb dataset name." << endl;
     cout << "The second parameter is the dipole itentifier. "
	  << "(i.e. \"001002003\").\n" << "  The first 3 characters are the "
	  << "station id.\n  The next 3 are the rsp id.\n  The last 3 are the "
	  << "rcu id." << endl;
     cout << "The third parameter is the station group itentifier." << endl;
     cout << endl;
     return DAL::FAIL;
   }

   dalDataset * ds = new dalDataset;
   int ret;
   ret = ds->open( argv[1] );

   string id = argv[2];
   int start = 25;
   int length = 20;
   short data[length];
   ds->read_tbb(id, start, length, data);

   printf("\n");
   printf("Dataset: \n");
   for (int jj = 0; jj < length; jj++)
   {
      printf("%d ", data[jj]);
      printf("\n");
   }

    ds->getGroupNames();

    dalArray * myarray = ds->openArray(argv[2],argv[3]);

    cout << "\nGetting TIME attribute from array...";
    unsigned int * time;
    time = reinterpret_cast<unsigned int*>( myarray->getAttribute("TIME") );
    if (time) cout << *(unsigned int*)time << " ...done."; else   cout << "FAILED.";

    cout << "\nGetting SAMPLE_NR attribute from array...";
    unsigned int * sample_nr;
    sample_nr = reinterpret_cast<unsigned int*>( myarray->getAttribute("SAMPLE_NR") );
    if (sample_nr) cout << *(unsigned int*)sample_nr << " ...done."; else   cout << "FAILED.";

    cout << "\n\nClosing integer array... ";
    if ( 0==myarray->close() ) cout << "done." << endl; else  cout << "FAILED.";

   ds->close();
   delete ds;

   cout << "\nSUCCESS" << endl;
   return DAL::SUCCESS;
}

