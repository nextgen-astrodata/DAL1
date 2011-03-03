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
#include <core/HDF5Attribute.h>

// Namespace usage
using std::cerr;
using std::cout;
using std::endl;
using DAL::HDF5Attribute;
using DAL::HDF5Object;

/*!
  \file tHDF5Attribute.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::HDF5Attribute class
 
  \author Lars B&auml;hren
 
  \date 2011/01/27
*/

// ==============================================================================
//
//  Test routines
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                   test_H5Aread

int test_H5Aread (hid_t const &location)
{
  std::cout << "\n[tHDF5Attribute::test_H5Aread]\n" << endl;
  
  int nofFailedTests = 0;
  hid_t attributeID  = 0;
  htri_t h5err       = 0;
  std::string name   = "AttributeVectorInt";

  /*________________________________________________________
    Check if attribute of given 'name' is attached to object
  */
  h5err = H5Aexists (location, name.c_str());

  if (h5err) {
    std::cout << "-- Found attribute " << name << std::endl;
  } else {
    std::cout << "-- Unable to find attribute " << name << " !" << std::endl;
    return nofFailedTests;
  }

  attributeID = H5Aopen (location, name.c_str(), H5P_DEFAULT);

  /*________________________________________________________
    Retrieve properties of the attribute
  */
  
  hsize_t storageSize = H5Aget_storage_size (attributeID);
  hid_t dataspaceID   = H5Aget_space (attributeID);
  hid_t datatypeID    = H5Aget_type(attributeID);
  htri_t isSimple     = H5Sis_simple (dataspaceID);

  std::cout << "-- Attribute name     = " << name        << std::endl;
  std::cout << "-- Attribute ID       = " << attributeID << std::endl;
  std::cout << "-- Dataspace ID       = " << dataspaceID << std::endl;
  std::cout << "-- Datatype ID        = " << datatypeID  << std::endl;
  std::cout << "-- Storage size       = " << storageSize << std::endl;
  std::cout << "-- Dataspace is simple? " << isSimple    << std::endl;
  
  /*________________________________________________________
    Release HDF5 object identifiers
  */
  
  H5Sclose (dataspaceID);
  H5Tclose (datatypeID);
  H5Aclose (attributeID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                          test_static_functions

/*!
  \brief Test static functions provided by HDF5Attribute

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_functions (hid_t const &location)
{
  std::cout << "\n[tHDF5Attribute::test_static_functions]\n" << endl;

  int nofFailedTests = 0;
  
  /*__________________________________________________________________
    Test 1: Create/Set attribute storing atomic value.
  */
  
  cout << "[1] Testing setAttribute(hid_t,string,T) ..." << endl;
  try {
    int valInt            = 1;
    int valShort          = 2;
    int valLong           = 3;
    float valFloat        = 0.5;
    float valDouble       = 0.25;
    std::string valString = "bla";
    
    HDF5Attribute::setAttribute (location, "AttributeInt",    valInt);
    HDF5Attribute::setAttribute (location, "AttributeShort",  valShort);
    HDF5Attribute::setAttribute (location, "AttributeLong",   valLong);
    HDF5Attribute::setAttribute (location, "AttributeFloat",  valFloat);
    HDF5Attribute::setAttribute (location, "AttributeDouble", valDouble);
    HDF5Attribute::setAttribute (location, "AttributeString", valString);
  } catch (std::string message) {
    ++nofFailedTests;
  }
    
  /*__________________________________________________________________
    Test 2: Create/Set attribute storing array-type value, using
            interface for std::vector<T>
  */

  cout << "[2] Testing setAttribute(hid_t,string,vector<T>) ..." << endl;
  try {
    unsigned int nelem = 3;
    std::vector<int> valInt (nelem, 1);
    std::vector<short> valShort (nelem, 2);
    std::vector<long> valLong (nelem, 3);
    std::vector<float> valFloat (nelem, 0.5);
    std::vector<double> valDouble (nelem, 0.25);
    std::vector<std::string> valString (nelem, "bla");
    
    HDF5Attribute::setAttribute (location, "AttributeVectorInt",    valInt);
    HDF5Attribute::setAttribute (location, "AttributeVectorShort",  valShort);
    HDF5Attribute::setAttribute (location, "AttributeVectorLong",   valLong);
    HDF5Attribute::setAttribute (location, "AttributeVectorFloat",  valFloat);
    HDF5Attribute::setAttribute (location, "AttributeVectorDouble", valDouble);
    HDF5Attribute::setAttribute (location, "AttributeVectorString", valString);
  } catch (std::string message) {
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 3: Create/Set attribute storing array-type value
  */
  
  cout << "[3] Testing setAttribute(hid_t,string,T*,uint) ..." << endl;
  try {
    unsigned int nelem      = 5;
    int valInt[]            = {1,1,1,1,1};
    int valShort[]          = {2,2,2,2,2};
    int valLong[]           = {3,3,3,3,3};
    float valFloat[]        = {0.5,0.5,0.5,0.5,0.5};
    double valDouble[]      = {0.25,0.25,0.25,0.25,0.25};
    std::string valString[] = {"a","bb","ccc","dddd","eeeee"};

    HDF5Attribute::setAttribute (location, "AttributeArrayInt",    valInt,    nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayShort",  valShort,  nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayLong",   valLong,   nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayFloat",  valFloat,  nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayDouble", valDouble, nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayString", valString, nelem);
  } catch (std::string message) {
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 4: Set/Update attributes
  */

  cout << "[4] Updating attributes via setAttribute(hid_t,string,T) ..." << endl;
  try {
    int valInt            = 10;
    int valShort          = 20;
    int valLong           = 30;
    float valFloat        = 0.05;
    float valDouble       = 0.025;
    std::string valString = "bladibla";
    
    HDF5Attribute::setAttribute (location, "AttributeInt",    valInt);
    HDF5Attribute::setAttribute (location, "AttributeShort",  valShort);
    HDF5Attribute::setAttribute (location, "AttributeLong",   valLong);
    HDF5Attribute::setAttribute (location, "AttributeFloat",  valFloat);
    HDF5Attribute::setAttribute (location, "AttributeDouble", valDouble);
    HDF5Attribute::setAttribute (location, "AttributeString", valString);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 5: Set/Update attributes
  */

  cout << "[5] Updating attributes via setAttribute(hid_t,string,vector<T>) ..." << endl;
  try {
    unsigned int nelem = 3;
    std::vector<int> valInt (nelem, 10);
    std::vector<short> valShort (nelem, 20);
    std::vector<long> valLong (nelem, 30);
    std::vector<float> valFloat (nelem, 0.05);
    std::vector<double> valDouble (nelem, 0.025);
    std::vector<std::string> valString (nelem, "bladibla");
    
    HDF5Attribute::setAttribute (location, "AttributeVectorInt",    valInt);
    HDF5Attribute::setAttribute (location, "AttributeVectorShort",  valShort);
    HDF5Attribute::setAttribute (location, "AttributeVectorLong",   valLong);
    HDF5Attribute::setAttribute (location, "AttributeVectorFloat",  valFloat);
    HDF5Attribute::setAttribute (location, "AttributeVectorDouble", valDouble);
    HDF5Attribute::setAttribute (location, "AttributeVectorString", valString);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 6: Set/Update attributes
  */

  cout << "[6] Updating attributes via setAttribute(hid_t,string,T*,uint) ..." << endl;
  try {
    unsigned int nelem      = 5;
    int valInt[]            = {10,10,10,10,10};
    int valShort[]          = {20,20,20,20,20};
    int valLong[]           = {30,30,30,30,30};
    float valFloat[]        = {0.05,0.05,0.05,0.05,0.05};
    double valDouble[]      = {0.025,0.025,0.025,0.025,0.025};
    std::string valString[] = {"A","BB","CCC","DDDD","EEEEE"};

    HDF5Attribute::setAttribute (location, "AttributeArrayInt",    valInt,    nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayShort",  valShort,  nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayLong",   valLong,   nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayFloat",  valFloat,  nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayDouble", valDouble, nelem);
    HDF5Attribute::setAttribute (location, "AttributeArrayString", valString, nelem);
  } catch (std::string message) {
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 7: Get number, names and types of attributes
  */
  
  cout << "[7] Get number, names and types of attributes ..." << endl;
  try {
    cout << "-- nof attributes  = " << HDF5Object::nofAttributes(location) << endl;
    cout << "-- Attribute names = " << HDF5Attribute::attributes(location) << endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 8: Retrieve previously set attributes.
  */

  cout << "[8] Testing getAttribute(hid_t,string,vector<T>) ..." << endl;
  try {
    std::vector<int> valInt;
    std::vector<short> valShort;
    std::vector<long> valLong;
    // float valFloat        = 0.0;
    // float valDouble       = 0.0;
    // std::string valString = "UNDEFINED";
    
    HDF5Attribute::getAttribute (location, "AttributeArrayInt", valInt);
    // HDF5Attribute::setAttribute (location, "AttributeShort",  valShort);
    // HDF5Attribute::setAttribute (location, "AttributeLong",   valLong);
    // HDF5Attribute::setAttribute (location, "AttributeFloat",  valFloat);
    // HDF5Attribute::setAttribute (location, "AttributeDouble", valDouble);
    // HDF5Attribute::setAttribute (location, "AttributeString", valString);

    // std::cout << "-- AttributeInt    = " << valInt     << std::endl;
    // std::cout << "-- AttributeShort  = " << valShort   << std::endl;
    // std::cout << "-- AttributeLong   = " << valLong    << std::endl;
    // std::cout << "-- AttributeFloat  = " << valFloat   << std::endl;
    // std::cout << "-- AttributeDouble = " << valDouble  << std::endl;

  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 10: Create / Set attributes from casa::Vector<T>
  */

#ifdef DAL_WITH_CASA
  cout << "[10] Testing setAttribute(hid_t,string,casa::Vector<T>) ..." << endl;
  try {
    unsigned int nelem (10);
    casa::Vector<int> valInt (nelem, 1);
    casa::Vector<short> valShort (nelem, 2);
    casa::Vector<long> valLong (nelem, 3);
    casa::Vector<float> valFloat (nelem, 0.5);
    casa::Vector<double> valDouble (nelem, 0.25);
    casa::Vector<std::string> valString (nelem, "bla");

    HDF5Attribute::setAttribute (location, "AttributeCASAInt",    valInt);
    HDF5Attribute::setAttribute (location, "AttributeCASAShort",  valShort);
    HDF5Attribute::setAttribute (location, "AttributeCASALong",   valLong);
    HDF5Attribute::setAttribute (location, "AttributeCASAFloat",  valFloat);
    HDF5Attribute::setAttribute (location, "AttributeCASADouble", valDouble);
    HDF5Attribute::setAttribute (location, "AttributeCASAString", valString);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
#endif

  return nofFailedTests;
}
  
//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new HDF5Attribute object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &location)
{
  std::cout << "\n[tHDF5Attribute::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "-- nof. attributes = " << HDF5Object::nofAttributes(location) << endl;

  /*__________________________________________________________________
    Test 1: Test for the default constructor
  */

  std::cout << "[1] Testing HDF5Attribute() ..." << endl;
  try {
    HDF5Attribute attr;
    //
    attr.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*__________________________________________________________________
    Test 2: Testing argumented constructor
  */

  std::cout << "[2] Testing HDF5Attribute(hid_t,string) ..." << endl;
  try {
    HDF5Attribute attrInt (location, "AttributeInt");
    attrInt.summary(); 
    //
    HDF5Attribute attrFloat (location, "AttributeFloat");
    attrFloat.summary(); 
    //
    HDF5Attribute attrDouble (location, "AttributeDouble");
    attrDouble.summary(); 
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
  hid_t fileID         = 0;
  std::string filename = "tHDF5Attribute.h5";
  bool haveDataset     = false;

  //________________________________________________________
  // Process command line parameters

  if (argc>1) {
    filename    = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Open/create HDF5 file to work with
  
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
  
  //________________________________________________________
  // Run the tests
  
  if (H5Iis_valid(fileID)) {

    // Test for static methods
    nofFailedTests += test_static_functions (fileID);

    // Test usage of basic HDF5 library functions
    nofFailedTests += test_H5Aread (fileID);

    // Test for the constructor(s)
    nofFailedTests += test_constructors (fileID);
    
  } else {
    std::cerr << "[tHDF5Attribute] Failed to open file " << filename << endl;
    return -1;
  }
    
  //________________________________________________________
  // Release HDF5 objects
  
  if (H5Iis_valid(fileID)) {
    H5Fclose(fileID);
  }
  
  return nofFailedTests;
}
