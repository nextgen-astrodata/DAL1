/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 1159 2007-12-21 15:40:14Z baehren                  $ |
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

#include <hdf5.h>

#include <casa/BasicSL/String.h>
#include <casa/Arrays/ArrayIO.h>
#include <casa/Arrays/Vector.h>
#include <casa/HDF5/HDF5File.h>
#include <casa/HDF5/HDF5Record.h>

#include <Enumerations.h>
#include <TBB_DipoleDataset.h>

using std::cerr;
using std::cout;
using std::endl;

using DAL::TBB_DipoleDataset;

/*!
  \file tTBB_DipoleDataset.cpp

  \ingroup DAL

  \brief A collection of test routines for the DAL::TBB_DipoleDataset class

  \author Lars B&auml;hren

  \date 2008/01/10
  
  <h3>Usage</h3>
  
  To run the test program use:
  \verbatim
  tTBB_DipoleDataset <filename>
  \endverbatim
  where the <i>filename</i> points to an existing HDF5 time-series dataset.
*/

// ==============================================================================
//
//  Helper functions
//
// ==============================================================================

/*!
  \brief Get the names of station group and dipole dataset

  \param filename       -- Name of the HDF5 file
  \retval stationGroup  -- Name of the first station group within the file
  \retval dipoleDataset -- Name of the first dipole dataset within the first
          station group.
*/
void get_names (std::string const &filename,
		std::string &stationGroup,
		std::string &dipoleDataset)
{
  hid_t file_id (0);
  hid_t group_id (0);
  hid_t dataset_id (0);
  std::vector<std::string> names;
  bool status (true);

  /* Open the HDF5 file */
  
  file_id = H5Fopen (filename.c_str(),
		     H5F_ACC_RDONLY,
		     H5P_DEFAULT);

  /* Open the first station group */
  
  if (file_id > 0) {
    status = DAL::h5get_names (names,file_id,H5G_GROUP);
    if (status) {
      group_id = H5Gopen1 (file_id,
			   names[0].c_str());
      stationGroup = names[0];
    }
  }

  /* Open the first dipole dataset */
  
  if (group_id > 0) {
    status = DAL::h5get_names (names, group_id, H5G_DATASET);
    if (status) {
      dataset_id = H5Dopen1 (group_id,
			     names[0].c_str());
      dipoleDataset = names[0];
    }
  }

  /* Release handlers */
  if (file_id > 0)    { H5Fclose(file_id); }
  if (group_id > 0)   { H5Gclose(group_id); }
  if (dataset_id > 0) { H5Dclose(dataset_id);  }
}

// ==============================================================================
//
//  Test routines
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                   test_dataset

/*!
  \brief Test low-level access to the dataset using HDF5 library calls

  \param name_file    -- Name of the HDF5 file, within which the dataset is
         located.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_dataset (std::string const &name_file)
{
  cout << "\n[test_dataset]\n" << endl;

  int nofFailedTests (0);
  bool status (true);
  hid_t file_id (0);
  hid_t group_id (0);
  hid_t dataset_id (0);

  //__________________________________________________________________
  // Open the HDF5 dataset used for this test

  cout << "[1] Opening HDF5 file " << name_file << " ..." << endl;
  try {
    file_id = H5Fopen (name_file.c_str(),
		       H5F_ACC_RDONLY,
		       H5P_DEFAULT);
    cout << "-- HDF5 file = " << name_file    << endl;
    cout << "-- file ID   = " << file_id << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    return 1;
  }
  
  //__________________________________________________________________
  // Open the station group
  
  cout << "[2] Opening station group ..." << endl;
  if (file_id > 0) {
    std::vector<std::string> names;
    // Get the names of the groups attached to the root group of the file
    status = DAL::h5get_names (names,file_id,H5G_GROUP);
    // Open the first station group
    if (status) {
      group_id = H5Gopen1 (file_id,
			   names[0].c_str());
      cout << "-- group ID = " << group_id << endl;
    }
  }
  
  //__________________________________________________________________
  // Open the dipole dataset
  
  cout << "[3] Opening dipole dataset ..." << endl;
  if (group_id > 0) {
    std::vector<std::string> names;
    // Get the names of the groups attached to the root group of the file
    status = DAL::h5get_names (names, group_id, H5G_DATASET);
    if (status) {
      dataset_id = H5Dopen1 (group_id,
			     names[0].c_str());
      cout << "-- dataset ID = " << dataset_id << endl;
    }
  }
  
  //__________________________________________________________________
  // Test reading of the attributes attached to the root group
  
  if (file_id > 0) {
    cout << "[4] Reading attributes attached to root group ..." << endl;
    try {
      std::string name;
      std::string telescope;
      std::string observer;
      std::string project;
      std::string observation_id;
      std::string observation_mode;
      //
      DAL::h5get_name (name,file_id);
      DAL::h5get_attribute (file_id,"TELESCOPE",telescope);
      DAL::h5get_attribute (file_id,"OBSERVER",observer);
      DAL::h5get_attribute (file_id,"PROJECT",project);
      DAL::h5get_attribute (file_id,"OBSERVATION_ID",observation_id);
      DAL::h5get_attribute (file_id,"OBSERVATION_MODE",observation_mode);
      //
      cout << "-- HDF5 object name = " << name      << endl;
      cout << "-- TELESCOPE ...... = " << telescope << endl;
      cout << "-- OBSERVER ....... = " << observer  << endl;
      cout << "-- PROJECT ........ = " << project   << endl;
      cout << "-- OBSERVATION_ID   = " << observation_id   << endl;
      cout << "-- OBSERVATION_MODE = " << observation_mode   << endl;
    }
    catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
  }
  
  //__________________________________________________________________
  // Test reading the attributes attached to the station group

  if (group_id > 0) {
    cout << "[5] Reading attributes attached to the station group ..." << endl;
    try {
      std::string name ("");
      std::string trigger_type ("");
      double trigger_offset (0);
      std::vector<uint> triggered_antennas;
      std::vector<double> station_position_value;
      std::vector<std::string> station_position_unit;
      std::string station_position_frame;
      std::vector<double> beam_direction_value;
      std::vector<std::string> beam_direction_unit;
      std::string beam_direction_frame;
      //
      DAL::h5get_name (name,group_id);
      DAL::h5get_attribute (group_id, "TRIGGER_TYPE", trigger_type);
      DAL::h5get_attribute (group_id, "TRIGGER_OFFSET", trigger_offset);
      DAL::h5get_attribute (group_id, "STATION_POSITION_VALUE", station_position_value);
      DAL::h5get_attribute (group_id, "STATION_POSITION_UNIT", station_position_unit);
      DAL::h5get_attribute (group_id, "STATION_POSITION_FRAME", station_position_frame);
      DAL::h5get_attribute (group_id, "BEAM_DIRECTION_VALUE", beam_direction_value);
      DAL::h5get_attribute (group_id, "BEAM_DIRECTION_UNIT", beam_direction_unit);
      DAL::h5get_attribute (group_id, "BEAM_DIRECTION_FRAME", beam_direction_frame);
      //
      cout << "-- HDF5 object name ..... = " << name                   << endl;
      cout << "-- TRIGGER_TYPE ......... = " << trigger_type           << endl;
      cout << "-- TRIGGER_OFFSET ....... = " << trigger_offset         << endl;
      cout << "-- STATION_POSITION_VALUE = " << station_position_value << endl;
      cout << "-- STATION_POSITION_UNIT  = " << station_position_unit  << endl;
      cout << "-- STATION_POSITION_FRAME = " << station_position_frame << endl;
      cout << "-- BEAM_DIRECTION_VALUE . = " << beam_direction_value   << endl;
      cout << "-- BEAM_DIRECTION_UNIT .. = " << beam_direction_unit    << endl;
      cout << "-- BEAM_DIRECTION_FRAME . = " << beam_direction_frame   << endl;
    }
    catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
  }
  
  //__________________________________________________________________
  // Test reading the attributes attached to the dipole dataset
  
  if (dataset_id > 0) {
    cout << "[6] Reading attributes attached to the dipole dataset ..." << endl;
    try {
      uint station_id (0);
      uint rsp_id (0);
      uint rcu_id (0);
      uint time (0);
      double sample_frequency_value;
      std::string sample_frequency_unit;
      std::vector<double> antenna_position_value;
      std::vector<std::string> antenna_position_unit;
      std::string antenna_position_frame;
      std::vector<double> antenna_orientation_value;
      std::vector<std::string> antenna_orientation_unit;
      std::string antenna_orientation_frame;
      //
      DAL::h5get_attribute (dataset_id,"STATION_ID", station_id);
      DAL::h5get_attribute (dataset_id,"RSP_ID", rsp_id);
      DAL::h5get_attribute (dataset_id,"RCU_ID", rcu_id);
      DAL::h5get_attribute (dataset_id,"TIME", time);
      DAL::h5get_attribute (dataset_id,"SAMPLE_FREQUENCY_VALUE", sample_frequency_value);
      DAL::h5get_attribute (dataset_id,"SAMPLE_FREQUENCY_UNIT", sample_frequency_unit);
      DAL::h5get_attribute (dataset_id,"ANTENNA_POSITION_VALUE", antenna_position_value);
      DAL::h5get_attribute (dataset_id,"ANTENNA_POSITION_UNIT", antenna_position_unit);
      DAL::h5get_attribute (dataset_id,"ANTENNA_POSITION_FRAME", antenna_position_frame);
      DAL::h5get_attribute (dataset_id,"ANTENNA_ORIENTATION_VALUE", antenna_orientation_value);
      DAL::h5get_attribute (dataset_id,"ANTENNA_ORIENTATION_UNIT", antenna_orientation_unit);
      DAL::h5get_attribute (dataset_id,"ANTENNA_ORIENTATION_FRAME", antenna_orientation_frame);
      //
      cout << "-- STATION_ID .............. = " << station_id << endl;
      cout << "-- RSP_ID .................. = " << rsp_id     << endl;
      cout << "-- RCU_ID .................. = " << rcu_id     << endl;
      cout << "-- TIME .................... = " << time       << endl;
      cout << "-- SAMPLE_FREQUENCY_VALUE .. = " << sample_frequency_value    << endl;
      cout << "-- SAMPLE_FREQUENCY_UNIT ... = " << sample_frequency_unit     << endl;
      cout << "-- ANTENNA_POSITION_VALUE .. = " << antenna_position_value    << endl;
      cout << "-- ANTENNA_POSITION_UNIT ... = " << antenna_position_unit     << endl;
      cout << "-- ANTENNA_POSITION_FRAME .. = " << antenna_position_frame    << endl;
      cout << "-- ANTENNA_ORIENTATION_VALUE = " << antenna_orientation_value << endl;
      cout << "-- ANTENNA_ORIENTATION_UNIT  = " << antenna_orientation_unit  << endl;
      cout << "-- ANTENNA_ORIENTATION_FRAME = " << antenna_orientation_frame << endl;
    }
    catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
  }

  //__________________________________________________________________
  // Test writing the attributes attached to the dipole dataset

  if (dataset_id > 0) {
    cout << "[7] Writing attributes attached to the dipole dataset ..." << endl;
    try {
      uint station_id (0);
      uint rsp_id (0);
      uint rcu_id (0);
      std::vector<double> antenna_position_value;
      std::vector<std::string> antenna_position_unit;
      std::string antenna_position_frame;
      //
      DAL::h5get_attribute (dataset_id,"STATION_ID", station_id);
      DAL::h5get_attribute (dataset_id,"RSP_ID", rsp_id);
      DAL::h5get_attribute (dataset_id,"RCU_ID", rcu_id);
      DAL::h5get_attribute (dataset_id,
			    "ANTENNA_POSITION_VALUE",
			    antenna_position_value);
      DAL::h5get_attribute (dataset_id,
			    "ANTENNA_POSITION_UNIT",
			    antenna_position_unit);
      DAL::h5get_attribute (dataset_id,
			    "ANTENNA_POSITION_FRAME",
			    antenna_position_frame);
      //
      cout << "-- Original attribute values:" << endl;
      cout << "--> STATION_ID ........... = " << station_id             << endl;
      cout << "--> RSP_ID ............... = " << rsp_id                 << endl;
      cout << "--> RCU_ID ............... = " << rcu_id                 << endl;
      cout << "--> ANTENNA_POSITION_VALUE = " << antenna_position_value << endl;
      cout << "--> ANTENNA_POSITION_UNIT  = " << antenna_position_unit  << endl;
      cout << "--> ANTENNA_POSITION_FRAME = " << antenna_position_frame << endl;
    }
    catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
  }

  //__________________________________________________________________
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_construction

/*!
  \brief Test constructors for a new TBB_DipoleDataset object

  This function should provide tests for all the available constructors to a
  new DAL::TBB_DipoleDataset object.

  \param name_file -- Name of the HDF5 file, within which the dataset is located

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_construction (std::string const &name_file,
                       std::string const &name_station,
                       std::string const &name_dataset)
{
  cout << "\n[test_construction]\n" << endl;

  int nofFailedTests (0);
  herr_t h5error;
  std::string path_dataset = name_station + "/" + name_dataset;

  //__________________________________________________________________
  // Test for the default constructor taking no argument at all.

  cout << "[1] Testing default constructor ..." << endl;
  try {
    TBB_DipoleDataset dataset;
    //
    dataset.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  //__________________________________________________________________
  // Test for the constructor taking file name and name of the dataset
  // as arguments.

  cout << "[2] Testing argumented constructor ..." << endl;
  try {
    cout << "-- Filename ...... = " << name_file << endl;
    cout << "-- Path to dataset = " << path_dataset << endl;
    //
    TBB_DipoleDataset dataset (name_file,
			       path_dataset);
    //
    dataset.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  //__________________________________________________________________
  // Test for the constructor taking file identifier and name of the
  // dataset as arguments.
  
  cout << "[3] Testing argumented constructor with file as anchor ..." << endl;
  try {
    hid_t file_id = H5Fopen (name_file.c_str(),
			     H5F_ACC_RDONLY,
			     H5P_DEFAULT);
    
    if (file_id > 0) {
      TBB_DipoleDataset dataset (file_id,
				 path_dataset);
      dataset.summary();
      h5error = H5Fclose (file_id);
    }
    else {
      cerr << "-- Error opening HDF5 file " << name_file << endl;
      nofFailedTests++;
    }
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  //__________________________________________________________________
  // Instead of using the base of the file as reference location, also
  // the group within which the dataset is contained can serve as anchor
  // point.

  cout << "[4] Testing argumented constructor with group as anchor ..." << endl;
  try {
    // open file first
    hid_t file_id = H5Fopen (name_file.c_str(),
			     H5F_ACC_RDONLY,
			     H5P_DEFAULT);

    if (file_id > 0) {
      // open group within which the dataset is located
      hid_t group_id = H5Gopen1 (file_id,
				 name_station.c_str());
      
      if (group_id > 0) {
	// create new object
	TBB_DipoleDataset dataset (group_id,
				   name_dataset);
	// provide summary of object's properties
	dataset.summary();
	// release group ID
	h5error = H5Gclose (group_id);
      }
      else {
	cerr << "-- Error opening station group" << endl;
	nofFailedTests++;
      }
      
      // release file ID
      h5error = H5Fclose (file_id);
    }
    else {
      cerr << "-- Error opening HDF5 file " << name_file << endl;
      nofFailedTests++;
    }
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  //__________________________________________________________________
  // Instead of using a ID/name conbination, we should be able to
  // construct a new object directly from a provided identifier for
  // the dataset within the HDF5 file.

  cout << "[5] Testing argumented constructor with dataset identifier ..." << endl;
  try {
    // open file first
    hid_t file_id = H5Fopen (name_file.c_str(),
			     H5F_ACC_RDONLY,
			     H5P_DEFAULT);
    
    if (file_id > 0) {
      // open dataset within which the dataset is located
      hid_t dataset_id = H5Dopen1 (file_id,
				   path_dataset.c_str());
      
      if (dataset_id > 0) {
	// create new object
	TBB_DipoleDataset dataset (dataset_id);
	// provide summary of object's properties
	dataset.summary();
	// release dataset ID
	h5error = H5Dclose (dataset_id);
      }
      else {
	cerr << "-- Error opening dipole dataaset!" << endl;
	nofFailedTests++;
      }
      
      // release file ID
      h5error = H5Fclose (file_id);
    }
    else {
      cerr << "-- Error opening HDF5 file " << name_file << endl;
      nofFailedTests++;
    }
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  //__________________________________________________________________
  // Test copy constructor, as required to directly construct a new
  // object from an existing one.
  
  cout << "[6] Testing copy constructor ..." << endl;
  try {
    cout << "-- creating original object ..." << endl;
    TBB_DipoleDataset dataset (name_file,
			       path_dataset);
    dataset.summary();
    //
    cout << "-- creating new object as copy ..." << endl;
    TBB_DipoleDataset datasetCopy (dataset);
    datasetCopy.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  //__________________________________________________________________
  // Test creation of a number of TBB_DipoleDataset objects to handle
  // the multiple datasets contained within a station group.

  if (name_dataset == "001000001") {
    cout << "[7] Collecting objects for multiple datasets into vector..." << endl;
    try {
      cout << "-- opening HDF5 file ...." << endl;
      hid_t file_id = H5Fopen (name_file.c_str(),
			       H5F_ACC_RDONLY,
			       H5P_DEFAULT);
      cout << "-- opening station group ..." << endl;
      hid_t group_id = H5Gopen1 (file_id,
				 name_station.c_str());
      //
      std::vector<DAL::TBB_DipoleDataset> datasets;
      datasets.push_back(DAL::TBB_DipoleDataset (group_id,
						 "001000001"));
      datasets.push_back(DAL::TBB_DipoleDataset (group_id,
						 "001000002"));
      datasets.push_back(DAL::TBB_DipoleDataset (group_id,
						 "001000003"));
      datasets.push_back(DAL::TBB_DipoleDataset (group_id,
						 "001000004"));
      //
      cout << "-- nof. datasets attached to objects = " << datasets.size() << endl;
      //
      for (uint n(0); n<datasets.size(); n++) {
	datasets[n].summary();
      }
      //
      H5Gclose(group_id);
      H5Fclose(file_id);
    }
    catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
  }
  else {
    cout << "Skipping test." << endl;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                  test_datasets

/*!
  \brief Test handling of multiple TBB_DipoleDataset objects

  One of the essentional issues for mapping the interal structure of a HDF5
  TBB time-series file onto a set up C++ objects is the possibility for
  higher-level structure elements to contain a collection of lower-level ones.
  The prefered way to do this is to use a construct such as
  \code
  std::vector<DAL::TBB_DipoleDataset> datasets;
  \endcode
  so we should be testing well in advance that such a construct behaves as
  expected.

  \param name_file -- Data file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_datasets (std::string const &name_file,
                   std::string const &name_station,
                   std::string const &name_dataset)
{
  cout << "\n[test_datasets]\n" << endl;

  int nofFailedTests (0);
  hid_t file_id (0);
  hid_t dataset_id (0);
  herr_t h5error;
  std::string path_dataset = name_station + "/" + name_dataset;
  std::vector<std::string> dataset_names;
  uint nofDatasets (0);

  dataset_names.push_back (path_dataset);

  nofDatasets = dataset_names.size();

  //__________________________________________________________________
  // Open the HDF5 file for further reading

  cout << "[1] Opening the HDF5 file containing the data ..." << endl;
  file_id = H5Fopen (name_file.c_str(),
                     H5F_ACC_RDONLY,
                     H5P_DEFAULT);

  if (file_id > 0) {
    cout << "--> File name = " << name_file << endl;
    cout << "--> File ID   = " << file_id  << endl;
  }
  else {
    return 0;
  }
  
  //__________________________________________________________________
  // Use the basic methods provided by the HDF5 library to open a number
  // datasets contained within the file. The object identifiers are
  // collected in a STD vector.

  cout << "[2] Opening dipole datasets ..." << endl;
  try {
    // additional local variables
    std::vector<hid_t> dataset_ids;
    uint numDataset (0);
    
    cout << "-- start opening datasets within the file ..." << endl;
    for (uint n(0); n<dataset_names.size(); n++) {
      dataset_id = H5Dopen1 (file_id,
			     dataset_names[n].c_str());
      // check if opening of dataset was successful
      if (dataset_id > 0) {
	dataset_ids.push_back (dataset_id);
	cout << "--> " << dataset_names[n] << " = " << dataset_ids[numDataset] << endl;
	numDataset++;
      }
    }
    
    cout << "-- cleaning up the list of dataset identifiers ..." << endl;
    for (uint n(0); n<dataset_ids.size(); n++) {
      h5error = H5Dclose (dataset_ids[n]);
      cout << "--> " << dataset_ids[n] << " : " << h5error << endl;
    }
    
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  /*
   * TEST: While before we have been collecting the IDs for a set of non-identical
   *       objects in the file, this time we attempt to open up the same object in
   *       HDF5 file multiple time. Though this might not be advisable in terms of
   *       consistency, this at least should be ok in terms of simply applying the
   *       open() operation multiple times.
   */
  
  cout << "[3] Attempt opening dataset multiple times ..." << endl;
  try {
    // additional local variables
    int numDataset (0);
    std::vector<hid_t> dataset_ids;
    
    for (uint n(0); n<dataset_names.size(); n++) {
      // try to open the dataset
      dataset_id = H5Dopen1 (file_id,
			     dataset_names[numDataset].c_str());
      // check if opening of dataset was successful
      if (dataset_id > 0) {
	dataset_ids.push_back (dataset_id);
	//
	cout << "--> " << dataset_names[numDataset] << " = " << dataset_id << endl;
      }
    }
    
    // clean up the list of identifiers
    for (uint n(0); n<dataset_ids.size(); n++) {
      h5error = H5Dclose (dataset_ids[n]);
    }
    
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  /*
   * TEST: Use std::vector as container for keeping a set of multiple
   *       TBB_DipoleDataset objects. This will be required by classes
   *       representing higher-level structures of the HDF5 file, such as e.g.
   *       TBB_StationGroup.
   */
  
  cout << "[5] Creating a collection of dipole dataset objects ..." << endl;
  try {
    std::vector<DAL::TBB_DipoleDataset> datasets;
    
    for (uint n(0); n<nofDatasets; n++) {
      // try to open the dataset
      dataset_id = H5Dopen1 (file_id,
			     dataset_names[n].c_str());
      h5error = H5Eclear1();
      // if HDF5 object exists, create TBB_DipoleDataset object for it
      if (dataset_id > 0) {
	cout << "-- " << dataset_names[n] << endl;
	// create TBB_DipoleDataset object new to the list
	TBB_DipoleDataset dataset (dataset_id);
	datasets.push_back (TBB_DipoleDataset(dataset_id));
	// feedback
	cout << "--> Dateset ID      = " << dataset.dataset_id()    << endl;
	cout << "--> nof. attributes = " << dataset.nofAttributes() << endl;
      }
      // release the identifier for the located dataset object
      h5error = H5Dclose (dataset_id);
      h5error = H5Eclear1();
    }
    cout << "--> Located and assigned " << datasets.size() << " datasets." << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  // release the opened data file
  if (file_id > 0) {
    h5error = H5Fclose (file_id);
    h5error = H5Eclear1();
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_parameters

/*!
  \brief Test the methods used for retrieval of parameters/private data

  \param name_file -- Name of the HDF5 file, within which the dataset is located

  \return nofFailedTests -- The number of failed tests.
*/
int test_parameters (std::string const &name_file,
                     std::string const &name_station,
                     std::string const &name_dataset)
{
  cout << "\n[test_parameters]\n" << endl;

  int nofFailedTests (0);
  herr_t h5error;
  std::string path_dataset = name_station + "/" + name_dataset;

  // create object to use for the subsequent tests
  TBB_DipoleDataset dataset (name_file,
                             path_dataset);

  cout << "[1] Retrieve object parameters ..." << endl;
  try {
    cout << "-- Class name = " << dataset.className()  << endl;
    cout << "-- Dataset ID = " << dataset.dataset_id() << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  cout << "[2] Retrieve atomic valued attributes ..." << endl;
  try {
    uint station_id         = dataset.station_id();
    uint rsp_id             = dataset.rsp_id();
    uint rcu_id             = dataset.rcu_id();
    double sample_frequency = dataset.sample_frequency_value();
    uint nyquist_zone       = dataset.nyquist_zone();
    uint time               = dataset.time();
    uint sample_number      = dataset.sample_number();
    uint samples_per_frame  = dataset.samples_per_frame();
    // extra values derived from attributes
    double julianDay        = dataset.julianDay();
    //
    cout << "-- STATION_ID          = " << station_id        << endl;
    cout << "-- RSP_ID              = " << rsp_id            << endl;
    cout << "-- RCU_ID              = " << rcu_id            << endl;
    cout << "-- SAMPLE_FREQUENCY    = " << sample_frequency  << endl;
    cout << "-- NYQUIST_ZONE        = " << nyquist_zone      << endl;
    cout << "-- TIME [Unix seconds] = " << time              << endl;
    cout << "-- TIME [Julian Day  ] = " << julianDay         << endl;
    cout << "-- SAMPLE_NUMBER       = " << sample_number     << endl;
    cout << "-- SAMPLES_PER_FRAME   = " << samples_per_frame << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  cout << "[3] Retrieve vector valued attributes ..." << endl;
  try {
    casa::Vector<double> antennaPositionValue;
    casa::Vector<casa::String> antennaPositionUnit;
    casa::Vector<double> antennaOrientationValue;
    casa::Vector<casa::String> antennaOrientationUnit;
    //
    dataset.antenna_position_value(antennaPositionValue);
    dataset.antenna_position_unit(antennaPositionUnit);
    dataset.antenna_orientation_value(antennaOrientationValue);
    dataset.antenna_orientation_unit(antennaOrientationUnit);
    //
    cout << "-- ANTENNA_POSITION_VALUE    = " << antennaPositionValue    << endl;
    cout << "-- ANTENNA_POSITION_UNIT     = " << antennaPositionUnit     << endl;
    cout << "-- ANTENNA_ORIENTATION_VALUE = " << antennaOrientationValue << endl;
    cout << "-- ANTENNA_ORIENTATION_UNIT  = " << antennaOrientationUnit  << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test the retrieval of the actual TBB data contained within the dataset

  \param name_file -- Name of the HDF5 file, within which the dataset is located

  \return nofFailedTests -- The number of failed tests.
*/
int test_data (std::string const &name_file,
               std::string const &name_station,
               std::string const &name_dataset)
{
  cout << "\n[test_data]\n" << endl;

  int nofFailedTests (0);
  bool status (true);
  int start (0);
  uint blocksize (10);
  std::string path_dataset = name_station + "/" + name_dataset;

  // open dataset
  TBB_DipoleDataset dataset (name_file,
                             path_dataset);

  cout << "[1] Retrieve data via pointer to array ..." << std::endl;
  try {
    short *dataBuffer;
    
    dataBuffer = new short [blocksize];
    
    status = dataset.fx(start,
			blocksize,
			dataBuffer);
    
    if (status) {
      cout << "-- Channel name = " << dataset.channelName() << std::endl;
      cout << "-- Data start   = " << start     << std::endl;
      cout << "-- Blocksize    = " << blocksize << std::endl;
      cout << "-- Channel data = ["
	   << dataBuffer[0] << ", "
	   << dataBuffer[1] << ", "
	   << dataBuffer[2] << ", "
	   << dataBuffer[3] << ", .. "
	   << dataBuffer[blocksize-1] << " ]"
	   << std::endl;
    }
    else {
      std::cerr << "--> Error retrieving time-series data!" << std::endl;
      nofFailedTests++;
    }
    
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Retrieve data into casa::Vector ..." << std::endl;
  try {
    casa::Vector<double> data = dataset.fx (start,blocksize);
    
    if (data.size() == blocksize) {
      std::cout << "-- Channel name = " << dataset.channelName() << std::endl;
      std::cout << "-- Data start   = " << start                 << std::endl;
      std::cout << "-- Blocksize    = " << blocksize             << std::endl;
      std::cout << "-- Channel data = " << data                  << std::endl;
    }
    else {
      std::cerr << "--> Error retrieving time-series data!" << std::endl;
      nofFailedTests++;
    }
    
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Retrieve multiple blocks of data ..." << std::endl;
  try {
    uint nofBlocks (15);
    casa::Vector<double> data (blocksize);
    
    std::cout << "\tblock\tstart" << std::endl;
    for (uint n(0); n<nofBlocks; n++) {
      data = dataset.fx (start,blocksize);
      // feedback
      std::cout << "\t" << n << "\t" << start << "\t" << data << std::endl;
      // increment pointer to start of data block
      start += blocksize;
    }
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

  \param name_file -- Name of the HDF5 file, within which the dataset is located

  \return nofFailedTests -- The number of failed tests.
*/
int test_export2record (std::string const &name_file,
                        std::string const &name_station,
                        std::string const &name_dataset)
{
  cout << "\n[test_export2record]\n" << endl;

  int nofFailedTests (0);
  std::string path_dataset = name_station + "/" + name_dataset;

  //__________________________________________________________________
  // Test the TBB_DipoleDataset::attributes2record() function

  cout << "[1] Retrieve attributes of dataset into record ..." << endl;
  try {
    // open dataset
    TBB_DipoleDataset dataset (name_file,
			       path_dataset);
    // retrieve attributes into record
    casa::Record rec = dataset.attributes2record ();
    // feedback
    cout << "-- nof. attributes ...... = " << dataset.nofAttributes() << endl;
    cout << "-- nof. record fields ... = " << rec.nfields()           << endl;
    cout << "-- Contents of the record:\n" << rec << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  
  cout << "[2] Combine records from multiple dipole datasets ..." << endl;
  try {
    // open dataset
    TBB_DipoleDataset dataset (name_file,
			       path_dataset);
    // retrieve attributes into record
    casa::Record rec = dataset.attributes2record ();
    // combine records from dipole datasets at station level
    casa::Record recordStation;
    recordStation.define ("STATION", "Station001");
    recordStation.defineRecord ("dipole1",rec);
    recordStation.defineRecord ("dipole2",rec);
    recordStation.defineRecord ("dipole3",rec);
    // combine records from station groups at root level
    casa::Record record;
    record.define ("INSTRUMENT", "LOFAR");
    record.defineRecord ("Station001",recordStation);
    record.defineRecord ("Station002",recordStation);
    record.defineRecord ("Station003",recordStation);
    // display the created record
    std::cout << record << std::endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                          test_accessParameters

int test_accessParameters ()
{
  cout << "\n[test_accessParameters]\n" << endl;

  int nofFailedTests (0);

  unsigned int nelem (20);
  int offset (-5);
  int *data = new int[nelem];

  cout << "-- setting up array holding the data ..." << endl;
  try {
    for (unsigned int n(0); n<nelem; ++n) {
      data[n] = n+offset;
    }
    show (data,nelem);
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "-- create pointers to the various elements of the data ..." << endl;
  try{
    unsigned int n (10);
    int *subset = new int[n];
    subset = &data[5];
    show (subset,n);
    //
    for (unsigned int nn(0); nn<n; ++nn) {
      subset[nn] = nn+100;
    }
    show (subset,n);
    show (data,nelem);
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
  
  //__________________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    cerr << "[tTBB_DipoleDataset] Too few parameters!" << endl;
    cerr << "" << endl;
    cerr << "  tTBB_DipoleDataset <filename> <station group> <dipole dataset>" << endl;
    cerr << "" << endl;
    haveDataset = false;
  }
  
  //__________________________________________________________________
  // Run the tests

  nofFailedTests += test_accessParameters ();

  if (haveDataset) {
    std::string name_file = argv[1];
    std::string name_station;
    std::string name_dataset;
    get_names (name_file,
	       name_station,
	       name_dataset);
    // Test low-level access to the dataset through the HDF5 library directly
    nofFailedTests += test_dataset (name_file);
    // Test for the constructor(s)
    nofFailedTests += test_construction (name_file, name_station, name_dataset);
    // Test working with collection of multiple objects
    nofFailedTests += test_datasets (name_file, name_station, name_dataset);
    // Test retrieval of parameters/attributes attached to the dataset
    nofFailedTests += test_parameters (name_file, name_station, name_dataset);
    // Test retrieval of the actual TBB time-series values
    nofFailedTests += test_data (name_file, name_station, name_dataset);  
#ifdef HAVE_CASA
    // Test exporting the attributes to a casa::Record
    nofFailedTests += test_export2record (name_file, name_station, name_dataset);
#endif
  }
  
  return nofFailedTests;
}
