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

#include <data_common/CommonAttributes.h>
#include <data_hl/BF_RootGroup.h>
#include <data_hl/BF_SubArrayPointing.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::CommonAttributes;
using DAL::Filename;
using DAL::BF_RootGroup;
using DAL::BF_SubArrayPointing;

/*!
  \file tBF_SubArrayPointing.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::BF_SubArrayPointing class
 
  \author Lars B&auml;hren
 
  \date 2009/10/28
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_SubArrayPointing object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tBF_SubArrayPointing::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  std::string filename ("tBF_SubArrayPointing.h5");
  herr_t h5error;
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    BF_SubArrayPointing beam;
    beam.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing argumented constructor ..." << std::endl;
  try {
    // create HDF5 file to which the StationBeam group is getting attached
    hid_t fileID = H5Fcreate (filename.c_str(),
			      H5F_ACC_TRUNC,
			      H5P_DEFAULT,
			      H5P_DEFAULT);
    if (fileID>0) {
      BF_SubArrayPointing beam1 (fileID,1,true);
      BF_SubArrayPointing beam2 (fileID,2,true);
      BF_SubArrayPointing beam3 (fileID,3,true);
      BF_SubArrayPointing beam4 (fileID,4,true);
      //
      beam1.summary();
    }
    // release HDF5 file handler
    h5error = H5Fclose (fileID);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
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
  std::cout << "\n[tBF_SubArrayPointing::test_subGroups]\n" << endl;

  int nofFailedTests (0);
  std::string filename ("tBF_SubArrayPointing.h5");
  herr_t h5error;
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);

  cout << "[1] Create Beams within existing StationBeam ..." << endl;
  if (fileID) {
    // open StationBeam group
    BF_SubArrayPointing beam (fileID,1,true);
    cout << "-- group name = " << beam.getName(1) << endl;
    cout << "-- nof. beams = " << beam.nofBeams() << endl;
    // open/create Beam groups
    beam.openBeam (0,true);
    cout << "-- nof. beams = " << beam.nofBeams() << endl;
    beam.openBeam (1,true);
    cout << "-- nof. beams = " << beam.nofBeams() << endl;
    beam.openBeam (2,true);
    cout << "-- nof. beams = " << beam.nofBeams() << endl;
  }

  cout << "[2] Create Beams within new StationBeam ..." << endl;
  if (fileID) {
    // open StationBeam group
    BF_SubArrayPointing beam (fileID,10,true);
    cout << "-- nof. beams = " << beam.nofBeams() << endl;
    // open/create Beam groups
    beam.openBeam (0,true);
    cout << "-- nof. beams = " << beam.nofBeams() << endl;
    beam.openBeam (1,true);
    cout << "-- nof. beams = " << beam.nofBeams() << endl;
    beam.openBeam (2,true);
    cout << "-- nof. beams = " << beam.nofBeams() << endl;
    beam.openBeam (3,true);
    cout << "-- nof. beams = " << beam.nofBeams() << endl;
  }
    
  // release HDF5 file handler
  h5error = H5Fclose (fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_containers

/*!
  \brief Test usage of std::map<std::string,DAL::BF_SubArrayPointing>

  Instead of directly handling objects of type BF_SubArrayPointing use 
  \code
  std::map<std::string,DAL::BF_SubArrayPointing> stationBeams;
  
  name               = DAL::BF_SubArrayPointing::getName(pointingID);
  stationBeams[name] = DAL::BF_SubArrayPointing (fileID, pointingID, true);
  \endcode
  as a container.

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_containers ()
{
  cout << "\n[tBF_SubArrayPointing::test_containers]\n" << endl;

  int nofFailedTests (0);
  std::string filename ("tBF_SubArrayPointing.h5");
  std::string name;
  std::string groupType;
  herr_t h5error;
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  std::map<std::string,DAL::BF_SubArrayPointing> stationBeams;
  std::map<std::string,DAL::BF_SubArrayPointing>::iterator it;

  //______________________________________________
  // Test creation of groups

  for (unsigned int pointingID(10); pointingID<15; ++pointingID) {
    BF_SubArrayPointing beam (fileID, pointingID, true);
    name               = BF_SubArrayPointing::getName(pointingID);
    stationBeams[name] = beam;
    it                 = stationBeams.find(name);
    // try accessing attributes
    it->second.getAttribute("GROUPTYPE",groupType);
    // Feedback 
    cout << "-- created " << it->first
	 << " (" << it->second.locationID() << ")" << endl;
    cout << "--> nof. beams      = " << it->second.nofBeams() << endl;
    cout << "--> nof. attributes = " << it->second.nofAttributes()  << endl;
    // open pencil beam groups inside the station beam groups
    for (unsigned int beamID(0); beamID<5; ++beamID) {
      (it->second).openBeam(beamID);
    }
  }

  //______________________________________________
  // Release HDF5 file handler

  h5error = H5Fclose (fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
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
  // Test working with the embedded groups
  nofFailedTests += test_subGroups ();
  // Test usage of STL containers
  nofFailedTests += test_containers();

  return nofFailedTests;
}
