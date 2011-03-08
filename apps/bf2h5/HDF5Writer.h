/***************************************************************************
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

#ifndef HDF5WRITER_H
#define HDF5WRITER_H

#include <deque>
#include <fstream>
#include <pthread.h>
#include <map>
#include <string>
#include <sstream> // needed for type conversion
#include <time.h>
#include <vector>

// DAL header files
#include <dal_config.h>
#include <core/dalCommon.h>
#include <core/dalDataset.h>

// LOFAR header files
#ifdef DAL_WITH_LOFAR
#include <Interface/Parset.h>
#endif


// map key = blockNr, second.first = subband, second.second = pointer to data floats to write
typedef std::map<long int, std::deque<std::pair<uint8_t, float *> > > writeMap;

// Forward declaration
class BF2H5;

/*!
  \class HDF5Writer
  
  \ingroup DAL
  \ingroup dal_apps

  \author Alwin de Jong

  <h3>Prerequisite</h3>
  
  <ul type="square">
    <li>DAL::Bf2h5Calculator
    <li>LOFAR::RTCP::Parset
  </ul>
  
*/
class HDF5Writer {

public:

  // === Construction ===========================================================

#ifdef DAL_WITH_LOFAR
  HDF5Writer (BF2H5 *parent,
	      const std::string &output_file,
	      const LOFAR::RTCP::Parset *ps,
	      size_t output_block_size,
	      uint8_t nr_subbands);
#endif
  
  // === Destruction ============================================================

  ~HDF5Writer();
  
  // === Methods ================================================================

#ifdef DAL_WITH_LOFAR
  //! Create HDF5 output dataset
  void createHDF5File (const LOFAR::RTCP::Parset *ps);
#endif

  //! Start the separate writing thread
  bool start(void);
  //! Add a datablock for writing
  void writeSubband(long int blockNr, uint8_t subband, float *calculator_data);
  void openRawFile( const char* filename );
  //! Check if the writer still has something left to write
  bool dataLeft(void);
  //! Stop the writing thread
  bool stop(void);
  void showStatus(void);
  
 private:

  //! Get the data for the currently processed block
  bool getDataForCurrentBlock(void);
  //! Check if the currently processed block is complete
  void checkIfBlockComplete(void);
  void startNextBlock(void);
  //! Thread to perform the writing of the data
  void writeData(void);
  //! Start new internal thread
  static void * StartInternalThread(void * This)
  {
    ((HDF5Writer *)This)->writeData();
    return NULL;
  }
  
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
  //! Size of a data block (excluded its header)
  size_t outputBlockSize;
  std::string creation_mode;
  long int nrOfBlocks, currentBlockNr;
  bool * subbandReady; // points to array of flags marking subbands that have been written
  uint8_t nrOfSubbands;
  int64_t file_byte_size;
  pthread_t itsWriteThread;
};


#endif // HDF5WRITER_H
