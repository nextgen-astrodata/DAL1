/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 3179 2009-10-14 13:20:10Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

#include <TBB_StationTrigger.h>

// Namespace usage
using std::endl;
using DAL::TBB_StationTrigger;

/*!
  \file tTBB_StationTrigger.cc
  
  \ingroup DAL
  \ingroup data_hl
  
  \brief A collection of test routines for the DAL::TBB_StationTrigger class
  
  \author Lars B&auml;hren
  
  \date 2010/01/19
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new TBB_StationTrigger object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tTBB_StationTrigger::test_constructors]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  herr_t h5error;
  std::string filename ("tTBB_StationTrigger.h5");
  
  // Open/Create HDF5 file _________________________________

  fileID = H5Fcreate (filename.c_str(),
		      H5F_ACC_TRUNC,
		      H5P_DEFAULT,
		      H5P_DEFAULT);
  
  if (fileID < 0) {
    std::cerr << "ERROR : Failed to open/create file." << endl;
    return -1;
  }

  cout << "-- Filename  = " << filename  << endl;
  cout << "-- File ID   = " << fileID    << endl;

  // Test default constructor ______________________________

  std::cout << "[1] Testing default constructor ..." << endl;
  try {
    TBB_StationTrigger table;
    //
    table.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  // Construction with location and name ___________________

  std::cout << "[2] Testing argumented constructor ..." << endl;
  try {
    TBB_StationTrigger table (fileID);
    table.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[3] Testing argumented constructor ..." << endl;
  try {
    std::string tableName ("TRIGGER_TABLE");
    TBB_StationTrigger table (fileID, tableName);
    table.summary();
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  // release HDF5 object identifiers _______________________

  h5error = H5Fclose (fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  return nofFailedTests;
}
