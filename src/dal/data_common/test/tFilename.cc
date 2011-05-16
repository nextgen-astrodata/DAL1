/***************************************************************************
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

#include <data_common/Filename.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::Filename;

/*!
  \file tFilename.cc

  \ingroup DAL
  \ingroup data_common

  \brief A collection of test routines for the Filename class
 
  \author Lars B&auml;hren
 
  \date 2009/09/28
*/

//_______________________________________________________________________________
//                                                             test_staticMethods

/*!
  \brief Test the static methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_staticMethods ()
{
  cout << "\n[tFilename::test_staticMethods]\n" << endl;

  int nofFailedTests (0);

  std::cout << "[1] Testing Filename::extensionMap() ..." << std::endl;
  try {
    std::map<Filename::Extension,std::string> extension = Filename::extensionMap();

    std::map<Filename::Extension,std::string>::iterator it;

    for (it=extension.begin(); it!=extension.end(); ++it) {
      std::cout << "  Extension[" << it->first << "] = " << it->second
		<< std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing Filename::filetypeMap() ..." << std::endl;
  try {
    std::map<Filename::Type,std::string> filetype = Filename::filetypeMap();

    std::map<Filename::Type,std::string>::iterator it;

    for (it=filetype.begin(); it!=filetype.end(); ++it) {
      std::cout << "  Type[" << it->first << "] = " << it->second
		<< std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new Filename object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tFilename::test_constructors]\n" << endl;

  int nofFailedTests (0);
  std::string observationID       = "1234567890";
  std::string optionalDescription = "TBBraw";
  std::string path ("/tmp/lofarsoft");
  
  cout << "[1] Testing Filename() ..." << endl;
  try {
    Filename file;
    //
    file.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing Filename(string,Type) ..." << endl;
  try {
    Filename file (observationID,
		   Filename::tbb);
    //
    file.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing Filename(string,Type,Extension) ..." << endl;
  try {
    Filename file (observationID,
		   Filename::tbb,
		   Filename::h5);
    //
    file.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] Testing Filename(string,string,Type,Extension) ..." << endl;
  try {
    Filename file (observationID,
		   optionalDescription,
		   Filename::tbb,
		   Filename::h5);
    //
    file.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[5] Testing Filename(string,string,Type,Extension,string) ..."
       << endl;
  try {
    Filename file (observationID,
		   optionalDescription,
		   Filename::tbb,
		   Filename::h5,
		   path);
    //
    file.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[6] Testing copy constructor ..." << endl;
  try {
    Filename fileOrig (observationID,
		       optionalDescription,
		       Filename::tbb,
		       Filename::h5,
		       path);
    fileOrig.summary();
    //
    Filename fileCopy (fileOrig);
    fileCopy.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes of which the filename is build

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/

int test_attributes ()
{
  cout << "\n[tFilename::test_attributes]\n" << endl;

  int nofFailedTests (0);
  std::string path ("/tmp/lofarsoft");
  Filename file;

  cout << "[1] Assigning attributes ..." << endl;
  try {
    file.setObservationID ("1234567890");
    cout << "-- Observation ID       = " << file.observationID() << endl;
    cout << "--> " << file.filename()     << endl;
    //
    file.setOptionalDescription ("D20091010");
    cout << "-- Optional description = " << file.optionalDescription() << endl;
    cout << "--> " << file.filename()     << endl;
    //
    file.setFiletype (Filename::tbb);
    cout << "-- Filetype             = " << file.filetypeName() << endl;
    cout << "--> " << file.filename()     << endl;
    //
    file.setExtension (Filename::h5);
    cout << "-- File extension       = " << file.extensionName() << endl;
    cout << "--> " << file.filename()     << endl;
    //
    file.setPath (path);
    cout << "-- File path            = " << file.path() << endl;
    cout << "--> " << file.filename()     << endl;
    cout << "--> " << file.filename(true) << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main ()
{
  int nofFailedTests (0);

  // Test the static methods
  nofFailedTests += test_staticMethods ();

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test access to the attributes
  nofFailedTests += test_attributes ();

  return nofFailedTests;
}
