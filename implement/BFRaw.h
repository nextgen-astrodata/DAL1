/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
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

#ifndef BFRAW_H
#define BFRAW_H

#define DEBUGGING_MESSAGES

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#include <BFRawFormat.h>

// socket headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PI 3.141592653589793238462643

#define _OPENMP

namespace DAL {
  
  /*!
    \class BFRaw

    \ingroup DAL

    \brief High-level interface between raw beam-formed data and the DAL.

    \author Joseph Masters
    
    High-level interface between raw beam-formed data and the DAL.
  */
  
  class BFRaw : public BFRawFormat
  {
   public:
    char * DECrad2deg( const float &rad );
    char * RArad2deg( const float &rad );
    
	private:
    // declare handle for the input file
    dalTable ** table;
    fstream * rawfile;
    string outputfilename;
    dalDataset dataset;
    BFRaw_Header header;

		// following used in socket mode
//    fd_set readSet; 
//    struct timeval timeVal;
    //int server_socket;
    struct sockaddr_in incoming_addr;
		socklen_t socklen;
    BlockHeader blockheader;
		size_t blockHeaderSize;
		size_t dataSize;
		//char * pData; // pointer to temporary buffer to store data samples of one subband
		Sample * sampledata;
		uint32_t block_nr;
		// end used in socket mode

		int32_t index;
    bool bigendian;
    bool first_block;
    int downsample_factor;
    bool doDownsample_p;
    bool DO_FLOAT32_INTENSITY;
    bool DO_CHANNELIZATION;
    bool processBlocks( int16_t );

		Float32 * ds_data; // pointer to dynamically allocated buffer which will hold the downsampled data
		Float32 * totalintensity;
		uint32_t ds_size; // = header.nrSamplesPerSubband / downsample_factor = the size of the ds_data buffer in Float32 units

//    Float32 * downsample_to_float32_intensity( Sample *, int32_t, const uint64_t, int32_t );
		void downsampleSingleSubband( Sample *, uint16_t, Float32 *, uint );
//    Float32 * compute_float32_intensity( Sample * data, int32_t start, const uint64_t arraylength );
    void compute_float32_intensity( Sample * data, uint32_t start, uint32_t arraylength, Float32 * totalintensity );

	public:
    //! Argumented constructor
    BFRaw( string const& name,
	   bool doIntensity=false,
	   bool doDownsample=false,
	   bool doChannelization=false,
	   int factor=1 );
    
    /* Destruction */
    
    ~BFRaw();

public:
    //! Provide a summary of the object's properties
    inline void summary () { summary(std::cout); }
    //! Provide a summary of the object's properties
    void summary (std::ostream &os);
    //! Get the name of the output file
    inline string outfile () const { return outputfilename; }
    //! Set up the socket connection to the server
    bool connectsocket( const char * portnumber );
    //! Create HDF5 file from the raw data
    void makeH5OutputFile(void);
    //! Open file with raw output of beamformer
    void openRawFile(const char*);
		//! read and process the BFRaw data blocks from socket
		bool processBFRawDataBlockFromSocket(void);
		//! writes the currently stored BFRaw data block to the output file
		void writeBFRawDataBlockToFile(void);
    //! Read the main header block of the file with the raw data
    void readRawFileHeader();
		//! reads the main header block from the main_socket connection
    bool readRawSocketHeader();
    //! Processing of the data blocks within the input file
    void processBlocks();
    //! Check for reaching end-of-file
    bool eof();

private:
		//! reads upto nrOfBytesToRead from socket and checks for end ans reading errors
		int receiveBytes(void *storage, size_t nrOfBytesToRead);
		//! read raw bytes from the main_socket
//		bool readsocket( unsigned int nbytes, char* buf );
		//! converts endian of data read from socket
		void convertEndian(BlockHeader* blockheader);
		std::string off_online;
		int64_t file_byte_size;
		int64_t oneSecondBlockSize;
		int nrOfBlocks;


  }; // class BFRaw
  
} // DAL namespace


#endif // BFRAW_H
