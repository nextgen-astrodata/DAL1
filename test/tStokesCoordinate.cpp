/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 2286 2009-02-03 10:50:48Z baehren                  $ |
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

#include <StokesCoordinate.h>

/*!
  \file tStokesCoordinate.cc

  \ingroup DAL

  \brief A collection of test routines for the StokesCoordinate class
 
  \author Lars B&auml;hen
 
  \date 2009/09/13
*/

//_______________________________________________________________________________
//                                                          test_static_functions

/*!
  \brief Test the static functions provided by the class

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_functions ()
{
  std::cout << "\n[tStokesCoordinate::test_static_functions]\n" << std::endl;

  int nofFailedTests (0);

  std::vector<DAL::StokesCoordinate::Stokes> types;
  std::vector<std::string> names;

  types.push_back(DAL::StokesCoordinate::I);
  types.push_back(DAL::StokesCoordinate::Q);
  types.push_back(DAL::StokesCoordinate::U);
  types.push_back(DAL::StokesCoordinate::V);
  types.push_back(DAL::StokesCoordinate::RR);
  types.push_back(DAL::StokesCoordinate::LL);
  types.push_back(DAL::StokesCoordinate::RL);
  types.push_back(DAL::StokesCoordinate::LR);

  names.push_back("I");
  names.push_back("Q");
  names.push_back("U");
  names.push_back("V");
  names.push_back("RR");
  names.push_back("LL");
  names.push_back("RL");
  names.push_back("LR");

  std::cout << "[1] Get name from type ..." << std::endl;
  try {
    for (unsigned int n(0); n<types.size(); n++) {
      std::cout << "\t"
		<< types[n]
		<< " -> "
		<< DAL::StokesCoordinate::getName(types[n])
		<< std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[1] Get type from name ..." << std::endl;
  try {
    for (unsigned int n(0); n<names.size(); n++) {
      std::cout << "\t"
		<< names[n]
		<< " -> "
		<< DAL::StokesCoordinate::getType(names[n])
		<< std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new StokesCoordinate object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tStokesCoordinate::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    DAL::StokesCoordinate coord;
    //
    coord.summary(); 
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

  // Test the static functions defined as part of the class
  nofFailedTests += test_static_functions();
  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  return nofFailedTests;
}
