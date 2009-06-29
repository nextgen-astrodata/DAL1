/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
 *   J.S.Masters@uva.nl                                                    *
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

#ifndef TBB_H
#include "TBB.h"
#endif

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                          TBB

  /*!
    \param filename -- Name of the output HDF5 dataset to create
  */
  TBB::TBB( string const& filename )
    : name (filename)
  {
    init ();
    
    if ( FAIL == dataset->open( filename.c_str() ) )
      {
        cerr << "Creating new dataset " << filename << endl;
        delete dataset;
        dataset = new dalDataset( filename.c_str(), "HDF5" );
	
        dataset->setAttribute( attribute_name(TELESCOPE),
			       telescope_p );
        dataset->setAttribute( attribute_name(OBSERVER),
			       observer_p );
        dataset->setAttribute( attribute_name(PROJECT),
			       project_p );
        dataset->setAttribute( attribute_name(OBSERVATION_ID),
                               observation_id_p );
        dataset->setAttribute( attribute_name(OBSERVATION_MODE),
                               observationMode_p );

      }
    else
      {
        cerr << "Dataset " << filename << " exists." << endl;

        stations = dataset->getGroupNames();

#ifdef DEBUGGING_MESSAGES
        for ( unsigned int ss = 0; ss < stations.size(); ss++ )
          {
            cerr << stations[ss] << " group exists." << endl;
          }
#endif

      }

  }

  //_____________________________________________________________________________
  //                                                                          TBB

  /*!
    \param filename  -- Name of the output HDF5 dataset to create
    \param telescope -- Name of the telecope
    \param observer  -- Name of the observer
    \param project   -- Name of the project
  */
  TBB::TBB (string const &outfile,
	    string const &telescope,
	    string const &observer,
	    string const &project)
    : name (outfile),
      telescope_p (telescope),
      observer_p (observer),
      project_p (project)
  {
    headerp_p = &header;
  }
  

  //_____________________________________________________________________________
  //                                                                         init

  void TBB::init ()
  {
    /* Initialization of private data */

    telescope_p        = "LOFAR";
    observer_p         = "UNDEFINED";
    project_p          = "UNDEFINED";
    observation_id_p   = "UNDEFINED";
    observationMode_p  = "UNDEFINED";

    setTimeoutStart (-1.0);
    setTimeoutRead (1.0);
    
    seqnrLast_p  = 0;
    bigendian_p  = BigEndian();
    sampleTime_p = (time_t)0;
    dataset      = new dalDataset;
    station.clear();
    rr           = 0;
    main_socket  = -1;
    socklen      = sizeof(incoming_addr);
    status       = 0;
    stations.clear();
    stationGroup_p = NULL;
    dipoleArray_p  = NULL;
    dipoles.clear();
    dims.push_back(0);
    offset_p   = 0;
    cdims.push_back(CHUNK_SIZE);
    //stationstr = NULL;
    memset(uid,'-',10);
    payload_crc       = 0;
    tran_sample.value = 0;
    spec_sample.value = 0;
    size              = 0;
    memblock          = NULL;
    rawfile_p         = NULL;
    real_part         = 0;
    imag_part         = 0;
#ifdef USE_INPUT_BUFFER
    inBufProcessID = inBufStorID = 0;
    maxWaitingFrames = 0;
    noFramesDropped = 0;
    inputBuffer_P = new char [(INPUT_BUFFER_SIZE*UDP_PACKET_BUFFER_SIZE)];
    udpBuff_p = inputBuffer_P;
#endif 
    /* Initialization of public data */

    first_sample = true;

    /* Initialization of structs */

    headerp_p = &header;
    init_TBB_Header ();
  }
  
  //_____________________________________________________________________________
  //                                                              init_TBB_Header
  
  void TBB::init_TBB_Header ()
  {
    headerp_p->stationid           = 0;
    headerp_p->rspid               = 0;
    headerp_p->rcuid               = 0;
    headerp_p->sample_freq         = 0;
    headerp_p->seqnr               = 0;
    headerp_p->time                = 0;
    headerp_p->sample_nr           = 0;
    headerp_p->n_samples_per_frame = 0;
    headerp_p->n_freq_bands        = 0;
    headerp_p->spare               = 0;
    headerp_p->crc                 = 0;
    memset(headerp_p->bandsel,0,64);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  TBB::~TBB()
  {
    if ( dipoleArray_p )
      {
        delete dipoleArray_p;
        dipoleArray_p = 0;
      }
    if ( stationGroup_p )
      {
        delete stationGroup_p;
        stationGroup_p = 0;
      }
    delete dataset;
    if (main_socket)
      close(main_socket);
    if (rawfile_p)
      {
        if (rawfile_p->is_open())
          {
            rawfile_p->close();
          }
        delete rawfile_p;
        rawfile_p = 0;
      }
#ifdef USE_INPUT_BUFFER
    delete [] inputBuffer_P;
#endif
  }

  // ============================================================================
  //
  //  Access to parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                              setTimeoutStart
  
  /*!
    \param timeout -- Timeout, [sec], before stopping to listen for data; if a
           value greater zero is provided, the value is the time-out in seconds
	   -- otherwise the connection is kept open indefinitely.
  */
  void TBB::setTimeoutStart (double const &timeout)
  {
    /*
     * Decompose the floating point value of the timeout into two parts:
     * seconds (time_sec) and micro-seconds (time_usec).
     */
    int time_sec  = std::floor(timeout);
    int time_usec = 1e06*(timeout - time_sec);

#ifdef DEBUGGING_MESSAGES    
    std::cout << "[TBB::setTimeoutStart]" << std::endl;
    std::cout << "-- Timeout        = " << timeout   << std::endl;
    std::cout << "-- Timeout  [sec] = " << time_sec  << std::endl;
    std::cout << "-- Timeout [usec] = " << time_usec << std::endl;
#endif

    setTimeoutStart (time_sec,
		     time_usec);
  }
  
  //_____________________________________________________________________________
  //                                                               setTimeoutRead
  
  /*!
    \param timeout -- Timeout, [sec]
  */
  void TBB::setTimeoutRead (double const &timeout)
  {
    /*
     * Decompose the floating point value of the timeout into two parts:
     * seconds (time_sec) and micro-seconds (time_usec).
     */
    int time_sec  = std::floor(timeout);
    int time_usec = 1e06*(timeout - time_sec);

#ifdef DEBUGGING_MESSAGES    
    std::cout << "[TBB::setTimeoutRead]" << std::endl;
    std::cout << "-- Timeout        = " << timeout   << std::endl;
    std::cout << "-- Timeout  [sec] = " << time_sec  << std::endl;
    std::cout << "-- Timeout [usec] = " << time_usec << std::endl;
#endif

    setTimeoutRead (time_sec,
		    time_usec);
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is going to be written
  */
  void TBB::summary (std::ostream &os)
  {
    os << "[TBB] Summary of object properties"                << endl;
    os << "-- Last sequence number : " << seqnrLast_p         << endl;
    os << "-- System is big endian : " << bigendian_p         << endl;
    os << "-- Sample time ........ : " << sampleTime_p        << endl;
    os << "-- Attributes attached to file root group:"        << endl;
    os << "   -- TELESCOPE        = " << telescope()          << endl;
    os << "   -- OBSERVER         = " << observer()           << endl;
    os << "   -- PROJECT          = " << project()            << endl;
    os << "   -- OBSERVATION_ID   = " << observation_id()     << endl;
    os << "   -- OBSERVATION_MODE = " << observation_mode()   << endl;
    os << "-- Time-outs for socket mode"                      << endl;
    os << "   -- Start-up         = [" 
       << timeoutStart_p.tv_sec
       << ";" 
       << timeoutStart_p.tv_usec
       << "]" << endl;
    os << "   -- Reading          = ["
       << timeoutRead_p.tv_sec
       << ";"
       << timeoutRead_p.tv_usec
       << "]" << endl;
  }
  
  //_____________________________________________________________________________
  //                                                                connectsocket

  /*!
    \param ipaddress  -- IP number to which to connect
    \param portnumber -- Portnumber to which to connect
  */
  void TBB::connectsocket( const char* ipaddress,
			   const char* portnumber)
  {
    int port_number = atol( portnumber );
    const char * remote = ipaddress;

    // Step 1: Look up server to get numeric IP address
    hostent * record = gethostbyname(remote);
    if (record==NULL)
      {
        herror("gethostbyname failed");
        return;
      }

    // Step 2: Create a socket
    main_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (main_socket<0)
      {
        perror("socket creation");
        return;
      }

    // Step 3: Create a sockaddr_in to describe the local port
    sockaddr_in local_info;
    local_info.sin_family = AF_INET;
    local_info.sin_addr.s_addr = htonl(INADDR_ANY);
    local_info.sin_port = htons(port_number);

    // Step 4: Bind the socket to the port
    int rr = bind(main_socket, (sockaddr *) &local_info, sizeof(local_info));
    if (rr<0)
      {
        perror("bind");
        return;
      }
    printf("ready\n");

    // Step 5: Start listening to the port
    FD_ZERO(&readSet);
    FD_SET(main_socket, &readSet);
    
    if (timeoutStart_p.tv_sec+timeoutStart_p.tv_usec > 0) {
      select( main_socket + 1, &readSet, NULL, NULL, &timeoutStart_p );
    }
    else {
      select( main_socket + 1, &readSet, NULL, NULL, NULL );
    }
    
    // wait for up to "timeout" seconds for data to start showing up
  }
  
  //_____________________________________________________________________________
  // Open file containing data resulting from a TBB dump

  /*!
    \param filename -- Name of the data file

    \return status -- Status of the operation; returns \e true in case the input
            file could be opened successfully, if an error is encountered
	    \e false is returned.
  */
  bool TBB::openRawFile( const char* filename )
  {
    delete rawfile_p;
    rawfile_p = new fstream( filename, ios::binary|ios::in );
    if ( !rawfile_p->is_open() )
      {
        std::cerr << "Error opening intput file: " << std::string(filename)
                  << std::endl;
        return false;
      }
    else
      {
        rawfile_p->seekg (0, ios::beg);  // move to start of file
        return true;
      }
  }

  //_____________________________________________________________________________
  // Read data from a socket

#ifndef USE_INPUT_BUFFER
  /*!
    \param nbytes -- Number of Bytes read
    \param buf    -- Buffer holding the read data
  */
  int TBB::readsocket( unsigned int nbytes,
		       char* buf)
  {
    FD_ZERO(&readSet);
    FD_SET(main_socket, &readSet);

    struct timeval readTimeout;
    readTimeout = timeoutRead_p;

    int status;
    // waits for up to N seconds for data appearing in the socket
    if ( (status = select( main_socket + 1, &readSet, NULL, NULL, &readTimeout) ) )
      {
        rr = recvfrom( main_socket, buf, nbytes, 0,
                       (sockaddr *) &incoming_addr, &socklen);
#ifdef DEBUGGING_MESSAGES
	cout << "readRawSocketBlockHeader: Frame with " << rr << " bytes." << endl;
	if (rr == nbytes) 
	  {
	    cout << "readRawSocketBlockHeader: Oversized packet received." << endl;
	  };
#endif
      }
    else
      {
        cout << "Data stopped coming" << endl;
        return FAIL;
      }

    if (rr<0)
      {
        perror("recvfrom");
        return FAIL;
      }

    return SUCCESS;
  }
#endif
  //_____________________________________________________________________________
  // Read data from a socket into the input buffer
#ifdef USE_INPUT_BUFFER
  int TBB::readSocketBuffer() 
  {
    struct timeval readTimeout;
    FD_ZERO(&readSet);
    FD_SET(main_socket, &readSet);
    int newBufID, nFramesWaiting = 0;

    //set timeout to zero (don't wait, just poll)
    readTimeout.tv_sec = readTimeout.tv_usec = 0;
    while (select(main_socket + 1, &readSet, NULL, NULL, &readTimeout) > 0 ) {
      //there is a frame waiting in the vBuffer
      newBufID = inBufStorID+1;
      if (newBufID >= INPUT_BUFFER_SIZE) { newBufID =0; }
      if (newBufID == inBufProcessID) {
	//cerr << "TBB::readSocketBuffer: Buffer overflow! Overwriting last frame." << endl;
	noFramesDropped++;
	newBufID = inBufStorID;
      };
      //perform the actual read
      rr = recvfrom( main_socket, (inputBuffer_P + (newBufID*UDP_PACKET_BUFFER_SIZE)), 
		     UDP_PACKET_BUFFER_SIZE, 0, (sockaddr *) &incoming_addr, &socklen);
      nFramesWaiting++;
      inBufStorID = newBufID;
    };
    if (nFramesWaiting > maxWaitingFrames) { maxWaitingFrames = nFramesWaiting; };
    if ( inBufStorID == inBufProcessID) {
      // that means input buffer is empty
      readTimeout.tv_sec = timeoutRead_p.tv_sec;
      readTimeout.tv_usec = timeoutRead_p.tv_usec;
      FD_ZERO(&readSet);
      FD_SET(main_socket, &readSet);
      if (status = select(main_socket + 1, &readSet, NULL, NULL, &readTimeout) ) {
	inBufStorID++;
	if (inBufStorID >= INPUT_BUFFER_SIZE) { inBufStorID =0; }
	rr = recvfrom( main_socket, (inputBuffer_P+(inBufStorID*UDP_PACKET_BUFFER_SIZE)), 
		       UDP_PACKET_BUFFER_SIZE, 0, (sockaddr *) &incoming_addr, &socklen);	
      } else {
	// we waited for "timeoutRead_p" but still no data -> end of data
        cout << "TBB::readSocketBuffer: Data stopped coming" << endl;
	cout << "TBB::readSocketBuffer: inBufProcessID: " << inBufProcessID 
	     << " inBufStorID: " << inBufStorID << " select-status:" << status 
	     << " remaining-sec: " << readTimeout.tv_sec << " -usec: " << readTimeout.tv_usec << endl;
	cout << "TBB::readSocketBuffer: Max no. of frames waiting: " << maxWaitingFrames
	     << " number of discarded frames: " << noFramesDropped << endl;
        return FAIL;
      };
    };
#ifdef DEBUGGING_MESSAGES
    if (inBufStorID == inBufProcessID) {
      cerr << "TBB::readSocketBuffer: Empty buffer at end of method!" << endl;
      return Fail;
    };
#endif
    inBufProcessID++;
    if (inBufProcessID >= INPUT_BUFFER_SIZE) { inBufProcessID =0; };
    udpBuff_p = (inputBuffer_P + (inBufProcessID*UDP_PACKET_BUFFER_SIZE));
    return SUCCESS; 
  };
#endif

  //_____________________________________________________________________________
  // Read the 88-byte TBB frame header

  bool TBB::readRawSocketBlockHeader()
  {
#ifdef USE_INPUT_BUFFER
    // read data through the input buffer
    status = readSocketBuffer();
#else
    // read a single full UDP-datagram, as otherwise the remainder may be flushed by the OS
    status = readsocket( UDP_PACKET_BUFFER_SIZE, udpBuff_p );
#endif
    if (FAIL == status) {
      return false;
    }

    // set the headerpointer to the just read data
    headerp_p = (TBB_Header*)udpBuff_p;
    
    // reverse fields if big endian
    if ( bigendian_p )
		{
			swapbytes( (char *)&(headerp_p->seqnr), 4 );
			swapbytes( (char *)&(headerp_p->time), 4 );
			swapbytes( (char *)&(headerp_p->sample_nr), 4 );
			swapbytes( (char *)&(headerp_p->n_samples_per_frame), 2);
			swapbytes( (char *)&(headerp_p->n_freq_bands), 2 );
		}
		
    // Convert the time (seconds since 1 Jan 1970) to a human-readable string
    // "The samplenr field is used together with the time field to get
    //  an absolute time reference.  The samplenr field holds the number of
    //  samples that was counted by RSP since the start of the current second."
    // "By multiplying the samplenr with the sample period and additing it to
    //  the seconds field, the time instance of the first data sample of the
    //  frame is known"
    //   -- TBB Design Description document, Wietse Poiesz (2006-10-3)
    //      Doc..id: LOFAR-ASTRON-SDD-047
    sampleTime_p =
      (time_t)( headerp_p->time + headerp_p->sample_nr/(headerp_p->sample_freq*1000000) );
    char *time_string=ctime(&sampleTime_p);
    time_string[strlen(time_string)-1]=0;   // remove \n

#ifdef DEBUGGING_MESSAGES
    printf("Time:              : %s\n",time_string );
    printRawHeader();
#endif

    return true;
  }

  //_____________________________________________________________________________
  // Read 88-byte TBB frame header
  
  /*!
    Convert the time (seconds since 1 Jan 1970) to a human-readable string
    "The samplenr field is used together with the time field to get
    an absolute time reference.  The samplenr field holds the number of
    samples that was counted by RSP since the start of the current second."
    "By multiplying the samplenr with the sample period and additing it to
    the seconds field, the time instance of the first data sample of the
    frame is known"

    -- TBB Design Description document, Wietse Poiesz (2006-10-3)
        Doc..id: LOFAR-ASTRON-SDD-047
  */
  void TBB::readRawFileBlockHeader()
  {
    rawfile_p->read(reinterpret_cast<char *>(&header), sizeof(header));
    // set the headerpointer to the just read data
    headerp_p = &header;

    // reverse fields if big endian
    if ( bigendian_p )
      {
        swapbytes( (char *)&(headerp_p->seqnr), 4 );
        swapbytes( (char *)&(headerp_p->time), 4 );
	swapbytes( (char *)&(headerp_p->sample_nr), 4 );
	swapbytes( (char *)&(headerp_p->n_samples_per_frame), 2);
	swapbytes( (char *)&(headerp_p->n_freq_bands), 2 );
      }

    /*
     * Convert the time (seconds since 1 Jan 1970) to a human-readable string
     */
    sampleTime_p =
      (time_t)( headerp_p->time + headerp_p->sample_nr/(headerp_p->sample_freq*1000000) );
    char *time_string=ctime(&sampleTime_p);
    time_string[strlen(time_string)-1]=0;   // remove \n

#ifdef DEBUGGING_MESSAGES
    printf("Time:              : %s\n",time_string );
    printRawHeader();
#endif

  }

  //_____________________________________________________________________________
  // Check for the end of file

  bool TBB::eof()
  {
    if ( rawfile_p->peek() == EOF )
      return true;
    else
      return false;
  }

  //_____________________________________________________________________________
  // Print the contents of a raw TBB frame header
  
  void TBB::printRawHeader()
  {
    printf("Station ID         : %d\n",headerp_p->stationid);
    printf("RSP ID             : %d\n",headerp_p->rspid);
    printf("RCU ID             : %d\n",headerp_p->rcuid);
    printf("Sample Freqency    : %d MHz\n",headerp_p->sample_freq);
    printf("Sequence Number    : %d\n",headerp_p->seqnr);
    printf("Sample Number      : %d\n",headerp_p->sample_nr);
    printf("Samples Per Frame  : %d\n",headerp_p->n_samples_per_frame);
    printf("Num. of Freq. Bands: %d\n",headerp_p->n_freq_bands);
    printf("Bands present : ");
    for (int idx=0; idx<64; idx++)
      printf("%X,", headerp_p->bandsel[idx]);
    printf("\n");
  }
  
	// Generic CRC16 method working on 16-bit unsigned data
	// adapted from Python script by Gijs Schoonderbeek.
  
  uint16_t TBB::CRC16(uint16_t * buffer, uint32_t length)
  {
    uint16_t CRC = 0;
    const uint32_t CRC_poly = 0x18005;
    const uint16_t bits = 16;
    uint32_t data = 0;
    const uint32_t CRC_div = (CRC_poly & 0x7fffffff) << 15;
    
    data = (buffer[0] & 0x7fffffff) << 16;
    for(uint32_t i=1; i<length; i++)
      {
	data += buffer[i];
	for(uint16_t j=0; j<bits; j++)
	  {
	    if ((data & 0x80000000) != 0)
	      { data ^= CRC_div; }
	    data &= 0x7fffffff;
	    data <<= 1;
	  }
      }
    CRC = data >> 16;
    return CRC;
  }
  
  // Check the CRC of a TBB frame header. Uses CRC16. Returns TRUE if OK, FALSE otherwise
  bool TBB::headerCRC()
  { 
    unsigned int seqnr = headerp_p->seqnr; // temporary; we need to zero this out for CRC check
    headerp_p->seqnr = 0;
    
    uint16_t * headerBuf = reinterpret_cast<uint16_t*> (headerp_p); 
    uint16_t CRC = this->CRC16(headerBuf, sizeof(TBB_Header) / sizeof(uint16_t));
    headerp_p->seqnr = seqnr; // and set it back again
    
    return (CRC == 0);
  }
  
  
  
  //_____________________________________________________________________________
  // Check if the group for a given station exists within the HDF5 file
  
  void TBB::stationCheck()
  {   
    char stationstr[10];
    char dipolestr[10];
    memset(stationstr,'\0',10);
    sprintf( stationstr, "Station%03d", headerp_p->stationid );
    sprintf( dipolestr, "%03d%03d%03d", headerp_p->stationid, headerp_p->rspid, headerp_p->rcuid);
    first_sample = false;
    
    /* Use std::string for the name of the station group */
    std::string channelID (dipolestr);
    
    first_sample = false;

    // does the station exist?
    if ( it_exists( stations, stringify(stationstr)) ) {

      // if the station group already exists, then open it
      stationGroup_p = dataset->openGroup( stationstr );

      dipoles = stationGroup_p->getMemberNames();

      // does the dipole exist?
      if ( it_exists( dipoles, stringify(dipolestr)) )	{
        // if the dipole array already exists, then open it
        dipoleArray_p = dataset->openArray( dipolestr, stationGroup_p->getName() );
      }
      else
      {
	//Need to generate a new dipole structure
	makeNewDipole(dipolestr, stationGroup_p, headerp_p);
      }

    } else {

      //Station unknown, make new station group, which also makes a new dipole.

      makeNewStation(stationstr, headerp_p);

      dipoles = stationGroup_p->getMemberNames();
    }

    dims          = dipoleArray_p->dims();
    offset_p      = dims[0];
  }

  // ---------------------------------------------------------- makeStationHeader

  void TBB::makeOutputHeader(){
    //this function has become 
  };

  // ------------------------------------------------------------- makeNewStation

  void TBB::makeNewStation(char * newStationstr, TBB_Header * headerPtr)
  {
    stations.push_back( newStationstr );
    stationGroup_p = dataset->createGroup( newStationstr );

    double trigger_offset[1]             = { 0 };
    int triggered_antennas[1]            = { 0 };
    double station_position_value[3]     = { 0, 0, 0 };
    std::string station_position_unit[3] = { "m", "m", "m" };
    std::string station_position_frame   = "ITRF";
    double beam_direction_value[2]       = { 0, 90 };
    std::string beam_direction_unit[2]   = { "deg", "deg"};
    std::string beam_direction_frame     = "AZEL";
    
    // Add attributes to "Station" group
    stationGroup_p->setAttribute( attribute_name(STATION_POSITION_VALUE),
				  station_position_value, 3 );
    stationGroup_p->setAttribute( attribute_name(STATION_POSITION_UNIT),
				  station_position_unit, 3 );
    stationGroup_p->setAttribute( attribute_name(STATION_POSITION_FRAME),
				  station_position_frame );
    stationGroup_p->setAttribute( attribute_name(BEAM_DIRECTION_VALUE),
				  beam_direction_value, 2 );
    stationGroup_p->setAttribute( attribute_name(BEAM_DIRECTION_UNIT),
				  beam_direction_unit, 2 );
    stationGroup_p->setAttribute( attribute_name(BEAM_DIRECTION_FRAME),
				  beam_direction_frame );
    stationGroup_p->setAttribute( attribute_name(TRIGGER_TYPE),
				  std::string("UNDEFINED") );
    stationGroup_p->setAttribute( attribute_name(TRIGGER_OFFSET),
				  trigger_offset );
    stationGroup_p->setAttribute( attribute_name(TRIGGERED_ANTENNAS),
				  triggered_antennas);
    if ( 0 == headerPtr->n_freq_bands ) {
      stationGroup_p->setAttribute( attribute_name(OBSERVATION_MODE),
				     std::string("Transient") );
    } else {
      stationGroup_p->setAttribute( attribute_name(OBSERVATION_MODE),
				     std::string("Sub-band") );
    };


    //As there is a new Station, then there is also a new dipole!
    /* Generate channel ID for the individual dipole */
    sprintf(uid, "%03d%03d%03d",
	    headerPtr->stationid, headerPtr->rspid, headerPtr->rcuid);
    makeNewDipole(uid, stationGroup_p, headerPtr);
    /* Feedback */
    std::cout << "CREATED New station group: " << newStationstr << std::endl;

  };

  // ------------------------------------------------------------ makeNewDipole

  void TBB::makeNewDipole(string newuid, dalGroup * stationGroupPtr, TBB_Header * headerPtr)
  {
    vector<int> firstdims;
    firstdims.push_back( 0 );
    
    if ( 0 == headerPtr->n_freq_bands )
      {
        short nodata[0];
        dipoleArray_p = stationGroupPtr->createShortArray( newuid,
							   firstdims,
							   nodata,
							   cdims );
      }
    else
      {
        complex<Int16> nodata[0];
        dipoleArray_p = stationGroupPtr->createComplexShortArray( newuid,
								  firstdims,
								  nodata,
								  cdims );
        stationGroupPtr->setAttribute( attribute_name(OBSERVATION_MODE),
				       std::string("Sub-band") );
      }
    
    unsigned int sid = headerPtr->stationid;
    unsigned int rsp = headerPtr->rspid;
    unsigned int rcu = headerPtr->rcuid;
    double sf = headerPtr->sample_freq;
    unsigned int time                    = headerPtr->time;
    unsigned int samp_num                = headerPtr->sample_nr;
    unsigned int samples_per_frame       = headerPtr->n_samples_per_frame;
    unsigned int nyquist_zone            = 1;
    double antenna_position_value[3]     = { 0, 0, 0 };
    std::string antenna_position_unit[3] = { "m", "m", "m" };
    
    dipoleArray_p->setAttribute( attribute_name(STATION_ID), &sid );
    dipoleArray_p->setAttribute( attribute_name(RSP_ID), &rsp );
    dipoleArray_p->setAttribute( attribute_name(RCU_ID), &rcu );
    dipoleArray_p->setAttribute( attribute_name(TIME), &time );
    dipoleArray_p->setAttribute( attribute_name(SAMPLE_NUMBER), &samp_num );
    dipoleArray_p->setAttribute( attribute_name(SAMPLES_PER_FRAME), &samples_per_frame );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_POSITION_VALUE),
				 antenna_position_value, 3 );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_POSITION_UNIT),
				 antenna_position_unit, 3 );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_POSITION_FRAME),
				 std::string("ITRF") );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_ORIENTATION_VALUE),
				 antenna_position_value, 3 );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_ORIENTATION_UNIT),
				 antenna_position_unit, 3 );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_ORIENTATION_FRAME),
				 std::string("ITRF") );
    dipoleArray_p->setAttribute( attribute_name(FEED), std::string("UNDEFINED") );
    dipoleArray_p->setAttribute( attribute_name(NYQUIST_ZONE),
				 &nyquist_zone );
    dipoleArray_p->setAttribute( attribute_name(SAMPLE_FREQUENCY_VALUE), &sf, 1 );
    dipoleArray_p->setAttribute( attribute_name(SAMPLE_FREQUENCY_UNIT),
				 std::string("MHz") );
#ifdef DEBUGGING_MESSAGES
    /* Feedback */
    std::cout << "CREATED New dipole group: " << newuid << std::endl;
#endif

  };
  
  // -------------------------------------------------------------- transientMode

  bool TBB::transientMode()
  {
    if ( 0==headerp_p->n_freq_bands )
      return true;   // transient mode
    else
      return false;  // spectral mode
  }

  void TBB::fixDate(){
    static bool oddSecond = false;
    if (headerp_p->sample_freq == 200) {
      if ((headerp_p->time%2)!=0) {
	if (headerp_p->sample_nr == 199999488) {
	  headerp_p->time -= 1;
	} else {
	  headerp_p->sample_nr += 512;
	  oddSecond = true;
	};    
      } else if (oddSecond && (headerp_p->sample_nr == 199998464)) {
	headerp_p->time -= 1;
	headerp_p->sample_nr += 512;
      } else {
	oddSecond = false;
      };
    } else if (headerp_p->sample_freq == 160) {
      if (headerp_p->sample_nr == 159998976) {
	headerp_p->time -= 1;
      };
    } else {
      cerr << "fixDate: Unsupported samplerate!!!" << endl;
    };
  };

  void TBB::fixDateNew(){
    if (headerp_p->sample_freq == 200) {
      if ((headerp_p->time%2)!=1) {
	headerp_p->sample_nr += 512;
      };
    } else if (headerp_p->sample_freq == 160) {
    } else {
      cerr << "fixDate: Unsupported samplerate!!!" << endl;
    };
  };
  
  // -------------------------------------------- processTransientSocketDataBlock
  
  bool TBB::processTransientSocketDataBlock()
  {
    short * sdata;
    
		// just for debug info: block nr.
		static uint blocknr = 0;
		
    //set sdata to the (hopefully correct) position in the udp-buffer
    char *tmpptr = udpBuff_p+sizeof(TBB_Header);
    sdata = (short *)(tmpptr);
    if (rr < (int)(headerp_p->n_samples_per_frame*sizeof(short)+sizeof(TBB_Header)) )
      {
	cerr << "processTransientSocketDataBlock: Too few data read in! Aborting." << endl;
	cerr << "  block size: " << rr << " bytes, estimated size: " 
	     << (headerp_p->n_samples_per_frame*sizeof(short)+sizeof(TBB_Header))
	     << " bytes" << endl;
	printRawHeader();
	return FAIL;
      };
    
    if ( bigendian_p ) 
      {
	for ( short zz=0; zz < headerp_p->n_samples_per_frame; zz++ ) 
	  {
	    swapbytes( (char *)&(sdata[zz]), 2 );
	  };
      };
    
    //calculate the writeOffset from time of first block and this block
    uint starttime, startsamplenum;
    dipoleArray_p->getAttribute( attribute_name(TIME), starttime );
    dipoleArray_p->getAttribute( attribute_name(SAMPLE_NUMBER), startsamplenum );
    int writeOffset= (headerp_p->time-starttime)*headerp_p->sample_freq*1000000 + 
      (headerp_p->sample_nr-startsamplenum);
#ifdef DEBUGGING_MESSAGES
    uint sid, rsp, rcu;
    dipoleArray_p->getAttribute( attribute_name(STATION_ID), sid );
    dipoleArray_p->getAttribute( attribute_name(RSP_ID), rsp );
    dipoleArray_p->getAttribute( attribute_name(RCU_ID), rcu );
    std::cout << "Station: " << sid << " RSP: " << rsp << " RCU: " << rcu 
	      << " Sequence-Nr: " << headerp_p->seqnr << endl;
    std::cout << " starttime:"<< starttime << " startsamplenum:" << startsamplenum 
	      << " writeOffset:" << writeOffset << endl; 
#endif
    //only write data if this block comes after the first block
    // (don't extend the array to the front)
    if (writeOffset >= 0)
      {
	//extend array if neccessary.
	if ((writeOffset+ headerp_p->n_samples_per_frame)> dims[0])
	  {  
#ifdef DEBUGGING_MESSAGES
cout << "extending array to:" << writeOffset+ headerp_p->n_samples_per_frame 
     << " from:" << dims << endl;
#endif
	    dims[0] = writeOffset+ headerp_p->n_samples_per_frame;
	    dipoleArray_p->extend(dims);
	  };
	dipoleArray_p->write(writeOffset, sdata, headerp_p->n_samples_per_frame );
	offset_p = dims[0];
#ifdef DEBUGGING_MESSAGES
      }
    else
      {
	std::cout << "Block seq-nr: " << headerp_p->seqnr << " has negative write offset." 
		  << " Block discarded!" << endl;
#endif    
      };
 
    tmpptr += headerp_p->n_samples_per_frame*sizeof(short);
    UInt32 *payloadp = (UInt32 *)tmpptr;
    payload_crc = *payloadp;
    
		// debug info
		blocknr++;
		
    return true;
  }

  void TBB::discardFileBytes(uint bytes)
  {
    char discardbyte = 0;
    for(uint n = 0; n < bytes; n++)
    {
      rawfile_p->read(&discardbyte, 1);
    }
  }
  // ---------------------------------------------- processTransientFileDataBlock
  
  void TBB::processTransientFileDataBlock()
  {
    short sdata[ headerp_p->n_samples_per_frame];
    for (short zz=0; zz < headerp_p->n_samples_per_frame; zz++ )
      {
        rawfile_p->read( reinterpret_cast<char *>(&tran_sample),
                       sizeof(tran_sample) );

        if ( bigendian_p )  // reverse fields if big endian
          swapbytes( (char *)&tran_sample.value, 2 );

        sdata[zz] = tran_sample.value;
      }
    
    //calculate the writeOffset from time of first block and this block
    uint starttime, startsamplenum;
    dipoleArray_p->getAttribute( attribute_name(TIME), starttime );
    dipoleArray_p->getAttribute( attribute_name(SAMPLE_NUMBER), startsamplenum );
    int writeOffset= (headerp_p->time-starttime)*headerp_p->sample_freq*1000000 + 
      (headerp_p->sample_nr-startsamplenum);
#ifdef DEBUGGING_MESSAGES
    uint sid, rsp, rcu;
    dipoleArray_p->getAttribute( attribute_name(STATION_ID), sid );
    dipoleArray_p->getAttribute( attribute_name(RSP_ID), rsp );
    dipoleArray_p->getAttribute( attribute_name(RCU_ID), rcu );
    std::cout << "Station: " << sid << " RSP: " << rsp << " RCU: " << rcu 
	      << " Sequence-Nr: " << headerp_p->seqnr << endl;
    std::cout << " starttime:"<< starttime << " startsamplenum:" << startsamplenum 
	      << " writeOffset:" << writeOffset << endl; 
#endif

    //only write data if this block comes after the first block
    // (don't extend the array to the front)
    if (writeOffset >= 0)
      {
	//extend array if neccessary.
	if ((writeOffset+ headerp_p->n_samples_per_frame)> dims[0]) 
	  {
	    dims[0] = writeOffset+ headerp_p->n_samples_per_frame;
	    dipoleArray_p->extend(dims);
	  };
	dipoleArray_p->write(writeOffset, sdata, headerp_p->n_samples_per_frame );
	offset_p = dims[0];
#ifdef DEBUGGING_MESSAGES
      }
    else
      {
	std::cout << "Block seq-nr: " << headerp_p->seqnr << " has negative write offset." 
		  << " Block discarded!" << endl;
#endif    
      };
    rawfile_p->read( reinterpret_cast<char *>(&payload_crc),
                   sizeof(payload_crc) );

  }

  // ----------------------------------------------- processSpectralFileDataBlock

  void TBB::processSpectralFileDataBlock()
  {
    complex<Int16> csdata[ headerp_p->n_samples_per_frame];

    for ( short zz=0; zz < headerp_p->n_samples_per_frame; zz++ )
      {
        rawfile_p->read( reinterpret_cast<char *>(&spec_sample),
                       sizeof(spec_sample) );

        real_part = real(spec_sample.value);
        imag_part = imag(spec_sample.value);

        if ( bigendian_p ) // reverse fields if big endian
          {
            swapbytes( (char *)&real_part, 2 );
            swapbytes( (char *)&imag_part, 2 );
          }

        csdata[zz] = complex<Int16>( real_part, imag_part );
      }

    dims[0] += headerp_p->n_samples_per_frame;
    dipoleArray_p->extend(dims);
    dipoleArray_p->write(offset_p, csdata, headerp_p->n_samples_per_frame );
    offset_p += headerp_p->n_samples_per_frame;
    
    rawfile_p->read( reinterpret_cast<char *>(&payload_crc),
		     sizeof(payload_crc) );
    
  }
  
  //_____________________________________________________________________________
  // Set the antenna array position metadata from calibration file
  
#ifdef HAVE_BLITZ
  /*!
    \param infile -- Calibration file containing the antenna array positions
    \param name   -- Name of the group of antenna array positions
  */
  bool TBB::writeAntennaArrayPositions (std::string const &infile,
					std::string const &name)
  {
    bool status (true);
    std::ifstream m_file;
    std::string m_name (name);
    blitz::Array<double,1> m_geoloc;
    blitz::Array<double,3> m_positions;

    /* Open calibration file with positions */
    m_file.open(infile.c_str());
    
    /* Check status of file stream */
    if (!m_file.good()) {
      m_file.close();
      return false;
    }
   
    /* Get the name of the antenna array positions */
    std::getline(m_file, m_name);
    if ("" == m_name) {
      m_file.close();
      return false;
    }

    /* Get geographical location: 1-d array with 3 elements */
    try {
      m_file >> m_geoloc;
    } catch (std::string message) {
      std::cerr << "[TBB::writeAntennaArrayPositions] " << message << std::endl;
      return false;
    }
    
    if ((1 != m_geoloc.dimensions()) || (3 != m_geoloc.extent(blitz::firstDim))) {
      std::cerr << "[TBB::writeAntennaArrayPositions] "
		<< "Wrong shape of array storing geographic location!" << std::endl;
      return false;
    }

    /* Get the antenna positions */
    
    try {
      m_file >> m_positions;
      m_file.ignore(80,'\n');
    } catch (std::string message) {
      std::cerr << "[TBB::writeAntennaArrayPositions] " << message << std::endl;
      return false;
    }
    
    return status;
  }
#endif  

  void TBB::cleanup()
  {
      stationGroup_p->close();
      delete stationGroup_p;
      stationGroup_p = NULL;

      dipoleArray_p->close();
      delete dipoleArray_p;
      dipoleArray_p = NULL;
  }

} // end namespace DAL
