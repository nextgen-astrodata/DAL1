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
    std::string subtable = "SPECTRAL_WINDOW";
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

  /*________________________________________________________
    Test 5 : Test copy constructor.
  */
  
  cout << "\n[5] Testing MS_Table(MS_Table) ..." << endl;
  try {
    // create original object
    MS_Table ms (filename);
    // create copy
    MS_Table msCopy (ms);
    //
    msCopy.summary(); 
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
    std::cout << "-- Class name         = " << ms.className()       << std::endl;
    std::cout << "-- Is root table      = " << ms.isRootTable()     << std::endl;
    std::cout << "-- Column names       = " << ms.columnNames()     << std::endl;
    std::cout << "-- Sub-table names    = " << ms.tableNames()      << std::endl;
    std::cout << "-- Has column DATA    = " << ms.hasColumn("DATA") << std::endl;
    std::cout << "-- Has sub-table FEED = " << ms.hasTable("FEED")  << std::endl;
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
//                                                           test_expressionNodes

/*!
  \brief Test usage of table expression nodes to select table entries.

  \param filename -- Name of the MeasurementSet to work with.
  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_expressionNodes (std::string const &filename)
{
  cout << "\n[tMS_Table::test_expressionNodes]" << endl;

  int nofFailedTests = 0;

  /*________________________________________________________
    Test 1 : Get all values from within the 'TIME' and 'UVW'
             column
  */

  cout << "\n[1] Get all values of 'TIME' and 'UVW' column ..." << endl;
  try {
    /* Open the table to work with */
    casa::Table ms (filename);
    // TIME
    casa::ROScalarColumn<casa::Double> timeColumn (ms, "TIME");
    casa::Vector<casa::Double> timeValues = timeColumn.getColumn();
    // UVW
    casa::ROArrayColumn<casa::Double> uvwColumn (ms, "UVW");
    casa::Array<casa::Double> uvwValues = uvwColumn.getColumn();
    // show results
    cout << "-- shape(TIME) = " << timeValues.shape() << endl;
    cout << "-- shape(UVW)  = " << uvwValues.shape()  << endl;
  } catch (casa::AipsError x) {
    std::cerr << x.getMesg() << std::endl;
    ++nofFailedTests;
  }

  /*________________________________________________________
    Test 2 : Get values from 'TIME' and 'UVW' columns for
             selected antenna.
  */

  cout << "\n[2] Get 'TIME' and 'UVW' values for selected antenna ..." << endl;
  try {
    /* Open the table to work with */
    casa::Table ms (filename);
    // Define selection
    casa::TableExprNode exprNode (ms.col("ANTENNA1") == 1);
    // Apply selection to table
    casa::Table selection = ms (exprNode);
    // TIME
    casa::ROScalarColumn<casa::Double> timeColumn (selection, "TIME");
    casa::Vector<casa::Double> timeValues = timeColumn.getColumn();
    // UVW
    casa::ROArrayColumn<casa::Double> uvwColumn (selection, "UVW");
    casa::Array<casa::Double> uvwValues = uvwColumn.getColumn();
    // show results
    cout << "-- shape(TIME) = " << timeValues.shape() << endl;
    cout << "-- shape(UVW)  = " << uvwValues.shape()  << endl;
  } catch (casa::AipsError x) {
    std::cerr << x.getMesg() << std::endl;
    ++nofFailedTests;
  }

  /*________________________________________________________
    Test 3 : Combine multiple table expression nodes.
  */

  cout << "\n[3] Test combining expression nodes ..." << endl;
  try {
    /* Open the table to work with */
    casa::Table ms (filename);

    /* Selections on table */
    casa::TableExprNode ant1 (ms.col("ANTENNA1") == 1);
    casa::TableExprNode ant2 (ms.col("ANTENNA2") == 2);
    casa::TableExprNode time1 (ms.col("TIME") > 0);
    casa::TableExprNode time2 (ms.col("TIME") < 100);

    cout << "-- Individual expression nodes:" << std::endl;
    cout << "--> isNull(ant1)  = " << ant1.isNull()  << endl;
    cout << "--> isNull(ant2)  = " << ant2.isNull()  << endl;
    cout << "--> isNull(time1) = " << time1.isNull() << endl;
    cout << "--> isNull(time2) = " << time2.isNull() << endl;

    // Combine antenna selections
    casa::TableExprNode selAntenna (ant1 && ant2);

    // Combine time selections
    casa::TableExprNode selTime (time1 && time2);

    // Concatenate selections
    casa::TableExprNode sel = ant1;
    cout << "--> isNull(sel) = " << sel.isNull() << endl;
    sel && ant2;
    cout << "--> isNull(sel) = " << sel.isNull() << endl;
    sel && time1;
    cout << "--> isNull(sel) = " << sel.isNull() << endl;

  } catch (casa::AipsError x) {
    std::cerr << x.getMesg() << std::endl;
    ++nofFailedTests;
  }

  /*________________________________________________________
    Test 4 : Read data from table with active selection.
  */

  cout << "\n[4] Read data from table with active selection ..." << endl;
  try {
    DAL::MS_Table tab (filename);
    std::string column = "ANTENNA1";
    int antenna        = 1;
    // Apply selection to table
    tab.setSelection(column, DAL::Operator::Equal, antenna);
    /* Read data from table selection */
    std::vector<double> data;
    tab.readData (data, "TIME");
    std::cout << "-- shape(TIME) = " << data.size() << std::endl;
  } catch (casa::AipsError x) {
    std::cerr << x.getMesg() << std::endl;
    ++nofFailedTests;
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
    // Test working with table expression nodes
    nofFailedTests += test_expressionNodes (filename);
    
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
