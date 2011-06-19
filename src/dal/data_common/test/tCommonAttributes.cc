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

// Namespace usage
using std::cout;
using std::endl;
using DAL::CommonAttributes;

/*!
  \file tCommonAttributes.cc

  \ingroup DAL
  \ingroup data_common

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
  DAL::Filename filename ("123456789","",DAL::Filename::uv,DAL::Filename::h5);

  /*________________________________________________________
    Test 1 : Testing default constructor
  */
  
  cout << "[1] Testing CommonAttributes() ..." << endl;
  try {
    CommonAttributes attributes;
    //
    attributes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 2 : Testing argumented constructor, taking
             DAL::Filename object as input
  */
  
  cout << "[2] Testing CommonAttributes(Filename) ..." << endl;
  try {
    CommonAttributes attributes (filename);
    //
    attributes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 3 : Testing argumented constructor, taking
             DAL::Filename object and filedate as input.
  */
  
  cout << "[3] Testing CommonAttributes(Filename,string) ..." << endl;
  try {
    std::string filedate ("2009-10-10T00:00:00.0");
    CommonAttributes attributes (filename,
				 filedate);
    //
    attributes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 4 : Testing argumented constructor, taking basic
             project attributes as input
  */
  
  cout << "[4] Testing CommonAttributes(Filename,string) ..." << endl;
  try {
    std::string projectID      = "DAL";
    std::string projectTitle   = "Data Access Library";
    std::string projectPI      = "Lars Baehren";
    std::string projectCoI     = "Sven Duscha";
    std::string projectContact = "dal@lofar.org";
    //
    CommonAttributes attributes (projectID,
				 projectTitle,
				 projectPI,
				 projectCoI,
				 projectContact);
    //
    attributes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 5 : Testing argumented constructor.
  */
  
  cout << "[5] Testing copy constructor ..." << endl;
  try {
    std::string filedate ("2009-10-10T00:00:00.0");
    CommonAttributes attributes (filename);
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
//                                                                   test_methods

/*!
  \brief Test access to the individual attributes

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_methods (hid_t const &fileID)
{
  cout << "\n[tCommonAttributes::test_methods]" << endl;

  int nofFailedTests (0);
  DAL::Filename filename ("123456789","",DAL::Filename::uv,DAL::Filename::h5);
  CommonAttributes attr;

  /*__________________________________________________________________
    Assign values to the common attributes
  */
  
  cout << "\n[1] Assign new values to the attributes ..." << endl;
  try {
    std::vector<std::string> stations;
    stations.push_back("CS001");
    stations.push_back("CS003");
    stations.push_back("CS005");

    attr.setFilename (filename);
    attr.setFiledate ("2009-10-10T01:00:00.0");
    attr.setTelescope ("LOFAR");
    attr.setProjectID ("CR-2009-10-10");
    attr.setProjectTitle ("CR test observation");
    attr.setProjectPI ("Mr. CR");
    attr.setProjectCoI ("Mrs. CR");
    attr.setProjectContact ("cr@lofar.org");
    attr.setObserver("Mr. LOFAR");
    // attr.setObservationID ("1234567890");
    attr.setAntennaSet ("LBA_X");
    attr.setFilterSelection ("LBH_30_80");
    attr.setClockFrequency (200);
    attr.setClockFrequencyUnit ("MHz");
    attr.setTarget ("Everything under the sky");
    attr.setSystemVersion ("LOFAR system v1.0");
    attr.setPipelineName ("CR_Offline");
    attr.setPipelineVersion ("v0.9");
    // attr.setStationsList (stations);
    attr.setNotes ("Life is a Lemmon");
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*__________________________________________________________________
    Show the new values of the attributes for later comparison
  */
  
  cout << "\n[2] Show new values of the attributes ..." << endl;
  try {
    attr.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "\n[3] Create a copy of the object storing the attributes ..." << endl;
  try {
    CommonAttributes attrCopy (attr);
    //
    attrCopy.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  // Write attributes to HDF5 file _________________________

  cout << "\n[4] Write attributes to file ..." << endl;
  try {
    attr.h5write (fileID);
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "\n[5] Read the attributes back in from file ..." << endl;
  try {
    CommonAttributes attributeNew;
    // read the attributes from the file ...
    attributeNew.h5write (fileID);
    // ... and display them
    attributeNew.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*__________________________________________________________________
    Retrieve attributes through casa::Record
  */

#ifdef DAL_WITH_CASA

  cout << "[6] Testing getAttributes(casa::Record) ..." << endl;
  {
    casa::Record rec;

    attr.getAttributes (rec);
    
    cout << rec << endl;
  }

#endif
  
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
  std::string filename = "tCommonAttributes.h5";
  bool haveDataset     = false;

  //________________________________________________________
  // Process command line parameters

  if (argc>1) {
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
  nofFailedTests += test_constructors ();
  
  // if (H5Iis_valid(fileID)) {
  //   // Test access to the individual attributes
  //   nofFailedTests += test_methods (fileID);
  // }
  
  //________________________________________________________
  // Release HDF5 objects
  
  if (H5Iis_valid(fileID)) {
    H5Fclose(fileID);
  }
  
  return nofFailedTests;
}
