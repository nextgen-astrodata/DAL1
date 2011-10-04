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

#include <data_hl/Sky_ImageGroup.h>

// Namespace usage
using DAL::Sky_ImageGroup;

/*!
  \file tSky_ImageGroup.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::Sky_ImageGroup class
 
  \author Lars B&auml;hren
 
  \date 2011-09-19
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new Sky_ImageGroup object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &fileID)
{
  std::cout << "\n[tSky_ImageGroup::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  /*________________________________________________________
    Test 1 : Default constructor for a new object.
  */

  std::cout << "[1] Testing Sky_ImageGroup() ..." << std::endl;
  try {
    Sky_ImageGroup newObject;
    //
    newObject.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 2 : Argumented constructor, to open an exiting or
             create new image group.
  */

  std::cout << "[2] Testing Sky_ImageGroup(hid_t,uint,IO_Mode) ..." << std::endl;
  try {
    Sky_ImageGroup group (fileID, 0);
    //
    group.summary(); 
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
  std::string filename = "tSky_ImageGroup.h5";
  
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
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }
  
  //________________________________________________________
  // close HDF5 file
  
  H5Fclose (fileID);

  return nofFailedTests;
}
