/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#include <BF_StokesDataset.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::BF_StokesDataset;

/*!
  \file tBF_StokesDataset.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::BF_StokesDataset class
 
  \author Lars B&auml;hren
 
  \date 2010/12/05

  The generated HDF5 file will have the following structure:
  \verbatim
  tBF_StokesDataset.h5
  |-- StokesI
  |-- StokesQ
  |-- StokesU.001
  |-- StokesU.002
  |-- StokesU.003
  `-- StokesU.004
  \endverbatim
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_StokesDataset object

  \param filename -- Name of the HDF5 file to be employed for testing.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename)
{
  cout << "\n[tBF_StokesDataset::test_constructors]\n" << endl;

  int nofFailedTests (0);
  std::string nameDataset;
  std::vector<hsize_t> shape (2);

  shape[0] = 100;    // Time bins
  shape[1] = 1024;   // Frequency bins
  
  //________________________________________________________
  // Create HDF5 file to work with
  
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  
  /* test of file creation was successful */
  if (H5Iis_valid(fileID)) {
    cout << "-- Successfully opened file " << filename << endl;
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }
  
  //________________________________________________________
  // Test 1: Default constructor

  cout << "[1] Testing BF_StokesDataset() ..." << endl;
  try {
    BF_StokesDataset stokes;
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 2: Argumented constructor

  cout << "[2] Testing BF_StokesDataset(hid_t, string) ..." << endl;
  try {
    nameDataset = "StokesI";
    BF_StokesDataset stokes (fileID, nameDataset);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 3: Argumented constructor

  cout << "[3] Testing BF_StokesDataset(hid_t, string, vector<hsize_t>) ..."
	    << endl;
  try {
    nameDataset = "StokesI";
    BF_StokesDataset stokes (fileID, nameDataset, shape);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 4: Argumented constructor

  cout << "[4] Testing BF_StokesDataset(hid_t, string, vector<hsize_t>, Stokes::Component) ..."
	    << endl;
  try {
    nameDataset = "StokesQ";
    BF_StokesDataset stokes (fileID, nameDataset, shape, DAL::Stokes::Q);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Close HDF5 file used for testing

  H5Fclose(fileID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes

  \param filename -- Name of the HDF5 file to be employed for testing.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_attributes (std::string const &filename)
{
  cout << "\n[tBF_StokesDataset::test_attributes]\n" << endl;

  int nofFailedTests (0);
  std::string nameDataset ("StokesI");

  //________________________________________________________
  // Open HDF5 file to work with
  
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  /* test of file creation was successful */
  if (H5Iis_valid(fileID)) {
    cout << "-- Successfully opened file " << filename << endl;
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }

  // Open dataset
  BF_StokesDataset stokes (fileID, nameDataset);
  int nofChannels;
  int nofSubbands;
  std::string groupType;
  std::string dataType;
  std::string stokesComponent;

  cout << "[1] Testing getAttribute(string, T) ..." << endl;
  try {
    stokes.getAttribute ("GROUPTYPE",        groupType);
    stokes.getAttribute ("DATATYPE",         dataType);
    stokes.getAttribute ("NOF_CHANNELS",     nofChannels);
    stokes.getAttribute ("NOF_SUBBANDS",     nofSubbands);
    stokes.getAttribute ("STOKES_COMPONENT", stokesComponent);
    
    cout << "-- GROUPTYPE        = " << groupType       << endl;
    cout << "-- DATATYPE         = " << dataType        << endl;
    cout << "-- NOF_CHANNELS     = " << nofChannels     << endl;
    cout << "-- NOF_SUBBANDS     = " << nofSubbands     << endl;
    cout << "-- STOKES_COMPONENT = " << stokesComponent << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Testing setAttribute(string, T) ..." << endl;
  try {
    
    /* New values for the attributes */
    groupType       += "_new";
    dataType        += "_new";
    nofChannels     += 1;
    nofSubbands     += 1;
    stokesComponent += "_new";

    /* Set new attribute values */
    stokes.setAttribute ("GROUPTYPE",        groupType);
    stokes.setAttribute ("DATATYPE",         dataType);
    stokes.setAttribute ("NOF_CHANNELS",     nofChannels);
    stokes.setAttribute ("NOF_SUBBANDS",     nofSubbands);
    stokes.setAttribute ("STOKES_COMPONENT", stokesComponent);

    /* Get updated attributes values ... */
    stokes.getAttribute ("GROUPTYPE",        groupType);
    stokes.getAttribute ("DATATYPE",         dataType);
    stokes.getAttribute ("NOF_CHANNELS",     nofChannels);
    stokes.getAttribute ("NOF_SUBBANDS",     nofSubbands);
    stokes.getAttribute ("STOKES_COMPONENT", stokesComponent);

    /* ... and display them */
    cout << "-- GROUPTYPE        = " << groupType       << endl;
    cout << "-- DATATYPE         = " << dataType        << endl;
    cout << "-- NOF_CHANNELS     = " << nofChannels     << endl;
    cout << "-- NOF_SUBBANDS     = " << nofSubbands     << endl;
    cout << "-- STOKES_COMPONENT = " << stokesComponent << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Close HDF5 file used for testing

  H5Fclose(fileID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_data

/*!
  \brief Test read/write access to the data

  \param filename -- Name of the HDF5 file to be employed for testing.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_data (std::string const &filename)
{
  cout << "\n[tBF_StokesDataset::test_data]\n" << endl;

  int nofFailedTests (0);

  //________________________________________________________
  // Open HDF5 file to work with
  
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  /* test of file creation was successful */
  if (H5Iis_valid(fileID)) {
    cout << "-- Successfully opened file " << filename << endl;
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }
  
  //________________________________________________________
  // Create new dataset to work with

  cout << "--> Create new dataset to work with ..." << endl;

  std::string nameDataset;
  std::vector<hsize_t> shape (2);
  std::vector<int> start (2,0);
  std::vector<int> stride;
  std::vector<int> count;
  std::vector<int> block (2,0);
  int nofSteps;
  unsigned int nofDatapoints;

  shape[0] = 100;
  shape[1] = 2048;

  //________________________________________________________
  // Test 1

  cout << "[1] Test writing single rows to dataset ..." << endl;
  try {
    nameDataset   = "StokesU.001";
    nofSteps      = shape[0];
    block[0]      = shape[0]/nofSteps;
    block[1]      = shape[1];
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Dataset name = " << nameDataset << endl;
    cout << "-- Shape        = " << shape    << endl;
    cout << "-- nof. steps   = " << nofSteps << endl;
    cout << "-- block        = " << block    << endl;

    BF_StokesDataset stokes (fileID,
			     nameDataset,
			     shape,
			     DAL::Stokes::U);
    
    for (int step(0); step<nofSteps; ++step) {
      // set position marker
      start[0] = step*block[0];
      // update data array values
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step;
      }
      // write data to dataset
      stokes.writeData (data, start, block);
    }
    
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 2

  cout << "[2] Test writing multiple rows to dataset ..." << endl;
  try {
    nameDataset   = "StokesU.002";
    nofSteps      = 20;
    start[1]      = 0;
    block[0]      = shape[0]/nofSteps;
    block[1]      = shape[1];
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Dataset name = " << nameDataset << endl;
    cout << "-- Shape        = " << shape    << endl;
    cout << "-- nof. steps   = " << nofSteps << endl;
    cout << "-- block        = " << block    << endl;

    BF_StokesDataset stokes (fileID,
			     nameDataset,
			     shape,
			     DAL::Stokes::U);
    
    for (int step(0); step<nofSteps; ++step) {
      // set position marker
      start[0] = step*block[0];
      // update data array values
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step;
      }
      // feedback
      cout << "-> writing datablock " << step << "/" << nofSteps
	   << " starting from " << start
	   << " ..." << endl;
      // write data to dataset
      stokes.writeData (data, start, block);
    }

    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  //________________________________________________________
  // Test 3

  cout << "[3] Test writing single columns to dataset ..." << endl;
  try {
    nameDataset   = "StokesU.003";
    nofSteps      = shape[1];
    start[0]      = 0;
    block[0]      = shape[0];
    block[1]      = shape[1]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Dataset name = " << nameDataset << endl;
    cout << "-- Shape        = " << shape    << endl;
    cout << "-- nof. steps   = " << nofSteps << endl;
    cout << "-- block        = " << block    << endl;

    BF_StokesDataset stokes (fileID,
			     nameDataset,
			     shape,
			     DAL::Stokes::U);
    
    for (int step(0); step<nofSteps; ++step) {
      // set position marker
      start[1] = step*block[1];
      // update data array values
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step;
      }
      // write data to dataset
      stokes.writeData (data, start, block);
    }
    
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  //________________________________________________________
  // Test 4

  cout << "[4] Test writing multiple columns to dataset ..." << endl;
  try {
    nameDataset   = "StokesU.004";
    nofSteps      = 16;
    start[0]      = 0;
    block[0]      = shape[0];
    block[1]      = shape[1]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Dataset name = " << nameDataset << endl;
    cout << "-- Shape        = " << shape    << endl;
    cout << "-- nof. steps   = " << nofSteps << endl;
    cout << "-- block        = " << block    << endl;

    BF_StokesDataset stokes (fileID,
			     nameDataset,
			     shape,
			     DAL::Stokes::U);
    
    for (int step(0); step<nofSteps; ++step) {
      // set position marker
      start[1] = step*block[1];
      // update data array values
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step;
      }
      // feedback
      cout << "-> writing datablock " << step << "/" << nofSteps
	   << " starting from " << start
	   << " ..." << endl;
      // write data to dataset
      stokes.writeData (data, start, block);
    }
    
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  //________________________________________________________
  // Test 5

  cout << "[5] Test extending the number of rows in the dataset ..." << endl;

  //________________________________________________________
  // Close HDF5 file used for testing

  H5Fclose(fileID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);
  std::string filename ("tBF_StokesDataset.h5");
  
  // Test for the constructor(s)
  nofFailedTests += test_constructors (filename);
  // Test access to the attributes
  nofFailedTests += test_attributes (filename);
  // Test read/write access to the data
  nofFailedTests += test_data (filename);

  return nofFailedTests;
}
