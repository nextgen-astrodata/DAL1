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

  // ------------------------------------------------------------------------ TBB

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

  // ------------------------------------------------------------------------ TBB

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
  }
  

  // ----------------------------------------------------------------------- init

  void TBB::init ()
  {
    /* Initialization of private data */

    telescope_p        = "LOFAR";
    observer_p         = "UNDEFINED";
    project_p          = "UNDEFINED";
    observation_id_p   = "UNDEFINED";
    observationMode_p  = "UNDEFINED";  

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

    /* Initialization of public data */

    first_sample = true;

    /* Initialization of structs */

    init_TBB_Header ();
  }
  
  // ------------------------------------------------------------ init_TBB_Header
  
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
    if (rawfile)
      {
        if (rawfile->is_open())
          {
            rawfile->close();
          }
        delete rawfile;
        rawfile = 0;
      }
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________

  /*!
    \param os -- Output stream to which the summary is going to be written
  */
  void TBB::summary (std::ostream &os)
  {
    os << "[TBB] Summary of object properties"         << endl;
    os << "-- Last sequence number : " << seqnrLast_p  << endl;
    os << "-- System is big endian : " << bigendian_p  << endl;
    os << "-- Sample time ........ : " << sampleTime_p << endl;
    os << "-- Attributes attached to file root group:"      << endl;
    os << "   -- TELESCOPE        = " << telescope()        << endl;
    os << "   -- OBSERVER         = " << observer()         << endl;
    os << "   -- PROJECT          = " << project()          << endl;
    os << "   -- OBSERVATION_ID   = " << observation_id()   << endl;
    os << "   -- OBSERVATION_MODE = " << observation_mode() << endl;
  }
  
  //_____________________________________________________________________________

  /*!
    \param ipaddress  -- IP number to which to connect
    \param portnumber -- Portnumber to which to connect
    \param timeout    -- Timeout before stopping to listen for data; if a value
           greater zero is provided, the value is the time-out in seconds --
	   otherwise the connection is kept open indefinitely.
  */
  void TBB::connectsocket( const char* ipaddress,
			   const char* portnumber,
			   const int &timeout )
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
    
    if (timeout > 0) {
      timeVal.tv_sec  = timeout;
      timeVal.tv_usec = 0;
      select( main_socket + 1, &readSet, NULL, NULL, &timeVal );
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

  //_____________________________________________________________________________
  // Read data from a socket

  /*!
    \param nbytes -- Number of Bytes read
    \param buf    -- Buffer holding the read data
  */
  int TBB::readsocket( unsigned int nbytes,
		       char* buf )
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

  //_____________________________________________________________________________
  // Read the 88-byte TBB frame header

  bool TBB::readRawSocketBlockHeader()
  {
    status = readsocket( sizeof(header), reinterpret_cast<char *>(&header) );
    if (FAIL == status) {
      return false;
    }
    
    // reverse fields if big endian
    if ( bigendian_p )
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
    sampleTime_p =
      (time_t)( header.time + header.sample_nr/(header.sample_freq*1000000) );
    char *time_string=ctime(&sampleTime_p);
    time_string[strlen(time_string)-1]=0;   // remove \n

#ifdef DEBUGGING_MESSAGES
    printf("Time:              : %s\n",time_string );
    printRawHeader();
#endif

    if ( !first_sample )
      {
        if ( ( seqnrLast_p > header.seqnr ) || ( seqnrLast_p+1 != header.seqnr ) )
          {
            printf("WARNING: Frame missing or out of order\n");
            return false;
          }
      }
    seqnrLast_p = header.seqnr;

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
    rawfile->read(reinterpret_cast<char *>(&header), sizeof(header));

    // reverse fields if big endian
    if ( bigendian_p )
      {
        swapbytes( (char *)&header.seqnr, 4 );
        swapbytes( (char *)&header.sample_nr, 4 );
        swapbytes( (char *)&header.n_samples_per_frame, 8);
        swapbytes( (char *)&header.n_freq_bands, 2 );
      }

    /*
     * Convert the time (seconds since 1 Jan 1970) to a human-readable string
     */
    sampleTime_p =
      (time_t)( header.time + header.sample_nr/(header.sample_freq*1000000) );
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
    if ( rawfile->peek() == EOF )
      return true;
    else
      return false;
  }

  //_____________________________________________________________________________
  // Print the contents of a raw TBB frame header
  
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
  
  //_____________________________________________________________________________
  // Check if the group for a given station exists within the HDF5 file
  
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
            offset_p     = dims[0];
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
    
    unsigned int sid = header.stationid;
    unsigned int rsp = header.rspid;
    unsigned int rcu = header.rcuid;
    double sf = header.sample_freq;
    unsigned int time                    = header.time;
    unsigned int samp_num                = header.sample_nr;
    unsigned int samples_per_frame       = header.n_samples_per_frame;
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

        if ( bigendian_p )  // reverse fields if big endian
          swapbytes( (char *)&tran_sample.value, 2 );

        sdata[zz] = tran_sample.value;
      }

    dims[0] += header.n_samples_per_frame;
    dipoleArray_p->extend(dims);
    dipoleArray_p->write(offset_p, sdata, header.n_samples_per_frame );
    offset_p += header.n_samples_per_frame;

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

        if ( bigendian_p )  // reverse fields if big endian
          swapbytes( (char *)&tran_sample.value, 2 );

        sdata[zz] = tran_sample.value;
      }

    dims[0] += header.n_samples_per_frame;
    dipoleArray_p->extend(dims);
    dipoleArray_p->write(offset_p, sdata, header.n_samples_per_frame );
    offset_p += header.n_samples_per_frame;

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

        if ( bigendian_p ) // reverse fields if big endian
          {
            swapbytes( (char *)&real_part, 2 );
            swapbytes( (char *)&imag_part, 2 );
          }

        csdata[zz] = complex<Int16>( real_part, imag_part );
      }

    dims[0] += header.n_samples_per_frame;
    dipoleArray_p->extend(dims);
    dipoleArray_p->write(offset_p, csdata, header.n_samples_per_frame );
    offset_p += header.n_samples_per_frame;

    rawfile->read( reinterpret_cast<char *>(&payload_crc),
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

} // end namespace DAL
