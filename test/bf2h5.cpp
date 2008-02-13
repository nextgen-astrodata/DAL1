/******************************************************************************
 *
 *  This file represents a Lofar Tied-Array (aka Beam-Formed, aka Pulsar) data
 *  reader.
 *
 *   It reads raw data and dumps samples to the screen in blocks of
 *   155,648 per ~1 second.  There are no time stamps provided.  The output
 *   columns (as described by the first two lines) are:
 * 
 *    - sample number (1 to 155648 for each frame)
 *    - X coordinate as a complex number (Xr,Xi)
 *    - Y coordinate as a complex number (Yr,Yi)
 * 
 * The very last line of output gives the cumulative observation time for the file.
 * 
 * Usage is:  lta_reader <filename>
 * Example:   lta_reader 
 * 
 * The program was written for and tested on little-endian machines
 *
 * Author:  Joseph Masters <jmasters at science dot uva dot nl>
 * Created:  February 2007
 *
 ******************************************************************************/

/*!
  \file bf2h5.cpp
  
  \ingroup DAL

  \brief Write TBB time-series data into an HDF5 file.

  \author Joseph Masters

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

#ifndef TIMESERIES_H
#include "timeseries.h"
#endif

// reading a complete binary file
#include <iostream> // for cout
#include <iomanip>  // for cout field width
#include <fstream>  // for file handle
#include <complex>  // for complex datatypes

using namespace std;
using std::complex;

// define a few datatypes

typedef unsigned int  UInt32;
typedef int            Int32;
typedef float        Float32;

#define FILENAME "bf.h5"

/************************************************
 *
 * Function:  print_banner()
 *
 * Returns:   void
 * Purpose:   Prints a banner that looks like:
 *
 * Sample              (Xr,Xi)     (Yr,Yi)
 * ------              -------     -------
 *
 ***********************************************/
void print_banner() {
	cout << setw(20) << left << "Sample"
		 << left << "(Xr,Xi)\t"
		 << left << "(Yr,Yi)" << endl
		 << setw(20) << left << "------"
		 << left << "-------\t"
		 << left << "-------" << endl;
}

/************************************************
 *
 * Function:  main(int argc, char *argv[])
 *
 * Returns:   int
 * Purpose:   MAIN
 *
 ***********************************************/
int
main (int argc, char *argv[]) {

	// If less than two arguments provided, print usage
	if ( 2 != argc &&  4 != argc ) {
		cout << "Usage:  lta_reader <filename> [start sample #] " << 
			"[stop sample #]" << endl << endl;
		exit(1);
	}
		
	dalDataset * dataset;
	dataset = new dalDataset( FILENAME, "HDF5" );

	// root-level headers
	int n_stations[] = { 32 };
	vector<string> srcvec;
	srcvec.push_back( "PSR J1643-1224" );
	srcvec.push_back( "PSR J1643-1225" );
	srcvec.push_back( "PSR J1643-1226" );
	srcvec.push_back( "PSR J1643-1227" );
	double epoch_mjd[] = { 54218.928347566934865 };
	int main_beam_diam[] = { 60 };
	int center_freq[] = { 180 };
	int bandwidth[] = { 90 }; // Total bandwidth (MHz)
	double total_integration_time[] = { 1800.000 };
	int breaks_in_data[] = { 0 }; // Any breaks in data?
	int dispersion_measure[] = { 42 };
	int number_of_samples[] = { 1923879 };
	double sampling_time[] = { 0.0001024 };
	int number_of_beams[] = { 3 };
	int sub_beam_diameter[] = { 5 }; // fwhm of the sub-beams (arcmin)
	int weather_temperature[] = { 32 }; // approx. centigrade
	int weather_humidity[] = { 88 }; // approx. %
	int tsys[] = { 122, 188, 116 }; // for various stations (K)
	// write headers using above
	dataset->setAttribute_string( "FILENAME", "bh.h5" );
	dataset->setAttribute_string( "TELESCOPE", "LOFAR" );
	dataset->setAttribute_int(    "NUMBER_OF_STATIONS", n_stations );
	dataset->setAttribute_string( "DATATYPE", "Timing" );
	dataset->setAttribute_string( "EMBAND", "Radio_HIGH" );
	dataset->setAttribute_string( "SOURCE", srcvec );
	dataset->setAttribute_string( "OBSERVATION_ID", "PSR_1234" );
	dataset->setAttribute_string( "PROJ_ID", "PSR_Scint_Studies" );
	dataset->setAttribute_string( "POINT_RA", "14:12:55:5112" );
	dataset->setAttribute_string( "POINT_DEC", "79:12:55:5112" );
	dataset->setAttribute_string( "OBSERVER", "Bob the Builder" );
	dataset->setAttribute_double( "EPOCH_MJD", epoch_mjd );
	dataset->setAttribute_string( "EPOCH_DATE", "20/07/08" );
	dataset->setAttribute_string( "EPOCH_UTC", "22:53:23" );
	dataset->setAttribute_string( "EPOCH_LST", "13:08:33" );
	dataset->setAttribute_int( "MAIN_BEAM_DIAM", main_beam_diam );
	dataset->setAttribute_int( "CENTER_FREQUENCY", center_freq );
	dataset->setAttribute_int( "BANDWIDTH", bandwidth );
	dataset->setAttribute_double( "TOTAL_INTEGRATION_TIME",
	  total_integration_time );
	dataset->setAttribute_int( "BREAKS_IN_DATA", breaks_in_data );
	dataset->setAttribute_int( "DISPERSION_MEASURE", dispersion_measure );
	dataset->setAttribute_int( "NUMBER_OF_SAMPLES", number_of_samples );
	dataset->setAttribute_double( "SAMPLING_TIME", sampling_time );
	dataset->setAttribute_string( "NOTES", "Test data for pulsar survey" );
	dataset->setAttribute_int( "NUMBER_OF_BEAMS", number_of_beams );
	dataset->setAttribute_int( "SUB_BEAM_DIAMETER", sub_beam_diameter );
	dataset->setAttribute_int( "WEATHER_TEMPERATURE", weather_temperature );
	dataset->setAttribute_int( "WEATHER_HUMIDITY", weather_humidity );
	dataset->setAttribute_int( "TSYS", tsys, 3 );


	dalGroup * beamGroup;
	char * beamstr = new char[10];

	int beam_number;

	int center_frequency[] = { 140 };
	double dataBandwidth[] = { 0.156 };
	double channel_bandwidth[] = { 0.156 };
	double channel_center_freq[] = { 140.0000, 140.0150, 140.0300 };

	beam_number = 0;
	sprintf( beamstr, "beam%03d", beam_number );
        beamGroup = dataset->createGroup( beamstr );
	beamGroup->setAttribute_string( "RA", "14:12:34.2342" );
	beamGroup->setAttribute_string( "DEC", "14:12:34.2342" );
	cout << "CREATED New beam group: " << string(beamstr) << endl;
	dalTable * dataTable0 = dataset->createTable( "SUB0", beamstr );
	dataTable0->setAttribute_int( "CENTER_FREQUENCY", center_frequency );
	dataTable0->setAttribute_double( "BANDWIDTH", dataBandwidth );
	dataTable0->setAttribute_double( "CHANNEL_BANDWIDTH", channel_bandwidth );
	dataTable0->setAttribute_double( "CHANNEL_CENTER_FREQUENCY", channel_center_freq, 3 );
	dataTable0->addColumn( "X", dal_COMPLEX_CHAR );
	dataTable0->addColumn( "Y", dal_COMPLEX_CHAR );

	const long BufferSIZE = 1000;
	typedef struct dataStruct {
		dalcomplex_char xx;
		dalcomplex_char yy;
	} dataStruct;

	dataStruct data_s[ BufferSIZE ];
	delete [] beamstr;

	// define memory buffers
	typedef struct Polarization {
		complex<char> val;
	};
	typedef struct Sample {
		Polarization X;
		Polarization Y;
	};

	// define memory buffers
	char header[4096];
	Sample p_Data[ BufferSIZE ];
	int size = sizeof(Sample) * BufferSIZE;

	// declare handle for the input file
	cout << "Reading from the data file: " << argv[1] << endl;
	ifstream myFile (argv[1], ios::in | ios::binary);

	cout << "read pointer position: " << myFile.tellg() << endl;
	if (!myFile.read (header, sizeof(header) ))
	{
	   cout << "ERROR: problem with read (3)." << endl;
	   cout << "read pointer position: " << myFile.tellg() << endl;
	   exit(3);
	}
	myFile.clear();
	int counter = 0;
	if (myFile.is_open())
	{
		while ( !myFile.eof()/* && counter < 100000*/ )
		{
		  if ( !myFile.read (reinterpret_cast<char *>(&p_Data), size) )
		  {
		     cout << "ERROR: problem with read (2)." << endl;
		     cout << "read pointer position: " << myFile.tellg() << endl;
		     exit(2);
		  }
		  for (int row=0; row<BufferSIZE; row++)
		  {
		    data_s[row].xx.r = real((p_Data[row]).X.val);
		    data_s[row].xx.i = imag((p_Data[row]).X.val);
		    data_s[row].yy.r = real((p_Data[row]).Y.val);
		    data_s[row].yy.i = imag((p_Data[row]).Y.val);
		  }
		  dataTable0->appendRows( data_s, BufferSIZE );

		  if ( !myFile.eof() )
		  {
		    myFile.clear();
		    counter+=BufferSIZE;
		  }
		}
	}
	else cout << "Unable to open file" << argv[1] << endl;
	return 0;
}
