/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#include <casa/Arrays/ArrayIO.h>
#include <casa/HDF5/HDF5File.h>
#include <casa/HDF5/HDF5Record.h>

#include <TBB_Timeseries.h>

using std::cerr;
using std::cout;
using std::endl;
using DAL::TBB_Timeseries;  // Namespace usage

/*!
  \file tTBB_Timeseries.cpp

  \ingroup DAL

  \brief A collection of test routines for the TBB_Timeseries class
 
  \author Lars B&auml;hren
 
  \date 2008/02/06
*/

// -----------------------------------------------------------------------------

/*!
  \brief Test constructors for a new TBB_Timeseries object

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_construction (std::string const &filename)
{
  std::cout << "\n[test_construction]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    TBB_Timeseries newTBB_Timeseries;
    //
    newTBB_Timeseries.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing argumented constructor ..." << std::endl;
  try {
    TBB_Timeseries newTBB_Timeseries (filename);
    //
    newTBB_Timeseries.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Testing copy constructor ..." << std::endl;
  try {
    std::cout << "--> creating original object ..." << std::endl;
    TBB_Timeseries timeseries (filename);
    timeseries.summary(); 
    //
    std::cout << "--> creating new object by copy ..." << std::endl;
    TBB_Timeseries timeseriesCopy (timeseries);
    timeseriesCopy.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test the various methods of the class

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_methods (std::string const &filename)
{
  std::cout << "\n[test_methods]\n" << std::endl;

  int nofFailedTests = 0;
  TBB_Timeseries timeseries (filename);

  cout << "[1] Retrieving sample frequencies ..." << endl;
  try {
#ifdef HAVE_CASA
    casa::Vector<double> sampleFreq = timeseries.sample_frequencies();
    cout << "-- nof. values        = " << sampleFreq.nelements() << endl;
    cout << "-- Sample frequencies = " << sampleFreq << endl;
#else 
    std::vector<double> sampleFreq = timeseries.sample_frequencies();
    cout << "-- nof. values        = " << sampleFreq.size() << endl;
#endif
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Retrieve recording times ..." << endl;
  try {
#ifdef HAVE_CASA
    casa::Vector<uint> times = timeseries.times();
    cout << "-- nof. values      = " << times.nelements() << endl;
    cout << "-- Recording times  = " << times << endl;
#else 
    std::vector<uint> times = timeseries.times();
    cout << "-- nof. values      = " << times.size() << endl;
#endif
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test export of the attribute values to a casa::Record object
  
  \param filename -- Name of the HDF5 file, within which the dataset is located

  \return nofFailedTests -- The number of failed tests.
*/
int test_attributes2record (string const &filename)
{
  std::cout << "\n[test_attributes2record]\n" << std::endl;

  int nofFailedTests (0);
  TBB_Timeseries ts (filename);

  std::cout << "[1] Retreiving attributes of group into record ..." << std::endl;
  try {
    // retrieve attributes into record
    casa::Record rec = ts.attributes2record ();
    // Create HDF5 file and write the record to it
    casa::HDF5File file("tTimeseries_1.h5", casa::ByteIO::New);
    casa::HDF5Record::writeRecord (file, "Timeseries", rec);
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[2] Retreiving attributes of group into record (recursive) ..."
	    << std::endl;
  try {
    // retrieve attributes into record
    casa::Record rec = ts.attributes2record (true);
    // Create HDF5 file and write the record to it
    casa::HDF5File file("tTimeseries_2.h5", casa::ByteIO::New);
    casa::HDF5Record::writeRecord (file, "Timeseries", rec);
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[3] Fill dataset information into a record ..." << std::endl;
  try {
    casa::Record rec;
    //
    std::cout << "-- TELESCOPE" << std::endl;
    rec.define("TELESCOPE",ts.telescope());
    //
    std::cout << "-- OBSERVER" << std::endl;
    rec.define("OBSERVER",ts.observer());
    //
    std::cout << "-- PROJECT" << std::endl;
    rec.define("PROJECT",ts.project());
    //
    std::cout << "-- CHANNEL_NAME" << std::endl;
    rec.define("CHANNEL_NAME",ts.channelNames());
    //
    std::cout << "-- CHANNEL_ID" << std::endl;
    rec.define("CHANNEL_ID",ts.channelID());
    //
    std::cout << "-- TIME" << std::endl;
    rec.define("TIME",ts.times());
    //
    std::cout << "-- DATA_LENGTH" << std::endl;
    rec.define("DATA_LENGTH",ts.data_lengths());
    // Create HDF5 file and write the record to it
    casa::HDF5File file("tTimeseries_3.h5", casa::ByteIO::New);
    casa::HDF5Record::writeRecord (file, "Timeseries", rec);
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[4] Test operations on parameters " << std::endl;
  try {
    uint min_times      = min(ts.times());
    uint min_dataLength = min(ts.data_lengths());
    //
    std::cout << "-- min(times)       = " << min_times      << std::endl;
    std::cout << "-- min(data_length) = " << min_dataLength << std::endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test retrieval of TBB data

  \param filename -- Name of the HDF5 file used for testing

  \return nofFailedTests -- The number of failed tests.
*/
int test_data (std::string const &filename)
{
  std::cout << "\n[test_data]\n" << std::endl;

  int nofFailedTests = 0;
  int start          = 0;
  int nofSamples     = 1024;
  TBB_Timeseries timeseries (filename);

  std::cout << "[1] Retrieve time-series data without channel selection"
	    << std::endl;
  try {
    casa::Matrix<double> data = timeseries.fx (start,
					       nofSamples);
    // feedback 
    std::cout << "-- Data start     = " << start        << std::endl;
    std::cout << "-- Data blocksize = " << nofSamples   << std::endl;
    std::cout << "-- Data array     = " << data.shape() << std::endl;
    std::cout << "-- Data [0,]      = " << data.row(0)  << std::endl;
    std::cout << "-- Data [1,]      = " << data.row(1)  << std::endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main (int argc,
	  char *argv[])
{
  int nofFailedTests (0);
  
  /*
    Check if filename of the dataset is provided on the command line; if not
    exit the program.
  */
  if (argc < 2) {
    std::cerr << "[tTBB_StationGroup] Too few parameters!" << endl;
    std::cerr << "" << endl;
    std::cerr << "  tTBB_StationGroup <filename>" << endl;
    std::cerr << "" << endl;
    return -1;
  }

  std::string filename = argv[1];

  // Test for the constructor(s)
  nofFailedTests += test_construction (filename);

  if (nofFailedTests == 0) {
    nofFailedTests += test_methods (filename);
    nofFailedTests += test_attributes2record (filename);
    nofFailedTests += test_data (filename);
  } else {
    std::cerr << "[tTBB_StationGroup]"
	      << " Skipping tests after testing constructors returned errors!"
	      << std::endl;
  }
  
  return nofFailedTests;
}
