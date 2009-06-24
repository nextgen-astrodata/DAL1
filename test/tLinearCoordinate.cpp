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

#include <LinearCoordinate.h>

/*!
  \file tLinearCoordinate.cc

  \ingroup DAL

  \brief A collection of test routines for the LinearCoordinate class
 
  \author Lars B&auml;hren
 
  \date 2009/06/24
*/

// -----------------------------------------------------------------------------

/*!
  \brief Test constructors for a new LinearCoordinate object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tLinearCoordinate::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    DAL::LinearCoordinate coord;
    //
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing argumented constructor ..." << std::endl;
  try {
    DAL::LinearCoordinate coord (3);
    //
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing fully argumented constructor ..." << std::endl;
  try {
    unsigned int nofAxes (2);
    std::vector<std::string> worldAxisNames (nofAxes);
    std::vector<std::string> worldAxisUnits (nofAxes);
    std::vector<double> refValue (nofAxes,0.0);
    std::vector<double> refPixel (nofAxes,0.0);
    std::vector<double> increment (nofAxes,1.0);
    std::vector<double> pc (nofAxes*nofAxes);
    //
    worldAxisNames[0] = "Length";
    worldAxisNames[1] = "Time";
    worldAxisUnits[0] = "m";
    worldAxisUnits[1] = "s";
    //
    DAL::LinearCoordinate coord (nofAxes,
				 worldAxisNames,
				 worldAxisUnits,
				 refValue,
				 refPixel,
				 increment,
				 pc);
    //
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  return nofFailedTests;
}
