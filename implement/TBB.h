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

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifdef HAVE_BLITZ
#include <blitz/array.h>
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
#include <fstream>

#define ETHEREAL_HEADER_LENGTH = 46;
#define FIRST_EXTRA_HDR_LENGTH = 40;
#define EXTRA_HDR_LENGTH = 16;

namespace DAL {
  
  /*!
    \class TBB
    \ingroup DAL
    \brief High-level interface between TBB data and the DAL
    \author Joseph Masters
  */
  class TBB
  {
    
    ///////////////////////////////////////////////////////////////
    //
    // constant values and structures
    //
    ///////////////////////////////////////////////////////////////
    
    //! Structure for the storage of data for an individual antenna
    struct AntennaStruct
    {
      //! Counter for the frame number
      unsigned int frameno;
      //! Identifier for the RSP board
      unsigned int rsp_id;
      //! Identifier for the RCU board
      unsigned int rcu_id;
      //! Time for the first sample of the dipole dataset
      unsigned int time;
      //! Offset in number of samples
      unsigned int sample_nr;
      //! Number of samples sent within a frame of data
      unsigned int samples_per_frame;
      //! Type of antenna feed (dipole type)
      char feed[16];
      //! Antenna position
      double ant_position[ 3 ];
      //! Antenna orientation
      double ant_orientation[ 3 ];
      //! The data themselves
      hvl_t data[1];
    };
    
    //! Structure storing the buffer which is getting written to output
    struct writebuffer {
      AntennaStruct antenna;
    };
    
    //! Sample when using TBB in time-series mode
    struct TransientSample {
      Int16 value;
    };
    
    //! Sample when using TBB in spectral mode
    struct SpectralSample {
      complex<Int16> value;
    };
    
    //! Structure storing metadata stored in the TBB header block
    struct TBB_Header {
      //! The identifier to the station
      unsigned char stationid;
      //! The identifier for the RSP board
      unsigned char rspid;
      //! The identifier for the RCU board
      unsigned char rcuid;
      //! The sample frequency in the A/D conversion
      unsigned char sample_freq;
      UInt32 seqnr;
      Int32 time;
      UInt32 sample_nr;
      //! The number of samples per frame
      UInt16 n_samples_per_frame;
      UInt16 n_freq_bands;
      char bandsel[64];
      Int16 spare;
      UInt16 crc;
    };
    
    //! Status tracking
    int status;
    UInt32 seqnrLast_p;
    //! Is the system big-endian?
    bool bigendian_p;
    time_t sampleTime_p;  // For date
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
    vector<string> stations;
    dalGroup * stationGroup_p;
    dalArray * dipoleArray_p;
    vector<string> dipoles;
    //! Definition of array dimensions (shape)
    vector<int> dims;
    int offset_p;
    vector<int> cdims;
    //! Name of the HDF5 group storing data for a station
    char * stationstr;
    //! Unique identifier for an individual dipole
    char uid[10];
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
    
    //! Constructor
    TBB (string const& name);
    //! Destructor
    ~TBB();
    //! Set up the socket connection to the server
    void connectsocket( const char* ipaddress,
			const char* portnumber );
    //! Open file containing data resulting from a TBB dump
    bool openRawFile( const char* filename );
    bool readRawSocketBlockHeader();
    void readRawFileBlockHeader();
    //! Print the contents of a raw TBB frame header
    void printRawHeader();
    //! Check if the group for a given station exists within the HDF5 file
    void stationCheck();
    void makeOutputHeader();
    bool transientMode();
    bool processTransientSocketDataBlock();
    bool processSpectralSocketDataBlock();
    void processTransientFileDataBlock();
    void processSpectralFileDataBlock();
    //! Check for the end-of-file
    bool eof();

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

#ifdef HAVE_BLITZ
    //! Set the antenna array position metadata from calibration file
    bool writeAntennaArrayPositions (std::string const &infile,
				     std::string const &name);
#endif

  private:
    
    void init_TBB_Header ();
    
  }; // class TBB
  
} // DAL namespace


#endif // TBB_H
