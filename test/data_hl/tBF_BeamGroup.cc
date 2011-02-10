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

  int nofFailedTests = 0;
  
  /*__________________________________________________________________
    Test 1: Default constructor.
  */
  
  cout << "[1] Testing BF_BeamGroup() ..." << endl;
  try {
    BF_BeamGroup beam;
    beam.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  /*__________________________________________________________________
    Test 2: Argumented constructor, providing identifier for reference
            location, beam ID and switch whether or not to create
	    group if not existing yet.
  */
  
  cout << "[2] Testing BF_BeamGroup(hid_t,uint,bool) ..." << endl;
  try {
    BF_BeamGroup beam1 (fileID,1,true);
    beam1.summary();
    //
    BF_BeamGroup beam2 (fileID,2,true);
    beam2.summary();
    //
    BF_BeamGroup beam3 (fileID,3,true);
    beam3.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*__________________________________________________________________
    Test 3: Create object as copy from another object.
  */
  
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
  
  /*__________________________________________________________________
    Test 4: Create series of beam groups, which are collected within
            a std::map container.
  */

  std::cout << "[4] Testing std::map<std::string,DAL::BF_BeamGroup> ..." << endl;
  try {
    unsigned int nofBeams = 10;
    unsigned int num      = 0;
    std::string name;
    std::map<std::string,DAL::BF_BeamGroup> beams;
    // std::map<std::string,DAL::BF_BeamGroup>::iterator it;

    for (unsigned int n=0; n<nofBeams; ++n) {
      // get the name of the beam group
      num  = 100+n;
      name = BF_BeamGroup::getName (num);
      // create new beam group
      std::cout << "-- creating new beam group " << name << " ..." << endl;
      beams[name] = DAL::BF_BeamGroup (fileID, num, true);
    }
    
    /* Summary */
    std::cout << "--> Created " << beams.size() << " map entries." << endl;
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
//                                                             test_StokesDataset

/*!
  \brief Test creation of and access to Stokes datasets

  \param fileID -- Object identifier for the HDF5 file to work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_StokesDataset (hid_t const &fileID)
{
  cout << "\n[tBF_BeamGroup::test_StokesDataset]\n" << endl;

  int nofFailedTests = 0;
  bool status        = true;
  std::string name   = BF_BeamGroup::getName (1);

  /*__________________________________________________________________
    Open the beam group to which the Stokes datasets will be attached
  */
  
  BF_BeamGroup beam (fileID, name);
  beam.summary();
  
  /*__________________________________________________________________
    Set up variables required for the creation of a Stokes dataset
  */
  
  unsigned int nofSamples  = 1000;
  unsigned int nofSubbands = 36;
  unsigned int nofChannels = 128;
  
  status = beam.openStokesDataset ("Stokes000",
				   nofSamples,
				   nofSubbands,
				   nofChannels,
				   DAL::Stokes::I);
  status = beam.openStokesDataset ("Stokes001",
				   nofSamples,
				   nofSubbands,
				   nofChannels,
				   DAL::Stokes::Q);
  status = beam.openStokesDataset ("Stokes002",
				   nofSamples,
				   nofSubbands,
				   nofChannels,
				   DAL::Stokes::U);
  status = beam.openStokesDataset ("Stokes003",
				   nofSamples,
				   nofSubbands,
				   nofChannels,
				   DAL::Stokes::V);
  beam.summary();
  
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
    // Test creation of and access to Stokes datasets
    nofFailedTests += test_StokesDataset (fileID);
  } else {
    cerr << "-- ERROR: Failed to open file " << filename << endl;
    return -1;
  }
  
  //________________________________________________________
  // close HDF5 file
  
  H5Fclose(fileID);

  return nofFailedTests;
}
