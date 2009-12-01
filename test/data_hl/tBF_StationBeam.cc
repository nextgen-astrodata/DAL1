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

// Namespace usage
using DAL::CommonAttributes;
using DAL::Filename;
using DAL::BF_Dataset;
using DAL::BF_StationBeam;

/*!
  \file tBF_StationBeam.cc

  \ingroup DAL

  \brief A collection of test routines for the BF_StationBeam class
 
  \author Lars B&auml;hren
 
  \date 2009/10/28
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new BF_StationBeam object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tBF_StationBeam::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  std::string filename ("tBF_StationBeam.h5");
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    BF_StationBeam beam;
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
      BF_StationBeam beam1 (fileID,1,true);
      BF_StationBeam beam2 (fileID,2,true);
      BF_StationBeam beam3 (fileID,3,true);
      //
      beam1.summary();
    }
    H5Fclose (fileID);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
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
