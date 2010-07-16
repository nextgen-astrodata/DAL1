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
  
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  if (H5Iis_valid(fileID)) {
    cout << "-- Create file " << filename << endl;
  } else {
    cerr << "Failed to open file " << filename << endl;
    return 0;
  }
  
  //________________________________________________________
  // Parameters for creating dataset

  /*  Dataset shape (initial)  : [ 512 512 512 ]
   *  Data block shape         : [ 100 100 100 ]
   *  nof. blocks per axis     : 10
   *  Dataset shape (extended) : [ 1000 1000 1000 ]
   */

  int rank (3);
  int size (512);
  int nelem (100);
  std::vector<hsize_t> shape (rank,size);
  unsigned int startBlock (1);
  unsigned int nofBlocks (6);
  std::vector<int> start (rank);
  std::vector<int> block (rank,nelem);
  std::vector<int> stride;
  std::vector<int> count;
  std::vector<hsize_t> posStart;
  std::vector<hsize_t> posEnd;    
  
  //________________________________________________________
  // Apply hyperslab to dataset
  
  cout << "[1] Assign Hyperslab to dataset created with H5Dcreate ..." << endl;
  try {
    hid_t dataspaceID;
    hid_t datasetID;
    hsize_t dims[rank];
    hsize_t offset [rank];
    htri_t selectValid;
    std::string datasetName = "Dataset1";
    
    /* Create dataset */
    dims[0]     = size/2;
    dims[1]     = size;
    dims[2]     = size;
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
    dims[0] = nelem;
    dims[1] = nelem;
    dims[2] = nelem;
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
      // Get the bounding box of the hyperslab selection
      DAL::HDF5Hyperslab::getBoundingBox (dataspaceID,posStart,posEnd);
      cout << "--> Hyperslab : " << posStart << " .. " << posEnd << endl;
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
    unsigned int numBlock (0);
    // Create dataset
    HDF5Dataset dataset (fileID,
			 "Dataset2",
			 shape,
			 H5T_NATIVE_INT);
    // Reset unused hyperslab parameters
    stride.clear();
    count.clear();
    // Walk through the data volume
    for (unsigned int nx(startBlock); nx<nofBlocks; ++nx) {
      start[0] = nx*block[0];
      for (unsigned int ny(startBlock); ny<nofBlocks; ++ny) {
	start[1] = ny*block[1];
	for (unsigned int nz(startBlock); nz<nofBlocks; ++nz) {
	  start[2] = nz*block[2];
	  // Feedback
	  cout << "-- # block = " << numBlock
	       << " , start = " << start
	       << " , block = " << block
	       << " , end = " << DAL::HDF5Hyperslab::end(start,stride,count,block)
	       << endl << std::flush;
	  ++numBlock;
	  // Assign hyperslab selection
	  dataset.setHyperslab(start,block);
	}
      }
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
    unsigned int numBlock (0);
    // Create dataset
    HDF5Dataset dataset (fileID,
			 "Dataset3",
			 shape,
			 H5T_NATIVE_INT);
    // Reset unused hyperslab parameters
    stride.clear();
    count.clear();
    // Array with the data to be written
    unsigned int nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    int *data                  = new int [nofDatapoints];

    cout << "-- # datapoints = " << nofDatapoints << endl;

    for (unsigned int nx(startBlock); nx<nofBlocks; ++nx) {
      start[0] = nx*block[0];
      for (unsigned int ny(startBlock); ny<nofBlocks; ++ny) {
	start[1] = ny*block[1];
	for (unsigned int nz(startBlock); nz<nofBlocks; ++nz) {
	  start[2] = nz*block[2];
	  // Assign values to the data array written to file
	  for (unsigned int n(0); n<nofDatapoints; ++n) {
	    data[n] = numBlock;
	  }
	  // Feedback
	  cout << "-- # block = " << numBlock
	       << " , start = " << start
	       << " , block = " << block
	       << " , end = " << DAL::HDF5Hyperslab::end(start,stride,count,block)
	       << endl << std::flush;
	  ++numBlock;
	  // Write array to dataset
	  dataset.writeData(data,start,count,block);
	}
      }
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
  herr_t status;
  int rank (2);
  hsize_t dims[rank];
  hsize_t maxdims[rank];
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

  dims[0]    = 512;
  dims[1]    = 512;
  maxdims[0] = H5S_UNLIMITED;
  maxdims[1] = H5S_UNLIMITED;
  
  // Create dataspace
  hid_t dataspaceID = H5Screate_simple (rank, dims, maxdims);
  // Dataset creation property list
  hid_t dcpl = H5Pcreate (H5P_DATASET_CREATE);
  H5Pset_chunk (dcpl, rank, dims);

  // Create dataset
  hid_t datasetID = H5Dcreate (fileID,
			       datasetName.c_str(),
			       H5T_STD_I32LE,
			       dataspaceID,
			       H5P_DEFAULT,
			       dcpl,
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

  cout << "[2] Write blocks of data after extending the dataset ..." << endl;
  try {
    
    /* Define new dimensions for the dataset */
    
    for (int n(0); n<rank; ++n) {
      dims[n] *= 2;
    }

    /* Extend the size of the dataset */

    status      = H5Sclose (dataspaceID);
    status      = H5Dset_extent (datasetID, dims);
    dataspaceID = H5Dget_space(datasetID);
    std::cout << "Done." << std::endl << std::flush;

    /* Write data to the newly extended dataset area */

    for (unsigned int row(nofSteps); row<2*nofSteps; ++row) {
      // Set offset from which to start writing data
      start[0] = row*count[0];
      for (unsigned int col(nofSteps); col<2*nofSteps; ++col) {
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
  // Release HDF5 object handlers & allocated memory
  
  cout << "-- Releasing allocated memory ... " << std::flush;
  delete [] data;
  cout << "Done." << std::endl << std::flush;
  
  cout << "-- Releasing HDF5 object handlers ... " << std::flush;
  status = H5Dclose (datasetID);
  status = H5Sclose (dataspaceID);
  status = H5Sclose (memspace);
  status = H5Fclose (fileID);
  cout << "Done." << std::endl << std::flush;
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

//! Main function
int main ()
{
  int nofFailedTests (0);
  std::string filename ("tHDF5Hyperslab.h5");

  //________________________________________________________
  // Create HDF5 file to work with

  // create new file
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  // check if file creation was successful
  if (H5Iis_valid(fileID)) {
    cout << "-- Created file " << filename << endl;
  } else {
    cerr << "Failed to open file " << filename << endl;
    return 0;
  }
  // close file again
  H5Fclose (fileID);

  //________________________________________________________
  // Run the tests
  
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
