/*-------------------------------------------------------------------------*
 | $Id:: tHDF5Common.cpp 2022 2008-09-26 12:06:09Z baehren               $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

#include <dalCommon.h>
#include <Enumerations.h>
#include <dalDataset.h>

#ifdef HAVE_CASA
#include <casa/Arrays/Vector.h>
#include <casa/BasicSL/String.h>
#endif

using std::cout;
using std::cerr;
using std::endl;

// -----------------------------------------------------------------------------

/*!
  \file tdalCommon.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the routines contained in dalCommon

  \author Lars B&auml;hren

  \date 2008/10/09

  <h3>Examples</h3>

  Calling the test program without any further command-line parameters will 
  result in a number of tests which do not require any existing data-sets.
*/

//_______________________________________________________________________________
//                                                                test_timeseries

/*!
  \brief Test access to a TBB time-series dataset

  \param filename -- Name of the input HDF5 file on which the access routines are
         tested.

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_timeseries (hid_t const &fileID)
{
  cout << "\n[tdalCommon::test_timeseries]\n" << endl;

  int nofFailedTests = 0;
  bool status        = true;
  hid_t groupID      = -1;
  hid_t datasetID    = -1;

  //__________________________________________________________________
  // Access the attributes attached to the root group of the file

  cout << "[2] Access the attributes attached to the root group of the file"
       << endl;
  try {
    std::string name;
    std::string telescope;
    std::string observer;
    std::string project;
    std::string observation_id;
    std::string observation_mode;
    //
    DAL::h5get_name (name,fileID);
    DAL::h5get_attribute (fileID,"TELESCOPE",telescope);
    DAL::h5get_attribute (fileID,"OBSERVER",observer);
    DAL::h5get_attribute (fileID,"PROJECT",project);
    DAL::h5get_attribute (fileID,"OBSERVATION_ID",observation_id);
    DAL::h5get_attribute (fileID,"OBSERVATION_MODE",observation_mode);
    //
    cout << "-- Filename ....... = " << name             << endl;
    cout << "-- TELESCOPE ...... = " << telescope        << endl;
    cout << "-- OBSERVER ....... = " << observer         << endl;
    cout << "-- PROJECT ........ = " << project          << endl;
    cout << "-- OBSERVATION_ID   = " << observation_id   << endl;
    cout << "-- OBSERVATION_MODE = " << observation_mode << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] repeated read access to attributes ..." << endl;
  try {
    uint nofTrials (500);
    std::string telescope;
    std::string observer;
    std::string project;
    std::string observation_id;
    std::string observation_mode;
    
    for (uint n(0); n<nofTrials; n++) {
      DAL::h5get_attribute (fileID,"TELESCOPE",telescope);
      DAL::h5get_attribute (fileID,"OBSERVER",observer);
      DAL::h5get_attribute (fileID,"PROJECT",project);
      DAL::h5get_attribute (fileID,"OBSERVATION_ID",observation_id);
      DAL::h5get_attribute (fileID,"OBSERVATION_MODE",observation_mode);
    }
    cout << "-- " << nofTrials << " read completed." << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Write acces to attributes in the root group

  std::cout << "[4] write access to attributes in the root group ..." << endl;
  try {
    std::string observer;
    std::string project;
    //
    DAL::h5get_attribute (fileID,"OBSERVER",observer);
    DAL::h5get_attribute (fileID,"PROJECT",project);
    cout << "-- original values:" << endl;
    cout << "--> OBSERVER = " << observer << endl;
    cout << "--> PROJECT  = " << project  << endl;
    //
    observer = "Lars Baehren";
    project  = "CR";
    status = DAL::h5set_attribute (fileID,"OBSERVER",observer);
    status = DAL::h5set_attribute (fileID,"PROJECT",project);
    if (status) {
      DAL::h5get_attribute (fileID,"OBSERVER",observer);
      DAL::h5get_attribute (fileID,"PROJECT",project);
      cout << "-- new values:" << endl;
      cout << "--> OBSERVER = " << observer << endl;
      cout << "--> PROJECT  = " << project  << endl;
    }
    //
    observer = "UNDEFINED";
    project  = "UNDEFINED";
    status = DAL::h5set_attribute (fileID,"OBSERVER",observer);
    status = DAL::h5set_attribute (fileID,"PROJECT",project);
    cout << "-- reverted to original values" << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Open the station group
  
  cout << "[5] opening station group ..." << endl;
  try {
    std::vector<std::string> names;
    // Get the names of the groups attached to the root group of the file
    status = DAL::h5get_names (names,fileID,H5G_GROUP);
    // Open up the first station group
    if (status) {
      cout << "-- Station groups = " << names << endl;
      groupID = H5Gopen1 (fileID,names[0].c_str());
    }
    // If opening of group failed, do not continue
    if (groupID < 0) {
      cerr << "Error opening the station group!" << endl;
      return 1;
    }
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Access the attributes attached to the station group
  
  cout << "[6] Reading attributes attached to station group ..." << endl;
  try {
    std::string name;
    std::string trigger_type;
    double trigger_offset;
    std::vector<uint> triggered_antennas;
    std::vector<double> station_position_value;
    std::vector<std::string> station_position_unit;
    std::string station_position_frame;
    std::vector<double> beam_direction_value;
    std::vector<std::string> beam_direction_unit;
    std::string beam_direction_frame;
    //
    DAL::h5get_name (name,groupID);
    DAL::h5get_attribute (groupID, "TRIGGER_TYPE", trigger_type);
    DAL::h5get_attribute (groupID, "TRIGGER_OFFSET", trigger_offset);
    DAL::h5get_attribute (groupID, "STATION_POSITION_VALUE", station_position_value);
    DAL::h5get_attribute (groupID, "STATION_POSITION_UNIT", station_position_unit);
    DAL::h5get_attribute (groupID, "STATION_POSITION_FRAME", station_position_frame);
    DAL::h5get_attribute (groupID, "BEAM_DIRECTION_VALUE", beam_direction_value);
    DAL::h5get_attribute (groupID, "BEAM_DIRECTION_UNIT", beam_direction_unit);
    DAL::h5get_attribute (groupID, "BEAM_DIRECTION_FRAME", beam_direction_frame);
    //
    cout << "-- Group name             = " << name                   << endl;
    cout << "-- TRIGGER_TYPE           = " << trigger_type           << endl;
    cout << "-- TRIGGER_OFFSET         = " << trigger_offset         << endl;
    cout << "-- STATION_POSITION_VALUE = " << station_position_value << endl;
    cout << "-- STATION_POSITION_UNIT  = " << station_position_unit  << endl;
    cout << "-- STATION_POSITION_FRAME = " << station_position_frame << endl;
    cout << "-- BEAM_DIRECTION_VALUE   = " << beam_direction_value   << endl;
    cout << "-- BEAM_DIRECTION_UNIT    = " << beam_direction_unit    << endl;
    cout << "-- BEAM_DIRECTION_FRAME   = " << beam_direction_frame   << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Open the dipole dataset

  cout << "[7] opening dipole dataset ..." << endl;
  try {
    std::vector<std::string> names;
    // Get the names of the groups attached to the root group of the file
    status = DAL::h5get_names (names, groupID, H5G_DATASET);
    // Open up the first station group
    if (status) {
      cout << "-- Dipole datasets = " << names << endl;
      datasetID = H5Dopen1 (groupID, names[0].c_str());
    }
    // Check if dataset was opened successfully
    if (datasetID < 0) {
      cerr << "Error opening the dipole dataset!" << endl;
      return 1;
    }
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Access the attributes attached to the dipole dataset
  
  cout << "[6] Reading attributes attached to dipole dataset ..." << endl;
  try {
    uint station_id;
    uint rsp_id;
    uint rcu_id;
    uint time;
    double sample_frequency_value;
    std::string sample_frequency_unit;
    std::vector<double> antenna_position_value;
    std::vector<std::string> antenna_position_unit;
    std::string antenna_position_frame;
    std::vector<double> antenna_orientation_value;
    std::vector<std::string> antenna_orientation_unit;
    std::string antenna_orientation_frame;
    //
    DAL::h5get_attribute (datasetID,"STATION_ID", station_id);
    DAL::h5get_attribute (datasetID,"RSP_ID", rsp_id);
    DAL::h5get_attribute (datasetID,"RCU_ID", rcu_id);
    DAL::h5get_attribute (datasetID,"TIME", time);
    DAL::h5get_attribute (datasetID,"SAMPLE_FREQUENCY_VALUE", sample_frequency_value);
    DAL::h5get_attribute (datasetID,"SAMPLE_FREQUENCY_UNIT", sample_frequency_unit);
    DAL::h5get_attribute (datasetID,"SAMPLE_FREQUENCY_VALUE", sample_frequency_value);
    DAL::h5get_attribute (datasetID,"SAMPLE_FREQUENCY_UNIT", sample_frequency_unit);
    DAL::h5get_attribute (datasetID,"ANTENNA_POSITION_VALUE", antenna_position_value);
    DAL::h5get_attribute (datasetID,"ANTENNA_POSITION_UNIT", antenna_position_unit);
    DAL::h5get_attribute (datasetID,"ANTENNA_POSITION_FRAME", antenna_position_frame);
    DAL::h5get_attribute (datasetID,"ANTENNA_ORIENTATION_VALUE", antenna_orientation_value);
    DAL::h5get_attribute (datasetID,"ANTENNA_ORIENTATION_UNIT", antenna_orientation_unit);
    DAL::h5get_attribute (datasetID,"ANTENNA_ORIENTATION_FRAME", antenna_orientation_frame);
    //
    cout << "-- STATION_ID .............. = " << station_id << endl;
    cout << "-- RSP_ID .................. = " << rsp_id     << endl;
    cout << "-- RCU_ID .................. = " << rcu_id     << endl;
    cout << "-- TIME .................... = " << time       << endl;
    cout << "-- SAMPLE_FREQUENCY_VALUE .. = " << sample_frequency_value << endl;
    cout << "-- SAMPLE_FREQUENCY_UNIT ... = " << sample_frequency_unit  << endl;
    cout << "-- ANTENNA_POSITION_VALUE .. = " << antenna_position_value << endl;
    cout << "-- ANTENNA_POSITION_UNIT ... = " << antenna_position_unit  << endl;
    cout << "-- ANTENNA_POSITION_FRAME .. = " << antenna_position_frame << endl;
    cout << "-- ANTENNA_ORIENTATION_VALUE = " << antenna_orientation_value << endl;
    cout << "-- ANTENNA_ORIENTATION_UNIT  = " << antenna_orientation_unit  << endl;
    cout << "-- ANTENNA_ORIENTATION_FRAME = " << antenna_orientation_frame << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Test retrieval of positions and directions as Measures

#ifdef HAVE_CASA
  try {
    casa::MPosition station_position;
    casa::MPosition antenna_position;
    casa::MDirection beam_direction;
    //
    station_position = DAL::h5get_position (groupID,
					    "STATION_POSITION_VALUE",
					    "STATION_POSITION_UNIT",
					    "STATION_POSITION_FRAME");
    beam_direction = DAL::h5get_direction (groupID,
					   "BEAM_DIRECTION_VALUE",
					   "BEAM_DIRECTION_UNIT",
					   "BEAM_DIRECTION_FRAME");
    antenna_position = DAL::h5get_position (datasetID,
					    "ANTENNA_POSITION_VALUE",
					    "ANTENNA_POSITION_UNIT",
					    "ANTENNA_POSITION_FRAME");
    //
    cout << "-- STATION_POSITION = " << station_position << endl;
    cout << "-- ANTENNA_POSITION = " << antenna_position << endl;
    cout << "-- BEAM_DIRECTION   = " << beam_direction   << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
#endif

  /*__________________________________________________________________
    Release HDF5 object identifiers
  */

  if (H5Iis_valid(groupID))   { H5Gclose(groupID);   }
  if (H5Iis_valid(datasetID)) { H5Dclose(datasetID); }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main (int argc,
          char *argv[])
{
  int nofFailedTests = 0;
  std::string filename;
  
  /*__________________________________________________________________
    Check for command-line parameters
  */
  if (argc > 1) {
    filename = argv[1];
  } else {
    cerr << "[tdalCommon_tbb] Missing dataset - skipping tests!" << endl;
    return -1;
  }

  /*__________________________________________________________________
    Open HDF5 file, which is used for the tests
  */

  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  if (H5Iis_valid(fileID)) {
    nofFailedTests += test_timeseries (fileID);
  } else {
    cerr << "[tdalCommon_tbb] Failed to open file " << filename 
	 << " - skipping tests!"
	 << endl;
    return -1;
  }

  /*__________________________________________________________________
    Close HDF5 file and return status
  */

  H5Fclose (fileID);
  
  return nofFailedTests;
}
