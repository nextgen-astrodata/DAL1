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

#include <Stokes.h>

// Namespace usage
using std::endl;
using DAL::Stokes;

/*!
  \file tStokes.cc

  \ingroup DAL
  \ingroup coordinates

  \brief A collection of test routines for the Stokes class
 
  \author Lars B&auml;hren
 
  \date 2010/10/22
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new Stokes object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tStokes::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing Stokes() ..." << endl;
  try {
    Stokes stokes;
    //
    stokes.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing Stokes(Stokes::Component) ..." << endl;
  try {
    Stokes stokesI (Stokes::I);
    stokesI.summary();
    Stokes stokesQ (Stokes::Q);
    stokesQ.summary();
    Stokes stokesU (Stokes::U);
    stokesU.summary();
    Stokes stokesV (Stokes::V);
    stokesV.summary();
    Stokes stokesR (Stokes::R);
    stokesR.summary();
    Stokes stokesL (Stokes::L);
    stokesL.summary();
    Stokes stokesRR (Stokes::RR);
    stokesRR.summary();
    Stokes stokesRL (Stokes::RL);
    stokesRL.summary();
    Stokes stokesX (Stokes::X);
    stokesX.summary();
    Stokes stokesY (Stokes::Y);
    stokesY.summary();
    Stokes stokesXX (Stokes::XX);
    stokesXX.summary();
    Stokes stokesXY (Stokes::XY);
    stokesXY.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing Stokes(Stokes) ..." << endl;
  try {
    Stokes stokesI (Stokes::I);
    Stokes stokesIcopy (stokesI);
    stokesIcopy.summary();
    //
    Stokes stokesQ (Stokes::Q);
    Stokes stokesQcopy (stokesQ);
    stokesQcopy.summary();
    //
    Stokes stokesU (Stokes::U);
    Stokes stokesUcopy (stokesU);
    stokesUcopy.summary();
    //
    Stokes stokesR (Stokes::R);
    Stokes stokesRcopy (stokesR);
    stokesRcopy.summary();
    //
    Stokes stokesX (Stokes::X);
    Stokes stokesXcopy (stokesX);
    stokesXcopy.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

/*!
  \brief Test the various object methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_methods ()
{
  std::cout << "\n[tStokes::test_methods]\n" << endl;

  int nofFailedTests (0);

  // objects with which to perform the tests
  Stokes stokesI (Stokes::I);
  Stokes stokesQ (Stokes::Q);
  Stokes stokesU (Stokes::U);
  Stokes stokesV (Stokes::V);
  Stokes stokesR (Stokes::R);
  Stokes stokesL (Stokes::L);
  Stokes stokesX (Stokes::X);
  Stokes stokesY (Stokes::Y);
  
  std::cout << "[1] Testing Stokes::I component ..." << endl;
  try {
    std::cout << "-- type()       = " << stokesI.type()       << endl;
    std::cout << "-- name()       = " << stokesI.name()       << endl;
    std::cout << "-- isLinear()   = " << stokesI.isLinear()   << endl;
    std::cout << "-- isCircular() = " << stokesI.isCircular() << endl;
    std::cout << "-- isParallel() = " << stokesI.isParallel() << endl;
    std::cout << "-- isCross()    = " << stokesI.isCross()    << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[2] Testing Stokes::X component ..." << endl;
  try{
    std::cout << "-- type()       = " << stokesX.type()       << endl;
    std::cout << "-- name()       = " << stokesX.name()       << endl;
    std::cout << "-- isLinear()   = " << stokesX.isLinear()   << endl;
    std::cout << "-- isCircular() = " << stokesX.isCircular() << endl;
    std::cout << "-- isParallel() = " << stokesX.isParallel() << endl;
    std::cout << "-- isCross()    = " << stokesX.isCross()    << endl;
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

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test the various methods
  nofFailedTests += test_methods ();

  return nofFailedTests;
}
