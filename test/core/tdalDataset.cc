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

  /*__________________________________________________________________
    Test 1: Default constructor. The created object is not connected 
            to a dataset/-file.
  */

  std::cout << "[1] Testing dalDataset() ..." << std::endl;
  try {
    DAL::dalDataset dataset;
    dataset.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*__________________________________________________________________
    Test 2: Argumented constructor, proving the necessary set of 
            parameters for the creation of a dataset/-file.
   */

  std::cout << "[2] Testing dalDataset(string,string,bool) ..." << std::endl;
  try {
    DAL::dalDataset dataset (filename.c_str(),
			     dalType,
			     true);
    dataset.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test creation of and access to attributes

  \brief filename -- Name of the dataset to be created and used for the test
*/
int test_attributes (std::string const &filename,
			std::string const dalType)
{
  std::cout << "\n[tdalDataset::test_attributes]\n" << std::endl;

  int nofFailedTests (0);

  /* Open the dataset to work with */
  DAL::dalDataset dataset (filename.c_str());
  dataset.summary();
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  bool haveDataset     = false;
  std::string dalType  = "HDF5";
  std::string filename = "tdalDataset.h5";
  std::string dataset;

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc > 1) {
    dataset     = std::string(argv[1]);
    haveDataset = true;
  }

  if (argc > 2) {
    dalType = std::string(argv[2]);
  }
  
  //________________________________________________________
  // Run the tests

  //! Test constructors for dalDataset object
  nofFailedTests += test_constructors(filename, dalType);
  //! Test creation of and access to attributes
  nofFailedTests += test_attributes (filename, dalType);
  
  return nofFailedTests;
}
