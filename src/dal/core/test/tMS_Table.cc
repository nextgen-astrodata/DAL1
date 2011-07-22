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

#ifdef DAL_WITH_CASA

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
  std::cout << "\n[tMS_Table::test_readData]" << std::endl;

  int nofFailedTests = 0;
  bool status        = true;

  // Open MS to work with
  MS_Table ms (filename);
  ms.summary(false); 
  
  /*________________________________________________________
    Test 1: Read data from columns of type double
  */
  
  std::cout << "\n[1] Testing readData (Array<double>,string) ..." << std::endl;
  try {
    casa::Array<casa::Double> data;
    // Define the colums from which to try reading data
    std::vector<std::string> columns;
    columns.push_back("TIME");
    columns.push_back("UVW");
    columns.push_back("INTERVAL");
    columns.push_back("EXPOSURE");
    
    // Read the data from the table columns and display their properties
    for (unsigned int n=0; n<columns.size(); ++n) {
      status = ms.readData (data, columns[n]);
      std::cout << std::setw(15) << columns[n]
		<< std::setw(15) << data.shape()
		<< std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 2: Read data from columns of type float
  */
  
  std::cout << "\n[2] Testing readData (Array<float>,string) ..." << std::endl;
  try {
    casa::Array<casa::Float> data;
    // Define the colums from which to try reading data
    std::vector<std::string> columns;
    columns.push_back("WEIGHT");
    columns.push_back("SIGMA");
    columns.push_back("WEIGHT_SPECTRUM");
    
    // Read the data from the table columns and display their properties
    for (unsigned int n=0; n<columns.size(); ++n) {
      status = ms.readData (data, columns[n]);
      std::cout << std::setw(15) << columns[n]
		<< std::setw(15) << data.shape()
		<< std::endl;
    }  
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 3: Read data from columns of type double
  */
  
  std::cout << "\n[3] Testing readData (vector<double>,string) ..." << std::endl;
  try {
    std::vector<double> data;
    // Define the colums from which to try reading data
    std::vector<std::string> columns;
    columns.push_back("TIME");
    columns.push_back("UVW");
    columns.push_back("INTERVAL");
    columns.push_back("EXPOSURE");
    
    // Read the data from the table columns and display their properties
    for (unsigned int n=0; n<columns.size(); ++n) {
      status = ms.readData (data, columns[n]);
      std::cout << std::setw(15) << columns[n]
		<< std::setw(15) << data.size()
		<< "   [ " << data[0] << " " << data[1] << " " << data[2]
		<< " "     << data[3] << " " << data[4] << " " << data[5]
		<< " .. ]" 
		<< std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 4: Read data from columns of type float
  */
  
  std::cout << "\n[4] Testing readData (vector<float>,string) ..." << std::endl;
  try {
    std::vector<float> data;
    // Define the colums from which to try reading data
    std::vector<std::string> columns;
    columns.push_back("WEIGHT");
    columns.push_back("SIGMA");
    columns.push_back("WEIGHT_SPECTRUM");
    
    // Read the data from the table columns and display their properties
    for (unsigned int n=0; n<columns.size(); ++n) {
      status = ms.readData (data, columns[n]);
      std::cout << std::setw(15) << columns[n]
		<< std::setw(15) << data.size()
		<< "   [ " << data[0] << " " << data[1] << " " << data[2]
		<< " "     << data[3] << " " << data[4] << " " << data[5]
		<< " .. ]" 
		<< std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

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
  std::string filename = "tMS_Table.ms";

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
  
  // Test for the constructor(s)
  nofFailedTests += test_constructors (filename);
  // Test reading data from MS table columns.
  nofFailedTests += test_readData (filename);

  return nofFailedTests;
}

#else 

int main (int argc, char *argv[])
{
  int nofFailedTests = 0;

  std::cout << "[tMS_Table] Testing MS_Table() ..."      << std::endl;
  std::cout << "-- nof. command line args = " << argc    << std::endl;
  std::cout << "-- Test program name      = " << argv[0] << std::endl;

  try {
    MS_Table ms;
    //
    ms.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  return nofFailedTests;
}

#endif
