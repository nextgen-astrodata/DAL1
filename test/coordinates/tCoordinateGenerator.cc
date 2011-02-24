/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#include <coordinates/CoordinateGenerator.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::CoordinateGenerator;

/*!
  \file tCoordinateGenerator.cc

  \ingroup DAL
  \ingroup coordinates

  \brief A collection of test routines for the DAL::CoordinateGenerator class
 
  \author Lars B&auml;hren
 
  \date 2010/06/08
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new CoordinateGenerator object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tCoordinateGenerator::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    CoordinateGenerator coord;
    //
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                               test_coordinates

#ifdef DAL_WITH_CASA

/*!
  \brief Test generation of the various coordinate objects

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_coordinates ()
{
  std::cout << "\n[tCoordinateGenerator::test_coordinates]\n" << std::endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing casa::DirectionCoordinate ..." << endl;
  try {
    casa::DirectionCoordinate coord;

    casa::Vector<casa::Quantum<double> > refValue (2);
    casa::Vector<casa::Quantum<double> > increment (2);
    casa::Vector<double> refPixel (2);

    refValue(0)  = casa::Quantum<double> (90.0, "deg");
    refValue(1)  = casa::Quantum<double> ( 0.0, "deg");
    increment(0) = casa::Quantum<double> ( 1.0, "deg");
    increment(1) = casa::Quantum<double> ( 1.0, "deg");
    refPixel(0)  = 100.0;
    refPixel(1)  = 100.0;

    CoordinateGenerator::makeCoordinate (coord,
					 casa::MDirection::AZEL,
					 casa::Projection::STG,
					 refValue,
					 increment,
					 refPixel);

    CoordinateGenerator::summary (coord);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing casa::LinearCoordinate ..." << endl;
  try {
    casa::LinearCoordinate coord;
    unsigned int nofAxes (3);
    casa::Vector<casa::String> names (nofAxes,"Length");
    casa::Vector<casa::String> units (nofAxes,"m");

    CoordinateGenerator::makeCoordinate (coord,
					 nofAxes,
					 names,
					 units);

    CoordinateGenerator::summary (coord);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing casa::LinearCoordinate ..." << endl;
  try {
    casa::LinearCoordinate coord;
    unsigned int nofAxes (2);

    CoordinateGenerator::makeCoordinate (coord,
					 nofAxes);

    CoordinateGenerator::summary (coord);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  cout << "[4] Testing casa::TabularCoordinate ..." << endl;
  try {
    double refPixel   = 1.0;
    double increment  = 0.5;
    double refValue   = 2.0;
    casa::String unit = "km";
    casa::String name = "Radius";

    casa::TabularCoordinate coord (refPixel,
				   increment,
				   refValue,
				   unit,
				   name);

    CoordinateGenerator::summary (coord);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  cout << "[5] Testing casa::TabularCoordinate ..." << endl;
  try {
    int nelem (10);
    casa::Vector<double> pixelValues (nelem);
    casa::Vector<double> worldValues (nelem);
    casa::String unit = "km";
    casa::String name = "Radius";

    // regularly spaced world axis points
    for (int n(0); n<nelem; ++n) {
      pixelValues(n) = n;
      worldValues(n) = 1.23456*n;
    }

    casa::TabularCoordinate coord (pixelValues,
				   worldValues,
				   unit,
				   name);

    CoordinateGenerator::summary (coord);
    cout << "-- Pixel axis values = " << pixelValues << endl;
    cout << "-- World axis values = " << worldValues << endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  cout << "[6] Testing casa::TabularCoordinate ..." << endl;
  try {
    int nelem (10);
    casa::Vector<double> pixelValues (nelem);
    casa::Vector<double> worldValues (nelem);
    casa::String unit = "km";
    casa::String name = "Radius";

    for (int n(0); n<nelem; ++n) {
      pixelValues(n) = n;
    }

    /* non-regular gridding along the world axis coordinates */
    worldValues(0) =  0.0;
    worldValues(1) =  1.0;
    worldValues(2) =  4.0;
    worldValues(3) =  9.0;
    worldValues(4) = 16.0;
    worldValues(5) = 25.0;
    worldValues(6) = 36.0;
    worldValues(7) = 49.0;
    worldValues(8) = 64.0;
    worldValues(9) = 81.0;

    casa::TabularCoordinate coord (pixelValues,
				   worldValues,
				   unit,
				   name);

    CoordinateGenerator::summary (coord);
    cout << "-- Pixel axis values = " << pixelValues << endl;
    cout << "-- World axis values = " << worldValues << endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

#endif

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

#ifdef DAL_WITH_CASA
  // Test generation of various coordinates
  nofFailedTests += test_coordinates ();
#endif

  return nofFailedTests;
}
