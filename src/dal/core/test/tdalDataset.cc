/***************************************************************************
 *   Copyright (C) 2008-2011                                               *
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

/*!
  \file tdalDataset.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::dalDataset class

  \author Lars B&auml;hren

  \date 2008/09/21
*/

#include <core/dalDataset.h>
#include <core/IO_Mode.h>

using std::endl;

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test the various constructors for an object of type DAL::dalDataset

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::map<std::string,std::string> &filenames)
{
  std::cout << "\n[tdalDataset::test_constructors]" << endl;

  int nofFailedTests  = 0;
  DAL::IO_Mode flags (DAL::IO_Mode::OpenOrCreate);
  std::map<std::string,std::string>::iterator it;

  /*__________________________________________________________________
    Test 1: Default constructor. The created object is not connected 
            to a dataset/-file.
  */

  std::cout << "\n[1] Testing dalDataset() ..." << endl;
  try {
    DAL::dalDataset dataset;
    dataset.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*__________________________________________________________________
    Test 2: Argumented constructor, proving the necessary set of 
            parameters for the creation of a dataset/-file.
  */

  std::cout << "\n[2] Testing dalDataset(string,string,IO_Mode) ..." << endl;
  try {
    for (it=filenames.begin(); it!=filenames.end(); ++it) {
      // Display which type of dataset is being created
      std::cout << "--> Creating dataset of type '" << it->first
		<< "' ..." << endl;
      // Create dataset
      DAL::dalDataset dataset (it->second.c_str(),
			       it->first.c_str(),
			       flags);
      dataset.summary();
    }
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*__________________________________________________________________
    Test 3: Argumented constructor, proving the necessary set of 
            parameters for the creation of a dataset/-file.
  */

  std::cout << "\n[3] Testing dalDataset(string,string) ..." << endl;
  try {
    for (it=filenames.begin(); it!=filenames.end(); ++it) {
      // Display which type of dataset is being created
      std::cout << "--> Opening dataset of type '" << it->first
		<< "' ..." << endl;
      // Create dataset
      DAL::dalDataset dataset (it->second.c_str(),
			       it->first.c_str());
      dataset.summary();
    }
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_open

/*!
  \param filename -- Name of the dataset to open.
  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_open (std::string const &filename)
{
  std::cout << "\n[tdalDataset::test_open]\n" << endl;

  int nofFailedTests  = 0;

  std::cout << "--> Opening dataset " << filename << " ..." << std::endl;

  DAL::dalDataset dataset (filename);
  dataset.summary();

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test creation of and access to attributes

  \brief filename -- Name of the dataset to be created and used for the test
*/
int test_attributes (std::string const &filename)
{
  std::cout << "\n[tdalDataset::test_attributes]\n" << endl;

  int nofFailedTests = 0;
  unsigned int nelem = 5;
  
  /* Open the dataset to work with */
  DAL::dalDataset ds (filename);
  ds.summary();

  /*__________________________________________________________________
    Test 1: Integer-type attributes
  */

  cout << "[1] Integer-type attributes ..." << endl;
  try {
    int var        = 1;
    int varArray[] = {1,2,3,4,5};
    
    ds.setAttribute ("h5a_int", var);
    ds.setAttribute ("h5a_array_int", varArray, nelem);
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*__________________________________________________________________
    Test 2: Float-type attributes
  */

  // cout << "[2] Float-type attributes ..." << endl;
  // try {
  //   float var        = 0.1;
  //   float varArray[] = {0.1, 0.2, 0.3, 0.4, 0.5};

  //   ds.setAttribute ("h5a_float", var);
  //   ds.setAttribute ("h5a_array_float", varArray, nelem);
  // } catch (std::string message) {
  //   std::cerr << message << endl;
  //   nofFailedTests++;
  // }
  
  /*__________________________________________________________________
    Test 3: Double-type attributes
  */

  // cout << "[3] Double-type attributes ..." << endl;
  // try {
  //   double var        = 0.1;
  //   double varArray[] = {0.1, 0.2, 0.3, 0.4, 0.5};

  //   ds.setAttribute ("ATTRIBUTE_DOUBLE", var);
  //   ds.setAttribute ("ATTRIBUTE_DOUBLE_ARRAY", varArray, nelem);
  // } catch (std::string message) {
  //   std::cerr << message << endl;
  //   nofFailedTests++;
  // }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                        test_MS

int test_MS (std::string const &filename,
	     std::string const &tablename="MAIN")
{
  std::cout << "\n[tdalDataset::test_MS]\n" << endl;

  int nofFailedTests = 0;

  /*________________________________________________________
    Open MeasurementSet file
  */
  
  std::cout << "[1] Opening MeasurementSet ..." << endl;

  DAL::dalDataset ms (filename);
  ms.summary();

  /*________________________________________________________
    Assign filter to access tables; the additional condition 
    applied to the column selection will cause extraction of
    the auto-correlation products.
  */
  
  std::cout << "[2] Set Filter used when acessing table ..." << endl;

  try {  
    string columns      = "UVW, TIME, ANTENNA1, ANTENNA2, DATA";
    string filter_conditions = "ANTENNA1 = 1 AND ANTENNA2 = 1";
    
    ms.setFilter (columns, filter_conditions);
    ms.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*________________________________________________________
    Open a specific table in the MeasurementSet; by default 
    this will be the 'MAIN' table at the root lavel of the 
    data set.
  */

  std::cout << "[3] Open table " << tablename << " ..." << endl;

  try {
    DAL::dalTable * table = ms.openTable (tablename);
    table->summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*________________________________________________________
    Get data from the TIME column of the MAIN table.
  */
  
  std::cout << "[4] Get data from the TIME column of the MAIN table ..." << endl;

  try {
    std::cout << "--> Opening table ..." << std::endl;
    DAL::dalTable * table = ms.openTable (tablename);
    //
    std::cout << "--> Opening table column ..." << std::endl;
    DAL::dalColumn * columTime = table->getColumn("TIME");
    // 
    columTime->summary();
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
  std::string filename = "tdalDataset.h5";
  DAL::dalFileType filetype;

  std::map<std::string,std::string> filenames;
  filenames["HDF5"]    = "tdalDataset.h5";
  filenames["FITS"]    = "tdalDataset.fits";
  filenames["CASA_MS"] = "tdalDataset.ms";
  
  //________________________________________________________
  // Process parameters from the command line
  
  if (argc > 1) {
    filename    = std::string(argv[1]);
    haveDataset = true;
  }

  if (argc > 2) {
    filetype.setType(std::string(argv[2]));
  }
  
  //________________________________________________________
  // Run the tests
  
  //! Test constructors for dalDataset object
  nofFailedTests += test_constructors(filenames);
  //! Test creation of and access to attributes
  // nofFailedTests += test_attributes (filename);

  if (haveDataset) {
    nofFailedTests += test_open (filename);
  }
  
  return nofFailedTests;
}
