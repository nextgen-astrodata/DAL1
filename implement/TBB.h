/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#ifndef TBB_H
#define TBB_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef COMMON_H
#include <Common.h>
#endif

// socket headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace DAL {

  ///////////////////////////////////////////////////////////////
  //
  // constant values and structures
  //
  ///////////////////////////////////////////////////////////////

  const Int32 ETHEREAL_HEADER_LENGTH = 46;
  const Int32 FIRST_EXTRA_HDR_LENGTH = 40;
  const Int32 EXTRA_HDR_LENGTH = 16;

  typedef struct AntennaStruct {
    unsigned int frameno;
    unsigned int rsp_id;
    unsigned int rcu_id;
    unsigned int time;
    unsigned int sample_nr;
    unsigned int samples_per_frame;
    char feed[16];
    double ant_position[ 3 ];
    double ant_orientation[ 3 ];
    hvl_t data[1];
  };

  typedef struct writebuffer {
    AntennaStruct antenna;
  };

  typedef struct TransientSample {
    Int16 value;
  };

  typedef struct SpectralSample {
    complex<Int16> value;
  };

  typedef struct CosmicRayStruct {
    //int nofDatapoints;
    Int16 data;
  };


/*!
   \class TBB

   \ingroup DAL

   \brief High-level interface between TBB data and the DAL

   \author Joseph Masters
 */

  class TBB {

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
  UInt32 seqnrLast;
  bool bigendian;
  time_t sample_time;  // For date
  string name;
  dalDataset * dataset;
  std::vector<dalGroup> station;
  fd_set readSet;
  struct timeval timeVal;
  TBB_Header header;
  int rr;
  int main_socket;
  struct sockaddr_in incoming_addr;
  unsigned int socklen;
  int status;
  vector<string> stations;
  dalGroup * stationGroup;
  dalArray * dipoleArray;
  vector<string> dipoles;
  vector<int> dims; // define dimensions of array
  int offset;
  vector<int> cdims;
  char * stationstr;
  char uid[10];  // dipole identifier
  int readsocket( unsigned int nbytes, char* buf );
  UInt32 payload_crc;
  TransientSample tran_sample;
  SpectralSample spec_sample;
  // for file i/o
  ifstream::pos_type size;
  unsigned char * memblock;
  fstream * rawfile;
  Int16 real_part, imag_part;

  public:

  bool first_sample;

  TBB( string const& name );  // constructor
  ~TBB(); // destructor
  void connectsocket( char* ipaddress, char* portnumber );
  bool openRawFile( char* filename );
  bool readRawSocketBlockHeader();
  void readRawFileBlockHeader();
  void printRawHeader();
  void stationCheck();
  void makeOutputHeader();
  bool transientMode();
  bool processTransientSocketDataBlock();
  bool processSpectralSocketDataBlock();
  void processTransientFileDataBlock();
  void processSpectralFileDataBlock();
  bool eof();

  }; // class TBB

} // DAL namespace


#endif // TBB_H
