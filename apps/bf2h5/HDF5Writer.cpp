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

#include "HDF5Writer.h"
#include <vector>
#include <string>
#include <time.h>
#include <sstream> // needed for type conversion
#include <Interface/Parset.h>
#include <BFRawFormat.h>
#include "bf2h5.h"

extern const char *WRITER_SOFTWARE_VERSION;
using namespace DAL;
using std::vector;
using std::string;

HDF5Writer::HDF5Writer(BF2H5 *parent, const string &output_file, const LOFAR::RTCP::Parset *ps, size_t output_block_size, uint8_t nr_subbands)
	: itsParent(parent), rawfile(0), table(0), stopWriting(false), itsOutputFile(output_file), 
waitForDataTimeOut(0), foundDataForCurrentBlock(false), outputBlockSize(output_block_size), creation_mode("TCP"), nrOfBlocks(0), currentBlockNr(0), 
                   nrOfSubbands(nr_subbands), file_byte_size(0)
{
	subbandReady = new bool [nrOfSubbands];
	for (uint8_t i=0; i < nrOfSubbands; ++i) {
		subbandReady[i] = false;
	}
	
	pthread_mutex_init(&writeMapMutex, NULL);
	
	zeroBlock = new float [outputBlockSize];
	memset(zeroBlock, 0, outputBlockSize * sizeof(float));
	// create output file
	createHDF5File(ps);
}


HDF5Writer::~HDF5Writer() {
	pthread_mutex_destroy(&writeMapMutex);
	delete [] zeroBlock;
	delete [] subbandReady;
	for (uint8_t i = 0; i < nrOfSubbands; ++i) {
		delete table[i];
	}
	delete table;
}

void HDF5Writer::createHDF5File(const LOFAR::RTCP::Parset *ps) {
  std::stringstream sstr; // used for type conversion
  std::string strValue;
	dataset = dalDataset( itsOutputFile.c_str(), "HDF5" );
	const BFRawFormat::BFRaw_Header & header = itsParent->getMainHeader();
    // root-level headers
//	int n_stations = 1;
//	int main_beam_diam      = 0;
//	int bandwidth           = 0; // Total bandwidth (MHz)
//	int breaks_in_data      = 0; // Any breaks in data?
//	int dispersion_measure  = 0;
//		int number_of_samples_per_block = header.nrSubbands * header.nrSamplesPerSubband;
  /*
	int64_t tnos(0);
	if (!itsParent->doDownSampling()) {
		tnos = nrOfBlocks * header.nrSamplesPerSubband;
	}
	const int64_t &total_number_of_samples = tnos;
	*/
    //		double sampling_time   = header.sampleRate;
//	int number_of_beams     = 1;
//	int sub_beam_diameter   = 0; // fwhm of the sub-beams (arcmin)
//	int weather_temperature = 0; // approx. centigrade
//	int weather_humidity    = 0; // approx. %
	uint downsample_factor = itsParent->getDownSampleFactor();

    // write headers using above
  dataset.setAttribute( "ANTENNA_SET", ps->antennaSet() );
/*
  // station clock frequency
  if (header.nrSamplesPerSubband == 155648) strValue = "160MHz";
  else if (header.nrSamplesPerSubband == 196608) strValue = "200MHz";
  else station_clock = "unknown";
*/
  
  // station clock frequency
  uint clock_speed = ps->clockSpeed();
  sstr.str("");
  sstr.clear();
  sstr << clock_speed;
  strValue = sstr.str();
  dataset.setAttribute( "CLOCK_FREQUENCY", strValue);
  dataset.setAttribute( "CLOCK_FREQUENCY_UNIT", string("Hz") );
  dataset.setAttribute( "CREATION_MODE", creation_mode );
  dataset.setAttribute( "DOWNSAMPLE_RATE", &downsample_factor );
	dataset.setAttribute( "FILENAME", itsOutputFile );
	dataset.setAttribute( "FILETYPE", string("bfstation") );
  // get current time = file creation time
  time_t rawtime;
  struct tm * timeinfo;
  char timestr [80];
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime (timestr,80,"%Y-%m-%dT%X",timeinfo );
  dataset.setAttribute( "FILEDATE",  string(timestr) );
  
  dataset.setAttribute( "FILTER_SELECTION", ps->bandFilter() );
  dataset.setAttribute( "GROUPTYPE", string("Root") );
	dataset.setAttribute( "INPUT_FILESIZE", &file_byte_size );
  
    // number of stations
  uint nrOfStations = ps->nrStations();
  sstr.str("");
  sstr.clear();
  sstr << nrOfStations;
  strValue = sstr.str();
  dataset.setAttribute( "NOF_STATIONS", strValue );
  
  dataset.setAttribute( "NOTES", string("") );
  
  // observationID
  sstr.str("");
  sstr.clear();
  sstr << ps->observationID();
  strValue = sstr.str();
  dataset.setAttribute( "OBSERVATION_ID", strValue );
  
  // observation start time
  time_t obsStartTime = ps->startTime() / 86400 + 40587; // convert from unix time to MJD
  sstr.str("");
  sstr.clear();
  sstr << obsStartTime;
  strValue = sstr.str();
  dataset.setAttribute( "OBSERVATION_START_MJD", strValue );
  time ( &obsStartTime );
  tm * ptm = gmtime ( &obsStartTime );
  strftime (timestr,80,"%Y-%m-%dT%X",ptm);
  dataset.setAttribute( "OBSERVATION_START_UTC", string(timestr) );
  dataset.setAttribute( "OBSERVATION_START_TAI", string("") );
  
  // observation start time
  time_t obsStopTime = ps->stopTime()  / 86400 + 40587; // convert from unix time to MJD
  sstr.str("");
  sstr.clear();
  sstr << obsStopTime;
  strValue = sstr.str();
  dataset.setAttribute( "OBSERVATION_END_MJD",strValue );
  time ( &obsStopTime );
  ptm = gmtime ( &obsStopTime );
  strftime (timestr,80,"%Y-%m-%dT%X", ptm);
  dataset.setAttribute( "OBSERVATION_END_UTC", string(timestr) );
  dataset.setAttribute( "OBSERVATION_END_TAI", string("") );
  dataset.setAttribute( "OBSERVER", ps->observerName() );
  dataset.setAttribute( "PIPELINE_NAME", string("") );
  dataset.setAttribute( "PIPELINE_VERSION", string("") );
	dataset.setAttribute( "PROJECT_ID", string("") );
  dataset.setAttribute( "PROJECT_TITLE", ps->projectName() );
  dataset.setAttribute( "PROJECT_PI", string("") );
  dataset.setAttribute( "PROJECT_CO_I", string("") );
  dataset.setAttribute( "PROJECT_CONTACT", ps->contactName() );
	dataset.setAttribute( "PROJECT_DESCRIPTION", string("") );
  // station list
  if (nrOfStations > 0) {
    std::string stations;
    uint station_idx(0);
    for (station_idx = 0; station_idx < nrOfStations-1; ++station_idx) {
      stations += ps->stationName(station_idx) + ",";
    }
    stations += ps->stationName(station_idx);
    dataset.setAttribute( "STATIONS_LIST", stations );
  }
  else {
    dataset.setAttribute( "STATIONS_LIST",  string("no stations defined") );
  }
  dataset.setAttribute( "SYSTEM_VERSION",  string("") );
  dataset.setAttribute( "TARGET", string("") );
  dataset.setAttribute( "TELESCOPE", string("LOFAR") );
  std::cout << WRITER_SOFTWARE_VERSION << std::endl;
  dataset.setAttribute( "WRITER_SOFTWARE_VERSION", string(WRITER_SOFTWARE_VERSION) );
/*
	dataset.setAttribute( "NUMBER_OF_STATIONS", &n_stations );
	dataset.setAttribute( "STATION_LIST", string(header.station) );
    //		dataset.setAttribute_string( "SOURCE", srcvec ); // replaced by TARGET
	dataset.setAttribute( "MAIN_BEAM_DIAM", &main_beam_diam );
	dataset.setAttribute( "BANDWIDTH", &bandwidth );
	dataset.setAttribute( "BREAKS_IN_DATA", &breaks_in_data );
	dataset.setAttribute( "DISPERSION_MEASURE", &dispersion_measure );
// 	dataset.setAttribute( "SECONDS_OF_DATA", &nrOfBlocks );
	dataset.setAttribute( "SAMPLE_RATE", &header.sampleRate );
	dataset.setAttribute( "NOF_SAMPLES_PER_SUBBAND", &header.nrSamplesPerSubband );
	dataset.setAttribute( "TOTAL_NUMBER_OF_SAMPLES", &total_number_of_samples );
	dataset.setAttribute( "NUMBER_OF_BEAMS", &number_of_beams );
	dataset.setAttribute( "SUB_BEAM_DIAMETER", &sub_beam_diameter );
	dataset.setAttribute( "WEATHER_TEMPERATURE", &weather_temperature );
	dataset.setAttribute( "WEATHER_HUMIDITY", &weather_humidity );
*/
	dalGroup * beamGroup;
    
	char * beamstr = new char[10];
    
	int beam_number;
    
	beam_number = 0;
	sprintf( beamstr, "beam%03d", beam_number );
	beamGroup = dataset.createGroup( beamstr );

	float ra_val  = header.beamDirections[beam_number+1][0];
	float dec_val = header.beamDirections[beam_number+1][1];
	beamGroup->setAttribute( "RA", &ra_val, 1 );
	beamGroup->setAttribute( "DEC", &dec_val, 1 );

	int n_subbands[] = { header.nrSubbands };
	beamGroup->setAttribute( "NUMBER_OF_SUBBANDS", n_subbands );
		
		// write the center frequencies of the subbands
	int * center_frequency = new int[header.nrSubbands];
	char * cfName = new char[22];
	for (unsigned int idx=0; idx < header.nrSubbands; idx++)
	{
		center_frequency[idx] = (int)header.subbandFrequencies[ idx ];
		sprintf( cfName, "CENTER_FREQUENCY_SB%03d", idx );
		beamGroup->setAttribute( cfName, &center_frequency[idx] );
	}
	delete [] cfName;
	delete beamGroup;

#ifdef DEBUGGING_MESSAGES
    std::cerr << "CREATED New beam group: " << string(beamstr) << std::endl;
    std::cerr << "   " << header.nrSubbands << " subbands" << std::endl;
#endif

    table = new dalTable * [ header.nrSubbands ];
    char * sbName = new char[8];

    // nrSubbands is actually the number of subbands (see email from J.Romein)
		for (unsigned int idx=0; idx<header.nrSubbands; idx++)
		{
			sprintf( sbName, "SB%03d", idx );
			table[idx] = dataset.createTable( sbName, beamstr );
		}

		for (unsigned int idx=0; idx<header.nrSubbands; idx++)
		{
			if ( itsParent->doDownSampling() || itsParent->doIntensity() )
			{
				table[idx]->addColumn( "TOTAL_INTENSITY_SQUARED", dal_FLOAT );
			}
			else
			{
				table[idx]->addColumn( "X", dal_COMPLEX_SHORT );
				table[idx]->addColumn( "Y", dal_COMPLEX_SHORT );
			}
		}

		delete [] sbName;
		sbName = 0;
		delete [] center_frequency;
		center_frequency = 0;
		delete [] beamstr;
		beamstr = 0;
}


bool HDF5Writer::start(void) {
	stopWriting = false;
#ifdef DEBUGGING_MESSAGES
	cout << "setting attribute EPOCH_UTC to " << itsParent->getEpochUTC() << endl;
	cout << "setting attribute EPOCH_DATE to " << itsParent->getEpochDate() << endl;	
#endif
	dataset.setAttribute( "EPOCH_UTC", itsParent->getEpochUTC() );
	dataset.setAttribute( "EPOCH_DATE", itsParent->getEpochDate() );
	
	if (pthread_create(&itsWriteThread, NULL, StartInternalThread, (void *) this) == 0) {
		return true;
	}
	else {
#ifdef DEBUGGING_MESSAGES
				cout << "HDF5Writer::start, ERROR: main write thread could not be started" << endl;
#endif
		return false;
	}	
}

bool HDF5Writer::stop(void) {
	bool bResult = true;
	int status;
	void *thread_result;
#ifdef DEBUGGING_MESSAGES
	cout << "Stopping thr writer" << endl;
#endif 
	stopWriting = true;
	status = pthread_join (itsWriteThread, &thread_result);
	if (status != 0) {
		bResult = false;
	}
	if (thread_result != NULL) {
		bResult = false;
	}
	return bResult;
}


// gets called for every subband that has been calculated by the calculator
void HDF5Writer::writeSubband(long int blockNr, uint8_t subband, float *calculator_data) {
	std::pair<unsigned int, float *> dataPair(subband, calculator_data);
	pthread_mutex_lock (&writeMapMutex);
 	itsData[blockNr].push_back(dataPair);
	pthread_mutex_unlock(&writeMapMutex);
}

bool HDF5Writer::dataLeft(void) {
	pthread_mutex_lock (&writeMapMutex);
	for (writeMap::const_iterator it = itsData.begin(); it != itsData.end(); ++it) {
		if (!(it->second.empty())) {
			cout << "data left, subband: ";
			cout << static_cast<int>(it->second.front().first);
			cout << ", block: " << currentBlockNr << endl;
			pthread_mutex_unlock(&writeMapMutex);
			return true;
		}
	}
	pthread_mutex_unlock(&writeMapMutex);
	return false;
}


bool HDF5Writer::getDataForCurrentBlock(void) {
	pthread_mutex_lock(&writeMapMutex);
	writeMap::iterator it = itsData.find(currentBlockNr);
	if (it != itsData.end()) {
		if (!(it->second.empty())) {
			dataPair = it->second.front();
			
			foundDataForCurrentBlock = true;
			it->second.pop_front();
			pthread_mutex_unlock(&writeMapMutex);
			return true;
		}
	}
	pthread_mutex_unlock(&writeMapMutex);
	return false; // no data found for current block
}


void HDF5Writer::checkIfBlockComplete(void) {
	for (uint8_t i=0; i < nrOfSubbands; ++i) {
		if (subbandReady[i] == false) {
			return; // we are not ready with the current block
		}
	}
		// this block is done
	startNextBlock();
}

void HDF5Writer::startNextBlock(void) {
	cout << "block " << currentBlockNr << " is done." << endl;
	for (uint8_t i=0; i < nrOfSubbands; ++i) {
		subbandReady[i] = false;
	}
	pthread_mutex_lock(&writeMapMutex);
	itsData.erase(itsData.find(currentBlockNr++));
	pthread_mutex_unlock(&writeMapMutex);
	waitForDataTimeOut = 0;
	foundDataForCurrentBlock = false;
	return;
}


void HDF5Writer::writeData(void) { // this function runs in a separate thread
	while (!stopWriting) {
		if (getDataForCurrentBlock()) {
			table[dataPair.first]->appendRows( dataPair.second, outputBlockSize );
			subbandReady[dataPair.first] = true;
/*#ifdef DEBUGGING_MESSAGES
			cout << "HDF5Writer:Wrote subband " << static_cast<int>(dataPair.first) << " for data block " << currentBlockNr << endl;
#endif*/
			checkIfBlockComplete();
			waitForDataTimeOut = 0;
			usleep(50);
		}
		else { // no data to write, check if subbands received within time limit and go to sleep for a little while
			if (foundDataForCurrentBlock) { // we don't want to skip a block which the calculator hasn't yet started
				if (++waitForDataTimeOut > 25) {
					cout << "HDF5Writer: block " << currentBlockNr << ", skipping subbands: ";
					for (uint8_t sb=0; sb < nrOfSubbands; ++sb) {
						if (subbandReady[sb] == false) {
							table[sb]->appendRows( zeroBlock, outputBlockSize );
							cout << static_cast<int>(sb) << ", ";
						}
					}
					startNextBlock();
				}
			}
			usleep(10000); // wait when the data of the new block hasn't arrived yet
		}
	}
  /*
#ifdef DEBUGGING_MESSAGES
cout << "HDF5Writer is done." << endl;
#endif
  */
}
	
	
	  // ---------------------------------------------------------------- openRawFile

	void HDF5Writer::openRawFile( const char* filename )
	{
		creation_mode = string("FILE");
		if (rawfile) {
			delete rawfile; 
		}
		rawfile = new std::fstream( filename, ios::binary|ios::in );
		rawfile->seekg(0, ios::end); // move to end of file to determine its file size
		file_byte_size = static_cast<unsigned long int>(rawfile->tellg())-2; // see how many bytes in file
		rawfile->seekg(0, ios::beg);  // move to start of file
	}
	

	void HDF5Writer::showStatus(void) {
		cout << "Writer busy with block: " << currentBlockNr << endl;
		if (foundDataForCurrentBlock) {
			cout << "writer did find data for this block" << endl;
		}
		else {
			cout << "writer did not find data for this block" << endl;
		}
		cout << "subbands that still needs processing: " << endl;
		for (uint8_t i=0; i < nrOfSubbands; ++i) {
			if (subbandReady[i] == false) {
				cout << static_cast<int>(i) << ",";
			}
		}
		pthread_mutex_lock (&writeMapMutex);
		cout << "data left: ";
		for (writeMap::const_iterator it = itsData.begin(); it != itsData.end(); ++it) {
			if (!(it->second.empty())) {
				cout << "block: " << it->first << ", subbands: ";
				for (std::deque<std::pair<uint8_t, float *> >::const_iterator cit = it->second.begin();
								 cit != it->second.end() ; ++cit ) {
									 cout << static_cast<int>(it->second.front().first) << ",";
								 }
			}
		}
		pthread_mutex_unlock(&writeMapMutex);
		return;
	}
	
