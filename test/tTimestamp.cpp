/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2006                                                    *
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

/* Standard header files */
#include <iostream>
#include <math.h>

/* casacore header files */
#ifdef HAVE_CASA
#include <casa/OS/Time.h>
#endif

#include <Timestamp.h>

using std::endl;
using DAL::Timestamp;

/*!
  \file tTimestamp.cpp

  \ingroup DAL

  \brief A collection of test routines for the DAL::Timestamp class
 
  \author Lars B&auml;hren
 
  \date 2006/07/16
*/

//_______________________________________________________________________________
//                                                                    test_common

/*!
  \brief A few tests on some common functionality/operations

  \return nofFailedTests -- The number of failed tests within this function
*/
int test_common ()
{
  std::cout << "\n[test_common]\n" << endl;
  
  int nofFailedTests (0);

  double seconds (0.87654321);
  double fractionOfSecond = seconds-floor(seconds);

  std::cout << "-- double           = " << seconds        << std::endl;
  std::cout << "-- int(double)      = " << int(seconds)   << std::endl;
  std::cout << "-- floor(double)    = " << floor(seconds) << std::endl;
  std::cout << "-- ceil(double)     = " << ceil(seconds)  << std::endl;
  std::cout << "-- fraction(double) = " << fractionOfSecond << std::endl;

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                      test_Time

#ifdef HAVE_CASA

/*!
  \brief Test working with casa/OS/Time class

  \return nofFailedTests -- The number of failed tests within this function
*/
int test_Time ()
{
  std::cout << "\n[test_Time]\n" << endl;
  
  int nofFailedTests (0);

  try {
    casa::Time casaTime;
    std::cout << "-- Current time, GMT     = " << casaTime.toString(false) << endl;
    std::cout << "-- Current time, ISO8601 = " << casaTime.toString(true) << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

#endif

//_______________________________________________________________________________
//                                                                 test_Timestamp

/*!
  \brief Test constructors for a new Timestamp object

  \return nofFailedTests -- The number of failed tests in this function.
*/
int test_Timestamp ()
{
  std::cout << "\n[test_Timestamp]\n" << endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing default constructor ..." << endl;
  {
    Timestamp t;
    t.summary();
  }
  
  std::cout << "[2] Testing copy constructor ..." << endl;
  {
    Timestamp t;
    //
    std::cout << " - Original object:" << endl;
    t.summary();
    //
    Timestamp nb2 (t);
    //
    std::cout << " - Copied object:" << endl;
    nb2.summary();
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test working time the casacore Time class

  \return nofFailedTests -- The number of failed test encountered throughout this
          program.
*/
int test_attributes ()
{
  std::cout << "\n[test_attributes]\n" << endl;

  int nofFailedTests (0);
  Timestamp t;

  std::cout << "[1] Adjust value of month ..." << std::endl;
  try {
    for (int month(-1); month<15; month++) {
      t.setMonth(month);
      std::cout << "--> " << t.iso8601() << "\t" << t.rfc2822();
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Adjust value of day ..." << std::endl;
  try {
    for (int day(-1); day<35; day++) {
      t.setDay(day);
      std::cout << "--> " << t.iso8601() << "\t" << t.rfc2822();
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Adjust value of the hour ..." << std::endl;
  try {
    for (int hour(-1); hour<30; hour+=2) {
      t.setHour(hour);
      std::cout << "--> " << t.iso8601() << "\t" << t.rfc2822();
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[4] Adjust value of the minute ..." << std::endl;
  try {
    for (int minute(-1); minute<65; minute+=5) {
      t.setMinute(minute);
      std::cout << "--> " << t.iso8601() << "\t" << t.rfc2822();
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[5] Adjust value of the second ..." << std::endl;
  try {
    for (int second(-1); second<65; second+=5) {
      t.setSecond(second);
      std::cout << "--> " << t.iso8601() << "\t" << t.rfc2822();
    }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main function

  \return nofFailedTests -- The number of failed test encountered throughout this
          program.
*/
int main ()
{
  int nofFailedTests (0);

  // Test a few common operations which are required
  nofFailedTests += test_common();

#ifdef HAVE_CASA
  // Test working time the casacore Time class
  nofFailedTests += test_Time ();
#endif

  // Test for the constructor(s)
  nofFailedTests += test_Timestamp ();
  // Test assignment of time
  nofFailedTests += test_attributes ();
  
  return nofFailedTests;
}
