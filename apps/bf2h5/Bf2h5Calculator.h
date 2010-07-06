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

#ifndef BF2H5CALCULATOR_H
#define BF2H5CALCULATOR_H

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

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class Bf2h5Calculator
    
    \ingroup DAL
    \ingroup dal_apps
    
    \author Alwin de Jong
  */
  class Bf2h5Calculator
  {
  public:
    
    // === Construction =========================================================
    
    //! Argumented constructor
    Bf2h5Calculator (BF2H5 *parent,
		     uint8_t nofSubbands,
		     uint32_t nr_samples_subband);
    
    // === Destruction ==========================================================
    
    //! Default destructor
    ~Bf2h5Calculator();
    
    // === Methods ==============================================================
    
    //! Allocate memory
    void allocateMemory (void);
    
    //! Calculate the numer of the data block
    void calculateDataBlock (long int blockNr,
			     BFRawFormat::Sample *sampleData);
    
    //! Signal that the subband has been written
    void subbandWritten(/*unsigned int blockNr,*/ uint8_t subband);
    
    //! Enable the processing of datablock
    void startProcessing(void);
    
    //! Stop the processing of datablocks
    bool stop (void);
    
    //! Check if calculator is still busy
    bool stillProcessing (void);
    
    //! Print out what the calculator thinks it's doing
    std::string whatAreYouDoing (void);
    
    //! Show the status of the calculator
    void showStatus(void);
    
  private:
    //		bool unscheduledDataLeft(void); // checks if there are still data blocks that need to be scheduled for processing
    
    //! Check if a complete block of subbands has been calculated
    void checkIfBlockComplete (void);
    
    //! Each calculation thread uses one of these structs
    struct thread_data
    {
      //! Is this thread currently being used?
      bool busy;
      //! Number of the block being processed
      long int blockNr;
      //! Number of the subband being processed by this thread
      uint8_t subbandNr;
      //! Pointer to the input data block
      BFRawFormat::Sample *input_data;
      float * subband_output_data; // pointer into output buffer where the calculated ata for this subband needs to be written
      Bf2h5Calculator * This;
    } thread_data_array[NUM_CALCULATION_THREADS];
    
    static void * startInternalThread(void * tData)
    {
      reinterpret_cast<thread_data *>(tData)->This->doDownSampleSingleSubband(tData);
      return NULL;
    }
    
    void * doDownSampleSingleSubband(void *); // the actual thread that does the downsample calculation for a single subband
    
  private:
    
    unsigned level;
    //! Parent application BF2H5    
    BF2H5 * itsParent;
    unsigned short itsDownSampleFactor;
    uint8_t nrOfSubbands;
    uint32_t nrSamplesPerSubband;
    //! Points to array of flags marking subbands that have been written
    bool * subbandReady;
    bool checking_completeness;
    // the parallel threads for calculation
    pthread_t itsCalculationThread[NUM_CALCULATION_THREADS]; // the calculation threads
    // the tread_data_array contain input, settings and output data for that tread
    bool itsStopProcessing;
    //! Number of the currently processed block
    long int currentBlockNr;
    //! The size in float units of a single subband output data block
    uint32_t itsSingleSubbandNrOutputSamples;
    float ** dataBlockOutput; // the pointers to the output buffers for output data. Pointer to pointer to single subband output buffer
    
    // itsDatamap is protected by the calculationMapMutex and the pthread condition
    calculationMap itsData; // first = blockNr; second.first = subbandNr; second.second = pointer to start sample of one subband datablock
    pthread_mutex_t calculationMapMutex;
    pthread_cond_t  condition;
  };
  
} // Namespace DAL -- end

#endif  /* BF2H5CALCULATOR_H */
