/*------------------------------------------------------------------------*
| $Id::                                                                 $ |
*-------------------------------------------------------------------------*
***************************************************************************
*   Copyright (C) 2009 by ASTRON the Netherlands                          *
*   Author: Alwin de Jong                                                 *
*   jong@astron.nl                                                        *
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

#ifndef _StationBeamReader_
#define _StationBeamReader_

#include <string>
#include <netdb.h>

#include <sys/socket.h>
#include <BFRawFormat.h>

#define PI 3.141592653589793238462643

class fstream;
class BF2H5;

class StationBeamReader {
public:
	StationBeamReader(BF2H5 *parent, bool socket_mode);
	~StationBeamReader();

	bool setSocketMode(unsigned port_number) {return connectSocket(port_number);};
	bool setFileMode(std::string &input_file) {return openRawFile(input_file);};
	
	// read functions
	bool readMainHeader(BFRawFormat::BFRaw_Header &header); // reads the main file or socket header
	bool readFirstDataBlock(BFRawFormat::BlockHeader &first_block_header, BFRawFormat::Sample *sample_data, size_t data_block_size);
	bool readDataBlock(BFRawFormat::Sample *sample_data);
	bool finishedReading(void) const {return finished_reading;};
	
	void printHeaderParameters(BFRawFormat::BFRaw_Header &header); // prints debug info of the main header
		
private:
	int64_t receiveBytes(void *storage, int64_t nrOfBytesToRead); // low level read from socket
	bool openRawFile( std::string &filename );
	bool connectSocket(unsigned int port_number);
	void swapHeaderEndians(BFRawFormat::BFRaw_Header &header); // swap the byte endians if not in bigendian
	void convertEndian(BFRawFormat::BlockHeader *pBlockHeader);
	const std::string &RArad2deg( const float &rad );
	const std::string &DECrad2deg( const float &dec );
	void finishReading(void); // closes the socket or file and sets the finished_reading flag
			
public:
	static int server_socket;
//	static bool time_out; //! used for time out handler on socket
	
private:
	bool finished_reading;
	bool bigendian;

// socket things:
	unsigned portNumber;
	struct sockaddr_in incoming_addr;
	socklen_t socklen;
	
	//file things
	std::fstream * rawfile;	
	size_t file_byte_size;
	
	BF2H5 * itsParent;
	bool socketmode;
	bool memAllocOK;
	std::string ra_str,dec_str;
	size_t dataBlockSize; // the size of a data block (excluded its header)
	size_t blockHeaderSize;
};

#endif // _StationBeamReader_
