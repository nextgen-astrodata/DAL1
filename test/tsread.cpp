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
  \file tsread.cpp
  
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

#ifndef TIMESERIES_H
#include <timeseries.h>
#endif

void get_args( int argc, char** argv, long* start_value, long* stop_value,
	       char** table_value, char** group_value )
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

		case 'g':	if ( strlen(argv[i]) > 2 ) {
				char* goo;
				goo = (char*)malloc( sizeof(char) * 100 );
				for (unsigned int jj=2;jj<strlen(argv[i]);jj++) {
				   goo += argv[i][jj];
				}
				cout << goo << endl;
				*group_value = (char*)goo;
				cout << *group_value << endl;
				}
				else {
				  *group_value = argv[++i];
				}
				break;

		default:	fprintf(stderr,
				"Unknown switch %s\n", argv[i]);
	    }
	}
    }
}


/*! sample doxygen comment in dal.cpp */
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
  char *group = "Station";

  get_args(argc, argv, &start, &stop, &table, &group);

  dalDataset * dataset = new dalDataset();

  if ( 0 != dataset->open( argv[1] ) )
  {
  	cout << "Problem opening dataset: " << argv[1] << '.' << " Quiting."
	     << endl;
  	exit(FAIL);
  }

  // Open Station group
  dalGroup * stationGroup = dataset->openGroup( group );
  if ( NULL == stationGroup ) {
	cout << "Could not open group " << group << '.' << endl;
	exit(7);
  }

  // Read Station group attributes
  cout << endl << group << " Group Attributes:" << endl << endl;

  stationGroup->getAttributes();  // iterate over all group attributes

  cout << endl;

  // Open ANTENNA table in Station group
  dalTable * antennaTable = dataset->openTable( table, group );
  
  cout << "ANTENNA Table attributes:" << endl << endl;
  //antennaTable->getAttributes();
double * sample_freq;
unsigned int * data_length;
  /*if (*/ (sample_freq = reinterpret_cast<double*>(antennaTable->getAttribute("SAMPLE_FREQ"))) ;//)
    cout << "Sample Freq = " << *sample_freq << " Mhz" << endl;
  /*if (*/ (data_length = reinterpret_cast<unsigned int*>(antennaTable->getAttribute("DATA_LENGTH"))) ;//)
    cout << "Data length = " << *data_length << " frames" << endl;
  cout << endl;

  long maximum = antennaTable->getNumberOfRows();
  if ( maximum <= 0 ) {
	cout << table << " table contains no rows." << endl;
	exit(2);
  }

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

  cout << "Number of " << table << " table rows: " << maximum
     << " (i.e. 0:" << maximum-1 << ')' << endl << endl;

  if (stop > maximum-1) {
	cout << "Stop value larger than table. Reset to table maximum." << endl;
	stop = maximum-1;
  }
  antennaTable->listColumns();

  string antpos, antorient;
  
  long bufsize = sizeof(AntennaStruct) + (1034*sizeof(short));
  //cout << bufsize << endl;
  //exit(2);
  for ( long ii = start; ii <= stop; ii ++ ) {

	antennaTable->readRows( &data_out, ii, /*startRow,*/ NUMBERROWS, bufsize);

	// print some values from the read
	for (int gg=0; gg < NUMBERROWS; gg++)
	{
		cout << setw(17) << data_out[gg].rsp_id;
		cout << setw(17) << data_out[gg].rcu_id;
		/* compute time */
		time_t mytime = data_out[gg].time;
                tm *time=localtime( reinterpret_cast<time_t*>(&mytime) );
                string time_str = stringify(time->tm_mon+1) + '/' +
			stringify(time->tm_mday) + ' ' +
			stringify(time->tm_hour) + ':' +
			stringify(time->tm_min) + ':' +
			stringify(time->tm_sec);
		cout << setw(17) << time_str;
		cout << setw(17) << data_out[gg].sample_nr;
		cout << setw(17) << data_out[gg].samples_per_frame;
		cout << setw(17) << data_out[gg].feed;
		antpos = '[' +
			 stringify(data_out[gg].ant_position[0]) + ',' +
			 stringify(data_out[gg].ant_position[1]) + ',' +
			 stringify(data_out[gg].ant_position[2]) + ']';
		cout << setw(17) << antpos;
		antorient = '[' +
			    stringify(data_out[gg].ant_orientation[0]) + ',' +
			    stringify(data_out[gg].ant_orientation[1]) + ',' +
			    stringify(data_out[gg].ant_orientation[2]) + ']';
		cout << setw(17) << antorient << '\t';

		// print data
//		char * mystr = (char*)(stationGroup->getAttribute("OBS_MODE"));
/*		if ( 0 == strcmp("Sub-band",mystr) ) {
			for (unsigned int cc=0; cc<data_out[gg].samples_per_frame; cc+=2) {
				cout << '(';
				cout << *( (short*)(data_out[gg].data[0].p) + cc ) << ',';
				cout << *( (short*)(data_out[gg].data[0].p) + (cc+1) );
				cout << "),";
			}
			cout << '\b';
		} else {
*/
// ASSUME NORMAL TRANSIENT MODE
			for (unsigned int cc=0; cc<data_out[gg].samples_per_frame; cc++) {
				cout << *( (short*)(data_out[gg].data[0].p) + cc ) << ',';
			}
			cout << '\b';
//		}
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
