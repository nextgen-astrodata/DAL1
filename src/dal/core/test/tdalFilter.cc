/***************************************************************************
 *   Copyright (C) 2010 by                                                 *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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
  \file tdalFilter.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::dalFilter class

  \author Lars B&auml;hren

  \date 2010/03/11
*/

#include <core/dalFilter.h>

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test the various constructors for an object of type DAL::dalFilter

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors ()
{
  std::cout << "\n[tdalDataset::test_constructors]" << std::endl;

  int nofFailedTests     = 0;
  std::string columns    = "DATA";
  std::string conditions = "ANTENNA1 = 1 AND ANTENNA2 = 1";

  /*__________________________________________________________________
    Test 1: Default constructor
  */
  
  std::cout << "\n[1] Testing dalFilter() ..." << std::endl;
  try {
    DAL::dalFilter filter;
    //
    filter.summary();
  }
  catch (std::string& message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  /*__________________________________________________________________
    Test 2: Test argumented constructor
  */
  
  std::cout << "\n[2] Testing dalFilter(dalFileType,string) ..." << std::endl;
  try {
    DAL::dalFilter filterHDF5 (DAL::dalFileType::HDF5, columns);
    filterHDF5.summary();
    //
    DAL::dalFilter filterFITS (DAL::dalFileType::FITS, columns);
    filterFITS.summary();
    //
    DAL::dalFilter filterMS (DAL::dalFileType::CASA_MS, columns);
    filterMS.summary();
  }
  catch (std::string& message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  /*__________________________________________________________________
    Test 3: Test argumented constructor
  */
  
  std::cout << "\n[3] Testing dalFilter(dalFileType,string,string) ..." << std::endl;
  try {
    DAL::dalFilter filterHDF5 (DAL::dalFileType::HDF5, columns, conditions);
    filterHDF5.summary();
    //
    DAL::dalFilter filterFITS (DAL::dalFileType::FITS, columns, conditions);
    filterFITS.summary();
    //
    DAL::dalFilter filterMS (DAL::dalFileType::CASA_MS, columns, conditions);
    filterMS.summary();
  }
  catch (std::string& message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

/*!
  \brief Test the various public methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_methods ()
{
  std::cout << "\n[tdalDataset::test_methods]" << std::endl;
  
  int nofFailedTests (0);
  std::string columns ("DATA");
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  std::string filename = "tdalFilter.h5";

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    /* nothing else to do so far */
  } else {
    filename    = argv[1];
  }

  //________________________________________________________
  // Run the tests

  nofFailedTests += test_constructors ();
  nofFailedTests += test_methods ();

  return nofFailedTests;
}
