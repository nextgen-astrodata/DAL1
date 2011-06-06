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

#include <data_common/HDF5Measure.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::HDF5Measure;

/*!
  \file tHDF5Measure.cc

  \ingroup DAL
  \ingroup data_common

  \brief A collection of test routines for the HDF5Measure class
  
  \author Lars B&auml;hren
  
  \date 2011/06/01
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new HDF5Measure object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tHDF5Measure::test_constructors]" << endl;

  int nofFailedTests (0);
  
  //________________________________________________________
  // Test 1 : Default constructor

  cout << "\n[1] Testing HDF5Measure() ..." << endl;
  try {
    HDF5Measure m;
    //
    m.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 2 : Argumented constructor

  cout << "\n[2] Testing HDF5Measure(string) ..." << endl;
  try {
    HDF5Measure m ("TIME");
    //
    m.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  //________________________________________________________
  // Test 3 : Argumented constructor

  cout << "\n[3] Testing HDF5Measure(string) ..." << endl;
  try {
    HDF5Measure m ("TIME",1.0,"s","UTC");
    //
    m.summary(); 
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
