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

#include <HDF5Common.h>
#include <Enumerations.h>

#ifdef HAVE_CASA
#include <casa/aips.h>
#include <casa/Arrays/Vector.h>
#endif

using std::cerr;
using std::cout;
using std::endl;

/*!
  \file tHDF5Common.cc

  \ingroup DAL

  \brief A collection of test routines for the HDF5Common class
 
  \author Lars B&auml;hren
 
  \date 2008/01/17
*/

// -----------------------------------------------------------------------------

/*!
  \brief Test the additional methods to go along with the HDF5 functionality

  \return nofFailedTests -- The number of failed tests in this function.
*/
int test_support_methods ()
{
  cout << "\n[test_support_methods]\n" << endl;

  int nofFailedTests (0);
  uint nofElements (10);

  try {
    std::vector<int> vec (nofElements);
    //
    for (uint n(0); n<vec.size(); n++) {
      vec[n] = int(n);
    }
    //
    DAL::show_vector(std::cout,vec);
    cout << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  try {
    std::vector<double> vec (nofElements);
    //
    for (uint n(0); n<vec.size(); n++) {
      vec[n] = 0.5*n;
    }
    //
    DAL::show_vector(std::cout,vec);
    cout << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test the basic steps for the creation of a new HDF5 file

  Even though the creation of standard LOFAR datasets basd on the HDF5 library 
  should be handled by the DAL, some basic understanding of the steps involved
  seems to be a reasonable thing; therefore we are doing some of this on our
  own at this point.

  \verbatim
  /
  |-- TELESCOPE               Attribute
  \endverbatim
  
  \return nofFailedTests -- The number of failed tests within this function 
*/
int test_create_file ()
{
  cout << "\n[test_create_file]\n" << endl;

  int nofFailedTests (0);

  std::string outfile ("testfile.h5");
  std::string attribute_name ("TELESCOPE");
  herr_t h5error;
  hid_t file_id (0);
//   hid_t dataspace_id (0);
//   hid_t attribute_id (0);
  
  try {
    file_id = H5Fcreate ("testfile.h5",
			 H5F_ACC_TRUNC,
			 H5P_DEFAULT,
			 H5P_DEFAULT);
  } catch (std::string message ) {
    cerr << message << endl;
    nofFailedTests++;
  }

  // check if file creation was succesfull
  if (file_id > 0) {
    cout << "-- Successfully created HDF5 file." << endl;
    cout << "--> ID = " << file_id << endl;
    
    std::string attribute_name ("ATTR");
    uint attribute_value (1);
    
    if (DAL::h5set_attribute (file_id,
				attribute_name,
				attribute_value)) {
      cout << "-- Successfully created attribute." << endl;
    } else {
      cerr << "-- There was an error creating the attribute!" << endl;
    }
    
  } else {
    cout << "-- Error creating HDF5 file." << endl;
  }

  if (file_id > 0) {
    h5error = H5Fclose (file_id);
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*
  \brief Test retrieval of an attribute's identifier within the HDF5 file

  \param file_id --
  
  \return nofFailedTests -- The number of failed tests within this function 
*/
int get_attribute_id (hid_t const &file_id)
{
  std::cout << "\n[get_attribute_id]\n" << endl;

  int nofFailedTests (0);
  hid_t attribute_id (0);
  hid_t group_id (0);

  /*
   * Get the ID of the station group
   */
  group_id = H5Gopen (file_id,
		      "Station001");
  
  if (group_id > 0) {

    std::cout << "[1] Retrieving IDs of group attributes..." << endl;

    attribute_id = H5Aopen_name(group_id,
				DAL::attribute_name(DAL::TELESCOPE).c_str());
    std::cout << "-- TELESCOPE = " << attribute_id << endl;

    attribute_id = H5Aopen_name(group_id,
				DAL::attribute_name(DAL::OBSERVER).c_str());
    std::cout << "-- OBSERVER  = " << attribute_id << endl;

    attribute_id = H5Aopen_name(group_id,
				DAL::attribute_name(DAL::PROJECT).c_str());
    std::cout << "-- PROJECT   = " << attribute_id << endl;

    attribute_id = H5Aopen_name(group_id,
				DAL::attribute_name(DAL::OBSERVATION_ID).c_str());
    std::cout << "-- OBS_ID    = " << attribute_id << endl;

    attribute_id = H5Aopen_name(group_id,
				DAL::attribute_name(DAL::OBSERVATION_MODE).c_str());
    std::cout << "-- OBS_MODE  = " << attribute_id << endl;

    attribute_id = H5Aopen_name(group_id,
				DAL::attribute_name(DAL::TRIGGER_TYPE).c_str());
    std::cout << "-- TRIG_TYPE = " << attribute_id << endl;

    attribute_id = H5Aopen_name(group_id,
				DAL::attribute_name(DAL::TRIGGER_OFFSET).c_str());
    std::cout << "-- TRIG_OFST = " << attribute_id << endl;

    attribute_id = H5Aopen_name(group_id,
				DAL::attribute_name(DAL::TRIGGERED_ANTENNAS).c_str());
    std::cout << "-- TRIG_ANTS = " << attribute_id << endl;

    attribute_id = H5Aopen_name(group_id,
				DAL::attribute_name(DAL::BEAM_DIRECTION).c_str());
    std::cout << "-- BEAM_DIR  = " << attribute_id << endl;
  }

  /*
   * Get ID of the first dataset in the first station group
   */
  hid_t dataset_id = H5Dopen (file_id,
			      "Station001/001000000");

  if (dataset_id > 0) {

    std::cout << "[2] Retrieving IDs of dataset attributes..." << endl;

    attribute_id = H5Aopen_name(dataset_id,
				DAL::attribute_name(DAL::STATION_ID).c_str());
    std::cout << "-- STATION_ID        = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,
				DAL::attribute_name(DAL::RSP_ID).c_str());
    std::cout << "-- RSP_ID            = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,
				DAL::attribute_name(DAL::RCU_ID).c_str());
    std::cout << "-- RCU_ID            = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,
				DAL::attribute_name(DAL::SAMPLE_FREQUENCY).c_str());
    std::cout << "-- SAMPLE_FREQ       = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,
				DAL::attribute_name(DAL::TIME).c_str());
    std::cout << "-- TIME              = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,
				DAL::attribute_name(DAL::SAMPLE_NUMBER).c_str());
    std::cout << "-- SAMPLE_NR         = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,"SAMPLES_PER_FRAME");
    std::cout << "-- SAMPLES_PER_FRAME = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,"DATA_LENGTH");
    std::cout << "-- DATA_LENGTH       = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,"NYQUIST_ZONE");
    std::cout << "-- NYQUIST_ZONE      = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,
				DAL::attribute_name(DAL::FEED).c_str());
    std::cout << "-- FEED              = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,"ANT_POSITION");
    std::cout << "-- ANT_POSITION      = " << attribute_id << endl;
    
    attribute_id = H5Aopen_name(dataset_id,"ANT_ORIENTATION");
    std::cout << "-- ANT_ORIENTATION   = " << attribute_id << endl;
    
  }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test extracting the values of attributes attached to a group or dataset

  \param file_id -- Identifier for the HDF5 file

  \return nofFailedTests -- The number of failed tests within this function.
*/
int get_attributes (hid_t const &file_id)
{
  std::cout << "\n[get_attributes]\n" << endl;
  
  int nofFailedTests (0);

  bool status (true);

  /*
   * Get the ID of the station group
   */
  hid_t group_id = H5Gopen (file_id,
			    "Station001");
  hid_t dataset_id = H5Dopen (file_id,
			      "Station001/001002021");
  
  if (group_id > 0) {
    std::cout << "[1] Retrieve attributes of type std::string" << endl;

    std::string telescope ("UNDEFINED");
    std::string observer ("UNDEFINED");
    std::string project ("UNDEFINED");
    std::string observation_id ("UNDEFINED");
    std::string observation_mode ("UNDEFINED");
    std::string trigger_type ("UNDEFINED");
    double trigger_offset (0);
    std::vector<int> triggered_antennas;
    std::vector<double> beam_direction;
    
    try {
      status = DAL::h5get_attribute (telescope,
				       DAL::attribute_name(DAL::TELESCOPE),
				       group_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (observer,
				       DAL::attribute_name(DAL::OBSERVER),
				       group_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (project,
				       DAL::attribute_name(DAL::PROJECT),
				       group_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (observation_id,
				       DAL::attribute_name(DAL::OBSERVATION_ID),
				       group_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (observation_mode,
				       DAL::attribute_name(DAL::OBSERVATION_MODE),
				       group_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (trigger_type,
				       DAL::attribute_name(DAL::TRIGGER_TYPE),
				       group_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (trigger_offset,
				       DAL::attribute_name(DAL::TRIGGER_OFFSET),
				       group_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (triggered_antennas,
				       DAL::attribute_name(DAL::TRIGGERED_ANTENNAS),
				       group_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (beam_direction,
				       DAL::attribute_name(DAL::BEAM_DIRECTION),
				       group_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    std::cout << "-- TELESCOPE          = " << telescope        << endl;
    std::cout << "-- OBSERVER           = " << observer         << endl;
    std::cout << "-- PROJECT            = " << project          << endl;
    std::cout << "-- OBSERVATION_ID     = " << observation_id   << endl;
    std::cout << "-- OBSERVATION_MODE   = " << observation_mode << endl;
    std::cout << "-- TRIGGER_TYPE       = " << trigger_type     << endl;
    std::cout << "-- TRIGGER_OFFSET     = " << trigger_offset   << endl;
    std::cout << "-- TRIGGERED_ANTENNAS = "; DAL::show_vector(std::cout,
							     triggered_antennas);
    std::cout << endl;
    std::cout << "-- BEAM_DIRECTION     = "; DAL::show_vector(std::cout,
							   beam_direction);
    std::cout << endl;

  } else {
    cerr << "[get_attributes] Unable to open station group!" << endl;
    nofFailedTests++;
  }

  if (dataset_id > 0) {
    std::cout << "[2] Retrieving attributes from dipole dataset ..." << endl;

    uint station_id (0);
    uint rsp_id (0);
    uint rcu_id (0);
    double sample_frequency (0);
    uint time (0);
    uint sample_number (0);
    uint samples_per_frame (0);
    uint data_length (0);
    uint nyquist_zone (0);
    std::vector<double> antenna_position;
    std::vector<double> antenna_orientation;

    try {
      status = DAL::h5get_attribute (station_id,"STATION_ID",dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (rsp_id,"RSP_ID",dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (rcu_id,"RCU_ID",dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (sample_frequency,
				     "SAMPLE_FREQ",
				     dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (time,
				     "TIME",
				     dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (sample_number,
				     "SAMPLE_NR",
				     dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (samples_per_frame,
				     "SAMPLES_PER_FRAME",
				     dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (data_length,
				     "DATA_LENGTH",
				     dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (nyquist_zone,
				     "NYQUIST_ZONE",
				     dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (antenna_position,
				     "ANT_POSITION",
				     dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    try {
      status = DAL::h5get_attribute (antenna_orientation,
				     "ANT_ORIENTATION",
				     dataset_id);
    } catch (std::string message) {
      cerr << message << endl;
      nofFailedTests++;
    }
    
    std::cout << "-- STATION_ID        = " << station_id        << endl;
    std::cout << "-- RSP_ID            = " << rsp_id            << endl;
    std::cout << "-- RCU_ID            = " << rcu_id            << endl;
    std::cout << "-- SAMPLE_FREQ       = " << sample_frequency  << endl;
    std::cout << "-- TIME              = " << time              << endl;
    std::cout << "-- SAMPLE_NUMBER     = " << sample_number     << endl;
    std::cout << "-- SAMPLES_PER_FRAME = " << samples_per_frame << endl;
    std::cout << "-- DATA_LENGTH       = " << data_length       << endl;
    std::cout << "-- NYQUIST_ZONE      = " << nyquist_zone      << endl;
    
  } else {
    cerr << "[get_attributes] Unable to open dipole dataset!" << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*
  \brief Test retrieval of an objects name based on its identifier

  \param file_id -- Identifier for the HDF5 file.

  \return nofFailedTests -- The number of failed tests in this function.
*/
int get_name (hid_t const &file_id)
{
  int nofFailedTests (0);
  std::string name ("UNDEFINED");
  bool status;

  cout << "[1] Retrieve name of the HDF5 file ..." << endl;
  try {
    status = DAL::h5get_name (name,file_id);
    if (status) {
      cout << "-- File ID   = " << file_id << endl;
      cout << "-- File name = " << name    << endl;
    }
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Retrieve name of the station group ..." << endl;
  try {
    // open group
    hid_t group_id = H5Gopen (file_id,
			      "Station001");
    // retrieve name of group
    status = DAL::h5get_name (name,group_id);
    // feedback
    if (status) {
      cout << "-- Group ID   = " << group_id << endl;
      cout << "-- Group name = " << name     << endl;
    }
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] Retrieve name of dipole dataset ..." << endl;
  try {
    // open dataset
    hid_t dataset_id = H5Dopen (file_id,
				"Station001/001002021");
    // retrieve name of dataset
    status = DAL::h5get_name (name,dataset_id);
    // feedback
    if (status) {
      cout << "-- Dataset ID   = " << dataset_id << endl;
      cout << "-- Dataset name = " << name       << endl;
    }
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test retrieval of attribute value into casacore objects

  \param file_id -- Identifier for the HDF5 file.

  \return nofFailedTests -- The number of failed tests in this function.
*/
#ifdef HAVE_CASA
int test_casacore (hid_t const &file_id)
{
  cout << "\n[test_casacore]\n" << endl;
  
  int nofFailedTests (0);

  cout << "[1] Retrieve attributes to Vector<T> ..." << endl;
  try {
    casa::Vector<double> beam_direction;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}
#endif

// -----------------------------------------------------------------------------

int main (int argc,
	  char *argv[])
{
  int nofFailedTests (0);

  /*
    Some tests do not require any external data input
  */
//   nofFailedTests += test_support_methods ();
  nofFailedTests += test_create_file ();

  return 0;

  /*
    Check if filename of the dataset is provided on the command line; if not
    exit the program.
  */
  if (argc < 2) {
    cerr << "[tHDF5Common] Too few parameters!" << endl;
    cerr << "" << endl;
    cerr << "  tHDF5Common <filename>" << endl;
    cerr << "" << endl;
    return -1;
  }

  /*
   * Open the HDF5 file; the library function returns a file handler which 
   * is to be used in subsequent interactions with the data.
   */
  herr_t h5error (0);
  hid_t file_id = H5Fopen (argv[1],
			   H5F_ACC_RDONLY,
			   H5P_DEFAULT);
  
  if (file_id > 0) {
    nofFailedTests += get_attribute_id (file_id);
    nofFailedTests += get_attributes (file_id);
    nofFailedTests += get_name (file_id);
#ifdef HAVE_CASA
    nofFailedTests += test_casacore (file_id);
#endif
  } else {
    cerr << "[tHDF5Common] Error opening HDF5 file!" << endl;
    return -1;
  }

  /* Clean release of opened HDF5 file */
  if (file_id > 0) {
    h5error = H5Fclose (file_id);
  }

  return nofFailedTests;
}
