/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars Baehren (lbaehren@gmail.com)                                     *
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

#include <core/MS_Table.h>

// Namespace usage
using DAL::MS_Table;

/*!
  \file tMS_Table.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::MS_Table class
 
  \author Lars B&auml;hren
 
  \date 2011-07-18
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new MS_Table object

  \param filename -- Name of the MeasurementSet to work with.
  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename)
{
  std::cout << "\n[tMS_Table::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing MS_Table() ..." << std::endl;
  try {
    MS_Table ms;
    //
    ms.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing MS_Table(string) ..." << std::endl;
  try {
    MS_Table ms (filename);
    //
    ms.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                  test_readData

/*!
  \brief Test reading data from MS table columns.

  \param filename -- Name of the MeasurementSet to work with.
  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_readData (std::string const &filename)
{
  std::cout << "\n[tMS_Table::test_readData]\n" << std::endl;

  int nofFailedTests (0);
  
  // Open MS to work with
  MS_Table ms (filename);
  ms.summary(false); 

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
  int nofFailedTests (0);
  bool haveDataset     = false;
  std::string filename = "tMS_Table.ms";

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }
  
  std::cout << "[tHDF5Dataset] Output HDF5 file = " << filename << std::endl;


  // Test for the constructor(s)
  nofFailedTests += test_constructors (filename);
  // Test reading data from MS table columns.
  nofFailedTests += test_readData (filename);

  return nofFailedTests;
}
