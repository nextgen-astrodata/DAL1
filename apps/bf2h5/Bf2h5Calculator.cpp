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

#include <iostream>
//#include <sstream> // only for whatAreYouDoing
#include "Bf2h5Calculator.h"
#include "bf2h5.h"

using std::cout;
using std::cerr;
using std::endl;
using std::real;
using std::imag;
using std::bad_alloc;


Bf2h5Calculator::Bf2h5Calculator(BF2H5 *its_parent, uint8_t nr_subbands, uint32_t nr_samples_subband)
	: level(0), itsParent(its_parent), nrOfSubbands(nr_subbands), 
		nrSamplesPerSubband(nr_samples_subband), checking_completeness(false), itsStopProcessing(false), currentBlockNr(0)
{
	pthread_mutex_init(&calculationMapMutex, 0);
	pthread_cond_init(&condition, 0);
	
	itsDownSampleFactor = itsParent->getDownSampleFactor();
	itsSingleSubbandNrOutputSamples = nr_samples_subband / itsDownSampleFactor;
	
	for (unsigned short i = 0; i < NUM_CALCULATION_THREADS; ++i) {
		thread_data_array[i].busy = false;
//		thread_data_array[i].threadID = 0;
		thread_data_array[i].blockNr = 0;
		thread_data_array[i].subbandNr = 0;
		thread_data_array[i].input_data = 0;
		thread_data_array[i].subband_output_data = 0;
		thread_data_array[i].This = this;
	}
	
	subbandReady = new bool [nrOfSubbands];
	for (uint8_t i=0; i < nrOfSubbands; ++i) {
		subbandReady[i] = false;
	}

	allocateMemory();
}


Bf2h5Calculator::~Bf2h5Calculator() {
	pthread_mutex_destroy(&calculationMapMutex);
	pthread_cond_destroy(&condition);
//	cout << "destructor Bf2h5Calculator executing" << endl;
	delete [] subbandReady;
//	cout << "deleted subbandReady array" << endl;
	for (uint8_t i=0; i < nrOfSubbands; ++i) {
			delete [] dataBlockOutput[i];
//			cout << "deleted dataBlockOutput[" << static_cast<int>(i) << "]" << endl;
	}
	delete [] dataBlockOutput;
//	cout << "deleted dataBlockOutput array" << endl;
}


void Bf2h5Calculator::allocateMemory(void) {
	// allocate memory for output data buffers
	try {
#ifdef DEBUGGING_MESSAGES
		std::cout << "Allocating " << nrOfSubbands * itsSingleSubbandNrOutputSamples * sizeof(float) << " bytes for downsampled data..." << std::endl;
#endif

		dataBlockOutput = new float * [nrOfSubbands];
		for (uint8_t i = 0; i < nrOfSubbands; ++i) {
			dataBlockOutput[i] = new float [itsSingleSubbandNrOutputSamples];
			memset(dataBlockOutput[i], 0, itsSingleSubbandNrOutputSamples * sizeof(float));
		}
	}
	catch (bad_alloc)
	{
		cerr << "Bf2h5Calculator: Could not allocate memory." << endl;
		return;
	}
	return;
}

/*
void Bf2h5Calculator::wakeUpCall(void) {
  pthread_mutex_lock (&calculationMapMutex);
  pthread_cond_broadcast(&condition);
  pthread_mutex_unlock(&calculationMapMutex);
}
*/

// CalculateDataBlock adds a datablock for writing
void Bf2h5Calculator::calculateDataBlock(long int blockNr, BFRawFormat::Sample *sampleData) {
	std::pair<unsigned int, BFRawFormat::Sample *> dataPair;
	pthread_mutex_lock (&calculationMapMutex);
	for (uint8_t subband = 0; subband < nrOfSubbands; ++subband) {
		dataPair.first = subband;
		dataPair.second = &(sampleData[ subband * nrSamplesPerSubband]);
				//	itsData.insert(std::pair<unsigned int, std::pair<unsigned int, BFRawFormat::Sample *> >(blockNr, dataPair));
		itsData[blockNr].push_back(dataPair);
	}
	level += nrOfSubbands;
	pthread_cond_broadcast(&condition);
	pthread_mutex_unlock(&calculationMapMutex);
	return;
}

/*
bool Bf2h5Calculator::unscheduledDataLeft(void) {
	pthread_mutex_lock (&calculationMapMutex);
	for (calculationMap::const_iterator it = itsData.begin(); it != itsData.end(); ++it) {
		if (!(it->second.empty())) {
			pthread_mutex_unlock(&calculationMapMutex);
			return true;
		}
	}
	pthread_mutex_unlock(&calculationMapMutex);
	return false;
}


bool Bf2h5Calculator::stillProcessing(void) {
	if (unscheduledDataLeft()) {
		return true;
	}
	// now also check if all threads are finished processing
	for (unsigned short i = 0; i < NUM_CALCULATION_THREADS; ++i) { 
		if (thread_data_array[i].busy == true) {
			return true;
		}
	}
	return false;
}
*/

bool Bf2h5Calculator::stillProcessing(void) {
	return ((level > 0)); // this should actually be done while mutex is locked, but because the answer may lag a bit with reality it's no problem, and speeds up the processing at the end (when the reader is already done, see bf2h5.cpp)
}

bool Bf2h5Calculator::stop(void) {
	bool bResult = true;
	int status;
	void *thread_result;
#ifdef DEBUGGING_MESSAGES
	cout << "Stopping the calculator" << endl;
#endif 
	pthread_mutex_lock (&calculationMapMutex);
	itsStopProcessing = true;
	level = 10;
	pthread_cond_broadcast(&condition);
	pthread_mutex_unlock (&calculationMapMutex);
//	cout << "trying to join calculator threads" << endl;
	for (unsigned short threadIdx = 0; threadIdx < NUM_CALCULATION_THREADS; ++threadIdx) {
		status = pthread_join (itsCalculationThread[threadIdx], &thread_result);
		if (status != 0) {
//			cout << "calculator thread " << threadIdx << " returned " << status << endl;
			bResult = false;
		}
		if (thread_result != NULL) {
//			cout << "calculator thread " << threadIdx << " non NULL result value" << endl;
			bResult = false;
		}
	}
	return bResult;
}
	

std::string Bf2h5Calculator::whatAreYouDoing(void) {
//	pthread_mutex_lock (&calculationMapMutex);
	for (calculationMap::const_iterator it = itsData.begin(); it != itsData.end(); ++it) {
		if (!(it->second.empty())) {
			char buf[10];
			std::string ss("Calculator says: I am still processing block ");
			sprintf(buf, "%ld, ", it->first);
			ss += buf;
			ss += " and subband(s) ";
				
			for (std::deque<std::pair<uint8_t, BFRawFormat::Sample *> >::const_iterator cit = it->second.begin();
							cit != it->second.end(); ++cit) {
								sprintf(buf, "%u, ", cit->first);
								ss += buf;
							}
							pthread_mutex_unlock(&calculationMapMutex);
							return ss;
		}
	}
//	pthread_mutex_unlock(&calculationMapMutex);
	
	for (unsigned short i = 0; i < NUM_CALCULATION_THREADS; ++i) {
		if (thread_data_array[i].busy == true) {
			char buf[10];
			std::string ss("Calculator says: my thread ");
			sprintf(buf, "%d", i);
			ss += buf;
			ss += " is still processing block ";
			sprintf(buf, "%ld", thread_data_array[i].blockNr);
			ss += buf;
			ss += " and subband ";
			sprintf(buf, "%u", thread_data_array[i].subbandNr);
			ss += buf;
			return ss;
		}
	}
	return std::string("Calculator says: I don't know what I am doing, but I'm surely busy with something...");
}


void Bf2h5Calculator::startProcessing(void) {
	// start the calculation threads
	for (unsigned short threadIdx = 0; threadIdx < NUM_CALCULATION_THREADS; ++threadIdx) {
		if (pthread_create(&itsCalculationThread[threadIdx], NULL, startInternalThread, (void *) &thread_data_array[threadIdx]) != 0) {
			cerr << "Bf2h5Calculator::startProcessing, ERROR, could not start calculation thread " << static_cast<int>(threadIdx) << endl;
		}
	}
}


void * Bf2h5Calculator::doDownSampleSingleSubband(void *threaddata) {
	thread_data *tdata(0);
	
  while(!itsStopProcessing) {
		
		pthread_mutex_lock(&calculationMapMutex);
		while ((level < 1) && (!itsStopProcessing))
			pthread_cond_wait(&condition, &calculationMapMutex);
	
		if ((level > 0) && (!itsStopProcessing)) {
		// grab one subband of the data
			tdata = reinterpret_cast<thread_data *>(threaddata);
			calculationMap::iterator firstBlock = itsData.find(currentBlockNr);
			if (firstBlock != itsData.end()) {
				std::deque<std::pair<uint8_t, BFRawFormat::Sample *> > &blockDeque = firstBlock->second;
				tdata->busy = true;
				tdata->blockNr = firstBlock->first;
				tdata->subbandNr = blockDeque.front().first;
				tdata->input_data = blockDeque.front().second;
				tdata->subband_output_data = dataBlockOutput[tdata->subbandNr];
				blockDeque.pop_front();
				if (blockDeque.empty()) {
					itsData.erase(itsData.begin());
				}
				--level;
				pthread_mutex_unlock(&calculationMapMutex);
		
		// do the actual processing of the data (mutex is unlocked)
				uint32_t xx_intensity(0), yy_intensity(0);
				uint64_t start(0);
	
				for ( uint32_t count = 0; count < itsSingleSubbandNrOutputSamples; ++count ) // count loops over all output samples
				{
					tdata->subband_output_data[count] = 0;
					for ( uint64_t idx = start; idx < (start + itsDownSampleFactor); ++idx ) // loop over nr of samples defined by downsampling factor
					{
						xx_intensity = (uint32_t)(real(tdata->input_data[ idx ].xx) * real(tdata->input_data[ idx ].xx) +
								imag(tdata->input_data[ idx ].xx) * imag(tdata->input_data[ idx ].xx) ); // this will be max 33 bits integer
						yy_intensity = (uint32_t)(real(tdata->input_data[ idx ].yy) * real(tdata->input_data[ idx ].yy) +
								imag(tdata->input_data[ idx ].yy) * imag(tdata->input_data[ idx ].yy) );
						tdata->subband_output_data[count] += (float)xx_intensity + (float)yy_intensity;
            //TODO: check if this intensity data needs to be divided by itsDownSampleFactor to get averaged value
					}
					start += itsDownSampleFactor;
				}
	
	//  keep track of finished subbands
				itsParent->calculatorDataReady(tdata->blockNr, tdata->subbandNr, tdata->subband_output_data); // signal itsParent app to write the data
				subbandReady[tdata->subbandNr] = true;
				checkIfBlockComplete(); // TODO: do this somewhere else?
				tdata->busy = false;
      }
      else { // didn't find data for current block
        pthread_mutex_unlock(&calculationMapMutex);
        return 0;
      }
    }
		else {
      pthread_mutex_unlock(&calculationMapMutex);
      return 0;
    }
	}
  return 0;
}


void Bf2h5Calculator::checkIfBlockComplete(void) {
	while (1) {
	if (!checking_completeness) { // added for thread safety (it could be that a thread becomes ready when just checking completeness for another thread, then we have to wait for the first check to complete before doing the next check
		checking_completeness = true;
		for (uint8_t i=0; i < nrOfSubbands; ++i) {
		if (subbandReady[i] == false) {
			checking_completeness = false;
			return; // we are not ready with the current block
		}
	}

	// apparently all subbands for this block are done
	for (uint8_t i=0; i < nrOfSubbands; ++i) { // set subbandReady array to all false for next data block
		subbandReady[i] = false;
	}
	//lastReadyBlock = currentBlockNr++;
	itsParent->blockComplete(++currentBlockNr); // signal parent
	checking_completeness = false;
	return;
	}
	usleep(100);
	}
}


void Bf2h5Calculator::showStatus(void) {
	if (!itsStopProcessing) {
		cout << "Calculator busy with block: " << currentBlockNr << endl;
	}
	else {
		cout << "Calculator has stopped, status of its threads and data follow below" << endl;
	}
	for (unsigned short threadIdx = 0; threadIdx < NUM_CALCULATION_THREADS; ++threadIdx) {
		cout << "thread[" << threadIdx << "]";
		if (thread_data_array[threadIdx].busy == true) {
			cout << " is busy with: block=" << thread_data_array[threadIdx].blockNr << ", subband=" << thread_data_array[threadIdx].subbandNr << ", input data 1st sample xx=" << thread_data_array[threadIdx].input_data->xx << ", yy=" << thread_data_array[threadIdx].input_data->yy << endl;
		}
		else {
			cout << "is idle." << endl;
		}
	}
	cout << "writemap contains: " << endl;
	pthread_mutex_lock (&calculationMapMutex);
	for (calculationMap::const_iterator it = itsData.begin(); it != itsData.end(); ++it) {
		cout << "block " << it->first;
		if (it->second.empty()) {
			cout << " has no subbands left" << endl;
		}
		else {
			cout << ", subbands: ";
			for (std::deque<std::pair<uint8_t, BFRawFormat::Sample *> >::const_iterator cit = it->second.begin(); cit != it->second.end(); ++cit) {
				cout << static_cast<int>(cit->first) << ",";
			}
		}
	}
	pthread_mutex_unlock(&calculationMapMutex);
	cout << "subband(s) that still need processing for the current block: " << endl;
	for (uint8_t i=0; i < nrOfSubbands; ++i) {
		if (subbandReady[i] == false) {
			cout << static_cast<int>(i) << ",";
		}
	}
}

