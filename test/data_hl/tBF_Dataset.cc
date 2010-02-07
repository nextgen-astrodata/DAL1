/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 3179 2009-10-14 13:20:10Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#include <CommonAttributes.h>
#include <BF_Dataset.h>

// Namespace usage
using std::cerr;
using std::cout;
using std::endl;
using DAL::CommonAttributes;
using DAL::Filename;
using DAL::BF_Dataset;
using DAL::BF_SysLog;

/*!
  \file tBF_Dataset.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the BF_Dataset class
 
  \author Lars B&auml;hren
 
  \date 2009/10/28
*/

//_______________________________________________________________________________
//                                                              test_constructors

DAL::Filename getFilename ()
{
  Filename file ("123456789",
		 "test",
		 Filename::bf,
		 Filename::h5);

  return file;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_Dataset object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tBF_Dataset::test_constructors]\n" << endl;

  int nofFailedTests (0);
  Filename file = getFilename();

  cout << "-- Filename = " << file.filename() << endl;
  
  cout << "[1] Testing construction with Filename ..." << endl;
  try {
    BF_Dataset dataset (file);
    //
    dataset.summary();
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing construction with CommonAttributes ..." << endl;
  try {
    CommonAttributes commonAttr;
    commonAttr.setFilename (file);
    //
    BF_Dataset dataset (commonAttr);
    //
    dataset.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing construction with filename ..." << endl;
  try {
    std::string filename = file.filename();
    //
    BF_Dataset dataset (filename);
    //
    dataset.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

/*!
  \brief Test the various methods of the class

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_methods ()
{
  cout << "\n[tBF_Dataset::test_methods]\n" << endl;

  int nofFailedTests (0);
  Filename file (getFilename());
  BF_Dataset dataset (file);

  cout << "[1] Extract SysLog from BF dataset ..." << endl;
  try {
    BF_SysLog sysLog = dataset.sysLog();
    //
    std::string groupType;
    sysLog.getAttribute("GROUPTYPE",groupType);
    //
    cout << "-- Location ID = " << sysLog.locationID() << endl;
    cout << "-- Attributes  = " << sysLog.attributes() << endl;
    cout << "-- GROUPTYPE   = " << groupType           << endl;
  } catch (std::string message) {
    cerr << message << endl;
    ++nofFailedTests;
  }


  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                 test_subGroups

/*!
  \brief Test working with the embedded groups

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_subGroups ()
{
  cout << "\n[tBF_Dataset::test_subGroups]\n" << endl;

  int nofFailedTests (0);
  Filename file = getFilename();
  BF_Dataset dataset (file);
  
  cout << "[1] Open SysLog group ..." << endl;
  try {
    dataset.openSysLog();
    dataset.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    ++nofFailedTests;
  }

  cout << "[2] Open PrimaryPointing groups ..." << endl;
  try {
    dataset.openPrimaryPointing(0,true);
    dataset.openPrimaryPointing(1,true);
    dataset.openPrimaryPointing(2,true);
    dataset.openPrimaryPointing(3,true);
    //
    dataset.openPrimaryPointing(0,true);
    //
    dataset.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] Open Beam groups ..." << endl;
  try {
    unsigned int nofBeams (5);
    // open Beam groups within existing PrimaryPointing group
    dataset.openBeam(0,0,true);
    dataset.openBeam(0,1,true);
    dataset.openBeam(0,2,true);
    // open Beam groups without previously existing PrimaryPointing group
    for (unsigned int beam(0); beam<nofBeams; ++beam) {
      dataset.openBeam(10,beam,true);
    }
    //
    dataset.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc,
          char *argv[])
{
  int nofFailedTests (0);
  bool haveDataset (true);
  std::string filename ("UNDEFINED");

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
  nofFailedTests += test_constructors ();
  // Test the various methods 
  nofFailedTests += test_methods ();
  // Test working with the embedded groups
  nofFailedTests += test_subGroups ();

  return nofFailedTests;
}
