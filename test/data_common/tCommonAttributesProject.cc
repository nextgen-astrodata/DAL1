/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#include <fstream>
#include <CommonAttributesProject.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::CommonAttributesProject;

/*!
  \file tCommonAttributesProject.cc

  \ingroup DAL
  \ingroup data_common

  \brief A collection of test routines for the DAL::CommonAttributesProject class
 
  \author Lars B&auml;hren
 
  \date 2010/12/03
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new CommonAttributesProject object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tCommonAttributesProject::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing CommonAttributesProject() ..." << endl;
  try {
    CommonAttributesProject attr;
    //
    attr.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing CommonAttributesProject(string,string..) ..." << endl;
  try {
    std::string projectID ("LOFAR-001");
    std::string projectTitle ("My little test Project");
    std::string projectPI ("Dr. Frankenstein");
    std::string projectCoI ("Dr. Jeckyl");
    std::string projectContact ("frankenstein@lofar.org");

    CommonAttributesProject attr (projectID,
				  projectTitle,
				  projectPI,
				  projectCoI,
				  projectContact);;

    attr.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_parameters

/*!
  \brief Test access to internal parameters

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_parameters ()
{
  cout << "\n[tCommonAttributesProject::test_parameters]\n" << endl;

  int nofFailedTests (0);
  CommonAttributesProject attr;

  attr.summary();

  attr.setProjectID ("LOFAR-001");
  attr.summary();

  attr.setProjectTitle ("My little test project");
  attr.summary();

  attr.setProjectPI ("Dr. Frankenstein");
  attr.summary();

  attr.setProjectCoI ("Dr. Jeckyl");
  attr.summary();

  attr.setProjectContact ("frankenstein@lofar.org");
  attr.summary();

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_hdf5

/*!
  \brief Test I/O of attributes as HDF5 group

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_hdf5 (std::string const &filename="tCommonAttributes.h5")
{
  cout << "\n[tCommonAttributesProject::test_hdf5]\n" << endl;

  int nofFailedTests (0);
  hid_t fileID (0);
  herr_t h5error (0);
  
  //________________________________________________________
  // Open HDF5 file and group to work with
  
  std::ifstream infile;
  infile.open (filename.c_str(), std::ifstream::in);
  
  if (infile.is_open() && infile.good()) {
    cout << "-- Opening existing file " << filename << " ..." << endl;
    // If the file already exists, close it ...
    infile.close();
    // ... and open it as HDF5 file
    fileID = H5Fopen (filename.c_str(),
		      H5F_ACC_RDWR,
		      H5P_DEFAULT);
  } else {
    cout << "-- Creating new file " << filename << " ..." << endl;
    // If the file does not exist yet, close 
    infile.close();
    // create a new HDF5 file
    fileID = H5Fcreate (filename.c_str(),
			H5F_ACC_TRUNC,
			H5P_DEFAULT,
			H5P_DEFAULT);
  }
  
  
  //________________________________________________________
  // close the HDF5 file once we are done
  h5error = H5Fclose (fileID);
  
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test access to internal parameters
  nofFailedTests += test_parameters ();

#ifdef HAVE_HDF5
  nofFailedTests += test_hdf5 ();
#endif

  return nofFailedTests;
}
