/*------------------------------------------------------------------------*
| $Id::$                                                                  |
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

#ifndef _Bf2h5Calculator_
#define _Bf2h5Calculator_

#include <pthread.h>
#include <map>
#include <deque>
#include <string>

#include <BFRawFormat.h>

class BF2H5;
//                 block                               subband       pointer to data
typedef std::map<long int, std::deque<std::pair<uint8_t, BFRawFormat::Sample *> > > calculationMap;

#define NUM_CALCULATION_THREADS 4
#define NUM_OUTPUT_BUFFERS 2

/*!
  \class Bf2h5Calculator
  \ingroup DAL
  \ingroup data_common
  \author Alwin de Jong
*/
class Bf2h5Calculator
{
 public:

  // === Construction ===========================================================

  //! Argumented constructor
  Bf2h5Calculator (BF2H5 *parent,
		   uint8_t nr_subbands,
		   uint32_t nr_samples_subband);

  // === Destruction ============================================================

  //! Default destructor
  ~Bf2h5Calculator();

  // === Methods ================================================================
  
  //! Allocate memory
  void allocateMemory(void);
  //		bool memoryAllocated(void) const {return memAllocOK;};
  
  //! Calculate the numer of the data block
  void calculateDataBlock (long int blockNr,
			   BFRawFormat::Sample *sampleData);
  void subbandWritten(/*unsigned int blockNr,*/ uint8_t subband); // is called by bf2h5 to signal that the subband has been written by HDF5Writer
  //! Enable the processing of datablock
  void startProcessing(void);
  bool stop(void); // stops the processing of datablocks
  bool stillProcessing(void); // checks if calculator is still busy
  std::string whatAreYouDoing(void); // prints out what the calculator thinks it's doing
  void showStatus(void);
  //    void wakeUpCall(void);
  
 private:
  //		bool unscheduledDataLeft(void); // checks if there are still data blocks that need to be scheduled for processing
  void checkIfBlockComplete(void); //checks if a complete block of subbands has been calculated
  
  struct thread_data // each calculation thread uses one of these structs
  {
    bool busy; // is this thread currently being used
    long int blockNr;
    uint8_t subbandNr; // the subband being processed by this thread
    BFRawFormat::Sample *input_data; // pointer to the input data block
    float * subband_output_data; // pointer into output buffer where the calculated ata for this subband needs to be written
    Bf2h5Calculator * This;
  } thread_data_array[NUM_CALCULATION_THREADS];
  
  static void * startInternalThread(void * tData) { reinterpret_cast<thread_data *>(tData)->This->doDownSampleSingleSubband(tData); return NULL;}
  
  void * doDownSampleSingleSubband(void *); // the actual thread that does the downsample calculation for a single subband
  
 private:
  unsigned level;
  //		bool memAllocOK; // No errors in allocation memory?
  BF2H5 * itsParent; // parent application BF2H5
  unsigned short itsDownSampleFactor;
  uint8_t nrOfSubbands;
  uint32_t nrSamplesPerSubband;
  bool * subbandReady; // points to array of flags marking subbands that have been written
  bool checking_completeness;
  // the parallel threads for calculation
  pthread_t itsCalculationThread[NUM_CALCULATION_THREADS]; // the calculation threads
  // the tread_data_array contain input, settings and output data for that tread
  bool itsStopProcessing;
  long int currentBlockNr;//, lastReadyBlock;
  uint32_t itsSingleSubbandNrOutputSamples; // the size in float units of a single subband output data block
  float ** dataBlockOutput; // the pointers to the output buffers for output data. Pointer to pointer to single subband output buffer
  
  // itsDatamap is protected by the calculationMapMutex and the pthread condition
  calculationMap itsData; // first = blockNr; second.first = subbandNr; second.second = pointer to start sample of one subband datablock
  pthread_mutex_t calculationMapMutex;
  pthread_cond_t  condition;
};

#endif // _Bf2h5Calculator_
