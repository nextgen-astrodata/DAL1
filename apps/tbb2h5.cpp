/***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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

/*!
  \file tbb2h5.cpp
  
  \ingroup DAL

  \brief Write TBB time-series data into an HDF5 file.

  \author Joseph Masters

  <h4>Usage</h4>

  In order to write the data distributed over a set of dumps from raw TBB
  data into a HDF5 time-series dataset:
  \verbatim
  tbb2h5 <outfile> <infile1 infile2 ...>
  \endverbatim
  where 
  <ul>
    <li><tt>outfile</tt> is the name of the created HDF5 output file
    <li><tt>infile1 infile2 ...</tt> is a list of input files containing the
    dump of the raw TBB data
  </ul>
*/

// socket headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

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



bool it_exists( vector<string> vec, string name )
{
  for( unsigned int ss = 0; ss < vec.size(); ss++ )
  {
    if ( name == vec[ss] )
      return true;
  }
  return false;
}

template <class T>
void list_vector_members( vector<T> vec )
{
  for( unsigned int ss = 0; ss < vec.size(); ss++ )
  {
     cerr << vec[ss] << endl;
  }
}

int main(int argc, char *argv[])
{

  // parameter check
  if ( argc < 4 )
  {
     cout << endl << "Too few parameters..." << endl << endl;
     cout << "The first parameter is the output dataset name.\n";
     cout << "The second parameter is the IP address to accept data from.\n";
     cout << "The third parameter is the port number to accept data from.\n";
     cout << endl;
     return DAL::FAIL;
  }

// ------------------------------------------------------------- 
//
//  Set up the socket connection to the server
//
// ------------------------------------------------------------- 

  int port_number = atol(argv[3]);
  const char * remote = argv[2];
  int remote_port = atol(argv[3]);
  fd_set readSet;
  struct timeval timeVal;

  // Step 1 Look up server to get numeric IP address
  hostent * record = gethostbyname(remote);
  if (record==NULL) { herror("gethostbyname failed"); exit(1); }
  in_addr * addressptr = (in_addr *) record->h_addr;

  // Step 2 Create a socket
  int main_socket = socket(PF_INET, SOCK_DGRAM, 0);
  if (main_socket<0) { perror("socket creation"); exit(1); }

  // Step 3 Create a sockaddr_in to describe the local port
  sockaddr_in local_info;
  local_info.sin_family = AF_INET;
  local_info.sin_addr.s_addr = htonl(INADDR_ANY);
  local_info.sin_port = htons(port_number);

  // Step 4 Bind the socket to the port
  int r = bind(main_socket, (sockaddr *) &local_info, sizeof(local_info));
  if (r<0) { perror("bind"); exit(1); }
  printf("ready\n");

  // Step 5 Create a sockaddr_in to describe the remote application
  sockaddr_in remote_info;
  remote_info.sin_family = AF_INET;
  remote_info.sin_addr = *addressptr;
  remote_info.sin_port = htons(remote_port);


// ------------------------------------------------------------- 
//
//  Create an hdf5 output file
//
// ------------------------------------------------------------- 


  dalDataset * dataset;

  dataset = new dalDataset();

  UInt32 payload_crc;

  dalGroup * stationGroup;

  vector<string> stations;
  vector<string> dipoles;

  bool first_sample = true;

  dalArray * dipoleArray;
  vector<int> cdims;
  cdims.push_back(CHUNK_SIZE);

  // define dimensions of array
  vector<int> dims;
  dims.push_back(0);

   if ( DAL::FAIL == dataset->open( argv[1] ) )
   {
      cerr << "Creating new dataset " << argv[1] << endl;
      delete dataset;
      dataset = new dalDataset( argv[1], "HDF5" );
   }
   else
   {
       cerr << "Dataset " << argv[1] << " exists." << endl;

       stations = dataset->getGroupNames();
       for( unsigned int ss = 0; ss < stations.size(); ss++ )
       {
          cerr << stations[ss] << endl;
       }

   }

// ------------------------------------------------------------- 
//
//  Wait for and receive data
//
// ------------------------------------------------------------- 


  bool bigendian = BigEndian();

  struct sockaddr_in incoming_info;
  unsigned int socklen = sizeof(incoming_info);

  TransientSample tran_sample;
  SpectralSample spec_sample;
  TBB_Header header;
  int counter=0;
  int offset=0;

  while ( true )
  {

    counter++;

    // ------------------------------------------------------  read the header 

    //
    // read 88-byte TBB frame header
    //
    // wait for up to 2 seconds for data appearing in the socket

    FD_ZERO(&readSet);
    FD_SET(main_socket, &readSet);

    timeVal.tv_sec =   10;
    timeVal.tv_usec =  0;

    if ( select( main_socket + 1, &readSet, NULL, NULL, &timeVal ) ) {
      r = recvfrom( main_socket, reinterpret_cast<char *>(&header),
                    sizeof(header), 0,
                    (sockaddr *) &incoming_info, &socklen);
    }
    else
    {
       cout << "Data stopped coming" << endl;
       close(main_socket);
       delete dataset;
       return DAL::SUCCESS;
    }

    if (r<0) { perror("recvfrom"); exit(1); }

    #ifdef DEBUGGING_MESSAGES
    cerr << r << " bytes received." << endl;
    #endif

    // reverse fields if big endian
    if ( bigendian )
      {
	  header.seqnr = Int32Swap( header.seqnr );
	  header.sample_nr = Int32Swap( header.sample_nr );
	  header.n_samples_per_frame = 
	  Int16Swap( header.n_samples_per_frame);
	  header.n_freq_bands = Int16Swap( header.n_freq_bands );
      }

    char * stationstr = new char[10];
    sprintf( stationstr, "Station%03d", header.stationid );
    char uid[10];  // dipole identifier

    // does the station exist?
    if ( it_exists( stations, stationstr ) )
    {
       stationGroup = dataset->openGroup( stationstr );
       dipoles = stationGroup->getMemberNames(); 
       sprintf(uid, "%03d%03d%03d",
               header.stationid, header.rspid, header.rcuid);

       // does the dipole exist?
       if ( it_exists( dipoles, uid ) )
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
       delete [] stationstr;
       sprintf(uid, "%03d%03d%03d",
               header.stationid, header.rspid, header.rcuid);
    }

     // if this is the first sample for this station set header attributes
     if ( first_sample )
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
	  string observer           = "J.S. Masters";
	  string project            = "Transients";
	  string observation_id     = "1287";
	  string observation_mode   = "TransientDetection";
	  string trigger_type       = "Unknown";
	  double trigger_offset[1]  = { 0 };
	  int triggered_antennas[1] = { 0 };
	  double beam_direction[2]  = { 0, 90 };

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

	first_sample = false;
      }  // end if first sample

    short sdata[ header.n_samples_per_frame];

    // ------------------------------------------------------  read the data 

    if ( 0==header.n_freq_bands )  // Transient-mode
    {
      for ( short zz=0; zz < header.n_samples_per_frame; zz++ )
      {

        // wait for up to 2 seconds for data appearing in the socket
        FD_ZERO(&readSet);
        FD_SET(main_socket, &readSet);

        timeVal.tv_sec = 2;
        timeVal.tv_usec = 0;

        // Read Payload
        if ( select( main_socket + 1, &readSet, NULL, NULL, &timeVal ) ) {
          r = recvfrom( main_socket, reinterpret_cast<char *>(&tran_sample),
                        sizeof(tran_sample), 0,
                        (sockaddr *) &incoming_info, &socklen );
        }
        else
        {
           cout << "Data stopped coming" << endl;
           close(main_socket);
           delete dataset;
           return DAL::FAIL;
        }

        if ( r < 0 ) { perror("recvfrom"); exit(1); }

        #ifdef DEBUGGING_MESSAGES
        cerr << r << " bytes received." << endl;
        #endif

        if ( 0 == counter % 10000 )
           cout << counter << " value         " << tran_sample.value << endl;

        if ( bigendian )  // reverse fields if big endian
          tran_sample.value = Int16Swap( tran_sample.value );

        sdata[zz] = tran_sample.value;
      }

      dims[0] += header.n_samples_per_frame;
      dipoleArray->extend(dims);
      int arraysize = header.n_samples_per_frame;
      dipoleArray->write(offset, sdata, arraysize );
      offset += header.n_samples_per_frame;

    }  // end if transient mode

    r=recvfrom( main_socket, reinterpret_cast<char *>(&payload_crc),
                sizeof(payload_crc), 0,
                (sockaddr *) &incoming_info, &socklen );

  } // end while (true)

  // Step 8 Close the socket and exit
  close(main_socket);

  delete dataset;

  cout << "SUCCESS" << endl;
  return DAL::SUCCESS;

} // end of main()
