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
  \file tdalTable.cc

  \ingroup DAL

  \brief A collection of test routines for the DAL::dalTable class

  \author Lars B&auml;hren

  \date 2008/09/22
*/

#include <dal.h>
#include <dalCommon.h>

//_______________________________________________________________________________
//                                                                      test_H5TB

/*!
  \brief Test the various methods of the HDF5 Table API

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_H5TB ()
{
  std::cout << "\n[tdalTable::test_H5TB]\n" << std::endl;

  int nofFailedTests (0);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test the various constructors for an object of type dalTable

  Test the various constructors for an object of type dalTable. If no input 
  dataset is available (<tt>haveDataset=false</tt>), only the subset of tests
  will be performed, which in fact do not require the existence of a dataset.

  \param filename    -- Name of the input data file
  \param haveDataset -- Is an input dataset provided to perform access tests?

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::string const &filename,
		       bool const &haveDataset)
{
  std::cout << "\n[tdalTable::test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  std::cout << "[1] Default constructor..." << std::endl;
  try {
    DAL::dalTable table;
    //
    table.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Construct object for table of type HDF5..." << std::endl;
  try {
    DAL::dalTable table ("HDF5");
    table.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Construct object for table of type FITS..." << std::endl;
  try {
    DAL::dalTable table ("FITS");
    table.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[4] Construction from dalDataset..." << std::endl;
  try {
    if (haveDataset) {
      DAL::dalDataset dataset;
      dataset.open(filename.c_str());
      dataset.summary();
      //
      DAL::dalTable * table = dataset.openTable ("/beam000","SB000");
      table->summary();
    } else {
      std::cout << "--> Skipping test - missing input dataset." << std::endl;
    }
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test the various methods to access parameters to the class

  \param filename    -- Name of the input data file
  \param haveDataset -- Is an input dataset provided to perform access tests?

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_parameters (std::string const &filename,
		     bool const &haveDataset)
{
  std::cout << "\n[tdalTable::test_parameters]\n" << std::endl;

  int nofFailedTests (0);

  std::cout << "[1] Getting group names from dataset ..." << std::endl;
  try {
    if (haveDataset) {
      DAL::dalDataset dataset (filename.c_str(),"HDF5");
      std::vector<std::string> groupNames = dataset.getGroupNames();
    } else {
      std::cout << "--> Skipping test - missing input dataset." << std::endl;
    }
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test the various methods for working with metadata
  
  \param filename    -- Name of the input data file
  \param haveDataset -- Is an input dataset provided to perform access tests?

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_methods (std::string const &filename,
		  bool const &haveDataset)
{
  std::cout << "\n[tdalTable::test_methods]\n" << std::endl;
  
  int nofFailedTests (0);

  if (haveDataset) {
    DAL::dalDataset dataset (filename.c_str(),"HDF5");
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc,
          char *argv[])
{
  int nofFailedTests (0);
  bool haveDataset (true);
  std::string filename ("tHDF5Dataset.h5");
  std::string tableName ("SB000");

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Run the tests

  if (haveDataset) {
    nofFailedTests += test_constructors(filename, haveDataset);
    nofFailedTests += test_parameters(filename, haveDataset);
    nofFailedTests += test_methods(filename, haveDataset);
  } else {
    std::cerr << "[tdalTable] Skipping tests - no input file!" << std::endl;
  }
  
  return nofFailedTests;
}
