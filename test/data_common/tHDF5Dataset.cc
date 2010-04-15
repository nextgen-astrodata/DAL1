/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

/*!
  \file tHDF5Dataset.cc

  \ingroup DAL
  \ingroup data_common

  \brief A collection of tests for working with the DAL::HDF5Dataset class
 
  \author Lars B&auml;hren
 
  \date 2009/12/03
*/

#include <iostream>
#include <string>

#include <dalCommon.h>
#include "HDF5Dataset.h"

using std::cerr;
using std::cout;
using std::endl;
using DAL::HDF5Dataset;
using DAL::HDF5Hyperslab;

//_______________________________________________________________________________
//                                                                    test_create

/*!
  \brief Find a dataset within the HDF5 file \e filename

  \param filename -- Name of the HDF5 file.
  \retval dataset -- Name of the dataset.

  \return status -- Returns \e true if a dataset could be located.
*/
bool find_dataset (std::string const &filename,
		   std::string &dataset)
{
  bool haveDataset (false);
  hid_t location;
  std::string name;
  std::vector<hid_t> locations;
  std::vector<hid_t>::reverse_iterator loc;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  dataset="";

  //______________________________________________
  // Open the HDF5 file to inspect
  
  location = H5Fopen (filename.c_str(),
		      H5F_ACC_RDWR,
		      H5P_DEFAULT);
  if (H5Iis_valid(location)) {
    locations.push_back(location);
  } else {
    return false;
  }
  
  //______________________________________________
  // Go through the data structure

  while (!haveDataset) {
    // Check for dataset attached to the current location
    cout << "-- Checking for datasets location " << location << " ..." << endl;
    names.clear();
    DAL::h5get_names (names,location,H5G_DATASET);
    // Did we find a dataset?
    if (names.empty()) {
      // Check for groups attached to the current location
      cout << "-- Checking for groups location " << location << " ..." << endl;
      names.clear();
      DAL::h5get_names (names,location,H5G_GROUP);
      // Did we find a group? If not, that's it.
      if (names.empty()) {
	return false;
      }
      else {
	it    = names.begin();
	name += "/" + *it;
	// Open HDF5 group
	cout << "-- Opening group " << name << " ..." << endl;
	location = H5Gopen (locations[0],
			    name.c_str(),
			    H5P_DEFAULT);
	// Store object identifier
	if (H5Iis_valid(location)) {
	  locations.push_back(location);
	}
      }
    }
    else {
      // Store information
      it          = names.begin();
      dataset     = name + "/" + *it;
      haveDataset = true;
      // Feedback
      std::cout << "-- Dataset       = " << dataset      << std::endl;
      std::cout << "-- nof. datasets = " << names.size() << std::endl;
    }
  }  //  END -- while (!haveDataset)
  
  //______________________________________________
  // Close HDF5 object identifiers

  for (loc=locations.rbegin(); loc!=locations.rend(); ++loc) {
    if (H5Iis_valid(*loc)) {
      switch (H5Iget_type(*loc)) {
      case H5I_FILE:
	cout << "-- Closing HDF5 file " << *loc << " ..." << endl;
	H5Fclose(*loc);
	break;
      case H5I_GROUP:
	cout << "-- Closing HDF5 group " << *loc << " ..." << endl;
	H5Gclose(*loc);
	break;
      default:
	H5Oclose(*loc);
      break;
      }
    }
  }

  return haveDataset;
}

//_______________________________________________________________________________
//                                                                    test_create

/*!
  \brief Test the creation of a Dataset object within a file

  The generated HDF5 file will have the following structure:
  \verbatim
  /
  |-- Data1D             ...  Dataset
  |-- Data2D             ...  Dataset
  |-- Data3D             ...  Dataset
  `-- Group              ...  Group
      |-- Data1D         ...  Dataset
      |-- Data2D         ...  Dataset
      `-- Data3D         ...  Dataset
  \endverbatim

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_create (std::string const &filename="tHDF5Dataset.h5")
{
  cout << "\n[tHDF5Datatset::test_create]\n" << endl;

  int nofFailedTests (0);
  std::string name;
  std::string groupname ("Group");

  //________________________________________________________
  // Create the file to work with

  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  hid_t groupID = H5Gcreate (fileID,
			     groupname.c_str(),
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT);
  
  //________________________________________________________
  // Run the tests
  
  cout << "[1] Creating 1D dataset ..." << endl;
  try {
    std::vector<hsize_t> shape (1);
    
    name     = "Data1D";
    shape[0] = 1024;
    
    /* Create HDF5 Dataset at the root level of the file */
    HDF5Dataset dataset (fileID,
			 name,
			 shape);
    dataset.addAttribute("NOF_AXES");
    dataset.addAttribute("NAXIS1");
    dataset.summary();
    
    dataset.setAttribute ("NOF_AXES", int(shape.size()) );
    dataset.setAttribute ("NAXIS1",   int(shape[0])     );
    
    /* Create HDF5 dataset within group */
    HDF5Dataset datasetGroup1 (groupID,
			       name,
			       shape);
    datasetGroup1.addAttribute("NOF_AXES");
    datasetGroup1.addAttribute("NAXIS1");
    datasetGroup1.setAttribute ("NOF_AXES", int(shape.size()) );
    datasetGroup1.setAttribute ("NAXIS1",   int(shape[0])     );

    /* Create HDF5 dataset within group */
    std::string path = "/" + groupname + "/Array1D";
    DAL::HDF5Dataset datasetGroup2 (fileID,
				    path.c_str(),
				    shape);
    datasetGroup2.addAttribute("NOF_AXES");
    datasetGroup2.addAttribute("NAXIS1");
    datasetGroup2.setAttribute ("NOF_AXES", int(shape.size()) );
    datasetGroup2.setAttribute ("NAXIS1",   int(shape[0])     );
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[2] Creating 2D dataset ..." << endl;
  try {
    std::vector<hsize_t> shape (2);
    
    name     = "Data2D";
    shape[0] = 1024;
    shape[1] = 4;
    
    /* Create HDF5 Dataset at the root level of the file */
    DAL::HDF5Dataset dataset (fileID,
			      name,
			      shape);
    dataset.addAttribute("NOF_AXES");
    dataset.addAttribute("NAXIS1");
    dataset.addAttribute("NAXIS2");
    dataset.summary();
    
    dataset.setAttribute ("NOF_AXES", int(shape.size()) );
    dataset.setAttribute ("NAXIS1",   int(shape[0])     );
    dataset.setAttribute ("NAXIS2",   int(shape[1])     );

    /* Create HDF5 dataset within group */
    DAL::HDF5Dataset datasetGroup (groupID,
				   name,
				   shape);
    datasetGroup.addAttribute("NOF_AXES");
    datasetGroup.addAttribute("NAXIS1");
    datasetGroup.addAttribute("NAXIS2");
    datasetGroup.setAttribute ("NOF_AXES", int(shape.size()) );
    datasetGroup.setAttribute ("NAXIS1",   int(shape[0])     );
    datasetGroup.setAttribute ("NAXIS2",   int(shape[1])     );
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[3] Creating 3D dataset ..." << endl;
  try {
    std::vector<hsize_t> shape (3);
    
    name     = "Data3D";
    shape[0] = 1024;
    shape[1] = 1024;
    shape[2] = 3;
    
    /* Create HDF5 Dataset at the root level of the file */
    DAL::HDF5Dataset dataset (fileID,
			      name,
			      shape);
    dataset.addAttribute("NOF_AXES");
    dataset.addAttribute("NAXIS1");
    dataset.addAttribute("NAXIS2");
    dataset.addAttribute("NAXIS3");
    dataset.summary();
    
    dataset.setAttribute ("NOF_AXES", int(shape.size()) );
    dataset.setAttribute ("NAXIS1",   int(shape[0]) );
    dataset.setAttribute ("NAXIS2",   int(shape[1]) );
    dataset.setAttribute ("NAXIS3",   int(shape[2]) );

    /* Create HDF5 dataset within group */
    DAL::HDF5Dataset datasetGroup (groupID,
				   name,
				   shape);
    datasetGroup.addAttribute("NOF_AXES");
    datasetGroup.addAttribute("NAXIS1");
    datasetGroup.addAttribute("NAXIS2");
    datasetGroup.addAttribute("NAXIS3");
    datasetGroup.setAttribute ("NOF_AXES", int(shape.size()) );
    datasetGroup.setAttribute ("NAXIS1",   int(shape[0]) );
    datasetGroup.setAttribute ("NAXIS2",   int(shape[1]) );
    datasetGroup.setAttribute ("NAXIS3",   int(shape[2]) );
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  cout << "[4] Open dataset attached to root group ..." << endl;
  try {
    DAL::HDF5Dataset dataset (fileID,
			      name);
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[5] Open dataset attached to sub-group ..." << endl;
  try {
    DAL::HDF5Dataset dataset (groupID,
			      name);
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[6] Open dataset attached to sub-group ..." << endl;
  try {
    // adjust the name
    name = "/" + groupname + "/" + name;
    DAL::HDF5Dataset dataset (fileID,
			      name);
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  //________________________________________________________
  // Close the file
  
  H5Fclose(fileID);
  H5Gclose (groupID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_array1d

/*!
  \brief Test application of hyperslab to write and read 1-dim array data

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_array1d (std::string const &filename="tHDF5Dataset.h5")
{
  cout << "\n[tHDF5Datatset::test_array1d]\n" << endl;

  int nofFailedTests (0);
  unsigned int nofSteps;
  unsigned int nofDatapoints;

  //________________________________________________________
  // Open the file to work with
  
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);

  if (!H5Iis_valid(fileID)) {
    std::cerr << "Failed to open file " << filename << endl;
    return 0;
  }

  //________________________________________________________
  // Open/Create dataset to work with

  std::string name ("Array1D");
  std::vector<hsize_t> shape (1,1024);

  DAL::HDF5Dataset dataset (fileID,
			    name,
			    shape);
  dataset.summary();

  std::vector<int> start;
  std::vector<int> stride;
  std::vector<int> count;
  std::vector<int> block;
  
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
    
    for (unsigned int step(0); step<nofSteps; ++step) {
      // set the starting position
      start[0] = step*shape[0]/nofSteps;
      // assign data values to be written to the dataset
      for (int n(0); n<block[0]*count[0]; ++n) {
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
  
  cout << "[2] Read data from 1D dataset ..." << endl;
  try {
    start.resize(shape.size());
    block.resize(shape.size());
    
    nofSteps      = 8;
    count[0]      = 1;
    block[0]      = shape[0]/nofSteps;
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];
    
    for (unsigned int step(0); step<nofSteps; ++step) {
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
  
  cout << "[3] Read data from 1D dataset ..." << endl;
  try {
    start.resize(shape.size());
    count.resize(shape.size());
    block.resize(shape.size());

    nofSteps      = 8;
    count[0]      = 2;
    block[0]      = shape[0]/(count[0]*nofSteps);
    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    double *data  = new double [nofDatapoints];

    for (unsigned int step(0); step<nofSteps; ++step) {
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
      for (int n(0); n<block[0]*count[0]; ++n) {
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

  //________________________________________________________
  // Close the file

  H5Fclose(fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_array2d

/*!
  \brief Test application of hyperslab to write and read 2-dim array data

  - Access data by column.
  - Access data by row.
  - Access data by 2D sub-array.

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_array2d (std::string const &filename="tHDF5Dataset.h5")
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
  // Open the file to work with
  
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);

  if (!H5Iis_valid(fileID)) {
    std::cerr << "Failed to open file " << filename << endl;
    return 0;
  }

  //________________________________________________________
  // Run the tests

  cout << "[1] Write data to 2D dataset by row ..." << endl;
  try {
    std::string name ("Array2D_rows");
    DAL::HDF5Dataset dataset (fileID,
			      name,
			      shape);
    
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
  
  //________________________________________________________
  // Close the file

  H5Fclose(fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                 test_hyperslab

int test_hyperslab (std::string const &filename="tHDF5Dataset.h5")
{
  cout << "\n[tHDF5Datatset::test_hyperslab]\n" << endl;

  int nofFailedTests (0);

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

    dataset.addAttributes (attributes);
    dataset.setAttribute ("start",start);
    dataset.setAttribute ("block",block);

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
    HDF5Hyperslab slab (shape,start,stride,count,block);

    dataset.addAttributes (attributes);
    dataset.setAttribute ("start",start);
    dataset.setAttribute ("stride",stride);
    dataset.setAttribute ("count",count);
    dataset.setAttribute ("block",block);

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
    HDF5Hyperslab slab (shape,start,stride,count,block);

    dataset.writeData (data,slab);

    delete [] data;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  //________________________________________________________
  // Close the file

  H5Gclose(groupID);
  H5Fclose(fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                 test_expansion

/*!
  \brief Test expansion of extendable datasets

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_expansion (std::string const &filename="tHDF5Dataset.h5")
{
  cout << "\n[tHDF5Datatset::test_expansion]\n" << endl;

  int nofFailedTests (0);
  unsigned int rank (2);
  unsigned int sidelength (1024);
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

  DAL::HDF5Dataset dataset (fileID, "ExtendableDataset", shape);
  dataset.summary();

  //________________________________________________________
  // Run the tests

  // Write first block of data, completely fitting into existing dataspace

  start[0] = sidelength/4;
  start[1] = sidelength/4;
  block[0] = sidelength/2;
  block[1] = sidelength/2;
  count[0] = 1;
  count[1] = 1;

  //________________________________________________________
  // Close the file

  H5Fclose(fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                               test_openDataset

/*!
  \brief Test opening an existing dataset

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_openDataset (std::string const &filename)
{
  cout << "\n[tHDF5Datatset::test_openDataset]\n" << endl;

  int nofFailedTests (0);
  std::string dataset;

  //________________________________________________________
  // Check if the HDF5 file contains a dataset; if yes open the file

  if (!find_dataset(filename,dataset)) {
    std::cerr << "-- Unable to find dataset within HDF5 file "
	      << filename
	      << std::endl;
    return 0;
  }

  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);

  //________________________________________________________
  // Run the tests
  
  std::cout << "[1]  Open dataset " << dataset << " ..." << std::endl;
  try {
    DAL::HDF5Dataset data (fileID,dataset);
    data.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }

  std::cout << "[2] Retrieving data ..." << std::endl;
  try {
    DAL::HDF5Dataset data (fileID,dataset);
    //
    unsigned int nofDatapoints;
    std::vector<hsize_t> shape = data.shape();
    std::vector<int> start (shape.size(),0);
    std::vector<int> count (shape.size(),1);
    std::vector<int> block (shape.size());

    for (unsigned int n(0); n<shape.size(); ++n) {
      block[n] = shape[n];
    }

    nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
    
    /* Feedback */
    std::cout << "-- Shape            = " << shape << std::endl;
    std::cout << "-- Count            = " << count << std::endl;
    std::cout << "-- Block            = " << block << std::endl;
    std::cout << "-- nof. data points = " << nofDatapoints << std::endl;
    
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  //________________________________________________________
  // Close the file

  H5Fclose(fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc,
          char *argv[])
{
  int nofFailedTests (0);
  bool haveDataset (true);
  std::string filename ("tHDF5Dataset.h5");

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

  // Test constructors for a HDF5Dataset object
  nofFailedTests += test_create ();
  // Test access R/W access to 1-dim data arrays
  nofFailedTests += test_array1d ();
  // Test access R/W access to 2-dim data arrays
  nofFailedTests += test_array2d ();
  // Test the effect of the various Hyperslab parameters
  nofFailedTests += test_hyperslab ();
  // Test expansion of extendable datasets
  nofFailedTests += test_expansion ();

  if (haveDataset) {
    nofFailedTests += test_openDataset (filename);
  }
  
  return nofFailedTests;
}
