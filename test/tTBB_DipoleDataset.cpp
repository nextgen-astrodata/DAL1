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

  \brief A collection of test routines for the TBB_DipoleDataset class
 
  \author Lars B&auml;hren
 
  \date 2008/01/10
*/

// -----------------------------------------------------------------------------

/*!
  \brief Test low-level access to the dataset using HDF5 library calls

  \param name_file    -- Name of the HDF5 file, within which the dataset is
         located.
  \param name_station -- Name of the station group within which the dipole
         dataset is contained.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_dataset (std::string const &name_file,
		  std::string const &name_station,
		  std::string const &name_dataset)
{
  cout << "\n[test_dataset]\n" << endl;
  
  int nofFailedTests (0);
  hid_t file_id (0);
  hid_t group_id (0);
  hid_t dataset_id (0);
  std::string path_dataset = name_station + "/" + name_dataset;

  cout << "-- HDF5 file .... = " << name_file    << endl;
  cout << "-- Station group  = " << name_station << endl;
  cout << "-- Dipole dataset = " << name_dataset << endl;
  cout << "-- Dipole dataset path = " << path_dataset << endl;

  //__________________________________________________________________
  // Open the HDF5 dataset used for this test

  cout << "[1] Opening HDF5 file " << name_file << " ..." << endl;
  try {
    file_id = H5Fopen (name_file.c_str(),
		      H5F_ACC_RDONLY,
		      H5P_DEFAULT);
    cout << "-- file ID = " << file_id << endl;
  } catch (std::string message) {
    cerr << message << endl;
    return 1;
  }

  //__________________________________________________________________
  // Open the station group

  cout << "[2] Opening group " << name_station << " ..." << endl;
  if (file_id > 0) {
    group_id = H5Gopen1 (file_id,
			 name_station.c_str());
    cout << "-- group ID = " << group_id << endl;
  }

  //__________________________________________________________________
  // Open the dipole dataset

  cout << "[3] Opening dataset " << name_dataset << " ..." << endl;
  if (group_id > 0) {
    dataset_id = H5Dopen1 (group_id,
			   name_dataset.c_str());
    cout << "-- dataset ID = " << dataset_id << endl;
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
    } catch (std::string message) {
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
      std::vector<double> beam_direction_value;
      //
      DAL::h5get_name (name,group_id);
      DAL::h5get_attribute (group_id, "TRIGGER_TYPE", trigger_type);
      DAL::h5get_attribute (group_id, "TRIGGER_OFFSET", trigger_offset);
      DAL::h5get_attribute (group_id, "STATION_POSITION_VALUE", station_position_value);
      DAL::h5get_attribute (group_id, "BEAM_DIRECTION_VALUE", beam_direction_value);
      //
      cout << "-- HDF5 object name ..... = " << name                   << endl;
      cout << "-- TRIGGER_TYPE ......... = " << trigger_type           << endl;
      cout << "-- TRIGGER_OFFSET ....... = " << trigger_offset         << endl;
      cout << "-- STATION_POSITION_VALUE = " << station_position_value << endl;
      cout << "-- BEAM_DIRECTION_VALUE . = " << beam_direction_value   << endl;
    } catch (std::string message) {
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
      double sample_frequency_value (0);
      std::string sample_frequency_unit;
      uint time (0);
      //
      DAL::h5get_attribute (dataset_id, "STATION_ID", station_id);
      DAL::h5get_attribute (dataset_id, "RSP_ID", rsp_id);
      DAL::h5get_attribute (dataset_id, "RCU_ID", rcu_id);
      DAL::h5get_attribute (dataset_id, "TIME", time);
      DAL::h5get_attribute (dataset_id, "SAMPLE_FREQUENCY_VALUE", sample_frequency_value);
      DAL::h5get_attribute (dataset_id, "SAMPLE_FREQUENCY_UNIT", sample_frequency_unit);
      //
      cout << "-- STATION_ID ........... = " << station_id << endl;
      cout << "-- RSP_ID ............... = " << rsp_id     << endl;
      cout << "-- RCU_ID ............... = " << rcu_id     << endl;
      cout << "-- TIME ................. = " << time       << endl;
      cout << "-- SAMPLE_FREQUENCY_VALUE = " << sample_frequency_value << endl;
      cout << "-- SAMPLE_FREQUENCY_UNIT  = " << sample_frequency_unit  << endl;
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test constructors for a new TBB_DipoleDataset object

  This function should provide tests for all the available constructors to a 
  new DAL::TBB_DipoleDataset object:
  \code
    TBB_DipoleDataset ();

    TBB_DipoleDataset (std::string const &name_file,
			 std::string const &dataset);

    TBB_DipoleDataset (hid_t const &location,
			 std::string const &dataset);

    TBB_DipoleDataset (hid_t const &dataset_id);

    TBB_DipoleDataset (TBB_DipoleDataset const &other);
  \endcode
  
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
  } catch (std::string message) {
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
  } catch (std::string message) {
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
    } else {
      cerr << "-- Error opening HDF5 file " << name_file << endl;
      nofFailedTests++;
    }
  } catch (std::string message) {
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
      } else {
	cerr << "-- Error opening station group" << endl;
	nofFailedTests++;
      }

      // release file ID
      h5error = H5Fclose (file_id);
    } else {
      cerr << "-- Error opening HDF5 file " << name_file << endl;
      nofFailedTests++;
    }
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();

  /*
   * Instead of using a ID/name conbination, we should be able to construct a
   * new object directly from a provided identifier for the dataset within the HDF5
   * file.
   */
  
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
      } else {
	cerr << "-- Error opening dipole dataaset!" << endl;
	nofFailedTests++;
      }

      // release file ID
      h5error = H5Fclose (file_id);
    } else {
      cerr << "-- Error opening HDF5 file " << name_file << endl;
      nofFailedTests++;
    }
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();

  /*
   * Test copy constructor, as required to directly construct a new object from an
   * existing one.
   */

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
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();
  
  return nofFailedTests;
  }

// -----------------------------------------------------------------------------

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

  /*
   * TEST:
   * Open the HDF5 file for further reading
   */

  cout << "[1] Opening the HDF5 file containing the data ..." << endl;
  file_id = H5Fopen (name_file.c_str(),
		     H5F_ACC_RDONLY,
		     H5P_DEFAULT);

  if (file_id > 0) {
    cout << "--> File name = " << name_file << endl;
    cout << "--> File ID   = " << file_id  << endl;
  } else {
    return 0;
  }

  /* 
   * TEST: Use the basic methods provided by the HDF5 library to open a number
   *       datasets contained within the file. The object identifiers are
   *       collected in a STD vector.
   */

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

  } catch (std::string message) {
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

  } catch (std::string message) {
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
  } catch (std::string message) {
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

// -----------------------------------------------------------------------------

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
  } catch (std::string message) {
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
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  h5error = H5Eclear1();

  cout << "[3] Retrieve vector valued attributes ..." << endl;
  try {
    casa::Vector<double> antenna_position    = dataset.antenna_position_value();
    casa::Vector<double> antenna_orientation = dataset.antenna_orientation_value();
    //
    cout << "-- ANTENNA_POSITION    = " << antenna_position    << endl;
    cout << "-- ANTENNA_ORIENTATION = " << antenna_orientation << endl;
  } catch (std::string message) {
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
  
  std::cout << "[1] Retrieve data via pointer to array ..." << std::endl;
  try {
    short *dataBuffer;

    dataBuffer = new short [blocksize];

    status = dataset.fx(start,
			blocksize,
			dataBuffer);
    
    if (status) {
      std::cout << "-- Channel name = " << dataset.channelName() << std::endl;
      std::cout << "-- Data start   = " << start     << std::endl;
      std::cout << "-- Blocksize    = " << blocksize << std::endl;
      std::cout << "-- Channel data = ["
		<< dataBuffer[0] << ", "
		<< dataBuffer[1] << ", "
		<< dataBuffer[2] << ", "
		<< dataBuffer[3] << ", .. "
		<< dataBuffer[blocksize-1] << " ]"
		<< std::endl;
    } else {
      std::cerr << "--> Error retrieving time-series data!" << std::endl;
      nofFailedTests++;
    }

  } catch (std::string message) {
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
    } else {
      std::cerr << "--> Error retrieving time-series data!" << std::endl;
      nofFailedTests++;
    }

  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[3] Retrieve multiple blocks of data ..." << std::endl;
  try {
    uint nofBlocks (15);
    casa::Vector<double> data (blocksize);

    for (uint n(0); n<nofBlocks; n++) {
      data = dataset.fx (start,blocksize);
      // feedback
      std::cout << "\t" << n << "\t" << start << "\t" << data << std::endl;
      // increment pointer to start of data block
      start += blocksize;
    }
  } catch (std::string message) {
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
  } catch (std::string message) {
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
    // create record containing the other records
    casa::Record record;
    record.define ("STATION", "Station001");
    record.defineRecord ("dipole1",rec);
    record.defineRecord ("dipole2",rec);
    record.defineRecord ("dipole3",rec);
    // Create HDF5 file and write the record to it
    casa::HDF5File file("tDipoleDataset_1.h5", casa::ByteIO::New);
    casa::HDF5Record::writeRecord (file, "DipoleDataset", record);
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main (int argc,
	  char *argv[])
{
  int nofFailedTests (0);

  //__________________________________________________________________
  // Process parameters from the command line

  if (argc < 2) {
    cerr << "[tHDF5Common] Too few parameters!" << endl;
    cerr << "" << endl;
    cerr << "  tHDF5Common <name_file>" << endl;
    cerr << "" << endl;
    return -1;
  }
  
  std::string name_file     = argv[1];
  std::string name_station = "Station001";
  std::string name_dataset = "001000001";

  if (argc == 3) {
    name_station = argv[2];
  }

  if (argc == 4) {
    name_dataset = argv[3];
  }
  
  //__________________________________________________________________
  // Run the tests

  nofFailedTests += test_dataset (name_file,
				  name_station,
				  name_dataset);

  // Test for the constructor(s)
  nofFailedTests += test_construction (name_file,
				       name_station,
				       name_dataset);

  // Test working with collection of multiple objects
  nofFailedTests += test_datasets (name_file,
				   name_station,
				   name_dataset);

  // Test retrieval of parameters/attributes attached to the dataset
  nofFailedTests += test_parameters (name_file,
				     name_station,
				     name_dataset);

  // Test retrieval of the actual TBB time-series values
  nofFailedTests += test_data (name_file,
			       name_station,
			       name_dataset);

#ifdef HAVE_CASA
  // Test exporting the attributes to a casa::Record
  nofFailedTests += test_export2record (name_file,
					name_station,
					name_dataset);
#endif
  
  return nofFailedTests;
}
