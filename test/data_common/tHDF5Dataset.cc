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

//_______________________________________________________________________________
//                                                                    test_create

/*!
  \brief Test the creation of a Dataset object within a file

  The generated HDF5 file will have the following structure:
  \verbatim
  /
  |-- Array1D            ...  Dataset
  |-- Array1D            ...  Dataset
  |-- Array1D            ...  Dataset
  `-- Group              ...  Group
      |-- Array1D        ...  Dataset
      |-- Array1D        ...  Dataset
      `-- Array1D        ...  Dataset
  \endverbatim

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_create ()
{
  std::cout << "\n[tHDF5Datatset::test_create]\n" << std::endl;

  int nofFailedTests (0);
  std::string name;
  std::string filename ("tHDF5Dataset.h5");
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
  
  std::cout << "[1] Creating 1D dataset ..." << std::endl;
  try {
    std::vector<hsize_t> shape (1);
    
    name     = "Data1D";
    shape[0] = 1024;
    
    /* Create HDF5 Dataset at the root level of the file */
    DAL::HDF5Dataset dataset (fileID,
			      name,
			      shape);
    dataset.addAttribute("NOF_AXES");
    dataset.addAttribute("NAXIS1");
    dataset.summary();
    
    dataset.setAttribute ("NOF_AXES", int(shape.size()) );
    dataset.setAttribute ("NAXIS1",   int(shape[0])     );

    /* Create HDF5 dataset within group */
    DAL::HDF5Dataset datasetGroup1 (groupID,
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
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  std::cout << "[2] Creating 2D dataset ..." << std::endl;
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
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  std::cout << "[3] Creating 3D dataset ..." << std::endl;
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
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }

  cout << "[4] Open dataset attached to root group ..." << endl;
  try {
    DAL::HDF5Dataset dataset (fileID,
			      name);
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  cout << "[5] Open dataset attached to sub-group ..." << endl;
  try {
    DAL::HDF5Dataset dataset (groupID,
			      name);
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
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
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  //________________________________________________________
  // Close the file
  
  H5Fclose(fileID);
  H5Gclose (groupID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                 test_hyperslab

/*!
  \brief Test selecting a hyperslab for the dataspace attached to the dataset. 

  \return nofFailedTests -- The number of failed tests encountered within this
          functions.
*/
int test_hyperslab ()
{
  std::cout << "\n[tHDF5Datatset::test_hyperslab]\n" << std::endl;

  int nofFailedTests (0);
  std::string filename ("tHDF5Dataset.h5");
  std::string groupname ("Group");
  std::string name;
  std::vector<hsize_t> shape;
  std::vector<int> start;
  std::vector<int> stride;
  std::vector<int> count;
  std::vector<int> block;

  //________________________________________________________
  // Open the file to work with

  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  hid_t groupID = H5Gopen (fileID,
			   groupname.c_str(),
			   H5P_DEFAULT);

  //________________________________________________________
  // Run the tests

  std::cout << "[1] Write data to 1D dataset in root group ..." << std::endl;
  try {
    name = "Data1D";
    unsigned int nofSteps (16);
    
    // Open the HDF5 Dataset
    DAL::HDF5Dataset dataset (fileID,
			      name);
    dataset.summary();

    // Set the parameters for the Hyperslab
    shape = dataset.shape();

    start.resize(shape.size());
    block.resize(shape.size());

    block[0]     = shape[0]/nofSteps;
    double *data = new double [block[0]];

    for (unsigned int step(0); step<nofSteps; ++step) {
      // set the starting position
      start[0] = step*shape[0]/nofSteps;
      // assign data values to be written to the dataset
      for (int n(0); n<block[0]; ++n) {
	data[n] = step+1;
      }
      // summary
      std::cout << "-- Hyperslab " << step << " :"
		<< "\tStart = " << start
		<< "\tBlock = " << block
		<< std::endl;
      // Write the data
      dataset.writeData (data,start,block);
    }

    // release allocated memory
    delete [] data;
    
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  std::cout << "[2] Write data to 1D dataset in subgroup ..." << std::endl;
  try {
    name = "Data1D";
    unsigned int nofSteps (16);
    
    // Open the HDF5 Dataset
    DAL::HDF5Dataset dataset (groupID,
			      name);
    dataset.summary();

    // Set the parameters for the Hyperslab
    shape = dataset.shape();

    start.resize(shape.size());
    block.resize(shape.size());

    block[0]     = shape[0]/nofSteps;
    double *data = new double [block[0]];

    for (unsigned int step(0); step<nofSteps; ++step) {
      // set the starting position
      start[0] = step*shape[0]/nofSteps;
      // assign data values to be written to the dataset
      for (int n(0); n<block[0]; ++n) {
	data[n] = step+1;
      }
      // summary
      std::cout << "-- Hyperslab " << step << " :"
		<< "\tStart = " << start
		<< "\tBlock = " << block
		<< std::endl;
      // Write the data
      dataset.writeData (data,start,block);
    }

    // release allocated memory
    delete [] data;
    
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  std::cout << "[3] Read data from 1D dataset ..." << std::endl;
  try {
    name = "Data1D";
    unsigned int nofSteps (8);
    
    // Open the HDF5 Dataset
    DAL::HDF5Dataset dataset (fileID,
			      name);
    dataset.summary();

    // Set the parameters for the Hyperslab
    shape = dataset.shape();

    start.resize(shape.size());
    block.resize(shape.size());

    block[0]     = shape[0]/nofSteps;
    double *data = new double [block[0]];

    for (unsigned int step(0); step<nofSteps; ++step) {
      // set the starting position
      start[0] = step*shape[0]/nofSteps;
      // summary
      std::cout << "-- Hyperslab " << step << " :"
		<< "\tStart = " << start
		<< "\tBlock = " << block
		<< std::endl;
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
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  //________________________________________________________
  // Close the file

  H5Fclose(fileID);
  H5Gclose(groupID);
  
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

  nofFailedTests += test_create ();
  nofFailedTests += test_hyperslab ();
  
  return nofFailedTests;
}
