/***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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

/*!
  \file tbb2h5.cpp
  
  \ingroup DAL

  \brief Write TBB time-series data into an HDF5 file.

  \author Joseph Masters

*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include "dal.h"
#endif

#ifndef DALDATASET_H
#include "dalDataset.h"
#endif

#ifndef TIMESERIES_H
#include "timeseries.h"
#endif

bool new_station( vector<int> vec, int station )
{
  for( unsigned int ss = 0; ss < vec.size(); ss++ )
  {
    if ( station == vec[ss] )
      return false;
  }
  return true;
}

int main(int argc, char *argv[])
{

  // parameter check
  if ( argc < 3 )
  {
     cout << endl << "Too few parameters..." << endl << endl;
     cout << "The first parameter is the output dataset name.\n";
     cout << "The remaining parameters are the raw TBB input file names.\n";
     cout << endl;
     return FAIL;
  }

  dalDataset * dataset;
  dataset = new dalDataset( argv[1], "HDF5" );

  //
  /////////////////////////////////////////
  // read the RAW TBB input data
  /////////////////////////////////////////
  //
  ifstream::pos_type size=0;		// buffer size

  UInt32 payload_crc;

  vector<string> myfiles;

  for ( int tbb=2; tbb<argc; tbb++)
  {
    myfiles.push_back( argv[tbb] );
  }
  vector<int> stations;
  dalGroup * stationGroup;

  bool bigendian = BigEndian();

  for( unsigned int cc = 0; cc < myfiles.size(); cc++ )
  {

   // declare handle for the input file
   fstream file (myfiles[cc].c_str(), ios::binary|ios::in);

   bool first_sample = true;

   if (file.is_open())
   {
    TransientSample tran_sample;
    SpectralSample spec_sample;
    TBB_Header header;
    file.seekg (0, ios::beg);
    int counter=0;
    int offset=0;

    dalArray * sarray;
    vector<int> cdims;
    cdims.push_back(CHUNK_SIZE);

    // define dimensions of array
    vector<int> dims;
    dims.push_back(0);

    // loop through the file
    while ( !file.eof() )
    {
      counter++;

      //
      // read 88-byte TBB frame header
      //
      file.read(reinterpret_cast<char *>(&header), sizeof(header));

      // reverse fields if big endian
      if ( bigendian )
      {
	header.seqnr = Int32Swap( header.seqnr );
	header.sample_nr = Int32Swap( header.sample_nr );
	header.n_samples_per_frame = 
	Int16Swap( header.n_samples_per_frame);
	header.n_freq_bands = Int16Swap( header.n_freq_bands );
      }

      if ( new_station( stations, header.stationid ) )
      {
        stations.push_back( header.stationid );
	char * stationstr = new char[10];
	sprintf( stationstr, "Station%03d", header.stationid );
        stationGroup = dataset->createGroup( stationstr );
	cout << "CREATED New station group: " << string(stationstr) << endl;
	delete [] stationstr;
      }

      // set the STATION_ID, SAMPLE_FREQ and DATA_LENGTH attributes
      //    for the ANTENNA table
      if ( first_sample )
      {
	if ( 0!=header.n_freq_bands )
	{
	}
	else
	{
	  char uid[10];
	  sprintf(uid, "%03d%03d%03d",
                  header.stationid, header.rspid, header.rcuid);
	  vector<int> firstdims;
	  firstdims.push_back( 0 );
	  short nodata[0];
	  sarray =
	    stationGroup->createShortArray( string(uid),firstdims,nodata,cdims );

	  string telescope = "LOFAR";
	  string observer = "J.S. Masters";
	  string project = "Transients";
	  string observation_id = "1287";
	  string observation_mode = "TransientDetection";
	  string trigger_type = "Unknown";
	  double trigger_offset[1] = { 0 };
	  int triggered_antennas[1] = { 0 };
	  double beam_direction[2] = { 0, 0 };

	  // Add attributes to "Station" group
	  stationGroup->setAttribute_string("TELESCOPE", telescope );
	  stationGroup->setAttribute_string("OBSERVER", observer );
	  stationGroup->setAttribute_string("PROJECT", project );
	  stationGroup->setAttribute_string("OBS_ID", observation_id );
	  stationGroup->setAttribute_string("OBS_MODE", observation_mode );
	  stationGroup->setAttribute_string("TRIG_TYPE", trigger_type );
	  stationGroup->setAttribute_double("TRIG_OFST", trigger_offset );
	  stationGroup->setAttribute_int(   "TRIG_ANTS", triggered_antennas );
	  stationGroup->setAttribute_double("BEAM_DIR", beam_direction, 2 );

	  unsigned int sid[] = { (unsigned int)(header.stationid) };
	  unsigned int rsp[] = { (unsigned int)(header.rspid) };
	  unsigned int rcu[] = { (unsigned int)(header.rcuid) };
	  double sf[] = { (double)header.sample_freq };
	  unsigned int time[] = { (unsigned int)(header.time) };
	  unsigned int samp_num[] = { (unsigned int)(header.sample_nr) };
	  unsigned int spf[] = { (unsigned int)header.n_samples_per_frame };
	  unsigned int datalen[] = { (unsigned int)0 };
	  unsigned int nyquist_zone[] = { (unsigned int)0 };
	  string feed = "NONE";
	  double apos[3]    = { 0, 0, 0 };
	  double aorient[3] = { 0, 0, 0 };

	  sarray->setAttribute_uint("STATION_ID", sid );
	  sarray->setAttribute_uint("RSP_ID", rsp );
	  sarray->setAttribute_uint("RCU_ID", rcu );
	  sarray->setAttribute_double("SAMPLE_FREQ", sf );
	  sarray->setAttribute_uint("TIME", time );
	  sarray->setAttribute_uint("SAMPLE_NR", samp_num );
	  sarray->setAttribute_uint("SAMPLES_PER_FRAME", spf );
	  sarray->setAttribute_uint("DATA_LENGTH", datalen );
	  sarray->setAttribute_uint("NYQUIST_ZONE", nyquist_zone );
	  sarray->setAttribute_string("FEED", feed );
	  sarray->setAttribute_double("ANT_POSITION", apos );
	  sarray->setAttribute_double("ANT_ORIENTATION", aorient );
	}

	first_sample = false;
      }

      short sdata[ header.n_samples_per_frame];

      // Read Payload
      if ( 0==header.n_freq_bands )
      {
	for (short zz=0; zz < header.n_samples_per_frame; zz++)
        {
	  file.read(reinterpret_cast<char *>(&tran_sample),sizeof(tran_sample));
	  if ( bigendian )  // reverse fields if big endian
		tran_sample.value = Int16Swap( tran_sample.value );

	  sdata[zz] = tran_sample.value;

	}
				
	dims[0] += header.n_samples_per_frame;
	sarray->extend(dims);
	int arraysize = header.n_samples_per_frame;
	sarray->write(offset, sdata, arraysize );
	offset += header.n_samples_per_frame;
			
      }
      else
      {
	Int16 real_part, imag_part;
	for (int ii=0; ii < (header.n_samples_per_frame*2); ii+=2)
	{
	  file.read(reinterpret_cast<char *>(&spec_sample),sizeof(spec_sample));
	  // reverse fields if big endian
	  if ( bigendian )
          {
	    real_part = Int16Swap( real(spec_sample.value) );
	    imag_part = Int16Swap( imag(spec_sample.value) );
	  }
	  else
	  {
	    real_part = real(spec_sample.value);
	    imag_part = imag(spec_sample.value);
	  }
	}

      }
						
      file.read( reinterpret_cast<char *>(&payload_crc), sizeof(payload_crc) );
    } // end while()
 
    file.close();

   } // end file.is_open()
   else
   {
     cout << "Unable to open file " << myfiles[cc] << endl;
   }

  } // end loop over files

  /////////////////////////////////////////
  // create CALIBRATION table
  /////////////////////////////////////////
  //
  dalTable * CalibrationTable = dataset->createTable( "CALIBRATION" );

  // add attributes to CALIBRATION table

  // add columns to CALIBRATION table
  CalibrationTable->addColumn( "ADC2VOLT", dal_DOUBLE );  // simple column
  CalibrationTable->addColumn( "GAIN_CURVE", dal_COMPLEX );
  CalibrationTable->addColumn( "GAIN_FREQS", dal_DOUBLE );
  CalibrationTable->addColumn( "BEAM_SHAPE", dal_COMPLEX );
  CalibrationTable->addColumn( "BEAM_DIRS", dal_DOUBLE );
  CalibrationTable->addColumn( "BEAM_FREQS", dal_DOUBLE );
  CalibrationTable->addColumn( "NOISE_CURV", dal_COMPLEX );
  CalibrationTable->addColumn( "NOISE_FREQ", dal_DOUBLE );

  // Fill CALIBRATION table with data
  const long CALBufferSIZE = 10;
  typedef struct CalStruct {
  		double adc2voltage;
  		dalcomplex gain_curve;
  		double gain_frequencies;
  		dalcomplex beam_shape;
  		double beam_directions;
  		double beam_frequencies;
  		dalcomplex noise_curve;
  		double noise_frequencies;
  } CalStruct;

  CalStruct calibration[ CALBufferSIZE ];
  const int calLOOPMAX = 1;
  for ( int uu=0 ; uu < calLOOPMAX; uu++)
  {
	for (long row=0; row<CALBufferSIZE; row++) {
		calibration[row].adc2voltage = 0;
		calibration[row].gain_curve.r = 1;
		calibration[row].gain_curve.i = 2;
		calibration[row].gain_frequencies = 3;
		calibration[row].beam_shape.r = 4;
		calibration[row].beam_shape.i = 5;
		calibration[row].beam_directions = 6;
		calibration[row].beam_frequencies = 7;
		calibration[row].noise_curve.r = 8;
		calibration[row].noise_curve.i = 9;
		calibration[row].noise_frequencies = 10;
	}
	CalibrationTable->appendRows( calibration, CALBufferSIZE );
  }

  delete CalibrationTable;

  delete dataset;

  cout << "SUCCESS" << endl;
  return SUCCESS;
}
