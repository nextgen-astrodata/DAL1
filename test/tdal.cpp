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

/*!
  \file tdal.cpp
  
  \ingroup DAL
 
  \author Joseph Masters

  \date 12-04-06
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef DALGROUP_H
#include <dalGroup.h>
#endif

#define FILENAME "./tdal_test.h5"

typedef struct rowStruct {
	float time;
	float uvw[3];
	float antenna1;
} rowStruct;

typedef struct writebuffer {
	rowStruct rs[3];
} writebuffer;

/*! sample doxygen comment in dal.cpp */
int main()
{

// create a dataset

   dalDataset * ds;
   ds = new dalDataset( FILENAME, "HDF5" );

//    ds->setFilter("A,B,C","where B=1");
   dalGroup * groupA = ds->createGroup( "groupA" );
   dalGroup * groupB = ds->createGroup( "groupB" );
   dalGroup * groupC = ds->createGroup( "groupC" );

   dalTable * table1 = ds->createTable( "table1", "groupA" );
//   dalTable * table2 = ds->createTable( "table2", "groupA" );
//   dalTable * table3 = ds->createTable( "table3", "groupB" );


   // define dimensions of array
   vector<int> dims;
   dims.push_back(4);
   dims.push_back(5);
   dims.push_back(6);

   int idata[4*5*6];
   for (int gg=0; gg<(4*5*6); gg++)
	idata[gg] = gg;

   vector<int> cdims;
   dalArray * iarray = ds->createIntArray( "int_array", dims, idata, cdims );

   float fdata[4*5*6];
   for (int gg=0; gg<(4*5*6); gg++)
	fdata[gg] = rand();
   dalArray * farray = ds->createFloatArray( "float_array", dims, fdata, cdims );

   complex<float> * cdata = new complex<float>[ 4*5*6 ];
   for (int gg=0; gg<(4*5*6); gg++) {
	cdata[ gg ] = 0;
   }
   dalArray * carray = ds->createComplexArray( "complex_array", dims, cdata, cdims );
   delete cdata;

   table1->addColumn( "TIME", dal_FLOAT );
   table1->addColumn( "UVW", dal_FLOAT, 3 );
   table1->addColumn( "ANTENNA1", dal_FLOAT );

   rowStruct rs;

   rs.time = 1;
   rs.uvw[0] = 1;
   rs.uvw[1] = 1;
   rs.uvw[2] = 1;
   rs.antenna1 = 1;
   table1->appendRow(&rs);
   table1->appendRow(&rs);
   table1->appendRow(&rs);
   table1->appendRow(&rs);
   table1->appendRow(&rs);
   table1->appendRow(&rs);
   float md = 3;
   float mi = 5;
   table1->writeDataByColNum( &md, 0, 0 );
   table1->writeDataByColNum( &md, 0, 1 );
   table1->writeDataByColNum( &md, 0, 2 );
   table1->writeDataByColNum( &mi, 2, 0 );
   table1->writeDataByColNum( &mi, 2, 1 );
   table1->writeDataByColNum( &mi, 2, 2 );
/*   table2->addColumn( "col1", dal_UINT );   table2->addColumn( "col2", dal_STRING );
   table2->addColumn( "col3", dal_DOUBLE );

   table3->addColumn( "col1", dal_INT );
   table3->addColumn( "col2", dal_INT );
   table3->addColumn( "col3", dal_INT );*/
   ds->close();
/*
   delete table1;
   delete table2;
   delete table3;
*/
   delete iarray;
   delete farray;
   delete carray;
   delete groupA;
   delete groupB;
   delete groupC;
   delete ds;

   cout << "SUCCESS" << endl;
   return SUCCESS;
}

