/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <HDF5Object.h>

// Namespace usage
using DAL::HDF5Object;
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

  int nofFailedTests      = 0;
  std::string nameGroup   = "GROUP";
  std::string nameDataset = "DATASET_2D_NATIVE_INT";
  
  cout << "[1] Testing HDF5Object() ..." << endl;
  try {
    HDF5Object newObject;
    //
    newObject.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing HDF5Object(string) ..." << endl;
  try {
    HDF5Object h5file (filename);
    //
    h5file.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing HDF5Object(hid_t,string) ..." << endl;
  try {
    cout << "--> opening group " << nameGroup << " ..." << endl;
    HDF5Object h5group (fileID, nameGroup);
    h5group.summary(); 
    //
    cout << "--> opening dataset " << nameDataset << " ..." << endl;
    HDF5Object h5dataset (fileID, nameDataset);
    h5dataset.summary(); 
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
  cout << "\n[tHDF5Object::test_staticMethods]\n" << endl;
  
  int nofFailedTests (0);
  
  cout << "[1] Get general object type information ..." << endl;
  try {
    std::map<H5I_type_t,std::string> objectsMap = HDF5Object::objectTypesMap();
    std::vector<H5I_type_t> objectTypes         = HDF5Object::objectTypes();
    std::vector<std::string> objectNames        = HDF5Object::objectNames();
    
    cout << "-- nof object types = " << objectsMap.size() << endl;
    cout << "-- Object types     = " << objectTypes << endl;
    cout << "-- Object names     = " << objectNames << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Retrieve object information ..." << endl;
  try {
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
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] Test generic open/close functions with existing group ..." << endl;
  try {
    std::string groupName   = "GROUP";

    hid_t id = HDF5Object::open (fileID, groupName);

    if (id) {
      cout << "-- Name            = " << HDF5Object::name(id)          << endl;
      cout << "-- Object type     = " << HDF5Object::objectType(id)    << endl;
      cout << "-- Object name     = " << HDF5Object::objectName(id)    << endl;
      cout << "-- Object address  = " << HDF5Object::objectAddress(id) << endl;
      cout << "-- nof. attributes = " << HDF5Object::nofAttributes(id) << endl;
    } else {
      std::cerr << "-- Failed to open group " << groupName << endl;
    }
    
    HDF5Object::close (id);
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] Test generic open/close functions with missing group ..." << endl;
  try {
    std::string groupName   = "GROUP2";

    hid_t id = HDF5Object::open (fileID, groupName);

    if (id) {
      cout << "-- Name            = " << HDF5Object::name(id)          << endl;
      cout << "-- Object type     = " << HDF5Object::objectType(id)    << endl;
      cout << "-- Object name     = " << HDF5Object::objectName(id)    << endl;
      cout << "-- Object address  = " << HDF5Object::objectAddress(id) << endl;
      cout << "-- nof. attributes = " << HDF5Object::nofAttributes(id) << endl;
    } else {
      std::cerr << "-- Failed to open group " << groupName << endl;
    }
    
    HDF5Object::close (id);
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[5] Test generic open/close functions with existing dataset ..." << endl;
  try {
    std::string datasetName = "DATASET_2D_NATIVE_INT";

    hid_t id = HDF5Object::open (fileID, datasetName);

    cout << "-- Name            = " << HDF5Object::name(id)          << endl;
    cout << "-- Object type     = " << HDF5Object::objectType(id)    << endl;
    cout << "-- Object name     = " << HDF5Object::objectName(id)    << endl;
    cout << "-- Object address  = " << HDF5Object::objectAddress(id) << endl;
    cout << "-- nof. attributes = " << HDF5Object::nofAttributes(id) << endl;

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
  int nofFailedTests = 0;
  std::string filename;

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
