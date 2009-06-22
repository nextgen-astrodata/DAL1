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

//!size of the buffer for the UDP-datagram 
// 1 byte larger than the frame size (subband-frames are smaller)
#define UDP_BUFFER_SIZE 2141

namespace DAL {
  
  /*!
    \class TBB

    \ingroup DAL

    \brief High-level interface between TBB data and the DAL

    \author Joseph Masters, Lars B&auml;hren, Andreas Horneffer

    \test tTBB.cpp
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>
    </ul>
    
    <h3>Synopsis</h3>

    
    <h3>Example(s)</h3>
    
  */
  class TBB
  {
    
    //___________________________________________________________________________
    // Constant values and structures
    
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

    //! Name of the output HDF5 file
    string name;
    //! Name of the telescope
    std::string telescope_p;
    //! Name of the observer
    std::string observer_p;
    //! Name of the project
    std::string project_p;
    //! Observation ID
    std::string observation_id_p;
    //! Telescope observation mode
    std::string observationMode_p;
    //! Status tracking
    int status;
    UInt32 seqnrLast_p;
    //! Is the system big-endian?
    bool bigendian_p;
    time_t sampleTime_p;  // For date
    dalDataset * dataset;
    std::vector<dalGroup> station;
    fd_set readSet;
    struct timeval timeoutStart_p;
    struct timeval timeoutRead_p;
    //!buffer for the UDP-datagram
    char udpBuff_p[UDP_BUFFER_SIZE];
    //!header of the TBB-frame 
    TBB_Header header;
    //!header of the TBB-frame 
    TBB_Header *headerp_p;
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
    //char * stationstr;
    //! Unique identifier for an individual dipole
    char uid[10];
    //! Read data from a socket
    int readsocket( unsigned int nbytes,
		    char* buf);
    UInt32 payload_crc;
    TransientSample tran_sample;
    SpectralSample spec_sample;
    // for file i/o
    ifstream::pos_type size;
    unsigned char * memblock;
    fstream * rawfile_p;
    Int16 real_part, imag_part;
    
  public:
    
    bool first_sample;

    //___________________________________________________________________________
    // Construction/Destruction
    
    //! Constructor
    TBB (string const &outfile);
    //! Constructor
    TBB (string const &outfile,
	 string const &telescope,
	 string const &observer,
	 string const &project);
    //! Destructor
    ~TBB();

    //___________________________________________________________________________
    // Parameter access

    /*!
      \brief Get the name of the telescope
      \return telescope -- The name of the telescope 
    */
    inline std::string telescope ()        const { return telescope_p;        }
    /*!
      \brief Get the name of the observer
      \return observer -- The name of the observer 
    */
    inline std::string observer ()         const { return observer_p;         }
    /*!
      \brief Get the name of the project
      \return project -- The name of the project
    */
    inline std::string project ()          const { return project_p;          }
    //! Get the identifier for the observation
    inline std::string observation_id ()   const { return observation_id_p;   }
    //! Set the telescope observation mode
    inline std::string observation_mode () const { return observationMode_p;  }
    
    /*!
      \brief Set the name of the telescope
      \param telescope -- The name of the telescope
    */
    inline void setTelescope (std::string const &telescope) {
      telescope_p = telescope;
    }
    /*!
      \brief Set the name of the observer
      \param observer -- The name of the observer
    */
    inline void setObserver (std::string const &observer) {
      observer_p = observer;
    }
    /*!
      \brief Set the name of the project
      \param project -- The name of the project
    */
    inline void setProject (std::string const &project) {
      project_p = project;
    }
    //! Set the identifier for the observation
    inline void setObservation_id (std::string const &observation_id) {
      observation_id_p = observation_id;
    }
    //! Set the telescope observation mode
    inline void setObservation_mode (std::string const &observation_mode) {
      observationMode_p = observation_mode;
    }

    //! Get the time-out before dropping socket connection at connect
    inline void timeoutStart (unsigned int &time_sec,
			      unsigned int &time_usec)
    {
      time_sec  = timeoutStart_p.tv_sec;
      time_usec = timeoutStart_p.tv_usec;
    }
    //! Set the time-out before dropping socket connection at connect
    void setTimeoutStart (double const &timeout=-1.0);
    
    //! Set the time-out before dropping socket connection at connect
    inline void setTimeoutStart (unsigned int const &time_sec,
				 unsigned int const &time_usec)
    {
      timeoutStart_p.tv_sec  = time_sec;
      timeoutStart_p.tv_usec = time_usec;
    }

    /*!
      \brief Get the time-out before dropping socket connection while reading
      \retval time_sec  -- 
      \retval time_usec -- 
    */
    inline void timeoutRead (unsigned int &time_sec,
			     unsigned int &time_usec)
    {
      time_sec  = timeoutRead_p.tv_sec;
      time_usec = timeoutRead_p.tv_usec;
    }
    
    //! Set the time-out before dropping socket connection while reading
    void setTimeoutRead (double const &timeout=1.0);
    
    //! Set the time-out before dropping socket connection while reading
    inline void setTimeoutRead (unsigned int const &time_sec,
				unsigned int const &time_usec)
    {
      timeoutRead_p.tv_sec  = time_sec;
      timeoutRead_p.tv_usec = time_usec;
    }
    
    //___________________________________________________________________________
    // Methods

    //! Set up the socket connection to the server
    void connectsocket( const char* ipaddress,
			const char* portnumber);
    //! Open file containing data resulting from a TBB dump
    bool openRawFile( const char* filename );
    bool readRawSocketBlockHeader();
    void readRawFileBlockHeader();
    //! Print the contents of a raw TBB frame header
    void printRawHeader();
    //! Check the CRC of a TBB frame header
    uint16_t CRC16(uint16_t * buffer, uint32_t length);
    bool headerCRC();
    //! Check if the group for a given station exists within the HDF5 file
    void stationCheck();
    void makeOutputHeader();
    void makeNewStation(char *, TBB_Header *);
    void makeNewDipole(string, dalGroup *, TBB_Header *);
    bool transientMode();
    void fixDate();
    void fixDateNew();
    bool processTransientSocketDataBlock();
    bool processSpectralSocketDataBlock();
    void discardFileBytes(uint bytes);
    void processTransientFileDataBlock();
    void processSpectralFileDataBlock();
    void cleanup();

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
    
    //! Initialize internal data
    void init ();
    //! Initialize the values within the TBB_Header struct
    void init_TBB_Header ();
    
  }; // class TBB
  
} // DAL namespace


#endif // TBB_H
