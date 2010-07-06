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

#ifndef BF2H5_H
#define BF2H5_H

// Standard header files
#include <string>
#include <map>

// LOFAR header files
#ifdef HAVE_LOFAR
#include <Interface/Parset.h>
#endif

// DAL header files
#include "HDF5Writer.h"
#include "Bf2h5Calculator.h"
#include "StationBeamReader.h"
#include <BFRawFormat.h>

#define DEBUGGING_MESSAGES

#define INITIAL_NR_OF_READ_BUFFERS 2

typedef std::vector<BFRawFormat::Sample *> sampleBuffers;
/*!
  - key = nr of sample buffer
  - value = current block using the sample buffer (-1 = not in use)
*/
typedef std::map<uint8_t, long int> bufferTracker;

/*!
  \class BF2H5

  \ingroup DAL
  \ingroup dal_apps

  \author Alwin de Jong

  \todo Instead of reading a main header from file or socket a lot of information
        needs to come from a parset file
  \todo LCSCommon needs to be integrated to use the parset reader

  <h3>Prerequisite</h3>
  
  <ul type="square">
    <li>DAL::Bf2h5Calculator
    <li>LOFAR::RTCP::Parset
  </ul>
  
*/
class BF2H5 {

 public:
  
  // === Construction ===========================================================

  //! Argumented constructor
  BF2H5 (const std::string &outfile,
	 const std::string &parset_filename,
	 uint downsample_factor,
	 bool do_intensity);

  // === Destruction ============================================================

  //! Default destructor
  ~BF2H5();
  
  // === Parameter access =======================================================

  //! Is computation of the intensity enabled?
  inline bool doIntensity (void) const {
    return itsDoIntensity;
  }
  //! Is downsampling of the data enabled?
  inline bool doDownSampling (void) const {
    return itsDoDownSample;
  }
  //! Get the downsampling factor
  inline uint getDownSampleFactor (void) const {
    return itsDownsampleFactor;
  }
  //! Set input mode to read from socket
  void setSocketMode(uint port);
  //! Set input mode to read from file
  void setFileMode(std::string &infile);
  //! Start the bf2h5 main process
  void start (bool const &verbose=false);
  //! Get sample data header
  inline const BFRawFormat::Sample &getSampleData (void) const {
    return *itsSampleBuffers[itsReadBuffer];
  }
  //! Get BF raw data main header
  inline const BFRawFormat::BFRaw_Header &getMainHeader (void) const {
    return BFMainHeader;
  }
  //! Get BF raw data main header for writing
  BFRawFormat::BFRaw_Header &getMainHeaderForWrite (void) {
    return BFMainHeader;
  }
  //! Get the number of samples per subband
  inline uint32_t getNrSamplesPerSubband (void) const {
    return BFMainHeader.nrSamplesPerSubband;
  }
  //! Get the number of subbands
  inline uint16_t getNrSubbands(void) const {
    return BFMainHeader.nrSubbands;
  }
  
  // === Signaling functions for threads ========================================

  //! Non-blocking write of block \e blockNr within subband \e subband
  inline void calculatorDataReady (unsigned blockNr,
				   unsigned subband,
				   float * calculator_data)
  {
    itsWriter->writeSubband(blockNr, subband, calculator_data);
  };

  //! Called by the calculator when a block of subbands was completed
  void blockComplete(long int blockNr);

  //! Get epoch as UTC
  inline const std::string &getEpochUTC(void) const {
    return EpochUTC;
  }
  //! Get epoch as date string
  inline const std::string &getEpochDate(void) const {
    return EpochDate;
  }
  
 private:
  void getTimeFromBlockHeader(void);
  bool allocateSampleBuffers(void);
  bool switchReadBuffer(long int block_nr); // switch to the next unused readbuffer, to be used by block: block_nr
  
 private:
  
  //! Input mode: socket (true) or file (false)
  bool socketmode;
  //! Compute intensities?
  bool itsDoIntensity;
  //! Downsample the data?
  bool itsDoDownSample;
  //! Downsampling factor
  uint itsDownsampleFactor;
  
  // some main header parameters we need to know here
  std::string itsParseFile;
  std::string inputFile;
  std::string outputFile;
  uint tcpPort;
  DAL::Bf2h5Calculator *itsCalculator;
  HDF5Writer *itsWriter;
  DAL::StationBeamReader *itsReader;
  
  // data structures:
  
  BFRawFormat::BFRaw_Header BFMainHeader; // holds the main header
  BFRawFormat::BlockHeader firstBlockHeader; // will hold the header of the first data block
  
  size_t oneBlockdataSize;
  //sample buffers things
  uint8_t itsReadBuffer, itsCurrentNrOfReadBuffers; // the current read buffer
  bufferTracker itsBufferTracker; // keeps track of which buffer is used for which data block
  sampleBuffers itsSampleBuffers; // pointers to input data samplebuffers
  
  std::string EpochUTC;
  std::string EpochDate;
  
#ifdef HAVE_LOFAR
  const LOFAR::RTCP::Parset *itsParset;
#endif 
};

#endif // _BF2H5_
