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

#include <complex>

// Namespace usage
using std::cerr;
using std::cout;
using std::endl;
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
  cout << "\n[tMS_Dataset::test_constructors]\n" << endl;

  int nofFailedTests = 0;
  
  /*________________________________________________________
    Test 1 : Default constructor.
  */
  
  cout << "[1] Testing MS_Dataset() ..." << endl;
  try {
    MS_Dataset ms;
    //
    ms.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 2 : Argumented constructor to open existing MS.
  */
  
  cout << "[2] Testing MS_Dataset(string) ..." << endl;
  try {
    MS_Dataset ms (filename);
    //
    ms.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
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
#ifdef DAL_WITH_CASA
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
    cerr << message << endl;
    nofFailedTests++;
  }
#endif
  
  /*________________________________________________________
    Test 2: Read data from columns of type double for a
            selected number of rows.
  */
  
  cout << "\n[2] Testing readData (Array<double>,string,uint,uint) ..." << endl;
#ifdef DAL_WITH_CASA
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
    cerr << message << endl;
    nofFailedTests++;
  }
#endif
  
  /*________________________________________________________
    Test 3: Read data from columns of type float.
  */
  
  cout << "\n[3] Testing readData (Array<float>,string) ..." << endl;
#ifdef DAL_WITH_CASA
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
    cerr << message << endl;
    nofFailedTests++;
  }
#endif
  
  /*________________________________________________________
    Test 4: Read data from columns of type float for a
            selected number of rows.
  */
  
  cout << "\n[4] Testing readData (Array<float>,string,uint,uint) ..." << endl;
#ifdef DAL_WITH_CASA
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
    cerr << message << endl;
    nofFailedTests++;
  }
#endif
  
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
    cerr << message << endl;
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
    cerr << message << endl;
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
    cerr << message << endl;
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
    cerr << message << endl;
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
    cerr << message << endl;
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
      cerr << "Failed to read TIME data!" << endl;
    }
  }
  
  /*________________________________________________________
    Test 12 : Test high-level interface to read data from 
              'EXPOSURE' column.
  */

  cout << "\n[13] Testing exposureValues(vector<double>) ..." << endl;
  {
    std::vector<double> data;

    if (ms.exposureValues(data)) {
      cout << "-- EXPOSURE = [ " << data[0] << " " << data[1] << " .. "
	   << data[data.size()-1] << " ]" << endl;
    } else {
      cerr << "Failed to read EXPOSURE data!" << endl;
    }
  }
  
  /*________________________________________________________
    Test 13 : Test high-level interface to read data from
              the 'FREQ_CHAN' column of the 'SPECTRAL_WINDOWS'
	      table
  */

  cout << "\n[13] Testing channelFrequencyValues(vector<double>) ..." << endl;
  {
    std::vector<double> data;

    if (ms.channelFrequencyValues(data)) {
      cout << "-- nof. channels     = " << data.size() << endl;
      cout << "-- Frequncy channels = [ " << data[0] << " " << data[1] << " .. "
	   << data[data.size()-1] << " ]" << endl;
    } else {
      cerr << "Failed to read FREQ_CHAN data!" << endl;
    }
  }
  
  /*________________________________________________________
    Test 14 : Test high-level interface to read data from
              the 'FREQ_WIDTH' column of the 'SPECTRAL_WINDOWS'
	      table
  */

  cout << "\n[14] Testing channelFrequencyValues(vector<double>) ..." << endl;
  {
    std::vector<double> data;

    if (ms.channelWidthValues(data)) {
      cout << "-- nof. channels     = " << data.size() << endl;
      cout << "-- Frequncy channels = [ " << data[0] << " " << data[1] << " .. "
	   << data[data.size()-1] << " ]" << endl;
    } else {
      cerr << "Failed to read FREQ_WIDTH data!" << endl;
    }
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                 test_selection

/*!
  \brief Test applying selections to the table data

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_selection (std::string const &filename)
{
  cout << "\n[tMS_Dataset::test_selection]" << endl;

  int nofFailedTests = 0;

  /* Open MeasurementSet to work with */
  DAL::MS_Dataset ms (filename);
  ms.summary();

  /*________________________________________________________
    Test 0 : Perform selection using basic casacore table
             routines.
  */
  
  cout << "\n[0] Direct selection on table ..." << endl;
#ifdef DAL_WITH_CASA
  try {
    // Get all entries in the UVW column
    {
      casa::Table tab (filename);
      casa::ROArrayColumn<double> col (tab, "UVW");
      casa::Array<double> data = col.getColumn();
      std::cout << "-- ANTENNA1 (all)           = " << data.shape() << std::endl;
    }
    
    // Get all entries in the ANTENNA1 column where ANTENNA1=1
    {
      casa::Table tab (filename);
      casa::Table selection = tab(tab.col("ANTENNA1") == 1);
      casa::ROArrayColumn<double> col (selection, "UVW");
      casa::Array<double> data = col.getColumn();
      std::cout << "-- ANTENNA1=1               = " << data.shape() << std::endl;
    }
    
    // Get all entries in the ANTENNA1 column where ANTENNA1=ANTENNA2=1
    {
      // open table
      casa::Table tab (filename);
      // define selection
      casa::TableExprNode exprNode1 (tab.col("ANTENNA1") == 1);
      casa::TableExprNode exprNode2 (tab.col("ANTENNA2") == 1);
      // apply selection
      casa::Table selection = tab(exprNode1&&exprNode2);
      // open column
      casa::ROArrayColumn<double> col (selection, "UVW");
      casa::Array<double> data = col.getColumn();
      std::cout << "-- ANTENNA1=1 && ANTENNA2=1 = " << data.shape() << std::endl;
    }
    
    // Get all entries in the ANTENNA1 column where ANTENNA1=1, ANTENNA2=2
    {
      // open table
      casa::Table tab (filename);
      // define selection
      casa::TableExprNode exprNode1 (tab.col("ANTENNA1") == 1);
      casa::TableExprNode exprNode2 (tab.col("ANTENNA2") == 2);
      // apply selection
      casa::Table selection = tab(exprNode1&&exprNode2);
      // open column
      casa::ROArrayColumn<double> col (selection, "UVW");
      casa::Array<double> data = col.getColumn();
      std::cout << "-- ANTENNA1=1 && ANTENNA2=2 = " << data.shape() << std::endl;
    }

  } catch (casa::AipsError x) {
    cerr << x.getMesg() << endl;
    ++nofFailedTests;
  }
#endif
  
  /*________________________________________________________
    Test 1 : Read UVW data without any selection applied to
             to the table.
  */
  
  cout << "\n[1] Read UVW data without any selection ..." << endl;
  {
    std::vector<int> data;
    // read baseline values
    ms.readData (data, "ANTENNA1");
    cout << "-- shape(ANTENNA1) = " << data.size() << endl;
  }
  
  /*________________________________________________________
    Test 2 : Read UVW data for a specific antenna.
  */
  
  cout << "\n[2] Read UVW data for specific antenna ..." << endl;
  {
    std::vector<int> data;
    // select individual antenna
    ms.selectAntenna (1);
    // read baseline values
    ms.readData (data, "ANTENNA1");
    cout << "-- shape(ANTENNA1) = " << data.size() << endl;
  }
  
  /*________________________________________________________
    Test 3 : Read UVW data for baselines including a
             specific antenna.
  */
  
  cout << "\n[3] Read UVW data baselines including specific antenna ..." << endl;
  {
    std::vector<int> data;
    // select individual antenna
    ms.selectBaseline (1);
    // read baseline values
    ms.readData (data, "ANTENNA1");
    cout << "-- shape(ANTENNA1) = " << data.size() << endl;
  }
  
  /*________________________________________________________
    Test 4 : Read UVW data for baseline between two specific
             antennae.
  */
  
  cout << "\n[3] Read UVW data baselines including specific antenna ..." << endl;
  {
    int antenna1 = 1;
    int antenna2 = 2;
    std::vector<int> data;
    // select individual antenna
    ms.selectBaseline (antenna1, antenna2);
    // read baseline values
    ms.readData (data, "ANTENNA1");
    cout << "-- shape(ANTENNA1) = " << data.size() << endl;
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
    // Test applying selections to the table data
    nofFailedTests += test_selection (filename);
  } else {
    cerr << "[tMS_Dataset] No dataset provided - skipping tests!" << endl;
  }
  
  return nofFailedTests;
}
