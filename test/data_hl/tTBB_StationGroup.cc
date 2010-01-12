/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007                                                    *
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

#include <iostream>

#include <dal.h>
#include <dalDataset.h>
#include <TBB_StationGroup.h>

#include <casa/Arrays/ArrayIO.h>
#include <casa/Arrays/Vector.h>
#include <casa/BasicSL/String.h>
#include <casa/Containers/Record.h>
#include <casa/HDF5/HDF5File.h>
#include <casa/HDF5/HDF5Record.h>

// Namespace usage
using std::cerr;
using std::cout;
using std::endl;
using DAL::TBB_StationGroup;

/*!
  \file tTBB_StationGroup.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the TBB_StationGroup class

  \author Lars B&auml;hren

  \date 2007/12/10

  <h3>Usage</h3>

  To run the test program use:
  \verbatim
  tTBB_StationGroup <filename>
  \endverbatim
  where the <i>filename</i> points to an existing HDF5 time-series dataset.
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new TBB_StationGroup object

  This function should provide tests for all the available constructors to a
  new DAL::TBB_StationGroup object.

  \return nofFailedTests -- The number of failed tests.
*/
int test_constructors ()
{
  std::cout << "\n[tTBB_StationGroup::test_constructors]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  herr_t h5error;
  std::string filename ("tTBB_StationGroup.h5");
  std::string groupname ("Station001");
  
  // Open/Create HDF5 file _________________________________

  fileID = H5Fcreate (filename.c_str(),
		      H5F_ACC_TRUNC,
		      H5P_DEFAULT,
		      H5P_DEFAULT);
  
  if (fileID < 0) {
    std::cerr << "ERROR : Failed to open/create file." << endl;
    return -1;
  }

  std::cout << "-- Filename  = " << filename  << std::endl;
  std::cout << "-- File ID   = " << fileID    << std::endl;
  std::cout << "-- Groupname = " << groupname << std::endl;

  // Test default constructor ______________________________

  cout << "[1] Testing default constructor ..." << endl;
  try {
    TBB_StationGroup group;
    group.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  // Construction with location and name ___________________

  cout << "[2] Testing argumented constructor ..." << endl;
  try {
    TBB_StationGroup group (fileID,groupname);
    group.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] Construction from file ID and station ID ..." << endl;
  try {
    unsigned int stationID (10);
    TBB_StationGroup group (fileID, stationID,true);
    group.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  // release HDF5 object identifiers _______________________

  h5error = H5Fclose (fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new TBB_StationGroup object

  This function should provide tests for all the available constructors to a
  new DAL::TBB_StationGroup object.

  \param filename -- Data file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_constructors (std::string const &filename)
{
  std::cout << "\n[tTBB_StationGroup::test_constructors]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  herr_t h5error;
  std::string groupname;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open HDF5 file and embedeed groups ____________________

  fileID = H5Fopen (filename.c_str(),
		    H5F_ACC_RDWR,
		    H5P_DEFAULT);

  if (fileID > 0) {
    DAL::h5get_names (names,fileID,H5G_GROUP);
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (names.size() > 0) {
    it = names.begin();
    groupname = *it;
  } else {
    std::cerr << "Skipping tests - no station group found." << endl;
    return -1;
  }

  std::cout << "-- Filename        = " << filename  << std::endl;
  std::cout << "-- File identifier = " << fileID    << std::endl;
  std::cout << "-- Station groups  = " << names     << std::endl;
  std::cout << "-- Groupname       = " << groupname << std::endl;

  //__________________________________________________________________
  // TEST: Default constructor

  cout << "[1] Testing default constructor ..." << endl;
  try {
    TBB_StationGroup group;
    group.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // TEST: Argumented constructor using file identifier (as obtained from
  //       previous call to HDF5 library) and group name as input parameters.
  
  cout << "[2] Construction from file ID and groupname ..." << endl;
  try {
    TBB_StationGroup group (fileID, groupname);
    group.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // TEST: Copy constructor

  cout << "[3] Testing copy constructor ..." << endl;
  try {
    cout << "--> creating original object ..." << endl;
    TBB_StationGroup group (fileID, groupname);
    group.summary();
    //
    cout << "--> creating new object as copy ..." << endl;
    TBB_StationGroup groupCopy (group);
    groupCopy.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

/*!
  \brief Test the various methods provided by the class

  The tests included in this function also go through the various methods
  provided to recursively retrieve attributes from the embedded dipole datasets.

  \param name_file    -- Data file used for testing
  \param name_station -- Name of the station group to access

  \return nofFailedTests -- The number of failed tests.
*/
int test_methods (std::string const &filename)
{
  cout << "\n[tTBB_StationGroup::test_methods]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  herr_t h5error;
  std::string groupname;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open HDF5 file and station group ______________________

  std::cout << "--> opening file ..." << std::endl;
  fileID = H5Fopen (filename.c_str(),
		    H5F_ACC_RDWR,
		    H5P_DEFAULT);

  if (fileID > 0) {
    DAL::h5get_names (names,fileID,H5G_GROUP);
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (names.size() > 0) {
    it = names.begin();
    groupname = *it;
  } else {
    std::cerr << "Skipping tests - no station group found." << endl;
    return -1;
  }

  std::cout << "--> opening station group ..." << std::endl;
  TBB_StationGroup group (fileID,groupname);

  //__________________________________________________________________
  // Retrieve attributes attached to the station group

  cout << "[1] Retrieve attributes attached to the station group ..." << endl;
  try
    {
      std::string trigger_type;
      double trigger_offset;
#ifdef HAVE_CASA
      casa::Vector<uint> triggered_antennas;
      casa::Vector<double> station_position_value;
      casa::Vector<casa::String> station_position_unit;
      std::string stationPositionFrame;
      casa::Vector<double> beam_direction_value;
      casa::Vector<casa::String> beam_direction_unit;
      std::string beam_direction_frame;
#else
      std::vector<uint> triggered_antennas;
      std::vector<double> station_position_value;
      std::vector<std::string> station_position_unit;
      std::string stationPositionFrame;
      std::vector<double> beam_direction_value;
      std::vector<std::string> beam_direction_unit;
      std::string beam_direction_frame;
#endif
      // retrieve the values ...
      group.getAttribute ("TRIGGER_TYPE",           trigger_type);
      group.getAttribute ("TRIGGER_OFFSET",         trigger_offset);
      group.getAttribute ("TRIGGERED_ANTENNAS",     triggered_antennas);
      group.getAttribute ("STATION_POSITION_VALUE", station_position_value);
      group.getAttribute ("STATION_POSITION_UNIT",  station_position_unit);
      group.getAttribute ("STATION_POSITION_FRAME", stationPositionFrame);
      group.getAttribute ("BEAM_DIRECTION_VALUE",   beam_direction_value);
      group.getAttribute ("BEAM_DIRECTION_UNIT",    beam_direction_unit);
      group.getAttribute ("BEAM_DIRECTION_FRAME",   beam_direction_frame);
      //... and display them
      cout << "-- TRIGGER_TYPE           = " << trigger_type           << endl;
      cout << "-- TRIGGER_OFFSET         = " << trigger_offset         << endl;
      cout << "-- TRIGGERED_ANTENNAS     = " << triggered_antennas     << endl;
      cout << "-- STATION_POSITION_VALUE = " << station_position_value << endl;
      cout << "-- STATION_POSITION_UNIT  = " << station_position_unit  << endl;
      cout << "-- STATION_POSITION_FRAME = " << stationPositionFrame   << endl;
      cout << "-- BEAM_DIRECTION_VALUE   = " << beam_direction_value   << endl;
      cout << "-- BEAM_DIRECTION_UNIT    = " << beam_direction_unit    << endl;
      cout << "-- BEAM_DIRECTION_FRAME   = " << beam_direction_frame   << endl;
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }

  //__________________________________________________________________
  // Retrieve attributes attached to the dipole datasets

  cout << "[2] Retrieve attributes attached to the dipole datasets ..." << endl;
  try {
#ifdef HAVE_CASA
      // retrieve the values ...
      casa::Vector<uint> station_id;
      casa::Vector<uint> rsp_id;
      casa::Vector<uint> rcu_id;
      casa::Vector<double> sample_freq_value;
      casa::Vector<casa::String> sample_freq_unit;
      casa::Vector<uint> nyquist_zone;
      casa::Vector<uint> time;
      casa::Vector<uint> sample_number;
      casa::Vector<uint> samples_per_frame;
      casa::Vector<int> sample_offset;
      casa::Vector<uint> data_length;
#else
      // retrieve the values ...
      std::vector<uint> station_id;
      std::vector<uint> rsp_id;
      std::vector<uint> rcu_id;
      std::vector<double> sample_freq_value;
      std::vector<std::string> sample_freq_unit;
      std::vector<uint> nyquist_zone;
      std::vector<uint> time;
      std::vector<uint> sample_number;
      std::vector<uint> samples_per_frame;
      std::vector<uint> data_length;
#endif
      // retrieve the values ...
      group.getAttributes("STATION_ID",             station_id);
      group.getAttributes("RSP_ID",                 rsp_id);
      group.getAttributes("RCU_ID",                 rcu_id);
      group.getAttributes("SAMPLE_NUMBER",          sample_number);
      group.getAttributes("SAMPLES_PER_FRAME",      samples_per_frame);
      group.getAttributes("TIME",                   time);
      group.getAttributes("NYQUIST_ZONE",           nyquist_zone);
      group.getAttributes("SAMPLE_FREQUENCY_VALUE", sample_freq_value);
      group.getAttributes("SAMPLE_FREQUENCY_UNIT",  sample_freq_unit);
      // ... and display them
      cout << "-- station_id ........... = " << station_id        << endl;
      cout << "-- rsp_id ............... = " << rsp_id            << endl;
      cout << "-- rcu_id ............... = " << rcu_id            << endl;
      cout << "-- sample_frequency_value = " << sample_freq_value << endl;
      cout << "-- sample_frequency_unit  = " << sample_freq_unit  << endl;
      cout << "-- nyquist_zone ......... = " << nyquist_zone      << endl;
      cout << "-- time ................. = " << time              << endl;
      cout << "-- sample_number ........ = " << sample_number     << endl;
      cout << "-- samples_per_frame .... = " << samples_per_frame << endl;
      cout << "-- sample_offset ........ = " << sample_offset     << endl;
      cout << "-- Data lengths ......... = " << data_length       << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Retrieve additional properties of the dipole datasets
  
  cout << "[3] Retrieve additional properties of the dipole datasets ..." << endl;
  try {
    std::vector<std::string> dipoleNames;
    std::vector<int> dipoleNumbers;
    std::set<std::string> selectedDipoles = group.selectedDipoles();
    //
    group.dipoleNames (dipoleNames);
    group.dipoleNumbers (dipoleNumbers);
    //
    cout << "-- Dipole IDs       = " << dipoleNumbers   << endl;
    cout << "-- Dipole names     = " << dipoleNames     << endl;
    cout << "-- Selected dipoles = " << selectedDipoles << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Test construction of higher-level products from the attributes

#ifdef HAVE_CASA
  cout << "[3] Higher-level products derived from the attributes" << endl;
  try
    {
      casa::MPosition station_position = group.station_position();
      casa::MDirection beam_direction  = group.beam_direction();
      //
      cout << "-- Station position = " << station_position << endl;
      cout << "-- Beam direction   = " << beam_direction   << endl;
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }
#endif

  // release the file ID
  h5error = H5Fclose (fileID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                             test_export2record

/*!
  \brief Test export of the attributes to a casa::Record container

  \param name_file    -- Data file used for testing
  \param name_station -- Name of the station group to access

  \return nofFailedTests -- The number of failed tests.
*/
int test_export2record (std::string const &filename)
{
  std::cout << "\n[test_export2record]\n" << std::endl;

  int nofFailedTests (0);
  hid_t fileID;
  std::string groupname;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open HDF5 file and station group ______________________

  std::cout << "--> opening file ..." << std::endl;
  fileID = H5Fopen (filename.c_str(),
		    H5F_ACC_RDWR,
		    H5P_DEFAULT);

  if (fileID > 0) {
    DAL::h5get_names (names,fileID,H5G_GROUP);
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (names.size() > 0) {
    it = names.begin();
    groupname = *it;
  } else {
    std::cerr << "Skipping tests - no station group found." << endl;
    return -1;
  }

  std::cout << "--> opening station group ..." << std::endl;
  TBB_StationGroup group (fileID,groupname);

  // Perform the tests _____________________________________

  std::cout << "[1] Retreiving attributes of group into record ..." << std::endl;
  try
    {
      // retrieve attributes into record
      casa::Record rec = group.attributes2record ();
      // display the record
      std::cout << rec << std::endl;
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }

  std::cout << "[2] Retreiving attributes of group into record (recursive) ..."
            << std::endl;
  try
    {
      // retrieve attributes into record
      casa::Record rec = group.attributes2record (true);
      // display the record
      std::cout << rec << std::endl;
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }

  std::cout << "[3] Combined multiple station group records ..." << std::endl;
  try
    {
      // retrieve attributes into record
      casa::Record rec = group.attributes2record (true);
      // set up recording holding multiple station group records
      casa::Record record;
      record.defineRecord ("Station001",rec);
      record.defineRecord ("Station002",rec);
      record.defineRecord ("Station003",rec);
      record.defineRecord ("Station004",rec);
      // display the record
      std::cout << record << std::endl;
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_data

/*!
  \brief Test retrieval of the actual time-series data form the dipoles

  \param name_file -- Data file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_data (std::string const &filename)
{
  cout << "\n[tTBB_DipoleDataset::test_data]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  std::string groupname;
  std::set<std::string> names;
  std::set<std::string>::iterator it;
  int start (0);
  int blocksize (1024);

  // Open HDF5 file and station group ______________________

  std::cout << "--> opening file ..." << std::endl;
  fileID = H5Fopen (filename.c_str(),
		    H5F_ACC_RDWR,
		    H5P_DEFAULT);

  if (fileID > 0) {
    DAL::h5get_names (names,fileID,H5G_GROUP);
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (names.size() > 0) {
    it = names.begin();
    groupname = *it;
  } else {
    std::cerr << "Skipping tests - no station group found." << endl;
    return -1;
  }

  std::cout << "--> opening station group ..." << std::endl;
  TBB_StationGroup group (fileID,groupname);

  // Perform the tests _____________________________________

  std::cout << "[1] Retrieve data for all dipoles ..." << std::endl;
  try {
    casa::Matrix<double> data = group.fx (start,
					  blocksize);
    // feedback
    std::cout << "-- Data start     = " << start        << std::endl;
    std::cout << "-- Data blocksize = " << blocksize    << std::endl;
    std::cout << "-- Data array     = " << data.shape() << std::endl;
    std::cout << "-- Data [0,]      = " << data.row(0)  << std::endl;
    std::cout << "-- Data [1,]      = " << data.row(1)  << std::endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Retrieve data for selected dipoles ..." << std::endl;
  try {
    
  }
  catch (std::string message) {
    cerr << message << endl;
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
  
  //________________________________________________________
  // Run the tests

  nofFailedTests += test_constructors ();

  if (haveDataset) {
    // Test for the constructor(s)
    nofFailedTests += test_constructors (filename);
    // Test methods to retrieve attributes
    nofFailedTests += test_methods (filename);
    // Test access to the data
    nofFailedTests += test_data (filename);
  } else {
    std::cout << "\n[tTBB_StationGroup] Skipping tests with input dataset.\n"
	      << endl;
  }

  return nofFailedTests;
}
