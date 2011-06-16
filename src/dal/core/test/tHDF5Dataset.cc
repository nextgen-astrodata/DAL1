/***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Lars B"ahren <lbaehren@gmail.com>                                     *
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

/*!
  \file tHDF5Dataset.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of tests for working with the DAL::HDF5Dataset class
 
  \author Lars B&auml;hren
 
  \date 2009/12/03
*/

#include <iostream>
#include <string>

#include <core/dalCommon.h>
#include <core/HDF5Attribute.h>
#include <core/HDF5Dataset.h>
#include "core_test.h"

using std::cerr;
using std::cout;
using std::endl;
using DAL::HDF5Attribute;
using DAL::HDF5Dataset;
using DAL::HDF5Hyperslab;

// ==============================================================================
//
//  Helper functions
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                 set_attributes

/*!
  \param data    -- Dataset for which to set the attributes
  \param name    -- Name of the attribute.
  \param shape   -- Shape of the attribute.
  \return status -- Status of the operation; returns \e false in case an error
          was encountered.
 */
bool set_attributes (DAL::HDF5Dataset &data,
		     std::vector<hsize_t> const &shape)
{
  bool status    = true;
  hid_t location = data.objectID();

  /* Write values of the attributes */
  HDF5Attribute::write (location, "NAME",  data.name()       );
  HDF5Attribute::write (location, "RANK",  int(shape.size()) );
  HDF5Attribute::write (location, "SHAPE", shape             );
  
  return status;
}

// ==============================================================================
//
//  Testing routines
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                      test_open

/*!
  \brief Test opening and inspecting provided test data set (h5example_dal.h5)

  The contents of the test HDF5 data set is organized as follows:
  \verbatim
  h5example_dal.h5
  /
  |-- Dataset_2D_int8                   array<2,int8>
  |-- Dataset_2D_int8_chunk             array<2,int8>
  |-- Dataset_2D_int16                  array<2,int16>
  |-- Dataset_2D_int16_chunk            array<2,int16>
  |-- Dataset_2D_int32                  array<2,int32>
  |-- Dataset_2D_int32_chunk            array<2,int32>
  |-- Dataset_2D_int64                  array<2,int64>
  |-- Dataset_2D_int64_chunk            array<2,int64>
  |-- Dataset_2D_float32                array<2,float32>
  |-- Dataset_2D_float64                array<2,float64>
  |-- Dataset_3D_int8                   array<3,int8>
  |-- Dataset_3D_int8_chunk             array<3,int8>
  |-- Dataset_3D_int16                  array<3,int16>
  |-- Dataset_3D_int16_chunk            array<3,int16>
  |-- Dataset_3D_int32
  |-- Dataset_3D_int32_chunk
  |-- Dataset_3D_int64
  |-- Dataset_3D_int64_chunk
  |-- Dataset_3D_float32
  |-- Dataset_3D_float32_chunk
  |-- Dataset_3D_float64
  `-- Dataset_3D_float64_chunk
  \endverbatim

  \param fileID          -- HDF5 object identifier for the file, to which the 
         dataset are attached.
  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_open (hid_t const &fileID)
{
  cout << "\n[tHDF5Datatset::test_open]\n" << endl;

  int nofFailedTests = 0;
  std::vector<std::string> names;
  std::vector<std::string>::iterator it;

  /* array<2,T> */
  names.push_back ("Dataset_2D_int8");
  names.push_back ("Dataset_2D_int8_chunk");
  names.push_back ("Dataset_2D_int16");
  names.push_back ("Dataset_2D_int16_chunk");
  names.push_back ("Dataset_2D_int32");
  names.push_back ("Dataset_2D_int32_chunk");
  names.push_back ("Dataset_2D_int64");
  names.push_back ("Dataset_2D_int64_chunk");
  names.push_back ("Dataset_2D_float32");
  names.push_back ("Dataset_2D_float32_chunk");
  names.push_back ("Dataset_2D_float64");
  names.push_back ("Dataset_2D_float64_chunk");
  /* array<3,T> */
  names.push_back ("Dataset_3D_int8");
  names.push_back ("Dataset_3D_int8_chunk");
  names.push_back ("Dataset_3D_int16");
  names.push_back ("Dataset_3D_int16_chunk");
  names.push_back ("Dataset_3D_int32");
  names.push_back ("Dataset_3D_int32_chunk");
  names.push_back ("Dataset_3D_int64");
  names.push_back ("Dataset_3D_int64_chunk");
  names.push_back ("Dataset_3D_float32");
  names.push_back ("Dataset_3D_float64");

  cout << "[1] Open datasets attached to the root group of the file ..." << endl;
  try {
    
    for (it=names.begin(); it!=names.end(); ++it) {
      // Open the dataset ...
      DAL::HDF5Dataset dataset (fileID, *it);
      // ... and show a summary of its properties
      dataset.summary();
    }

  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test the various constructors for an HDF5Dataset object

  \verbatim
  tHDF5Dataset.h5
  /
  |-- (Dataset_ND)                           does not exist, but test opening
  \endverbatim

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &fileID)
{
  cout << "\n[tHDF5Dataset::test_constructors]" << endl;
  
  int nofFailedTests = 0;
  hsize_t sidelength = 1000;

  /*_______________________________________________________________________
    Test for the default constructor to check if all internal parameters
    are being initialized.
  */

  cout << "\n[1] Testing HDF5Dataset() ..." << endl;
  try {
    DAL::HDF5Dataset dataset;
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Simplest constructor for HDF5Dataset will only open existing dataset,
    as parameters for the creation of a new one are not available.
  */

  cout << "\n[2] Testing HDF5Dataset(hid_t, string) ..." << endl;
  try {
    std::string name = "Dataset_ND";
    //
    DAL::HDF5Dataset dataset (fileID,
			      name);
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    If unset, the chunking size will be initialized with the overall shape
    of the dataset to be created; if however the resulting chunking size
    exceeds limit for chunks to be buffered, the chunking dimensions are
    adjusted automatically.
  */

  cout << "\n[3] Testing HDF5Dataset(hid_t, string, vector<hsize_t>) ..." << endl;
  try {
    std::string name ("Dataset");
    std::vector<hsize_t> shape (2, sidelength);
    //
    DAL::HDF5Dataset dataset (fileID,
			      name,
			      shape);
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Specify datatype of the individual array elements
  */

  cout << "\n[4] Testing HDF5Dataset(hid_t, string, vector<hsize_t>, hid_t) ..."
       << endl;
  try {
    unsigned int rank (2);
    std::vector<hsize_t> shape (rank, sidelength);
    std::map<std::string,hid_t> types = datasetNames (rank);
    std::map<std::string,hid_t>::iterator it;

    for (it=types.begin(); it!=types.end(); ++it) {
      // create dataset
      DAL::HDF5Dataset dataset (fileID,
				it->first,
				shape,
				it->second);
      // show summary of dataset properties
      dataset.summary();
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Test 5: Create new dataset, explicitely specifying the chunking
            dimensions.
  */

  cout << "\n[5] Testing HDF5Dataset(hid_t, string, vector<hsize_t>, vector<hsize_t>) ..."
       << endl;
  try {
    unsigned int rank (3);
    std::vector<hsize_t> shape (rank, sidelength);
    std::vector<hsize_t> chunk (rank, sidelength/4);
    std::map<std::string,hid_t> types = datasetNames (rank,true);
    std::map<std::string,hid_t>::iterator it;

    for (it=types.begin(); it!=types.end(); ++it) {
      // create dataset
      DAL::HDF5Dataset dataset (fileID,
				it->first,
				shape,
				chunk,
				it->second);
      // show summary of dataset properties
      dataset.summary();
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*_______________________________________________________________________
    Test 6: Re-open the previously created datasets
  */
  
  cout << "\n[6] Testing HDF5Dataset(hid_t, string) ..."
       << endl;
  try {
    unsigned int rank (2);
    std::map<std::string,hid_t> types = datasetNames (rank);
    std::map<std::string,hid_t>::iterator it;

    for (it=types.begin(); it!=types.end(); ++it) {
      // create dataset
      DAL::HDF5Dataset dataset (fileID,
				it->first);
      // show summary of dataset properties
      dataset.summary();
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*_______________________________________________________________________
    Test 7: Testing copy constructor
  */
  
  cout << "\n[7] Testing HDF5Dataset(HDF5Dataset) ..." << endl;
  try {
    unsigned int rank (2);
    std::vector<hsize_t> shape (rank);
    std::map<std::string,hid_t> types = datasetNames (rank);
    std::map<std::string,hid_t>::iterator it;

    for (it=types.begin(); it!=types.end(); ++it) {
      // Create/Open original dataset
      cout << "--> Original dataset object:" << endl;
      DAL::HDF5Dataset dataset (fileID,
				it->first);
      dataset.summary();
      // Create new dataset as copy of the first one
      cout << "--> Copied dataset object:" << endl;
      DAL::HDF5Dataset datasetCopy (dataset);
      datasetCopy.summary();
      // Retrieve some of the basic dataset parameters ...
      shape = datasetCopy.shape();
      rank  = datasetCopy.rank();
      // ... and store them as attributes.
      dataset.writeAttribute("Rank",  rank);
      dataset.writeAttribute("Shape", shape);
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_array1d

/*!
  \brief Test application of hyperslab to write and read 1-dim array data

  \param fileID          -- HDF5 object identifier for the file, to which the 
         dataset are attached.
  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_array1d (hid_t const &fileID)
{
  cout << "\n[tHDF5Datatset::test_array1d]\n" << endl;

  int nofFailedTests         = 0;
  unsigned int nofSteps      = 1;
  unsigned int nofDatapoints = 1;
  std::string name           = "Array1D";

  //________________________________________________________
  // Open/Create dataset to work with

  std::vector<hsize_t> shape (1,1024);
  std::vector<int> start;
  std::vector<int> stride;
  std::vector<int> count;
  std::vector<int> block;

  DAL::HDF5Dataset dataset (fileID,
			    name,
			    shape);
  set_attributes (dataset, shape);
  dataset.summary();

  //________________________________________________________
  // Run the tests
  
  cout << "[1] Write data to 1D dataset ..." << endl;
  try {
    start.resize(shape.size());
    count.resize(shape.size());
    block.resize(shape.size());

    nofSteps      = 16;
    count[0]      = 1;
    block[0]      = shape[0]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];
    
    for (unsigned int step=0; step<nofSteps; ++step) {
      // set the starting position
      start[0] = step*shape[0]/nofSteps;
      // assign data values to be written to the dataset
      for (int n=0; n<block[0]*count[0]; ++n) {
	data[n] = step+1;
      }
      // summary
      cout << "\tStart = "  << start
	   << "\tCount = "  << count
	   << "\tBlock = "  << block
	   << "\t# data = " << nofDatapoints
	   << endl;
      // Write the data
      dataset.writeData (data,start,block);
    }
    
    // release allocated memory
    delete [] data;
    
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 2: Read back in the data from the previously created dataset.
   */

  cout << "[2] Read data from 1D dataset ..." << endl;
  try {
    start.resize(shape.size());
    block.resize(shape.size());
    
    nofSteps      = 8;
    count[0]      = 1;
    block[0]      = shape[0]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];
    
    for (unsigned int step=0; step<nofSteps; ++step) {
      // set the starting position
      start[0] = step*shape[0]/nofSteps;
      // summary
      cout << "\tStart = "  << start
	   << "\tCount = "  << count
	   << "\tBlock = "  << block
	   << "\t# data = " << nofDatapoints
	   << endl;
      // Write the data
      dataset.readData (data,start,block);
      // Show the data
      cout << "[";
      for (int n(0); n<block[0]; ++n) {
	cout << " " << data[n];
      }
      cout << " ]" << endl;
    }

    // release allocated memory
    delete [] data;
    
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_array2d

/*!
  \brief Test application of hyperslab to write and read 2-dim array data

  - Access data by column.
  - Access data by row.
  - Access data by 2D sub-array.

  \param filename -- Name of the HDF5 file, within which the datasets are being
         created.

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_array2d (hid_t const &fileID)
{
  cout << "\n[tHDF5Datatset::test_array2d]\n" << endl;

  int nofFailedTests (0);
  std::string groupname ("Group");
  unsigned int nofSteps;
  unsigned int nofDatapoints;
  // Hyperslab parameters
  unsigned int sidelength (1024);
  std::vector<hsize_t> shape (2,sidelength);
  std::vector<int> start;
  std::vector<int> stride;
  std::vector<int> count;
  std::vector<int> block;

  //________________________________________________________
  // Run the tests

  cout << "[1] Write data to 2D dataset by row ..." << endl;
  try {
    std::string name ("Array2D_rows");
    DAL::HDF5Dataset dataset (fileID,
			      name,
			      shape);
    set_attributes (dataset, shape);
    
    start.resize(shape.size());
    count.resize(shape.size());
    block.resize(shape.size());

    nofSteps      = sidelength;
    count[0]      = 1;
    count[1]      = 1;
    block[0]      = 1;
    block[1]      = sidelength;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];

    // Write the data to the dataset

    for (unsigned int step(0); step<nofSteps; ++step) {
      // set the starting position
      start[0] = step;
      start[1] = 0;
      // assign data values to be written to the dataset
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step+1;
      }
      // Write the data
      dataset.writeData (data,start,block);
    }

    // release allocated memory
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[2] Write data to 2D dataset by column ..." << endl;
  try {
    std::string name ("Array2D_columns");
    DAL::HDF5Dataset dataset (fileID,
			      name,
			      shape);
    set_attributes (dataset, shape);
    
    start.resize(shape.size());
    count.resize(shape.size());
    block.resize(shape.size());

    nofSteps      = sidelength;
    count[0]      = 1;
    count[1]      = 1;
    block[0]      = sidelength;
    block[1]      = 1;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];

    // Write the data to the dataset

    for (unsigned int step(0); step<nofSteps; ++step) {
      // set the starting position
      start[0] = 0;
      start[1] = step;
      // assign data values to be written to the dataset
      for (unsigned int n(0); n<nofDatapoints; ++n) {
	data[n] = step+1;
      }
      // Write the data
      dataset.writeData (data,start,block);
    }

    // release allocated memory
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  cout << "[3] Write data to 2D dataset by 2D blocks ..." << endl;
  try {
    std::string name ("Array2D_blocks");
    DAL::HDF5Dataset dataset (fileID,
			      name,
			      shape);
    set_attributes (dataset, shape);
    
    start.resize(shape.size());
    count.resize(shape.size());
    block.resize(shape.size());

    nofSteps      = 16;
    count[0]      = 1;
    count[1]      = 1;
    block[0]      = shape[0]/(nofSteps*count[0]);
    block[1]      = shape[1]/(nofSteps*count[1]);
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];

    // Write the data to the dataset

    unsigned int val (0);
    
    for (unsigned int nx(0); nx<nofSteps; ++nx) {
      start[0] = nx*shape[0]/nofSteps;
      for (unsigned int ny(0); ny<nofSteps; ++ny, ++val) {
	start[1] = ny*shape[1]/nofSteps;
	// assign data values to be written to the dataset
	for (unsigned int n(0); n<nofDatapoints; ++n) {
	  data[n] = val;
	}
	// Write the data
	dataset.writeData (data,start,block);
      }
    }

    // release allocated memory
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                 test_hyperslab

/*!
  \param filename -- Name of the HDF5 file, within which the datasets are being
         created.

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_hyperslab (hid_t const &fileID)
{
  cout << "\n[tHDF5Datatset::test_hyperslab]\n" << endl;

  int nofFailedTests (0);

  hid_t groupID = H5Gcreate (fileID,
			     "Hyperslabs",
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT);

  //________________________________________________________
  // Set attributes attached to the datasets

  std::set<std::string> attributes;

  attributes.insert("shape");
  attributes.insert("start");
  attributes.insert("stride");
  attributes.insert("count");
  attributes.insert("block");
  attributes.insert("end");

  //________________________________________________________
  // Run the tests

  unsigned int rank (2);
  unsigned int sidelength (1024);
  unsigned int nofDatapoints (16);
  std::vector<hsize_t> shape (rank,sidelength);
  std::vector<int> start (rank);
  std::vector<int> block (rank);

  cout << "[1] Create reference dataset" << endl;
  try {
    DAL::HDF5Dataset dataset (groupID,
			      "test0",
			      shape);
    set_attributes (dataset, shape);
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[2] Write single block of data" << endl;
  try {
    std::vector<int> count;

    start[0] = 10;
    start[1] = 10;
    block[0] = 4;
    block[1] = 4;

    cout << "-- start = " << start << endl;
    cout << "-- count = " << count << endl;
    cout << "-- block = " << block << endl;

    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];
    
    for (unsigned int n(0); n<nofDatapoints; ++n) {
      data[n] = n+1.0;
    } 
    
    DAL::HDF5Dataset dataset (groupID, "test1", shape);
    set_attributes (dataset, shape);

    dataset.writeData (data,start,block);
    
    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[3] Write multiple blocks" << endl;
  try {
    std::vector<int> stride (rank);
    std::vector<int> count (rank);
    
    start[0]  = 5;
    start[1]  = 5;
    stride[0] = 10;
    stride[1] = 10;
    count[0]  = 2;
    count[1]  = 2;
    block[0]  = 5;
    block[1]  = 5;

    cout << "-- start  = " << start  << endl;
    cout << "-- stride = " << stride << endl;
    cout << "-- count  = " << count  << endl;
    cout << "-- block  = " << block  << endl;

    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];
    
    for (unsigned int n(0); n<nofDatapoints; ++n) {
      data[n] = n+1.0;
    } 
    
    HDF5Dataset dataset (groupID, "test2", shape);
    HDF5Hyperslab slab (start,stride,count,block);

    dataset.writeData (data,slab);

    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[4] Write multiple blocks" << endl;
  try {
    std::vector<int> stride (rank);
    std::vector<int> count (rank);
    
    start[0]  = 5;
    start[1]  = 5;
    stride[0] = 5;
    stride[1] = 5;
    count[0]  = 2;
    count[1]  = 2;
    block[0]  = 5;
    block[1]  = 5;

    cout << "-- start  = " << start  << endl;
    cout << "-- stride = " << stride << endl;
    cout << "-- count  = " << count  << endl;
    cout << "-- block  = " << block  << endl;

    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];
    
    for (unsigned int n(0); n<nofDatapoints; ++n) {
      data[n] = n+1.0;
    } 
    
    HDF5Dataset dataset (groupID, "test3", shape);
    HDF5Hyperslab slab (start,stride,count,block);

    dataset.writeData (data,slab);

    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  //________________________________________________________
  // Release object handles

  H5Gclose(groupID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                 test_extension

/*!
  \brief Test dynamic extension of a dataset

  \param filename -- Name of the HDF5 file, within which the datasets are being
         created.

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_extension (std::string const &filename)
{
  cout << "\n[tHDF5Datatset::test_extension]\n" << endl;

  int nofFailedTests (0);
  unsigned int nofDatapoints;
  unsigned int rank (2);
  unsigned int sidelength (100);
  std::vector<hsize_t> shape (rank,sidelength);
  std::vector<int> start (rank);
  std::vector<int> count (rank);
  std::vector<int> block (rank);

  //________________________________________________________
  // Open the file to work with
  
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);

  if (!H5Iis_valid(fileID)) {
    std::cerr << "Failed to open file " << filename << endl;
    return 0;
  }

  hid_t groupID = H5Gcreate (fileID,
			     "ExtendableDatasets",
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT);

  //__________________________________________________________________
  // Test 1 : Write tiles of sidelength "sidelength/nofSteps" within
  //          the existing boundaries of the dataset.

  std::cout << "[1] Write data within the existing boundaries ..." << std::endl;
  {
    HDF5Dataset dataset (groupID, "test1", shape);
    unsigned int nofSteps = 4;
    unsigned int n        = 0;

    block[0] = sidelength/nofSteps;
    block[1] = sidelength/nofSteps;
    count.clear();

    // allocate memory for the data array
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];

    for (unsigned int nx=0; nx<nofSteps; ++nx) {
      start[0] = nx*sidelength/nofSteps;
      for (unsigned int ny=0; ny<nofSteps; ++ny) {
	start[1] = ny*sidelength/nofSteps;
	// hyperslab position
	std::cout << "-- start = " << start
		  << ", block = " << block << std::endl;
	// assign data array
	for (n=0; n<nofDatapoints; ++n) {
	  data[n] = 1.0*(nofSteps*nx+ny);
	}
	// write the array to the dataset
	dataset.writeData (data, start, count, block);
      }
    }

    // release allocated memory
    delete [] data;
  }
  
  //__________________________________________________________________
  // Test 2 : Write tiles in total exceeding the original boundaries
  //          of the dataset.

  std::cout << "[2] Write data exceeding the original boundaries ..." << std::endl;
  {
    HDF5Dataset dataset (groupID, "test2", shape);
    unsigned int nofSteps = 4;
    unsigned int n        = 0;

    block[0] = 2*sidelength/nofSteps;
    block[1] = 2*sidelength/nofSteps;
    count.clear();

    // allocate memory for the data array
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];

    for (unsigned int nx=0; nx<nofSteps; ++nx) {
      start[0] = nx*sidelength/nofSteps;
      for (unsigned int ny=0; ny<nofSteps; ++ny) {
	start[1] = ny*sidelength/nofSteps;
	// hyperslab position
	std::cout << "-- start = " << start
		  << ", block = " << block << std::endl;
	// assign data array
	for (n=0; n<nofDatapoints; ++n) {
	  data[n] = 1.0*(nofSteps*nx+ny);
	}
	// write the array to the dataset
	dataset.writeData (data, start, count, block);
      }
    }

    // release allocated memory
    delete [] data;
  }
 
  //__________________________________________________________________
  // Test 3 : 
 
  std::cout << "[3] Write data exceeding the original boundaries ..." << std::endl;
  {
    HDF5Dataset dataset (groupID, "test3", shape);
    unsigned int nofSteps = 4;
    unsigned int n        = 0;

    block[0] = 2*sidelength/nofSteps;
    block[1] = sidelength/nofSteps;
    count.clear();

    // allocate memory for the data array
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];

    for (unsigned int nx=0; nx<nofSteps; ++nx) {
      start[0] = nx*sidelength/nofSteps;
      for (unsigned int ny=0; ny<nofSteps; ++ny) {
	start[1] = ny*sidelength/nofSteps;
	// hyperslab position
	std::cout << "-- start = " << start
		  << ", block = " << block << std::endl;
	// assign data array
	for (n=0; n<nofDatapoints; ++n) {
	  data[n] = 1.0*(nofSteps*nx+ny);
	}
	// write the array to the dataset
	dataset.writeData (data, start, count, block);
      }
    }

    // release allocated memory
    delete [] data;
  }
 
  //________________________________________________________
  // Close the file

  H5Gclose(groupID);
  H5Fclose(fileID);
  
  return nofFailedTests;
}

// ==============================================================================
//
//  Main program routine
//
// ==============================================================================

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main (int argc,
          char *argv[])
{
  int nofFailedTests   = 0;
  hid_t fileID         = 0;
  bool haveDataset     = false;
  std::string filename = "tHDF5Dataset.h5";

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }
  
  std::cout << "[tHDF5Dataset] Output HDF5 file = " << filename << std::endl;

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
  
  /* If file creation was successful, run the tests. */
  if (H5Iis_valid(fileID)) {

    if (haveDataset) {

      // Test opening provided test data set (h5example_dal.h5)
      nofFailedTests += test_open (fileID);

    } else {
      
      // Test the various constructors for an HDF5Dataset object
      nofFailedTests += test_constructors (fileID);
      // Test access R/W access to 1-dim data arrays
      nofFailedTests += test_array1d (fileID);
      // Test access R/W access to 2-dim data arrays
      nofFailedTests += test_array2d (fileID);
      // // Test the effect of the various Hyperslab parameters
      // nofFailedTests += test_hyperslab (fileID);
      // // Test expansion of extendable datasets
      // nofFailedTests += test_extension (fileID);
      
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
