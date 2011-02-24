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

#include <coordinates/TabularCoordinate.h>

using std::cerr;
using std::cout;
using std::endl;

/*!
  \file tTabularCoordinate.cc

  \ingroup DAL
  \ingroup coordinates

  \brief A collection of test routines for the DAL::TabularCoordinate class

  \author Lars B&auml;hren

  \date 2009/06/24
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new TabularCoordinate object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tTabularCoordinate::test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  cout << "[1] Testing TabularCoordinate() ..." << std::endl;
  try {
    cout << "--> TabularCoordinate<int>" << endl;
    DAL::TabularCoordinate<int> coordInt;
    coordInt.summary();
    //
    cout << "--> TabularCoordinate<float>" << endl;
    DAL::TabularCoordinate<float> coordFloat;
    coordFloat.summary();
    //
    cout << "--> TabularCoordinate<double>" << endl;
    DAL::TabularCoordinate<double> coordDouble;
    coordDouble.summary();
    //
    cout << "--> TabularCoordinate<std::string>" << endl;
    DAL::TabularCoordinate<std::string> coordString;
    coordString.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing TabularCoordinate(string,string) ..." << std::endl;
  try {
    std::string names ("Time");
    std::string units ("s");
    //
    DAL::TabularCoordinate<double> coord (names, units);
    //
    coord.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing TabularCoordinate(string,string,vector<double>,vector<double>) ..." << std::endl;
  try {
    unsigned int nelem (4);
    std::string axisNames("Length");
    std::string axisUnits("m");
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
    DAL::TabularCoordinate<double> coord (axisNames,
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

/*!
  \brief Test public methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_methods ()
{
  cout << "\n[tTabularCoordinate::test_methods]\n" << endl;

  int nofFailedTests (0);
  DAL::TabularCoordinate<double> coord;

  cout << "[1] Testing setAxisNames(string) ..." << std::endl;
  coord.setAxisNames ("Time");
  cout << "--> World axis names = " << coord.axisNames() << std::endl;

  cout << "[2] Testing setAxisUnits(string) ..." << std::endl;
  coord.setAxisUnits ("s");
  cout << "--> World axis units = " << coord.axisUnits() << std::endl;

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_hdf5

int test_hdf5 ()
{
  cout << "\n[tTabularCoordinate::test_hdf5]\n" << endl;

  int nofFailedTests (0);
  std::string filename ("tTabularCoordinate.h5");
  
  unsigned int nelem (4);
  std::string axisNames("Length");
  std::string axisUnits("m");
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

  DAL::TabularCoordinate<double> coord (axisNames,
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
    coord.write_hdf5(fileID);
    
    cout << "-- Write the coordinate to new group within the file ..." << endl;
    coord.write_hdf5(fileID,"TabularCoordinate");
    
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
//                                                             example_beamformed

/*!
  \brief Coordinate example for non-linear frequency axis in beamformed data

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int example_beamformed ()
{
  cout << "\n[tTabularCoordinate::example_beamformed]\n" << endl;

  int nofFailedTests (0);
  int n (0);
  /* Total number of frequency bands */
  int nofBands (3);
  /* Number of channels per frequency band */
  int channelsPerBand (512);
  /* Start frequency of the individual bands, [MHz] */
  std::vector<double> startFrequency (nofBands);
  /* Frequency incremen between two channels within a band, [MHz]*/
  double increment (0.1953125);
  /* Vector with the frequency values, [MHz] */
  std::vector<double> worldValues (nofBands*channelsPerBand);
  /* Vector with the pixel values */
  std::vector<double> pixelValues (nofBands*channelsPerBand);

  startFrequency[0] = 140;
  startFrequency[1] = 150;
  startFrequency[2] = 165;

  //________________________________________________________
  // [1] Assign pixel and world values

  /* Step through the frequency bands */
  for (int band(0); band<nofBands; band++) {
    /* Step through the channels within a band */
    for (int channel(0); channel<channelsPerBand; channel++) {
      pixelValues[n] = n;
      worldValues[n] = startFrequency[band]+channel*increment;
      n++;
    }
  }

  //________________________________________________________
  // [2] Create the coordinate object

  std::string axisNames("Length");
  std::string axisUnits("m");
  
  DAL::TabularCoordinate<double> coord (axisNames,
					axisUnits,
					pixelValues,
					worldValues);
  
  //________________________________________________________
  // [3] Write coordinate object to HDF5 file as group
  
  try {
    std::string filename ("tTabularCoordinate_bf.h5");
    hid_t fileID;
    
    cout << "-- Create HDF5 file to work with ..." << endl;
    fileID = H5Fcreate (filename.c_str(),
			H5F_ACC_TRUNC,
			H5P_DEFAULT,
			H5P_DEFAULT);
    
    cout << "-- Write the coordinate to the root group ..." << endl;
    coord.write_hdf5(fileID);
    
    cout << "-- Write the coordinate to new group within the file ..." << endl;
    coord.write_hdf5(fileID,"TabularCoordinate");
    
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
  // Test public methods
  nofFailedTests += test_methods ();

#ifdef DAL_WITH_HDF5
  // Test the various methods
  nofFailedTests += test_hdf5();
#endif

  // Coordinate example for non-linear frequency axis in beamformed data
  nofFailedTests += example_beamformed ();

  return nofFailedTests;
}
