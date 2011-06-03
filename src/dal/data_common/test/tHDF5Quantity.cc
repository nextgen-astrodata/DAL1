/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#include <data_common/HDF5Quantity.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::HDF5Quantity;

/*!
  \file tHDF5Quantity.cc

  \ingroup DAL
  \ingroup data_common

  \brief A collection of test routines for the HDF5Quantity class
  
  \author Lars B&auml;hren
  
  \date 2011/06/01
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new HDF5Quantity object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tHDF5Quantity::test_constructors]" << endl;

  int nofFailedTests = 0;
  
  //________________________________________________________
  // Test 1 : Default constructor

  cout << "\n[1] Testing HDF5Quantity() ..." << endl;
  try {
    HDF5Quantity q;
    //
    q.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 2 : Argumented constructor

  cout << "\n[2] Testing HDF5Quantity(string) ..." << endl;
  try {
    HDF5Quantity q ("TIME");
    //
    q.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________

  cout << "\n[3] Testing HDF5Quantity(string,string,string,string) ..." << endl;
  try {
    HDF5Quantity q ("Time","Value","Units",".");
    //
    q.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________

  cout << "\n[4] Testing HDF5Quantity(string,double,string) ..." << endl;
  try {
    double value (1.0);
    std::string unit ("s");
    HDF5Quantity q ("TIME",value, unit);
    //
    q.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________

  cout << "\n[5] Testing HDF5Quantity(string,vector<double>,string) ..." << endl;
  try {
    std::vector<double> value (3,1.0);
    std::string unit ("m");
    HDF5Quantity q ("POSITION",value, unit);
    //
    q.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________

  cout << "\n[6] Testing HDF5Quantity(vector<string>,vector<double>,string) ..."
       << endl;
  try {
    unsigned int nelem (3);
    std::vector<double> value (nelem);
    std::vector<std::string> units (nelem);
    //
    value[0] = 0;
    value[1] = 90;
    value[2] = 1;
    units[0] = "deg";
    units[1] = "deg";
    units[2] = "m";
    //
    HDF5Quantity q ("POSITION",value, units);
    //
    q.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________

  cout << "\n[7] Testing HDF5Quantity(HDF5Quantity) ..." << endl;
  try {
    std::vector<double> value (3,1.0);
    std::string unit ("m");
    //
    HDF5Quantity q ("POSITION",value, unit);
    q.summary(); 
    //
    HDF5Quantity copy (q);
    copy.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_parameters

/*!
  \brief Test methods for accessing internal parameters

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_parameters ()
{
  cout << "\n[tHDF5Quantity::test_parameters]" << endl;

  int nofFailedTests = 0;
  
  HDF5Quantity q;
  q.summary();

  cout << "\n[1] Set base of the attribute name ..." << endl;
  {
    q.setName ("Position");
    q.summary();
  }

  cout << "\n[2] Set the separator between the sub-strings ..." << endl;
  {
    q.setSeparator (".");
    q.summary();
  }
  
  cout << "\n[3] Set the suffix for the attribute storing the value ..." << endl;
  {
    q.setValueSuffix ("Value");
    q.summary();
  }
  
  cout << "\n[4] Set the suffix for the attribute storing the units ..." << endl;
  {
    q.setUnitsSuffix ("Units");
    q.summary();
  }
  
  cout << "\n[5] Set the value of the quantity ..." << endl;
  {
    std::vector<double> value (3);
    //
    value[0] =  0.0;
    value[1] = 90.0;
    value[2] =  0.5;
    //
    q.setValue(value);
    q.summary();
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                        test_io

/*!
  \brief Test writing/reading attributes storing quantity components

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_io (std::string const &filename)
{
  cout << "\n[tHDF5Quantity::test_io]" << endl;

  int nofFailedTests = 0;
  hid_t fileID       = H5Fcreate (filename.c_str(),
				  H5F_ACC_TRUNC,
				  H5P_DEFAULT,
				  H5P_DEFAULT);
  
  HDF5Quantity q ("Position","Value","Units",".");
  
  /*________________________________________________________
    Set numerical value and units of the quantity
  */
  {
    unsigned int nelem (3);
    std::vector<double> value (nelem);
    std::vector<std::string> units (nelem);
    //
    value[0] = 0;
    value[1] = 90;
    value[2] = 1;
    units[0] = "deg";
    units[1] = "deg";
    units[2] = "m";
    //
    q.setQuantity (value,units);
    //
    q.summary();
  }
  
  /*________________________________________________________
    Write attributes to file
  */
  {
    q.write (fileID);
  }
  
  H5Fclose(fileID);
  
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
  std::string filename = "tHDF5Quantity.h5";

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  // Test methods for accessing internal parameters
  nofFailedTests += test_parameters ();

  // Test writing/reading attributes storing quantity components
  nofFailedTests += test_io (filename);

  return nofFailedTests;
}
