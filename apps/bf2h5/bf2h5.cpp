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

#include "bf2h5.h"
#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::bad_alloc;

// ==============================================================================
//
//  Construction
//
// ==============================================================================

/*!
  \param outfile -- Name of the output HDF5 dataset.
  \param parset_filename -- Name of the parameter set file.
  \param downsample_factor -- Downsample factor.
  \param do_intensity -- Compute intensities?
*/
BF2H5::BF2H5 (const std::string &outfile,
	      const std::string &parset_filename,
	      uint downsample_factor,
	      bool do_intensity)
  : socketmode(false),
    itsDownsampleFactor(downsample_factor),
    itsDoIntensity(do_intensity),
    outputFile(outfile),
    itsCalculator(0),
    itsWriter(0),
    itsReader(0),
    oneBlockdataSize(0),
    itsReadBuffer(0),
    itsCurrentNrOfReadBuffers(INITIAL_NR_OF_READ_BUFFERS)
{
#ifdef HAVE_LOFAR
  // create parset
  itsParset = new LOFAR::RTCP::Parset(parset_filename.c_str());
#endif
  
  if (downsample_factor > 1) {
    itsDoDownSample = true;
    itsDoIntensity = true;
  }
  else {
    itsDoDownSample = false;
  }
}

// ==============================================================================
//
//  Destruction
//
// ==============================================================================

BF2H5::~BF2H5()
{
  delete itsReader;
  delete itsWriter;
  delete itsCalculator;
  for (sampleBuffers::iterator it = itsSampleBuffers.begin(); it != itsSampleBuffers.end(); ++it) {
    delete [] *it;
  }

#ifdef HAVE_LOFAR
  delete itsParset;
#endif
}

// ==============================================================================
//
//  Destruction
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                  setSocketMode

void BF2H5::setSocketMode(uint port)
{
  tcpPort    = port;
  socketmode = true;
}

//_______________________________________________________________________________
//                                                                    setFileMode

void BF2H5::setFileMode(std::string &infile)
{
  inputFile  = infile;
  socketmode = false;
}

//_______________________________________________________________________________
//                                                         getTimeFromBlockHeader

void BF2H5::getTimeFromBlockHeader(void)
{
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

//_______________________________________________________________________________
//                                                          allocateSampleBuffers

bool BF2H5::allocateSampleBuffers(void)
{
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

//_______________________________________________________________________________
//                                                                  blockComplete

void BF2H5::blockComplete (long int blockNr)
{
  for (bufferTracker::iterator it = itsBufferTracker.begin(); it != itsBufferTracker.end(); ++it) {
    if (it->second == blockNr) {
      it->second = -1;
      return;
    }
  }
  std::cerr << "[BF2H5::blockComplete] ERROR, trying to free a read buffer for block "
	    << blockNr
	    << " that doesn't have a read buffer!"
	    << endl;
}

//_______________________________________________________________________________
//                                                               switchReadBuffer

bool BF2H5::switchReadBuffer (long int block_nr)
{
  for (bufferTracker::iterator it = itsBufferTracker.begin(); it != itsBufferTracker.end(); ++it) {
    if (it->second == -1) { // not in use
      it->second    = block_nr;
      itsReadBuffer = it->first;
      return true;
    }
  }

  // we didn't find a free read buffer, allocat a new buffer
  try {
#ifdef DEBUGGING_MESSAGES
    cout << "BF2H5::switchReadBuffer, Calculation not fast enough, adding read buffer no. " << static_cast<int>(itsCurrentNrOfReadBuffers) << " for block " << block_nr << endl;
#endif
    BFRawFormat::Sample *pbuf = new BFRawFormat::Sample[oneBlockdataSize];
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

//_______________________________________________________________________________
//                                                                          start

void BF2H5::start (bool const &verbose)
{
  bool result          = true;
  unsigned int blockNr = 0;
  itsReader            = new DAL::StationBeamReader(this, socketmode);

  /* Connect to the input source */
  if (socketmode) {
    result = itsReader->setSocketMode(tcpPort);
  }
  else {
    result = itsReader->setFileMode(inputFile);
  }

  if (result) {
    if (itsReader->readMainHeader(getMainHeaderForWrite())) {
      
      if (verbose) {
	std::cout << "[BF2H5::start]" << std::endl;
	std::cout << "-- BFMainHeader.nrSamplesPerSubband = "
		  << BFMainHeader.nrSamplesPerSubband         << std::endl;
	std::cout << "-- BFMainHeader.nrSubbands          = "
		  << BFMainHeader.nrSubbands                  << std::endl;
#ifdef HAVE_LOFAR
	std::cout << "-- parset:nrSubbandSamples          = "
		  << itsParset->nrSubbandSamples()            << std::endl;
	std::cout << "-- parset.nrSubbands                = "
		  << itsParset->nrSubbands()                  << std::endl;
#endif
      }  // END : if (verbose)
      
      oneBlockdataSize = BFMainHeader.nrSamplesPerSubband * BFMainHeader.nrSubbands;
      size_t downSampledDataSize = BFMainHeader.nrSamplesPerSubband / itsDownsampleFactor;

      if (allocateSampleBuffers()) {

	// Start the calculator
        itsCalculator = new DAL::Bf2h5Calculator (this,
						  BFMainHeader.nrSubbands,
						  getNrSamplesPerSubband());
	// Start the writer
#ifdef HAVE_LOFAR
        itsWriter = new HDF5Writer (this,
				    outputFile,
				    itsParset,
				    downSampledDataSize,
				    BFMainHeader.nrSubbands);
#else
	itsWriter = NULL;
#endif

        if (itsReader->readFirstDataBlock(firstBlockHeader, itsSampleBuffers[itsReadBuffer], oneBlockdataSize * sizeof(BFRawFormat::Sample))) {
          getTimeFromBlockHeader();
	  
	  /* Start up the writer to listen for incoming data */
          if (itsWriter->start()) {
            itsCalculator->startProcessing();
            itsCalculator->calculateDataBlock(blockNr++, itsSampleBuffers[itsReadBuffer]); // calculator will call calculationFinished when done
            switchReadBuffer(blockNr);
            while (!(itsReader->finishedReading())) {
              itsReader->readDataBlock(itsSampleBuffers[itsReadBuffer]); // blocking read
              itsCalculator->calculateDataBlock(blockNr++, itsSampleBuffers[itsReadBuffer]); // non-blocking calculator will call calculationFinished
              switchReadBuffer(blockNr);
            }
            cout << "[BF2H5::start] Reader finished, connection closed" << endl;
            while ((itsCalculator->stillProcessing()) || (itsWriter->dataLeft())) {
              cout << "[BF2H5::start] Still processing last received data..." << endl;
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

            if (!itsCalculator->stop()) {
              cerr << "[BF2H5::start] Calculator didn't stop all its threads correctly!"
		   << endl;
            }

            if (!itsWriter->stop()) {
              cerr << "[BF2H5::start] Writer thread didn't stop correctly!" << endl;
            }
	    
            cout << "HDF5 file " << inputFile << " has been written." << endl
		 << "all done!" << endl;
          } // END : if (itsWriter->start())
          else {
            cerr << "[BF2H5::start] Could not start writer thread!" << endl;
          }
        }
        else {
          cerr << "[BF2H5::start] Error reading first data block!" << endl;
        }
      }
    }
  }  // END : if (result)

}
