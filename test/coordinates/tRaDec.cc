/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 3179 2009-10-14 13:20:10Z baehren                  $ |
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

#include <RaDec.h>

// Namespace usage
using std::endl;
using DAL::RaDec;

/*!
  \file tRaDec.cc

  \ingroup DAL
  \ingroup coordinates

  \brief A collection of test routines for the DAL::RaDec class
 
  \author Lars B&auml;hren
 
  \date 2010/05/05
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new RaDec object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tRaDec::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing RaDec() ..." << endl;
  try {
    RaDec radec;
    //
    radec.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing RaDec(double,double) ..." << endl;
  try {
    double ra (1.0);
    double dec (2.0);
    //
    RaDec radec (ra, dec);
    //
    radec.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                           test_staticFunctions

/*!
  \brief Test the public static functions

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_staticFunctions ()
{
  std::cout << "\n[tRaDec::test_staticFunctions]\n" << endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing toDegreesRA (double) ..." << endl;
  try {
    double ra;

    ra = 1.0;
    std::cout << "    " << ra << " -> " << RaDec::toDegreesRA(ra) << endl;

    ra = 2.0;
    std::cout << "    " << ra << " -> " << RaDec::toDegreesRA(ra) << endl;

    ra = 4.0;
    std::cout << "    " << ra << " -> " << RaDec::toDegreesRA(ra) << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[2] Testing toDegreesRA (int,int,double,double) ..." << endl;
  try {
    double ra;
    int hour;
    int min;
    double sec;

    ra = 1.0;
    RaDec::toDegreesRA(hour, min, sec, ra);
    std::cout << "    " << ra
	      << " -> " << hour << " : " << min << " : " << sec << endl;

    ra = 2.0;
    RaDec::toDegreesRA(hour, min, sec, ra);
    std::cout << "    " << ra
	      << " -> " << hour << " : " << min << " : " << sec << endl;

    ra = 4.0;
    RaDec::toDegreesRA(hour, min, sec, ra);
    std::cout << "    " << ra
	      << " -> " << hour << " : " << min << " : " << sec << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[3] Testing toDegreesDec (double) ..." << endl;
  try {
    double dec;

    dec = 1.0;
    std::cout << "    " << dec << " -> " << RaDec::toDegreesDec(dec) << endl;

    dec = 2.0;
    std::cout << "    " << dec << " -> " << RaDec::toDegreesDec(dec) << endl;

    dec = 4.0;
    std::cout << "    " << dec << " -> " << RaDec::toDegreesDec(dec) << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[4] Testing toDegreesDec (int,int,double,double) ..." << endl;
  try {
    double dec;
    int hour;
    int min;
    double sec;

    dec = 1.0;
    RaDec::toDegreesDec(hour, min, sec, dec);
    std::cout << "    " << dec
	      << " -> " << hour << " : " << min << " : " << sec << endl;

    dec = 2.0;
    RaDec::toDegreesDec(hour, min, sec, dec);
    std::cout << "    " << dec
	      << " -> " << hour << " : " << min << " : " << sec << endl;

    dec = 4.0;
    RaDec::toDegreesDec(hour, min, sec, dec);
    std::cout << "    " << dec
	      << " -> " << hour << " : " << min << " : " << sec << endl;
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
  // Test the public static functions
  nofFailedTests += test_staticFunctions ();

  return nofFailedTests;
}
