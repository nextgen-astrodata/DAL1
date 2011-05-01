/***************************************************************************
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

#include <coordinates/CoordinatesGroup.h>

// Namespace usage
using DAL::CoordinatesGroup;

/*!
  \file tCoordinatesGroup.cc

  \ingroup DAL
  \ingroup coordinates

  \brief A collection of test routines for the DAL::CoordinatesGroup class
 
  \author Lars B&auml;hren
 
  \date 2009/09/13
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new CoordinatesGroup object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &fileID)
{
  std::cout << "\n[tCoordinatesGroup::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  std::string groupname ("Coordinates");
  
  std::cout << "[1] Testing CoordinatesGroup() ..." << std::endl;
  try {
    CoordinatesGroup coord;
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing CoordinatesGroup(hid_t) ..." << std::endl;
  try {
    CoordinatesGroup coord (fileID);
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Testing CoordinatesGroup(hid_t,string) ..." << std::endl;
  try {
    CoordinatesGroup coord (fileID,groupname);
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

/*!
  \brief Test reading/writing of the cordinates group to a file

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_methods (hid_t const &fileID)
{
  std::cout << "\n[tCoordinatesGroup::test_methods]\n" << std::endl;

  int nofFailedTests = 0;
  bool status        = true;
  CoordinatesGroup coord;

  cout << "[1] Testing setReferenceLocation() ... " << endl;
  try {
    unsigned int nelem (3);
    std::vector<double> value (nelem,1);
    std::vector<std::string> units (nelem,"m");
    std::string frame ("Cartesian");
    
    coord.setReferenceLocation (value, units, frame);
    coord.summary();

  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  cout << "[2] Testing setReferenceTime() ... " << endl;
  try {
    double value      = 123456;
    std::string units = "s";
    std::string frame = "UTC";
    
    coord.setReferenceTime (value, units, frame);
    coord.summary();

  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  cout << "[3] Testing h5write() ... " << endl;
  try {
    status = coord.h5write (fileID);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);
  std::string filename ("tCoordinatesGroup.h5");
  
  //________________________________________________________
  // Create HDF5 file to work with
  
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  
  /* If file creation was successful, run the tests. */
  if (H5Iis_valid(fileID)) {
    
    // Test for the constructor(s)
    nofFailedTests += test_constructors (fileID);
    // Test reading/writing of the cordinates group to a file
    nofFailedTests += test_methods (fileID);
    
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }
  
  //________________________________________________________
  // close HDF5 file
  
  H5Fclose(fileID);

  return nofFailedTests;
}
