/***************************************************************************
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

#include <coordinates/Coordinate.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::Coordinate;

/*!
  \file tCoordinate.cc

  \ingroup DAL
  \ingroup coordinates

  \brief A collection of test routines for the DAL::Coordinate class
 
  \author Lars B&auml;hren
 
  \date 2010/11/01
*/

//_______________________________________________________________________________
//                                                            test_static_methods

/*!
  \brief Test the various static methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_methods ()
{
  std::cout << "\n[tCoordinate::test_static_methods]\n" << endl;

  int nofFailedTests (0);

  std::cout << "[1] Testing getName() ..." << endl;
  try {

    cout << "-- Coordinate::Direction = "
	 << Coordinate::getName(Coordinate::DIRECTION) << endl;
    cout << "-- Coordinate::Linear    = "
	 << Coordinate::getName(Coordinate::LINEAR) << endl;
    cout << "-- Coordinate::Tabular   = "
	 << Coordinate::getName(Coordinate::TABULAR) << endl;
    cout << "-- Coordinate::Stokes    = "
	 << Coordinate::getName(Coordinate::STOKES) << endl;
    cout << "-- Coordinate::Tabular   = "
	 << Coordinate::getName(Coordinate::TABULAR) << endl;

  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test the static methods
  nofFailedTests += test_static_methods ();

  return nofFailedTests;
}
