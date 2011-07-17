/***************************************************************************
 *   Copyright (C) 2007-2011                                               *
 *   Lars Baehren (lbaehren@gmail.com)                                     *
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

#include <iostream>
#include <string>

#include <casa/OS/File.h>
#include <casa/OS/SymLink.h>
#include <ms/MeasurementSets.h>
#include <ms/MeasurementSets/MeasurementSet.h>
#include <tables/Tables/TableDesc.h>
#include <tables/Tables/SetupNewTab.h>

/*!
  \file tMeasurementSets.cc

  \ingroup DAL

  \brief Test building against the "ms" modules of casacore

  \author Lars B&auml;hren

  \date 2007/08/13

  This test program contains a number of routines which make use of various
  modules and classes of the casacore libraries; rather than providing a full
  test and demonstration of all the available functionality, this program collects
  a number of very simple test such creating some basic objects and using their
  APIs.
*/

/* Namespace usage */
using std::cout;
using std::endl;

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Test working with the classes in the CASA "ms" module
  
  \return nofFaildTests -- The number of failed tests within this function
*/
int test_open_create (std::string const &filename="tcasacore_ms.ms")
{
  cout << "\n[tcasacore_ms::test_open_create]\n" << endl;
  
  int nofFailedTests = 0;
  bool fileExists    = false;
  std::string absoluteName;
  casa::File msfile (filename);
  casa::MeasurementSet ms;

  fileExists = msfile.exists();
  
  if (fileExists) {
    
    /*____________________________________________
      Open existing MeasurementSet
    */
    
    if (msfile.isSymLink()) {
      casa::SymLink link (msfile);
      casa::Path realFileName = link.followSymLink();
      absoluteName = realFileName.absoluteName();
    } else {
      absoluteName = filename;
    }
    
    ms = casa::MeasurementSet (absoluteName);
    
  } else {

    /*____________________________________________
      Create new MeasurementSet
    */
    
    unsigned int nofColumns (10);
    // create table descriptor
    casa::TableDesc simpleDesc (casa::MS::requiredTableDesc());
    // set up a new table
    casa::SetupNewTable newTab (filename,
				simpleDesc,
				casa::Table::New);
    // create the new MeasurementSet
    ms = casa::MeasurementSet (newTab, nofColumns);
    // create the required sub-tables
    ms.createDefaultSubtables(casa::Table::New);
    ms.flush();
    
  }
  
  /* Summary */
  
  std::cout << "-- Filename          = " << filename            << std::endl;
  std::cout << "-- File exists       = " << fileExists          << std::endl;
  std::cout << "-- Absolute filename = " << absoluteName        << std::endl;
  std::cout << "-- Feed table name   = " << ms.feedTableName()  << std::endl;
  std::cout << "-- Field table name  = " << ms.fieldTableName() << std::endl;
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_open

int test_open (std::string const &filename)
{
  cout << "\n[tcasacore_ms::test_open]\n" << endl;
  
  int status      = 0;
  bool fileExists = false;
  std::string absoluteName;
  casa::File msfile (filename);

  fileExists = msfile.exists();

  if (fileExists) {
    
    if (msfile.isSymLink()) {
      casa::SymLink link (msfile);
      casa::Path realFileName = link.followSymLink();
      absoluteName = realFileName.absoluteName();
    } else {
      absoluteName = filename;
    }
    
    casa::MeasurementSet ms (absoluteName);
    
    /* Summary */
    
    std::cout << "-- Filename          = " << filename            << std::endl;
    std::cout << "-- Absolute filename = " << absoluteName        << std::endl;
    std::cout << "-- Feed table name   = " << ms.feedTableName()  << std::endl;
    std::cout << "-- Field table name  = " << ms.fieldTableName() << std::endl;
    
  } else {
    std::cerr << "ERROR: MeasurementSet " << filename << " does not exist!" << std::endl;
    status = 1;
  }

  return status;
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
  std::string filename = "tcasacore_ms.ms";

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc > 1) {
    filename    = std::string(argv[1]);
    haveDataset = true;
  }

  //________________________________________________________
  // Run the tests

  if (haveDataset) {
    nofFailedTests += test_open (filename);
  }

  nofFailedTests += test_open_create ();
  
  return nofFailedTests;
}
