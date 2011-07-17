/***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#include <core/dalCommon.h>
#include <data_hl/TBB_DipoleDataset.h>

// Namespace usage
using DAL::TBB_DipoleDataset;
using std::cerr;
using std::cout;
using std::endl;

/*!
  \file tTBB_DipoleDataset.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::TBB_DipoleDataset class
 
  \author Lars B&auml;hren
 
  \date 2009-10-28
*/

//! Test access to the attributes attached to the dipole dataset
int test_attributes (std::string const &filename);

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \param fileID          -- HDF5 object identifier for the file.
  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &fileID)
{
  cout << "\n[tTBB_DipoleDataset::test_constructors]\n" << endl;

  int nofFailedTests = 0;
  std::string name   = "Station000/000000000";

  cout << "[1] Testing TBB_DipoleDataset() ..." << endl;
  try {
    TBB_DipoleDataset dataset;
    dataset.summary();
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing TBB_DipoleDataset(hid_t,string) ..." << endl;
  try {
    TBB_DipoleDataset dataset (fileID,name);
    dataset.summary();
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new TBB_DipoleDataset object

  \param filename  -- Name of the HDF5 file to  work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename)
{
  cout << "\n[tTBB_DipoleDataset::test_constructors]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  hid_t groupID;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open HDF5 file and embedeed groups ____________________

  fileID = H5Fopen (filename.c_str(),
		    H5F_ACC_RDWR,
		    H5P_DEFAULT);

  if (fileID > 0) {
    names.clear();
    DAL::h5get_names (names,fileID,H5G_GROUP);
    //
    if (names.size() > 0) {
      cout << "-- Groups  = " << names << endl;
      it      = names.begin();
      groupID = H5Gopen (fileID,
			 it->c_str(),
			 H5P_DEFAULT);
    } else {
      cerr << "Skipping tests - unable to open group." << endl;
      return -1;
    }
  } else {
    cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (groupID > 0) {
    names.clear();
    DAL::h5get_names (names,groupID,H5G_DATASET);
    cout << "-- Datasets = " << names << endl;
  } else {
    cerr << "Skipping tests - unable to open group." << endl;
    return -1;
  }

  // Open the dipole datasets ______________________________

  if (names.size() > 0) {

    cout << "[1] Testing argumented constructor ..." << endl;
    try {
      for (it=names.begin(); it!=names.end(); ++it) {
	cout << "-- opening dataset " << *it << endl;
	TBB_DipoleDataset data (groupID,*it);
	data.summary();
      }
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    cout << "[2] Testing copy constructor ..." << endl;
    try {
      it=names.begin();
      //
      cout << "--> Creating original object ..." << endl;
      TBB_DipoleDataset data (groupID,*it);
      cout << "--> Summary of original object:" << endl;
      data.summary();
      //
      cout << "--> Creating copy ..." << endl;
      TBB_DipoleDataset dataCopy (data);
      cout << "--> Summary of object copy:" << endl;
      dataCopy.summary();
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
  } else {
    cerr << "Skipping tests - no datasets found." << endl;
    return -1;
  }
  
  // Release HDF5 object identifiers _______________________
  
  H5Fclose (fileID);
  H5Gclose (groupID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

int test_attributes (std::string const &filename)
{
  cout << "\n[tTBB_DipoleDataset::test_attributes]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  hid_t groupID;
  herr_t h5error;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open HDF5 file and embedeed groups ____________________

  fileID = H5Fopen (filename.c_str(),
		    H5F_ACC_RDWR,
		    H5P_DEFAULT);

  if (fileID > 0) {
    names.clear();
    DAL::h5get_names (names,fileID,H5G_GROUP);
    //
    if (names.size() > 0) {
      it      = names.begin();
      groupID = H5Gopen (fileID,
			 it->c_str(),
			 H5P_DEFAULT);
    } else {
      cerr << "Skipping tests - unable to open group." << endl;
      return -1;
    }
  } else {
    cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (groupID > 0) {
    names.clear();
    DAL::h5get_names (names,groupID,H5G_DATASET);
  } else {
    cerr << "Skipping tests - unable to open group." << endl;
    return -1;
  }

  // Read attributes attached to the dataset _______________

  std::cout << "[1] Retrieving attributes from dipole group ..." << std::endl;
  if (names.size() > 0) {
    it = names.begin();
    TBB_DipoleDataset data (groupID,*it);
    //
    uint                     stationID;
    uint                     rspID;
    uint                     rcuID;
    uint                     time;
    uint                     sampleNumber;
    uint                     samplesPerFrame;
    uint                     nyquistZone;
    uint                     dataLength;
    std::string              feed;
    std::vector<double>      antennaPositionValue;
    std::vector<std::string> antennaPositionUnit;
    std::string              antennaPositionFrame;
    std::vector<double>      antennaOrientationValue;
    std::vector<std::string> antennaOrientationUnit;
    std::string              antennaOrientationFrame;
    //
    data.getAttribute ("STATION_ID",                stationID);
    data.getAttribute ("RSP_ID",                    rspID);
    data.getAttribute ("RCU_ID",                    rcuID);
    data.getAttribute ("TIME",                      time);
    data.getAttribute ("SAMPLE_NUMBER",             sampleNumber);
    data.getAttribute ("SAMPLES_PER_FRAME",         samplesPerFrame);
    data.getAttribute ("NYQUIST_ZONE",              nyquistZone);
    data.getAttribute ("DATA_LENGTH",               dataLength);
    data.getAttribute ("FEED",                      feed);
    data.getAttribute ("ANTENNA_POSITION_VALUE",    antennaPositionValue);
    data.getAttribute ("ANTENNA_POSITION_UNIT",     antennaPositionUnit);
    data.getAttribute ("ANTENNA_POSITION_FRAME",    antennaPositionFrame);
    data.getAttribute ("ANTENNA_ORIENTATION_VALUE", antennaOrientationValue);
    data.getAttribute ("ANTENNA_ORIENTATION_UNIT",  antennaOrientationUnit);
    data.getAttribute ("ANTENNA_ORIENTATION_FRAME", antennaOrientationFrame);
    //
    cout << "-- STATION_ID                 = " << stationID               << endl;
    cout << "-- RSP_ID                     = " << rspID                   << endl;
    cout << "-- RCU_ID                     = " << rcuID                   << endl;
    cout << "-- TIME                       = " << time                    << endl;
    cout << "-- SAMPLE_NUMBER              = " << sampleNumber            << endl;
    cout << "-- SAMPLES_PER_FRAME          = " << samplesPerFrame         << endl;
    cout << "-- NYQUIST_ZONE               = " << nyquistZone             << endl;
    cout << "-- DATA_LENGTH                = " << dataLength              << endl;
    cout << "-- FEED                       = " << feed                    << endl;
    cout << "-- ANTENNA_POSITION_VALUE     = " << antennaPositionValue    << endl;
    cout << "-- ANTENNA_POSITION_UNIT      = " << antennaPositionUnit     << endl;
    cout << "-- ANTENNA_POSITION_FRAME     = " << antennaPositionFrame    << endl;
    cout << "-- ANTENNA_ORIENTATION_VALUE  = " << antennaOrientationValue << endl;
    cout << "-- ANTENNA_ORIENTATION_UNIT   = " << antennaOrientationUnit  << endl;
    cout << "-- ANTENNA_ORIENTATION_FRAME  = " << antennaOrientationFrame << endl;
  } else {
    cerr << "Skipping tests - no datasets found." << endl;
    return -1;
  }

  // Read attributes into casa::Record _____________________

#ifdef DAL_WITH_CASA

  std::cout << "[2] Read attributes into casa::Record ..." << std::endl;
  if (names.size() > 0) {
    it = names.begin();
    TBB_DipoleDataset data (groupID,*it);
    //
    casa::Record rec;
    data.getAttributes (rec);
    //
    cout << rec << endl;
  } else {
    cerr << "Skipping tests - no datasets found." << endl;
    return -1;
  }

#endif

  // Release HDF5 object identifiers _______________________
  
  h5error = H5Fclose (fileID);
  h5error = H5Gclose (groupID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_data

/*!
  \brief Test accessing the underlying dataset

  \param filename  -- Name of the HDF5 file to  work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_data (std::string const &filename)
{
  cout << "\n[tTBB_DipoleDataset::test_data]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  hid_t groupID;
  herr_t h5error;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open HDF5 file and embedeed groups ____________________

  fileID = H5Fopen (filename.c_str(),
		    H5F_ACC_RDWR,
		    H5P_DEFAULT);

  if (fileID > 0) {
    names.clear();
    DAL::h5get_names (names,fileID,H5G_GROUP);
    //
    if (names.size() > 0) {
      it      = names.begin();
      groupID = H5Gopen (fileID,
			 it->c_str(),
			 H5P_DEFAULT);
    } else {
      cerr << "Skipping tests - unable to open group." << endl;
      return -1;
    }
  } else {
    cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (groupID > 0) {
    names.clear();
    DAL::h5get_names (names,groupID,H5G_DATASET);
  } else {
    cerr << "Skipping tests - unable to open group." << endl;
    return -1;
  }

  if (names.empty()) {
    cerr << "-- Skipping tests - unable to find dataset in file" << endl;
    return -1;
  }

  //________________________________________________________
  // Run the tests

  cout << "[1] Read a sequence of blocks from the  dataset ..." << endl;
  try{
    it = names.begin();
    TBB_DipoleDataset dataset (groupID,*it);
    //
    int blocksize = 512;
    short * data  = new short [blocksize];
    int start     = 0;
    unsigned int nofSteps (10);
    
    for (unsigned int n(0); n<nofSteps; ++n, start+=blocksize/2) {
      cout << "-- Start = " << start << endl;
      // Read in the data ...
      dataset.readData (start,blocksize,data);
      // ... and display them
      show (data,blocksize);
      cout << endl;
    }
    
    delete [] data;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  // Release HDF5 object identifiers _______________________
  
  h5error = H5Fclose (fileID);
  h5error = H5Gclose (groupID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  hid_t fileID         = 0;
  bool haveDataset     = false;
  std::string filename = "tTBB_DipoleDataset.h5";
  
  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Open/Create HDF5 file to work with

  if (haveDataset) {
    fileID = H5Fopen (filename.c_str(),
                      H5F_ACC_RDWR,
                      H5P_DEFAULT);
  } else {
    fileID = H5Fcreate (filename.c_str(),
                        H5F_ACC_TRUNC,
                        H5P_DEFAULT,
                        H5P_DEFAULT);
  }

  //________________________________________________________
  // Run the tests

  if (H5Iis_valid(fileID)) {
    
    // Test for the constructor(s)
    nofFailedTests += test_constructors (fileID);
    
    if (haveDataset) {
      // Test for the constructor(s)
      nofFailedTests += test_constructors (filename);
      // Test access to the attributes attached to the dataset
      nofFailedTests += test_attributes (filename);
      // Test access to the data
      nofFailedTests += test_data (filename);
    } else {
      cout << "\n[tTBB_DipoleDataset] Skipping tests which input dataset.\n"
	   << endl;
    }
    
  } else {
    cerr << "[tTBB_DipoleDataset] Failed to open file " << filename << endl;
    return -1;
  }
  
  //________________________________________________________
  // close HDF5 file
  
  H5Fclose(fileID);

  return nofFailedTests;
}
