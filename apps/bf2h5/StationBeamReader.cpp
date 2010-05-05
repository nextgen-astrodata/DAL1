/*-------------------------------------------------------------------------*
 | $Id:: TBB_Timeseries.h 4783 2010-04-28 09:49:12Z baehren              $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Alwin de Jong <jong@astron.nl>                                        *
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

#include "StationBeamReader.h"
#include <iostream> // for cout,cerr etc.
#include <fstream> // for file mode
#include <signal.h> // for time-out on socket
#include <sstream> // for RArad2deg and DECrad2deg
#include "bf2h5.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ios;
using std::stringstream;
using namespace DAL;

namespace DAL { // Namespace DAL -- begin
  
  int StationBeamReader::server_socket = 0;
  // bool StationBeamReader::time_out = false;

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  StationBeamReader::StationBeamReader (BF2H5 *parent,
					bool socket_mode)
    : socklen(sizeof(incoming_addr)),
      rawfile(0),
      itsParent(parent),
      socketmode(socket_mode), 
      memAllocOK(true),
      blockHeaderSize(sizeof(BFRawFormat::BlockHeader))
  {
    bigendian = BigEndian();
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  StationBeamReader::~StationBeamReader()
  {
    // close sockets and input file if open
    if (server_socket)
      close(server_socket);
    if (rawfile) {
      if (rawfile->is_open()) {
	rawfile->close();
      }
      delete rawfile;
      rawfile = NULL;
    }
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                timeout_alarm

  //! used to handle time out for socket connection
  /*
    void timeout_alarm (int sig)
    {
    StationBeamReader::time_out = true;
    std::cout << " time out on socket read" << std::endl;
    // shutdown and close socket connection
    shutdown(StationBeamReader::server_socket, SHUT_RDWR);
    close(StationBeamReader::server_socket);
    std::cout << "connection closed" << std::endl;
    signal (sig, timeout_alarm);
    }
  */
  
  //_____________________________________________________________________________
  //                                                                  openRawFile

  bool StationBeamReader::openRawFile (std::string &filename)
  {
    rawfile = new std::fstream( filename.data(), ios::binary|ios::in );

    /* Move to end of file to determine its file size. */
    rawfile->seekg(0, ios::end);
    /* See how many bytes in file */
    file_byte_size = static_cast<size_t>(rawfile->tellg())-2;
    /* Move to start of file */
    rawfile->seekg(0, ios::beg);

    return rawfile->is_open();
  }
  
  //_____________________________________________________________________________
  //                                                                finishReading

  void StationBeamReader::finishReading (void)
  {
#ifdef DEBUGGING_MESSAGES
    cout << "StationBeamReader::finishReading, now closing socket connection" << endl;
#endif
    
    if (socketmode) {
      // close socket
      shutdown(server_socket, SHUT_RDWR);
      close(server_socket);
    }
    else {
      if (rawfile->is_open())
	rawfile->close();
    }
    finished_reading = true;
  }
  
  //_____________________________________________________________________________
  //                                                                connectSocket

  /*!
    \brief portnumber -- Portnumber to which to connect
  */
  bool StationBeamReader::connectSocket (unsigned int port_number)
  {
    portNumber     = port_number;
    file_byte_size = 0;
    
    // Step 1 Create a socket using TCP
    server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (server_socket<0) {
      std::cerr << "[StationBeamReader::connectSocket] Socket creation failed!"
		<< std::endl;
      return false;
    }
    
    // Step 2 Create a sockaddr_in to describe the local port
    //sockaddr_in local_info;
    memset(&incoming_addr, 0, sizeof(incoming_addr));
    incoming_addr.sin_family = AF_INET;
    incoming_addr.sin_addr.s_addr = INADDR_ANY;
    incoming_addr.sin_port = htons(portNumber);
    
    // Step 3 Bind the socket to the port
    if (-1 == bind(server_socket, (sockaddr *) &incoming_addr, sizeof(incoming_addr)))
      {
#ifdef DEBUGGING_MESSAGES
	cerr << "StationBeamReader::connectSocket: ERROR socket bind failed" << endl;
#endif
	close(server_socket);
	return false;
      }
    
    // Step 4 listen for incoming connections
    if (-1 == listen(server_socket,5))
      {
#ifdef DEBUGGING_MESSAGES
	cerr << "StationBeamReader::connectSocket: ERROR listen failed" << endl;
#endif
	close(server_socket);
	return false;
      }
    int old_server_socket = server_socket;
    if (-1 == (server_socket = accept(server_socket, 0, 0))) {
#ifdef DEBUGGING_MESSAGES
      cerr << "StationBeamReader::connectSocket: ERROR accept failed" << endl;
#endif
      close(server_socket);
      return false;
    }
#ifdef DEBUGGING_MESSAGES
    cout << "StationBeamReader::connectSocket: socket connected" << endl;
#endif
    close(old_server_socket);
    
    return true;
  }
  
  //_____________________________________________________________________________
  //                                                               readMainHeader

  bool StationBeamReader::readMainHeader (BFRawFormat::BFRaw_Header &header)
  {
    if (socketmode) {
      if (recvfrom(server_socket, reinterpret_cast<char *>(&header), sizeof(header),0,  (sockaddr *) &incoming_addr, &socklen) < static_cast<int>(sizeof(header))) {
#ifdef DEBUGGING_MESSAGES
	cerr << "ERROR reading main header from socket" << endl;
#endif
	close(server_socket);
	return false;
      }
    }
    else { // file mode
      if ( !rawfile->read ( reinterpret_cast<char *>(&header), sizeof(header) ))
	{
#ifdef DEBUGGING_MESSAGES
	  cerr << "ERROR reading main header from file" << endl;
	  cerr << "read pointer position: " << rawfile->tellg() << endl;
#endif
	  return false;
	}
      
#ifdef DEBUGGING_MESSAGES
      cout << "size of main header: " << sizeof(header) << endl;
      cout << "read pointer position: " << rawfile->tellg() << endl;
#endif
    }
    
    // swap endians when necessary
    if ( !bigendian ) {
      swapHeaderEndians(header);
    }
    
#ifdef DEBUGGING_MESSAGES
    printHeaderParameters(header);
#endif
    
    return true;
  }
  
  //_____________________________________________________________________________
  //                                                           readFirstDataBlock

  bool
  StationBeamReader::readFirstDataBlock (BFRawFormat::BlockHeader &first_block_header,
					 BFRawFormat::Sample *sample_data,
					 size_t data_block_size)
  {
    dataBlockSize      = data_block_size;
    int64_t read_bytes = receiveBytes(reinterpret_cast<char *>(&first_block_header),
				      sizeof(BFRawFormat::BlockHeader));

    if (read_bytes > 0) {
      if (!bigendian) { convertEndian(&first_block_header); }
      if ((read_bytes = receiveBytes(reinterpret_cast<char *>(sample_data), dataBlockSize)) != -1) {
	return true;
      }
      else if (read_bytes == 0) {
	finishReading();
	return false;
      }
      else {
#ifdef DEBUGGING_MESSAGES
	cerr << "ERROR, receiving the first data block samples" << endl;
#endif
	return false;
      }
    }
    else if (read_bytes == 0) {
      finishReading();
      return false;
    }
    else {
#ifdef DEBUGGING_MESSAGES
      cerr << "ERROR, receiving the first data block header" << endl;
#endif
      //throw "Error, receiving the first data block header";
    }
    return false;
    //}
  }
  
  //_____________________________________________________________________________
  //                                                                readDataBlock

  bool StationBeamReader::readDataBlock (BFRawFormat::Sample *sample_data)
  {
    BFRawFormat::BlockHeader *dump = new BFRawFormat::BlockHeader();
    int64_t read_bytes = receiveBytes(reinterpret_cast<char *>(dump), blockHeaderSize);
    delete dump;
    if (read_bytes > 0) { // throw away block header
      //	if (!bigendian) { convertEndian(&blockheader); }
      if ((read_bytes = receiveBytes(reinterpret_cast<char *>(sample_data), dataBlockSize)) > 0) {
	cout << "sampledata[0].xx=" << sample_data->xx << ", yy=" << sample_data->yy << endl;
	return true;
      }
      else if (read_bytes == 0) {
	finishReading();
	return false;
      }
      else {
	cerr << "ERROR, receiving data block samples" << endl;
	return false;
      }
    }
    else if (read_bytes == 0) {
      finishReading();
      return false;
    }
    else {
#ifdef DEBUGGING_MESSAGES
      cerr << "ERROR, receiving data block header" << endl;
#endif
      //throw "Error, receiving the first data block header";
      return false;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                 receiveBytes

  int64_t StationBeamReader::receiveBytes (void *storage,
					   int64_t nrOfBytesToRead)
  {
    int64_t bytes_read  = 0;
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
  
  //_____________________________________________________________________________
  //                                                            swapHeaderEndians

  void StationBeamReader::swapHeaderEndians (BFRawFormat::BFRaw_Header &header)
  {
    // change byte order for all frequencies.
    for (int ii = 0; ii < BFRawFormat::maxNrSubbands; ii++)
      {
	swapbytes((char *)&header.subbandFrequencies[ii],8);
	swapbytes((char *)&header.subbandToBeamMapping,2);
      }
    
    for (int ii = 0; ii < 8; ii++)
      {
	for (int jj = 0; jj < 2; jj++)
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
  
  //_____________________________________________________________________________
  //                                                                convertEndian

  void StationBeamReader::convertEndian (BFRawFormat::BlockHeader *pBlockHeader)
  {
    swapbytes((char *)&pBlockHeader->magic,4);
    
    for ( uint ii = 0; ii < 8; ii++ )
      {
	swapbytes((char *)&pBlockHeader->coarseDelayApplied[ ii ],4);
	swapbytes((char *)&pBlockHeader->fineDelayRemainingAtBegin[ ii ],8);
	swapbytes((char *)&pBlockHeader->fineDelayRemainingAfterEnd[ ii ],8);
	swapbytes((char *)&pBlockHeader->time[ ii ],8);
	swapbytes((char *)&pBlockHeader->flags[ ii ].nrFlagsRanges,4);
	//				swapbytes((char *)&pBlockHeader->nrFlagsRanges[ ii ],4);
	for ( uint jj = 0; jj < 16; jj++ )
	  {
	    swapbytes( (char *)&pBlockHeader->flags[ ii ].flagsRanges[ jj ].begin,4 );
	    swapbytes( (char *)&pBlockHeader->flags[ ii ].flagsRanges[ jj ].end,4 );
	    /*						swapbytes( (char *)&pBlockHeader->flagsRanges[ ii ][ jj ].begin,4 );
							swapbytes( (char *)&pBlockHeader->flagsRanges[ ii ][ jj ].end,4 ); */
	  }
      }
  }
  
  //_____________________________________________________________________________
  //                                                        printHeaderParameters

  void StationBeamReader::printHeaderParameters (BFRawFormat::BFRaw_Header &header)
  {
    cout << "-- Magic number                   = " << header.magic               << endl
	 << "-- bits per sample                = " << header.bitsPerSample       << endl
	 << "-- nof. polarizations             = " << header.nrPolarizations     << endl
	 << "-- nof. subbands                  = " << header.nrSubbands          << endl
	 << "-- Samples per subband            = " << header.nrSamplesPerSubband << endl
	 << "-- Station ID                     = " << header.station             << endl
	 << "-- Sample rate                    = " << header.sampleRate          << endl
	 << "-- Centre freq. of subbands (MHz) = " << endl;
    for (int ii=0;ii<header.nrSubbands;ii++)
      {
	cout << header.subbandFrequencies[ii]/1000000.0 << "  ";
	if (((ii+1)%4) == 0 ) cout << endl;
      }
    
    cout << "Beam Directions J2000 radians:" << endl;
    
    for (int ii=0;ii<8;ii++)
      {
	dec_str = "";
	cout << "[" << ii << "]";
	for (int jj=0;jj<2;jj++)
	  {
	    cout << header.beamDirections[ii][jj] << "   ";
	    if ( 0 == jj )
	      RArad2deg( header.beamDirections[ii][jj] );
	    else
	      DECrad2deg( header.beamDirections[ii][jj] );
	  }
	cout << "[ " << ra_str << ", " << dec_str << "]" << endl;
      }
  }
  
  //_____________________________________________________________________________
  //                                                                    RArad2deg

  /*!
    \brief Convert RA position (in radian) to degrees
    
    \param rad -- The RA position in radian
    
    \return deg -- The RA position in degrees
  */
  const std::string &StationBeamReader::RArad2deg( const float &rad )
  {
    ra_str = "";
    float deg_hour = ( rad * (180./PI) ) / 15.;
    int   hour     = (int)deg_hour;
    float deg_min  = (deg_hour - hour) * 60.;
    int    min     = int( deg_min );
    float  sec     = (deg_min - min) * 60;
    
    //sprintf(ra_string,"%02d:%02d:%02.4f",  , min, sec);
    std::stringstream rass;
    rass << hour << ":" << min << ":" << sec;
    rass >> ra_str;
    
    return ra_str;
  }
  
  //_____________________________________________________________________________
  //                                                                   DECrad2deg

  /*!
    \brief Convert Dec position (in radian) to degrees
    
    \param rad -- The Dec position in radian
    
    \return deg -- The Dec position in degrees
  */
  const std::string &StationBeamReader::DECrad2deg( const float &dec )
  {
    float deg_hour = ( dec * (180./PI) );
    int   hour     = int( deg_hour );
    float deg_min  = (deg_hour - hour) * 60.;
    int   min      = int( deg_min );
    float sec      = (deg_min - min) * 60;
    
    stringstream dess;
    dess << hour << ":" << min << ":" << sec;
    dess >> dec_str;
    
    return dec_str;
  }

} // END : namespace DAL
