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
  // Test the argumented constructor taking filename as input

  cout << "[2] Testing argumented constructor ..." << endl;
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

  cout << "[3] Testing copy constructor ..." << endl;
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

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests encountered within this 
          function.
*/
int test_attributes (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_attributes]\n" << endl;

  int nofFailedTests (0);
  TBB_Timeseries ts (filename);

  std::cout << "[1] Attributes attached to the root group ..." << endl;
  try {
    std::string telescope;
    std::string observer;
    std::string observationID;
    //
    ts.getAttribute("TELESCOPE",telescope);
    ts.getAttribute("OBSERVER",observer);
    ts.getAttribute("OBSERVATION_ID",observationID);
    //
    std::cout << "-- TELESCOPE       = " << telescope      << endl;
    std::cout << "-- OBSERVER        = " << observer       << endl;
    std::cout << "-- OBSERVATION_ID  = " << observationID  << endl;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[2] Attributes attached to the station group ..." << endl;
  try {
    casa::Matrix<double>       stationPositionValue;
    casa::Matrix<casa::String> stationPositionUnit;
    casa::Vector<casa::String> stationPositionFrame;
    casa::Matrix<double>       beamDirectionValue;
    casa::Matrix<casa::String> beamDirectionUnit;
    casa::Vector<casa::String> beamDirectionFrame;
    //
    stationPositionValue = ts.station_position_value();
    stationPositionUnit  = ts.station_position_unit();
    stationPositionFrame = ts.station_position_frame();
    beamDirectionValue   = ts.beam_direction_value();
    beamDirectionUnit    = ts.beam_direction_unit();
    beamDirectionFrame   = ts.beam_direction_frame();
    //
    cout << "-- STATION_POSITION_VALUE = " << stationPositionValue.row(0) << endl;
    cout << "-- STATION_POSITION_UNIT  = " << stationPositionUnit.row(0)  << endl;
    cout << "-- STATION_POSITION_FRAME = " << stationPositionFrame(0)     << endl;
    cout << "-- BEAM_DIRECTION_VALUE   = " << beamDirectionValue.row(0)   << endl;
    cout << "-- BEAM_DIRECTION_UNIT    = " << beamDirectionUnit.row(0)    << endl;
    cout << "-- BEAM_DIRECTION_FRAME   = " << beamDirectionFrame(0)       << endl;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

/*!
  \brief Test the various methods 

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests encountered within this 
          function.
*/
int test_methods (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_methods]\n" << endl;

  int nofFailedTests (0);
  TBB_Timeseries ts (filename);

  cout << "[1] Access to embedded StationGroup objects ..." << endl;
  {
    uint nofStations              = ts.nofStationGroups();
    std::set<std::string> names   = ts.stationGroupNames();
    DAL::TBB_StationGroup station = ts.stationGroup(10);
    // Methods of the embedded StationGroup object
    std::string groupName = station.group_name();
    std::string groupType;
    uint nofDipoles;
    // Retrieve attribute values
    station.getAttribute ("GROUPTYPE", groupType);
    station.getAttribute ("NOF_DIPOLES", nofDipoles);
    //
    cout << "-- nof. station groups = " << nofStations << endl;
    cout << "-- Station group names = " << names       << endl;
    cout << "-- Station group name  = " << groupName   << endl;
    cout << "-- GROUPTYPE           = " << groupType   << endl;
    cout << "-- NOF_DIPOLES         = " << nofDipoles  << endl;
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

//_______________________________________________________________________________
//                                                                      test_data

/*!
  \brief Test retrieval of TBB data

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_data (std::string const &filename)
{
  cout << "\n[tTBB_Timeseries::test_data]\n" << endl;

  int nofFailedTests = 0;
  int start          = 0;
  int nofSamples     = 1024;
  TBB_Timeseries ts (filename);
  std::set<std::string> dipoles = ts.selectedDipoles();
  
  cout << "[1] Retrieve time-series data without channel selection" << endl;
  try {
    casa::Matrix<double> data;
    
    ts.fx (data, start, nofSamples);

    cout << "-- Data start     = " << start        << endl;
    cout << "-- Data blocksize = " << nofSamples   << endl;
    cout << "-- Data array     = " << data.shape() << endl;
    cout << "-- Data [0,]      = " << data.row(0)  << endl;
    cout << "-- Data [1,]      = " << data.row(1)  << endl;
    cout << "-- Data [2,]      = " << data.row(2)  << endl;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Retrieve data for selected dipoles ..." << endl;
  try {
    casa::Matrix<double> data;
    uint nofDipoles = dipoles.size();
    std::set<std::string>::iterator it;

    for (uint n(0); n<(nofDipoles-1); ++n) {
      // remove the first element from the selection
      it = dipoles.begin();
      cout << "-- removing dipole " << *it << " from the selection ..." << endl;
      dipoles.erase(it);
      // set the new selection ...
      ts.setSelectedDipoles(dipoles);
      // ... and retrieve the data
      ts.fx (data, start, nofSamples);
      //
      cout << " --> Dipoles     = " << ts.selectedDipoles() << endl;
      cout << " --> shape(data) = " << data.shape()         << endl;
      cout << " --> data [0,]   = " << data.row(0)          << endl;
    }
  }
  catch (std::string message) {
    cerr << message << endl;
    ++nofFailedTests;
  }

  cout << "[3] Retrieve data after selecting all dipoles ..." << endl;
  try {
    casa::Matrix<double> data;
    // Reset the dipole selection and retrive the data
    ts.selectAllDipoles();
    ts.fx (data, start, nofSamples);
    //
    cout << " --> Dipoles     = " << ts.selectedDipoles() << endl;
    cout << " --> shape(data) = " << data.shape()         << endl;
    cout << " --> data [0,]   = " << data.row(0)          << endl;
    cout << " --> data [1,]   = " << data.row(1)          << endl;
    cout << " --> data [2,]   = " << data.row(2)          << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    ++nofFailedTests;
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

  if (haveDataset) {
    // Test constructors for TBB_Timeseries object
    nofFailedTests += test_construction (filename);
    // Test access to the attributes
    nofFailedTests += test_attributes (filename);
    // Test test various methods provided by the class
    nofFailedTests += test_methods (filename);
//     nofFailedTests += test_attributes2record (filename);
    // Test access to the data stored within the dipole datasets
    nofFailedTests += test_data (filename);
  } else {
    std::cout << "\n[tTBB_Timeseries] Skipping tests with input dataset.\n"
	      << endl;
  }
  
  return nofFailedTests;
}
