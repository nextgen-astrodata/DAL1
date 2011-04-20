/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include <core/HDF5Datatype.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::HDF5Datatype;

/*!
  \file tHDF5Datatype.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::HDF5Datatype class
 
  \author Lars B&auml;hren
 
  \date 2011/01/31
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new HDF5Datatype object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tHDF5Datatype::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing default constructor ..." << endl;
  try {
    HDF5Datatype newObject;
    //
    newObject.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                          test_static_functions

/*!
  \brief Test static functions

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_functions ()
{
  cout << "\n[tHDF5Datatype::test_static_functions]\n" << endl;

  int nofFailedTests (0);
  
  /*__________________________________________________________________
    Test 1: 
  */

  cout << "[1] Get map of recognized dataypes ..." << endl;
  try {
    std::map<hid_t,std::string>::iterator it;
    std::map<hid_t,std::string> typesMap = HDF5Datatype::datatypesMap();
    //
    for (it=typesMap.begin(); it!=typesMap.end(); ++it) {
      cout << " [ " << it->first << "  ->  " << it->second << " ]" << endl;
    }
    //
    typesMap.clear();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  /*__________________________________________________________________
    Test 2: Get names for datatypes
  */
  
  cout << "[2] Query datatypes ..." << endl;
  try {
    std::vector<hid_t> types  = HDF5Datatype::datatypes();
    unsigned int nofDatatypes = types.size();

    std::cout << "-- nof. datatypes = " << nofDatatypes << std::endl;

    for (unsigned int n=0; n<nofDatatypes; ++n) {
      cout << " [ "
	   << n
	   << "  ->  " << types[n]
	   << "  ->  " << HDF5Datatype::datatype(types[n])
	   << "  ->  " << HDF5Datatype::datatypeName(types[n])
	   << " ]" 
	   << endl;
    }


  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
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

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  // Test static functions
  nofFailedTests += test_static_functions ();

  return nofFailedTests;
}
