/***************************************************************************
 *   Copyright (C) 2008 by Lars B"ahren                                    *
 *   lbaehren@gmail,com                                                    *
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
  \file tdalTable.cpp

  \ingroup DAL

  \brief Test program for table within a dataset

  \author Lars B&auml;hren

  \date 2008/09/22
*/

#include <dal.h>

const std::string tableName ("SB000");

// ------------------------------------------------------------------------------

/*!
  \brief Test the various constructors for an object of type dalTable

  \param filename -- Name of the input data file

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::string const &filename)
{
  std::cout << "\n[tdalTable::test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  std::cout << "[1] Default constructor..." << std::endl;
  try {
    DAL::dalTable table;
    //
    table.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[2] Construct object for table of type HDF5..." << std::endl;
  try {
    DAL::dalTable table ("HDF5");
    table.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[3] Construct object for table of type FITS..." << std::endl;
  try {
    DAL::dalTable table ("FITS");
    table.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[4] Construction from dalDataset..." << std::endl;
  try {
    DAL::dalDataset dataset;
    dataset.open(filename.c_str());
    dataset.summary();
    //
    DAL::dalTable * table = dataset.openTable ("/beam000","SB000");
    table->summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test the various methods to access parameters to the class

  \param filename -- Name of the input data file

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_parameters (std::string const &filename)
{
  std::cout << "\n[tdalTable::test_parameters]\n" << std::endl;

  int nofFailedTests (0);
  DAL::dalTable * table;

  try {
    DAL::dalDataset dataset (filename.c_str(),"HDF5");
    std::vector<std::string> groupNames = dataset.getGroupNames();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test the various methods for working with metadata

  \param filename -- Name of the input data file

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_methods (std::string const &filename)
{
  std::cout << "\n[tdalTable::test_methods]\n" << std::endl;

  int nofFailedTests (0);
  
  return nofFailedTests;
}

// ------------------------------------------------------------------------------

int main (int argc,char *argv[]) 
{
  int nofFailedTests (0);
  std::string filename;

  /* Check command line parameters */

  if (argc > 1) {
    filename = std::string(argv[1]);
  } else {
    std::cout << "[tdalTable] Missing name of input test file." << std::endl;
    return(DAL::FAIL);
  }

  /* Test the constructors */
  nofFailedTests += test_constructors(filename);

  if (nofFailedTests == 0) {
    nofFailedTests += test_parameters(filename);
    nofFailedTests += test_methods(filename);
  }

  return nofFailedTests;
}
