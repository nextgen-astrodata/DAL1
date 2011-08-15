/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#include <data_hl/MS_Dataset.h>

// Namespace usage
using DAL::MS_Dataset;

/*!
  \file tMS_Dataset.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::MS_Dataset class
 
  \author Lars B&auml;hren
 
  \date 2011-08-11
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new MS_Dataset object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename)
{
  std::cout << "\n[tMS_Dataset::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  /*________________________________________________________
    Test 1 : Default constructor.
  */
  
  std::cout << "[1] Testing MS_Dataset() ..." << std::endl;
  try {
    MS_Dataset ms;
    //
    ms.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 2 : Argumented constructor to open existing MS.
  */
  
  std::cout << "[2] Testing MS_Dataset(string) ..." << std::endl;
  try {
    MS_Dataset ms (filename);
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
  cout << "\n[tMS_Dataset::test_readData]" << endl;

  int nofFailedTests = 0;
  bool status        = true;

  // Open MS to work with
  MS_Dataset ms (filename);
  
  /*________________________________________________________
    Test 1: Read data from columns of type double
  */
  
  cout << "\n[1] Testing readData (Array<double>,string) ..." << endl;
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
      cout << std::setw(15) << columns[n]
		<< std::setw(15) << data.shape()
		<< endl;
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 2: Read data from columns of type double for a
            selected number of rows.
  */
  
  cout << "\n[2] Testing readData (Array<double>,string,uint,uint) ..." << endl;
  try {
    unsigned int start  = 0;
    unsigned int length = 10;
    casa::Array<casa::Double> data;
    // Define the colums from which to try reading data
    std::vector<std::string> columns;
    columns.push_back("TIME");
    columns.push_back("UVW");
    columns.push_back("INTERVAL");
    columns.push_back("EXPOSURE");
    
    // Read the data from the table columns and display their properties
    for (unsigned int n=0; n<columns.size(); ++n) {
      status = ms.readData (data, columns[n], start, length);
      cout << std::setw(15) << columns[n]
		<< std::setw(15) << data.shape()
		<< endl;
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 3: Read data from columns of type float.
  */
  
  cout << "\n[3] Testing readData (Array<float>,string) ..." << endl;
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
      cout << std::setw(15) << columns[n]
		<< std::setw(15) << data.shape()
		<< endl;
    }  
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 4: Read data from columns of type float for a
            selected number of rows.
  */
  
  cout << "\n[4] Testing readData (Array<float>,string,uint,uint) ..." << endl;
  try {
    unsigned int start  = 0;
    unsigned int length = 10;
    casa::Array<casa::Float> data;
    // Define the colums from which to try reading data
    std::vector<std::string> columns;
    columns.push_back("WEIGHT");
    columns.push_back("SIGMA");
    columns.push_back("WEIGHT_SPECTRUM");
    
    // Read the data from the table columns and display their properties
    for (unsigned int n=0; n<columns.size(); ++n) {
      status = ms.readData (data, columns[n], start, length);
      cout << std::setw(15) << columns[n]
		<< std::setw(15) << data.shape()
		<< endl;
    }  
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 5: Read data from columns of type double
  */
  
  cout << "\n[5] Testing readData (vector<double>,string) ..." << endl;
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
      if (ms.readData (data, columns[n])) {
	cout << std::setw(15) << columns[n]
	     << std::setw(15) << data.size()
	     << "   [ " << data[0] << " " << data[1] << " " << data[2]
	     << " "     << data[3] << " " << data[4] << " " << data[5]
	     << " .. ]" 
	     << endl;
      }
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 6: Read data from columns of type double from a 
            selected number of rows.
  */
  
  cout << "\n[6] Testing readData (vector<double>,string,uint,uint) ..." << endl;
  try {
    unsigned int start  = 0;
    unsigned int length = 10;
    std::vector<double> data;
    // Define the colums from which to try reading data
    std::vector<std::string> columns;
    columns.push_back("TIME");
    columns.push_back("UVW");
    columns.push_back("INTERVAL");
    columns.push_back("EXPOSURE");
    
    // Read the data from the table columns and display their properties
    for (unsigned int n=0; n<columns.size(); ++n) {
      if (ms.readData (data, columns[n], start, length)) {
	cout << std::setw(15) << columns[n]
	     << std::setw(15) << data.size()
	     << "   [ " << data[0] << " " << data[1] << " " << data[2]
	     << " "     << data[3] << " " << data[4] << " " << data[5]
	     << " .. ]" 
	     << endl;
      }
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 7: Read data from columns of type float
  */
  
  cout << "\n[7] Testing readData (vector<float>,string) ..." << endl;
  try {
    std::vector<float> data;
    // Define the colums from which to try reading data
    std::vector<std::string> columns;
    columns.push_back("WEIGHT");
    columns.push_back("SIGMA");
    columns.push_back("WEIGHT_SPECTRUM");
    
    // Read the data from the table columns and display their properties
    for (unsigned int n=0; n<columns.size(); ++n) {
      if ( ms.readData (data, columns[n]) ) {
	cout << std::setw(15) << columns[n]
	     << std::setw(15) << data.size()
	     << "   [ " << data[0] << " " << data[1] << " " << data[2]
	     << " "     << data[3] << " " << data[4] << " " << data[5]
	     << " .. ]" 
	     << endl;
      }
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*________________________________________________________
    Test 8: Read data from columns of type float for a 
            selected number of rows.
  */
  
  cout << "\n[8] Testing readData (vector<float>,string,uint,uint) ..." << endl;
  try {
    unsigned int start  = 0;
    unsigned int length = 10;
    std::vector<float> data;
    // Define the colums from which to try reading data
    std::vector<std::string> columns;
    columns.push_back("WEIGHT");
    columns.push_back("SIGMA");
    columns.push_back("WEIGHT_SPECTRUM");
    
    // Read the data from the table columns and display their properties
    for (unsigned int n=0; n<columns.size(); ++n) {
      if ( ms.readData (data, columns[n], start, length) ) {
	cout << std::setw(15) << columns[n]
	     << std::setw(15) << data.size()
	     << "   [ " << data[0] << " " << data[1] << " " << data[2]
	     << " "     << data[3] << " " << data[4] << " " << data[5]
	     << " .. ]" 
	     << endl;
      }
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*________________________________________________________
    Test 9: Read data from columns of type double for a
            sequence of rows.
  */
  
  cout << "\n[9] Testing sequential reading from UVW column ..." << endl;
  try {
    unsigned int start   = 0;
    unsigned int nofRows = 10;
    std::string column   = "UVW";
    std::vector<double> data;

    if (ms.hasColumn(column)) {
      // Read the data from the table columns and display their properties
      for (unsigned int n=0; n<nofRows; ++n) {
	// read the data from the current start position onwards
	status = ms.readData (data, column, start);
	// display properties
	cout << std::setw(15) << column
	     << std::setw(10) << start
	     << std::setw(10) << data.size()
	     << "   [ " << data[0] << " " << data[1] << " " << data[2] << " ]" 
	     << endl;
	// increment start position
	++start;
      }
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 10: Read data from columns of type double for a
            sequence of rows.
  */
  
  cout << "\n[10] Testing sequential reading from DATA column ..." << endl;
  {
    unsigned int start   = 0;
    unsigned int nofRows = 10;
    std::string column   = "DATA";
    std::vector<std::complex<float> > data;
    
    if (ms.hasColumn(column)) {
      // Read the data from the table columns and display their properties
      for (unsigned int n=0; n<nofRows; ++n) {
	// read the data from the current start position onwards
	status = ms.readData (data, column, start);
	// display properties
	cout << std::setw(15) << column
	     << std::setw(10) << start
	     << std::setw(10) << data.size()
	     << "   [ " << data[0] << " " << data[1]
	     << " " << data[2] << " " << data[3] << " ]" 
	     << endl;
	// increment start position
	++start;
      }
    }
  }
  
  /*________________________________________________________
    Test 11 : Test high-level interface to read data from 
              'TIME' column.
  */

  cout << "\n[11] Testing timeValues(vector<double>) ..." << endl;
  {
    std::vector<double> data;

    if (ms.timeValues(data)) {
      cout << "-- TIME = [ " << data[0] << " " << data[1] << " .. "
	   << data[data.size()-1] << " ]" << endl;
    } else {
      std::cerr << "Failed to read TIME data!" << std::endl;
    }
  }
  
  /*________________________________________________________
    Test 11 : Test high-level interface to read data from 
              'EXPOSURE' column.
  */

  cout << "\n[12] Testing exposureValues(vector<double>) ..." << endl;
  {
    std::vector<double> data;

    if (ms.exposureValues(data)) {
      cout << "-- EXPOSURE = [ " << data[0] << " " << data[1] << " .. "
	   << data[data.size()-1] << " ]" << endl;
    } else {
      std::cerr << "Failed to read EXPOSURE data!" << std::endl;
    }
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
  std::string filename = "tMS_Dataset.ms";
  
  //________________________________________________________
  // Process parameters from the command line
  
  if (argc > 1) {
    filename    = std::string(argv[1]);
    haveDataset = true;
  }

  //________________________________________________________
  // Run the tests
  
  if (haveDataset) {
    // Test for the constructor(s)
    nofFailedTests += test_constructors (filename);
    // Test reading data from MS table columns.
    nofFailedTests += test_readData (filename);
  }
  
  return nofFailedTests;
}
