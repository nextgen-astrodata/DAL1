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

#include <core/dalCommon.h>
#include <core/HDF5Object.h>

// Namespace usage
using DAL::HDF5Object;
using std::cerr;
using std::cout;
using std::endl;

/*!
  \file tHDF5Object.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::HDF5Object class
 
  \author Lars B&auml;hren
 
  \date 2011/01/17

  Structure of the HDF5 test file:
  \verbatim
  HDF5Testfile.h5                       ...  File
  |-- ATTRIBUTE_SINGLE_BYTE             ...  Attribute
  |-- ATTRIBUTE_SINGLE_SHORT            ...  Attribute
  |-- ATTRIBUTE_SINGLE_INT              ...  Attribute
  |-- ATTRIBUTE_SINGLE_UBYTE            ...  Attribute
  |-- ATTRIBUTE_SINGLE_USHORT           ...  Attribute
  |-- ATTRIBUTE_SINGLE_UINT             ...  Attribute
  |-- DATASET_2D_NATIVE_INT             ...  Dataset
  |-- DATASET_2D_NATIVE_FLOAT           ...  Dataset
  `-- GROUP                             ...  Group
      |-- ATTRIBUTE_SINGLE_BYTE         ...  Attribute
      |-- ATTRIBUTE_SINGLE_SHORT        ...  Attribute
      |-- ATTRIBUTE_SINGLE_INT          ...  Attribute
      |-- ATTRIBUTE_SINGLE_UBYTE        ...  Attribute
      |-- ATTRIBUTE_SINGLE_USHORT       ...  Attribute
      `-- ATTRIBUTE_SINGLE_UINT         ...  Attribute
  \endverbatim

*/

//_______________________________________________________________________________
//                                                                      test_hdf5

/*!
  \brief Test underlying HDF5 functions being wrapped

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_hdf5 (hid_t const &location)
{
  cout << "\n[tHDF5Object::test_hdf5]\n" << endl;
  
  int nofFailedTests = 0;
  std::string nameDataset   = "DATASET_2D_NATIVE_INT";
  std::string nameAttribute = "ATTRIBUTE_SINGLE_INT";

  /*__________________________________________________________________
    Test 1: Testing H5Lexists(hid_t,string) on existing group
  */
  
  cout << "[1] Testing H5Lexists(hid_t,string) ..." << endl;
  {
    std::string name = "GROUP";
    htri_t status    = H5Lexists (location, name.c_str(), H5P_DEFAULT);
    //
    std::cout << "-- location  = " << location  << std::endl;
    std::cout << "-- name      = " << name      << std::endl;
    std::cout << "-- H5Lexists = " << status    << std::endl;
  }
  
  /*__________________________________________________________________
    Test 2: Testing H5Lexists(hid_t,string) on missing group
  */
  
  cout << "[2] Testing H5Lexists(hid_t,string) on missing group ..." << endl;
  {
    std::string name = "MISSING_GROUP";
    htri_t status    = H5Lexists (location, name.c_str(), H5P_DEFAULT);
    //
    std::cout << "-- location  = " << location  << std::endl;
    std::cout << "-- name      = " << name      << std::endl;
    std::cout << "-- H5Lexists = " << status    << std::endl;
  }
  
  /*__________________________________________________________________
    Test 3: Testing H5Iget_type(hid_t) on existing object
  */
  
  cout << "[3] Testing H5Iget_type(hid_t) ..." << endl;
  {
    H5I_type_t status = H5Iget_type (location);
    //
    std::cout << "-- location    = " << location  << std::endl;
    std::cout << "-- H5Iget_type = " << status    << std::endl;
  }
  
  /*__________________________________________________________________
    Test 4: Testing H5Iget_type(hid_t) on invalid object
  */
  
  cout << "[4] Testing H5Iget_type(hid_t) on invalid object ..." << endl;
  {
    hid_t testID      = 0;
    H5I_type_t status = H5Iget_type (testID);
    //
    std::cout << "-- location    = " << testID    << std::endl;
    std::cout << "-- H5Iget_type = " << status    << std::endl;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new DAL::HDF5Object object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename,
		       hid_t const &fileID)
{
  cout << "\n[tHDF5Object::test_constructors]\n" << endl;

  int nofFailedTests        = 0;
  std::string nameGroup     = "GROUP";
  std::string nameDataset   = "DATASET_2D_NATIVE_INT";
  std::string nameAttribute = "ATTRIBUTE_SINGLE_INT";
  
  /*__________________________________________________________________
    Test 1: Default constructor - created object not connected to HDF5
            object.
  */
  
  cout << "[1] Testing HDF5Object() ..." << endl;
  try {
    HDF5Object newObject;
    //
    newObject.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*__________________________________________________________________
    Test 2: Open HDF5 file.
  */
  
  cout << "[2] Testing HDF5Object(string) ..." << endl;
  try {
    HDF5Object h5file (filename);
    //
    h5file.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*__________________________________________________________________
    Test 3: Open objects - group, dataset, attribute - within file.
            Also try opening non-existing object. 
  */
  
  cout << "[3] Testing HDF5Object(hid_t,string) ..." << endl;
  try {
    cout << "--> opening group " << nameGroup << " ..." << endl;
    HDF5Object h5group (fileID, nameGroup);
    h5group.summary(); 
    //
    cout << "--> opening dataset " << nameDataset << " ..." << endl;
    HDF5Object h5dataset (fileID, nameDataset);
    h5dataset.summary(); 
    //
    cout << "--> opening attribute " << nameAttribute << " ..." << endl;
    HDF5Object h5attribute (fileID, nameAttribute);
    h5attribute.summary();
    //
    std::string attr ("MISSING_GROUP");
    cout << "--> try opening attribute " << attr << " ..." << endl;
    HDF5Object h5attr (fileID, attr);
    h5attr.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*__________________________________________________________________
    Test 4: Copy constructor
  */
  
  cout << "[4] Testing HDF5Object(HDF5Object) ..." << endl;
  try {
    std::cout << "--> creating object 1 ..." << std::endl;
    HDF5Object group1 (fileID, nameGroup);
    group1.summary();
    //
    std::cout << "--> creating object 2 ..." << std::endl;
    HDF5Object group2 (group1);
    group2.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                             test_staticMethods

/*!
  \brief Test the various static methods provided by the DAL::HDF5Object class

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_staticMethods (std::string const &filename,
			hid_t const &fileID)
{
  std::cout << "\n[tHDF5Object::test_staticMethods]\n" << std::endl;
  
  int nofFailedTests = 0;
  std::string name   = filename;
  
  /*__________________________________________________________________
    Test 1: Obtain general type information, which does not require
            interaction with an actual HDF5 object.
  */
  
  std::cout << "[1] Get general object type information ..." << std::endl;
  try {
    std::map<H5I_type_t,std::string> objectsMap = HDF5Object::objectTypesMap();
    std::vector<H5I_type_t> objectTypes         = HDF5Object::objectTypes();
    std::vector<std::string> objectNames        = HDF5Object::objectNames();
    std::map<H5I_type_t,std::string>::iterator it;
    
    std::cout << "-- nof object types = " << objectsMap.size() << std::endl;
    
    for (it=objectsMap.begin(); it!=objectsMap.end(); ++it) {
      cout << " [ " << it->first << "  ->  " << it->second << " ]" << endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  /*__________________________________________________________________
    Test 2: Retrieve information on an exiting valid HDF5 object
  */
  
  cout << "[2] Retrieve information on valid object ID ..." << endl;
  {
    cout << "-- Name              = " << HDF5Object::name(fileID)             << endl;
    cout << "-- Object type       = " << HDF5Object::objectType(fileID)       << endl;
    cout << "-- Object name       = " << HDF5Object::objectName(fileID)       << endl;
    cout << "-- Object address    = " << HDF5Object::objectAddress(fileID)    << endl;
    cout << "-- Reference count   = " << HDF5Object::referenceCount(fileID)   << endl;
    cout << "-- Access time       = " << HDF5Object::accessTime(fileID)       << endl;
    cout << "-- Modification time = " << HDF5Object::modificationTime(fileID) << endl;
    cout << "-- Change time       = " << HDF5Object::changeTime(fileID)       << endl;
    cout << "-- Birth time        = " << HDF5Object::birthTime(fileID)        << endl;
    cout << "-- nof. attributes   = " << HDF5Object::nofAttributes(fileID)    << endl;
  }

  /*__________________________________________________________________
    Test 3: Attempt retrieving information from an invalid object
            identifier - all static functions need to be able to exit
	    gracefully under such circumstances.
  */
  
  std::cout << "[3] Retrieve information from an invalid object ID ..." << std::endl;
  {
    hid_t id = 0;

    cout << "-- Name              = " << HDF5Object::name(id)             << endl;
    cout << "-- Object type       = " << HDF5Object::objectType(id)       << endl;
    cout << "-- Object name       = " << HDF5Object::objectName(id)       << endl;
    cout << "-- Object address    = " << HDF5Object::objectAddress(id)    << endl;
    cout << "-- Reference count   = " << HDF5Object::referenceCount(id)   << endl;
    cout << "-- Access time       = " << HDF5Object::accessTime(id)       << endl;
    cout << "-- Modification time = " << HDF5Object::modificationTime(id) << endl;
    cout << "-- Change time       = " << HDF5Object::changeTime(id)       << endl;
    cout << "-- Birth time        = " << HDF5Object::birthTime(id)        << endl;
    cout << "-- nof. attributes   = " << HDF5Object::nofAttributes(id)    << endl;
  }

  /*__________________________________________________________________
    Test 4: Test the generic open(hid_t,string) function on an 
            existing group inside the test file.
  */
  
  cout << "[4] Test generic open/close functions with existing group ..." << endl;
  {
    name     = "GROUP";
    hid_t id = HDF5Object::open (0, name);

    if (H5Iis_valid(id)) {
      cout << "-- Name            = " << HDF5Object::name(id)          << endl;
      cout << "-- Object type     = " << HDF5Object::objectType(id)    << endl;
      cout << "-- Object name     = " << HDF5Object::objectName(id)    << endl;
      cout << "-- Object address  = " << HDF5Object::objectAddress(id) << endl;
      cout << "-- nof. attributes = " << HDF5Object::nofAttributes(id) << endl;
    } else {
      std::cerr << "-- Failed to open group " << name << endl;
    }
    
    HDF5Object::close (id);
  }
  
  /*__________________________________________________________________
    Test 5: 
  */
  
  std::cout << "[5] Test generic open/close functions with missing group ..."
	    << std::endl;
  {
    name     = "MISSING_GROUP";
    hid_t id = HDF5Object::open (fileID, name);
    
    if (H5Iis_valid(id)) {
      std::cout << "-- Name            = " << HDF5Object::name(id)          << endl;
      std::cout << "-- Object type     = " << HDF5Object::objectType(id)    << endl;
      std::cout << "-- Object name     = " << HDF5Object::objectName(id)    << endl;
      std::cout << "-- Object address  = " << HDF5Object::objectAddress(id) << endl;
      std::cout << "-- nof. attributes = " << HDF5Object::nofAttributes(id) << endl;
    } else {
      std::cerr << "-- Failed to open group " << name << endl;
    }
    
    HDF5Object::close (id);
  }

  /*__________________________________________________________________
    Test 6: 
  */
  
  cout << "[6] Test generic open/close functions with existing dataset ..." << endl;
  try {
    std::string datasetName = "DATASET_2D_NATIVE_INT";

    hid_t id = HDF5Object::open (fileID, datasetName);

    if (H5Iis_valid(id)) {
      cout << "-- Name            = " << HDF5Object::name(id)          << endl;
      cout << "-- Object type     = " << HDF5Object::objectType(id)    << endl;
      cout << "-- Object name     = " << HDF5Object::objectName(id)    << endl;
      cout << "-- Object address  = " << HDF5Object::objectAddress(id) << endl;
      cout << "-- nof. attributes = " << HDF5Object::nofAttributes(id) << endl;
    } else {
      std::cerr << "-- Failed to open dataset " << datasetName << endl;
    }
    
    HDF5Object::close (id);
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
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
  std::string filename = "tHDF5Object.h5";

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    std::cerr << "-- ERROR: No test file specified!" << std::endl;
    return -1;
  } else {
    filename = argv[1];
  }

  //________________________________________________________
  // Open HDF5 file to work with
  
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  //________________________________________________________
  // Run the tests
  
  if (H5Iis_valid(fileID)) {

    cout << "[tHDF5Object] Successfully opened " << filename << std::endl;

    // Test underlying HDF5 functions being wrapped
    nofFailedTests += test_hdf5 (fileID);
    
    // Test for the constructor(s)
    nofFailedTests += test_constructors (filename, fileID);
    // Test of static methods
    nofFailedTests += test_staticMethods (filename, fileID);

  } else {
    std::cerr << "[tHDF5Object] Failed to open file " << filename << std::endl;
    return -1;
  }
  
  //________________________________________________________
  // close HDF5 file
  
  if (H5Iis_valid(fileID)) {
    H5Fclose(fileID);
  }
  
  return nofFailedTests;
}
