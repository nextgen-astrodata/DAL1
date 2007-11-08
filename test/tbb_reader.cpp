/******************************************************************************
 *
 *  This file represents a Lofar Transient Buffer Board (TBB) data reader.
 * 
 *  Usage is:  tbb_reader <filename>
 *  Example:   tbb_reader tbb_cep_data_512_frames_rw.dat
 *
 *
 *  Sample output (transient mode):
 *
 *     Station Id:     11
 *     RSP Id:         0
 *     RCU Id:         0
 *     Sample Freq:    160 MHz
 *     Seq Number:     261632
 *     Time (s1970):   1171535717
 *     Sample Number:  61685760
 *     No. of Samps:   1024
 *     No. of Freqs.:  0
 *     CRC:            42269
 *     Samples (1):
 *     1,-1,5,1,1,10,1,2,-2,-4,-1,1,-5,10,11,-7,11,-3,0,-1,-1,4,4,0,6,10,-4,
 *     -2,1,-4,9,6,0,1,6,-2,-6,6,-4,6,12,-2,-3,12,-2,-6,7,-7,4,8,-5, ...
 *     Payload CRC: 3153134066
 *
 *  The program was tested on little-endian machines only.
 *
 *  Author:  Joseph Masters <jmasters at science dot uva dot nl>
 *  Created:  March 2007
 *
 ******************************************************************************/

// hdf5 include files
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

#define FILENAME "./tbb_output.h5"

// reading a complete binary file
#include <iostream> // for cout
#include <iomanip>  // for cout field width
#include <fstream>  // for file handle
#include <complex>  // for complex datatypes
#include <time.h>   // for local time conversion

// declare namespaces
using namespace std;
using std::complex;

// define some global datatypes
typedef unsigned int   UInt32;
typedef unsigned short UInt16;
typedef short           Int16;
typedef int             Int32;
typedef float         Float32;

/****************************************************
 *
 * Function:  BigEndian()
 *
 * Returns:   bool
 * Purpose:   Determines if a system is big-endian.
 *
 ***************************************************/
bool BigEndian( void )
{
  unsigned char SwapTest[2] = { 1, 0 };

  if( *(short *) SwapTest == 1 ) {
    //little endian
    return false;
  } else {
    //big endian
    return true;
  }
}

/****************************************************
 *
 * Function:  Int32Swap()
 *
 * Returns:   Int32
 * Purpose:   Reverses the bytes in a 32-bit integer.
 *
 ***************************************************/
Int32 Int32Swap (int i)
{
  unsigned char b1, b2, b3, b4;

  b1 = i & 255;
  b2 = ( i >> 8 ) & 255;
  b3 = ( i>>16 ) & 255;
  b4 = ( i>>24 ) & 255;

  return ((int)b1 << 24) + ((int)b2 << 16) + ((int)b3 << 8) + b4;
}

/****************************************************
 *
 * Function:  IntI6Swap()
 *
 * Returns:   short
 * Purpose:   Reverses the bytes in a 16-bit integer.
 *
 ***************************************************/
short Int16Swap( short s )
{
  unsigned char b1, b2;
  
  b1 = s & 255;
  b2 = (s >> 8) & 255;

  return (b1 << 8) + b2;
}

/****************************************************
 *
 * Function:  main(int argc, char* argv[])
 *
 * Returns:   int
 *            0 if success
 *            non-zero if failure
 *
 * Purpose:   MAIN
 *
 ***************************************************/
int
main (int argc, char *argv[])
{

	// declare an hdf5 file to store the tbb data
	dalDataset * ds;
	ds = new dalDataset( FILENAME, "HDF5" );

	// If less than two arguments provided, print usage.
	if ( 2 != argc )
	{
		cout << "Usage:  tbb_reader <filename>" << endl << endl;
		exit(1);
	}

	// define some constants describing packet header sizes
	const Int32 ETHEREAL_HEADER_LENGTH = 46;
	const Int32 FIRST_EXTRA_HDR_LENGTH = 40;
	const Int32 EXTRA_HDR_LENGTH = 16;

	//
	// Define something to hold the current buffer size.
	// This will change before each read into memblock.
	//
	ifstream::pos_type size=0;
	
	// define a memory buffer
	unsigned char * memblock=NULL;

	// define a structure describing the TBB header
	typedef struct TBB_Header {
		unsigned char stationid;
		unsigned char rspid;
		unsigned char rcuid;
		unsigned char sample_freq;
		UInt32 seqnr;
		Int32 time;
		UInt32 sample_nr;
		UInt16 n_samples_per_frame;
		UInt16 n_freq_bands;
		char bandsel[64];
		Int16 spare;
		UInt16 crc;
	};
	
	// define a structure describing samples in
	//   "transient mode"
	typedef struct TransientSample {
		Int16 value;
	};

	// define a structure describing samples in
	//   "spectral mode"
	typedef struct SpectralSample {
		complex<Int16> value;
	};

	UInt32 payload_crc;
	
	// declare handle for the input file
	fstream file (argv[1], ios::binary|ios::in);

	// flag for the first sample in a packet
	bool first_sample = true;

	// endianness flag
	bool bigendian = BigEndian();
	
	// begin reading the file
	if (file.is_open())
	{
		// declare a few structure variables
		TransientSample tran_sample;
		SpectralSample 	spec_sample;
		TBB_Header	header;
		
		size = ETHEREAL_HEADER_LENGTH;

		// set the size of the memory block
		memblock = new unsigned char [size];

		// make sure to start at the beginning of the file
		file.seekg (0, ios::beg);

		//if (DEBUG)
		//  cout << "Size of header is " << sizeof(header) << " bytes." << endl;

		int counter=0;  // initialize a loop counter

		// loop through the file
		while ( !file.eof() )
		{
			counter++;  // increase the loop counter

			// skip 46-byte ethereal header (temporary)
			//   we shouldn't need to do this with the next
			//   tbb data revision
			size = ETHEREAL_HEADER_LENGTH;

			//if (DEBUG)
			//  cout << "*** Reading " << size << " byte ETHEREAL header." << endl;

			// read the first block of memory, in this case skipping the
			//   ethereal header
			file.read((char*)memblock, size);

			//
			// skip "extra" first 40-byte header (temporary)
			//   we shouldn't need to do this with the next
			//   tbb data revision
			//
			if ( first_sample )
			{
				size = FIRST_EXTRA_HDR_LENGTH;

				//if (DEBUG) cout << "*** Reading " << size << " bytes" << endl;

				file.read((char*)memblock, size);
				first_sample = false;
			}
			else
			{
				size = EXTRA_HDR_LENGTH;

				//cout << "*** Reading " << size << " bytes" << endl;

				file.read((char*)memblock, size);
			}
			
			//
			// read 88-byte TBB frame header
			//
			file.read(reinterpret_cast<char *>(&header), sizeof(header));

			// reverse fields if big endian
			if ( bigendian )
			{
				header.seqnr = Int32Swap( header.seqnr );
				header.sample_nr = Int32Swap( header.sample_nr );
				header.n_samples_per_frame = 
					Int16Swap( header.n_samples_per_frame);
				header.n_freq_bands = Int16Swap( header.n_freq_bands );
			}
			
			//
			// these print statements should become hdf5 writes
			//
			printf("Station Id:   \t%u\n",header.stationid);
			printf("RSP Id:       \t%u\n",header.rspid);
			printf("RCU Id:       \t%u\n",header.rcuid);
			printf("Sample Freq:  \t%u MHz\n",header.sample_freq);
			printf("Seq Number:   \t%u\n",header.seqnr);
			
			/* compute time
			tm *time=localtime( reinterpret_cast<time_t*>(&header.time) );
			printf("Date:          \t%d/%02d/%02d\n", time->tm_year+1900,
					time->tm_mon+1, time->tm_mday);
			printf("Time:          \t%02d:%02d\n", time->tm_hour, time->tm_min);*/
			printf("Time (s1970):  \t%u\n",header.time );

			printf("Sample Number: \t%u\n",header.sample_nr );			
			printf("No. of Samps:  \t%u\n",header.n_samples_per_frame );
			printf("No. of Freqs.: \t%u\n",header.n_freq_bands );
			printf("CRC:           \t%u\n",header.crc );
			
			// Read Payload
			printf("Samples (%d):\n",counter);

			//
			// If there are 0 frequency bands, then we're in transient mode.
			//   Otherwise, we're in spectral mode.
			//
			if ( 0==header.n_freq_bands )
			{
				//
				// for each sample:
				//   - read into the buffer
				//   - reverse if necessary
				//   - print to screen (or write to hdf5)
				//
				for (unsigned int ii=0; ii < header.n_samples_per_frame; ii++)
				{
					file.read( reinterpret_cast<char *>(&tran_sample),
						   sizeof(tran_sample) );

					// reverse fields if big endian
					if ( bigendian )
						tran_sample.value = Int16Swap( tran_sample.value );

					printf( "%hd,", tran_sample.value );
				}
			}
			else
			{
				// define real and imaginary components of spectral sample
				Int16 real_part;
				Int16 imag_part;
				//
				// for each sample:
				//   - read into the buffer
				//   - reverse if necessary
				//   - print to screen (or write to hdf5)
				//
				for (unsigned int ii=0; ii < header.n_samples_per_frame; ii++)
				{
					file.read( reinterpret_cast<char *>(&spec_sample),
						   sizeof(spec_sample) );

					// reverse fields if big endian
					if ( bigendian )
					{
						real_part = Int16Swap( real(spec_sample.value) );
						imag_part = Int16Swap( imag(spec_sample.value) );
					}
					else
					{
						real_part = real(spec_sample.value);
						imag_part = imag(spec_sample.value);
					}

					printf( "(%hd,%hd),", real_part, imag_part );
				}
			}
			
			// read the "payload crc" field			
			file.read( reinterpret_cast<char *>(&payload_crc),
					   sizeof(payload_crc) );
			printf("\b\n");

			// print to screen (or write to hdf5)
			printf("Payload CRC: %u\n\n",payload_crc);

		}
		
		file.close();	// close the tbb input file
		ds->close();	// close the hdf5 output dataset

		// cleanup the memory block
		delete[] memblock;
	}
	else
	{
	   // print an error if unable to open the file
	   cout << "Unable to open file" << argv[1] << endl;
	   return -1;
	}
	return 0;
}
