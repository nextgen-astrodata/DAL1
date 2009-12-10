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

#include <SAS_Settings.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::SAS_Settings;

/*!
  \file tSAS_Settings.cc

  \ingroup DAL
  \ingroup data_common

  \brief A collection of test routines for the SAS_Settings class
 
  \author Lars B&auml;hren
 
  \date 2009/12/09
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new SAS_Settings object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tSAS_Settings::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing default constructor ..." << endl;
  try {
    SAS_Settings settings;
    //
    settings.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing copy constructor ..." << endl;
  try {
    SAS_Settings settings1;
    SAS_Settings settings2 (settings1);
    //
    settings2.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

/*!
  \brief Test the various methods of the SAS_Settings class

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_methods ()
{
  cout << "\n[tSAS_Settings::test_methods]\n" << endl;

  int nofFailedTests (0);
  SAS_Settings settings;
  
  cout << "[1] AntennaSet" << endl;
  try {
    cout << "-- Values         = " << settings.antennaSet()                 << endl;
    cout << "-- have LBA_ALL   = " << settings.haveAntennaSet ("LBA_ALL")   << endl;
    cout << "-- have LBA_INNER = " << settings.haveAntennaSet ("LBA_INNER") << endl;
    cout << "-- have LBA_OUTER = " << settings.haveAntennaSet ("LBA_OUTER") << endl;
    cout << "-- have HBA_BOTH  = " << settings.haveAntennaSet ("HBA_BOTH")  << endl;
    cout << "-- have HBA_ONE   = " << settings.haveAntennaSet ("HBA_ONE")   << endl;
    cout << "-- have HBA_ALL   = " << settings.haveAntennaSet ("HBA_ALL")   << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] FilterSelection" << endl;
  try {
    cout << "-- Values = " << settings.filterSelection() << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test the various methods of the SAS_Settings class
  nofFailedTests += test_methods ();
  
  return nofFailedTests;
}
