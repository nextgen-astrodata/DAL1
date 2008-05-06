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
using namespace DAL;
using std::complex;

#define PI 3.141592653589793238462643
char * RArad2deg( const float &rad )
{
  char * ra_string = new char[14];

  float deg_hour = ( rad * (180./PI) ) / 15.;
  int   hour = (int)deg_hour;
  float deg_min = (deg_hour - hour) * 60.;
  int    min = int( deg_min );
  float  sec = (deg_min - min) * 60;

  sprintf(ra_string,"%02d:%02d:%02.4f", hour, min, sec);

  return ra_string;
}

char * DECrad2deg( const float &rad )
{
  char * ra_string = new char[14];

  float deg_hour = ( rad * (180./PI) );
  int   hour = int( deg_hour );
  float deg_min = (deg_hour - hour) * 60.;
  int    min = int( deg_min );
  float  sec = (deg_min - min) * 60;

  sprintf(ra_string,"%02d:%02d:%02.4f", hour, min, sec);

  return ra_string;
}

typedef struct FileHeader {
   UInt32    magic;        // 0x3F8304EC, also determines endianness
   UInt8     bitsPerSample;
   UInt8     nrPolarizations;
   UInt16    nrBeamlets;
   UInt32    nrSamplesPerBeamlet; // 155648 (160Mhz) or 196608 (200Mhz)
   char      station[20];
   Float64   sampleRate;       //156250.0 or 195312.5 .. double
   Float64   subbandFrequencies[54];
   Float64   beamDirections[8][2];
   Int16     beamlet2beams[54];
   UInt32    padding;  // padding to circumvent 8-byte alignment
} FileHeader;

typedef struct BlockHeader {
   UInt8       magic[4]; // 0x2913D852
   Int32       coarseDelayApplied[8];
   UInt8       padding[4];  // padding to circumvent 8-byte alignment
   Float64     fineDelayRemainingAtBegin[8];
   Float64     fineDelayRemainingAfterEnd[8];
   Int64       time[8]; // compatible with TimeStamp class.
   UInt32      nrFlagsRanges[8];
   struct range {
     UInt32    begin; // inclusive
     UInt32    end;   // exclusive
   } flagsRanges[8][16];
 } BlockHeader;

  // dataStruct is 8 bytes
  typedef struct dataStruct {
    complex<short> xx;
    complex<short> yy;
  } dataStruct;

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

dataStruct * downsample( dataStruct * data,
                         int start,
                         const unsigned long arraylength,
                         int factor )
{
  const int DS_SIZE = arraylength / factor;
  dataStruct * ds_data = NULL;
  try
  {
    ds_data = new dataStruct[ DS_SIZE ];
  }
  catch (bad_alloc)
  {
    cerr << "Can't allocate memory for downsampled array." << endl;
  }

  for (int count=0; count<DS_SIZE; count++)
  {
    for (int idx=start; idx < (start+factor); idx++)
    {
      ds_data[count].xx += data[idx].xx;
      ds_data[count].yy += data[idx].yy;
    }
    start += factor;
  }
  return ds_data;
}

Float32 * downsample_to_float32_intensity( dataStruct * data,
                                           int start,
                                           const unsigned long arraylength,
                                           int factor )
{
  const int DS_SIZE = arraylength / factor;
  double xx_intensity = 0;
  double yy_intensity = 0;
  Float32 * ds_data = NULL;
  try
  {
    ds_data = new Float32[ DS_SIZE ];
  }
  catch (bad_alloc)
  {
    cerr << "Can't allocate memory for downsampled array." << endl;
  }

  for (int count=0; count<DS_SIZE; count++)
  {
    ds_data[count] = 0;
    for (int idx=start; idx < (start+factor); idx++)
    {
      xx_intensity = ( (double)real(data[idx].xx)*real(data[idx].xx) +
                       (double)imag(data[idx].xx)*imag(data[idx].xx) );
      yy_intensity = ( (double)real(data[idx].yy)*real(data[idx].yy) +
                       (double)imag(data[idx].yy)*imag(data[idx].yy) );
      ds_data[count] +=
        (Float32)std::sqrt( xx_intensity*xx_intensity + yy_intensity*yy_intensity );
    }
    start += factor;
  }
  return ds_data;
}

int main (int argc, char *argv[])
{

  // If less than two arguments provided, print usage
  if ( 3 != argc ) {
    cout << "Usage:  bf2h5 <input filename> <output filename>" << endl << endl;
    exit(1);
  }

  bool bigendian = BigEndian();

  int downsample_factor = 128;

  const bool DO_DOWNSAMPLE = false;
  const bool DO_FLOAT32_INTENSITY = false;

  // define memory buffers
  FileHeader fileheader;
  BlockHeader blockheader;


#ifdef DEBUGGING_MESSAGES
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

#ifdef DEBUGGING_MESSAGES
  printf("size of file header: %lu\n", sizeof(fileheader));
  cout << "read pointer position: " << myFile.tellg() << endl;
#endif

  // swap values when necessary
  if ( !bigendian )
  {
    // change byte order for all frequencies.
    for (int ii=0;ii<54;ii++)
    {
      swapbytes((char *)&fileheader.subbandFrequencies[ii],8);
      swapbytes((char *)&fileheader.beamlet2beams,2);
    }

    for (int ii=0;ii<8;ii++)
    {
      for (int jj=0;jj<2;jj++)
      {
        swapbytes((char *)&fileheader.beamDirections[ii][jj],8);
      }
    }

    // change byte order for beamlets.
    swapbytes((char *)&fileheader.nrBeamlets,2);

    // change byte order for nrSamplesPerBeamlet
    swapbytes((char *)&fileheader.nrSamplesPerBeamlet,4);

    // change byte order for sampleRate
    swapbytes((char *)&fileheader.sampleRate,8);

    // change byte order for Magic Number
    swapbytes((char *)&fileheader.magic,4);

   }

   #ifdef DEBUGGING_MESSAGES
   printf("Magic number: %8X\n",fileheader.magic);
   printf("bits per sample: %u\n",fileheader.bitsPerSample);
   printf("Polarizations : %u\n",fileheader.nrPolarizations);
   printf("Beamlets : %u\n",fileheader.nrBeamlets);
   printf("Samples per beamlet: %u\n", fileheader.nrSamplesPerBeamlet);
   printf("Station ID: %s\n", fileheader.station);
   printf("Sample rate: %g\n", fileheader.sampleRate);
   printf("Centre Freq. of subbands (MHz): \n");
   for (int ii=0;ii<fileheader.nrBeamlets;ii++)
   {
     printf("%9.6f ",fileheader.subbandFrequencies[ii]/1000000.0);
     if (((ii+1)%4) == 0 ) printf("\n");
   }
   printf("Beam Directions J2000 radians:\n");
   char * ra = NULL;
   char * dec = NULL;
   for (int ii=0;ii<8;ii++)
   {
     printf("[%d] ", ii );
     for (int jj=0;jj<2;jj++)
     {
        printf("%f   ",fileheader.beamDirections[ii][jj]);
        if ( 0 == jj )
           ra = RArad2deg( fileheader.beamDirections[ii][jj] );
        else
           dec = DECrad2deg( fileheader.beamDirections[ii][jj] );
     }
     printf("[  %s, %s ]", ra, dec );
     printf("\n");
     delete [] ra;
     ra = NULL;
     delete [] dec;
     dec = NULL;
   }

   #endif

  const unsigned long BufferSIZE = fileheader.nrSamplesPerBeamlet;

  dalDataset * dataset;
  dataset = new dalDataset( argv[2], "HDF5" );

  // root-level headers
  int n_stations[] = { 1 };
  vector<string> srcvec;
  srcvec.push_back( "" );
  int main_beam_diam[] = { 0 };
  int bandwidth[] = { 0 }; // Total bandwidth (MHz)
  int breaks_in_data[] = { 0 }; // Any breaks in data?
  int dispersion_measure[] = { 0 };
  int number_of_samples[] =
     { fileheader.nrBeamlets * fileheader.nrSamplesPerBeamlet };
  Float64 sampling_time[] = { fileheader.sampleRate };
  int number_of_beams[] = { 1 };
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
  dataset->setAttribute_int( "MAIN_BEAM_DIAM", main_beam_diam );
//   dataset->setAttribute_int( "CENTER_FREQUENCY", center_freq );
  dataset->setAttribute_int( "BANDWIDTH", bandwidth );
  dataset->setAttribute_int( "BREAKS_IN_DATA", breaks_in_data );
  dataset->setAttribute_int( "DISPERSION_MEASURE", dispersion_measure );
  dataset->setAttribute_int( "NUMBER_OF_SAMPLES", number_of_samples );
  dataset->setAttribute_double( "SAMPLING_TIME", sampling_time );
  dataset->setAttribute_string( "NOTES", "Test data for pulsar survey" );
  dataset->setAttribute_int( "NUMBER_OF_BEAMS", number_of_beams );
  dataset->setAttribute_int( "SUB_BEAM_DIAMETER", sub_beam_diameter );
  dataset->setAttribute_int( "WEATHER_TEMPERATURE", weather_temperature );
  dataset->setAttribute_int( "WEATHER_HUMIDITY", weather_humidity );
  dataset->setAttribute_int( "TSYS", tsys );

  dalGroup * beamGroup;

  char * beamstr = new char[10];

  int beam_number;

  beam_number = 0;
  sprintf( beamstr, "beam%03d", beam_number );
  beamGroup = dataset->createGroup( beamstr );

  char * ra_val  = new char[20];
  char * dec_val = new char[20];
  sprintf( ra_val, "%f", fileheader.beamDirections[beam_number+1][0] );
  sprintf( dec_val, "%f",fileheader.beamDirections[beam_number+1][1] );
  beamGroup->setAttribute_string( "RA", ra_val );
  beamGroup->setAttribute_string( "DEC", dec_val );
  delete [] ra_val;
  ra_val = NULL;
  delete [] dec_val;
  dec_val = NULL;

  int n_subbands[] = { fileheader.nrBeamlets };
  beamGroup->setAttribute_int( "NUMBER_OF_SUBBANDS", n_subbands );

  delete beamGroup;


#ifdef DEBUGGING_MESSAGES
	cerr << "CREATED New beam group: " << string(beamstr) << endl;
#endif

  dalTable ** table;
  table = new dalTable * [ fileheader.nrBeamlets ];

  char * sbName = new char[8];
  int * center_frequency = new int[fileheader.nrBeamlets];

  // nrBeamlets is actually the number of subbands (see email from J.Romein)
  for (unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
  {
    sprintf( sbName, "SB%03d", idx );
    table[idx] = dataset->createTable( sbName, beamstr );
  }

  for (unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
  {
     if (DO_FLOAT32_INTENSITY)
     {
        table[idx]->addColumn( "TOTAL_INTENSITY", dal_FLOAT );
     }
     else
     {
       table[idx]->addColumn( "X", dal_COMPLEX_SHORT );
       table[idx]->addColumn( "Y", dal_COMPLEX_SHORT );
     }
  }
  for (unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
  {
    center_frequency[idx] = (int)fileheader.subbandFrequencies[ idx ];
    table[idx]->setAttribute_int( "CENTER_FREQUENCY", &center_frequency[idx] );
  }

  delete [] sbName;
  sbName = NULL;
  delete [] center_frequency;
  center_frequency = NULL;
  delete [] beamstr;
  beamstr = NULL;

  dataStruct * data_s = NULL; // pointer to dataStruct
  try
  {
    // allocate memory
    data_s = new dataStruct[ BufferSIZE * fileheader.nrBeamlets ];
  }
  catch ( bad_alloc )
  {
    cerr << "Could not allocate memory buffer" << endl;
    exit(3);
  }

  int xx=0;
  int counter = 0;
  bool first_block (true);
  while ( myFile.read ( reinterpret_cast<char *>(&blockheader),
                        sizeof(blockheader) ) && xx < 2 )
  {

    // swap values when necessary
    if ( !bigendian )
    {
      for (unsigned int idx=0; idx<8; idx++)
      {
        swapbytes((char *)&blockheader.coarseDelayApplied[idx],4);
        swapbytes((char *)&blockheader.fineDelayRemainingAtBegin[idx],8);
        swapbytes((char *)&blockheader.fineDelayRemainingAfterEnd[idx],8);
        swapbytes((char *)&blockheader.time[idx],8);
        swapbytes((char *)&blockheader.nrFlagsRanges[idx],4);
        for(unsigned int idx2=0; idx2<16; idx2++)
        {
          swapbytes( (char *)&blockheader.flagsRanges[idx][idx2].begin,4 );
          swapbytes( (char *)&blockheader.flagsRanges[idx][idx2].end,4 );
        }
      }
    }

   if ( first_block )
   {
     time_t utc;
     utc = (time_t)(blockheader.time[0]/(Int64)fileheader.sampleRate);
     char * timeDateString = NULL;
     uint buf_size = 128;
     if (!timeDateString)
       timeDateString = (char *)malloc(buf_size*sizeof(char));

     memset (timeDateString,'\0',buf_size);
     strftime(timeDateString, buf_size, "%T", gmtime(&utc));
     dataset->setAttribute_string( "EPOCH_UTC", timeDateString );

     memset (timeDateString,'\0',buf_size);
     strftime(timeDateString, buf_size, "%D", gmtime(&utc));
     dataset->setAttribute_string( "EPOCH_DATE", timeDateString );

     memset (timeDateString,'\0',buf_size);
     strftime(timeDateString, buf_size, "%D", gmtime(&utc));
     dataset->setAttribute_string( "EPOCH_MJD", "" );

     memset (timeDateString,'\0',buf_size);
     strftime(timeDateString, buf_size, "%D", gmtime(&utc));
     dataset->setAttribute_string( "EPOCH_LST", "" );
     free(timeDateString);
     first_block = false;
   }

    xx++;

  if ( !myFile.read ( reinterpret_cast<char *>(data_s),
                       sizeof(dataStruct)*BufferSIZE*fileheader.nrBeamlets) )
  {
     cout << "ERROR: problem with read (2)." << endl;
     cout << "read pointer position: " << myFile.tellg() << endl;
     exit(2);
   }
   for(unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
    {
         if ( DO_DOWNSAMPLE )  // if downsampling
         {
            if ( DO_FLOAT32_INTENSITY ) // if you want float32 intensities
            {
               Float32 * downsampled_data;
               int start = BufferSIZE*idx;
               downsampled_data =
                  downsample_to_float32_intensity( data_s,
                                                   start,
                                                   BufferSIZE,
                                                   downsample_factor );
               table[idx]->appendRows( downsampled_data,
                                       BufferSIZE / downsample_factor );
               delete [] downsampled_data;
               downsampled_data = NULL;
            }
            else // if you want complex numbers
            {
               dataStruct * downsampled_data;
               int start = BufferSIZE*idx;
               downsampled_data = downsample( data_s,
                                              start,
                                              BufferSIZE,
                                              downsample_factor );
               table[idx]->appendRows( downsampled_data,
                                       BufferSIZE / downsample_factor );
               delete [] downsampled_data;
               downsampled_data = NULL;
            }
         }
         else  // no downsampling
         {
            table[idx]->appendRows( data_s, BufferSIZE );
         }
         if ( !myFile.eof() )
         {
           myFile.clear();
           counter+=BufferSIZE;
         }
     }  // end for each beamlet(subband)

   }  // end for each block

  Float64 total_integration_time[1] =
     { xx * ( fileheader.nrSamplesPerBeamlet / fileheader.sampleRate ) };
  dataset->setAttribute_double( "TOTAL_INTEGRATION_TIME",
                                total_integration_time );

  // cleanup memory
  for (unsigned int idx=0; idx<fileheader.nrBeamlets; idx++)
  {
    delete table[idx];
  }
  delete [] table;
  table = NULL;

  delete dataset;

  delete [] data_s;
  data_s = NULL;

  return 0;
}
