/*------------------------------------------------------------------------*
| $Id::                                                                 $ |
*-------------------------------------------------------------------------*
***************************************************************************
*   Copyright (C) 2009 by ASTRON                                          *
*    Author: Alwin de Jong ( jong@astron.nl )                             *
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

#include "bf2h5.h"
#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::bad_alloc;


BF2H5::BF2H5(const std::string &outfile, uint downsample_factor, bool do_intensity)
  :	socketmode(false), itsDownsampleFactor(downsample_factor), itsDoIntensity(do_intensity), outputFile(outfile), itsCalculator(0), itsWriter(0), itsReader(0), oneBlockdataSize(0), itsReadBuffer(0), itsCurrentNrOfReadBuffers(INITIAL_NR_OF_READ_BUFFERS)
{
  if (downsample_factor > 1) {
    itsDoDownSample = true;
    itsDoIntensity = true;
  }
  else {
    itsDoDownSample = false;
  }
}

BF2H5::~BF2H5()
{
  delete itsReader;
  delete itsWriter;
  delete itsCalculator;
  for (sampleBuffers::iterator it = itsSampleBuffers.begin(); it != itsSampleBuffers.end(); ++it) {
    delete [] *it;
  }
}

void BF2H5::setSocketMode(uint port) {
  tcpPort = port;
  socketmode = true;
}
  
void BF2H5::setFileMode(std::string &infile){
  inputFile = infile;
  socketmode = false;
}

void BF2H5::getTimeFromBlockHeader(void) {
	// write the utc time to hdf5 file according to header time info
  time_t utc;
  utc = (time_t)(firstBlockHeader.time[0]/(int64_t)BFMainHeader.sampleRate);
  uint16_t buf_size(128);
  char * time_date = new char[buf_size];
  memset (time_date,'\0',buf_size);
  strftime(time_date, buf_size, "%H:%M:%S", gmtime(&utc));
  EpochUTC = time_date;
	
	//dataset.setAttribute( "EPOCH_UTC", time_date );
  
  memset (time_date,'\0',buf_size);
  strftime(time_date, buf_size, "%d/%m/%y", gmtime(&utc));
  EpochDate = time_date ;
//	dataset.setAttribute( "EPOCH_DATE", time_date );
  
  delete [] time_date;
}


bool BF2H5::allocateSampleBuffers(void) {
#ifdef DEBUGGING_MESSAGES
  cout << "BF2H5::allocateSampleBuffers: allocating " << itsCurrentNrOfReadBuffers * oneBlockdataSize * sizeof(BFRawFormat::Sample) << " bytes for sample input data" << endl;
#endif
  try {
    for (unsigned short i = 0; i < itsCurrentNrOfReadBuffers; ++i) {
      BFRawFormat::Sample *pbuf = new BFRawFormat::Sample[ oneBlockdataSize ];
//		memset(pbuf, 0, oneBlockdataSize * sizeof(BFRawFormat::Sample));
      itsSampleBuffers.push_back(pbuf);
      itsBufferTracker[i] = -1;
    }
    itsBufferTracker[0] = 0; // first buffer will be used by block 0
  }
  catch (bad_alloc) {
    cerr << "Can't allocate memory for input databuffer." << endl;
    return false;
  }
  return true;
}


void BF2H5::blockComplete(long int blockNr) {
  for (bufferTracker::iterator it = itsBufferTracker.begin(); it != itsBufferTracker.end(); ++it) {
    if (it->second == blockNr) {
      it->second = -1;
      return;
    }
  }
  cerr << "BF2H5::calculatorDataReady: ERROR, trying to free a read buffer for block " << blockNr << " that doesn't have a read buffer!" << endl;
}


bool BF2H5::switchReadBuffer(long int block_nr) {
  for (bufferTracker::iterator it = itsBufferTracker.begin(); it != itsBufferTracker.end(); ++it) {
    if (it->second == -1) { // not in use
      it->second = block_nr;
      itsReadBuffer = it->first;
      return true;
    }
  }
	// we didn't find a free read buffer, allocat a new buffer
  try {
#ifdef DEBUGGING_MESSAGES
    cout << "BF2H5::switchReadBuffer, Calculation not fast enough, adding read buffer no. " << static_cast<int>(itsCurrentNrOfReadBuffers) << " for block " << block_nr << endl;
#endif
    BFRawFormat::Sample *pbuf = new BFRawFormat::Sample[ oneBlockdataSize ];
    itsSampleBuffers.push_back(pbuf);
  }
  catch (bad_alloc) {
    cerr << "BF2H5::switchReadBuffer, ERROR cannot allocate memory for new input read buffer." << endl;
    return false;
  }
  itsBufferTracker.insert(std::pair<uint8_t, long int>(itsCurrentNrOfReadBuffers, block_nr));
  itsReadBuffer = itsCurrentNrOfReadBuffers++; // switch to new buffer
//	itsCalculator->showStatus();
//	itsWriter->showStatus();
  return true;
}


void BF2H5::start(void) {
	// start the reader
  bool result;
  unsigned int blockNr(0);
  itsReader = new StationBeamReader(this, socketmode);
  if (socketmode) {
    result = itsReader->setSocketMode(tcpPort);
  }
  else {
    result = itsReader->setFileMode(inputFile);
  }
  if (result)	{
    if (itsReader->readMainHeader(getMainHeaderForWrite())) {
      oneBlockdataSize = BFMainHeader.nrSamplesPerSubband * BFMainHeader.nrSubbands;
      size_t downSampledDataSize = BFMainHeader.nrSamplesPerSubband / itsDownsampleFactor;
      if (allocateSampleBuffers()) {
		//start the calculator
        itsCalculator = new Bf2h5Calculator(this, BFMainHeader.nrSubbands, getNrSamplesPerSubband());
        itsWriter = new HDF5Writer(this, outputFile, downSampledDataSize, BFMainHeader.nrSubbands);
        if (itsReader->readFirstDataBlock(firstBlockHeader, itsSampleBuffers[itsReadBuffer], oneBlockdataSize * sizeof(BFRawFormat::Sample))) {
          getTimeFromBlockHeader();
          if (itsWriter->start()) { // starts the writer, it will wait for data
            itsCalculator->startProcessing();
            itsCalculator->calculateDataBlock(blockNr++, itsSampleBuffers[itsReadBuffer]); // calculator will call calculationFinished when done
            switchReadBuffer(blockNr);
            while (!(itsReader->finishedReading())) {
              itsReader->readDataBlock(itsSampleBuffers[itsReadBuffer]); // blocking read
              itsCalculator->calculateDataBlock(blockNr++, itsSampleBuffers[itsReadBuffer]); // non-blocking calculator will call calculationFinished
              switchReadBuffer(blockNr);
            }
            cout << "Reader finished, connection closed" << endl;
            while ((itsCalculator->stillProcessing()) || (itsWriter->dataLeft())) {
              cout << "still processing last received data..." << endl;
              /*
#ifdef DEBUGGING_MESSAGES
              if (itsCalculator->stillProcessing()) {
              cout << "calculator says it's still processing..." << endl;
              cout << itsCalculator->whatAreYouDoing() << endl;
            }
              if (itsWriter->dataLeft()) {
              cout << "writer says it has still data left to write..." << endl;
            }
#endif
              */
              sleep(1); // calculator or hdf5 writer still busy
            }
							// stop all threads
	//					cout << "calling calculator stop" << endl;
            if (!itsCalculator->stop()) {
              cerr << "calculator didn't stop all its threads correctly!" << endl;
            }
            if (!itsWriter->stop()) {
              cerr << "writer thread didn't stop correctly!" << endl;
            }
            cout << "HDF5 file " << inputFile << " has been written." << endl
                << "all done!" << endl;
          }
          else {
            cerr << "Could not start writer thread!" << endl;
          }
        }
        else {
          cerr << "error reading first data block!" << endl;
        }
      }
    }
  }
}
