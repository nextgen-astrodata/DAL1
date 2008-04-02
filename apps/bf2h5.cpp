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
#include <assert.h>
#include <new>

using namespace std;
using std::complex;

//#define FILENAME "/mnt/disk2/data/cs1/pulsar/B0329.h5"
#define DEBUG

typedef struct FileHeader {
   UInt8     magic[4];        // 0x3F8304EC, also determines endianness
   UInt8     bitsPerSample;
   UInt8     nrPolarizations;
   UInt16    nrBeamlets;
   UInt32    nrSamplesPerBeamlet; // 155648 (160Mhz) or 196608 (200Mhz)
   char      station[20];
   Float64    sampleRate;
   Float64    subbandFrequencies[54];
   Float64    beamDirections[8][2];
   Int16     beamlet2beams[54];
   UInt8     padding;
} FileHeader;

typedef struct BlockHeader {
   UInt8       magic[4]; // 0x2913D852
   Int32       coarseDelayApplied[8];
   Float64     fineDelayRemainingAtBegin[8];
   Float64     fineDelayRemainingAfterEnd[8];
   Int64       time[8]; // compatible with TimeStamp class.
   UInt32      nrFlagsRanges[8];
   struct range {
     UInt32    begin; // inclusive
     UInt32    end;   // exclusive
   } flagsRanges[8][16];
 } BlockHeader;

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

int main (int argc, char *argv[])
{

  // If less than two arguments provided, print usage
  if ( 3 != argc ) {
    cout << "Usage:  bf2h5 <input filename> <output filename>" << endl << endl;
    exit(1);
  }

  bool bigendian = BigEndian();

  // dataStruct is 8 bytes
  typedef struct dataStruct {
    dalcomplex_int16 xx;
    dalcomplex_int16 yy;
  } dataStruct;


  const unsigned long BufferSIZE = 155648;//608;

  dataStruct * data_s = NULL; // pointer to dataStruct
  try
  {
    data_s = new dataStruct[ BufferSIZE ];  // allocate memory
  }
  catch ( bad_alloc )
  {
    cerr << "Could not allocate memory buffer" << endl;
    exit(3);
  }


  // define memory buffers
  FileHeader fileheader;
  BlockHeader blockheader;


#ifdef DEBUG
  cout << "Reading from the data file: " << argv[1] << endl;
#endif


  // declare handle for the input file
  ifstream myFile (argv[1], ios::in | ios::binary);

//----------------------------------------------------- read file header

  if (!myFile.read ( reinterpret_cast<char *>(&fileheader),
                     sizeof(fileheader) ))
  {
     cout << "ERROR: problem with read (3)." << endl;
     cout << "read pointer position: " << myFile.tellg() << endl;
     exit(3);
  }

#ifdef DEBUG
  printf("size of file header: %ld\n", sizeof(fileheader));
  cout << "read pointer position: " << myFile.tellg() << endl;
#endif

  // swap values when necessary
  if ( !bigendian )
  {
    fileheader.nrBeamlets = Int16Swap( fileheader.nrBeamlets );
    fileheader.nrSamplesPerBeamlet = Int32Swap( fileheader.nrSamplesPerBeamlet );
    fileheader.sampleRate = Int64Swap( fileheader.sampleRate );
    for (unsigned int idx=0; idx<54; idx++)
    {
      fileheader.subbandFrequencies[idx] =
        Int64Swap( fileheader.subbandFrequencies[idx] );

      fileheader.beamlet2beams[idx] =
        Int16Swap( fileheader.beamlet2beams[idx] );
    }
    for (unsigned int aa=0; aa<8; aa++)
    {
      for (unsigned int bb=0; bb<2; bb++)
      {
        fileheader.beamDirections[aa][bb] =
         Int64Swap( fileheader.beamDirections[aa][bb]);
      }
    }
  }


  dalDataset * dataset;
  dataset = new dalDataset( argv[2], "HDF5" );

  // root-level headers
  int n_stations[] = { 1 };
  vector<string> srcvec;
  srcvec.push_back( "A" );
  srcvec.push_back( "B" );
  srcvec.push_back( "C" );
  srcvec.push_back( "D" );
  Float64 epoch_mjd[] = { 0 };
  int main_beam_diam[] = { 0 };
  int center_freq[] = { 0 };
  int bandwidth[] = { 0 }; // Total bandwidth (MHz)
  Float64 total_integration_time[] = { 0 };
  int breaks_in_data[] = { 0 }; // Any breaks in data?
  int dispersion_measure[] = { 0 };
  int number_of_samples[] =
     { fileheader.nrBeamlets * fileheader.nrSamplesPerBeamlet };
  Float64 sampling_time[] = { fileheader.sampleRate };
  int number_of_beams[] = { fileheader.nrBeamlets };
  int sub_beam_diameter[] = { 0 }; // fwhm of the sub-beams (arcmin)
  int weather_temperature[] = { 0 }; // approx. centigrade
  int weather_humidity[] = { 0 }; // approx. %
  int tsys[] = { 0 }; // for various stations (K)

  // write headers using above
  dataset->setAttribute_string( "FILENAME", argv[2] );
  dataset->setAttribute_string( "TELESCOPE", "LOFAR" );
  dataset->setAttribute_int( "NUMBER_OF_STATIONS", n_stations );
  dataset->setAttribute_string( "DATATYPE", "" );
  dataset->setAttribute_string( "EMBAND", "" );
  dataset->setAttribute_string( "SOURCE", srcvec );
  dataset->setAttribute_string( "OBSERVATION_ID", "" );
  dataset->setAttribute_string( "PROJ_ID", "" );
  dataset->setAttribute_string( "POINT_RA", "" );
  dataset->setAttribute_string( "POINT_DEC", "" );
  dataset->setAttribute_string( "OBSERVER", "" );
  dataset->setAttribute_double( "EPOCH_MJD", epoch_mjd );
  dataset->setAttribute_string( "EPOCH_DATE", "" );
  dataset->setAttribute_string( "EPOCH_UTC", "" );
  dataset->setAttribute_string( "EPOCH_LST", "" );
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

  Float64 dataBandwidth[] = { 0 };
  Float64 channel_bandwidth[] = { 0 };
  Float64 channel_center_freq[] = { 0, 0, 0 };

  beam_number = 0;
  sprintf( beamstr, "beam%03d", beam_number );
  beamGroup = dataset->createGroup( beamstr );
  beamGroup->setAttribute_string( "RA", "" );
  beamGroup->setAttribute_string( "DEC", "" );
  int n_subbands[] = { 1 };
  beamGroup->setAttribute_int( "NUMBER_OF_SUBBANDS", n_subbands );

#ifdef DEBUG
	cout << "CREATED New beam group: " << string(beamstr) << endl;
#endif

  dalTable ** table;
  table = new dalTable * [ fileheader.nrBeamlets ];

  char * sbName = new char[8];
  int center_frequency[] = { 0 };
  // nrBeamlets is actually the number of subbands (see email from J.Romein)
  for (unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
  {
    sprintf( sbName, "SB%03d", idx );
    table[idx] = dataset->createTable( sbName, beamstr );
    center_frequency[0] = fileheader.subbandFrequencies[ idx ];
    table[idx]->setAttribute_int( "CENTER_FREQUENCY", center_frequency );
    table[idx]->setAttribute_double( "BANDWIDTH", dataBandwidth );
    table[idx]->setAttribute_double( "CHANNEL_BANDWIDTH", 
                                      channel_bandwidth );
    table[idx]->setAttribute_double( "CHANNEL_CENTER_FREQUENCY",
                                      channel_center_freq,
                                      3 );
    table[idx]->addColumn( "X", dal_COMPLEX_SHORT );
    table[idx]->addColumn( "Y", dal_COMPLEX_SHORT );
  }

  delete [] beamstr;

  int xx=0;
  int counter = 0;
  while ( myFile.read ( reinterpret_cast<char *>(&blockheader),
                        sizeof(blockheader) ) /*&& xx < 10*/ )
  {

    // swap values when necessary
    if ( !bigendian )
    {
      for (unsigned int idx=0; idx<8; idx++)
      {
        blockheader.coarseDelayApplied[idx] =
           Int32Swap( blockheader.coarseDelayApplied[idx] );
        blockheader.fineDelayRemainingAtBegin[idx] =
           Int64Swap( blockheader.fineDelayRemainingAtBegin[idx] );
        blockheader.fineDelayRemainingAfterEnd[idx] =
           Int64Swap( blockheader.fineDelayRemainingAfterEnd[idx] );
        blockheader.time[idx] = Int64Swap( blockheader.time[idx] );
        blockheader.nrFlagsRanges[idx] =
           Int32Swap( blockheader.nrFlagsRanges[idx] );
        for(unsigned int idx2=0; idx2<16; idx2++)
        {
          blockheader.flagsRanges[idx][idx2].begin =
            Int32Swap( blockheader.flagsRanges[idx][idx2].begin );
          blockheader.flagsRanges[idx][idx2].end =
            Int32Swap( blockheader.flagsRanges[idx][idx2].end );
        }
      }
    }

    xx++;

   for(unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
    {
        if ( !myFile.read ( reinterpret_cast<char *>(data_s),
                           sizeof(dataStruct)*BufferSIZE) )
         {
          cout << "ERROR: problem with read (2)." << endl;
          cout << "read pointer position: " << myFile.tellg() << endl;
          exit(2);
         }
         table[idx]->appendRows( data_s, BufferSIZE );
         if ( !myFile.eof() )
         {
           myFile.clear();
           counter+=BufferSIZE;
         }
     }
   }

  // cleanup memory
  for (unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
  {
    sprintf( sbName, "SB_%03d", idx );
    delete table[idx];
  }
  delete [] sbName;
  delete [] table;
  delete [] data_s;

  return 0;
}
