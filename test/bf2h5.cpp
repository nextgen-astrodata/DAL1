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
		
	// Declare some constants that define the beam-formed format
	const Float32 BLOCK_TIME = 155648.00/156250.00; // ~1 second per block
	const Int32 OLDSAMPLE_LENGTH = 3840;
	const Int32 NEWSAMPLE_LENGTH = 155648;

	Float32 observation_time = 0;	// total observation time
	Float32 read_time = 0;	// time actually read from the file
	Int32 xx=0;			// loop counter
	Int32 ranges = -1;		// dropped samples
	UInt32 header,footer;		// header & footer buffer
	ifstream::pos_type size=0;	// buffer size
	
	// define memory buffers
	unsigned char * memblock=NULL;
	typedef struct Polarization {
		complex<short> val;
	};
	typedef struct Sample {
		Polarization X;
		Polarization Y;
	};

	dalDataset * dataset;
	dataset = new dalDataset( FILENAME, "HDF5" );

	dalGroup * beamGroup;
	char * beamstr = new char[10];

	string filename = string(FILENAME);
	string telescope = "LOFAR";
	int n_stations[] = { 32 };

	dataset->setAttribute_string( "FILENAME", filename );
	dataset->setAttribute_string( "TELESCOPE", telescope );
	dataset->setAttribute_int(    "N_STATIONS", n_stations );
// 	stationGroup->setAttribute_double("BEAM_DIR", beam_direction, 2 );

	sprintf( beamstr, "beam%03d", 1 );
        beamGroup = dataset->createGroup( beamstr );
	cout << "CREATED New beam group: " << string(beamstr) << endl;

	sprintf( beamstr, "beam%03d", 2 );
        beamGroup = dataset->createGroup( beamstr );
	cout << "CREATED New beam group: " << string(beamstr) << endl;

	sprintf( beamstr, "beam%03d", 3 );
        beamGroup = dataset->createGroup( beamstr );
	cout << "CREATED New beam group: " << string(beamstr) << endl;

	delete [] beamstr;

	// declare handle for the input file
	fstream file (argv[1], ios::binary|ios::in);
  
	if (file.is_open())
	{
	        long count = 0;
		
		Sample p_Data;
		Sample oldsample[ OLDSAMPLE_LENGTH ];
		Sample newsample[ NEWSAMPLE_LENGTH ];
		size = 4;
		memblock = new unsigned char [size];
		file.seekg (0, ios::beg);

	        // print a small banner before the data
		print_banner();
		
		// loop through the file
		while ( !file.eof() ) {

			// scan for 32-byte header by reading 4 bytes at a time
			// and looking for the hex signature: bebebebe
			do {
				file.read(reinterpret_cast<char *>(&header), sizeof(header));
			} while ( header != 0xbebebebe && !file.eof() );
			file.read((char*)memblock, 28);  // read last 28 bytes of header

			// read old data: 3840 bytes
			file.read( reinterpret_cast<char *>(&oldsample),
					   sizeof(Sample)*OLDSAMPLE_LENGTH );
			
			// read 155648 samples (8 bytes each)
			file.read( reinterpret_cast<char *>(&newsample),
					   sizeof(Sample)*NEWSAMPLE_LENGTH );

			// write out data
			if ( 4 == argc )
			{
			  if ( atol(argv[3]) <= atol(argv[2]) )
			  {
			     cout << "ERROR: start time must be less than stop time." << endl;
			     exit(-1);
			  }

			  if ( count >= atol(argv[2]) && count < atol(argv[3]) )
			  {
			    for (xx=0; xx < NEWSAMPLE_LENGTH; xx++)
				{
				  cout << setw(20) << xx+1;
				  cout << left << '(' << real(newsample[xx].X.val) << ','
				       << imag(newsample[xx].X.val) << ')' << '\t';;
				  cout << left << '(' << real(newsample[xx].Y.val) << ','
					   << imag(newsample[xx].Y.val) << ')' << endl;
			     }
			     read_time += BLOCK_TIME;
			  }
			  else if ( count >= atol(argv[3]) )
			  {
		  	     cout << "Total time read from the file: " << read_time << " seconds." << endl;
		  	     cout << "\tRange: " << atol(argv[2])*BLOCK_TIME << " seconds ";
		  	     cout << "to " << atol(argv[3])*BLOCK_TIME << " seconds." << endl;
			     exit(0);
			  }
			     
			}
			else if ( 3 == argc )
			{
			  if ( count >= atol(argv[2]) )
			  {
			    for (xx=0; xx < NEWSAMPLE_LENGTH; xx++)
				{
				  cout << setw(20) << xx+1;
				  cout << left << '(' << real(newsample[xx].X.val) << ','
				       << imag(newsample[xx].X.val) << ')' << '\t';;
				  cout << left << '(' << real(newsample[xx].Y.val) << ','
					   << imag(newsample[xx].Y.val) << ')' << endl;
			     }
			  }
			}
			else
			{
			   for (xx=0; xx < NEWSAMPLE_LENGTH; xx++)
			   {
				 cout << setw(20) << xx+1;
				 cout << left << '(' << real(newsample[xx].X.val) << ','
				   << imag(newsample[xx].X.val) << ')' << '\t';;
			         cout << left << '(' << real(newsample[xx].Y.val) << ','
			           << imag(newsample[xx].Y.val) << ')' << endl;
			   }
			}

			// scan for "Flags" (or simply skip 33 bytes)
			size = 33;
			file.read((char*)memblock, size);

			// Read the number of lost samples
			file.read( reinterpret_cast<char *>(&ranges), sizeof(ranges) );
			//cout << "Lost samples: " << dec << ranges << endl;

			// scan for footer, signature: 0xbfbfbfbf
			do {
				file.read( reinterpret_cast<char *>(&footer), sizeof(footer) );
			} while ( footer != 0xbfbfbfbf && !file.eof() );

			// for some reason there is now an extra 0xbfbfbfbf000000 after the
			//  above footer signature ???
			size = 7;
			file.read((char*)memblock, size);
			
			count += 1;
			observation_time += BLOCK_TIME;
		}
		cout << "Total observation time: " << observation_time << " seconds." << endl;

		delete dataset;

		exit(0);
//  		file.close();
//  		delete[] memblock;
	}
	else cout << "Unable to open file" << argv[1] << endl;
	return 0;
}
