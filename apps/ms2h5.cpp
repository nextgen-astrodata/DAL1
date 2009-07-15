/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

/*!
  \file ms2h5.cpp

  \ingroup DAL

  \brief Test program for writing measurement set data into an HDF5 file.

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

using namespace DAL;

/*! doxygen comment in dal.cpp */
int main(int argc, char *argv[])
{

  // parameter check
  if ( argc < 2 )
    {
      cout << endl << "Too few parameters..." << endl << endl;
      cout << "The first parameter is the dataset name." << endl;
      cout << "The second parameter is the filetype. (optional)" << endl;
      cout << endl;
      return DAL::FAIL;
    }

  dalDataset * dataset;
  if ( NULL == argv[2] )
    dataset = new dalDataset( argv[1], "HDF5" );
  else
    dataset = new dalDataset( argv[1], argv[2] );

  // define the structure of a table
  // define the data to go in the table
  // create the table in the file or group
  dalGroup * group = dataset->createGroup( "Sub" );
  dalTable * tableA = dataset->createTable( "MAIN", "Sub" );

  // add columns to the measurement set
  tableA->addColumn( "time", dal_DOUBLE );  // simple column
  tableA->addColumn( "antenna1", dal_INT );  // simple column
  tableA->addColumn( "antenna2", dal_INT );  // simple column
  tableA->addColumn( "feed1", dal_INT );  // simple column
  tableA->addColumn( "feed2", dal_INT );  // simple column
  tableA->addColumn( "data_desc_id", dal_INT );  // simple column
  tableA->addColumn( "processor_id", dal_INT );  // simple column
  tableA->addColumn( "field_id", dal_INT );  // simple column
  tableA->addColumn( "interval", dal_DOUBLE );  // simple column
  tableA->addColumn( "exposure", dal_DOUBLE );  // simple column
  tableA->addColumn( "time_centroid", dal_DOUBLE );  // simple column
  tableA->addColumn( "scan_number", dal_INT );  // simple column
  tableA->addColumn( "array_id", dal_INT );  // simple column
  tableA->addColumn( "obs_id", dal_INT );  // simple column
  tableA->addColumn( "state_id", dal_INT );  // simple column

  tableA->addColumn( "uvw", dal_DOUBLE, 3 );

  typedef struct skycol
    {
      int x;
      int y;
    } skycol;

  vector<dalColumn> cv;

  dalColumn col_a( "x", dal_INT );
  dalColumn col_b( "y", dal_INT );

  cv.push_back( col_a );
  cv.push_back( col_b );

  tableA->addComplexColumn( "sky", cv, 2 );

//  tableA->printColumns();

  // describe and fill data, and provide offsets and types
  // describe the data or structure of the table
  const long BSIZE = 1000;
  typedef struct MainTable
    {
      double time;
      int antenna1;
      int antenna2;
      int feed1;
      int feed2;
      int data_desc_id;
      int processor_id;
      int field_id;
      double interval;
      double exposure;
      double time_centroid;
      int scan_number;
      int array_id;
      int observation_id;
      int state_id;
      double uvw[3];
      skycol sky;
    } MainTable;

  MainTable mainB[BSIZE];
  const int LOOPMAX = 10000;
  for ( int uu=0 ; uu < LOOPMAX; uu++)
    {
      for (long row=0; row<BSIZE; row++)
        {
          mainB[row].time = row + 0.1;
          mainB[row].antenna1 = row;
          mainB[row].antenna2 = row;
          mainB[row].feed1 = row;
          mainB[row].feed2 = row;
          mainB[row].data_desc_id = row;
          mainB[row].processor_id = row;
          mainB[row].field_id = row;
          mainB[row].interval = row + 0.1;
          mainB[row].exposure = row + 0.1;
          mainB[row].time_centroid = row + 0.1;
          mainB[row].scan_number = row;
          mainB[row].array_id = row;
          mainB[row].observation_id = row;
          mainB[row].state_id = row;
          mainB[row].uvw[0] = row + 0.1;
          mainB[row].uvw[1] = row + 0.1;
          mainB[row].uvw[2] = row + 0.1;
          mainB[row].sky.x = row;
          mainB[row].sky.y = row+1;
        }
      tableA->appendRows( mainB, BSIZE );
    }

  /* Create and write the attribute "attr1" on the dataset "dset" */
  const int attrSize = 5;
  typedef struct dstct
    {
      int a;
    } dstct;
  dstct     data[attrSize];
  data[0].a=1;
  data[1].a=2;
  data[2].a=3;
  data[3].a=4;
  data[4].a=5;

//tableA->setAttribute("attrTEST", data, attrSize );

  delete tableA;


  // define the structure of an image
  // define the data to go in the image
  // create the image in the file or group
//   dataset.createImage();


  delete group;
  delete dataset;

  cout << "SUCCESS" << endl;
  return DAL::SUCCESS;
}
