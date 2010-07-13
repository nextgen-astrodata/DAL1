/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 3179 2009-10-14 13:20:10Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#include <HDF5Dataset.h>
#include <HDF5Hyperslab.h>

// Namespace usage
using std::cerr;
using std::cout;
using std::endl;
using DAL::HDF5Dataset;
using DAL::HDF5Hyperslab;

/*!
  \file tHDF5Hyperslab.cc

  \ingroup DAL
  \ingroup data_common

  \brief A collection of test routines for the HDF5Hyperslab class
 
  \author Lars B&auml;hren
 
  \date 2010/02/09
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new HDF5Hyperslab object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tHDF5Hyperslab::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] HDF5Hyperslab () ..." << endl;
  try {
    HDF5Hyperslab slab;
    //
    slab.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] HDF5Hyperslab (rank) ..." << endl;
  try {
    int rank (2);
    //
    HDF5Hyperslab slab (rank);
    slab.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] HDF5Hyperslab (start,block) ..." << endl;
  try {
    int rank (2);
    std::vector<hsize_t> shape (rank);
    std::vector<int> start (rank);
    std::vector<int> block (rank);
    //
    shape[0] = 1024;
    shape[1] = 10;
    start[0] = 0;
    start[1] = 0;
    block[0] = shape[0]/2;
    block[1] = shape[1]/2;
    //
    HDF5Hyperslab slab (start,block);
    slab.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] HDF5Hyperslab (start,stride,count,block) ..." << endl;
  try {
    int rank (2);
    std::vector<hsize_t> shape (rank);
    std::vector<int> start (rank);
    std::vector<int> stride (rank);
    std::vector<int> count (rank);
    std::vector<int> block (rank);
    //
    shape[0]  = 1024;
    shape[1]  = 10;
    start[0]  = shape[0]/4;
    start[1]  = shape[1]/4;
    stride[0] = 1;
    stride[1] = 1;
    count[0]  = 1;
    count[1]  = 1;
    block[0]  = shape[0]/2;
    block[1]  = shape[1]/2;
    //
    HDF5Hyperslab slab (start,stride,count,block);
    slab.summary(); 
    //
    count[0]  = 2;
    count[1]  = 2;
    //
    HDF5Hyperslab slab2 (start,stride,count,block);
    slab2.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}


//_______________________________________________________________________________
//                                                          test_parameter_access

/*!
  \brief Test access to the object's internal parameters

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_parameter_access ()
{
  cout << "\n[tHDF5Hyperslab::test_parameter_access]\n" << endl;

  int nofFailedTests (0);
  int rank (3);

  cout << "[1] HDF5Hyperslab::setStart(std::vector<int>) ..." << endl;
  try {
    std::vector<int> start (rank,100);
    //
    HDF5Hyperslab slab;
    slab.setStart (start);
    // 
    cout << "-- start = " << slab.start() << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] HDF5Hyperslab::setStride(std::vector<int>) ..." << endl;
  try {
    std::vector<int> stride (rank,100);
    //
    HDF5Hyperslab slab;
    slab.setStride (stride);
    // 
    cout << "-- stride = " << slab.stride() << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] HDF5Hyperslab::setCount(std::vector<int>) ..." << endl;
  try {
    std::vector<int> count (rank,100);
    //
    HDF5Hyperslab slab;
    slab.setCount (count);
    // 
    cout << "-- count = " << slab.count() << endl;
    //
    count.clear();
    slab.setCount (count);
    cout << "-- count = " << slab.count() << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[4] HDF5Hyperslab::setBlock(std::vector<int>) ..." << endl;
  try {
    std::vector<int> block (rank,100);
    //
    HDF5Hyperslab slab;
    slab.setBlock (block);
    // 
    cout << "-- block = " << slab.block() << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                          test_static_functions

/*!
  \brief Test the static methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_functions ()
{
  cout << "\n[tHDF5Hyperslab::test_test_static_functions]\n" << endl;

  int nofFailedTests (0);

  try {
    unsigned int nelem (3);
    std::vector<int> start (nelem,0);
    std::vector<int> block (nelem,10);
    std::vector<int> stride;
    std::vector<int> count;

    cout << "-- start        = " << start  << endl;
    cout << "-- block        = " << block  << endl;
    cout << "-- stride       = " << stride << endl;
    cout << "-- count        = " << count  << endl;
    cout << "--> end         = " << HDF5Hyperslab::end (start,stride,count,block) << endl;
    cout << "--> nof. points = " << HDF5Hyperslab::nofDatapoints (count,block) << endl;
    cout << endl;

    count.resize(nelem);
    count = std::vector<int> (nelem,5);

    cout << "-- start        = " << start  << endl;
    cout << "-- block        = " << block  << endl;
    cout << "-- stride       = " << stride << endl;
    cout << "-- count        = " << count  << endl;
    cout << "--> end         = " << HDF5Hyperslab::end (start,stride,count,block) << endl;
    cout << "--> nof. points = " << HDF5Hyperslab::nofDatapoints (count,block) << endl;
    cout << endl;
    
    stride.resize(nelem);
    stride = std::vector<int> (nelem,2);

    cout << "-- start        = " << start  << endl;
    cout << "-- block        = " << block  << endl;
    cout << "-- stride       = " << stride << endl;
    cout << "-- count        = " << count  << endl;
    cout << "--> end         = " << HDF5Hyperslab::end (start,stride,count,block) << endl;
    cout << "--> nof. points = " << HDF5Hyperslab::nofDatapoints (count,block) << endl;
    
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_setHyperslab

/*!
  \brief Test setting hyperslab selection for a dataset

  \param filename -- Name of the file within within which the test datasets are
         being created.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_setHyperslab (std::string const &filename)
{
  cout << "\n[tHDF5Hyperslab::test_setHyperslab]\n" << endl;

  int nofFailedTests (0);

  //________________________________________________________
  // Open the file to work with
  
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  
  if (H5Iis_valid(fileID)) {
    cout << "-- Create file " << filename << endl;
  } else {
    cerr << "Failed to open file " << filename << endl;
    return 0;
  }
  
  //________________________________________________________
  // Parameters for creating dataset

  int rank (3);
  int size (1024);
  int nelem (100);
  std::vector<hsize_t> shape (rank,size);
  unsigned int nofBlocks (15);
  std::vector<int> start (rank);
  std::vector<int> block (rank,nelem);
  std::vector<int> stride;
  std::vector<int> count;

  //________________________________________________________
  // Apply hyperslab to dataset

  cout << "[1] Assign Hyperslab to dataset create with H5Dcreate ..." << endl;
  try {
    hid_t dataspaceID;
    hid_t datasetID;
    hsize_t dims[rank];
    hsize_t offset [rank];
    htri_t selectValid;
    std::string datasetName = "Dataset1";

    /* Create dataset */
    dims[0]     = 512;
    dims[1]     = 1024;
    dims[2]     = 1024;
    dataspaceID = H5Screate_simple (rank, dims, NULL);
    datasetID   = H5Dcreate2 (fileID,
			      datasetName.c_str(),
			      H5T_STD_I32LE,
			      dataspaceID,
			      H5P_DEFAULT,
			      H5P_DEFAULT,
			      H5P_DEFAULT);
    
    if (H5Iis_valid(datasetID)) {
      DAL::h5get_dataspace_shape (datasetID,shape);
      //
      cout << "-- Opened dataset " << datasetName << endl;
      cout << "-- Shape = " << shape << endl;
    } else {
      cerr << "Failed to open dataset " << datasetName << endl;
      return 0;
    }

    /* Step through the dataset array using hyperslab definition */
    dims[0] = 100;
    dims[1] = 100;
    dims[2] = 100;
    for (unsigned int block(0); block<5; ++block) {
      cout << "-- Assigning hyperslab for block " << block << " ..." << endl;
      // Set offset marking start position of hyperset
      offset[0] = offset[1] = offset[2] = block;
      // Assign hyperslab
      H5Sselect_hyperslab (dataspaceID,
			   H5S_SELECT_SET,
			   offset,
			   NULL,
			   dims,
			   NULL);
      // Check assignment of Hyperslab
      DAL::HDF5Hyperslab::checkSelectionValid (dataspaceID,selectValid);
    }
    
    /* Release HDF5 object handlers */
    H5Dclose (datasetID);
    H5Sclose (dataspaceID);
    
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << std::flush;

  //________________________________________________________
  // Assign hyperslab without writing of data

  cout << "[2] Assign hyperslab without writing of data ..." << endl;
  try {
    // Create dataset
    HDF5Dataset dataset (fileID,
			 "Dataset2",
			 shape,
			 H5T_NATIVE_INT);
    // Walk through the data volume
    for (unsigned int numBlock(0); numBlock<nofBlocks; ++numBlock) {
      /* Set starting point of hyperslab */
      start[0] = numBlock*nelem;
      start[1] = numBlock*nelem;
      start[2] = numBlock*nelem;
      /* Feedback */
      cout << "-- block = " << numBlock
		<< " , start = " << start
		<< " , end = " << DAL::HDF5Hyperslab::end(start,stride,count,block)
		<< std::endl << std::flush;
      /* Set the hyperslab */
      dataset.setHyperslab (start,block);
    }
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << std::flush;

  //________________________________________________________
  // Write blocks of data to dataset

  cout << "[3] Assign hyperslab before writing of data ..." << endl;
  try {
    // Create dataset
    HDF5Dataset dataset (fileID,
			 "Dataset3",
			 shape,
			 H5T_NATIVE_INT);

    // Array with the data to be written
    unsigned int nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    int *data                  = new int [nofDatapoints];

    // Walk through the data volume
    for (unsigned int numBlock(0); numBlock<nofBlocks; ++numBlock) {
      /* Assign the data points */
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = numBlock+1;
      }
      /* Set starting point of hyperslab */
      start[0] = start[1] = start[2] = numBlock*nelem;
      /* Feedback */
      cout << "-- block = " << numBlock
	   << " , start = " << start
	   << " , count = " << count
	   << " , block = " << block
	   << " , #data = " << nofDatapoints << endl << std::flush;
      /* Write the block to data to the dataset */
      dataset.writeData (data,start,count,block);
    }

    /* Release allocated memory */
    delete [] data;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << std::flush;

  //________________________________________________________
  // Close the file

  H5Fclose(fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_hdf5

/*!
  \brief Test working with the low-level HDF5 library functions

  While the rest of the test functions make use of functionality provided by the
  DAL, this function should try some testing only using the bare HDF5 library 
  methods, in order to help isolating errors in the more high-level interfaces
  to datasets and Hyperslabs.

  \param filename -- Name of the file within within which the test datasets are
         being created.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_hdf5 (std::string const &filename)
{
  cout << "\n[tHDF5Hyperslab::test_hdf5]\n" << endl;

  int nofFailedTests (0);
  std::string datasetName ("DatasetInt");
  herr_t  status;
  int rank (2);
  hsize_t dims[rank];
  hsize_t start[rank];
  hsize_t count[rank];
  unsigned int nofSteps (16);

  //________________________________________________________
  // Open the file to work with
  
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  if (H5Iis_valid(fileID)) {
    cout << "-- Opened file " << filename << endl;
  } else {
    cerr << "Failed to open file " << filename << endl;
    return 0;
  }
  
  //________________________________________________________
  // Create dataset to work with

  dims[0] = 1024;
  dims[1] = 1024;

  // Create dataspace
  hid_t dataspaceID = H5Screate_simple (rank, dims, NULL);

  // Create dataset
  hid_t datasetID = H5Dcreate (fileID,
			       datasetName.c_str(),
			       H5T_STD_I32LE,
			       dataspaceID,
			       H5P_DEFAULT,
			       H5P_DEFAULT,
			       H5P_DEFAULT);

  if (H5Iis_valid(datasetID)) {
    cout << "-- Opened dataset " << datasetName << " of shape ["
	 << dims[0] << "," << dims[1] << "]" << endl;
  } else {
    cerr << "Failed to open dataset " << datasetName << endl;
    return 0;
  }
  
  //________________________________________________________
  // Set up parameters for the data array to be written
  
  count[0]          = dims[0]/nofSteps;
  count[1]          = dims[1]/nofSteps;
  hid_t memspace    = H5Screate_simple(rank, count, NULL);
  int nofDatapoints = count[0]*count[1];
  int *data         = new int[nofDatapoints];
  
  //________________________________________________________
  // Write blocks of data without extending the dataset

  cout << "[1] Write blocks of data without extending the dataset ..." << endl;
  try {
    for (unsigned int row(0); row<nofSteps; ++row) {
      // Set offset from which to start writing data
      start[0] = row*count[0];
      for (unsigned int col(0); col<nofSteps; ++col) {
	// Set offset from which to start writing data
	start[1] = col*count[1];
	// Assign values to the data array written to file
	for (int n(0); n<nofDatapoints; ++n) {
	  data[n] = 100*row+col;
	}
	// Hyperslab selection on dataspace
	status = H5Sselect_hyperslab (dataspaceID,
				      H5S_SELECT_SET,
				      start,
				      NULL, 
				      count,
				      NULL);
	// Write the data to file
	status = H5Dwrite (datasetID,
			   H5T_NATIVE_INT,
			   memspace,
			   dataspaceID,
			   H5P_DEFAULT,
			   data);
      }
    }
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //________________________________________________________
  // Write blocks of data after extending the dataset

  for (int n(0); n<rank; ++n) {
    dims[n] *= 2;
  }

  //________________________________________________________
  // Release HDF5 object handlers & allocated memory
  
  delete [] data;

  cout << "-- Release HDF5 object handlers ... " << std::endl;
  status = H5Dclose (datasetID);
  status = H5Sclose (dataspaceID);
  status = H5Fclose(fileID);
  
  return nofFailedTests;
}
  
//_______________________________________________________________________________
//                                                                           main

//! Main function
int main ()
{
  int nofFailedTests (0);
  std::string filename ("tHDF5Hyperslab.h5");

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test the static methods
  nofFailedTests += test_parameter_access ();
  // Test the static methods
  nofFailedTests += test_static_functions ();
  //! Test setting hyperslab selection for a dataset
  nofFailedTests += test_setHyperslab (filename);
  //! Test working with the low-level HDF5 library functions
  nofFailedTests += test_hdf5 (filename);  

  return nofFailedTests;
}
