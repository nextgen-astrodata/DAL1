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
  std::cout << "\n[tTBB_DipoleDataset::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  std::string filename ("tTBB_DipoleDataset.h5");
  hid_t fileID = H5Fcreate (filename.c_str(),
			    H5F_ACC_TRUNC,
			    H5P_DEFAULT,
			    H5P_DEFAULT);
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    TBB_DipoleDataset dataset;
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing argumented constructor ..." << std::endl;
  try {
    TBB_DipoleDataset dataset (fileID,"DipoleDataset");
    dataset.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Testing argumented constructor ..." << std::endl;
  try {
    uint station (0);
    uint rsp (0);
    uint rcu (0);
    std::string name;
    std::vector<hsize_t> shape (1,1024);
    std::map<std::string,TBB_DipoleDataset> datasets;
    //
    for (; rcu<5; ++rcu) {
      name = TBB_DipoleDataset::channelName(station,rsp,rcu);
      datasets[name] = TBB_DipoleDataset (fileID,station,rsp,rcu,shape);
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Reopening previously created datasets ..." << std::endl;
  try {
//     TBB_DipoleDataset dataset1 (fileID,"000000001");
//     dataset1.setAttribute("RCU_ID",uint(1));
//     //
//     TBB_DipoleDataset dataset2 (fileID,"000001002");
//     dataset2.setAttribute("RSP_ID",uint(1));
//     dataset2.setAttribute("RCU_ID",uint(2));
//     //
//     TBB_DipoleDataset dataset3 (fileID,"000002002");
//     //
//     dataset1.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  // close the previously opened HDF5 file
  H5Fclose (fileID);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  return nofFailedTests;
}
