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

/*!
  \file tHDF5Attribute.cc

  \ingroup DAL

  \brief A collection of tests for working with a HDF5 Dataset object
 
  \author Lars B&auml;hren
 
  \date 2009/12/03
*/

#include <iostream>
#include <string>

#include "HDF5Attribute.h"

//_______________________________________________________________________________
//                                                              test_construction

/*!
  \brief Test the creation of a HDF5Attribute object

  \param fileID -- File identifier
*/
int test_construction ()
{
  std::cout << "\n[tHDF5Attribute::test_construction]\n" << std::endl;

  int nofFailedTests (0);
  std::string filename ("tHDF5Attribute.h5");
  std::string name;

  // Open the HDF5 file to work with
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);

  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    DAL::HDF5Attribute attribute;
    attribute.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }

  std::cout << "[2] Testing argumented constructor ..." << std::endl;
  try {
    name = "ATTRIBUTE";
    DAL::HDF5Attribute attribute (fileID,name);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }

  // Close the HDF5 file
  H5Fclose(fileID);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_construction

/*!
  \brief Test the creation of a HDF5Attribute object

  \param fileID -- File identifier
*/
int test_construction (std::string const &filename)
{
  std::cout << "\n[tHDF5Attribute::test_construction]\n" << std::endl;

  int nofFailedTests (0);
  
  // Open the HDF5 file to work with
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  if (H5Iis_valid(fileID)) {
    std::string nofStations = "NOF_STATIONS";
    DAL::HDF5Attribute attribute (fileID,nofStations);
    attribute.summary();
  }
  
  // Close the HDF5 file
  H5Fclose(fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                    test_static

int test_static (std::string const &filename)
{
  std::cout << "\n[tHDF5Attribute::test_static]\n" << std::endl;

  int nofFailedTests (0);
  
  // Open the HDF5 file to work with
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  if (H5Iis_valid(fileID)) {
    std::string name = "NOF_STATIONS";
    uint nofStations;
    DAL::HDF5Attribute::getAttribute (fileID,name,nofStations);
    //
    std::cout << "-- " << name << " = " << nofStations << std::endl;
  }
  
  // Close the HDF5 file
  H5Fclose(fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc, char *argv[])
{
  int nofFailedTests (0);
  bool haveDataset (false);
  std::string filename;

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Perform the tests

  nofFailedTests += test_construction ();

  if (haveDataset) {
    nofFailedTests += test_construction (filename);
    nofFailedTests += test_static (filename);
  }
  
  return nofFailedTests;
}
