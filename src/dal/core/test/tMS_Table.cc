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
using std::cout;
using std::endl;
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
  cout << "\n[tMS_Table::test_constructors]" << endl;

  int nofFailedTests (0);
  
  /*________________________________________________________
    Test 1 : Test default constructor.
  */
  
  cout << "\n[1] Testing MS_Table() ..." << endl;
  try {
    MS_Table ms;
    //
    ms.summary(); 
  } catch (casa::AipsError x) {
    std::cerr << x.getMesg() << endl;
    ++nofFailedTests;
  }
  
  /*________________________________________________________
    Test 2 : Test simple argumented constructor, taking name 
             of the table as input.
  */
  
  cout << "\n[2] Testing MS_Table(string) ..." << endl;
  try {
    MS_Table ms (filename);
    //
    ms.summary(); 
  } catch (casa::AipsError x) {
    std::cerr << x.getMesg() << endl;
    ++nofFailedTests;
  }

  /*________________________________________________________
    Test 3 : Open sub-table contained within another one.
  */
  
  cout << "\n[3] Testing MS_Table(string,string) ..." << endl;
  try {
    std::string subtable = "ANTENNA";
    //
    MS_Table ms (filename, subtable);
    //
    ms.summary(); 
  } catch (casa::AipsError x) {
    std::cerr << x.getMesg() << endl;
    ++nofFailedTests;
  }

  /*________________________________________________________
    Test 4 : Open sub-table contained within another already
             opened casa::Table.
  */
  
  cout << "\n[4] Testing MS_Table(Table,string) ..." << endl;
  try {
    std::string subtable = "OBSERVATION";
    // Open root table
    casa::Table table (filename);
    //
    MS_Table ms (table, subtable);
    //
    ms.summary(); 
  } catch (casa::AipsError x) {
    std::cerr << x.getMesg() << endl;
    ++nofFailedTests;
  }

  return nofFailedTests;
}


//_______________________________________________________________________________
//                                                          test_public_functions

/*!
  \brief Test public member functions

  \param filename -- Name of the MeasurementSet to work with.
  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_public_functions (std::string const &filename)
{
  cout << "\n[tMS_Table::test_public_functions]" << endl;

  int nofFailedTests (0);
  MS_Table ms (filename);

  try {
    std::cout << "-- Class name      = " << ms.className()   << std::endl;
    std::cout << "-- Is root table   = " << ms.isRootTable() << std::endl;
    std::cout << "-- Column names    = " << ms.columnNames() << std::endl;
    std::cout << "-- Sub-table names = " << ms.tableNames()  << std::endl;
    std::cout << "-- Has column DATA = " << ms.hasColumn("DATA") << std::endl;
  } catch (casa::AipsError x) {
    std::cerr << x.getMesg() << endl;
    ++nofFailedTests;
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
  cout << "\n[tMS_Table::test_readData]" << endl;

  int nofFailedTests = 0;
  bool status        = true;

  // Open MS to work with
  MS_Table ms (filename);
  ms.summary(false); 
  
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
  try {
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
  } catch (std::string message) {
    std::cerr << message << endl;
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
  // Run the tests (some of which actually require a dataset)
  
  // Test for the constructor(s)
  nofFailedTests += test_constructors (filename);
  // Test public member functions
  nofFailedTests += test_public_functions (filename);

  if (haveDataset) {
    
    // Test reading data from MS table columns.
    nofFailedTests += test_readData (filename);
    
  }

  return nofFailedTests;
}

#else 

int main (int argc, char *argv[])
{
  int nofFailedTests = 0;

  cout << "[tMS_Table] Testing MS_Table() ..."      << endl;
  cout << "-- nof. command line args = " << argc    << endl;
  cout << "-- Test program name      = " << argv[0] << endl;

  try {
    MS_Table ms;
    //
    ms.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  return nofFailedTests;
}

#endif
