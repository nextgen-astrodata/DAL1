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

#include <core/dalCommon.h>
#include <coordinates/Coordinate.h>

// Namespace usage
using std::cout;
using std::endl;

/*!
  \file tCoordinate.cc

  \ingroup DAL
  \ingroup coordinates

  \brief A collection of test routines for the DAL::Coordinate class
 
  \author Lars B&auml;hren
 
  \date 2010/11/01
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new Coordinate object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tCoordinate::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing Coordinate() ..." << endl;
  try {
    DAL::Coordinate coord;
    //
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing Coordinate(Coordinate::Type) ..." << endl;
  try {
    DAL::Coordinate coordDirection (DAL::Coordinate::DIRECTION);
    coordDirection.summary();
    //
    DAL::Coordinate coordLinear (DAL::Coordinate::LINEAR);
    coordLinear.summary();
    //
    DAL::Coordinate coordTabular (DAL::Coordinate::TABULAR);
    coordTabular.summary();
    //
    DAL::Coordinate coordStokes (DAL::Coordinate::STOKES);
    coordStokes.summary();
    //
    DAL::Coordinate coordSpectral (DAL::Coordinate::SPECTRAL);
    coordSpectral.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing Coordinate(Coordinate) ..." << endl;
  try {
    DAL::Coordinate coordStokes (DAL::Coordinate::STOKES);
    coordStokes.summary();
    //
    DAL::Coordinate coord (coordStokes);
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                   test_methods

/*!
  \brief Test for the various public methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_methods ()
{
  cout << "\n[tCoordinate::test_methods]\n" << endl;

  int nofFailedTests (0);
  bool status (true);
  DAL::Coordinate coord;

  coord.summary();

  cout << "[1] Testing setType(Coordinate::Type) ..." << endl;
  try {
    status = coord.setType (DAL::Coordinate::DIRECTION);
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
    //
    status = coord.setType (DAL::Coordinate::LINEAR);
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
    //
    status = coord.setType (DAL::Coordinate::TABULAR);
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
    //
    status = coord.setType (DAL::Coordinate::STOKES);
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
    //
    status = coord.setType (DAL::Coordinate::SPECTRAL);
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Testing setType(std::string) ..." << endl;
  try {
    status = coord.setType ("DIRECTION");
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
    //
    status = coord.setType ("LINEAR");
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
    //
    status = coord.setType ("TABULAR");
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
    //
    status = coord.setType ("STOKES");
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
    //
    status = coord.setType ("SPECTRAL");
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
    //
    status = coord.setType ("J2000");
    cout << "-- Coordinate type = " << coord.type() << "/" << coord.name() << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                            test_static_methods

/*!
  \brief Test for the various static methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_methods ()
{
  cout << "\n[tCoordinate::test_static_methods]\n" << endl;

  int nofFailedTests (0);

  cout << "[1] Testing coordinatesMap() ..." << endl;
  try {
    std::map<DAL::Coordinate::Type,std::string> coord;

    coord = DAL::Coordinate::coordinatesMap();

    cout << "-- nof. elements = " << coord.size() << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Testing coordinatesType() ..." << endl;
  try {
    unsigned int nelem;
    std::vector<DAL::Coordinate::Type> coord;
    
    coord = DAL::Coordinate::coordinatesType();
    nelem = coord.size();
    //
    cout << "-- nof. coord. types = " << nelem  << endl;
    cout << "-- coordinate types  = " << coord  << endl;
      
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] Testing coordinatesName() ..." << endl;
  try {
    std::vector<std::string> names = DAL::Coordinate::coordinatesName();
    //
    cout << "-- nof. coord. types = " << names.size() << endl;
    cout << "-- coordinate names  = " << names        << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[4] Testing getType() ..." << endl;
  try {
    std::vector<std::string> names = DAL::Coordinate::coordinatesName();
    //
    for (unsigned int n(0); n<names.size(); ++n) {
      cout << "\t" << names[n] << "\t->\t" << DAL::Coordinate::getType(names[n])
	   << endl;
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[5] Testing getName() ..." << endl;
  try {
    std::vector<DAL::Coordinate::Type> types = DAL::Coordinate::coordinatesType();
    //
    for (unsigned int n(0); n<types.size(); ++n) {
      cout << "\t" << types[n] << "\t->\t" << DAL::Coordinate::getName(types[n])
	   << endl;
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[6] Testing hasProjection() ..." << endl;
  try {
    std::vector<DAL::Coordinate::Type> types = DAL::Coordinate::coordinatesType();
    //
    for (unsigned int n(0); n<types.size(); ++n) {
      cout << "\t" << types[n] << "\t->\t" << DAL::Coordinate::hasProjection(types[n])
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

int main ()
{
  int nofFailedTests (0);

  // Test for the various static methods
  nofFailedTests += test_static_methods ();
  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test for the various public methods
  nofFailedTests += test_methods ();

  return nofFailedTests;
}
