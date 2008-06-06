/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
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

#define PI 3.141592653589793238462643

namespace DAL {

  struct FileHeader {
    UInt32    magic;        // 0x3F8304EC, also determines endianness
    UInt8     bitsPerSample;
    UInt8     nrPolarizations;
    UInt16    nrBeamlets;   // number of subbands
    UInt32    nrSamplesPerBeamlet; // 155648 (160Mhz) or 196608 (200Mhz)
    char      station[20];
    Float64   sampleRate;       //156250.0 or 195312.5 .. double
    Float64   subbandFrequencies[54];
    Float64   beamDirections[8][2];
    Int16     beamlet2beams[54];
    UInt32    padding;  // padding to circumvent 8-byte alignment
  };

  struct BlockHeader
  {
    UInt32      magic; // 0x2913D852
    Int32       coarseDelayApplied[8];
    UInt8       padding[4];  // padding to circumvent 8-byte alignment
    Float64     fineDelayRemainingAtBegin[8];
    Float64     fineDelayRemainingAfterEnd[8];
    Int64       time[8]; // compatible with TimeStamp class.
    UInt32      nrFlagsRanges[8];

    struct range {
     UInt32    begin; // inclusive
     UInt32    end;   // exclusive
    } flagsRanges[8][16];

  };

   // dataStruct is 8 bytes
  struct Sample {
     complex<int16_t> xx;
     complex<int16_t> yy;
  };

  class BFRaw {

    char * DECrad2deg( const float &rad );
    char * RArad2deg( const float &rad );

    // declare handle for the input file
    dalTable ** table;
    fstream * rawfile;
    string outputfilename;
    dalDataset dataset;
    FileHeader fileheader;
    BlockHeader blockheader;
    bool bigendian;
    bool first_block;
    int downsample_factor;
    bool DO_DOWNSAMPLE;
    bool DO_FLOAT32_INTENSITY;
    bool DO_CHANNELIZATION;
    bool processBlocks( int16_t );
    Float32 * downsample_to_float32_intensity( Sample *, int32_t,
                                               const uint64_t, int32_t );
    Float32 * compute_float32_intensity( Sample * data,
                                         int32_t start,
                                         const uint64_t arraylength );
    public:

    BFRaw( string const& name, bool doIntensity, bool doDownsample,
           bool doChannelization, int factor );  // constructor
    ~BFRaw();  // destructor
    void makeH5OutputFile();
    void readRawFileHeader();
    void openRawFile(char*);
    bool eof();
    void processBlocks();

  }; // class BFRaw

} // DAL namespace


#endif // BFRAW_H
