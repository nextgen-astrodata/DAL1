/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
 *   J.S.Masters@uva.nl                                               *
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

  TBB::TBB( string const& filename )
  {
    // initializations (private)
    seqnrLast = 0;
    bigendian = BigEndian();
    sample_time = (time_t)0;
    name = filename;
    dataset = new dalDataset;
    station.clear();
    header.stationid = 0;     // --------------------  TBB_Header
    header.rspid = 0;
    header.rcuid = 0;
    header.sample_freq = 0;
    header.seqnr = 0;
    header.time = 0;
    header.sample_nr = 0;
    header.n_samples_per_frame = 0;
    header.n_freq_bands = 0;
    memset(header.bandsel,0,64);
    header.spare = 0;
    header.crc = 0;
    rr = 0;
    main_socket = -1;
    socklen = sizeof(incoming_addr);
    status = 0;
    stations.clear();
    stationGroup = NULL;
    dipoleArray = NULL;
    dipoles.clear();
    dims.push_back(0);
    offset = 0;
    cdims.push_back(CHUNK_SIZE);
    stationstr = NULL;
    memset(uid,'-',10);
    payload_crc = 0;
    tran_sample.value = 0;
    spec_sample.value = 0;

    // initializations (public)
    first_sample = true;

    if ( DAL::FAIL == dataset->open( filename.c_str() ) )
    {
      cerr << "Creating new dataset " << filename << endl;
      delete dataset;
      dataset = new dalDataset( filename.c_str(), "HDF5" );
    }
    else
    {
       cerr << "Dataset " << filename << " exists." << endl;

       vector<string> stations = dataset->getGroupNames();
       for( unsigned int ss = 0; ss < stations.size(); ss++ )
       {
          cerr << stations[ss] << endl;
       }

    }

  }


  TBB::~TBB()
  {
    if ( dipoleArray )
    {
      delete dipoleArray;
      dipoleArray = NULL;
    }
    if ( stationGroup )
    {
      delete stationGroup;
      stationGroup = NULL;
    }
    delete dataset;
    close(main_socket);
  }

  // ------------------------------------------------------------- 
  //
  //  Set up the socket connection to the server
  //
  // ------------------------------------------------------------- 

  void TBB::connectsocket( char* ipaddress, char* portnumber )
  {
    int port_number = atol( portnumber );
    const char * remote = ipaddress;

    // Step 1 Look up server to get numeric IP address
    hostent * record = gethostbyname(remote);
    if (record==NULL) { herror("gethostbyname failed"); exit(1); }

    // Step 2 Create a socket
    main_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (main_socket<0) { perror("socket creation"); exit(1); }

    // Step 3 Create a sockaddr_in to describe the local port
    sockaddr_in local_info;
    local_info.sin_family = AF_INET;
    local_info.sin_addr.s_addr = htonl(INADDR_ANY);
    local_info.sin_port = htons(port_number);

    // Step 4 Bind the socket to the port
    int rr = bind(main_socket, (sockaddr *) &local_info, sizeof(local_info));
    if (rr<0) { perror("bind"); exit(1); }
    printf("ready\n");
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
       return DAL::FAIL;
    }

    if (rr<0) { perror("recvfrom"); exit(1); }

    return DAL::SUCCESS;
  }

  bool TBB::readRawSocketHeader()
  {
    // ------------------------------------------------------  read the header 
    //
    // read 88-byte TBB frame header
    //
    status = readsocket( sizeof(header), reinterpret_cast<char *>(&header) );
    if (DAL::FAIL == status)
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
         exit(9);
      }
    }
    seqnrLast = header.seqnr;

    return true;
  }

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

  void TBB::stationCheck()
  {
    char stationstr[10];
    memset(stationstr,'\0',10);
    sprintf( stationstr, "Station%03d", header.stationid );

    // does the station exist?
    if ( it_exists_str( stations, stationstr ) )
    {
       stationGroup = dataset->openGroup( stationstr );
       dipoles = stationGroup->getMemberNames();
       sprintf(uid, "%03d%03d%03d",
               header.stationid, header.rspid, header.rcuid);

       // does the dipole exist?
       if ( it_exists_str( dipoles, uid ) )
       {
          dipoleArray = dataset->openArray( uid, stationGroup->getName() );
          dims = dipoleArray->dims();
          offset = dims[0];
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
       stationGroup = dataset->createGroup( stationstr );
       cerr << "CREATED New station group: " << string(stationstr) << endl;
       sprintf(uid, "%03d%03d%03d",
               header.stationid, header.rspid, header.rcuid);
    }
  }

  void TBB::makeOutputHeader()
  {
	if ( 0!=header.n_freq_bands )
	{
          #ifdef DEBUGGING_MESSAGES
          cerr << "Spectral mode." << endl;
          #endif
	}
	else
	{
	  vector<int> firstdims;
	  firstdims.push_back( 0 );
	  short nodata[0];
	  dipoleArray =
	    stationGroup->createShortArray( string(uid),firstdims,nodata,cdims );

	  string telescope          = "LOFAR";
	  string observer           = "";
	  string project            = "";
	  string observation_id     = "";
	  string observation_mode   = "";
	  string trigger_type       = "";
	  double trigger_offset[1]  = { 0 };
	  int triggered_antennas[1] = { 0 };
	  double beam_direction[2]  = { 0, 0 };

	  // Add attributes to "Station" group
	  stationGroup->setAttribute_string("TELESCOPE", telescope );
	  stationGroup->setAttribute_string("OBSERVER", observer );
	  stationGroup->setAttribute_string("PROJECT", project );
	  stationGroup->setAttribute_string("OBS_ID", observation_id );
	  stationGroup->setAttribute_string("OBS_MODE", observation_mode );
	  stationGroup->setAttribute_string("TRIG_TYPE", trigger_type );
	  stationGroup->setAttribute_double("TRIG_OFST", trigger_offset );
	  stationGroup->setAttribute_int(   "TRIG_ANTS", triggered_antennas );
	  stationGroup->setAttribute_double("BEAM_DIR", beam_direction, 2 );

	  unsigned int sid[] = { (unsigned int)(header.stationid) };
	  unsigned int rsp[] = { (unsigned int)(header.rspid) };
	  unsigned int rcu[] = { (unsigned int)(header.rcuid) };
	  double sf[] = { (double)header.sample_freq };
	  unsigned int time[] = { (unsigned int)(header.time) };
	  unsigned int samp_num[] = { (unsigned int)(header.sample_nr) };
	  unsigned int spf[] = { (unsigned int)header.n_samples_per_frame };
	  unsigned int datalen[] = { (unsigned int)0 };
	  unsigned int nyquist_zone[] = { (unsigned int)0 };
	  string feed       = "NONE";
	  double apos[3]    = { 0, 0, 0 };
	  double aorient[3] = { 0, 0, 0 };

	  dipoleArray->setAttribute_uint("STATION_ID", sid );
	  dipoleArray->setAttribute_uint("RSP_ID", rsp );
	  dipoleArray->setAttribute_uint("RCU_ID", rcu );
	  dipoleArray->setAttribute_double("SAMPLE_FREQ", sf );
	  dipoleArray->setAttribute_uint("TIME", time );
	  dipoleArray->setAttribute_uint("SAMPLE_NR", samp_num );
	  dipoleArray->setAttribute_uint("SAMPLES_PER_FRAME", spf );
	  dipoleArray->setAttribute_uint("DATA_LENGTH", datalen );
	  dipoleArray->setAttribute_uint("NYQUIST_ZONE", nyquist_zone );
	  dipoleArray->setAttribute_string("FEED", feed );
	  dipoleArray->setAttribute_double("ANT_POSITION", apos );
	  dipoleArray->setAttribute_double("ANT_ORIENTATION", aorient );
	}
  }

  bool TBB::transientMode()
  {
    if ( 0==header.n_freq_bands )
      return true;   // transient mode
    else
      return false;  // spectral mode
  }

  bool TBB::processTransientSocketDataBlock()
  {
      short sdata[ header.n_samples_per_frame];
      for ( short zz=0; zz < header.n_samples_per_frame; zz++ )
      {

        status = readsocket( sizeof(tran_sample),
                             reinterpret_cast<char *>(&tran_sample) );
        if (DAL::FAIL == status)
          return false;

        if ( bigendian )  // reverse fields if big endian
          swapbytes( (char *)&tran_sample.value, 2 );

        sdata[zz] = tran_sample.value;
      }

      dims[0] += header.n_samples_per_frame;
      dipoleArray->extend(dims);
      dipoleArray->write(offset, sdata, header.n_samples_per_frame );
      offset += header.n_samples_per_frame;

      status = readsocket( sizeof(payload_crc),
                           reinterpret_cast<char *>(&payload_crc) );
      if (DAL::FAIL == status)
         return false;

      return true;
  }

} // end namespace DAL
