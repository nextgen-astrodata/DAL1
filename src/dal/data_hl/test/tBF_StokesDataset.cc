/***************************************************************************
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

#include <data_hl/BF_StokesDataset.h>

// Namespace usage
using std::cerr;
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
  |-- test_constructors                 Group
  |-- test_attributes                   Group
  \endverbatim
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_StokesDataset object

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &fileID)
{
  cout << "\n[tBF_StokesDataset::test_constructors]" << endl;

  int nofFailedTests       = 0;
  unsigned int rank        = 2;     /* Rank of the Stokes dataset           */
  unsigned int index       = 0;     /* Index number of the Stokes dataset   */
  unsigned int nofSamples  = 1000;  /* nof. samples along the time axis     */
  unsigned int nofSubbands = 36;    /* nof. frequency sub-bands             */
  unsigned int nofChannels = 128;   /* nof. frequency channels per sub-band */
  std::vector<hsize_t> shape (rank);
  std::string nameDataset;

  shape[0] = nofSamples;
  shape[1] = nofSubbands*nofChannels;
  
  /*__________________________________________________________________
    Create HDF5 group within which the datasets will be created
  */

  hid_t groupID = H5Gcreate (fileID,
			     "test_constructors",
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT);
  
  /*_______________________________________________________________________
    Test 1: Default constructor (no dataset created)
  */
  
  cout << "\n[1] Testing BF_StokesDataset() ..." << endl;
  try {
    BF_StokesDataset stokes;
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*_______________________________________________________________________
    Test 2: Argumented constructor to open existing dataset.
  */
  
  cout << "\n[2] Testing BF_StokesDataset(hid_t, string) ..." << endl;
  try {
    index       = 0;
    nameDataset = BF_StokesDataset::getName(index);
    BF_StokesDataset stokes (groupID, nameDataset);
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*_______________________________________________________________________
    Test 3: Argumented constructor to open existing dataset.
  */
  
  cout << "\n[3] Testing BF_StokesDataset(hid_t, uint) ..." << endl;
  try {
    index       = 0;
    BF_StokesDataset stokes (groupID, index);
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*_______________________________________________________________________
    Test 4: Argumented constructor, containing minimal required parameters
            for creation of a new dataset.
  */

  cout << "\n[4] Testing BF_StokesDataset(hid_t, string, vector<hsize_t>) ..."
	    << endl;
  try {
    index = 4;
    BF_StokesDataset stokes (groupID, index, shape);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Test 5: Argumented constructor.
  */

  cout << "\n[5] Testing BF_StokesDataset(hid_t, string, vector<hsize_t>, Stokes::Component) ..."
	    << endl;
  try {
    index = 5;
    BF_StokesDataset stokes (groupID,
			     index,
			     shape,
			     DAL::Stokes::Q);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*_______________________________________________________________________
    Test 6: Argumented constructor providing individual values for the
            number of bins along the time axis, the number of frequency
	    bands and the number of frequency channels per frequency band.
  */

  cout << "\n[6] Testing BF_StokesDataset(hid_t,string,uint,uint,uint,Stokes::Component) ..." << endl;
  try {
    index = 6;
    BF_StokesDataset stokes (groupID,
			     index,
			     nofSamples,
			     nofSubbands,
			     nofChannels,
			     DAL::Stokes::Q);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Test 7: Argumented constructor providing individual values for the
            number of bins along the time axis, the number of frequency
	    bands and the number of frequency channels per frequency band.
  */

  cout << "\n[7] Testing BF_StokesDataset(hid_t,string,uint,vector<uint>,Stokes::Component) ..." << endl;
  try {
    std::vector<unsigned int> channels (nofSubbands);
    index       = 7;
    nameDataset = BF_StokesDataset::getName(index);

    /* Assign number of channels per sub-band */
    for (unsigned int n=0; n<nofSubbands; ++n) {
      channels[n] = 10+n;
    }

    /* Create object */
    BF_StokesDataset stokes (groupID,
			     index,
			     nofSamples,
			     channels,
			     DAL::Stokes::Q);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Test 8: Copy constructor
  */

  cout << "[8] Testing BF_StokesDataset(BF_StokesDataset) ..." << endl;
  try {
    index = 8;
    // Create original object ...
    BF_StokesDataset stokesOrig (groupID, index, shape);
    stokesOrig.summary();
    // ... and create a new one as copy
    BF_StokesDataset stokesCopy (stokesOrig);
    stokesCopy.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /* Release handler for HDF5 group */

  H5Gclose (groupID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_attributes (hid_t const &fileID)
{
  cout << "\n[tBF_StokesDataset::test_attributes]\n" << endl;

  int nofFailedTests       = 0;
  unsigned int index       = 0;     /* Index number of the Stokes dataset   */
  unsigned int nofSamples  = 1000;  /* nof. samples along the time axis     */
  unsigned int nofSubbands = 36;    /* nof. frequency sub-bands             */
  unsigned int nofChannels = 128;   /* nof. frequency channels per sub-band */

  /*__________________________________________________________________
    Create HDF5 group within which the datasets will be created and
    the dataset itself.
  */

  hid_t groupID = H5Gcreate (fileID,
			     "test_attributes",
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT);

  BF_StokesDataset stokes (groupID,
			   index,
			   nofSamples,
			   nofSubbands,
			   nofChannels,
			   DAL::Stokes::I);
  
  /*__________________________________________________________________
    Test 1: Read in the attribute values using the single value
            interface; for array-array type attributes this will return
	    the first array element only.
  */
  
  cout << "[1] Testing getAttribute(string, T) ..." << endl;
  try {
    int nofSubbands;
    int nofChannels;
    std::string groupType;
    std::string dataType;
    std::string stokesComponent;
    
    stokes.readAttribute ("GROUPTYPE",        groupType);
    stokes.readAttribute ("DATATYPE",         dataType);
    stokes.readAttribute ("NOF_CHANNELS",     nofChannels);
    stokes.readAttribute ("NOF_SUBBANDS",     nofSubbands);
    stokes.readAttribute ("STOKES_COMPONENT", stokesComponent);
    
    cout << "-- GROUPTYPE        = " << groupType       << endl;
    cout << "-- DATATYPE         = " << dataType        << endl;
    cout << "-- NOF_CHANNELS     = " << nofChannels     << endl;
    cout << "-- NOF_SUBBANDS     = " << nofSubbands     << endl;
    cout << "-- STOKES_COMPONENT = " << stokesComponent << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 2: Read in the attributes attached to the Stokes datasets,
            using the std::vector<T> interface.
  */
  
  cout << "[2] Testing getAttribute(string, vector<T>) ..." << endl;
  try {
    std::vector<int> nofSubbands;
    std::vector<int> nofChannels;
    std::vector<std::string> groupType;
    std::vector<std::string> dataType;
    std::vector<std::string> stokesComponent;
    
    stokes.readAttribute ("GROUPTYPE",        groupType);
    stokes.readAttribute ("DATATYPE",         dataType);
    stokes.readAttribute ("NOF_CHANNELS",     nofChannels);
    stokes.readAttribute ("NOF_SUBBANDS",     nofSubbands);
    stokes.readAttribute ("STOKES_COMPONENT", stokesComponent);
    
    cout << "-- GROUPTYPE        = " << groupType       << endl;
    cout << "-- DATATYPE         = " << dataType        << endl;
    cout << "-- NOF_CHANNELS     = " << nofChannels     << endl;
    cout << "-- NOF_SUBBANDS     = " << nofSubbands     << endl;
    cout << "-- STOKES_COMPONENT = " << stokesComponent << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  /* Release HDF5 group handler */ 
  H5Gclose (groupID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_parameters

/*!
  \brief Test access to the internal parameters

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_parameters (hid_t const &fileID)
{
  cout << "\n[tBF_StokesDataset::test_parameters]\n" << endl;

  int nofFailedTests = 0;
  unsigned int index = 0;
  hid_t groupID      = H5Gopen (fileID,
				"test_attributes",
				H5P_DEFAULT);
  /* Open Stokes dataset to work with  */
  BF_StokesDataset stokes (groupID,
			   index);

  cout << "-- name          = " << stokes.name()          << endl;
  cout << "-- shape         = " << stokes.shape()         << endl;
  cout << "-- chunking      = " << stokes.chunking()      << endl;
  cout << "-- rank          = " << stokes.rank()          << endl;
  cout << "-- nofAxes       = " << stokes.nofAxes()       << endl;
  cout << "-- nofDatapoints = " << stokes.nofDatapoints() << endl;
  cout << "-- objectID      = " << stokes.objectID()      << endl;
  cout << "-- dataspaceID   = " << stokes.dataspaceID()   << endl;
  cout << "-- datatypeID    = " << stokes.datatypeID()    << endl;
  cout << "-- offset        = " << stokes.offset()        << endl;

  /* Release HDF5 group handler */ 
  H5Gclose (groupID);

  return nofFailedTests;
}


//_______________________________________________________________________________
//                                                                      test_data

/*!
  \brief Test read/write access to the data

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_data (hid_t const &fileID)
{
  cout << "\n[tBF_StokesDataset::test_data]\n" << endl;

  int nofFailedTests         = 0;
  unsigned int nofSamples    = 1000;  /* nof. samples along the time axis     */
  unsigned int nofSubbands   = 32;    /* nof. frequency sub-bands             */
  unsigned int nofChannels   = 128;   /* nof. frequency channels per sub-band */
  unsigned int nofDatapoints = 0;
  int nofSteps               = 0;
  std::vector<int> start (2,0);
  std::vector<int> stride;
  std::vector<int> count;
  std::vector<int> block (2,0);
  std::vector<hsize_t> shape;

  /*__________________________________________________________________
    Create HDF5 group within which the datasets will be created and
    the dataset itself.
  */

  hid_t groupID = H5Gcreate (fileID,
			     "test_data",
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT);


  //________________________________________________________
  // Test 1

  cout << "[1] Test writing single rows to dataset ..." << endl;
  try {
    BF_StokesDataset stokes (groupID,
			     1,
			     nofSamples,
			     nofSubbands,
			     nofChannels,
			     DAL::Stokes::I);
    
    shape         = stokes.shape();
    nofSteps      = shape[0];
    start[0]      = 0;
    start[1]      = 0;
    block[0]      = shape[0]/nofSteps;
    block[1]      = shape[1];
    count.clear();
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Shape           = " << shape    << endl;
    cout << "-- start           = " << start    << endl;
    cout << "-- stride          = " << stride   << endl;
    cout << "-- count           = " << count    << endl;
    cout << "-- block           = " << block    << endl;
    cout << "-- nof. steps      = " << nofSteps << endl;
    cout << "-- nof. datapoints = " << nofDatapoints << endl;
    
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
    BF_StokesDataset stokes (groupID,
			     2,
			     nofSamples,
			     nofSubbands,
			     nofChannels,
			     DAL::Stokes::I);
    
    shape         = stokes.shape();
    nofSteps      = shape[0]/10;
    start[0]      = 0;
    start[1]      = 0;
    block[0]      = shape[0]/nofSteps;
    block[1]      = shape[1];
    count.clear();
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Shape           = " << shape    << endl;
    cout << "-- start           = " << start    << endl;
    cout << "-- stride          = " << stride   << endl;
    cout << "-- count           = " << count    << endl;
    cout << "-- block           = " << block    << endl;
    cout << "-- nof. steps      = " << nofSteps << endl;
    cout << "-- nof. datapoints = " << nofDatapoints << endl;
    
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
  // Test 3

  cout << "[3] Test writing single columns to dataset ..." << endl;
  try {
    BF_StokesDataset stokes (groupID,
			     3,
			     nofSamples,
			     nofSubbands,
			     nofChannels,
			     DAL::Stokes::I);
    
    shape         = stokes.shape();
    nofSteps      = shape[1];
    start[0]      = 0;
    start[1]      = 0;
    block[0]      = shape[0];
    block[1]      = shape[1]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Shape           = " << shape    << endl;
    cout << "-- start           = " << start    << endl;
    cout << "-- stride          = " << stride   << endl;
    cout << "-- count           = " << count    << endl;
    cout << "-- block           = " << block    << endl;
    cout << "-- nof. steps      = " << nofSteps << endl;
    cout << "-- nof. datapoints = " << nofDatapoints << endl;
    
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
    BF_StokesDataset stokes (groupID,
			     4,
			     nofSamples,
			     nofSubbands,
			     nofChannels,
			     DAL::Stokes::I);
    
    shape         = stokes.shape();
    nofSteps      = shape[1]/10;
    start[0]      = 0;
    start[1]      = 0;
    block[0]      = shape[0];
    block[1]      = shape[1]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];

    cout << "-- Shape           = " << shape    << endl;
    cout << "-- start           = " << start    << endl;
    cout << "-- stride          = " << stride   << endl;
    cout << "-- count           = " << count    << endl;
    cout << "-- block           = " << block    << endl;
    cout << "-- nof. steps      = " << nofSteps << endl;
    cout << "-- nof. datapoints = " << nofDatapoints << endl;
    
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
  // Test 5

  cout << "[5] Testing writing 2D patches ..." << endl;
  try {
    nofSamples = 1024;

    BF_StokesDataset stokes (groupID,
			     5,
			     nofSamples,
			     nofSubbands,
			     nofChannels,
			     DAL::Stokes::I);
    
    shape         = stokes.shape();
    nofSteps      = 128;
    start[0]      = 0;
    start[1]      = 0;
    block[0]      = shape[0]/nofSteps;
    block[1]      = shape[1]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    float *data   = new float [nofDatapoints];
    int step      = 0;

    cout << "-- Shape           = " << shape    << endl;
    cout << "-- start           = " << start    << endl;
    cout << "-- stride          = " << stride   << endl;
    cout << "-- count           = " << count    << endl;
    cout << "-- block           = " << block    << endl;
    cout << "-- nof. steps      = " << nofSteps << endl;
    cout << "-- nof. datapoints = " << nofDatapoints << endl;
    
    // Increment counter
    for (int stepx=0; stepx<nofSteps; ++stepx) {
      // Adjust starting point
      start[0] = stepx*block[0];
      // Increment counter
      for (int stepy=0; stepy<nofSteps; ++stepy) {
	// Adjust starting point
	start[1] = stepy*block[1];
	// Update data array values
	for (unsigned int n(0); n<nofDatapoints; ++n) {
	  data[n] = step;
	}
	// write data to dataset
	stokes.writeData (data, start, block);
	// Increment step counter
	++ step;
      }
    }

    /* Release allocated memory */
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /* Release HDF5 group handler */ 
  H5Gclose (groupID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main ()
{
  int nofFailedTests   = 0;
  bool testData        = true;
  std::string filename = "tBF_StokesDataset.h5";

  //________________________________________________________
  // Create HDF5 file to work with
  
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  
  /* If file creation was successful, run the tests. */
  if (H5Iis_valid(fileID)) {

    // Test for the constructor(s)
    nofFailedTests += test_constructors (fileID);
    // // Test access to the attributes
    nofFailedTests += test_attributes (fileID);
    // // Test access to internal parameters
    nofFailedTests += test_parameters (fileID);

    if (testData) {
      // Test read/write access to the data
      nofFailedTests += test_data (fileID);
    }
    
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }
  
  //________________________________________________________
  // close HDF5 file
  
  H5Fclose(fileID);

  return nofFailedTests;
}
