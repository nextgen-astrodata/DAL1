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
  
    /*____________________________________________
      Summary of created MeasurementSet
    */
    
  cout << "-- Filename            = " << filename                   << endl;
  cout << "-- File exists         = " << fileExists                 << endl;
  cout << "-- Absolute filename   = " << absoluteName               << endl;
  cout << "-- Feed table          = " << ms.feedTableName()         << endl;
  cout << "-- Field table         = " << ms.fieldTableName()        << endl;
  cout << "-- Polarization table  = " << ms.polarizationTableName() << endl;
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_open

int test_open (std::string const &filename)
{
  cout << "\n[tcasacore_ms::test_open]\n" << endl;
  
  int status      = 0;
  std::string absoluteName;
  casa::File msfile (filename);

  if (msfile.exists()) {
    
    if (msfile.isSymLink()) {
      casa::SymLink link (msfile);
      casa::Path realFileName = link.followSymLink();
      absoluteName = realFileName.absoluteName();
    } else {
      absoluteName = filename;
    }
    
    /*______________________________________________________
      Use MS-specific classes to access data
    */
    
    // Create the MeasurementSet from an existing Table on disk
    casa::MeasurementSet ms (absoluteName); 
    // Create a columns object that accesses the data in the specified Table. 
    casa::ROMSMainColumns mainColumns (ms);
    // Create object to hold 'TIME' column
    casa::ROScalarColumn<casa::Double> timeColumn = mainColumns.time ();
    // Create object to hold 'UVW' column
    casa::ROArrayColumn<casa::Double> uvwColumn = mainColumns.uvw ();
    // Create object to hold 'DATA' column
    casa::ROArrayColumn<casa::Complex> dataColumn = mainColumns.data ();

    /*______________________________________________________
      Use generic Table classes to access data
    */

    // Open table
    casa::Table table (absoluteName);
    //
    casa::TableDesc tableDesc = table.tableDesc ();

    /*______________________________________________________
      Summary
    */
    
    cout << "-- Filename            = " << filename                   << endl;
    cout << "-- Absolute filename   = " << absoluteName               << endl;
    cout << "-- Feed table          = " << ms.feedTableName()         << endl;
    cout << "-- Field table         = " << ms.fieldTableName()        << endl;
    cout << "-- Polarization table  = " << ms.polarizationTableName() << endl;
    cout << "-- 'TIME' : nof. rows  = " << timeColumn.nrow()          << endl;
    cout << "-- 'UVW'  : nof. rows  = " << uvwColumn.nrow()           << endl;
    cout << "-- 'UVW'  : cell shape = " << uvwColumn.shape(0)         << endl;
    cout << "-- 'DATA' : nof. rows  = " << dataColumn.nrow()          << endl;
    cout << "-- 'DATA' : cell shape = " << dataColumn.shape(0)        << endl;
    cout << "-- Name of the table   = " << table.tableName()          << endl;
    cout << "-- nof. table rows     = " << table.nrow()               << endl;
    cout << "-- Tables column names = " << tableDesc.columnNames()    << endl;
    
  } else {
    std::cerr << "ERROR: MeasurementSet " << filename << " does not exist!" << endl;
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
