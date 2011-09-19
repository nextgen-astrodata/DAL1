/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include <data_hl/Sky_ImageDataset.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::Sky_ImageDataset;

/*!
  \file tSky_ImageDataset.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the Sky_ImageDataset class
 
  \author Lars B&auml;hren
 
  \date 2011/02/01
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new Sky_ImageDataset object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &fileID,
		       std::string const &nameDataset)
{
  cout << "\n[tSky_ImageDataset::test_constructors]" << endl;

  int nofFailedTests = 0;
  
  cout << "\n[1] Testing Sky_ImageDataset() ..." << endl;
  {
    Sky_ImageDataset sky;
    //
    sky.summary(); 
  }
  
  cout << "\n[2] Testing Sky_ImageDataset(hid_t,string) ..." << endl;
  {
    Sky_ImageDataset sky (fileID, nameDataset);
    //
    sky.summary(); 
  }

  
  cout << "\n[3] Testing Sky_ImageDataset(hid_t,string,vector<hsize_t>) ..." << endl;
  {
    unsigned int rank = 3;
    std::vector<hsize_t> shape (rank);

    shape[0] = 1024;
    shape[1] = 1024;
    shape[2] = 512;

    Sky_ImageDataset sky (fileID, nameDataset, shape);

    sky.summary(); 
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
int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  hid_t fileID         = 0;
  bool haveDataset     = false;
  std::string filename = "tSky_ImageDataset.h5";
  std::string nameDataset = "IMAGE_000";

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }
  
  //________________________________________________________
  // Open/create HDF5 file to work with
  
  if (haveDataset) {
    fileID = H5Fopen (filename.c_str(),
		      H5F_ACC_RDWR,
		      H5P_DEFAULT);
  } else {
    fileID = H5Fcreate (filename.c_str(),
			H5F_ACC_TRUNC,
			H5P_DEFAULT,
			H5P_DEFAULT);
  }
  
  //________________________________________________________
  // Run the tests
  
  // Test for the constructor(s)
  nofFailedTests += test_constructors (fileID,
				       nameDataset);

  return nofFailedTests;
}
