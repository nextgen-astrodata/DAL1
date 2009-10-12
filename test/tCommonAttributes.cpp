/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 2286 2009-02-03 10:50:48Z baehren                  $ |
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

// Namespace usage
using std::cout;
using std::endl;
using DAL::CommonAttributes;

/*!
  \file tCommonAttributes.cpp

  \ingroup DAL

  \brief A collection of test routines for the CommonAttributes class
 
  \author Lars B&auml;hren
 
  \date 2009/08/31
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new CommonAttributes object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tCommonAttributes::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing default constructor ..." << endl;
  try {
    CommonAttributes attributes;
    //
    attributes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing argumented constructor ..." << endl;
  try {
    std::string filename ("TBB_Dataset.h5");
    std::string filetype ("tbb");
    std::string filedate ("2009-10-10T00:00:00.0");
    CommonAttributes attributes (filename,
				 filetype,
				 filedate);
    //
    attributes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing argumented constructor ..." << endl;
  try {
    std::string observationID ("1234567890");
    std::string observationTimeSys ("UTC");
    std::string observationDateStart ("2009-10-10T00:00:00.0");
    std::string observationDateEnd ("2009-10-10T01:00:00.0");
    CommonAttributes attributes (observationID,
				 observationTimeSys,
				 observationDateStart,
				 observationDateEnd);
    //
    attributes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] Testing copy constructor ..." << endl;
  try {
    std::string observationID ("1234567890");
    std::string observationTimeSys ("UTC");
    std::string observationDateStart ("2009-10-10T00:00:00.0");
    std::string observationDateEnd ("2009-10-10T01:00:00.0");
    //
    CommonAttributes attributes (observationID,
				 observationTimeSys,
				 observationDateStart,
				 observationDateEnd);
    attributes.summary();
    //
    CommonAttributes attributesCopy (attributes);
    attributesCopy.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the individual attributes

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_attributes ()
{
  cout << "\n[tCommonAttributes::test_attributes]\n" << endl;

  int nofFailedTests (0);
  CommonAttributes attributes;

  attributes.setFilename ("LOFAR_TBB.h5");
  attributes.setFiletype ("tbb");
  attributes.setFiledate ("2009-10-10T01:00:00.0");
  attributes.setTelescope ("LOFAR");
  attributes.setProjectID ("CR-2009-10-10");
  attributes.setProjectTitle ("CR test observation");
  attributes.setProjectPI ("Mr. CR");

  cout << "-- Filename           = " << attributes.filename()     << endl;
  cout << "-- Filetype           = " << attributes.filetype()     << endl;
  cout << "-- File creation date = " << attributes.filedate()     << endl;
  cout << "-- Telescope name     = " << attributes.telescope()    << endl;
  cout << "-- Project ID         = " << attributes.projectID()    << endl;
  cout << "-- Project title      = " << attributes.projectTitle() << endl;
  cout << "-- Project PI         = " << attributes.projectPI()    << endl;

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test access to the individual attributes
  nofFailedTests += test_attributes ();

  return nofFailedTests;
}
