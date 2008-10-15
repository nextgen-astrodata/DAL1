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

  /*!
    \brief Argumented constructor

    \param filename -- Name of the output HDF5 dataset to create
  */
  TBB::TBB( string const& filename )
  {
    // initializations (private)
    seqnrLast   = 0;
    bigendian   = BigEndian();
    sample_time = (time_t)0;
    name        = filename;
    dataset     = new dalDataset;
    station.clear();
    rr = 0;
    main_socket = -1;
    socklen = sizeof(incoming_addr);
    status = 0;
    stations.clear();
    stationGroup_p = NULL;
    dipoleArray_p  = NULL;
    dipoles.clear();
    dims.push_back(0);
    offset = 0;
    cdims.push_back(CHUNK_SIZE);
    stationstr = NULL;
    memset(uid,'-',10);
    payload_crc       = 0;
    tran_sample.value = 0;
    spec_sample.value = 0;
    size              = 0;
    memblock          = NULL;
    rawfile           = NULL;
    real_part         = 0;
    imag_part         = 0;
    /* Initialize the TBB_Header struct */
    init_TBB_Header ();
    
    // initializations (public)
    first_sample = true;

    if ( FAIL == dataset->open( filename.c_str() ) )
      {
        cerr << "Creating new dataset " << filename << endl;
        delete dataset;
        dataset = new dalDataset( filename.c_str(), "HDF5" );

        string telescope          = "LOFAR";
        string observer           = "UNDEFINED";
        string project            = "UNDEFINED";
        string observation_id     = "UNDEFINED";
        string observation_mode   = "UNDEFINED";
        dataset->setAttribute( attribute_name(TELESCOPE),
			       telescope );
        dataset->setAttribute( attribute_name(OBSERVER),
			       observer );
        dataset->setAttribute( attribute_name(PROJECT),
			       project );
        dataset->setAttribute( attribute_name(OBSERVATION_ID),
                               observation_id );
        dataset->setAttribute( attribute_name(OBSERVATION_MODE),
                               observation_mode );

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

  // ------------------------------------------------------------ init_TBB_Header
  
  /*!
    \brief Initialize the values within the TBB_Header struct
  */
  void TBB::init_TBB_Header ()
  {
    header.stationid           = 0;
    header.rspid               = 0;
    header.rcuid               = 0;
    header.sample_freq         = 0;
    header.seqnr               = 0;
    header.time                = 0;
    header.sample_nr           = 0;
    header.n_samples_per_frame = 0;
    header.n_freq_bands        = 0;
    header.spare               = 0;
    header.crc                 = 0;
    memset(header.bandsel,0,64);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  /*!
    \brief Destructor
  */
  TBB::~TBB()
  {
    if ( dipoleArray_p )
      {
        delete dipoleArray_p;
        dipoleArray_p = NULL;
      }
    if ( stationGroup_p )
      {
        delete stationGroup_p;
        stationGroup_p = NULL;
      }
    delete dataset;
    if (main_socket)
      close(main_socket);
    if (rawfile)
      {
        if (rawfile->is_open())
          {
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
  
  /*!
    \brief Set up the socket connection to the server

    \brief ipaddress  -- IP number to which to connect
    \brief portnumber -- Portnumber to which to connect
  */
  void TBB::connectsocket( const char* ipaddress,
			   const char* portnumber )
  {
    int port_number = atol( portnumber );
    const char * remote = ipaddress;

    // Step 1 Look up server to get numeric IP address
    hostent * record = gethostbyname(remote);
    if (record==NULL)
      {
        herror("gethostbyname failed");
        return;
      }

    // Step 2 Create a socket
    main_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (main_socket<0)
      {
        perror("socket creation");
        return;
      }

    // Step 3 Create a sockaddr_in to describe the local port
    sockaddr_in local_info;
    local_info.sin_family = AF_INET;
    local_info.sin_addr.s_addr = htonl(INADDR_ANY);
    local_info.sin_port = htons(port_number);

    // Step 4 Bind the socket to the port
    int rr = bind(main_socket, (sockaddr *) &local_info, sizeof(local_info));
    if (rr<0)
      {
        perror("bind");
        return;
      }
    printf("ready\n");
  }

  // ---------------------------------------------------------------- openRawFile

  /*!
    \brief Open file containing data resulting from a TBBB dump

    \param filename -- Name of the data file

    \return status -- Status of the operation; returns \e true in case the input
            file could be opened successfully, if an error is encountered
	    \e false is returned.
  */
  bool TBB::openRawFile( const char* filename )
  {
    delete rawfile;
    rawfile = new fstream( filename, ios::binary|ios::in );
    if ( !rawfile->is_open() )
      {
        std::cerr << "Error opening intput file: " << std::string(filename)
                  << std::endl;
        return false;
      }
    else
      {
        rawfile->seekg (0, ios::beg);  // move to start of file
        return true;
      }
  }

  int TBB::readsocket( unsigned int nbytes, char* buf )
  {
    FD_ZERO(&readSet);
    FD_SET(main_socket, &readSet);

    timeVal.tv_sec =   3;
    timeVal.tv_usec =  0;

    // waits for up to N seconds for data appearing in the socket
    if ( select( main_socket + 1, &readSet, NULL, NULL, &timeVal ) )
      {
        rr = recvfrom( main_socket, buf, nbytes, 0,
                       (sockaddr *) &incoming_addr, &socklen);
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

  bool TBB::readRawSocketBlockHeader()
  {
    // ------------------------------------------------------  read the header
    //
    // read 88-byte TBB frame header
    //
    status = readsocket( sizeof(header), reinterpret_cast<char *>(&header) );
    if (FAIL == status)
      return false;

    // reverse fields if big endian
    if ( bigendian )
      {
        swapbytes( (char *)&header.seqnr, 4 );
        swapbytes( (char *)&header.sample_nr, 4 );
        swapbytes( (char *)&header.n_samples_per_frame, 8);
        swapbytes( (char *)&header.n_freq_bands, 2 );
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
    sample_time =
      (time_t)( header.time + header.sample_nr/(header.sample_freq*1000000) );
    char *time_string=ctime(&sample_time);
    time_string[strlen(time_string)-1]=0;   // remove \n

#ifdef DEBUGGING_MESSAGES
    printf("Time:              : %s\n",time_string );
    printRawHeader();
#endif

    if ( !first_sample )
      {
        if ( ( seqnrLast > header.seqnr ) || ( seqnrLast+1 != header.seqnr ) )
          {
            printf("WARNING: Frame missing or out of order\n");
            return false;
          }
      }
    seqnrLast = header.seqnr;

    return true;
  }

  // ------------------------------------------------------  read the header
  //
  // read 88-byte TBB frame header
  //

  void TBB::readRawFileBlockHeader()
  {
    rawfile->read(reinterpret_cast<char *>(&header), sizeof(header));

    // reverse fields if big endian
    if ( bigendian )
      {
        swapbytes( (char *)&header.seqnr, 4 );
        swapbytes( (char *)&header.sample_nr, 4 );
        swapbytes( (char *)&header.n_samples_per_frame, 8);
        swapbytes( (char *)&header.n_freq_bands, 2 );
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
    sample_time =
      (time_t)( header.time + header.sample_nr/(header.sample_freq*1000000) );
    char *time_string=ctime(&sample_time);
    time_string[strlen(time_string)-1]=0;   // remove \n

#ifdef DEBUGGING_MESSAGES
    printf("Time:              : %s\n",time_string );
    printRawHeader();
#endif

  }

  // ------------------------------------------------------------------------ eof

  bool TBB::eof()
  {
    if ( rawfile->peek() == EOF )
      return true;
    else
      return false;
  }

  // ------------------------------------------------------------- printRawHeader
  
  void TBB::printRawHeader()
  {
    printf("Station ID         : %d\n",header.stationid);
    printf("RSP ID             : %d\n",header.rspid);
    printf("RCU ID             : %d\n",header.rcuid);
    printf("Sample Freqency    : %d MHz\n",header.sample_freq);
    printf("Sequence Number    : %d\n",header.seqnr);
    printf("Sample Number      : %d\n",header.sample_nr);
    printf("Samples Per Frame  : %d\n",header.n_samples_per_frame);
    printf("Num. of Freq. Bands: %d\n",header.n_freq_bands);
    printf("Bands present : ");
    for (int idx=0; idx<64; idx++)
      printf("%X,", header.bandsel[idx]);
    printf("\n");
  }
  
  // --------------------------------------------------------------- stationCheck
  
  void TBB::stationCheck()
  {
    char stationstr[10];
    memset(stationstr,'\0',10);
    sprintf( stationstr, "Station%03d", header.stationid );
    sprintf(uid, "%03d%03d%03d",
	    header.stationid, header.rspid, header.rcuid);
    
    /* Use std::string for the name of the station group */
    std::string channelID (uid);
    
    // does the station exist?
    if ( it_exists( stations, stringify(stationstr) ) )
      {
        stationGroup_p = dataset->openGroup( stationstr );
        dipoles = stationGroup_p->getMemberNames();
        sprintf(uid, "%03d%03d%03d",
                header.stationid, header.rspid, header.rcuid);

        // does the dipole exist?
        if ( it_exists( dipoles, stringify(uid) ) )
          {
            dipoleArray_p  = dataset->openArray( uid, stationGroup_p->getName() );
            dims         = dipoleArray_p->dims();
            offset       = dims[0];
            first_sample = false;
          }
        else
          {
            sprintf(uid, "%03d%03d%03d",
                    header.stationid, header.rspid, header.rcuid);
          }
      }
    else
      {
        stations.push_back( stationstr );
        stationGroup_p = dataset->createGroup( stationstr );
	/* Generate channel ID for the individual dipole */
        sprintf(uid, "%03d%03d%03d",
                header.stationid, header.rspid, header.rcuid);
        first_sample = true;
	/* Feedback */
	std::cout << "CREATED New station group: " << stationstr << std::endl;
      }
  }

  // ----------------------------------------------------------- makeOutputHeader

  void TBB::makeOutputHeader()
  {
    vector<int> firstdims;
    firstdims.push_back( 0 );

    if ( 0 == header.n_freq_bands )
      {
        short nodata[0];
        dipoleArray_p = stationGroup_p->createShortArray( string(uid),
                      firstdims,
                      nodata,
                      cdims );
        stationGroup_p->setAttribute( attribute_name(OBSERVATION_MODE),
                                    std::string("Transient") );
      }
    else
      {
        complex<Int16> nodata[0];
        dipoleArray_p =
          stationGroup_p->createComplexShortArray( string(uid),
                                                 firstdims,
                                                 nodata,
                                                 cdims );
        stationGroup_p->setAttribute( attribute_name(OBSERVATION_MODE),
                                    std::string("Sub-band") );
      }

    double trigger_offset[1]  = { 0 };
    int triggered_antennas[1] = { 0 };
    double bdv[2]  = { 0, 90 };
    double spv[3]  = { 0, 0, 0 };
    
    // Add attributes to "Station" group
    stationGroup_p->setAttribute( attribute_name(STATION_POSITION_VALUE),
				  spv, 3 );
    stationGroup_p->setAttribute( attribute_name(STATION_POSITION_UNIT),
				  std::string("m") );
    stationGroup_p->setAttribute( attribute_name(STATION_POSITION_FRAME),
				  std::string("ITRF") );
    stationGroup_p->setAttribute( attribute_name(BEAM_DIRECTION_VALUE),
				  bdv, 2 );
    stationGroup_p->setAttribute( attribute_name(BEAM_DIRECTION_UNIT),
				  std::string("deg") );
    stationGroup_p->setAttribute( attribute_name(BEAM_DIRECTION_FRAME),
				  std::string("UNDEFINED") );
    stationGroup_p->setAttribute( attribute_name(TRIGGER_TYPE),
				  std::string("UNDEFINED") );
    stationGroup_p->setAttribute( attribute_name(TRIGGER_OFFSET),
				  trigger_offset );
    stationGroup_p->setAttribute( attribute_name(TRIGGERED_ANTENNAS),
				  triggered_antennas);
    
    unsigned int sid = header.stationid;
    unsigned int rsp = header.rspid;
    unsigned int rcu = header.rcuid;
    double sf = header.sample_freq;
    unsigned int time = header.time;
    unsigned int samp_num = header.sample_nr;
    unsigned int spf = header.n_samples_per_frame;
    unsigned int nyquist_zone = 1;
    double apos[3]    = { 0, 0, 0 };
    
    dipoleArray_p->setAttribute( attribute_name(STATION_ID), &sid );
    dipoleArray_p->setAttribute( attribute_name(RSP_ID), &rsp );
    dipoleArray_p->setAttribute( attribute_name(RCU_ID), &rcu );
    dipoleArray_p->setAttribute( attribute_name(TIME), &time );
    dipoleArray_p->setAttribute( attribute_name(SAMPLE_NUMBER), &samp_num );
    dipoleArray_p->setAttribute( attribute_name(SAMPLES_PER_FRAME), &spf );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_POSITION_VALUE),
				 apos, 3 );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_POSITION_UNIT),
				 std::string("m") );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_POSITION_FRAME),
				 std::string("ITRF") );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_ORIENTATION_VALUE),
				 apos, 3 );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_ORIENTATION_UNIT),
				 std::string("m") );
    dipoleArray_p->setAttribute( attribute_name(ANTENNA_ORIENTATION_FRAME),
				 std::string("ITRF") );
    dipoleArray_p->setAttribute( attribute_name(FEED), std::string("UNDEFINED") );
    dipoleArray_p->setAttribute( attribute_name(NYQUIST_ZONE),
				 &nyquist_zone );
    dipoleArray_p->setAttribute( attribute_name(SAMPLE_FREQUENCY_VALUE), &sf, 1 );
    dipoleArray_p->setAttribute( attribute_name(SAMPLE_FREQUENCY_UNIT),
				 std::string("MHz") );
  }
  
  // -------------------------------------------------------------- transientMode

  bool TBB::transientMode()
  {
    if ( 0==header.n_freq_bands )
      return true;   // transient mode
    else
      return false;  // spectral mode
  }

  // -------------------------------------------- processTransientSocketDataBlock
  
  bool TBB::processTransientSocketDataBlock()
  {
    short sdata[ header.n_samples_per_frame];
    for ( short zz=0; zz < header.n_samples_per_frame; zz++ )
      {

        status = readsocket( sizeof(tran_sample),
                             reinterpret_cast<char *>(&tran_sample) );
        if (FAIL == status)
          return false;

        if ( bigendian )  // reverse fields if big endian
          swapbytes( (char *)&tran_sample.value, 2 );

        sdata[zz] = tran_sample.value;
      }

    dims[0] += header.n_samples_per_frame;
    dipoleArray_p->extend(dims);
    dipoleArray_p->write(offset, sdata, header.n_samples_per_frame );
    offset += header.n_samples_per_frame;

    status = readsocket( sizeof(payload_crc),
                         reinterpret_cast<char *>(&payload_crc) );
    if (FAIL == status)
      return false;

    return true;
  }

  // ---------------------------------------------- processTransientFileDataBlock

  void TBB::processTransientFileDataBlock()
  {
    short sdata[ header.n_samples_per_frame];

    for ( short zz=0; zz < header.n_samples_per_frame; zz++ )
      {
        rawfile->read( reinterpret_cast<char *>(&tran_sample),
                       sizeof(tran_sample) );

        if ( bigendian )  // reverse fields if big endian
          swapbytes( (char *)&tran_sample.value, 2 );

        sdata[zz] = tran_sample.value;
      }

    dims[0] += header.n_samples_per_frame;
    dipoleArray_p->extend(dims);
    dipoleArray_p->write(offset, sdata, header.n_samples_per_frame );
    offset += header.n_samples_per_frame;

    rawfile->read( reinterpret_cast<char *>(&payload_crc),
                   sizeof(payload_crc) );

  }

  // ----------------------------------------------- processSpectralFileDataBlock

  void TBB::processSpectralFileDataBlock()
  {
    complex<Int16> csdata[ header.n_samples_per_frame];

    for ( short zz=0; zz < header.n_samples_per_frame; zz++ )
      {
        rawfile->read( reinterpret_cast<char *>(&spec_sample),
                       sizeof(spec_sample) );

        real_part = real(spec_sample.value);
        imag_part = imag(spec_sample.value);

        if ( bigendian ) // reverse fields if big endian
          {
            swapbytes( (char *)&real_part, 2 );
            swapbytes( (char *)&imag_part, 2 );
          }

        csdata[zz] = complex<Int16>( real_part, imag_part );
      }

    dims[0] += header.n_samples_per_frame;
    dipoleArray_p->extend(dims);
    dipoleArray_p->write(offset, csdata, header.n_samples_per_frame );
    offset += header.n_samples_per_frame;

    rawfile->read( reinterpret_cast<char *>(&payload_crc),
                   sizeof(payload_crc) );

  }

} // end namespace DAL
