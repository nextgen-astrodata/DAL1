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

#include <coordinates/LinearCoordinate.h>

/*!
  \file tLinearCoordinate.cc

  \ingroup DAL
  \ingroup coordinates

  \brief A collection of test routines for the DAL::LinearCoordinate class

  \author Lars B&auml;hren

  \date 2009/06/24
*/

using std::cerr;
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

  cout << "[1] Testing LinearCoordinate() ..." << endl;
  try {
    DAL::LinearCoordinate coord;
    //
    coord.summary();
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing LinearCoordinate(int) ..." << endl;
  try {
    
    cout << "--> coordinate with 2 axes" << endl;

    DAL::LinearCoordinate coord2d (2);
    coord2d.summary();

    cout << "--> coordinate with 3 axes" << endl;

    DAL::LinearCoordinate coord3d (3);
    coord3d.summary();

    cout << "--> coordinate with 4 axes" << endl;

    DAL::LinearCoordinate coord4d (4);
    coord4d.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing LinearCoordinate(int,vector<string>,vector<string>) ..." << endl;
  try {
    unsigned int nofAxes (2);
    std::vector<std::string> worldAxisNames (nofAxes);
    std::vector<std::string> worldAxisUnits (nofAxes);
    //
    worldAxisNames[0] = "Length";
    worldAxisNames[1] = "Time";
    worldAxisUnits[0] = "m";
    worldAxisUnits[1] = "s";
    //
    DAL::LinearCoordinate coord (nofAxes,
				 worldAxisNames,
				 worldAxisUnits);
    //
    coord.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] Testing fully argumented constructor ..." << endl;
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
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[5] Testing copy constructor ..." << endl;
  try {
    unsigned int nofAxes (2);
    std::vector<std::string> names (nofAxes);
    std::vector<std::string> units (nofAxes);
    //
    names[0] = "Time";
    names[1] = "Frequency";
    units[0] = "s";
    units[1] = "Hz";
    //
    DAL::LinearCoordinate coord (nofAxes);
    coord.setAxisNames (names);
    coord.setAxisUnits (units);
    coord.summary();
    //
    DAL::LinearCoordinate coordCopy (coord);
    coordCopy.summary();
  } catch (std::string message) {
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
  
  coord.summary();

  cout << "[1] Adjust world axis names ..." << endl;
  try {
    std::vector<std::string> names = coord.axisNames();
    cout << "-- old values = " << names << endl;
    
    names[0] = "Time";
    names[1] = "Frequency";
    
    coord.setAxisNames(names);
    cout << "-- new values = " << coord.axisNames() << endl;
    
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Adjust world axis units ..." << endl;
  try {
    std::vector<std::string> units = coord.axisUnits();
    cout << "-- old values = " << units << endl;
    
    units[0] = "s";
    units[1] = "Hz";
    
    coord.setAxisUnits(units);
    cout << "-- new values = " << coord.axisUnits() << endl;
    
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Adjust reference value ..." << endl;
  try {
    std::vector<double> refValue = coord.refValue();
    cout << "-- old values = " << refValue << endl;
    
    refValue[0] = 0.1;
    refValue[1] = 30e06;
    
    coord.setRefValue(refValue);
    cout << "-- new values = " << coord.refValue() << endl;
    
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] Adjust reference pixel ..." << endl;
  try {
    std::vector<double> refPixel = coord.refPixel();
    cout << "-- old values = " << refPixel << endl;
    
    refPixel[0] = 1;
    refPixel[1] = 2;
    
    coord.setRefPixel(refPixel);
    cout << "-- new values = " << coord.refPixel() << endl;
    
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[5] Adjust coordinate axis increment ..." << endl;
  try {
    std::vector<double> incr = coord.increment();
    cout << "-- old values = " << incr << endl;
    
    incr[0] = 0.1;
    incr[1] = 1e06;
    
    coord.setIncrement(incr);
    cout << "-- new values = " << coord.increment() << endl;
    
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[6] Adjust transformation matrix ..." << endl;
  try {
    std::vector<double> pc = coord.pc();
    cout << "-- old values = " << pc << endl;
    
    pc[0] = 0.5;
    pc[1] = 1.5;
    pc[2] = 0.5;
    pc[3] = 1.5;
    
    coord.setPc(pc);
    cout << "-- new values = " << coord.pc() << endl;
    
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[7] Retrive tabulated axis values ..." << endl;
  try {
    std::cout << "-- Pixel values = " << coord.pixelValues() << std::endl;
    std::cout << "-- World values = " << coord.worldValues() << std::endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_hdf5

#ifdef DAL_WITH_HDF5

int test_hdf5 (std::string const &filename="tLinearCoordinate.h5")
{
  cout << "\n[tLinearCoordinate::test_hdf5]\n" << endl;

  int nofFailedTests (0);
  unsigned int nofAxes;
  std::vector<std::string> worldAxisNames;
  std::vector<std::string> worldAxisUnits;
  std::vector<double> refValue;
  std::vector<double> refPixel;
  std::vector<double> increment;
  std::vector<double> pc;

  //________________________________________________________
  // Create HDF5 file to work with

  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);

  /* test of file creation was successful */
  if (H5Iis_valid(fileID)) {
    cout << "-- Successfully opened file " << filename << endl;
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }
    
  //________________________________________________________
  // Test 1: Linear coordinate with 1 axis (default)

  cout << "[1] Write linear coordinate with 1 axis (default) ..." << endl;
  try {
    // set number of axes
    nofAxes = 1;
    // create coordinate object
    DAL::LinearCoordinate coord (nofAxes);
    // show summary of object
    coord.summary();
    // write coordinate to file
    coord.write_hdf5(fileID,"LinearCoordinate1");
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 2: Linear coordinate with 1 axis (custom)

  cout << "[2] Write linear coordinate with 1 axis (custom) ..." << endl;
  try {
    // set number of axes
    nofAxes = 1;
    // resize arrays
    worldAxisNames.resize(nofAxes);
    worldAxisUnits.resize(nofAxes);
    refPixel.resize(nofAxes);
    refValue.resize(nofAxes);
    increment.resize(nofAxes);
    pc.resize(nofAxes*nofAxes);
    // assign arrays
    worldAxisNames[0] = "Length";
    worldAxisUnits[0] = "m";
    refPixel[0]       = 1.0;
    refValue[0]       = 2.0;
    increment[0]      = 0.5;
    pc[0]             = 1.0;
    // create coordinate object
    DAL::LinearCoordinate coord (nofAxes);
    // show summary of object
    coord.summary();
    // write coordinate to file
    coord.write_hdf5(fileID,"LinearCoordinate2");
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 3: Linear coordinate with 2 axes

  cout << "[3] Write linear coordinate with 2 axes ..." << endl;
  try {
    // set number of axes
    nofAxes = 2;
    // resize arrays
    worldAxisNames.resize(nofAxes);
    worldAxisUnits.resize(nofAxes);
    refPixel.resize(nofAxes);
    refValue.resize(nofAxes);
    increment.resize(nofAxes);
    pc.resize(nofAxes*nofAxes);
    // assign arrays
    worldAxisNames[0] = "Length";
    worldAxisNames[1] = "Time";
    worldAxisUnits[0] = "m";
    worldAxisUnits[1] = "s";
    refPixel[0]       = 1.0;
    refPixel[1]       = 10.0;
    refValue[0]       = 2.0;
    refValue[1]       = 20.0;
    increment[0]      = 0.5;
    increment[1]      = 5.0;
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
    // show summary of object
    coord.summary();
    // write coordinate to file
    coord.write_hdf5(fileID,"LinearCoordinate3");
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 4: Linear coordinate with 3 axes

  cout << "[4] Linear coordinate with 3 axes ..." << endl;
  try {
    // set number of axes
    nofAxes = 3;
    // resize arrays
    worldAxisNames.resize(nofAxes);
    worldAxisUnits.resize(nofAxes);
    refPixel.resize(nofAxes);
    refValue.resize(nofAxes);
    increment.resize(nofAxes);
    // assign arrays
    worldAxisNames[0] = "x";
    worldAxisNames[1] = "y";
    worldAxisNames[2] = "z";
    worldAxisUnits[0] = "m";
    worldAxisUnits[1] = "m";
    worldAxisUnits[2] = "m";
    refPixel[0]       = 1.0;
    refPixel[1]       = 1.0;
    refPixel[2]       = 1.0;
    refValue[0]       = 1.0;
    refValue[1]       = 2.0;
    refValue[2]       = 4.0;
    increment[0]      = 0.1;
    increment[1]      = 0.2;
    increment[2]      = 0.4;
    
    DAL::IdentityMatrix (pc, nofAxes);
    
    DAL::LinearCoordinate coord (nofAxes,
				 worldAxisNames,
				 worldAxisUnits,
				 refValue,
				 refPixel,
				 increment,
				 pc);
    coord.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  H5Fclose(fileID);
  
  return nofFailedTests;
}

#endif 

//_______________________________________________________________________________
//                                                                      test_casa

#ifdef DAL_WITH_CASA

int test_casa ()
{
  cout << "\n[tLinearCoordinate::test_casa]\n" << endl;

  int nofFailedTests (0);

  return nofFailedTests;
}

#endif

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);
  
  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test access to the internal paramters
  nofFailedTests += test_parameters();

#ifdef DAL_WITH_CASA
  // Test writing coordinate information to HDF5 file
  nofFailedTests += test_casa();
#endif

#ifdef DAL_WITH_HDF5
  // Test writing coordinate information to HDF5 file
  nofFailedTests += test_hdf5();
#endif

  return nofFailedTests;
}
