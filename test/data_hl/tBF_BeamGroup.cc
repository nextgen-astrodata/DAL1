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
#include <BF_RootGroup.h>
#include <BF_SubArrayPointing.h>
#include <BF_BeamGroup.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::CommonAttributes;
using DAL::Filename;
using DAL::BF_RootGroup;
using DAL::BF_SubArrayPointing;
using DAL::BF_BeamGroup;

/*!
  \file tBF_BeamGroup.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the BF_BeamGroup class
 
  \author Lars B&auml;hren
 
  \date 2009/10/28
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_BeamGroup object

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &fileID)
{
  cout << "\n[tBF_BeamGroup::test_constructors]\n" << endl;

  int nofFailedTests (0);

  cout << "[1] Testing default constructor ..." << endl;
  try {
    BF_BeamGroup beam;
    beam.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing argumented constructor ..." << endl;
  try {
    if (fileID>0) {
      BF_BeamGroup beam1 (fileID,1,true);
      beam1.summary();
      //
      BF_BeamGroup beam2 (fileID,2,true);
      beam2.summary();
      //
      BF_BeamGroup beam3 (fileID,3,true);
      beam3.summary();
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] Testing copy constructor ..." << endl;
  try {
    BF_BeamGroup beam (fileID,10,true);
    beam.summary();
    //
    BF_BeamGroup beamCopy (beam);
    beamCopy.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes attached to the Beam group

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_attributes (hid_t const &fileID)
{
  cout << "\n[tBF_BeamGroup::test_attributes]\n" << endl;

  int nofFailedTests (0);
  std::string groupname;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  /* Get list of groups attached to the file */
  DAL::h5get_names (names, fileID, H5G_GROUP);

  if (names.size() > 0) {
    it = names.begin();
    groupname = *it;
  } else {
    std::cerr << "Skipping tests - no station group found." << endl;
    return -1;
  }

  cout << "-- opening Beam " << groupname << " ..." << endl;
  BF_BeamGroup beam (fileID,groupname);

  // Carry out the tests ___________________________________

  std::string groupType;
  std::vector<std::string> target;
  int nofStations;
  std::vector<std::string> stationsList;
  double pointRA;
  double pointDEC;
  double positionOffsetRA;
  double positionOffsetDEC;
  double beamDiameterRA;
  double beamDiameterDEC;

  cout << "-- reading in attributes attached to group ..." << endl;

  beam.getAttribute ("GROUPTYPE",groupType);
  beam.getAttribute ("TARGET",target);
  beam.getAttribute ("NOF_STATIONS",nofStations);
  beam.getAttribute ("STATIONS_LIST",stationsList);
  beam.getAttribute ("POINT_RA",pointRA);
  beam.getAttribute ("POINT_DEC",pointDEC);
  beam.getAttribute ("POSITION_OFFSET_RA",positionOffsetRA);
  beam.getAttribute ("POSITION_OFFSET_DEC",positionOffsetDEC);
  beam.getAttribute ("PB_DIAMETER_RA",beamDiameterRA);
  beam.getAttribute ("PB_DIAMETER_DEC",beamDiameterDEC);

  cout << " -- GROUPTYPE           = " << groupType         << endl;
  cout << " -- TARGET              = " << target            << endl;
  cout << " -- NOF_STATIONS        = " << nofStations       << endl;
  cout << " -- STATIONS_LIST       = " << stationsList      << endl;
  cout << " -- POINT_RA            = " << pointRA           << endl;
  cout << " -- POINT_DEC           = " << pointDEC          << endl;
  cout << " -- POSITION_OFFSET_RA  = " << positionOffsetRA  << endl;
  cout << " -- POSITION_OFFSET_DEC = " << positionOffsetDEC << endl;
  cout << " -- PB_DIAMETER_RA      = " << beamDiameterRA    << endl;
  cout << " -- PB_DIAMETER_DEC     = " << beamDiameterDEC   << endl;

  cout << "-- assigning new valeus to the attributes ..." << endl;

  pointRA  = 45.0;
  pointDEC = 45.0;

  beam.setAttribute ("POINT_RA",pointRA);
  beam.setAttribute ("POINT_DEC",pointDEC);

  beam.getAttribute ("POINT_RA",pointRA);
  beam.getAttribute ("POINT_DEC",pointDEC);

  cout << " -- POINT_RA            = " << pointRA           << endl;
  cout << " -- POINT_DEC           = " << pointDEC          << endl;

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                       test_map

/*!
  \brief Test usage of std::map<std::string,DAL::BF_BeamGroup>

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_map (hid_t const &fileID)
{
  cout << "\n[tBF_BeamGroup::test_map]\n" << endl;

  int nofFailedTests (0);
  std::set<std::string> names;
  std::set<std::string>::iterator it;
  
  /* Get list of groups attached to the file */
  DAL::h5get_names (names, fileID, H5G_GROUP);
  
  std::string groupType;
  std::vector<std::string> target;
  
  // std::vector<DAL::BF_BeamGroup> _______________________
  
  cout << "[1] Test using std::vector<DAL::BF_BeamGroup> ..." << endl;
  if (names.size() > 0) {
    std::vector<DAL::BF_BeamGroup> beams (1);
    // open Beam group
    it = names.begin();
    BF_BeamGroup beam (fileID, *it);
    beams[0] = beam;
    // get attribute values
    beams[0].getAttribute ("GROUPTYPE",groupType);
    beams[0].getAttribute ("TARGET",target);
    // summary
    beams[0].summary();
    cout << "-- nof beams                     = " << beams.size() << endl;
    cout << "-- Attribute GROUPTYPE           = " << groupType    << endl;
    cout << "-- Attribute TARGET              = " << target       << endl;
  }
  
  // std::map<std::string,DAL::BF_BeamGroup> ______________

  cout << "[2] Test using std::map<std::string,DAL::BF_BeamGroup> ..." << endl;
  if (names.size() > 0) {
    std::map<std::string,DAL::BF_BeamGroup> beams;
    std::map<std::string,DAL::BF_BeamGroup>::iterator itMap;
    
    for (it=names.begin(); it!=names.end(); ++it) {
      // open Beam group
      BF_BeamGroup beam (fileID, *it);
      beams[*it] = beam;
      // get pointer to the created group
      itMap = beams.find(*it);
      itMap->second.summary();
      // get attribute values
      itMap->second.getAttribute ("GROUPTYPE",groupType);
      itMap->second.getAttribute ("TARGET",target);
      //
      cout << "-- nof beams                     = " << beams.size() << endl;
      cout << "-- Attribute GROUPTYPE           = " << groupType    << endl;
      cout << "-- Attribute TARGET              = " << target       << endl;
    }
  } else {
    std::cerr << "Skipping tests - no station group found." << endl;
    return -1;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main ()
{
  int nofFailedTests (0);
  std::string filename ("tBF_BeamGroup.h5");
  
  //________________________________________________________
  // Create HDF5 file to work with
  
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  
  /* If file creation was successful, run the tests. */
  if (H5Iis_valid(fileID)) {
    
    // Test for the constructor(s)
    nofFailedTests += test_constructors (fileID);
    // Test access to the attributes
    nofFailedTests += test_attributes (fileID);
    // Test usage of std::map<std::string,DAL::BF_BeamGroup>
    nofFailedTests += test_map (fileID);
    
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }
  
  //________________________________________________________
  // close HDF5 file
  
  H5Fclose(fileID);

  return nofFailedTests;
}
