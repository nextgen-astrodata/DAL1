/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <Angle.h>

// Namespace usage
using DAL::Angle;

/*!
  \file tAngle.cc

  \ingroup DAL
  \ingroup coordinates

  \brief A collection of test routines for the Angle class
 
  \author Lars B&auml;hren
 
  \date 2010/05/11
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new Angle object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tAngle::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing Angle() ..." << std::endl;
  try {
    Angle angle;
    //
    angle.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing Angle(double,bool) ..." << std::endl;
  try {
    double val (1.0);
    //
    Angle angle1 (val,false);
    Angle angle2 (val,true);
    //
    angle1.summary(); 
    angle2.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Testing Angle(Angle) ..." << std::endl;
  try {
    double val (1.0);
    //
    Angle angle (val,true);
    Angle angle_copy (angle);
    //
    angle.summary(); 
    angle_copy.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_parameters

int test_parameters ()
{
  std::cout << "\n[tAngle::test_parameters]\n" << std::endl;

  int nofFailedTests (0);
  double val (DAL::PI);
  Angle angle (val,false);
  
  std::cout << "[1] Retrieve internal parameters ..." << std::endl;
  try {
    std::cout << "-- deg()       = " << angle.deg() << std::endl;
    std::cout << "-- rad()       = " << angle.rad() << std::endl;
    std::cout << "-- hms()       = " << angle.hms() << std::endl;
    std::cout << "-- className() = " << angle.className() << std::endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Set angle to new value ..." << std::endl;
  try {
    angle.setAngle (DAL::PI/2);
    angle.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                           test_staticFunctions

int test_staticFunctions ()
{
  std::cout << "\n[tAngle::test_staticFunctions]\n" << std::endl;

  int nofFailedTests (0);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test access to the internal parameters
  nofFailedTests += test_parameters ();
  // Test the public static functions
  nofFailedTests += test_staticFunctions ();

  return nofFailedTests;
}
