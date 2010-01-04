/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 3179 2009-10-14 13:20:10Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <dalCommon.h>
#include <TBB_DipoleDataset.h>

// Namespace usage
using DAL::TBB_DipoleDataset;
using std::endl;

/*!
  \file tTBB_DipoleDataset.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the TBB_DipoleDataset class
 
  \author Lars B&auml;hren
 
  \date 2009/10/28
*/

int test_constructors ();
int test_constructors (std::string const &filename);
int test_attributes (std::string const &filename);

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new TBB_DipoleDataset object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tTBB_DipoleDataset::test_constructors]\n" << endl;

  int nofFailedTests (0);
  uint station (0);
  uint rsp (0);
  uint rcu (0);
  std::string filename ("tTBB_DipoleDataset.h5");
  std::string groupname ("Station001");
  hid_t fileID;
  hid_t groupID;
  herr_t h5error;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open/Create HDF5 file _________________________________

  std::cout << "-- Opening file " << filename << "  ..." << endl;
  
  fileID = H5Fcreate (filename.c_str(),
		      H5F_ACC_TRUNC,
		      H5P_DEFAULT,
		      H5P_DEFAULT);
  
  if (fileID < 0) {
    std::cerr << "ERROR : Failed to open/create file." << endl;
    return -1;
  }
  
  // Open/create group _____________________________________

  if (fileID > 0) {
    names.clear();
    DAL::h5get_names (names,fileID,H5G_GROUP);
    //
    if (names.size() > 0) {
      it        = names.begin();
      groupname = *it;
      groupID   = H5Gopen (fileID,
			   groupname.c_str(),
			   H5P_DEFAULT);
    } else {
      groupID   = H5Gcreate (fileID,
			     groupname.c_str(),
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT);
    }
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }
  
  if (groupID < 0) {
    std::cerr << "ERROR : Failed to open/create group." << endl;
    return -1;
  }
  
  // Perform the tests _____________________________________
  
  std::cout << "[1] Testing default constructor ..." << endl;
  try {
    TBB_DipoleDataset dataset;
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing argumented constructor ..." << endl;
  try {
    TBB_DipoleDataset dataset (fileID,"DipoleDataset");
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Testing argumented constructor ..." << endl;
  try {
    std::string name;
    std::vector<hsize_t> shape (1,1024);
    //
    station = 0;
    rsp     = 0;
    for (rcu=0; rcu<5; ++rcu) {
      name = TBB_DipoleDataset::channelName(station,rsp,rcu);
      TBB_DipoleDataset data (fileID,station,rsp,rcu,shape);
      std::cout << "-- created dataset " << name << endl;
    }
    //
    station = 1;
    for (rsp=0; rsp<5; ++rsp) {
      for (rcu=0; rcu<5; ++rcu) {
	name = TBB_DipoleDataset::channelName(station,rsp,rcu);
	TBB_DipoleDataset data (groupID,station,rsp,rcu,shape);
	std::cout << "-- created dataset " << name << endl;
      }
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[4] Reopening previously created datasets ..." << endl;
  try {
    std::string name;
    //
    station = 0;
    rsp     = 0;
    for (rcu=0; rcu<5; ++rcu) {
      name = TBB_DipoleDataset::channelName(station,rsp,rcu);
      TBB_DipoleDataset data (fileID,station,rsp,rcu);
      std::cout << "-- opened dataset " << name << endl;
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[5] Testing copy constructor ..." << endl;
  try {
    std::string name;
    //
    station = 0;
    rsp     = 0;
    for (rcu=0; rcu<5; ++rcu) {
      // convert IDs to name
      name = TBB_DipoleDataset::channelName(station,rsp,rcu);
      // create original object
      TBB_DipoleDataset data (fileID,station,rsp,rcu);
      cout << "-- Channel name (original) = " << data.channelName() << endl;
      // create copy 
      TBB_DipoleDataset dataCopy (data);
      cout << "-- Channel name (copy)     = " << dataCopy.channelName() << endl;
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  // release HDF5 object identifiers _______________________

  h5error = H5Gclose (groupID);
  h5error = H5Fclose (fileID);
  
  // Test access to the attributes _________________________

  nofFailedTests += test_attributes (filename);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new TBB_DipoleDataset object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename)
{
  std::cout << "\n[tTBB_DipoleDataset::test_constructors]\n" << endl;

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
      std::cout << "-- Groups  = " << names << endl;
      it      = names.begin();
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
    names.clear();
    DAL::h5get_names (names,groupID,H5G_DATASET);
    std::cout << "-- Datasets = " << names << endl;
  } else {
    std::cerr << "Skipping tests - unable to open group." << endl;
    return -1;
  }

  // Open the dipole datasets ______________________________

  if (names.size() > 0) {

    cout << "[1] Testing argumented constructor ..." << endl;
    try {
      for (it=names.begin(); it!=names.end(); ++it) {
	std::cout << "-- opening dataset " << *it << endl;
	TBB_DipoleDataset data (groupID,*it);
	data.summary();
      }
    } catch (std::string message) {
      std::cerr << message << endl;
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
      std::cerr << message << endl;
      nofFailedTests++;
    }
    
  } else {
    std::cerr << "Skipping tests - no datasets found." << endl;
    return -1;
  }
  
  // Release HDF5 object identifiers _______________________
  
  h5error = H5Fclose (fileID);
  h5error = H5Gclose (groupID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

int test_attributes (std::string const &filename)
{
  std::cout << "\n[tTBB_DipoleDataset::test_attributes]\n" << endl;

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
      std::cerr << "Skipping tests - unable to open group." << endl;
      return -1;
    }
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (groupID > 0) {
    names.clear();
    DAL::h5get_names (names,groupID,H5G_DATASET);
  } else {
    std::cerr << "Skipping tests - unable to open group." << endl;
    return -1;
  }

  // Read attributes attached to the dataset _______________

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
    data.getAttribute ("STATION_ID",stationID);
    data.getAttribute ("RSP_ID",rspID);
    data.getAttribute ("RCU_ID",rcuID);
    data.getAttribute ("TIME",time);
    data.getAttribute ("SAMPLE_NUMBER",sampleNumber);
    data.getAttribute ("SAMPLES_PER_FRAME",samplesPerFrame);
    data.getAttribute ("NYQUIST_ZONE",nyquistZone);
    data.getAttribute ("DATA_LENGTH",            dataLength);
    data.getAttribute ("FEED",                   feed);
    data.getAttribute ("ANTENNA_POSITION_VALUE", antennaPositionValue);
    data.getAttribute ("ANTENNA_POSITION_UNIT",  antennaPositionUnit);
    data.getAttribute ("ANTENNA_POSITION_FRAME", antennaPositionFrame);
    data.getAttribute ("ANTENNA_ORIENTATION_VALUE", antennaOrientationValue);
    data.getAttribute ("ANTENNA_ORIENTATION_UNIT",  antennaOrientationUnit);
    data.getAttribute ("ANTENNA_ORIENTATION_FRAME", antennaOrientationFrame);
    //
    cout << "-- STATION_ID                 = " << stationID        << endl;
    cout << "-- RSP_ID                     = " << rspID            << endl;
    cout << "-- RCU_ID                     = " << rcuID            << endl;
    cout << "-- TIME                       = " << time             << endl;
    cout << "-- SAMPLE_NUMBER              = " << sampleNumber     << endl;
    cout << "-- SAMPLES_PER_FRAME          = " << samplesPerFrame  << endl;
    cout << "-- NYQUIST_ZONE               = " << nyquistZone      << endl;
    cout << "-- DATA_LENGTH                = " << dataLength       << endl;
    cout << "-- FEED                       = " << feed             << endl;
    cout << "-- ANTENNA_POSITION_VALUE     = " << antennaPositionValue  << endl;
    cout << "-- ANTENNA_POSITION_UNIT      = " << antennaPositionUnit   << endl;
    cout << "-- ANTENNA_POSITION_FRAME     = " << antennaPositionFrame  << endl;
    cout << "-- ANTENNA_ORIENTATION_VALUE  = " << antennaOrientationValue << endl;
    cout << "-- ANTENNA_ORIENTATION_UNIT   = " << antennaOrientationUnit  << endl;
    cout << "-- ANTENNA_ORIENTATION_FRAME  = " << antennaOrientationFrame << endl;
  } else {
    std::cerr << "Skipping tests - no datasets found." << endl;
    return -1;
  }
  
  // Release HDF5 object identifiers _______________________
  
  h5error = H5Fclose (fileID);
  h5error = H5Gclose (groupID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_data

int test_data (std::string const &filename)
{
  std::cout << "\n[tTBB_DipoleDataset::test_data]\n" << endl;

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
      std::cerr << "Skipping tests - unable to open group." << endl;
      return -1;
    }
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (groupID > 0) {
    names.clear();
    DAL::h5get_names (names,groupID,H5G_DATASET);
  } else {
    std::cerr << "Skipping tests - unable to open group." << endl;
    return -1;
  }

  if (names.size() > 0) {
    it = names.begin();
    TBB_DipoleDataset dataset (groupID,*it);
    //
    int start (0);
    int blocksize (1024);
    short * data = new short [blocksize];
    //
    dataset.fx (start,blocksize,data);
    std::cout << "[" 
	      << data[0] << ","
	      << data[1] << ","
	      << data[2] << ","
	      << data[3] << ","
	      << data[4] << ","
	      << data[5] << ",..]"
	      << std::endl;
    //
    delete [] data;
  } else {
    std::cerr << "Skipping tests - no datasets found." << endl;
    return -1;
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

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  if (haveDataset) {
    // Test for the constructor(s)
    nofFailedTests += test_constructors (filename);
    // Test access to the attributes attached to the dataset
    nofFailedTests += test_attributes (filename);
    // Test access to the data
    nofFailedTests += test_data (filename);
  } else {
    std::cout << "\n[tTBB_DipoleDataset] Skipping tests which input dataset.\n"
	      << endl;
  }

  return nofFailedTests;
}
