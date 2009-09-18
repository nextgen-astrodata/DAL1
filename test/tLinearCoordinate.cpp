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

#include <LinearCoordinate.h>

/*!
  \file tLinearCoordinate.cpp

  \ingroup DAL

  \brief A collection of test routines for the LinearCoordinate class

  \author Lars B&auml;hren

  \date 2009/06/24
*/

using std::cout;
using std::endl;

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new LinearCoordinate object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tLinearCoordinate::test_constructors]\n" << endl;

  int nofFailedTests (0);

  cout << "[1] Testing default constructor ..." << endl;
  try {
    DAL::LinearCoordinate coord;
    //
    coord.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing argumented constructor ..." << endl;
  try {
    DAL::LinearCoordinate coord (3);
    //
    coord.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing fully argumented constructor ..." << endl;
  try {
    unsigned int nofAxes (2);
    std::vector<std::string> worldAxisNames (nofAxes);
    std::vector<std::string> worldAxisUnits (nofAxes);
    std::vector<double> refValue (nofAxes,0.0);
    std::vector<double> refPixel (nofAxes,0.0);
    std::vector<double> increment (nofAxes,1.0);
    std::vector<double> pc (nofAxes*nofAxes);
    //
    worldAxisNames[0] = "Length";
    worldAxisNames[1] = "Time";
    worldAxisUnits[0] = "m";
    worldAxisUnits[1] = "s";
    //
    DAL::LinearCoordinate coord (nofAxes,
				 worldAxisNames,
				 worldAxisUnits,
				 refValue,
				 refPixel,
				 increment,
				 pc);
    //
    coord.summary();
  }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }
  
  cout << "[4] Testing copy constructor ..." << endl;
  try
    {
      DAL::LinearCoordinate coord (3);
      coord.summary();
      //
      DAL::LinearCoordinate coordCopy (coord);
      coordCopy.summary();
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_parameters

/*!
  \brief Test access to the interal parameters of a LinearCoordinate object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_parameters ()
{
  cout << "\n[tLinearCoordinate::test_parameters]\n" << endl;

  int nofFailedTests (0);
  unsigned int nofAxes (2);
  DAL::LinearCoordinate coord (nofAxes);

  cout << "[1] Adjust world axis names ..." << endl;
  try
    {
      std::vector<std::string> names = coord.axisNames();
      cout << names << endl;

      names[0] = "Time";
      names[1] = "Frequency";

      coord.setAxisNames(names);
      cout << coord.axisNames() << endl;

    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[2] Adjust world axis units ..." << endl;
  try
    {
      std::vector<std::string> units = coord.axisUnits();
      cout << units << endl;

      units[0] = "s";
      units[1] = "Hz";

      coord.setAxisUnits(units);
      cout << coord.axisUnits() << endl;

    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[3] Adjust reference value ..." << endl;
  try
    {
      std::vector<double> refValue = coord.refValue();
      cout << refValue << endl;

      refValue[0] = 0.1;
      refValue[1] = 30e06;

      coord.setRefValue(refValue);
      cout << coord.refValue() << endl;

    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[4] Adjust reference pixel ..." << endl;
  try
    {
      std::vector<double> refPixel = coord.refPixel();
      cout << refPixel << endl;

      refPixel[0] = 1;
      refPixel[1] = 2;

      coord.setRefPixel(refPixel);
      cout << coord.refPixel() << endl;

    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[5] Adjust coordinate axis increment ..." << endl;
  try
    {
      std::vector<double> incr = coord.increment();
      cout << incr << endl;

      incr[0] = 0.1;
      incr[1] = 1e06;

      coord.setIncrement(incr);
      cout << coord.increment() << endl;

    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

int test_methods ()
{
  cout << "\n[tLinearCoordinate::test_methods]\n" << endl;

  int nofFailedTests (0);
  std::string filename ("tLinearCoordinate.h5");

  unsigned int nofAxes (2);
  std::vector<std::string> worldAxisNames (nofAxes);
  std::vector<std::string> worldAxisUnits (nofAxes);
  std::vector<double> refValue (nofAxes,0.0);
  std::vector<double> refPixel (nofAxes,0.0);
  std::vector<double> increment (nofAxes,1.0);
  std::vector<double> pc (nofAxes*nofAxes);

  worldAxisNames[0] = "Length";
  worldAxisNames[1] = "Time";
  worldAxisUnits[0] = "m";
  worldAxisUnits[1] = "s";
  pc[0]             = 1;
  pc[1]             = 0;
  pc[2]             = 0;
  pc[3]             = 1;

  DAL::LinearCoordinate coord (nofAxes,
                               worldAxisNames,
                               worldAxisUnits,
                               refValue,
                               refPixel,
                               increment,
                               pc);
  coord.summary();

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
    coord.h5write(fileID,"LinearCoordinate");
    
    cout << "-- Close the HDF5 file ..." << endl;
    H5Fclose(fileID);
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Read coordinate from a HDF5 file ..." << endl;
  try {
    hid_t fileID;
    
    cout << "-- Open HDF5 file to work with ..." << endl;
    fileID = H5Fopen (filename.c_str(),
		      H5F_ACC_RDONLY,
		      H5P_DEFAULT);
    
    cout << "-- Read the coordinate from the root group ..." << endl;
    DAL::LinearCoordinate coord1;
    coord1.h5read(fileID);
    coord1.summary();
    
    cout << "-- Read the coordinate from a group within the file ..." << endl;
    coord.h5read(fileID,"LinearCoordinate");
    coord1.summary();
    
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
  // Test access to the internal paramters
  nofFailedTests += test_parameters();
  // Test the various methods
  nofFailedTests += test_methods();
  
  return nofFailedTests;
}
