/*-------------------------------------------------------------------------*
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

#ifndef HDF5WRITER_H
#define HDF5WRITER_H

#include <pthread.h>
#include <fstream>
#include <string>
#include <map>
#include <deque>

#include <core/dalCommon.h>
#include <dalDataset.h>

// map key = blockNr, second.first = subband, second.second = pointer to data floats to write
typedef std::map<long int, std::deque<std::pair<uint8_t, float *> > > writeMap;

class BF2H5;

/*!
  \class HDF5Writer
  \ingroup DAL
  \ingroup data_common
  \author Alwin de Jong
*/
class HDF5Writer {

 public:

  // === Construction ===========================================================

  //! Argumented constructor
  HDF5Writer (BF2H5 *parent,
	      const std::string &output_file,
	      size_t output_block_size,
	      uint8_t nr_subbands);

  // === Destruction ============================================================

  //! Default destructor
  ~HDF5Writer();
  
  // === Methods ================================================================

  //! Create a new HDF5 file
  void createHDF5File(void);
  //! Starts the separate writing thread
  bool start(void);
  //! Adds a datablock for writing
  void writeSubband (long int blockNr,
		     uint8_t subband,
		     float *calculator_data);
  //! Open file with raw data
  void openRawFile( const char* filename );
  //! Check if the writer still has something left to write
  bool dataLeft(void);
  //! Stop the writing thread
  bool stop(void);
  //! Show the current status of the writer
  void showStatus(void);
  
 private:
  
  //! Get the data for the current block
  bool getDataForCurrentBlock(void);
  //! Check if the current block is complete
  void checkIfBlockComplete(void);
  //! Start with the next block of data
  void startNextBlock(void);
  //! Start thread for writing the data
  void writeData(void);
  static void * StartInternalThread(void * This) {((HDF5Writer *)This)->writeData(); return NULL;}
  
 private:
  BF2H5 * itsParent;
  std::fstream * rawfile;
  DAL::dalTable ** table;
  DAL::dalDataset dataset;
  bool stopWriting;
  std::string itsOutputFile;
  uint8_t waitForDataTimeOut;
  std::pair<uint8_t, float *> dataPair;
  writeMap itsData; // contains the block number, subbands and pointers to datablocks that still need to be written
  pthread_mutex_t writeMapMutex;
  bool foundDataForCurrentBlock;
  float * zeroBlock;
  size_t outputBlockSize; // the size of a data block (excluded its header)
  std::string off_online;
  long int nrOfBlocks, currentBlockNr;
  bool * subbandReady; // points to array of flags marking subbands that have been written
  uint8_t nrOfSubbands;
  int64_t file_byte_size;
  pthread_t itsWriteThread;
};


#endif // HDF5WRITER_H
