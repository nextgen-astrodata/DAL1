/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#include <casa/Arrays/ArrayIO.h>
#include <casa/Arrays/Matrix.h>
#include <casa/BasicSL/String.h>
#include <casa/HDF5/HDF5File.h>
#include <casa/HDF5/HDF5Record.h>

#include <TBB_Timeseries.h>

using std::cerr;
using std::cout;
using std::endl;
using DAL::TBB_Timeseries;

/*!
  \file tTBB_Timeseries.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the TBB_Timeseries class

  \author Lars B&auml;hren

  \date 2008/02/06

  <h3>Usage</h3>

  To run the test program use:
  \verbatim
  tTBB_Timeseries <filename>
  \endverbatim
  where the <i>filename</i> points to an existing HDF5 time-series dataset.
*/

//_______________________________________________________________________________
//                                                                   test_dataset

/*!
  \brief Check the dataset provided for testing

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_dataset (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_dataset]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  hid_t groupID;
  herr_t h5error;
  std::set<std::string> stationGroups;
  std::set<std::string> dipoleDatasets;
  std::set<std::string>::iterator it;

  // Open HDF5 file and embedeed groups ____________________

  fileID = H5Fopen (filename.c_str(),
		    H5F_ACC_RDWR,
		    H5P_DEFAULT);

  if (fileID > 0) {
    DAL::h5get_names (stationGroups,fileID,H5G_GROUP);
    //
    if (stationGroups.size() > 0) {
      it      = stationGroups.begin();
      groupID = H5Gopen (fileID,
			 it->c_str(),
			 H5P_DEFAULT);
    } else {
      std::cerr << "Skipping tests - unable to open group." << endl;
      return -1;
    }
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (groupID > 0) {
    DAL::h5get_names (dipoleDatasets,groupID,H5G_DATASET);
  } else {
    std::cerr << "Skipping tests - unable to open group." << endl;
    return -1;
  }

  std::cout << "-- Station groups  = " << stationGroups  << std::endl;
  std::cout << "-- Dipole datasets = " << dipoleDatasets << std::endl;

  // Attributes attached to the root group _________________

  

  // Release HDF5 object identifiers _______________________
  
  h5error = H5Fclose (fileID);
  h5error = H5Gclose (groupID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_construction

/*!
  \brief Test constructors for a new TBB_Timeseries object

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_construction (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_construction]\n" << endl;

  int nofFailedTests (0);
  int nofIterations (500);

  //__________________________________________________________________
  // Test the default constructor

  cout << "[1] Testing default constructor ..." << endl;
  try {
    TBB_Timeseries ts;
    //
    ts.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Test the repeated usage of the default constructor

  cout << "[2] Repeated testing default constructor ..." << endl;
  try {
    hid_t fileID;
    for (int n(0); n<nofIterations; n++) {
      // construct new object
      TBB_Timeseries ts;
      // probe the newly created object
      fileID = ts.locationID();
    }
    std::cout << "--> " << nofIterations << " object constructions completed."
	      << std::endl;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Test the argumented constructor taking filename as input

  cout << "[3] Testing argumented constructor ..." << endl;
  try {
    TBB_Timeseries ts (filename);
    //
    ts.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Test the copy constructor

  cout << "[4] Testing copy constructor ..." << endl;
  try {
    cout << "--> creating original object ..." << endl;
    TBB_Timeseries ts (filename);
    ts.summary();
    //
    cout << "--> creating new object by copy ..." << endl;
    TBB_Timeseries tsCopy (ts);
    tsCopy.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test the various methods of the class

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_methods (std::string const &filename)
{
  cout << "\n[test_methods]\n" << endl;

  int nofFailedTests (0);
  TBB_Timeseries ts (filename);

  assert (nofFailedTests==0);

  cout << "[1] Retrieve attributes attached to the root group ..." << endl;
  try {
    std::string telescope;
    std::string observer;
    std::string project;
    std::string observationID;

    ts.getAttribute("TELESCOPE",     telescope);
    ts.getAttribute("OBSERVER",      observer);
    ts.getAttribute("PROJECT_TITLE", project);
    ts.getAttribute("OBSERVATION_ID",observationID);

    cout << "-- TELESCOPE ...... = " << telescope      << endl;
    cout << "-- OBSERVER ....... = " << observer       << endl;
    cout << "-- PROJECT ........ = " << project        << endl;
    cout << "-- OBSERVATION_ID   = " << observationID  << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  assert (nofFailedTests==0);
  
  cout << "[2] Retrieve attributes attached to the station groups ..." << endl;
  try {
    uint nelem = ts.nofStationGroups();
    std::string group_name;
#ifdef HAVE_CASA
    casa::Vector<casa::String> trigger_type (nelem);
    casa::Vector<double> trigger_offset (nelem,0);
    casa::Matrix<double> station_position_value;
    casa::Matrix<casa::String> station_position_unit;
    casa::Vector<casa::MPosition> station_position;
    casa::Vector<casa::MDirection> beam_direction;
    //
    trigger_type           = ts.trigger_type();
    trigger_offset         = ts.trigger_offset();
    station_position_value = ts.station_position_value();
    station_position_unit  = ts.station_position_unit();
    station_position       = ts.station_position();
    beam_direction         = ts.beam_direction();
    //
    cout << "-- TRIGGER_TYPE ......... = " << trigger_type            << endl;
    cout << "-- TRIGGER_OFFSET ....... = " << trigger_offset          << endl;
    cout << "-- STATION_POSITION ..... = " << station_position        << endl;
    cout << "-- BEAM_DIRECTION ....... = " << beam_direction          << endl;
#else
    std::vector<std::string> trigger_type = ts.trigger_type();
    std::vector<double> trigger_offset    = ts.trigger_offset();
    //
    cout << "-- TRIGGER_TYPE ......... = " << trigger_type      << endl;
    cout << "-- TRIGGER_OFFSET ....... = " << trigger_offset    << endl;
#endif
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  assert (nofFailedTests==0);
  
  cout << "[3] Retrieve attributes attached to the dipole datasets ..." << endl;
  try {
#ifdef HAVE_CASA
    casa::Vector<uint> time;
    casa::Vector<uint> sample_number;
    casa::Vector<int> sample_offset;
    casa::Vector<uint> nyquist_zone;
    casa::Vector<double> sample_frequency_value;
    casa::Vector<casa::String> sample_frequency_unit;
    casa::Vector<casa::MFrequency> sample_frequency;
    //
    time                   = ts.time();
    sample_number          = ts.sample_number();
    sample_offset          = ts.sample_offset();
    nyquist_zone           = ts.nyquist_zone();
    sample_frequency_value = ts.sample_frequency_value();
    sample_frequency_unit  = ts.sample_frequency_unit();
    sample_frequency       = ts.sample_frequency();
    //
    cout << "-- TIME ................. = " << time                   << endl;
    cout << "-- SAMPLE_NUMBER ........ = " << sample_number          << endl;
    cout << "-- SAMPLE_OFFSET ........ = " << sample_offset          << endl;
    cout << "-- NYQUIST_ZONE ......... = " << nyquist_zone           << endl;
    cout << "-- SAMPLE_FREQUENCY_VALUE = " << sample_frequency_value << endl;
    cout << "-- SAMPLE_FREQUENCY_UNIT  = " << sample_frequency_unit  << endl;
    cout << "-- SAMPLE_FREQUENCY       = " << sample_frequency       << endl;
#else
    std::vector<uint> time;
    std::vector<uint> sample_number;
    std::vector<uint> nyquist_zone;
    std::vector<double> sample_frequency_value;
    std::vector<std::string> sample_frequency_unit;
    //
    time                   = ts.time();
    sample_number          = ts.sample_number();
    nyquist_zone           = ts.nyquist_zone();
    sample_frequency_value = ts.sample_frequency_value();
    sample_frequency_unit  = ts.sample_frequency_unit();
    //
    cout << "-- TIME ................. = " << time                   << endl;
    cout << "-- SAMPLE_NUMBER ........ = " << sample_number          << endl;
    cout << "-- NYQUIST_ZONE ......... = " << nyquist_zone           << endl;
    cout << "-- SAMPLE_FREQUENCY_VALUE = " << sample_frequency_value << endl;
    cout << "-- SAMPLE_FREQUENCY_UNIT  = " << sample_frequency_unit  << endl;
#endif
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test export of the attribute values to a casa::Record object

  \param filename -- Name of the HDF5 file, within which the dataset is located

  \return nofFailedTests -- The number of failed tests.
*/
int test_attributes2record (std::string const &filename)
{
  cout << "\n[test_attributes2record]\n" << endl;

  int nofFailedTests (0);
  TBB_Timeseries ts (filename);

  cout << "[1] Retreiving attributes of group into record ..." << endl;
  try {
    // retrieve attributes into record
    casa::Record rec = ts.attributes2record ();
    // display the contents of the record
    std::cout << rec << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Retreiving attributes of group into record (recursive) ..."
       << endl;
  try {
    // retrieve attributes into record
    casa::Record rec = ts.attributes2record (true);
    // display the contents of the record
    std::cout << rec << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
  
  cout << "[3] Retrieve attributes into a header record for the DR ..." << endl;
  try {
    // retrieve the attrbutes into the record
    casa::Record rec = ts.attributes2headerRecord();
    // display the contents of the record
    std::cout << rec << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] Test operations on parameters " << endl;
  try {
    uint min_times      = min(ts.time());
    uint min_dataLength = min(ts.data_length());
    //
    cout << "-- min(times)       = " << min_times      << endl;
    cout << "-- min(data_length) = " << min_dataLength << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test retrieval of TBB data

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_data (std::string const &filename)
{
  cout << "\n[test_data]\n" << endl;

  int nofFailedTests = 0;
  int start          = 0;
  int nofSamples     = 1024;
  TBB_Timeseries timeseries (filename);

  cout << "[1] Retrieve time-series data without channel selection"
       << endl;
  try {
    casa::Matrix<double> data = timeseries.fx (start,
					       nofSamples);
    // feedback
    cout << "-- Data start     = " << start        << endl;
    cout << "-- Data blocksize = " << nofSamples   << endl;
    cout << "-- Data array     = " << data.shape() << endl;
    cout << "-- Data [0,]      = " << data.row(0)  << endl;
    cout << "-- Data [1,]      = " << data.row(1)  << endl;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc,
          char *argv[])
{
  int nofFailedTests (0);
  bool haveDataset (true);
  std::string filename ("UNDEFINED");

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }

  std::cout << "[tTBB_Timeseries]" << std::endl;
  std::cout << "-- have dataset = " << haveDataset << std::endl;
  std::cout << "-- Filename     = " << filename    << std::endl;

  //________________________________________________________
  // Run the tests

  if (haveDataset) {
    // Some basic tests on the provided dataset
    nofFailedTests += test_dataset (filename);
    // Test constructors for TBB_Timeseries object
    nofFailedTests += test_construction (filename);
    nofFailedTests += test_methods (filename);
    nofFailedTests += test_attributes2record (filename);
    nofFailedTests += test_data (filename);
  } else {
    std::cout << "\n[tTBB_Timeseries] Skipping tests with input dataset.\n"
	      << endl;
  }
  
  return nofFailedTests;
}
