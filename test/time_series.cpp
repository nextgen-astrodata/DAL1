/***************************************************************************
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
/**
 * \file dal.cpp
 * \author Joseph Masters
 * \date 06-Feb-07
 */

 #ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALDATASET_H
#include "dalDataset.h"
#endif

#ifndef DALGROUP_H
#include "dalGroup.h"
#endif

/*! doxygen comment in dal.cpp */
int main(int argc, char *argv[])
{
  
  // parameter check
  if ( argc < 2 )
  {
     cout << endl << "Too few parameters..." << endl << endl;
     cout << "The first parameter is the dataset name." << endl;
     //cout << "The second parameter is the filetype. (optional)" << endl;
     cout << endl;
     return FAIL;
  }

  dalDataset * dataset;
  if ( NULL == argv[2] )
	  dataset = new dalDataset( argv[1] );
  else
	  dataset = new dalDataset( argv[1], argv[2] );
  
  // create the Station group
  dalGroup * stationGroup = dataset->createGroup( "Station" );

  string telescope = "LOFAR";
  string observer = "Iba User";
  string project = "Transients";
  string observation_id = "1287";
  string observation_mode = "Standard";
  string trigger_type = "Unknown";
  double trigger_offset[1] = { 0 };
  int triggered_antennas[1] = { 0 };
  double beam_direction[1] = { 0 };
  
  stationGroup->setAttribute_string("TELESCOPE", telescope );
  stationGroup->setAttribute_string("OBSERVER", observer );
  stationGroup->setAttribute_string("PROJECT", project );
  stationGroup->setAttribute_string("OBSERVATION_ID", observation_id );
  stationGroup->setAttribute_string("OBSERVATION_MODE", observation_mode );
  stationGroup->setAttribute_string("TRIGGER_TYPE", trigger_type );
  stationGroup->setAttribute_double("TRIGGER_OFFSET", trigger_offset );
  stationGroup->setAttribute_int("TRIGGERED_ANTENNAS", triggered_antennas );
  stationGroup->setAttribute_double("BEAM_DIRECTION", beam_direction );

  // create ANTENNA table
  dalTable * tableA = dataset->createTable( "ANTENNA", "Station" );
  
  // add columns to ANTENNA table
  tableA->addColumn( "rsp_id", dal_INT );  // simple column
  tableA->addColumn( "rcu_id", dal_INT );  // simple column
  tableA->addColumn( "time", dal_INT );  // simple column
  tableA->addColumn( "sample_nr", dal_INT );  // simple column
  tableA->addColumn( "samples_per_frame", dal_INT );  // simple column
  tableA->addColumn( "feed", dal_INT );  // simple column
//  tableA->addArrayColumn( "data", dal_SHORT, data_length );
//  tableA->addArrayColumn( "ant_position", dal_DOUBLE, 3 );
//  tableA->addArrayColumn( "ant_orientation", dal_DOUBLE, 3 );
  
  dalTable * tableB = dataset->createTable( "CALIBRATION", "Station" );

/*
  typedef struct skycol {
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
	const long BSIZE = 10000;
	typedef struct MainTable {
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
	for (long row=0; row<BSIZE; row++) {
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

 // Create and write the attribute "attr1" on the dataset "dset" 
 const int attrSize = 5;
 typedef struct dstct {
 	int a;
 } dstct;
 dstct     data[attrSize];
 data[0].a=1;
 data[1].a=2;
 data[2].a=3;
 data[3].a=4;
 data[4].a=5;
 
 tableA->setAttribute("attrTEST", data, attrSize );
*/
  delete tableA;
  delete tableB;


  // define the structure of an image
  // define the data to go in the image
  // create the image in the file or group
//   dataset.createImage();
  

  delete stationGroup;
  delete dataset;
  
  cout << "SUCCESS" << endl;
  return SUCCESS;
}
