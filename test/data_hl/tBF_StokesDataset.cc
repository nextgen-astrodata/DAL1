/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#include <BF_StokesDataset.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::BF_StokesDataset;

/*!
  \file tBF_StokesDataset.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::BF_StokesDataset class
 
  \author Lars B&auml;hren
 
  \date 2010/12/05
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_StokesDataset object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename)
{
  std::cout << "\n[tBF_StokesDataset::test_constructors]\n" << endl;

  int nofFailedTests (0);
  std::string nameDataset ("StokesI");
  std::vector<hsize_t> shape (2);

  shape[0] = 100;
  shape[1] = 1024;
  
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
  // Test 1: Default constructor

  std::cout << "[1] Testing BF_StokesDataset() ..." << endl;
  try {
    BF_StokesDataset stokes;
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 2: Argumented constructor

  std::cout << "[2] Testing BF_StokesDataset(hid_t, string) ..." << endl;
  try {
    BF_StokesDataset stokes (fileID, nameDataset);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 3: Argumented constructor

  std::cout << "[3] Testing BF_StokesDataset(hid_t, string, vector<hsize_t>) ..."
	    << endl;
  try {
    BF_StokesDataset stokes (fileID, nameDataset, shape);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 4: Argumented constructor

  std::cout << "[4] Testing BF_StokesDataset(hid_t, string, vector<hsize_t>, Stokes::Component) ..."
	    << endl;
  try {
    nameDataset = "StokesQ";
    //
    BF_StokesDataset stokes (fileID, nameDataset, shape, DAL::Stokes::Q);
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Close HDF5 file used for testing

  H5Fclose(fileID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

int test_attributes (std::string const &filename)
{
  std::cout << "\n[tBF_StokesDataset::test_attributes]\n" << endl;

  int nofFailedTests (0);
  std::string nameDataset ("StokesI");

  //________________________________________________________
  // Open HDF5 file to work with
  
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  /* test of file creation was successful */
  if (H5Iis_valid(fileID)) {
    cout << "-- Successfully opened file " << filename << endl;
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }

  // Open dataset
  BF_StokesDataset stokes (fileID, nameDataset);
  int nofChannels;
  int nofSubbands;

  cout << "[1] Testing getAttribute(string, T) ..." << endl;
  try {
    stokes.getAttribute ("NOF_CHANNELS", nofChannels);
    stokes.getAttribute ("NOF_SUBBANDS", nofSubbands);
    
    std::cout << "-- NOF_CHANNELS = " << nofChannels << std::endl;
    std::cout << "-- NOF_SUBBANDS = " << nofSubbands << std::endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Close HDF5 file used for testing

  H5Fclose(fileID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);
  std::string filename ("tBF_StokesDataset.h5");

  // Test for the constructor(s)
  nofFailedTests += test_constructors (filename);
  // Test access to the attributes
  nofFailedTests += test_attributes (filename);

  return nofFailedTests;
}
