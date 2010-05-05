/*-------------------------------------------------------------------------*
 | $Id:: TBB_Timeseries.h 4783 2010-04-28 09:49:12Z baehren              $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Alwin de Jong <jong@astron.nl>                                        *
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
#include <sstream>
#include <netdb.h>
#include <sys/socket.h>

#include <BFRawFormat.h>

// Forward declarations
class fstream;
class BF2H5;

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class StationBeamReader
    
    \ingroup DAL
    \ingroup data_common
    
    \author Alwin de Jong
  */
  class StationBeamReader {
    
  public:
    
    static int server_socket;
    
    // === Construction ===========================================================
    
    // ! Argumented constructor
    StationBeamReader(BF2H5 *parent,
		      bool socket_mode);
    
    // === Destruction ============================================================
    
    //! Default destructor
    ~StationBeamReader();
    
    // === Methods ================================================================
    
    //! Set input mode to read from socket
    inline bool setSocketMode (unsigned port_number) {
      return connectSocket(port_number);
    };
    
    //! Set input mode to read from a file
    inline bool setFileMode(std::string &input_file) {
      return openRawFile(input_file);
    };
    
    //! Read the main file or socket header
    bool readMainHeader (BFRawFormat::BFRaw_Header &header);
    
    //! Read the first block of data
    bool readFirstDataBlock (BFRawFormat::BlockHeader &first_block_header,
			     BFRawFormat::Sample *sample_data,
			     size_t data_block_size);
    
    //! Read a block of data
    bool readDataBlock(BFRawFormat::Sample *sample_data);
    
    //! Check if we have finished reading data
    inline bool finishedReading(void) const {
      return finished_reading;
    };
    
    //! Print debug info of the main header
    void printHeaderParameters(BFRawFormat::BFRaw_Header &header);
    
  private:
    
    // === Private methods ========================================================
    
    //! Low level read from socket
    int64_t receiveBytes (void *storage, int64_t nrOfBytesToRead);
    bool openRawFile( std::string &filename );
    bool connectSocket(unsigned int port_number);
    //! Swap the byte endians if not in bigendian
    void swapHeaderEndians(BFRawFormat::BFRaw_Header &header);
    void convertEndian(BFRawFormat::BlockHeader *pBlockHeader);
    const std::string &RArad2deg( const float &rad );
    const std::string &DECrad2deg( const float &dec );
    //! Close the socket or file and sets the finished_reading flag
    void finishReading(void);
    
    // === Private variables ======================================================
    
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

} // END : namespace DAL

#endif // _StationBeamReader_
