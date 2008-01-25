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
	dataTable0->addColumn( "CHANNEL001", dal_DOUBLE );
	dataTable0->addColumn( "CHANNEL002", dal_DOUBLE );
	dataTable0->addColumn( "CHANNEL003", dal_DOUBLE );
	dataTable0->addColumn( "CHANNEL004", dal_DOUBLE );
	dataTable0->addColumn( "CHANNEL005", dal_DOUBLE );
	dataTable0->addColumn( "CHANNEL006", dal_DOUBLE );
	dataTable0->addColumn( "CHANNEL007", dal_DOUBLE );
	dataTable0->addColumn( "CHANNEL008", dal_DOUBLE );
	dataTable0->addColumn( "CHANNEL009", dal_DOUBLE );
	dataTable0->addColumn( "CHANNEL010", dal_DOUBLE );
 
	beam_number = 1;
	sprintf( beamstr, "beam%03d", beam_number );
        beamGroup = dataset->createGroup( beamstr );
	beamGroup->setAttribute_string( "RA", "14:12:34.2342" );
	beamGroup->setAttribute_string( "DEC", "14:12:34.2342" );
	cout << "CREATED New beam group: " << string(beamstr) << endl;
	dalTable * dataTable1 = dataset->createTable( "SUB0", beamstr );
	dataTable1->setAttribute_int( "CENTER_FREQUENCY", center_frequency );
	dataTable1->setAttribute_double( "BANDWIDTH", dataBandwidth );
	dataTable1->setAttribute_double( "CHANNEL_BANDWIDTH", channel_bandwidth );
	dataTable1->setAttribute_double( "CHANNEL_CENTER_FREQUENCY", channel_center_freq, 3 );
	dataTable1->addColumn( "CHANNEL001", dal_DOUBLE );
	dataTable1->addColumn( "CHANNEL002", dal_DOUBLE );
	dataTable1->addColumn( "CHANNEL003", dal_DOUBLE );
	dataTable1->addColumn( "CHANNEL004", dal_DOUBLE );
	dataTable1->addColumn( "CHANNEL005", dal_DOUBLE );
	dataTable1->addColumn( "CHANNEL006", dal_DOUBLE );
	dataTable1->addColumn( "CHANNEL007", dal_DOUBLE );
	dataTable1->addColumn( "CHANNEL008", dal_DOUBLE );
	dataTable1->addColumn( "CHANNEL009", dal_DOUBLE );
	dataTable1->addColumn( "CHANNEL010", dal_DOUBLE );

	beam_number = 2;
	sprintf( beamstr, "beam%03d", beam_number );
        beamGroup = dataset->createGroup( beamstr );
	beamGroup->setAttribute_string( "RA", "14:12:34.2342" );
	beamGroup->setAttribute_string( "DEC", "14:12:34.2342" );
	cout << "CREATED New beam group: " << string(beamstr) << endl;
	dalTable * dataTable2 = dataset->createTable( "SUB0", beamstr );
	dataTable2->setAttribute_int( "CENTER_FREQUENCY", center_frequency );
	dataTable2->setAttribute_double( "BANDWIDTH", dataBandwidth );
	dataTable2->setAttribute_double( "CHANNEL_BANDWIDTH", channel_bandwidth );
	dataTable2->setAttribute_double( "CHANNEL_CENTER_FREQUENCY", channel_center_freq, 3 );
	dataTable2->addColumn( "CHANNEL001", dal_DOUBLE );
	dataTable2->addColumn( "CHANNEL002", dal_DOUBLE );
	dataTable2->addColumn( "CHANNEL003", dal_DOUBLE );
	dataTable2->addColumn( "CHANNEL004", dal_DOUBLE );
	dataTable2->addColumn( "CHANNEL005", dal_DOUBLE );
	dataTable2->addColumn( "CHANNEL006", dal_DOUBLE );
	dataTable2->addColumn( "CHANNEL007", dal_DOUBLE );
	dataTable2->addColumn( "CHANNEL008", dal_DOUBLE );
	dataTable2->addColumn( "CHANNEL009", dal_DOUBLE );
	dataTable2->addColumn( "CHANNEL010", dal_DOUBLE );
 
	delete [] beamstr;

   // define dimensions of array
   vector<int> dims;
   dims.push_back(4);
   dims.push_back(5);
   dims.push_back(6);

   int idata[4*5*6];
   for (int gg=0; gg<(4*5*6); gg++)
	idata[gg] = gg;

   vector<int> cdims;

	cout << "Creating a complex floating point array with dimensions 4x5x6... ";
	complex<float> * cdata = new complex<float>[ 4*5*6 ];
	for (int gg=0; gg<(4*5*6); gg++) {
	  cdata[ gg ] = 0;
	}
	dalArray * carray = dataset->createComplexArray( "complex_array", dims, cdata, cdims );
	delete cdata;
	cout << "done." << endl;

	// define memory buffers
	unsigned char * memblock=NULL;
	typedef struct Polarization {
		complex<unsigned char> val;
	};
	typedef struct Sample {
		Polarization X;
		Polarization Y;
	};

	// define memory buffers
	char header[4096];
	unsigned char buffer[1];
	Sample p_Data[1];
	int size = sizeof(Sample);
	memblock = new unsigned char [size];

	// declare handle for the input file
	ifstream myFile (argv[1], ios::in | ios::binary);

	cout << "read pointer position: " << myFile.tellg() << endl;
	if (!myFile.read (header, sizeof(header) ))
	{
	   cout << "ERROR: problem with read." << endl;
	   exit(3);
	}
	myFile.clear();
	if (myFile.is_open())
	{
		while ( !myFile.eof() )
		{
		  if ( !myFile.read (reinterpret_cast<char *>(&p_Data), size) )
		  {
		     cout << "ERROR: problem with read." << endl;
		     exit(2);
		  }
		  printf("(%hi,",real((*p_Data).X.val));
		  printf("%hi)",imag((*p_Data).X.val));
		  printf("(%hi,",real((*p_Data).Y.val));
		  printf("%hi)\n",imag((*p_Data).Y.val));
		  myFile.clear();
		}
		exit(0);
	}
	else cout << "Unable to open file" << argv[1] << endl;
	return 0;
}
