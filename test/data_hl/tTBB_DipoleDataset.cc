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

#include <dalCommon.h>
#include <TBB_DipoleDataset.h>

// Namespace usage
using DAL::TBB_DipoleDataset;
using std::endl;

/*!
  \file tTBB_DipoleDataset_test1.cc

  \ingroup DAL

  \brief A collection of test routines for the TBB_DipoleDataset class
 
  \author Lars B&auml;hren
 
  \date 2009/10/28
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new TBB_DipoleDataset object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tTBB_DipoleDataset::test_constructors]\n" << endl;

  int nofFailedTests (0);
  uint station (0);
  uint rsp (0);
  uint rcu (0);
  std::string filename ("tTBB_DipoleDataset.h5");
  std::string groupname ("Station001");
  hid_t fileID;
  hid_t groupID;
  herr_t h5error;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open/Create HDF5 file _________________________________

  std::cout << "-- Opening file " << filename << "  ..." << endl;
  
  fileID = H5Fcreate (filename.c_str(),
		      H5F_ACC_TRUNC,
		      H5P_DEFAULT,
		      H5P_DEFAULT);
  
  if (fileID < 0) {
    std::cerr << "ERROR : Failed to open/create file." << endl;
    return -1;
  }
  
  // Open/create group _____________________________________

  if (fileID > 0) {
    names.clear();
    DAL::h5get_names (names,fileID,H5G_GROUP);
    //
    if (names.size() > 0) {
      it        = names.begin();
      groupname = *it;
      groupID   = H5Gopen (fileID,
			   groupname.c_str(),
			   H5P_DEFAULT);
    } else {
      groupID   = H5Gcreate (fileID,
			     groupname.c_str(),
			     H5P_DEFAULT,
			     H5P_DEFAULT,
			     H5P_DEFAULT);
    }
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }
  
  if (groupID < 0) {
    std::cerr << "ERROR : Failed to open/create group." << endl;
    return -1;
  }
  
  // Perform the tests _____________________________________
  
  std::cout << "[1] Testing default constructor ..." << endl;
  try {
    TBB_DipoleDataset dataset;
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing argumented constructor ..." << endl;
  try {
    TBB_DipoleDataset dataset (fileID,"DipoleDataset");
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Testing argumented constructor ..." << endl;
  try {
    std::string name;
    std::vector<hsize_t> shape (1,1024);
    //
    station = 0;
    rsp     = 0;
    for (rcu=0; rcu<5; ++rcu) {
      name = TBB_DipoleDataset::channelName(station,rsp,rcu);
      TBB_DipoleDataset data (fileID,station,rsp,rcu,shape);
      std::cout << "-- created dataset " << name << endl;
    }
    //
    station = 1;
    for (rsp=0; rsp<5; ++rsp) {
      for (rcu=0; rcu<5; ++rcu) {
	name = TBB_DipoleDataset::channelName(station,rsp,rcu);
	TBB_DipoleDataset data (groupID,station,rsp,rcu,shape);
	std::cout << "-- created dataset " << name << endl;
      }
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[4] Reopening previously created datasets ..." << endl;
  try {
    std::string name;
    //
    station = 0;
    rsp     = 0;
    for (rcu=0; rcu<5; ++rcu) {
      name = TBB_DipoleDataset::channelName(station,rsp,rcu);
      TBB_DipoleDataset data (fileID,station,rsp,rcu);
      std::cout << "-- opened dataset " << name << endl;
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  // release HDF5 object identifiers _______________________

  h5error = H5Gclose (groupID);
  h5error = H5Fclose (fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new TBB_DipoleDataset object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename)
{
  std::cout << "\n[tTBB_DipoleDataset::test_constructors]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID;
  hid_t groupID;
  herr_t h5error;
  std::set<std::string> names;
  std::set<std::string>::iterator it;

  // Open HDF5 file and embedeed groups ____________________

  fileID = H5Fopen (filename.c_str(),
		    H5F_ACC_RDWR,
		    H5P_DEFAULT);

  if (fileID > 0) {
    names.clear();
    DAL::h5get_names (names,fileID,H5G_GROUP);
    //
    if (names.size() > 0) {
      std::cout << "-- Groups  = " << names << endl;
      it      = names.begin();
      groupID = H5Gopen (fileID,
			 it->c_str(),
			 H5P_DEFAULT);
    } else {
      std::cerr << "Skipping tests - unable to open group." << endl;
      return -1;
    }
  } else {
    std::cerr << "Skipping tests - unable to open file." << endl;
    return -1;
  }

  if (groupID > 0) {
    names.clear();
    DAL::h5get_names (names,groupID,H5G_DATASET);
    std::cout << "-- Datasets = " << names << endl;
  } else {
    std::cerr << "Skipping tests - unable to open group." << endl;
    return -1;
  }

  // Open the dipole datasets ______________________________

  if (names.size() > 0) {
    for (it=names.begin(); it!=names.end(); ++it) {
      std::cout << "-- opening dataset " << *it << endl;
      TBB_DipoleDataset data (groupID,*it);
      data.summary();
    }
  } else {
    std::cerr << "Skipping tests - no datasets found." << endl;
    return -1;
  }
  
  // Release HDF5 object identifiers _______________________
  
  h5error = H5Fclose (fileID);
  h5error = H5Gclose (groupID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc, char *argv[])
{
  int nofFailedTests (0);
  bool haveDataset (true);
  std::string nameFile ("UNDEFINED");
  std::string nameStation ("UNDEFINED");
  std::string nameDataset ("UNDEFINED");

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    nameFile = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Run the tests

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  if (haveDataset) {
    // Test for the constructor(s)
    nofFailedTests += test_constructors (nameFile);
  } else {
    std::cout << "\n[tTBB_DipoleDataset] Skipping tests which input dataset.\n"
	      << endl;
  }

  return nofFailedTests;
}
