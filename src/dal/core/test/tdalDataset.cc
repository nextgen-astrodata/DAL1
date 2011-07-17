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

#include <core/dalData.h>
#include <core/dalDataset.h>
#include <core/IO_Mode.h>

using std::cerr;
using std::cout;
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
  cout << "\n[tdalDataset::test_constructors]" << endl;

  int nofFailedTests  = 0;
  DAL::IO_Mode flags (DAL::IO_Mode::OpenOrCreate);
  std::map<std::string,std::string>::iterator it;

  /*__________________________________________________________________
    Test 1: Default constructor. The created object is not connected 
            to a dataset/-file.
  */

  cout << "\n[1] Testing dalDataset() ..." << endl;
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

  cout << "\n[2] Testing dalDataset(string,string,IO_Mode) ..." << endl;
  try {
    for (it=filenames.begin(); it!=filenames.end(); ++it) {
      // Display which type of dataset is being created
      cout << "--> Creating dataset of type '" << it->first
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

  cout << "\n[3] Testing dalDataset(string,string) ..." << endl;
  try {
    for (it=filenames.begin(); it!=filenames.end(); ++it) {
      // Display which type of dataset is being created
      cout << "--> Opening dataset of type '" << it->first
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
  cout << "\n[tdalDataset::test_open]\n" << endl;

  int nofFailedTests  = 0;

  cout << "--> Opening dataset " << filename << " ..." << endl;

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
int test_attributes (std::map<std::string,std::string> &filenames)
{
  cout << "\n[tdalDataset::test_attributes]\n" << endl;

  int nofFailedTests = 0;
  int numTest        = 0;
  unsigned int nelem = 5;
  std::map<std::string,std::string>::iterator it;

  DAL::IO_Mode flags (DAL::IO_Mode::ReadWrite);
  flags.addFlag(DAL::IO_Mode::Open);
  
  for (it=filenames.begin(); it!=filenames.end(); ++it) {
    // Display which type of dataset is being created
    cout << "--> Opening dataset of type '" << it->first << "' ..." << endl;
    // Create dataset
    DAL::dalDataset dataset (it->second.c_str(),
			     it->first.c_str(),
			     flags);

    /*______________________________________________________
      Test 1: Create/Set attribute storing atomic value.
    */

    ++numTest;

    cout << "[" << numTest << "] Testing setAttribute(string,T) ..." << endl;
    try {
      bool valBool             = false;
      short valShort           = 1;
      unsigned short valUshort = 2;
      int valInt               = 3;
      unsigned int valUint     = 4;
      long valLong             = 5;
      unsigned long valUlong   = 6;
      float valFloat           = 0.5;
      double valDouble         = 0.25;
      std::string valString    = "bla";
      
      std::cout << "-- Start writing attributes." << endl << std::flush;
      dataset.setAttribute ("h5a_bool",    valBool);
      dataset.setAttribute ("h5a_short",   valShort);
      dataset.setAttribute ("h5a_ushort",  valUshort);
      dataset.setAttribute ("h5a_int",     valInt);
      dataset.setAttribute ("h5a_uint",    valUint);
      dataset.setAttribute ("h5a_long",    valLong);
      dataset.setAttribute ("h5a_ulong",   valUlong);
      dataset.setAttribute ("h5a_float",   valFloat);
      dataset.setAttribute ("h5a_double",  valDouble);
      dataset.setAttribute ("h5a_string",  valString);
      std::cout << "-- Finished writing attributes." << endl << std::flush;
    } catch (std::string message) {
      std::cerr << message << endl;
      nofFailedTests++;
    }
    
    /*______________________________________________________
      Test 2: Create/Set attribute storing array-type value
    */
    
    ++numTest;

    cout << "[" << numTest << "] Testing setAttribute(string,T*,uint) ..." << endl;
    try {
      int valBool[]           = {false,true,false,true,false};
      int valInt[]            = {1,1,1,1,1};
      unsigned int valUint[]  = {2,2,2,2,2};
      short valShort[]        = {3,3,3,3,3};
      long valLong[]          = {4,4,4,4,4};
      float valFloat[]        = {0.5,0.5,0.5,0.5,0.5};
      double valDouble[]      = {0.25,0.25,0.25,0.25,0.25};
      std::string valString[] = {"a","bb","ccc","dddd","eeeee"};
      
      std::cout << "-- Start writing attributes." << endl << std::flush;
      dataset.setAttribute ("h5a_array_bool",   valBool,   nelem);
      dataset.setAttribute ("h5a_array_int",    valInt,    nelem);
      dataset.setAttribute ("h5a_array_uint",   valUint,   nelem);
      dataset.setAttribute ("h5a_array_short",  valShort,  nelem);
      dataset.setAttribute ("h5a_array_long",   valLong,   nelem);
      dataset.setAttribute ("h5a_array_float",  valFloat,  nelem);
      dataset.setAttribute ("h5a_array_double", valDouble, nelem);
      dataset.setAttribute ("h5a_array_string", valString, nelem);
      std::cout << "-- Finished writing attributes." << endl << std::flush;
    } catch (std::string message) {
      std::cerr << message << endl;
      nofFailedTests++;
    }
    
  }   // END: for()
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                        test_MS

int test_MS (std::string const &filename,
	     std::string const &tablename="MAIN")
{
  cout << "\n[tdalDataset::test_MS]" << endl;

  int nofFailedTests = 0;

  /*________________________________________________________
    Open MeasurementSet file
  */
  
  cout << "\n[1] Opening MeasurementSet ..." << endl;

  DAL::dalDataset ms (filename);
  ms.summary();

  /*________________________________________________________
    Assign filter to access tables; the additional condition 
    applied to the column selection will cause extraction of
    the auto-correlation products.
  */
  
  cout << "\n[2] Set Filter used when acessing table ..." << endl;

  try {  
    std::string columns           = "UVW, TIME, ANTENNA1, ANTENNA2, DATA";
    std::string filter_conditions = "ANTENNA1 = 1 AND ANTENNA2 = 1";
    
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

  cout << "\n[3] Open table " << tablename << " ..." << endl;

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
  
  cout << "\n[4] Get data from the TIME column of the MAIN table ..." << endl;

  try {
    cout << "--> Opening table ..." << endl;
    DAL::dalTable * table = ms.openTable (tablename);
    //
    cout << "--> Opening table column ..." << endl;
    DAL::dalColumn * column = table->getColumn("TIME");
    //
    unsigned int nofRows = 10;
    if ((column->nofRows())<nofRows) {
      nofRows = column->nofRows();
    }

    /* _____________________________________________________
     * Retrieve column data, version 1
     */

    cout << "--> Retrieve column data, version 1 ..." << endl;

    DAL::dalData * columnData = column->data();
    
    cout << " .. Column data = [";
    for (unsigned int n=0; n<nofRows; ++n) {
      std::cout << " " << *(double*)(columnData->get(n));
    }
    std::cout << " ]" << std::endl;

    /* _____________________________________________________
     * Retrieve column data, version 2
     */

    cout << "--> Retrieve column data, version 2 ..." << endl;
    
    std::vector<double> data;
    column->readData (data);
    //
    cout << " .. Column data = [";
    for (unsigned int n=0; n<nofRows; ++n) {
      std::cout << " " << data[n];
    }
    std::cout << " ]" << std::endl;

    /* _____________________________________________________
     * Retrieve column data, version 3
     */
    
    // cout << "--> Retrieve column data, version 3 ..." << endl;
    
    // {
    //   double * data;
    //   column->readData (data);
    // }

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
  nofFailedTests += test_attributes (filenames);

  if (haveDataset) {

    nofFailedTests += test_open (filename);

    if (filetype.type()==DAL::dalFileType::CASA_MS) {
      nofFailedTests += test_MS (filename);
    }

  }
  
  return nofFailedTests;
}
