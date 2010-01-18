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
#include <BF_Dataset.h>
#include <BF_StationBeam.h>
#include <BF_PencilBeam.h>

// Namespace usage
using std::cout;
using DAL::CommonAttributes;
using DAL::Filename;
using DAL::BF_Dataset;
using DAL::BF_StationBeam;
using DAL::BF_PencilBeam;

/*!
  \file tBF_PencilBeam.cc

  \ingroup DAL

  \brief A collection of test routines for the BF_PencilBeam class
 
  \author Lars B&auml;hren
 
  \date 2009/10/28
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_PencilBeam object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename)
{
  cout << "\n[tBF_PencilBeam::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing default constructor ..." << std::endl;
  try {
    BF_PencilBeam beam;
    beam.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing argumented constructor ..." << std::endl;
  try {
    // create HDF5 file to which the PencilBeam group is getting attached
    hid_t fileID = H5Fcreate (filename.c_str(),
			      H5F_ACC_TRUNC,
			      H5P_DEFAULT,
			      H5P_DEFAULT);
    
    cout << "--> File ID = " << fileID << std::endl;

    if (fileID>0) {
      BF_PencilBeam beam1 (fileID,1,true);
      beam1.summary();
      //
      BF_PencilBeam beam2 (fileID,2,true);
      beam2.summary();
      //
      BF_PencilBeam beam3 (fileID,3,true);
      beam3.summary();
    }
    H5Fclose (fileID);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes attached to the PencilBeam group

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_attributes (std::string const &filename)
{
  cout << "\n[tBF_PencilBeam::test_attributes]\n" << std::endl;

  int nofFailedTests (0);
  std::string groupname;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open HDF5 file and pencil beam group __________________

  cout << "-- opening file " << filename << " ..." << std::endl;
  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);

  if (fileID > 0) {
    DAL::h5get_names (names,fileID,H5G_GROUP);
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (names.size() > 0) {
    it = names.begin();
    groupname = *it;
  } else {
    std::cerr << "Skipping tests - no station group found." << endl;
    return -1;
  }

  cout << "-- opening PencilBeam " << groupname << " ..." << std::endl;
  BF_PencilBeam beam (fileID,groupname);

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

  cout << "-- reading in attributes attached to group ..." << std::endl;

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

  cout << " -- GROUPTYPE           = " << groupType         << std::endl;
  cout << " -- TARGET              = " << target            << std::endl;
  cout << " -- NOF_STATIONS        = " << nofStations       << std::endl;
  cout << " -- STATIONS_LIST       = " << stationsList      << std::endl;
  cout << " -- POINT_RA            = " << pointRA           << std::endl;
  cout << " -- POINT_DEC           = " << pointDEC          << std::endl;
  cout << " -- POSITION_OFFSET_RA  = " << positionOffsetRA  << std::endl;
  cout << " -- POSITION_OFFSET_DEC = " << positionOffsetDEC << std::endl;
  cout << " -- PB_DIAMETER_RA      = " << beamDiameterRA    << std::endl;
  cout << " -- PB_DIAMETER_DEC     = " << beamDiameterDEC   << std::endl;

  cout << "-- assigning new valeus to the attributes ..." << std::endl;

  pointRA  = 45.0;
  pointDEC = 45.0;

  beam.setAttribute ("POINT_RA",pointRA);
  beam.setAttribute ("POINT_DEC",pointDEC);

  beam.getAttribute ("POINT_RA",pointRA);
  beam.getAttribute ("POINT_DEC",pointDEC);

  cout << " -- POINT_RA            = " << pointRA           << std::endl;
  cout << " -- POINT_DEC           = " << pointDEC          << std::endl;

  // Release HDF5 file object ______________________________

  if (H5Iis_valid(fileID)) H5Fclose (fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);
  std::string filename ("tBF_PencilBeam.h5");

  // Test for the constructor(s)
  nofFailedTests += test_constructors (filename);
  // Test access to the attributes
  nofFailedTests += test_attributes (filename);

  return nofFailedTests;
}
