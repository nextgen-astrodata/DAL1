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

#include <core/dalData.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::dalData;

/*!
  \file tdalData.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::dalData class
 
  \author Lars B&auml;hren
 
  \date 2011-07-17
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new dalData object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tdalData::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  /*________________________________________________________
    Test 1 : Default constructor
  */
  
  cout << "[1] Testing dalData() ..." << endl;
  try {
    dalData dataObject;
    //
    dataObject.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 2 : Argumented constructor
  */
  
  cout << "[2] Testing dalData(dalFileType::Type,string,vector,long) ..." << endl;
  try {
    DAL::dalFileType::Type filetype (DAL::dalFileType::HDF5);
    std::string datatype (DAL::dal_INT);
    std::vector<int> shape (2,10);
    long nofRows (100);
    //
    dalData dataObject (filetype,
			datatype,
			shape,
			nofRows);
    //
    dataObject.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 3 : Argumented constructor
  */
  
  cout << "[3] Testing dalData(string,string,vector,long) ..." << endl;
  try {
    std::string filetype ("HDF5");
    std::string datatype (DAL::dal_INT);
    std::vector<int> shape (2,10);
    long nofRows (100);
    //
    dalData dataObject (filetype,
			datatype,
			shape,
			nofRows);
    //
    dataObject.summary(); 
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

  return nofFailedTests;
}
