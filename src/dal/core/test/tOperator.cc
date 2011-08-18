/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include <iomanip>
#include <core/Operator.h>

// Namespace usage
using DAL::Operator;

/*!
  \file tOperator.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::Operator class
 
  \author Lars B&auml;hren
 
  \date 2011-08-18
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new Operator object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tOperator::test_constructors]" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "\n[1] Testing Operator(Operator::Types) ..." << std::endl;
  try {
    Operator opAnd   (Operator::And);
    Operator opOr    (Operator::Or);
    Operator opXor   (Operator::Xor);
    Operator opNot   (Operator::Not);
    Operator opEqual (Operator::Equal);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "\n[2] Testing Operator(Operator) ..." << std::endl;
  try {
    Operator opAnd (Operator::And);
    //
    Operator copy (opAnd);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                          test_static_functions

/*!
  \brief Test static functions

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_functions ()
{
  std::cout << "\n[tOperator::test_static_functions]" << std::endl;

  int nofFailedTests = 0;

  std::cout << "\[1] Retrieve set with types and names ..." << std::endl;
  {
    std::set<DAL::Operator::Types> types = DAL::Operator::operatorTypes();
    std::set<std::string> names          = DAL::Operator::operatorNames();
    //
    std::cout << "-- Operator::operatorTypes = " << types << std::endl;
    std::cout << "-- Operator::operatorNames = " << names << std::endl;
  }
  
  std::cout << "\[2] Test Operator::name() ..." << std::endl;
  {
    std::set<DAL::Operator::Types> types = DAL::Operator::operatorTypes();
    std::set<DAL::Operator::Types>::iterator it;
    //
    for (it=types.begin(); it!=types.end(); ++it) {
      std::cout << std::setw(5) << *it << "  ->  " << Operator::name(*it) << std::endl;
    }
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test static functions
  nofFailedTests += test_static_functions ();

  return nofFailedTests;
}
