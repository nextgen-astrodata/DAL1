/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include <core/HDF5Dataspace.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::HDF5Dataspace;

/*!
  \file tHDF5Dataspace.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::HDF5Dataspace class
 
  \author Lars B&auml;hren
 
  \date 2011/01/31
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new HDF5Dataspace object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &location)
{
  cout << "\n[tHDF5Dataspace::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing HDF5Dataspace() ..." << endl;
  try {
    HDF5Dataspace ds;
    //
    ds.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing HDF5Dataspace(hid_t) ..." << endl;
  try {
    HDF5Dataspace ds (location);
    //
    ds.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                          test_static_functions

/*!
  \brief Test static functions

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_functions (hid_t const &location)
{
  cout << "\n[tHDF5Dataspace::test_static_functions]\n" << endl;

  int nofFailedTests        = 0;
  bool status               = false;
  std::string attributeName = "AttributeVectorInt";
  hid_t attributeID         = 0;
  hid_t dataspaceID         = 0;
  
  /*________________________________________________________
    Open attribute and dataspace
  */

  if (H5Aexists(location,attributeName.c_str())) {
    // Try to open attribute
    attributeID = H5Aopen (location,
			   attributeName.c_str(),
			   H5P_DEFAULT);
    // Check if open was successful
    if (H5Iis_valid(attributeID)) {
      // Open Dataspace
      dataspaceID = H5Aget_space (attributeID);
      // Check if open was successful
      if (H5Iis_valid(dataspaceID)) {
	status = true;
      } else {
	std::cerr << "-- Failed to open dataspace!" << std::endl;
      }
    } else {
      std::cerr << "-- Failed to open attribute " << attributeName << std::endl;
    }
  } else {
    std::cerr << "-- Unable to find attribute " << attributeName << std::endl;
  }

  /*________________________________________________________
    Test static functions
  */

  if (status) {

    std::vector<hsize_t> dimensions;
    std::vector<hsize_t> maxDimensions;

    /* Check if data space is simple */

    std::cout << "-- Is dataspace simple (attr.)  = "
	      << HDF5Dataspace::isSimple(attributeID)
	      << std::endl;
    std::cout << "-- Is dataspace simple (space)  = "
	      << HDF5Dataspace::isSimple(dataspaceID)
	      << std::endl;

    /* Get dataspace dimensions */
    
    HDF5Dataspace::shape (attributeID,
			  dimensions,
			  maxDimensions);
    std::cout << "-- Dataspace shape (attr.)      = [";
    for (unsigned int n=0; n<dimensions.size(); ++n) {
      std::cout << " " << dimensions[n];
    }
    std::cout << " ]" << std::endl;
    
  }
  
  H5Sclose (dataspaceID);
  H5Aclose (attributeID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  std::string filename = "tHDF5Dataspace.h5";
  hid_t fileID         = 0;
  bool haveDataset     = false;

  //________________________________________________________
  // Process command line parameters

  if (argc>1) {
    filename    = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Open HDF5 file to work with
  
  if (haveDataset) {
    fileID = H5Fopen (filename.c_str(),
		      H5F_ACC_RDONLY,
		      H5P_DEFAULT);
  }
  
  //________________________________________________________
  // Run the tests
  
  if (H5Iis_valid(fileID)) {
    
    // Test for the constructor(s)
    nofFailedTests += test_constructors (fileID);
    
    // Test static functions
    nofFailedTests += test_static_functions (fileID);

  }

  //________________________________________________________
  // Release HDF5 objects
  
  if (H5Iis_valid(fileID)) {
    H5Fclose(fileID);
  }
  
  return nofFailedTests;
}
