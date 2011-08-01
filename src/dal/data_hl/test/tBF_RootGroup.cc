/***************************************************************************
 *   Copyright (C) 2009-2011                                               *
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

#include <data_hl/BF_RootGroup.h>

// Namespace usage
using std::cerr;
using std::cout;
using std::endl;
using DAL::Filename;
using DAL::BF_RootGroup;

/*!
  \file tBF_RootGroup.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::BF_RootGroup class
 
  \author Lars B&auml;hren
 
  \date 2009/10/28
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Get DAL::Filename object

  \param obsID          -- Unique identifier for the observation.
  \param optDescription	-- Optional descriptors.
  \param filetype       -- Marker for the contents of the file.
  \param extension      -- Extension of the file.
*/
DAL::Filename getFilename (std::string const &obsID="123456789",
			   std::string const &optDescription="test",
			   Filename::Type const &filetype=Filename::bf,
			   Filename::Extension const &extension=Filename::h5)
{
  Filename file (obsID,
		 optDescription,
		 filetype,
		 extension);
  
  return file;
}

/*!
  \brief Show selected attributes attached to the root group of the BF file.
  \param location -- HDF5 object handle.
 */
bool show_attributes (hid_t const &location)
{
  bool status = true;
  std::string grouptype;
  std::string filename;
  std::string filetype;
  std::string telescope;
  std::string observer;

  status *= DAL::HDF5Attribute::read (location, "GROUPTYPE", grouptype );
  status *= DAL::HDF5Attribute::read (location, "FILENAME",  filename  );
  status *= DAL::HDF5Attribute::read (location, "FILETYPE",  filetype  );
  status *= DAL::HDF5Attribute::read (location, "TELESCOPE", telescope );
  status *= DAL::HDF5Attribute::read (location, "OBSERVER",  observer  );
  
  std::cout << "-- GROUPTYPE = " << grouptype << std::endl;
  std::cout << "-- FILENAME  = " << filename  << std::endl;
  std::cout << "-- FILETYPE  = " << filetype  << std::endl;
  std::cout << "-- TELESCOPE = " << telescope << std::endl;
  std::cout << "-- OBSERVER  = " << observer  << std::endl;

  return status;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_RootGroup object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tBF_RootGroup::test_constructors]\n" << endl;

  int nofFailedTests = 0;
  DAL::Filename file = getFilename();

  /*________________________________________________________
    Test 1 : Testing construction with DAL::Filename object.
  */
  
  cout << "[1] Testing BF_RootGroup(Filename) ..." << endl;
  try {
    // Create new file
    DAL::BF_RootGroup bf (file);
    bf.summary();
    // Display selected attributes
    show_attributes (bf.locationID());
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  return 0;
  
  /*________________________________________________________
    Test 2 : Testing construction with DAL::CommonAttributes
             object, from which then the proper file name
	     is being extracted.
  */
  
  cout << "[2] Testing BF_RootGroup(CommonAttributes) ..." << endl;
  try {
    DAL::CommonAttributes commonAttr;
    commonAttr.setFilename (file);
    //
    BF_RootGroup dataset (commonAttr);
    //
    dataset.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 3 : Testing construction with simple filename
             string; opens existing file.
  */
  
  cout << "[3] Testing BF_RootGroup(string) ..." << endl;
  try {
    std::string filename = file.filename();
    //
    BF_RootGroup dataset (filename);
    //
    dataset.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes attached to the root group.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_attributes ()
{
  cout << "\n[tBF_RootGroup::test_attributes]\n" << endl;

  int nofFailedTests = 0;
  DAL::Filename file = getFilename();

  // Open BF dataset
  BF_RootGroup bf (file);
  bf.summary();


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
  cout << "\n[tBF_RootGroup::test_subGroups]\n" << endl;

  int nofFailedTests (0);
  Filename file = getFilename();
  BF_RootGroup dataset (file);
  
  cout << "[1] Open SubArrayPointing groups ..." << endl;
  try {
    dataset.openSubArrayPointing(0);
    dataset.openSubArrayPointing(1);
    dataset.openSubArrayPointing(2);
    dataset.openSubArrayPointing(3);
    //
    dataset.openSubArrayPointing(0);
    //
    dataset.summary(); 
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] Open Beam groups ..." << endl;
  try {
    unsigned int nofBeams (5);
    // open Beam groups within existing SubArrayPointing group
    dataset.openBeam(0,0,true);
    dataset.openBeam(0,1,true);
    dataset.openBeam(0,2,true);
    // open Beam groups without previously existing SubArrayPointing group
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
//                                                                   test_methods

/*!
  \brief Test the various methods of the class

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_methods ()
{
  cout << "\n[tBF_RootGroup::test_methods]\n" << endl;

  int nofFailedTests = 0;
  Filename file      = getFilename();
  BF_RootGroup dataset (file);

  cout << "[1] Extract SysLog from BF dataset ..." << endl;
  try {
    DAL::SysLog sysLog = dataset.sysLog();
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

  cout << "[2] Extract SubArrayPointing group from BF dataset ..." << endl;
  try {
    DAL::BF_SubArrayPointing pointing = dataset.getSubArrayPointing (0);
    //
    std::string groupType;
    pointing.getAttribute("GROUPTYPE",groupType);
    //
    cout << "-- Location ID = " << pointing.locationID() << endl;
    cout << "-- Attributes  = " << pointing.attributes() << endl;
    cout << "-- GROUPTYPE   = " << groupType             << endl;
  } catch (std::string message) {
    cerr << message << endl;
    ++nofFailedTests;
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
  // Test access to the attributes attached to the root group
  nofFailedTests += test_attributes ();
  // // Test working with the embedded groups
  // nofFailedTests += test_subGroups ();
  // // Test the various methods 
  // nofFailedTests += test_methods ();

  return nofFailedTests;
}
