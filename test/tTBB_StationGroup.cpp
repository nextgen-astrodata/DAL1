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
  \file tTBB_StationGroup.cpp

  \ingroup DAL

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

// ------------------------------------------------------------------------------

/*!
  \brief Test localization and handling of datasets inside the group

  \param name_file -- Data file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_datasets (std::string const &name_file,
                   std::string const &name_station)
{
  cout << "\n[test_datasets]\n" << endl;

  int nofFailedTests (0);
  herr_t h5error (0);
  hid_t fileID (0);
  hid_t groupID (0);

  try
    {
      cout << "-- opening HDF5 file ..." << endl;
      fileID = H5Fopen (name_file.c_str(),
                        H5F_ACC_RDONLY,
                        H5P_DEFAULT);
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }

  if (fileID > 0)
    {

      cout << "--> successfully opened HDF5 file" << endl;

      cout << "-- opening HDF5 group ..." << endl;
      groupID = H5Gopen1 (fileID,
                          name_station.c_str());

      if (groupID > 0)
        {
          hsize_t nofObjects;
          // feedback
          cout << "--> successfully opened HDF5 group" << endl;
          // Number of objects in the group specified by its identifier
          h5error = H5Gget_num_objs(groupID,
                                    &nofObjects);
          cout << "--> " << nofObjects << " objects found in group." << endl;
          // go through the list of objects and extract the datasets
          for (hsize_t n(0); n<nofObjects; n++)
            {
              if (H5G_DATASET == H5Gget_objtype_by_idx (groupID,n))
                {
                  cout << "\tObject " << n << " is a dataset" << endl;
                }
            }
          // close HDF5 group
          h5error = H5Gclose (groupID);
        }
      else
        {
          cerr << "--> Unable to perform test; invalid group ID!" << endl;
        }

      // close file
      h5error = H5Fclose (fileID);

    }
  else
    {
      cerr << "--> Unable to perform test; invalid file ID!" << endl;
    }

  return nofFailedTests;
}

// ------------------------------------------------------------ test_construction

/*!
  \brief Test constructors for a new TBB_StationGroup object

  This function should provide tests for all the available constructors to a
  new DAL::TBB_StationGroup object:
  \code
    TBB_StationGroup ();

    TBB_StationGroup (std::string const &name_file,
			std::string const &group);

    TBB_StationGroup (hid_t const &location,
			std::string const &group);

    TBB_StationGroup (hid_t const &group_id);

    TBB_StationGroup (TBB_StationGroup const &other);
  \endcode

  \param name_file -- Data file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_construction (std::string const &name_file,
                       std::string const &name_station)
{
  cout << "\n[test_construction]\n" << endl;

  int nofFailedTests (0);
  herr_t h5error (0);

  //__________________________________________________________________
  // TEST: Default constructor

  cout << "[1] Testing default constructor ..." << endl;
  try
    {
      TBB_StationGroup group;
      group.summary();
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }
  h5error = H5Eclear1();

  //__________________________________________________________________
  // TEST: Argumented constructor using name_file and name of the group as
  //       input parameters.

  cout << "[2] Construction from file- and group-name ..." << endl;
  try
    {
      TBB_StationGroup group (name_file,
                              name_station);
      group.summary();
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }
  h5error = H5Eclear1();

  //__________________________________________________________________
  // TEST: Argumented constructor using file identifier (as obtained from
  //       previous call to HDF5 library) and group name as input parameters.

  cout << "[3] Construction from filename and group ID ..." << endl;
  try
    {
      // Open HDF5 file to get its object ID
      hid_t file_id = H5Fopen (name_file.c_str(),
                               H5F_ACC_RDONLY,
                               H5P_DEFAULT);

      if (file_id > 0)
        {
          TBB_StationGroup group (file_id,
                                  name_station);
          group.summary();
        }
      else
        {
          cerr << "--> Unable to perform test; invalid file ID!" << endl;
        }

      // release file identifier
      h5error = H5Fclose (file_id);
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }
  h5error = H5Eclear1();

  //__________________________________________________________________
  // TEST: Copy constructor

  cout << "[4] Testing copy constructor ..." << endl;
  try
    {
      cout << "--> creating original object ..." << endl;
      TBB_StationGroup group (name_file,
                              name_station);
      group.summary();
      //
      cout << "--> creating new object as copy ..." << endl;
      TBB_StationGroup groupCopy (group);
      groupCopy.summary();
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }
  h5error = H5Eclear1();

  return nofFailedTests;
}

// ------------------------------------------------------------------ test_groups

/*!
  \brief Test identification and access of groups a root level of the HDF5 file

  \param name_file -- Data file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_groups (std::string const &name_file)
{
  cout << "\n[test_groups]\n" << endl;

  bool status        = true;
  int nofFailedTests = 0;
  hsize_t nofObjects = 0;
  int objectType     = 0;
  std::string name;
  herr_t h5error     = 0;

  // open the HDF5 for further access
  hid_t fileID = H5Fopen (name_file.c_str(),
                          H5F_ACC_RDONLY,
                          H5P_DEFAULT);

  h5error = H5Gget_num_objs(fileID,
                            &nofObjects);

  std::cout << "-- File ID      = " << fileID     << std::endl;
  std::cout << "-- nof. objects = " << nofObjects << std::endl;

  for (hsize_t n(0); n<nofObjects; n++)
    {
      // retrieve the object type
      objectType = H5Gget_objtype_by_idx (fileID,
                                          n);
      // ... and report the finding
      switch (objectType)
        {
        case H5G_DATASET:
          std::cout << "--> Object " << n << " is of type H5G_DATASET." << std::endl;
          break;
        case H5G_GROUP:
          std::cout << "--> Object " << n << " is of type H5G_GROUP." << std::endl;
          // get the name of the group
          status = DAL::h5get_name (name,
                                    fileID,
                                    n);
          if (status)
            {
              std::cout << "--> Group name = " << name << std::endl;
            }
          break;
        case H5G_LINK:
          std::cout << "--> Object " << n << " is of type H5G_LINK." << std::endl;
          break;
        }
    }

  // release the file ID
  h5error = H5Fclose (fileID);

  return nofFailedTests;
}

// ----------------------------------------------------------------- test_methods

/*!
  \brief Test the various methods provided by the class

  The tests included in this function also go through the various methods
  provided to recursively retrieve attributes from the embedded dipole datasets.

  \param name_file    -- Data file used for testing
  \param name_station -- Name of the station group to access

  \return nofFailedTests -- The number of failed tests.
*/
int test_methods (std::string const &name_file,
                  std::string const &name_station)
{
  cout << "\n[test_methods]\n" << endl;

  int nofFailedTests (0);

  // create TBB_StationGroup object to continue working with
  TBB_StationGroup group (name_file,name_station);

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
      std::string station_position_frame;
      casa::Vector<double> beam_direction_value;
      casa::Vector<casa::String> beam_direction_unit;
      std::string beam_direction_frame;
#else
      std::vector<uint> triggered_antennas;
      std::vector<double> station_position_value;
      std::vector<std::string> station_position_unit;
      std::string station_position_frame;
      std::vector<double> beam_direction_value;
      std::vector<std::string> beam_direction_unit;
      std::string beam_direction_frame;
#endif
      // retrieve the values ...
      trigger_type           = group.trigger_type();
      trigger_offset         = group.trigger_offset();
      triggered_antennas     = group.triggered_antennas();
      station_position_value = group.station_position_value();
      station_position_unit  = group.station_position_unit();
      station_position_frame = group.station_position_frame();
      beam_direction_value   = group.beam_direction_value();
      beam_direction_unit    = group.beam_direction_unit();
      beam_direction_frame   = group.beam_direction_frame();
      //... and display them
      cout << "-- TRIGGER_TYPE           = " << trigger_type << endl;
      cout << "-- TRIGGER_OFFSET         = " << trigger_offset << endl;
      cout << "-- TRIGGERED_ANTENNAS     = " << triggered_antennas << endl;
      cout << "-- STATION_POSITION_VALUE = " << station_position_value << endl;
      cout << "-- STATION_POSITION_UNIT  = " << station_position_unit  << endl;
      cout << "-- STATION_POSITION_FRAME = " << station_position_frame << endl;
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
  try
    {
#ifdef HAVE_CASA
      // retrieve the values ...
      casa::Vector<uint> station_id;
      casa::Vector<uint> rsp_id;
      casa::Vector<uint> rcu_id;
      casa::Vector<double> sample_freq_value;
      casa::Vector<casa::String> sample_freq_unit;
      casa::Vector<uint> nyquist_zone;
      casa::Vector<uint> time              = group.time();
      casa::Vector<uint> sample_number     = group.sample_number();
      casa::Vector<uint> samples_per_frame = group.samples_per_frame();
      casa::Vector<int> sample_offset      = group.sample_offset();
      casa::Vector<uint> data_length       = group.data_length();
#else
      // retrieve the values ...
      std::vector<uint> station_id;
      std::vector<uint> rsp_id;
      std::vector<uint> rcu_id;
      std::vector<double> sample_freq_value;
      std::vector<std::string> sample_freq_unit;
      std::vector<uint> nyquist_zone;
      std::vector<uint> time              = group.time();
      std::vector<uint> sample_number     = group.sample_number();
      std::vector<uint> samples_per_frame = group.samples_per_frame();
      std::vector<uint> data_length       = group.data_length();
#endif
      // retrieve the values ...
      station_id        = group.station_id();
      rsp_id            = group.rsp_id();
      rcu_id            = group.rcu_id();
      sample_freq_value = group.sample_frequency_value();
      sample_freq_unit  = group.sample_frequency_unit();
      nyquist_zone      = group.nyquist_zone();
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
  catch (std::string message)
    {
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

  return nofFailedTests;
}

// ---------------------------------------------------------- test_export2record

/*!
  \brief Test export of the attributes to a casa::Record container

  \param name_file    -- Data file used for testing
  \param name_station -- Name of the station group to access

  \return nofFailedTests -- The number of failed tests.
*/
int test_export2record (std::string const &name_file,
                        std::string const &name_station)
{
  std::cout << "\n[test_export2record]\n" << std::endl;

  int nofFailedTests (0);
  TBB_StationGroup group (name_file,name_station);

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

// -----------------------------------------------------------------------------

/*!
  \brief Test retrieval of the actual time-series data form the dipoles

  \param name_file -- Data file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_data (std::string const &name_file,
               std::string const &name_station)
{
  cout << "\n[test_data]\n" << endl;

  int nofFailedTests (0);
  TBB_StationGroup group (name_file,name_station);
  int start (0);
  int blocksize (1024);

  std::cout << "[1] Retrieve data for all dipoles ..." << std::endl;
  try
    {
      casa::Matrix<double> data = group.fx (start,
                                            blocksize);
      // feedback
      std::cout << "-- Data start     = " << start        << std::endl;
      std::cout << "-- Data blocksize = " << blocksize    << std::endl;
      std::cout << "-- Data array     = " << data.shape() << std::endl;
      std::cout << "-- Data [0,]      = " << data.row(0)  << std::endl;
      std::cout << "-- Data [1,]      = " << data.row(1)  << std::endl;
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

int main (int argc,
          char *argv[])
{
  int nofFailedTests (0);

  //__________________________________________________________________
  // Check parameters provided from the command line

  if (argc < 2)
    {
      cerr << "[tTBB_StationGroup] Too few parameters!" << endl;
      cerr << "" << endl;
      cerr << "  tTBB_StationGroup <name_file>" << endl;
      cerr << "" << endl;
      return -1;
    }

  std::string name_file     = argv[1];
  std::string name_station = "Station010";
  std::string name_dataset = "010008064";

  if (argc == 3)
    {
      name_station = argv[2];
    }

  if (argc == 4)
    {
      name_dataset = argv[3];
    }

  cout << "[tTBB_StationGroup]" << endl;
  cout << "-- File name .... = " << name_file    << endl;
  cout << "-- Station group  = " << name_station << endl;
  cout << "-- Dipole dataset = " << name_dataset << endl;

  //__________________________________________________________________
  // Run the tests

  // Test localization and handling of datasets inside the group
  nofFailedTests += test_datasets (name_file,
                                   name_station);

  // Test for the constructor(s)
  nofFailedTests += test_construction (name_file,
                                       name_station);

  if (nofFailedTests == 0)
    {

      nofFailedTests += test_groups(name_file);

      // Test the varios methods implemented in the class
      nofFailedTests += test_methods (name_file,
                                      name_station);

      // Test extraction of channel data
      nofFailedTests += test_data(name_file,
                                  name_station);

      // Test exporting the attributes to a casa::Record
      nofFailedTests += test_export2record (name_file,
                                            name_station);
    }
  else
    {
      std::cerr << "[tTBB_StationGroup] Skipping further tests because testing of"
                << " constructor returned non-zero value!"
                << endl;
    }

  return nofFailedTests;
}
