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

#include <HDF5Attribute.h>

// Namespace usage
using std::cerr;
using std::cout;
using std::endl;
using DAL::HDF5Attribute;

/*!
  \file tHDF5Attribute.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::HDF5Attribute class
 
  \author Lars B&auml;hren
 
  \date 2011/01/27
*/

//_______________________________________________________________________________
//                                                          test_static_functions

/*!
  \brief Test static functions provided by HDF5Attribute

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_functions (hid_t const &location)
{
  std::cout << "\n[tHDF5Attribute::test_static_functions]\n" << std::endl;

  int nofFailedTests = 0;
  std::string name   = "Attribute";

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
    
  cout << "[2] Testing setAttribute(hid_t,string,vector<T>) ..." << endl;
  try {
    unsigned int nelem (5);
    std::vector<int> valInt (nelem, 1);
    std::vector<short> valShort (nelem, 2);
    std::vector<long> valLong (nelem, 2);
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
  } catch (std::string message) {
    ++nofFailedTests;
  }

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
  std::cout << "\n[tHDF5Attribute::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    HDF5Attribute newObject;
    //
    newObject.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
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
int main ()
{
  int nofFailedTests   = 0;
  std::string filename = "tHDF5Attribute.h5";

  //________________________________________________________
  // Open/create HDF5 file to work with
  
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  
  //________________________________________________________
  // Run the tests
  
  if (H5Iis_valid(fileID)) {

    cout << "[tHDF5Attribute] Successfully opened " << filename << std::endl;
    
    // Test for static methods
    nofFailedTests += test_static_functions (fileID);
    // Test for the constructor(s)
    nofFailedTests += test_constructors (fileID);

  } else {
    std::cerr << "[tHDF5Attribute] Failed to open file " << filename << std::endl;
    return -1;
  }
  
  //________________________________________________________
  // close HDF5 file
  
  if (H5Iis_valid(fileID)) {
    H5Fclose(fileID);
  }
  
  return nofFailedTests;
}
