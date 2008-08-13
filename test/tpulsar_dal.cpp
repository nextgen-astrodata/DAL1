/*-------------------------------------------------------------------------*
 | $Id:: tUseMeasures.cc 511 2007-08-05 13:14:48Z masters                $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

/**
  \file tpulsar_dal.cpp

  \ingroup DAL

  \brief Test program for working with pulsar data format.

  \author Joseph Masters
*/

#ifndef DAL_H
#include <dal.h>
#endif

#define FILENAME "./tpulsar.h5"

using namespace DAL;

/*! sample doxygen comment in dal.cpp */
int main()
{

// create a dataset

  dalDataset * ds;
  ds = new dalDataset( FILENAME, "HDF5" );

//    dalGroup * beam001 = ds->createGroup( "beam001" );
//    dalGroup * b001_sb0 = new dalGroup( beam001->getId(), "sb0" );
//    dalGroup * b001_sb1 = new dalGroup( beam001->getId(), "sb1" );
//    dalGroup * b001_sb2 = new dalGroup( beam001->getId(), "sb2" );

  const int BUFSIZE = 99;
  typedef struct datastruct
    {
// 	dalcomplex dt;
      float dt[256];
    } datastruct;
  datastruct data[ BUFSIZE ];

  /* Beam 0 -------------------------------------------------------*/

  dalGroup * beam000 = ds->createGroup( "beam000" );
  dalTable * b0_sb0 = ds->createTable( "sb0", "beam000" );
  b0_sb0->addColumn( "data", dal_FLOAT, 256 );
  for (long row=0; row<BUFSIZE; row++)
    {
      for (int channel=0; channel<256; channel++)
        data[row].dt[channel] = rand();
      /*	data[row].dt.i = rand();
      	data[row].dt.r = rand();*/
    }
  b0_sb0->appendRows( data, BUFSIZE );

  dalTable * b0_sb1 = ds->createTable( "sb1", "beam000" );
  b0_sb1->addColumn( "data", dal_FLOAT, 256 );
  for (long row=0; row<BUFSIZE; row++)
    {
      for (int channel=0; channel<256; channel++)
        data[row].dt[channel] = rand();
      /*	data[row].dt.i = rand();
      	data[row].dt.r = rand();*/
    }
  b0_sb1->appendRows( data, BUFSIZE );

  dalTable * b0_sb2 = ds->createTable( "sb2", "beam000" );
  b0_sb2->addColumn( "data", dal_FLOAT, 256 );
  for (long row=0; row<BUFSIZE; row++)
    {
      for (int channel=0; channel<256; channel++)
        data[row].dt[channel] = rand();
      /*	data[row].dt.i = rand();
      	data[row].dt.r = rand();*/
    }
  b0_sb2->appendRows( data, BUFSIZE );


  /* Beam 1 -------------------------------------------------------*/

  dalGroup * beam001 = ds->createGroup( "beam001" );
  dalTable * b1_sb0 = ds->createTable( "sb0", "beam001" );
  b1_sb0->addColumn( "data", dal_FLOAT, 256 );
  for (long row=0; row<BUFSIZE; row++)
    {
      for (int channel=0; channel<256; channel++)
        data[row].dt[channel] = rand();
      /*	data[row].dt.i = rand();
      	data[row].dt.r = rand();*/
    }
  b1_sb0->appendRows( data, BUFSIZE );

  dalTable * b1_sb1 = ds->createTable( "sb1", "beam001" );
  b1_sb1->addColumn( "data", dal_FLOAT, 256 );
  for (long row=0; row<BUFSIZE; row++)
    {
      for (int channel=0; channel<256; channel++)
        data[row].dt[channel] = rand();
      /*	data[row].dt.i = rand();
      	data[row].dt.r = rand();*/
    }
  b1_sb1->appendRows( data, BUFSIZE );

  dalTable * b1_sb2 = ds->createTable( "sb2", "beam001" );
  b1_sb2->addColumn( "data", dal_FLOAT, 256 );
  for (long row=0; row<BUFSIZE; row++)
    {
      for (int channel=0; channel<256; channel++)
        data[row].dt[channel] = rand();
      /*	data[row].dt.i = rand();
      	data[row].dt.r = rand();*/
    }
  b1_sb2->appendRows( data, BUFSIZE );


  /* Beam 2 -------------------------------------------------------*/

  dalGroup * beam002 = ds->createGroup( "beam002" );
  dalTable * b2_sb0 = ds->createTable( "sb0", "beam002" );
  b2_sb0->addColumn( "data", dal_FLOAT, 256 );
  for (long row=0; row<BUFSIZE; row++)
    {
      for (int channel=0; channel<256; channel++)
        data[row].dt[channel] = rand();
      /*	data[row].dt.i = rand();
      	data[row].dt.r = rand();*/
    }
  b2_sb0->appendRows( data, BUFSIZE );

  dalTable * b2_sb1 = ds->createTable( "sb1", "beam002" );
  b2_sb1->addColumn( "data", dal_FLOAT, 256 );
  for (long row=0; row<BUFSIZE; row++)
    {
      for (int channel=0; channel<256; channel++)
        data[row].dt[channel] = rand();
      /*	data[row].dt.i = rand();
      	data[row].dt.r = rand();*/
    }
  b2_sb1->appendRows( data, BUFSIZE );

  dalTable * b2_sb2 = ds->createTable( "sb2", "beam002" );
  b2_sb2->addColumn( "data", dal_FLOAT, 256 );
  for (long row=0; row<BUFSIZE; row++)
    {
      for (int channel=0; channel<256; channel++)
        data[row].dt[channel] = rand();
      /*	data[row].dt.i = rand();
      	data[row].dt.r = rand();*/
    }
  b2_sb2->appendRows( data, BUFSIZE );


  /*   dalTable * table1 = ds->createTable( "table1", "groupA" );
     dalTable * table2 = ds->createTable( "table2", "groupA" );*/
//    dalTable * table3 = ds->createTable( "table3", "groupB" );


  // define dimensions of array
//    vector<int> dims;
//    dims.push_back(4);
//    dims.push_back(5);
//    dims.push_back(6);
//
//    int idata[4*5*6];
//    for (int gg=0; gg<(4*5*6); gg++)
// 	idata[gg] = gg;
//    dalArray * iarray = ds->createArray( "int_array", dims, idata );
//
//    float fdata[4*5*6];
//    for (int gg=0; gg<(4*5*6); gg++)
// 	fdata[gg] = rand();
//    dalArray * farray = ds->createArray( "float_array", dims, fdata );

//    table1->addColumn( "col1", dal_INT );
//    table1->addColumn( "col2", dal_SHORT );
//    table1->addColumn( "col3", dal_FLOAT );
//
//    table2->addColumn( "col1", dal_UINT );
//    table2->addColumn( "col2", dal_STRING );
//    table2->addColumn( "col3", dal_DOUBLE );
//
//    table3->addColumn( "col1", dal_INT );
//    table3->addColumn( "col2", dal_INT );
//    table3->addColumn( "col3", dal_INT );

  ds->close();

  /*   delete table1;
     delete table2;
     delete table3;*/
  /*   delete iarray;
     delete farray;*/
  delete b0_sb0;
  delete b0_sb1;
  delete b0_sb2;
  delete beam000;
  delete b1_sb0;
  delete b1_sb1;
  delete b1_sb2;
  delete beam001;
  delete b2_sb0;
  delete b2_sb1;
  delete b2_sb2;
  delete beam002;
  delete ds;

  cout << "SUCCESS" << endl;
  return DAL::SUCCESS;
}

