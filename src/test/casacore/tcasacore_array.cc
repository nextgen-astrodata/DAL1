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

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include <casa/Arrays/Array.h>
#include <casa/Arrays/ArrayIO.h>
#include <casa/Arrays/IPosition.h>

/*!
  \file tcasacore_array.cc

  \ingroup DAL1

  \brief Tests for working with the casa::Array<T> class

  \author Lars B&auml;hren

  \date 2011-07-21

*/

/* Namespace usage */
using std::cerr;
using std::cout;
using std::endl;

//_______________________________________________________________________________
//                                                                       getArray

/*!
  \brief Get casa::Array<T> of a given \e shape.
  \retval arr  -- casa::Array<T> of a given \e shape.
  \param shape -- Shape of the array.
 */
template <class T>
bool getArray (casa::Array<T> &arr,
	       std::vector<int> const &shape) 
{
  casa::IPosition ip (shape.size());
  
  for (size_t n=0; n<shape.size(); ++n) {
    ip(n) = shape[n];
  }
  
  arr.resize(ip);
  arr.set(T(1));

  return true;
}

//_______________________________________________________________________________
//                                                                      getVector

/*!
  \brief Get std::vector<T> from casa::Array<T>
  \retval data   -- 
  \param shape   -- Shape of the original data array
  \return status -- Status of the operation; return \e false in case an error was 
          encountered.
 */
template <class T>
bool getVector (std::vector<T> &data,
		std::vector<int> const &shape)
{
  bool status = true;
  casa::Array<T> buffer;

  if (getArray (buffer,shape)) {
    buffer.tovector(data);
  } else {
    status = false;
  }
  
  return status;
}

//_______________________________________________________________________________
//                                                                   test_pointer

/*!
  \brief Test conversion of of casa::Array<T> to pointer to array of values
  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
bool test_pointer ()
{
  return true;
}

//_______________________________________________________________________________
//                                                                    test_vector

/*!
  \brief Test conversion of of casa::Array<T> to std::vector<T>
  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int test_vector ()
{
  cout << "\n[tcasacore_array::test_vector]\n" << std::endl;

  int nofFailedTests = 0;
  int rank           = 2;
  std::vector<int> shape (rank,10);

  cout << "[1] Testing casa::Array<float> -> std::vector<float> ..." << std::endl;
  try {
    std::vector<float> data;
    //
    getVector (data,shape);
    //
    cout << "[ " << data[0] << " " << data[1] << " " << data[2]
	 << " "  << data[3] << " " << data[4] << " " << data[5]
	 << " .. ]" 
	 << std::endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  cout << "[2] Testing casa::Array<double> -> std::vector<double> ..." << std::endl;
  try {
    std::vector<double> data;
    //
    getVector (data,shape);
    //
    cout << "[ " << data[0] << " " << data[1] << " " << data[2]
	 << " "  << data[3] << " " << data[4] << " " << data[5]
	 << " .. ]" 
	 << std::endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
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
  int nofFailedTests = 0;

  nofFailedTests += test_vector();
  
  return nofFailedTests;
}
