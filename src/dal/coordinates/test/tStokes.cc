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

#include <coordinates/Stokes.h>

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
  Stokes stokes;

  std::cout << "[1] Testing setType(Stokes::Component) ..." << endl;
  try {
    stokes.setType(Stokes::R);
    stokes.summary();
    //
    stokes.setType(Stokes::X);
    stokes.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[2] Testing setType(std::string) ..." << endl;
  try {
    stokes.setType("I");
    stokes.summary();
    //
    stokes.setType("Q");
    stokes.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                            test_static_methods

/*!
  \brief Test the various static methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_methods ()
{
  std::cout << "\n[tStokes::test_static_methods]\n" << endl;

  int nofFailedTests (0);

  std::cout << "[1] Testing componentsMap() ..." << endl;
  try {
    std::map<Stokes::Component,std::string> components = Stokes::componentsMap();
    std::map<Stokes::Component,std::string>::iterator it;
    std::vector<Stokes::Component> types;
    std::vector<std::string> names;
    //
    for (it=components.begin(); it!=components.end(); ++it){
      types.push_back(it->first);
      names.push_back(it->second);
    }
    //
    std::cout << "-- nof. Stokes components = " << components.size() << endl;
    std::cout << "-- Stokes component types = " << types             << endl;
    std::cout << "-- Stokes component names = " << names             << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing componentsType() ..." << endl;
  try {
    std::vector<Stokes::Component> types = Stokes::componentsType();
    //
    std::cout << "-- nof. Stokes components = " << types.size() << endl;
    std::cout << "-- Stokes component types = " << types        << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Testing componentsName() ..." << endl;
  try {
    std::vector<std::string> names = Stokes::componentsName();
    //
    std::cout << "-- nof. Stokes components = " << names.size() << endl;
    std::cout << "-- Stokes component names = " << names        << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[4] Testing isLinear() ..." << endl;
  try {
    std::cout << "-- isLinear(Stokes::I) = " << Stokes::isLinear(Stokes::I) << endl;
    std::cout << "-- isLinear(Stokes::Q) = " << Stokes::isLinear(Stokes::Q) << endl;
    std::cout << "-- isLinear(Stokes::U) = " << Stokes::isLinear(Stokes::U) << endl;
    std::cout << "-- isLinear(Stokes::V) = " << Stokes::isLinear(Stokes::V) << endl;
    std::cout << "-- isLinear(Stokes::R) = " << Stokes::isLinear(Stokes::R) << endl;
    std::cout << "-- isLinear(Stokes::X) = " << Stokes::isLinear(Stokes::X) << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[5] Testing isCircular() ..." << endl;
  try {
    std::cout << "-- isCircular(Stokes::I) = " << Stokes::isCircular(Stokes::I) << endl;
    std::cout << "-- isCircular(Stokes::Q) = " << Stokes::isCircular(Stokes::Q) << endl;
    std::cout << "-- isCircular(Stokes::U) = " << Stokes::isCircular(Stokes::U) << endl;
    std::cout << "-- isCircular(Stokes::V) = " << Stokes::isCircular(Stokes::V) << endl;
    std::cout << "-- isCircular(Stokes::R) = " << Stokes::isCircular(Stokes::R) << endl;
    std::cout << "-- isCircular(Stokes::X) = " << Stokes::isCircular(Stokes::X) << endl;
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
  // Test the static methods
  nofFailedTests += test_static_methods ();

  return nofFailedTests;
}
