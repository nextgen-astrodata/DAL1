/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters & Alwin de Jong                  *
 *   J.S.Masters@uva.nl                   																 *
 *   jong@astron.nl                                                        *
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

#ifndef BFRAW_H
#include "BFRaw.h"
#endif

#include <signal.h>


/*
dataStruct * channelize( dataStruct * data,
                         int start,
                         const unsigned long arraylength,
                         int factor )
{
  const int ds_size = arraylength / factor;
  dataStruct * ds_data = NULL;
  try
  {
    ds_data = new dataStruct[ ds_size ];
  }
  catch (bad_alloc)
  {
    cerr << "Can't allocate memory for downsampled array." << endl;
  }

  for (int count=0; count<ds_size; count++)
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

*/

namespace DAL {
  
  bool time_out; //! used for time out handler
  int server_socket(0);
  
  //_________________________________________________timeout_alarm
  //! used to handle time out for socket connection
  void timeout_alarm (int sig)
  {
    time_out = true;
    std::cout << " time out on socket read" << std::endl;
    // shutdown and close socket connection
    shutdown(server_socket, SHUT_RDWR);
    close(server_socket);
    std::cout << "connection closed" << std::endl;
    signal (sig, timeout_alarm);
  }
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  /*!
    This is the only constructor provided for an object of this class. The 
    optional processing parameters, which can can be passed along, are set to
    sensible default values, such that only the \e filename is required.

    \param filename         -- Name of the input data file.
    \param doIntensity      -- Compute total intensity
    \param doDownsample     -- Compute downsampled version of the original data
    \param doChannelization -- Compute channelization of the original data
    \param factor           -- Downsampling factor
  */
		BFRaw::BFRaw( string const& filename,
									bool doIntensity,
									bool doDownsample,
									bool doChannelization,
		int factor ) :
				rawfile(0),
		outputfilename(filename),
//server_socket(0),
		socklen(sizeof(incoming_addr)),
		blockHeaderSize(sizeof(blockheader)),
		dataSize(0),
		sampledata(0),
		block_nr(0),
		index(0),
		first_block(true),
		downsample_factor(factor),
		doDownsample_p(doDownsample),
		DO_FLOAT32_INTENSITY(doIntensity),
		DO_CHANNELIZATION(doChannelization),
		ds_data(0),
		totalintensity(0),
		nrOfBlocks(0)
		{
    	// initializations (private)
			bigendian = BigEndian();	
  	}
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  /*!
    \brief Destructor
  */
  BFRaw::~BFRaw()
  {
    if (server_socket)
      close(server_socket);
    if (rawfile)
      {
        if (rawfile->is_open())
          {
            rawfile->close();
					}
        delete rawfile;
        rawfile = NULL;
      }
	if (sampledata){		
		delete [] sampledata;
		sampledata = NULL;
	}
	if (ds_data) {
		delete [] ds_data;
		ds_data = NULL;
	}
	if (totalintensity) {
		delete [] totalintensity;
		totalintensity = NULL;
  }
	}
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  /*!
    \param os -- Output stream to which the summary is going to be written
  */
  void BFRaw::summary (std::ostream &os)
  {
    os << "[BFRaw] Summary of object properties." << endl;

    os << "-- Name of output file  = " << outputfilename    << std::endl;
    os << "-- Downsampling of data = " << doDownsample_p    << std::endl;
    os << "-- Downsample factor    = " << downsample_factor << std::endl;
  }

  // ------------------------------------------------------------------------ eof

  /*!
    \return eof -- Returns \e true if the end of file has been reached
  */
  bool BFRaw::eof()
  {
    if ( rawfile->peek() == EOF )
      return true;
    else
      return false;
  }

  // ------------------------------------------------------------------ RArad2deg

  /*!
    \brief Convert RA position (in radian) to degrees

    \param rad -- The RA position in radian

    \return deg -- The RA position in degrees
  */
  char * BFRaw::RArad2deg( const float &rad )
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

  // ----------------------------------------------------------------- DECrad2deg

  /*!
    \brief Convert Dec position (in radian) to degrees

    \param rad -- The Dec position in radian

    \return deg -- The Dec position in degrees
  */
  char * BFRaw::DECrad2deg( const float &rad )
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

  //________________________________________________________________connectsocket

  /*!
    \brief portnumber -- Portnumber to which to connect
  */
  bool BFRaw::connectsocket( const char *portnumber )
  {
		off_online = std::string("online");
		file_byte_size = 0;
    int port_number = atol( portnumber );

    // Step 1 Create a socket using TCP
    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket<0)
      {
        perror("socket creation");
        return false;
      }

    // Step 2 Create a sockaddr_in to describe the local port
    //sockaddr_in local_info;
		memset(&incoming_addr, 0, sizeof(incoming_addr));
    incoming_addr.sin_family = AF_INET;
    incoming_addr.sin_addr.s_addr = INADDR_ANY;
    incoming_addr.sin_port = htons(port_number);

    // Step 3 Bind the socket to the port
    if (-1 == bind(server_socket, (sockaddr *) &incoming_addr, sizeof(incoming_addr)))
      {
        perror("error bind failed");
				close(server_socket);
        return false;
      }
		
		// Step 4 listen for incoming connections
    if (-1 == listen(server_socket,5))
      {
        perror("error listen failed");
				close(server_socket);
        return false;
      }
		int old_server_socket = server_socket;
		if (-1 == (server_socket = accept(server_socket, 0, 0))) {
				perror("error accept failed");
				close(server_socket);
				return false;
		}
#ifdef DEBUGGING_MESSAGES
		cout << "socket connected" << endl;
#endif
		close(old_server_socket);

		return true;
  }

  // ---------------------------------------------------------------- openRawFile

  void BFRaw::openRawFile( const char* filename )
  {
    off_online = std::string("offline");
    delete rawfile;
    rawfile = new fstream( filename, ios::binary|ios::in );
    rawfile->seekg(0, ios::end); // move to end of file to determine its file size
    file_byte_size = static_cast<unsigned long int>(rawfile->tellg())-2; // see how many bytes in file
    //std::cout << "filesize in bytes: " << file_byte_size << std::endl;
    rawfile->seekg(0, ios::beg);  // move to start of file
  }
  
  // ---------------------------------------------------------- readRawFileHeader

  void BFRaw::readRawFileHeader()
  {
    if ( !rawfile->read ( reinterpret_cast<char *>(&header),
                          sizeof(header) ))
      {
        cout << "ERROR: problem with read (header)." << endl;
        cout << "read pointer position: " << rawfile->tellg() << endl;
      }

#ifdef DEBUGGING_MESSAGES
    printf("size of file header: %lu\n", sizeof(header));
    cout << "read pointer position: " << rawfile->tellg() << endl;
#endif

    // swap values when necessary
    if ( !bigendian )
      {
        // change byte order for all frequencies.
        for (int ii = 0; ii < maxNrSubbands; ii++)
          {
            swapbytes((char *)&header.subbandFrequencies[ii],8);
            swapbytes((char *)&header.subbandToBeamMapping,2);
          }

        for (int ii=0;ii<8;ii++)
          {
            for (int jj=0;jj<2;jj++)
              {
                swapbytes((char *)&header.beamDirections[ii][jj],8);
              }
          }

        // change byte order for subbands.
        swapbytes((char *)&header.nrSubbands,2);

        // change byte order for nrSamplesPerSubband
        swapbytes((char *)&header.nrSamplesPerSubband,4);

        // change byte order for sampleRate
        swapbytes((char *)&header.sampleRate,8);

        // change byte order for Magic Number
        swapbytes((char *)&header.magic,4);

      }
			// calculate the size of a one second datablock
			oneSecondBlockSize =
					sizeof(blockheader)
					+
					(	header.nrSamplesPerSubband * header.nrSubbands * sizeof(Sample) );
			
			nrOfBlocks = (file_byte_size - sizeof(header)) / oneSecondBlockSize;
			std::cout << "Number of one second blocks in file: " << nrOfBlocks << std::endl;

			// allocating memory for possible downsampling and intensity calculations
			ds_size = header.nrSamplesPerSubband / downsample_factor;
			if ( doDownsample_p ) { // if downsampling
				try {
#ifdef DEBUGGING_MESSAGES
				std::cout << "Allocating " << ds_size * sizeof(Float32) << " bytes for downsampled data..." << std::endl;
#endif
					ds_data = new Float32[ ds_size ];
				}
				catch (bad_alloc) {
					cerr << "Can't allocate memory for downsampled array." << endl;
				}
				memset(ds_data, 0, ds_size * sizeof(Float32) );
			}
			else {
				if ( DO_FLOAT32_INTENSITY ) {
					try
					{
						totalintensity = new Float32[ header.nrSamplesPerSubband ];
					}
					catch (bad_alloc)
					{
						cerr << "Can't allocate memory for total intensity array." << endl;
					}
					memset(totalintensity, 0, header.nrSamplesPerSubband * sizeof(Float32) );
				}
			}
			
#ifdef DEBUGGING_MESSAGES
    printf("Magic number: %8X\n",header.magic);
    printf("bits per sample: %u\n",header.bitsPerSample);
    printf("Polarizations : %u\n",header.nrPolarizations);
    printf("Subbands : %u\n",header.nrSubbands);
    printf("Samples per subband: %u\n", header.nrSamplesPerSubband);
    printf("Station ID: %s\n", header.station);
    printf("Sample rate: %g\n", header.sampleRate);
    printf("Centre Freq. of subbands (MHz): \n");
    for (int ii=0;ii<header.nrSubbands;ii++)
      {
        printf("%9.6f ",header.subbandFrequencies[ii]/1000000.0);
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
            printf("%f   ",header.beamDirections[ii][jj]);
            if ( 0 == jj )
              ra = RArad2deg( header.beamDirections[ii][jj] );
            else
              dec = DECrad2deg( header.beamDirections[ii][jj] );
          }
        printf("[  %s, %s ]", ra, dec );
        printf("\n");
        delete [] ra;
        ra = NULL;
        delete [] dec;
        dec = NULL;
      }
#endif
  }

  // ---------------------------------------------------------- readRawSocketHeader

  bool BFRaw::readRawSocketHeader()
  {
		if (recvfrom(server_socket, reinterpret_cast<char *>(&header), sizeof(header),0,  (sockaddr *) &incoming_addr, &socklen) < static_cast<int>(sizeof(header))) {
			cerr << "error reading header from socket" << endl;
			close(server_socket);
			perror("readRawSocketHeader");
      return false;
		}

    // swap values when necessary
    if ( !bigendian )
      {
        // change byte order for all frequencies.
        for (int ii=0;ii<maxNrSubbands;ii++)
          {
            swapbytes((char *)&header.subbandFrequencies[ii],8);
            swapbytes((char *)&header.subbandToBeamMapping,2);
          }

        for (int ii=0;ii<8;ii++)
          {
            for (int jj=0;jj<2;jj++)
              {
                swapbytes((char *)&header.beamDirections[ii][jj],8);
              }
          }

        // change byte order for beamlets.
        swapbytes((char *)&header.nrSubbands,2);
        // change byte order for nrSamplesPerSubband
        swapbytes((char *)&header.nrSamplesPerSubband,4);
        // change byte order for sampleRate
        swapbytes((char *)&header.sampleRate,8);
        // change byte order for Magic Number
        swapbytes((char *)&header.magic,4);

      }
	
	dataSize = header.nrSamplesPerSubband*header.nrSubbands;

#ifdef DEBUGGING_MESSAGES
			cout << "Allocating " << sizeof(Sample) * dataSize << " bytes for input databuffer" << endl;
#endif
		sampledata = new Sample[ dataSize ]; // sample contains 8 bytes
			
			// allocating memory for possible down sampling and intensity calculations
//			ds_data = NULL;
//			totalintensity = NULL;
			ds_size = header.nrSamplesPerSubband / downsample_factor; // the number of samples in the output downsampled data buffer
			if ( doDownsample_p ) { // if downsampling
#ifdef DEBUGGING_MESSAGES
			std::cout << "Allocating " << ds_size * sizeof(Float32) << " bytes for output downsampled databuffer" << std::endl;
#endif
				try {
					ds_data = new Float32[ ds_size ];
				}
				catch (bad_alloc) {
					cerr << "Can't allocate memory for downsampled databuffer." << endl;
				}
				memset(ds_data, 0, ds_size * sizeof(Float32) );
			}
			else {
				if ( DO_FLOAT32_INTENSITY ) {
#ifdef DEBUGGING_MESSAGES
			std::cout << "Allocating " << header.nrSamplesPerSubband * sizeof(Float32) << " bytes for output intensity databuffer" << std::endl;
#endif
					try
					{
						totalintensity = new Float32[ header.nrSamplesPerSubband ];
					}
					catch (bad_alloc)
					{
						cerr << "Can't allocate memory for total intensity array." << endl;
					}
					memset(totalintensity, 0, header.nrSamplesPerSubband * sizeof(Float32) );
				}
			}


#ifdef DEBUGGING_MESSAGES
    printf("Magic number: %8X\n",header.magic);
    printf("bits per sample: %u\n",header.bitsPerSample);
    printf("Polarizations : %u\n",header.nrPolarizations);
    printf("Subbands : %u\n",header.nrSubbands);
    printf("Samples per subband: %u\n", header.nrSamplesPerSubband);
    printf("Station ID: %s\n", header.station);
    printf("Sample rate: %g\n", header.sampleRate);
    printf("Centre Freq. of subbands (MHz): \n");
    for (int ii=0;ii<header.nrSubbands;ii++)
      {
        printf("%9.6f ",header.subbandFrequencies[ii]/1000000.0);
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
            printf("%f   ",header.beamDirections[ii][jj]);
            if ( 0 == jj )
              ra = RArad2deg( header.beamDirections[ii][jj] );
            else
              dec = DECrad2deg( header.beamDirections[ii][jj] );
          }
        printf("[  %s, %s ]", ra, dec );
        printf("\n");
        delete [] ra;
        ra = NULL;
        delete [] dec;
        dec = NULL;
      }
#endif
	return true;
  }


int BFRaw::receiveBytes(void *storage, size_t nrOfBytesToRead) {
	int bytes_read = 0;
	int8_t *bytepointer = reinterpret_cast<int8_t *>(storage);
	while (true) {
	bytes_read = recvfrom(server_socket, bytepointer, nrOfBytesToRead, 0, (sockaddr *) &incoming_addr, &socklen);
	if (bytes_read == -1) { // error reading
		shutdown(server_socket, SHUT_RDWR);
		close(server_socket);
		return -1;
	}
	else if (bytes_read == 0) { // end of stream?
		 return 0;
	}
	nrOfBytesToRead -= bytes_read;
	bytepointer += bytes_read;
	if (nrOfBytesToRead == 0) { // did we read enough?
		return bytes_read;
	}
	}
}

  //_________________________________________________processBFRawDataBlockFromSocket
	bool BFRaw::processBFRawDataBlockFromSocket(void) {

if (receiveBytes(reinterpret_cast<char *>(&blockheader), blockHeaderSize) == -1) {
			perror("Error, receiving the first data block header");
      throw "Error, receiving the first data block header";
	}

	if (!bigendian) { convertEndian(&blockheader); }

	makeH5OutputFile();

	// write the utc time to hdf5 file according to header time info
	time_t utc;
	utc = (time_t)(blockheader.time[0]/(Int64)header.sampleRate);
	char * timeDateString = NULL;
	uint16_t buf_size = 128;
	if (!timeDateString) {
		timeDateString = new char[buf_size*sizeof(char)];
	}
	memset (timeDateString,'\0',buf_size);
	strftime(timeDateString, buf_size, "%T", gmtime(&utc));
	dataset.setAttribute( "EPOCH_UTC", timeDateString );

	memset (timeDateString,'\0',buf_size);
	strftime(timeDateString, buf_size, "%d/%m/%y", gmtime(&utc));
	dataset.setAttribute( "EPOCH_DATE", timeDateString );

	memset (timeDateString,'\0',buf_size);


	delete[] timeDateString;
	first_block = false;

// read the first datablock in the temporary storage
	if ( receiveBytes( sampledata, dataSize * sizeof(Sample)) == -1) { // read all subbands for this datablock into sampledata buffer
			perror("Error, reading the first data block!");
			throw "Error reading the first data block!";
		}
	
		writeBFRawDataBlockToFile();
	 ++block_nr;

	// read rest of datablocks and write to file
	int result;
	
	// enable time out alarm
	// set a time out alarm
	time_out = false;
	signal( SIGALRM, timeout_alarm );
	alarm( 7 );
	
	while (!time_out) { // process all next data blocks
cout << "block_nr: " << block_nr << endl;

	result = receiveBytes(reinterpret_cast<char *>(&blockheader), blockHeaderSize);
	
	if (!time_out) {
		if (result == 0) {
			std::cout << "shutdown connection by remote" << std::endl;
			break;  // read end of stream
		}
		else if (result < -1) {
			std::cerr << "Error, read of data block: " << block_nr << " !" << std::endl;
			perror("processBFRawDataBlockFromSocket");
			throw "Error read data block!";
		}
		result = receiveBytes(sampledata, dataSize * sizeof(Sample));
		
		if (!time_out) {
			if (result == -1) {
				std::cerr << "Error, read of data block: " << block_nr << " !" << std::endl;
				perror("processBFRawDataBlockFromSocket");
				throw "Error read data block!";
			} else {
				alarm(7); // reset time out alarm
			}
		}
	}
//	std::cout << "sampledata[0]= " << sampledata[0].xx << ", " << sampledata[0].yy << std::endl;
	if (!bigendian) { convertEndian(&blockheader); }
//	std::cout << "processing block " << block_nr << std::endl;
	writeBFRawDataBlockToFile();
		++block_nr;
} // while
alarm (0); // cancel time out alarm


// shutdown and close socket connection
if (!time_out) { // if server socket was not yet closed in time out event handler
	shutdown(server_socket, SHUT_RDWR);
	close(server_socket);
	std::cout << "connection closed" << std::endl;
}


return true;
}

 // ---------------------------------------------------------writeBFRawDataBlock
void BFRaw::writeBFRawDataBlockToFile(void) {
#ifdef _OPENMP
#pragma omp parallel for ordered schedule(dynamic)
#endif
// Float32 * ds_data = NULL;
// Float32 * totalintensity = NULL;
// const int ds_size = header.nrSamplesPerSubband / downsample_factor;
// if ( doDownsample_p ) { // if downsampling
// 	try {
// 		ds_data = new Float32[ ds_size ];
// 	}
// 	catch (bad_alloc) {
// 		cerr << "Can't allocate memory for downsampled array." << endl;
// 	}
// 	memset(ds_data, 0, ds_size * sizeof(Float32) );
// }
// else {
// 	if ( DO_FLOAT32_INTENSITY ) {
// 		try
// 		{
// 			totalintensity = new Float32[ header.nrSamplesPerSubband ];
// 		}
// 		catch (bad_alloc)
// 		{
// 			cerr << "Can't allocate memory for total intensity array." << endl;
// 		}
// 		memset(totalintensity, 0, header.nrSamplesPerSubband * sizeof(Float32) );
// 	}
// }
//uint64_t subbandFirstSampleIdx;
//Sample * sample = new Sample[ fileheader.nrSamplesPerBeamlet ];
//Sample * singleSubbandDataBuf = new Sample[ header.nrSamplesPerSubband ];

for ( uint subband=0; subband < header.nrSubbands; ++subband )
{
	//index = sizeof(blockheader) +	subband * header.nrSamplesPerSubband * 4 * header.bitsPerSample/8;
//	subbandFirstSampleIdx = subband * header.nrSamplesPerSubband;// * 4 * header.bitsPerSample/8; // index is the byte index to the first sample of a subband
//	std::cout << "subbandIndex = " << subbandFirstSampleIdx << std::endl;
/*
#ifdef DEBUGGING_MESSAGES
	cout << "sampledata[0].xx: " << sampledata[0].xx << ", sampledata[0].yy: " << sampledata[0].yy <<  "sampledata[10].xx: " << sampledata[10].xx << ", sampledata[10].yy: " << sampledata[10].yy << endl;
#endif
*/
	
	/*
	index = blk *
			(
			(fileheader.nrSamplesPerBeamlet * 8) *
			fileheader.nrBeamlets
			)
			+ (blk+1) * sizeof(blockheader) +
			subband * fileheader.nrSamplesPerBeamlet * 8;

	sample = reinterpret_cast<Sample*>(&( buf[ index ]));
	*/
	//sample = reinterpret_cast<Sample*>(&( buf[ index ]));
	// should subbandIndex be a SAMPLE index instead of a BYTE index?
//	singleSubbandDataBuf = reinterpret_cast<Sample*>(&( sampledata[ subbandFirstSampleIdx ])); // sampledata buffer holds the samples of all the subbands of 1 datablock
//	singleSubbandDataBuf = sampledata[ subbandFirstSampleIdx ];
	
//	std::cout << "first sample: xx=" << sampledata[subbandFirstSampleIdx].xx << ", yy = " << sampledata[subbandFirstSampleIdx].yy << std::endl;
	
	if ( doDownsample_p )  // if downsampling
	{
//		downsampleSingleSubband( sampledata, 0, downsample_factor, ds_data, ds_size );
//		std::cout << "downsampling subband " << subband << std::endl;
//		std::cout << "first sample: xx=" << sampledata[subband * header.nrSamplesPerSubband].xx << ", yy = " << sampledata[subband * header.nrSamplesPerSubband].yy << "adres: " << &(sampledata[ subband * header.nrSamplesPerSubband ]) << std::endl;
		downsampleSingleSubband( &(sampledata[ subband * header.nrSamplesPerSubband]), downsample_factor, ds_data, ds_size );
#ifdef _OPENMP
#pragma omp ordered
#endif
		table[subband]->appendRows( ds_data, ds_size ); // writes header.nrSamplesPerSubband / downsample_factor  samples
	// initialize the downdample data buffer again	
	memset(ds_data, 0, ds_size * sizeof(Float32) );		
/*		delete [] ds_data;
		ds_data = NULL;*/
	}
	else  // no downsampling
	{
		if ( DO_FLOAT32_INTENSITY )
		{
			compute_float32_intensity( &(sampledata[subband * header.nrSamplesPerSubband]), 0, header.nrSamplesPerSubband, totalintensity );
#ifdef _OPENMP
#pragma omp ordered
#endif
										table[subband]->appendRows( totalintensity, header.nrSamplesPerSubband );
					//					delete [] totalintensity;
					//					totalintensity = NULL;
		}
		else
		{
			table[subband]->appendRows( &(sampledata[subband * header.nrSamplesPerSubband]), header.nrSamplesPerSubband );
		}
	}

} // for subband
//delete singleSubbandDataBuf;
}

  // --------------------------------------------------------------convertEndian
	void BFRaw::convertEndian(BlockHeader* blockheader)	{
		swapbytes((char *)&blockheader->magic,4);
		for ( uint ii = 0; ii < 8; ii++ )
			{
				swapbytes((char *)&blockheader->coarseDelayApplied[ ii ],4);
				swapbytes((char *)&blockheader->fineDelayRemainingAtBegin[ ii ],8);
				swapbytes((char *)&blockheader->fineDelayRemainingAfterEnd[ ii ],8);
				swapbytes((char *)&blockheader->time[ ii ],8);
				swapbytes((char *)&blockheader->flags[ ii ].nrFlagsRanges,4);
//				swapbytes((char *)&blockheader.nrFlagsRanges[ ii ],4);
				for ( uint jj = 0; jj < 16; jj++ )
					{
						swapbytes( (char *)&blockheader->flags[ ii ].flagsRanges[ jj ].begin,4 );
						swapbytes( (char *)&blockheader->flags[ ii ].flagsRanges[ jj ].end,4 );
/*						swapbytes( (char *)&blockheader.flagsRanges[ ii ][ jj ].begin,4 );
						swapbytes( (char *)&blockheader.flagsRanges[ ii ][ jj ].end,4 ); */
					}
			}
	}


// ----------------------------------------------------------- makeH5OutputFile

  void BFRaw::makeH5OutputFile(void)
  {
    dataset = dalDataset( outputfilename.c_str(), "HDF5" );
    
    // root-level headers
    int n_stations = 1;
    vector<string> srcvec;
    srcvec.push_back( "" );
    int main_beam_diam      = 0;
    int bandwidth           = 0; // Total bandwidth (MHz)
    int breaks_in_data      = 0; // Any breaks in data?
    int dispersion_measure  = 0;
//		int number_of_samples_per_block = header.nrSubbands * header.nrSamplesPerSubband;
    int32_t total_number_of_samples;
    if (!doDownsample_p) {
      total_number_of_samples = nrOfBlocks * header.nrSamplesPerSubband;
    }
#ifdef DEBUGGING_MESSAGES
    std::cout << "total_number_of_samples = " << total_number_of_samples << std::endl;
#endif
    //		double sampling_time   = header.sampleRate;
    int number_of_beams     = 1;
    int sub_beam_diameter   = 0; // fwhm of the sub-beams (arcmin)
    int weather_temperature = 0; // approx. centigrade
    int weather_humidity    = 0; // approx. %
    int tsys                = 0; // for various stations (K)
    std::string station_clock;
    if (header.nrSamplesPerSubband == 155648) {
      station_clock = std::string("160MHz");
    }
    else if (header.nrSamplesPerSubband == 196608) {
      station_clock = std::string("200MHz");
    }
    else {
      station_clock = std::string("unknown");
    }
    
    // write headers using above
    dataset.setAttribute( "GROUPTYPE", std::string("Root") );
    dataset.setAttribute( "FILENAME", outputfilename.c_str() );
    dataset.setAttribute( "FILETYPE", std::string("bfstation") );
    dataset.setAttribute( "INPUT_FILESIZE", &file_byte_size);
    dataset.setAttribute( "DOWNSAMPLE_RATE", &downsample_factor);
    dataset.setAttribute( "OFF-/ONLINE_CREATION", off_online );
    dataset.setAttribute( "TELESCOPE", std::string("LOFAR") );
    dataset.setAttribute( "OBSERVER", std::string("") );
    dataset.setAttribute( "PROJECT_ID", std::string("") );
    dataset.setAttribute( "PROJECT_NAME", std::string("LOFAR") );
    dataset.setAttribute( "PROJECT_DESCRIPTION", std::string("") );
    dataset.setAttribute( "OBSERVATION_ID", std::string("") );
    dataset.setAttribute( "OBSERVATION_MODE", std::string("") );
    dataset.setAttribute_string( "TARGET", srcvec );
    dataset.setAttribute( "TIME_SYSTEM", std::string("") );
    dataset.setAttribute( "SYSTEM_VERSION", std::string("") );
    dataset.setAttribute( "PIPELINE_NAME", std::string("") );
    dataset.setAttribute( "NUMBER_OF_STATIONS", &n_stations );
    dataset.setAttribute( "STATION_LIST", std::string(header.station) );
    dataset.setAttribute( "STATION_CLOCK", station_clock );
    dataset.setAttribute( "NOTES", std::string("") );
    dataset.setAttribute( "DATATYPE", std::string("") );
    dataset.setAttribute( "EMBAND", std::string("") );
    //		dataset.setAttribute_string( "SOURCE", srcvec ); // replaced by TARGET
    dataset.setAttribute( "POINT_RA", std::string("") );
    dataset.setAttribute( "POINT_DEC", std::string("") );
    dataset.setAttribute( "MAIN_BEAM_DIAM", &main_beam_diam );
    dataset.setAttribute( "BANDWIDTH", &bandwidth );
    dataset.setAttribute( "BREAKS_IN_DATA", &breaks_in_data );
    dataset.setAttribute( "DISPERSION_MEASURE", &dispersion_measure );
    dataset.setAttribute( "SECONDS_OF_DATA", &nrOfBlocks );
    dataset.setAttribute( "SAMPLE_RATE", &header.sampleRate );
    dataset.setAttribute( "NOF_SAMPLES_PER_SUBBAND", &header.nrSamplesPerSubband );
    dataset.setAttribute( "TOTAL_NUMBER_OF_SAMPLES", &total_number_of_samples );
    dataset.setAttribute( "NUMBER_OF_BEAMS", &number_of_beams );
    dataset.setAttribute( "SUB_BEAM_DIAMETER", &sub_beam_diameter );
    dataset.setAttribute( "WEATHER_TEMPERATURE", &weather_temperature );
    dataset.setAttribute( "WEATHER_HUMIDITY", &weather_humidity );
    dataset.setAttribute( "TSYS", &tsys );
    
    dalGroup * beamGroup;
    
    char * beamstr = new char[10];
    
    int beam_number;
    
    beam_number = 0;
    sprintf( beamstr, "beam%03d", beam_number );
    beamGroup = dataset.createGroup( beamstr );
    
    float ra_val  = header.beamDirections[beam_number+1][0];
    float dec_val = header.beamDirections[beam_number+1][1];
    beamGroup->setAttribute( "RA", &ra_val, 1 );
    beamGroup->setAttribute( "DEC", &dec_val, 1 );
    
    int n_subbands[] = { header.nrSubbands };
    beamGroup->setAttribute( "NUMBER_OF_SUBBANDS", n_subbands );
    
    delete beamGroup;
    
#ifdef DEBUGGING_MESSAGES
    std::cerr << "CREATED New beam group: " << std::string(beamstr) << std::endl;
    std::cerr << "   " << header.nrSubbands << " subbands" << std::endl;
#endif
    
    table = new dalTable * [ header.nrSubbands ];
    char * sbName = new char[8];
    int * center_frequency = new int[header.nrSubbands];
    
    // nrSubbands is actually the number of subbands (see email from J.Romein)
    for (unsigned int idx=0; idx<header.nrSubbands; idx++)
      {
        sprintf( sbName, "SB%03d", idx );
        table[idx] = dataset.createTable( sbName, beamstr );
      }
    
    for (unsigned int idx=0; idx<header.nrSubbands; idx++)
      {
        if ( doDownsample_p || DO_FLOAT32_INTENSITY )
          {
            table[idx]->addColumn( "TOTAL_INTENSITY", dal_FLOAT );
          }
        else
          {
            table[idx]->addColumn( "X", dal_COMPLEX_SHORT );
            table[idx]->addColumn( "Y", dal_COMPLEX_SHORT );
          }
      }
    for (unsigned int idx=0; idx<header.nrSubbands; idx++)
      {
        center_frequency[idx] = (int)header.subbandFrequencies[ idx ];
        table[idx]->setAttribute( "CENTER_FREQUENCY", &center_frequency[idx] );
      }
    
    delete [] sbName;
    sbName = NULL;
    delete [] center_frequency;
    center_frequency = NULL;
    delete [] beamstr;
    beamstr = NULL;
    
  } // BFRaw::makeH5OutputFile
  
  
  // -------------------------------------------------------------- processBlocks
  
  void BFRaw::processBlocks()
  {
#ifdef DEBUGGING_MESSAGES
    std::cout << "Starting to process blocks" << std::endl;
#endif
    // int16_t nseconds = 	20;
    bool ret = true;
    do
      {
	ret = processBlocks(20); // process blocks in 20 second chunks
      }
    while ( true == ret );
  }
  
  // -------------------------------------------------------------- processBlocks
  
  bool BFRaw::processBlocks( int16_t blocks )
  {
    
    bool retval = true;
    
    int64_t blocksize = oneSecondBlockSize * blocks;
    
    char * buf = NULL;
    try
      {
#ifdef DEBUGGING_MESSAGES
	std::cout << "Allocating " << blocksize << " bytes (" << blocks 
		  << " seconds)" << std::endl;
#endif
        buf = new char[ blocksize ];
      }
    catch (bad_alloc)
      {
        printf("WARNING: Can't allocate memory buffer for %d seconds of data.\n",
               blocks );
        printf("    Retrying with %d seconds of data.\n", blocks/2 );
        delete [] buf;
        processBlocks( blocks/2 );
        return retval;
      }
    
    rawfile->read( buf, blocksize ); // read all datablocks
    
#ifdef DEBUGGING_MESSAGES
    cerr << "read pointer position: " << rawfile->tellg() << endl;
    cerr << "bytes read:            " << rawfile->gcount() << endl;
#endif
    
    if ( rawfile->fail() || rawfile->eof() || (rawfile->gcount() != blocksize) )
      {
        blocksize = rawfile->gcount();
        blocks = blocksize / oneSecondBlockSize;
        retval = false;
      }

#ifdef DEBUGGING_MESSAGES
    cerr << "blocksize " << blocksize << endl;
    cerr << "blocks    " << blocks << endl;
#endif

    BlockHeader * pbuf = NULL;

/*#ifdef DEBUGGING_MESSAGES
		cout << " allocating " << sizeof(Sample) * header.nrSamplesPerSubband << " bytes for sample data." << endl;
#endif*/
    for ( int blk=0 ; blk < blocks ; blk++ )
      {
#ifdef DEBUGGING_MESSAGES
std::cout << "processing block " << blk << std::endl;
#endif
        pbuf = reinterpret_cast<BlockHeader*>(&(buf[ blk * (blocksize/blocks) ]));

        // convert endian when necessary
        if (!bigendian) { convertEndian(pbuf); }
				
        if ( first_block )
          {
            time_t utc;
            utc = (time_t)(pbuf->time[0]/(Int64)header.sampleRate);
            char * timeDateString = NULL;
            uint16_t buf_size = 128;
            if (!timeDateString)
              timeDateString = (char *)malloc(buf_size*sizeof(char));

            memset (timeDateString,'\0',buf_size);
            strftime(timeDateString, buf_size, "%T", gmtime(&utc));
            dataset.setAttribute( "EPOCH_UTC", std::string(timeDateString) );

            memset (timeDateString,'\0',buf_size);
            strftime(timeDateString, buf_size, "%d/%m/%y", gmtime(&utc));
            dataset.setAttribute( "EPOCH_DATE", std::string(timeDateString) );

            memset (timeDateString,'\0',buf_size);

            free(timeDateString);
            first_block = false;
          }

#ifdef _OPENMP
#pragma omp parallel for ordered schedule(dynamic)
#endif

				// needed when downsampling
/*				Float32 * ds_data = NULL;
				Float32 * totalintensity = NULL;
				const int ds_size = header.nrSamplesPerSubband / downsample_factor;
				if ( doDownsample_p ) {
					try {
						ds_data = new Float32[ ds_size ];
					}
					catch (bad_alloc) {
						cerr << "Can't allocate memory for downsampled array." << endl;
					}
					memset(ds_data, 0, ds_size * sizeof(Float32) );
					
				}
				else { // no downsampling
					if ( DO_FLOAT32_INTENSITY ) {
						try
						{
							totalintensity = new Float32[ header.nrSamplesPerSubband ];
						}
						catch (bad_alloc)
						{
							cerr << "Can't allocate memory for total intensity array." << endl;
						}
						memset(totalintensity, 0, header.nrSamplesPerSubband * sizeof(Float32) );
					}
				}*/
				
        for ( uint subband=0; subband < header.nrSubbands; ++subband )
          {
            index = blk *
                    (
                      (header.nrSamplesPerSubband * 8) *
                      header.nrSubbands
                    )
                    + (blk+1) * sizeof(blockheader) +
                    subband * header.nrSamplesPerSubband * 8;

            Sample * sample = reinterpret_cast<Sample*>(&( buf[ index ]));
/*
#ifdef DEBUGGING_MESSAGES
cout << "sample[0].xx: " << sample[0].xx << ", sample[0].yy: " << sample[0].yy <<  "sample[10].xx: " << sample[10].xx << ", sample[10].yy: " << sample[10].yy << endl;
#endif
*/

            if ( doDownsample_p )  // if downsampling
              {
								
								/*
								downsampleSingleSubband( sample,
										0,
									//	header.nrSamplesPerSubband,
										downsample_factor,
										ds_data,
										ds_size );
								*/
								downsampleSingleSubband( sample, downsample_factor, ds_data, ds_size );
								
#ifdef _OPENMP
#pragma omp ordered
#endif
                table[subband]->appendRows( ds_data, ds_size );
								// initialize the downsample data buffer again
								memset(ds_data, 0, ds_size * sizeof(Float32) );
								
              }
            else  // no downsampling
              {

                if ( DO_FLOAT32_INTENSITY )
                  {
                    compute_float32_intensity( sample,
                                                 0,
                                                 header.nrSamplesPerSubband,
																								 totalintensity );
#ifdef _OPENMP
#pragma omp ordered
#endif
                    table[subband]->appendRows( totalintensity,
                                                header.nrSamplesPerSubband );
             //       delete [] totalintensity;
						//				totalintensity = NULL;
                  }
                else
                  {
                    table[subband]->appendRows( sample,
                                                header.nrSamplesPerSubband );
               		}
              }

          } // for subband

      } // for block (blk)

    delete [] buf;
    buf = NULL;

#ifdef DEBUGGING_MESSAGES
// if (!first_block) return false;
#endif

    return retval;

  }

  // -------------------------------------------------- compute_float32_intensity

  inline void
  BFRaw::compute_float32_intensity( Sample * data,
																		uint32_t start,
																		uint32_t arraylength,
																		Float32 * totalintensity )
  {
		uint32_t xx_intensity(0);
		uint32_t yy_intensity(0);
//     Float32 * totalintensity = NULL;
//     try
//       {
//         totalintensity = new Float32[ arraylength ];
//       }
//     catch (bad_alloc)
//       {
//         cerr << "Can't allocate memory for total intensity array." << endl;
//       }
#ifdef _OPENMP
#pragma omp parallel for ordered schedule(dynamic)
#endif
    for ( uint count = 0; count < arraylength; ++count )
      {
//        totalintensity[ count ] = 0;

        for ( uint idx = start; idx < ( start + arraylength ); ++idx )
          {
						xx_intensity = (uint32_t)(real(data[ idx ].xx) * real(data[ idx ].xx) +
								imag(data[ idx ].xx) * imag(data[ idx ].xx) );
						yy_intensity = (uint32_t)(real(data[ idx ].yy) * real(data[ idx ].yy) +
								imag(data[ idx ].yy) * imag(data[ idx ].yy) );
						
            totalintensity[count] += (Float32)( xx_intensity * xx_intensity + yy_intensity * yy_intensity );
          }
			}
//    return totalintensity;
  }

	
	/*
	downsampleSingleSubband( sample,
	0,
									//	header.nrSamplesPerSubband,
	downsample_factor,
	ds_data,
	ds_size );
	*/
	
	//downsampleSingleSubband( singleSubbandDataBuf, downsample_factor, ds_data, ds_size );
  // -------------------------------------------- downsampleSingleSubband

  inline void
  BFRaw::downsampleSingleSubband( Sample * data,
                                         // uint32_t start,
                                         // const uint64_t arraylength,
                                          uint16_t factor,
																					Float32 * ds_data,
																					uint32_t ds_size)
  {
//    const int ds_size = arraylength / factor;
		
/*		double xx_intensity(0.0);
		double yy_intensity(0.0);*/
		uint32_t xx_intensity(0);//, xx_intensity2(0);
		uint32_t yy_intensity(0);//, yy_intensity2(0);
		uint32_t start(0);
//     Float32 * ds_data = NULL;
//     try
//       {
//         ds_data = new Float32[ ds_size ];
//       }
//     catch (bad_alloc)
//       {
//         cerr << "Can't allocate memory for downsampled array." << endl;
//       }

#ifdef _OPENMP
#pragma omp parallel for ordered schedule(dynamic)
#endif

    for ( uint samplecount = 0; samplecount < ds_size; ++samplecount ) // sample counter loops over all output samples
      {
		//		std::cout << "count: " << count << std::endl;
		       // ds_data[count] = 0;
				for ( uint idx = start; idx < (start+factor); ++idx ) // loop over nr of samples defined by downsampling factor
          {
							//std::cout << "calculating intensities for count 0" << std::endl;
//							std::cout << "real(data[" << idx << "].xx) = " << real(data[ idx ].xx) << std::endl;
						
						xx_intensity = (uint32_t)(real(data[ idx ].xx) * real(data[ idx ].xx) +
								imag(data[ idx ].xx) * imag(data[ idx ].xx) ); // this will be max 33 bits integer
						yy_intensity = (uint32_t)(real(data[ idx ].yy) * real(data[ idx ].yy) +
								imag(data[ idx ].yy) * imag(data[ idx ].yy) );
						/*
						xx_intensity2 = (uint32_t)((int32_t)real(data[ idx ].xx) * (int32_t)real(data[ idx ].xx) +
								(int32_t)imag(data[ idx ].xx) * (int32_t)imag(data[ idx ].xx) ); // this will be max 33 bits integer
						yy_intensity2 = (uint32_t)((int32_t)real(data[ idx ].yy) * (int32_t)real(data[ idx ].yy) +
								(int32_t)imag(data[ idx ].yy) * (int32_t)imag(data[ idx ].yy) );
						
						
						if (xx_intensity != xx_intensity2) {
							std::cout << "real(data[" << idx << "].xx = " << real(data[ idx ].xx) << std::endl
									<< "int32_t xx^2 = " << (int32_t)real(data[ idx ].xx) * (int32_t)real(data[ idx ].xx) << std::endl;
								//	<< "int32_t xx^2 = " << (int32_t)real(data[ idx ].xx) * (int32_t)real(data[ idx ].xx) << std::endl;
						}
						if (yy_intensity != yy_intensity2) {
							std::cout << "real(data[" << idx << "].yy= " << real(data[ idx ].yy) << std::endl
									<< "int32_t yy^2 = " << (int32_t)real(data[ idx ].yy) * (int32_t)real(data[ idx ].yy) << std::endl;
							//		<< "int32_t yy^2 = " << (int32_t)real(data[ idx ].yy) * (int32_t)real(data[ idx ].yy) << std::endl;
						}
						*/
//						std::cout << "xx_intensity = " << xx_intensity << ", yy_intensity = " << yy_intensity<< std::endl;
						
						/*
						xx_intensity = ( (double)real(data[ idx ].xx) * real(data[ idx ].xx) +
								(double)imag(data[ idx ].xx) * imag(data[ idx ].xx) );
						yy_intensity = ( (double)real(data[ idx ].yy) * real(data[ idx ].yy) +
								(double)imag(data[ idx ].yy) * imag(data[ idx ].yy) );
*/
/*            ds_data[count] +=
              (Float32)std::sqrt( xx_intensity * xx_intensity +
                                  yy_intensity * yy_intensity );*/
						ds_data[samplecount] += ( (Float32)xx_intensity * (Float32)xx_intensity + (Float32)yy_intensity * (Float32)yy_intensity );
//						ds_data[count] += (Float32)( xx_intensity * xx_intensity + yy_intensity * yy_intensity );
							/*
#ifdef DEBUGGING_MESSAGES
if ( (Float32)( xx_intensity * xx_intensity + yy_intensity * yy_intensity ) != 
(Float32)xx_intensity * (Float32)xx_intensity + (Float32)yy_intensity * (Float32)yy_intensity ) {
	std::cout << "xx_intensity = " << xx_intensity << ", yy_intensity = " << yy_intensity << ", xx^2+yy^2 = " << (Float32)( xx_intensity * xx_intensity + yy_intensity * yy_intensity ) << std::endl;
	std::cout << "xx_intensity(float) = " << (Float32)xx_intensity << ", yy_intensity(float) = " << (Float32)yy_intensity << ", xx^2+yy^2(float) = " << (Float32)( (Float32)xx_intensity * (Float32)xx_intensity + (Float32)yy_intensity * (Float32)yy_intensity ) << std::endl;
}
#endif
*/
							/*
#ifdef DEBUGGING_MESSAGES
						if (count == ds_size - 1) {
							std::cout << "ds_data[" << count << "] = " << ds_data[count] << std::endl;
						}
#endif
							*/
					}
        start += factor;
      }
//    return ds_data;
  }


} // DAL namespace
