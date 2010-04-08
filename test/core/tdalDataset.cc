/***************************************************************************
 *   Copyright (C) 2008                                                    *
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
  \file tdalDataset.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::dalDataset class

  \author Lars B&auml;hren

  \date 2008/09/21
*/

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test the various constructors for an object of type DAL::dalDataset

  \param filename -- Name of the input HDF5 data file
  \param dalType  -- Type of the dataset to open

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::string const &filename,
		       std::string const &dalType)
{
  std::cout << "\n[tdalDataset::test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  std::cout << "[1] Default constructor..." << std::endl;
  try {
    DAL::dalDataset dataset;
    //
    dataset.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Argumented constructor..." << std::endl;
  try {
    DAL::dalDataset dataset (filename.c_str(),
			     dalType);
    //
    dataset.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test the various methods which provide access to internal parameters
  
  \param filename -- Name of the input HDF5 data file
  \param dalType  -- Type of the dataset to open
  
  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_parameters (std::string const &filename,
		     std::string const dalType)
{
  std::cout << "\n[tdalDataset::test_parameters]\n" << std::endl;

  int nofFailedTests (0);
  DAL::dalDataset dataset (filename.c_str(),dalType);

  try {
    std::cout << "-- getId ()         = " << dataset.getId ()         << std::endl;
    std::cout << "-- getType ()       = " << dataset.getType ()       << std::endl;
    std::cout << "-- getName ()       = " << dataset.getName ()       << std::endl;
    std::cout << "-- getFileHandle () = " << dataset.getFileHandle () << std::endl;
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test writing attributes to the dataset

  \brief filename -- Name of the dataset to be created and used for the test
*/
int test_setAttributes (std::string const &filename,
			std::string const dalType)
{
  std::cout << "\n[tdalDataset::test_setAttributes]\n" << std::endl;

  int nofFailedTests (0);
  DAL::dalDataset dataset (filename.c_str(),dalType);


  return nofFailedTests;
}

// ------------------------------------------------------------------------------

int main (int argc,char *argv[])
{
  int nofFailedTests (0);
  std::string filename;
  std::string dalType ("HDF5");

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc > 1) {
    filename = std::string(argv[1]);
  }
  else {
    std::cout << "[tdalDataset] Missing name of input test file." << std::endl;
    return(DAL::FAIL);
  }

  if (argc > 2) {
    dalType = std::string(argv[2]);
  }
  
  //________________________________________________________
  // Run the tests

  nofFailedTests += test_constructors(filename,dalType);
  
  if (nofFailedTests == 0) {
    // Test access to the parameters of the object
    nofFailedTests += test_parameters (filename,dalType);
    // Test writing attributes to the dataset
    nofFailedTests += test_setAttributes (filename,dalType);
  }
  
  return nofFailedTests;
}
