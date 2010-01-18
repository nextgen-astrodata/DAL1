/*------------------------------------------------------------------------*
| $Id::                                                                 $ |
*-------------------------------------------------------------------------*
***************************************************************************
*   Copyright (C) 2009 by Alwin de Jong                                   *
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

#ifndef _BF2H5_
#define _BF2H5_

#include <string>
#include <map>
#include "HDF5Writer.h"
#include "Bf2h5Calculator.h"
#include "StationBeamReader.h"
#include <BFRawFormat.h>

#define DEBUGGING_MESSAGES

#define INITIAL_NR_OF_READ_BUFFERS 2

typedef std::vector<BFRawFormat::Sample *> sampleBuffers;
// bufferTracker key = nr of sample buffer, value = current block using the sample buffer (-1 = not in use)
typedef std::map<uint8_t, long int> bufferTracker;

class BF2H5 {
public:
  BF2H5(const std::string &outfile, uint downsample_factor, bool do_intensity);
	~BF2H5();
	
	void setSocketMode(uint port);
	void setFileMode(std::string &infile);
	
	void start(void); // starts the bf2h5 main process
	
	// access function for sample data and BFRawFormat headers
	const BFRawFormat::Sample &getSampleData(void) const {return *itsSampleBuffers[itsReadBuffer];}
	const BFRawFormat::BFRaw_Header &getMainHeader(void) const {return BFMainHeader;}
	BFRawFormat::BFRaw_Header &getMainHeaderForWrite(void) {return BFMainHeader;}
	
	uint32_t getNrSamplesPerSubband(void) const { return BFMainHeader.nrSamplesPerSubband;}
	uint16_t getNrSubbands(void) const {return BFMainHeader.nrSubbands;}
			    
	// signaling functions for threads
	void calculatorDataReady(unsigned blockNr, unsigned subband, float * calculator_data) {itsWriter->writeSubband(blockNr, subband, calculator_data);}; // non-blocking write
	void blockComplete(long int blockNr); // called by the calculator when a block of subbands was completed
	
	const std::string &getEpochUTC(void) const { return EpochUTC; }
	const std::string &getEpochDate(void) const { return EpochDate; }
	bool doDownSampling(void) const {return itsDoDownSample;}
	bool doIntensity(void) const {return itsDoIntensity;}
	uint getDownSampleFactor(void) const {return itsDownsampleFactor;}

private:
  void getTimeFromBlockHeader(void);
  bool allocateSampleBuffers(void);
  bool switchReadBuffer(long int block_nr); // switch to the next unused readbuffer, to be used by block: block_nr
	
private:
	bool socketmode;
	uint itsDownsampleFactor;
	bool itsDoDownSample;
	bool itsDoIntensity;
	
	// some main header parameters we need to know here
	std::string inputFile;
	std::string outputFile;
	uint tcpPort;
	Bf2h5Calculator *itsCalculator;
	HDF5Writer *itsWriter;
	StationBeamReader *itsReader;
	
	// data structures:
	
	BFRawFormat::BFRaw_Header BFMainHeader; // holds the main header
	BFRawFormat::BlockHeader firstBlockHeader; // will hold the header of the first data block
	
	size_t oneBlockdataSize;
	//sample buffers things
	uint8_t itsReadBuffer, itsCurrentNrOfReadBuffers; // the current read buffer
	bufferTracker itsBufferTracker; // keeps track of which buffer is used for which data block
	sampleBuffers itsSampleBuffers; // pointers to input data samplebuffers
	
	std::string EpochUTC, EpochDate;
		
		//TODO: instead of reading a main header from file or socket a lot of information needs to come from a parset file
		// LCSCommon needs to be integrated to use the parset reader
		
};

#endif // _BF2H5_
