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
  std::string filename ("LOFAR_TBB.h5");
  CommonAttributes attr;

  cout << "[1] Assign new values to the attributes ..." << endl;
  try {
    std::vector<std::string> stations;
    stations.push_back("CS001");
    stations.push_back("CS003");
    stations.push_back("CS005");

    attr.setFilename (filename);
    attr.setFiletype ("tbb");
    attr.setFiledate ("2009-10-10T01:00:00.0");
    attr.setTelescope ("LOFAR");
    attr.setProjectID ("CR-2009-10-10");
    attr.setProjectTitle ("CR test observation");
    attr.setProjectPI ("Mr. CR");
    attr.setProjectCoI ("Mrs. CR");
    attr.setProjectContact ("cr@lofar.org");
    attr.setObserver("Mr. LOFAR");
    attr.setObservationID ("1234567890");
    attr.setObservationTimeSys ("MET");
    attr.setObservationDateStart ("2009-10-10T00:00:00.0");
    attr.setObservationDateEnd ("2009-10-10T01:00:00.0");
    attr.setAntennaSet ("LBA_X");
    attr.setFilterSelection ("LBH_30_80");
    attr.setClockFrequency (200);
    attr.setClockFrequencyUnit ("MHz");
    attr.setTarget ("Everything under the sky");
    attr.setSystemVersion ("LOFAR system v1.0");
    attr.setPipelineName ("CR_Offline");
    attr.setPipelineVersion ("v0.9");
    attr.setStationsList (stations);
    attr.setNotes ("Life is a Lemmon");
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Show new values of the attributes ..." << endl;
  try {
    cout << "-- FILENAME               = " << attr.filename()             << endl;
    cout << "-- FILETYPE               = " << attr.filetype()             << endl;
    cout << "-- FILEDATE               = " << attr.filedate()             << endl;
    cout << "-- TELESCOPE              = " << attr.telescope()            << endl;
    cout << "-- PROJECT_ID             = " << attr.projectID()            << endl;
    cout << "-- PROJECT_TITLE          = " << attr.projectTitle()         << endl;
    cout << "-- PROJECT_PI             = " << attr.projectPI()            << endl;
    cout << "-- PROJECT_CO_I           = " << attr.projectCoI()           << endl;
    cout << "-- PROJECT_CONTACT        = " << attr.projectContact()       << endl;
    cout << "-- OBSERVER               = " << attr.observer()             << endl;
    cout << "-- OBSERVATION_ID         = " << attr.observationID()        << endl;
    cout << "-- OBSERVATION_TIMESYS    = " << attr.observationTimeSys()   << endl;
    cout << "-- OBSERVATION_DATE_START = " << attr.observationDateStart() << endl;
    cout << "-- OBSERVATION_DATE_END   = " << attr.observationDateEnd()   << endl;
    cout << "-- ANTENNA_SET            = " << attr.antennaSet()           << endl;
    cout << "-- FILTER_SELECTION       = " << attr.filterSelection()      << endl;
    cout << "-- CLOCK_FREQUENCY        = " << attr.clockFrequency()       << endl;
    cout << "-- CLOCK_FREQUENCY_UNIT   = " << attr.clockFrequencyUnit()   << endl;
    cout << "-- TARGET                 = " << attr.target()               << endl;
    cout << "-- SYSTEM_VERSION         = " << attr.systemVersion()        << endl;
    cout << "-- PIPELINE_NAME          = " << attr.pipelineName()         << endl;
    cout << "-- PIPELINE_VERSION       = " << attr.pipelineVersion()      << endl;
    cout << "-- NOF_STATIONS           = " << attr.nofStations()          << endl;
    cout << "-- STATIONS               = " << attr.stationsList()         << endl;
    cout << "-- NOTES                  = " << attr.notes()                << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] Create a copy of the object storing the attributes ..." << endl;
  try {
    CommonAttributes attrCopy (attr);
    //
    cout << "-- FILENAME               = " << attrCopy.filename()             << endl;
    cout << "-- FILETYPE               = " << attrCopy.filetype()             << endl;
    cout << "-- FILEDATE               = " << attrCopy.filedate()             << endl;
    cout << "-- TELESCOPE              = " << attrCopy.telescope()            << endl;
    cout << "-- PROJECT_ID             = " << attrCopy.projectID()            << endl;
    cout << "-- PROJECT_TITLE          = " << attrCopy.projectTitle()         << endl;
    cout << "-- PROJECT_PI             = " << attrCopy.projectPI()            << endl;
    cout << "-- PROJECT_CO_I           = " << attrCopy.projectCoI()           << endl;
    cout << "-- PROJECT_CONTACT        = " << attrCopy.projectContact()       << endl;
    cout << "-- OBSERVER               = " << attrCopy.observer()             << endl;
    cout << "-- OBSERVATION_ID         = " << attrCopy.observationID()        << endl;
    cout << "-- OBSERVATION_TIMESYS    = " << attrCopy.observationTimeSys()   << endl;
    cout << "-- OBSERVATION_DATE_START = " << attrCopy.observationDateStart() << endl;
    cout << "-- OBSERVATION_DATE_END   = " << attrCopy.observationDateEnd()   << endl;
    cout << "-- ANTENNA_SET            = " << attrCopy.antennaSet()           << endl;
    cout << "-- FILTER_SELECTION       = " << attrCopy.filterSelection()      << endl;
    cout << "-- CLOCK_FREQUENCY        = " << attrCopy.clockFrequency()       << endl;
    cout << "-- CLOCK_FREQUENCY_UNIT   = " << attrCopy.clockFrequencyUnit()   << endl;
    cout << "-- TARGET                 = " << attrCopy.target()               << endl;
    cout << "-- SYSTEM_VERSION         = " << attrCopy.systemVersion()        << endl;
    cout << "-- PIPELINE_NAME          = " << attrCopy.pipelineName()         << endl;
    cout << "-- PIPELINE_VERSION       = " << attrCopy.pipelineVersion()      << endl;
    cout << "-- NOF_STATIONS           = " << attrCopy.nofStations()          << endl;
    cout << "-- STATIONS               = " << attrCopy.stationsList()         << endl;
    cout << "-- NOTES                  = " << attrCopy.notes()                << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
#ifdef HAVE_HDF5

  cout << "[4] Write attribute to file ..." << endl;
  try {
    hid_t fileID (0);
    herr_t h5error (0);
    // create a new HDF5 file
    fileID = H5Fcreate (filename.c_str(),
			H5F_ACC_TRUNC,
			H5P_DEFAULT,
			H5P_DEFAULT);
    // write the attributes to the root group of the file
    attr.h5write (fileID);
    // close the HDF5 file once we are done
    h5error = H5Fclose (fileID);
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[5] Read the attributes back in from file ..." << endl;
  try {
    hid_t fileID (0);
    herr_t h5error (0);
    // open the file from which to read in the attributes
    fileID = H5Fopen (filename.c_str(),
		      H5F_ACC_RDWR,
		      H5P_DEFAULT);
    // close the HDF5 file once we are done
    h5error = H5Fclose (fileID);
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }


#endif
  
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
