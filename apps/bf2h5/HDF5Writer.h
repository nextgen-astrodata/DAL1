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

#ifndef _HDF5Writer_
#define _HDF5Writer_

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
	HDF5Writer(BF2H5 *parent, const std::string &output_file, size_t output_block_size, uint8_t nr_subbands);
	~HDF5Writer();

	void createHDF5File(void);
	bool start(void); // starts the separate writing thread
	void writeSubband(long int blockNr, uint8_t subband, float *calculator_data); // adds a datablock for writing
	void openRawFile( const char* filename );
	bool dataLeft(void); // checks if the writer still has something left to write
	bool stop(void); // stops the writing thread
	void showStatus(void);
	
	private:
		bool getDataForCurrentBlock(void);
		void checkIfBlockComplete(void);
		void startNextBlock(void);
		void writeData(void); // thread for doing the writing
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


#endif // _HDF5Writer_
