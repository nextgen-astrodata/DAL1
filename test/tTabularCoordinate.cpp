/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 2286 2009-02-03 10:50:48Z baehren                  $ |
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

#include <TabularCoordinate.h>

/*!
  \file tTabularCoordinate.cc

  \ingroup DAL

  \brief A collection of test routines for the TabularCoordinate class

  \author Lars Baehren

  \date 2009/06/24
*/

// -----------------------------------------------------------------------------

/*!
  \brief Test constructors for a new TabularCoordinate object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tTabularCoordinate::test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    DAL::TabularCoordinate coord;
    //
    coord.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing argumented constructor ..." << std::endl;
  try {
    unsigned int nelem (4);
    std::vector<std::string> axisNames(1,"Length");
    std::vector<std::string> axisUnits(1,"m");
    std::vector<double> pixelValues(nelem);
    std::vector<double> worldValues(nelem);
    //
    pixelValues[0] = 1;
    pixelValues[1] = 2;
    pixelValues[2] = 3;
    pixelValues[3] = 4;
    worldValues[0] = 1;
    worldValues[1] = 2;
    worldValues[2] = 5;
    worldValues[3] = 10;
    //
    DAL::TabularCoordinate coord (axisNames,
				  axisUnits,
				  pixelValues,
				  worldValues);
    //
    coord.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

int test_methods ()
{
  cout << "\n[tTabularCoordinate::test_methods]\n" << endl;

  int nofFailedTests (0);
  std::string filename ("tTabularCoordinate.h5");

  unsigned int nelem (4);
  std::vector<std::string> axisNames(1,"Length");
  std::vector<std::string> axisUnits(1,"m");
  std::vector<double> pixelValues(nelem);
  std::vector<double> worldValues(nelem);

  pixelValues[0] = 1;
  pixelValues[1] = 2;
  pixelValues[2] = 3;
  pixelValues[3] = 4;
  worldValues[0] = 1;
  worldValues[1] = 2;
  worldValues[2] = 5;
  worldValues[3] = 10;

  DAL::TabularCoordinate coord (axisNames,
                                axisUnits,
                                pixelValues,
                                worldValues);

  cout << "[1] Write coordinate to a HDF5 file ..." << endl;
  try {
    hid_t fileID;
    
    cout << "-- Create HDF5 file to work with ..." << endl;
    fileID = H5Fcreate (filename.c_str(),
			H5F_ACC_TRUNC,
			H5P_DEFAULT,
			H5P_DEFAULT);
    
    cout << "-- Write the coordinate to the root group ..." << endl;
    coord.h5write(fileID);
    
    cout << "-- Write the coordinate to new group within the file ..." << endl;
    coord.h5write(fileID,"TabularCoordinate");
    
    cout << "-- Close the HDF5 file ..." << endl;
    H5Fclose(fileID);
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test the various methods
  nofFailedTests += test_methods();

  return nofFailedTests;
}
