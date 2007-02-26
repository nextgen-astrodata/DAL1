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
/**
 * \file dal.cpp
 * \author Joseph Masters
 * \date 12-04-06
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
     return FAIL;
  }

  dalDataset * dataset = new dalDataset();

  if ( 0 != dataset->open( argv[1] ) )
  {
  	cout << "Problem opening dataset: " << argv[1] << '.' << " Quiting." << endl;
  	exit(FAIL);
  }

  // Open Station group
  dalGroup * stationGroup = dataset->openGroup("Station");

  // Read Station group attributes
  cout << endl << "Station Group Attributes:" << endl;

  stationGroup->getAttributes();  // iterate over all group attributes

  cout << endl;

  // Open ANTENNA table in Station group
  dalTable * antennaTable = dataset->openTable("ANTENNA","Station");
  typedef struct AntennaStruct {
	unsigned int rsp_id;
	unsigned int rcu_id;
	unsigned int time;
	unsigned int sample_nr;
	unsigned int samples_per_frame;
	short data;
	char feed[16];
	double ant_position[ 3 ];
	double ant_orientation[ 3 ];
  } AntennaStruct;

  // define a structure to read the data into
  //   in this case it's AntennaStruct (defined above)
  const int NUMBERROWS = 1;
  long startRow = 0;
  AntennaStruct data_out[NUMBERROWS];

  long maximum = antennaTable->getNumberOfRows();

  antennaTable->listColumns();

  string antpos, antorient;
  //cout << "ANTENNA table data:" << endl;
  for ( long ii = 0; ii < maximum; ii ++ ) {

	antennaTable->readRows( data_out, startRow, NUMBERROWS);

	// print some values from the read
	for (int gg=0; gg < NUMBERROWS; gg++)
	{
		cout << setw(11) << data_out[gg].rsp_id;
		cout << setw(11) << data_out[gg].rcu_id;
		cout << setw(11) << data_out[gg].time;
		cout << setw(11) << data_out[gg].sample_nr;
		cout << setw(11) << data_out[gg].samples_per_frame;
		cout << setw(11) << data_out[gg].data;
		cout << setw(11) << data_out[gg].feed;
		antpos = '[' +
			 stringify(data_out[gg].ant_position[0]) + ',' +
			 stringify(data_out[gg].ant_position[1]) + ',' +
			 stringify(data_out[gg].ant_position[2]) + ']';
		cout << setw(11) << antpos;
		antorient = '[' +
			    stringify(data_out[gg].ant_orientation[0]) + ',' +
			    stringify(data_out[gg].ant_orientation[1]) + ',' +
			    stringify(data_out[gg].ant_orientation[2]) + ']';
		cout << setw(11) << antorient;
		cout << endl;
	}

	startRow += NUMBERROWS;
  }
  cout << endl;
  delete antennaTable;

  delete stationGroup;
  delete dataset;

  cout << "SUCCESS" << endl;
  return SUCCESS;
}
