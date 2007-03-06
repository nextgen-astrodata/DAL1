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

void get_args( int argc, char** argv, long* start_value, long* stop_value,
	       char** table_value)
{
    int i;

    /* Start at i = 1 to skip the command name. */

    for (i = 1; i < argc; i++) {

	/* Check for a switch (leading "-"). */

	if (argv[i][0] == '-') {

	    /* Use the next character to decide what to do. */

	    switch (argv[i][1]) {

		case 's':	if ( strlen(argv[i]) > 2 ) {
				string goo;
				for (unsigned int jj=2;jj<strlen(argv[i]);jj++) {
				   goo += argv[i][jj];
				}
				*start_value = atol(goo.c_str());
				}
				else {
				  *start_value = atol(argv[++i]);
				}
				break;

		case 'S':	if ( strlen(argv[i]) > 2 ) {
				string goo;
				for (unsigned int jj=2;jj<strlen(argv[i]);jj++) {
				   goo += argv[i][jj];
				}
				*stop_value = atol(goo.c_str());
				}
				else {
				  *stop_value = atol(argv[++i]);
				}
				break;

		case 't':	if ( strlen(argv[i]) > 2 ) {
				char* goo;
				goo = (char*)malloc( sizeof(char) * 100 );
				for (unsigned int jj=2;jj<strlen(argv[i]);jj++) {
				   goo += argv[i][jj];
				}
				cout << goo << endl;
				*table_value = (char*)goo;
				cout << *table_value << endl;
				}
				else {
				  *table_value = argv[++i];
				}
				break;

		default:	fprintf(stderr,
				"Unknown switch %s\n", argv[i]);
	    }
	}
    }
}


/*! doxygen comment in dal.cpp */
int main(int argc, char *argv[])
{

  // parameter check
  if ( argc < 2 )
  {
     cout << endl << "Too few parameters..." << endl << endl;
     cout << "The first parameter is the dataset name." << endl;
     cout << "The second parameter is the row start value." << endl;
     cout << "The third parameter is the row stop value." << endl;
     cout << endl;
     return FAIL;
  }

  /* Set defaults for all parameters: */
  long start =0;
  long stop = 0;// read a table starting/stopping at these vals
  char *table = "ANTENNA";

  get_args(argc, argv, &start, &stop, &table);

  dalDataset * dataset = new dalDataset();

  if ( 0 != dataset->open( argv[1] ) )
  {
  	cout << "Problem opening dataset: " << argv[1] << '.' << " Quiting."
	     << endl;
  	exit(FAIL);
  }

  // Open Station group
  dalGroup * stationGroup = dataset->openGroup("Station");

  // Read Station group attributes
  cout << endl << "Station Group Attributes:" << endl << endl;

  stationGroup->getAttributes();  // iterate over all group attributes

  cout << endl;

  // Open ANTENNA table in Station group
  dalTable * antennaTable = dataset->openTable(table,"Station");
  long maximum = antennaTable->getNumberOfRows();
  if ( maximum <= 0 ) {
	cout << table << " table contains no rows." << endl;
	exit(2);
  }

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

  if (!stop) stop=maximum;

  if (start >= stop) {
	cout << "ERROR: start value must be less than stop." << endl;
	exit(7);
  }

  /*
  cout << "start = " << start << endl;
  cout << "stop = " << stop << endl;
  */

  cout << "Number of " << table << " table rows: " << maximum
     << " (i.e. 0:" << maximum-1 << ')' << endl << endl;

  if (stop > maximum-1) {
	cout << "Stop value larger than table. Reset to table maximum." << endl;
	stop = maximum-1;
  }
  antennaTable->listColumns();

  string antpos, antorient;

  for ( long ii = start; ii <= stop; ii ++ ) {

	antennaTable->readRows( data_out, ii, /*startRow,*/ NUMBERROWS);

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
