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

#include <core/ValMatrix.h>

// Namespace usage
using DAL::ValMatrix;

/*!
  \file tValMatrix.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::ValMatrix class
 
  \author Lars Baehren
 
  \date 2010/11/14
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new ValMatrix object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tValMatrix::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing ValMatrix<T>() ..." << std::endl;
  try {
    ValMatrix<int> matInt;
    matInt.summary();
    //
    ValMatrix<float> matFloat;
    matFloat.summary();
    //
    ValMatrix<double> matDouble;
    matDouble.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing ValMatrix<T>(size_t) ..." << std::endl;
  try {
    size_t nofRows = 4;
    //
    ValMatrix<int> matInt (nofRows);
    matInt.summary();
    //
    ValMatrix<float> matFloat (nofRows);
    matFloat.summary();
    //
    ValMatrix<double> matDouble (nofRows);
    matDouble.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Testing ValMatrix<T>(size_t,size_t) ..." << std::endl;
  try {
    size_t nofRows    = 4;
    size_t nofColumns = 3;
    //
    ValMatrix<int> matInt (nofRows,nofColumns);
    matInt.summary();
    //
    ValMatrix<float> matFloat (nofRows,nofColumns);
    matFloat.summary();
    //
    ValMatrix<double> matDouble (nofRows,nofColumns);
    matDouble.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[4] Testing ValMatrix<T>(size_t,size_t,T) ..." << std::endl;
  try {
    size_t nofRows    = 4;
    size_t nofColumns = 3;
    //
    ValMatrix<int> matInt (nofRows,nofColumns,1);
    matInt.summary();
    //
    ValMatrix<float> matFloat (nofRows,nofColumns,0.5);
    matFloat.summary();
    //
    ValMatrix<double> matDouble (nofRows,nofColumns,0.5);
    matDouble.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
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

  return nofFailedTests;
}
